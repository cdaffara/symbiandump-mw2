/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Manages object search from database using SQL,*
*/

#include "mdssqlfindoperation.h"

#include "mdcresult.h"
#include "mdcitem.h"
#include "mdcserializationbuffer.h"
#include "mdsfindsequence.h"
#include "mdsobjectdef.h"
#include "mdsnamespacedef.h"
#include "mdspropertydef.h"
#include "mdsfindsqlclause.h"
#include "mdsserver.h"
#include "mdsdbconnectionpool.h"
#include "mdeinternalerror.h"

const TInt KMaxResultSize = 1024*256; // 256 KiB

CMdSSqlFindOperation* CMdSSqlFindOperation::NewL(
    CMdSFindSequence& aFind,
    TUint aSetSize
    )
    {
    CMdSSqlFindOperation* that = CMdSSqlFindOperation::NewLC(
        aFind, aSetSize );
    CleanupStack::Pop( that );
    return that;
    }

CMdSSqlFindOperation* CMdSSqlFindOperation::NewLC(    
    CMdSFindSequence& aFind,
    TUint aSetSize
    )
    {
    CMdSSqlFindOperation* that = new (ELeave) CMdSSqlFindOperation(        
        aFind, 
        aSetSize
        );
    CleanupStack::PushL( that );
    that->ConstructL();
    return that;
    }

CMdSSqlFindOperation::CMdSSqlFindOperation(    
    CMdSFindSequence& aFind,
    TUint aSetSize
    )
    : iResultMode( EQueryResultModeFirst ), iResultRow(NULL),
    iResultRows(), iResultCount( 0 ), iResults( NULL ), iFind ( aFind ),
    iState( EStateIdle ), iSetSize( aSetSize ), iLimit( 0 ), 
    iLimitCounter( 0 ), iMemoryLimit( 0 ), 
    iHarvestingPrioritizationCount( 0 )
    {
    }

void CMdSSqlFindOperation::ConstructL()
    {    
	iFindClause = CMdSFindSqlClause::NewL( const_cast<CMdsSchema&>(iFind.Schema()) );	
    }


CMdSSqlFindOperation::~CMdSSqlFindOperation()
    {
	ConsumeRows();
    iResultRows.Close();

    iResultIds.Close();

    // doesn't own result row
    iResultRow = NULL;

    iQueryId.Close();

   	delete iResults;
   	iResults = NULL;
   	delete iFindClause;
   	iFindClause = NULL;
    }

TInt CMdSSqlFindOperation::ExecuteL()
    {
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    iResultRow = &iFindClause->ResultRow();
    iQueryType = iFindClause->QueryType();
    iResultMode = iFindClause->ResultMode();

	if( EQueryResultModeFirst >= iResultMode || EQueryResultModeLast <= iResultMode )
		{
#ifdef _DEBUG
		User::Panic( _L("MdSFOExe") , KErrMdEUnknownQueryResultMode );
#endif

		User::Leave( KErrMdEUnknownQueryResultMode );
		}

   	iFind.SetResultMode( iQueryType == EQueryTypeObject && iResultMode == EQueryResultModeItem );
        
   	connection.ExecuteQueryL( iFindClause->AsTextL(), iQueryId, iFindClause->Variables() );

    iState = EStateRunning;

	iSetCounter = 0;
	iMemoryLimit = 0;
	iHarvestingPrioritizationCount = 0;

    return FetchResultsL();
    }

TInt CMdSSqlFindOperation::ContinueL()
    {
    // Continue query: fetch next row
    iSetCounter = 0;
    iState = EStateRunning;

    return FetchResultsL();
    }

TInt CMdSSqlFindOperation::FetchResultsL()
    {
	iMemoryLimit += EstimateBaseResultSize();
	
	if( !iResultRow )
		{
#ifdef _DEBUG
		User::Panic( _L("MdSFOFe1") , KErrCorrupt );
#endif

		User::Leave( KErrCorrupt );
		}

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    while( ETrue )
        {
        RRowData* resultRow = new (ELeave) RRowData();        
        CleanupStack::PushL( resultRow );
        CleanupClosePushL( *resultRow );

        resultRow->AppendColumnTypesL( *iResultRow );

        // fetch next DB row
        if ( !connection.NextRowL( iQueryId, *resultRow ) )
            {
            CleanupStack::PopAndDestroy( 2, resultRow ); // close and delete resultRow separately
            resultRow = NULL;

           	TRAPD( err, AddToContainerL() );

           	if( KErrNone != err )
           		{
           		iResults = NULL;
           		ConsumeRows();
           		}

           	connection.Terminate( iQueryId );
            iState = EStateDead;
            return KErrNone;
            }

        iMemoryLimit += EstimateResultRowSizeL( *resultRow );

		// Check if maximum memory limit is reached or 
		// set/limit counter is more than maximum set/limit size. 
		if( KMaxResultSize <= iMemoryLimit )
			{
			// Store all previus items to result buffer.

           	TRAPD( err, AddToContainerL() );

           	if( KErrNone != err )
           		{
           		iResults = NULL;
           		ConsumeRows();
           		}

			// add estimated size to next result set
	        iMemoryLimit = EstimateResultRowSizeL( *resultRow );

			if( EQueryResultModeItem == iResultMode || EQueryResultModeDistinctValues == iResultMode )
				{
				// add result row to result rows pointer array
	           	iResultRows.AppendL( resultRow );
				
				iSetCounter++;
	           	
				// pop result row from cleanup stack twice
				CleanupStack::Pop( 2, resultRow );
				}
			else
				{
				if( EQueryResultModeId == iResultMode )
					{					
					TItemId itemId = 0;
					resultRow->Column( 0 ).Get( itemId );
					iResultIds.AppendL( itemId );

					iSetCounter++;
					}
				else
					{
					TUint32 count;
					resultRow->Column( 0 ).Get( count );
					iResultCount += count;
					}

				// pop and destroy result row from cleanup stack twice
				CleanupStack::PopAndDestroy( 2, resultRow );
				}


			resultRow = NULL;

           	++iLimitCounter;
                
            iState = EStateIdle;
            connection.EnableTransaction( EFalse, iQueryId );
            return KFindSetReady;
			}
		else
			{
			if( EQueryResultModeItem == iResultMode || EQueryResultModeDistinctValues == iResultMode )
				{
				// add result row to result rows pointer array
	           	iResultRows.AppendL( resultRow );

				iSetCounter++;

				// pop result row from cleanup stack twice
				CleanupStack::Pop( 2, resultRow );
				}
			else 
				{
				if( EQueryResultModeId == iResultMode )
					{					
					TItemId itemId = 0;
					resultRow->Column( 0 ).Get( itemId );
					iResultIds.AppendL( itemId ); 

					iSetCounter++;
					}
				else
					{
					TUint32 count;
					resultRow->Column( 0 ).Get( count );
					iResultCount += count;
					}

				// pop and destroy result row from cleanup stack twice
				CleanupStack::PopAndDestroy( 2, resultRow );
				}

			resultRow = NULL;

			++iLimitCounter;
			}

        // check for different exit conditions
        if ( iState == EStateRunning )
            {
            if ( iLimitCounter >= iLimit )
                {
	           	TRAPD( err, AddToContainerL() );

	           	if( KErrNone != err )
	           		{
	           		iResults = NULL;
	           		ConsumeRows();
	           		}

	           	connection.Terminate( iQueryId );
                iState = EStateDead;
                return KErrNone;
                }
            else if( iSetCounter >= iSetSize )
                {
	           	TRAPD( err, AddToContainerL() );

	           	if( KErrNone != err )
	           		{
	           		iResults = NULL;
	           		ConsumeRows();
	           		}

                iState = EStateIdle;
                connection.EnableTransaction( EFalse, iQueryId );
                return KFindSetReady;
                }
            }
        else if ( iState == EStateStop )
            {
            // stop instructed
            connection.Terminate( iQueryId );
            iState = EStateDead;
            return KErrCancel;
            }
        else 
            {
#ifdef _DEBUG
	    	User::Panic( _L("MdSFOFe2") , KErrCorrupt );
#endif
	    	User::Leave( KErrCorrupt );
            }
        }
    }

TInt CMdSSqlFindOperation::EstimateBaseResultSize()
	{
	switch( iResultMode )
		{
		case EQueryResultModeItem:
			return sizeof(TMdCItems);

		case EQueryResultModeId:
			return sizeof(TMdCItemIds);

		case EQueryResultModeCount:
			return sizeof(TMdCItemCounts);

		case EQueryResultModeDistinctValues:
			return sizeof(TMdCItemIds);

		// should never happen
		default:
#ifdef _DEBUG
			User::Panic( _L( "MdSFOEBS" ), KErrMdEUnknownQueryResultMode );
#endif
			return 0;
		}
	}

TInt CMdSSqlFindOperation::EstimateResultRowSizeL( RRowData& aRow )
	{
	if( iResultMode == EQueryResultModeId )
		{
		return CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}
	else if( iResultMode == EQueryResultModeCount )
		{
		return 0;
		}
	else if( iResultMode == EQueryResultModeDistinctValues )
		{
		if( iFindClause->PropertyFilters().Count() != 1 )
			{
			User::Leave( KErrCorrupt );
			}

		const RPointerArray<CMdsPropertyDef>& propertyFilters = iFindClause->PropertyFilters();

		TInt rowSize = 0;
		if( propertyFilters[0]->GetType() == EPropertyText )
	    	{
	    	if ( !aRow.Column( 0 ).IsNull() )
	    		{
				// Required size for text property
				TPtrC16 value = TPtr16((TUint16*)0, 0);
				aRow.Column( 0 ).Get( value );
				rowSize += CMdCSerializationBuffer::RequiredSize( value );
	    		}	    		
	    	}
	    else
	    	{
#ifdef _DEBUG
	    	User::Panic( _L("MdSFOEs1") , KErrMdEUnknownPropertyType );
#endif
	    	User::Leave( KErrMdEUnknownPropertyType );
	    	}
	    	
	    return rowSize;
		}

	switch( iQueryType )
		{
		case EQueryTypeObject:
			{
			TInt rowSize = sizeof(TMdCObject);

			const TInt KUriColumn = 7;

			TPtrC16 uri = TPtr16((TUint16*)0, 0);
			
			// Required size for object URI
			if (!aRow.Column( KUriColumn ).IsNull())
				{
				aRow.Column( KUriColumn ).Get( uri );
				rowSize += CMdCSerializationBuffer::RequiredSize( uri );
				}

			if( iFindClause->NoObjectLocking() && iFindClause->ObjectDef() && iFindClause->ObjectDef()->GetId() != KBaseObjectDefId )
				{
				// check if object is placeholder and add it to count of harvester 
				// prioritization URIs
				const TInt KFlagsColumn = 2;
				TUint32 flags = 0;
				aRow.Column( KFlagsColumn ).Get( flags );
				if( flags & EMdEObjectFlagPlaceholder )
					{
					if( iFind.Server().ReserveSpaceHarvestingPrioritizationUri( uri ) )
						{
						iHarvestingPrioritizationCount++;
						}
					}
				}

			// Required size for properties
			CMdsObjectDef* objectDef = iFindClause->ObjectDef();

			if( !objectDef )
				{
#ifdef _DEBUG
				User::Panic( _L("MdSFOEs2") , KErrMdEUnknownObjectDef );
#endif
				User::Leave( KErrMdEUnknownObjectDef );
				}

			const RPointerArray<CMdsPropertyDef>& propertyFilters = iFindClause->PropertyFilters();

			// no property filterss
			if( propertyFilters.Count() <= 0 )
				{
				const TInt count = objectDef->GetAllPropertiesCount();

				// space for every property, even if it's "null"
				rowSize += count * sizeof(TMdCProperty);

				for( TInt i = 0; i < count; i++ )
					{
					const CMdsObjectDef::TMdsColumnOrder& propertyColumn = 
						objectDef->GetPropertyColumnL( i );

					TColumn& column = aRow.Column( propertyColumn.iColumnId );

					if( column.IsNull() )
						{
						continue;
						}

					if( propertyColumn.iPropertyDef.GetType() == EPropertyText )
				    	{
						// Required size for text in text property
						TPtrC16 value = TPtr16((TUint16*)0, 0);
						column.Get( value );
						rowSize += CMdCSerializationBuffer::RequiredSize( value );
				    	}
					}
				}
			// property filters
			else
				{
				const TInt count = propertyFilters.Count();

				// space for every property, even if it's "null"
				rowSize += count * sizeof(TMdCProperty);

				for( TInt i = 0; i < count; i++ )
					{
					CMdsPropertyDef* propDef = propertyFilters[i];

					if( !propDef )
						{
#ifdef _DEBUG
						User::Panic( _L("MdSFOEs3") , KErrMdEUnknownPropertyDef );
#endif						
						User::Leave( KErrMdEUnknownPropertyDef );
						}

					TColumn& column = aRow.Column( KBaseObjectBasicValueColumnOffset + i );

					if( column.IsNull() )
						{
						continue;
						}

					if( propDef->GetType() == EPropertyText )
				    	{
						// Required size for text in text property
						TPtrC16 value = TPtr16((TUint16*)0, 0);
						column.Get( value );
						rowSize += CMdCSerializationBuffer::RequiredSize( value );
				    	}
					}
				}

			if( FindCriteria().IncludesFreetexts() )
				{
				// In result row the second lastest column is free text count
				const TInt freeTextCountColumn = aRow.Size() - 2;

				// Get free text count
				TUint32 freeTextCount = 0;
				aRow.Column( freeTextCountColumn ).Get( freeTextCount );

				if( freeTextCount > 0 )
					{
					// In result row last column is total free text length
					const TInt totalFreeTextsLengthColumn = aRow.Size() - 1;

					// Get total free text length.
					TUint32 totalFreeTextsLength = 0;
					aRow.Column( totalFreeTextsLengthColumn ).Get( totalFreeTextsLength );

					// Required size for free texts.
					// For every free text length (TUint16) 
					rowSize += freeTextCount * ( CMdCSerializationBuffer::KRequiredSizeForEmptyText );
					// and total free text length * TUint16
					rowSize += totalFreeTextsLength * sizeof( TUint16 );
					}
				}

			return rowSize;
			}

		case EQueryTypeRelation:
			return sizeof(TMdCRelation);

		case EQueryTypeEvent:
			{
			TInt rowSize = sizeof(TMdCEvent);

			// Required size for source and participant texts
			TPtrC16 source = TPtr16((TUint16*)0, 0);
			aRow.Column( 4 ).Get( source );
			rowSize += CMdCSerializationBuffer::RequiredSize( source );

			TPtrC16 participant = TPtr16((TUint16*)0, 0);
			aRow.Column( 5 ).Get( participant );
			rowSize += CMdCSerializationBuffer::RequiredSize( participant );

			return rowSize;
			}

		default:
#ifdef _DEBUG
			User::Panic( _L( "MdSFOEs4" ), KErrMdEUnknownQueryResultMode );
#endif
			return 0;
		}
	}

TInt CMdSSqlFindOperation::State()
    {
    return iState;
    }

void CMdSSqlFindOperation::Cancel()
    {
    if ( iState == EStateRunning )
        {
        iState = EStateStop;
        }
    }

RPointerArray<HBufC>& CMdSSqlFindOperation::QueryFreeText()
	{
	return iFindClause->QueryFreeText();
	}


CMdSFindSqlClause& CMdSSqlFindOperation::FindCriteria()
    {
    return *iFindClause;
    }

CMdCSerializationBuffer* CMdSSqlFindOperation::Results()
	{
	CMdCSerializationBuffer* results = iResults;
	iResults = NULL;
	return results;
	}

void CMdSSqlFindOperation::SetLimit(TUint32 aLimit)
	{
	iLimit = aLimit;
	}

void CMdSSqlFindOperation::SetOffset(TUint32 aOffset)
	{
	iOffset = aOffset;
	}

TMdCOffset CMdSSqlFindOperation::AddItemToContainerL( RRowData &aRow, TMdCOffset aFreespaceOffset )
    {
    switch( iQueryType )  
        {
        case EQueryTypeObject:
            {
            aFreespaceOffset = AddObjectToContainerL( aRow, aFreespaceOffset );
            break;
            }
        case EQueryTypeEvent:
            {
            TMdCEvent event;
            TPtrC16 source = TPtr16( ( TUint16* )0, 0 );      //KNullPtr16;
            TPtrC16 participant = TPtr16( ( TUint16* )0, 0 ); //KNullPtr16;

            aRow.Column( 0 ).Get( event.iId );
            aRow.Column( 1 ).Get( event.iObjectId );
            aRow.Column( 2 ).Get( event.iDefId );
            aRow.Column( 3 ).Get( event.iTime );
            aRow.Column( 4 ).Get( source );
            aRow.Column( 5 ).Get( participant );

            const TMdCOffset eventOffset = iResults->Position();
            
            if ( source.Length() > 0 )
            	{
            	event.iSourceText.iPtr.iCount = source.Length();
            	event.iSourceText.iPtr.iOffset = aFreespaceOffset;
            	iResults->PositionL( aFreespaceOffset );
            	aFreespaceOffset = iResults->InsertL( source );
            	}
            else
            	{
            	event.iSourceText.iPtr.iCount = 0;
            	event.iSourceText.iPtr.iOffset = KNoOffset;
            	}

            if ( participant.Length() > 0 )
            	{
            	event.iParticipantText.iPtr.iCount = participant.Length();
            	event.iParticipantText.iPtr.iOffset = aFreespaceOffset;
            	iResults->PositionL( aFreespaceOffset );
                aFreespaceOffset = iResults->InsertL( participant );
            	}
            else
            	{
            	event.iParticipantText.iPtr.iCount = 0;
            	event.iParticipantText.iPtr.iOffset = KNoOffset;
            	}
            
            iResults->PositionL( eventOffset );
            event.SerializeL( *iResults );
            break;
            }
        case EQueryTypeRelation:
            {
            TMdCRelation relation;
            TUint32 flags           ( 0 );

            aRow.Column( 0 ).Get( relation.iId );
            aRow.Column( 1 ).Get( flags );
            aRow.Column( 2 ).Get( relation.iDefId );
            aRow.Column( 3 ).Get( relation.iLeftObjectId );
            aRow.Column( 4 ).Get( relation.iRightObjectId );
            aRow.Column( 5 ).Get( relation.iParameter );
            aRow.Column( 6 ).Get( relation.iGuidHigh );
            aRow.Column( 7 ).Get( relation.iGuidLow );
            aRow.Column( 8 ).Get( relation.iLastModifiedDate );

            relation.SerializeL( *iResults );
            break;
            }
        // Unknown query type
        default:
            {
#ifdef _DEBUG
			User::Panic( _L("MdSFOAd1") , KErrMdEUnknownQueryType );
#endif
            User::Leave( KErrMdEUnknownQueryType );
            }
        }
    return aFreespaceOffset;
    }

void CMdSSqlFindOperation::AddIdToContainerL( TItemId aId )
    {
    switch( iQueryType )  
        {
        case EQueryTypeObject:
        case EQueryTypeEvent:
        case EQueryTypeRelation:
            {
            iResults->InsertL( aId );
            break;
            }
        default:
            {
#ifdef _DEBUG
			User::Panic( _L("MdSFOAd2") , KErrMdEUnknownQueryType );
#endif
            User::Leave( KErrMdEUnknownQueryType );
            }
        }
    }

void CMdSSqlFindOperation::CreateItemL()
    {
    TMdCItems items;
    items.iNamespaceDefId = iFindClause->NamespaceDef()->GetId();
    items.iObjects.iPtr.iCount = 0;
    items.iObjects.iPtr.iOffset = KNoOffset;
    items.iRelations.iPtr.iCount = 0;
    items.iRelations.iPtr.iOffset = KNoOffset;
    items.iEvents.iPtr.iCount = 0;
    items.iEvents.iPtr.iOffset = KNoOffset;

    // move after main header
    iResults->PositionL( sizeof(TMdCItems) );

    TUint32 count = iResultRows.Count();
    TMdCOffset freetextOffset = KNoOffset;
    switch( iQueryType )
        {
        case EQueryTypeObject:
            {
            // add objects header
            items.iObjects.iPtr.iCount = count;
            items.iObjects.iPtr.iOffset = iResults->Position();
            freetextOffset = items.iObjects.iPtr.iOffset + count * sizeof(TMdCObject);
            
            // initializate server's harvesting prioritization buffer
        	if( iHarvestingPrioritizationCount > 0 )
        		{
        		iFind.Server().StartAddingHarvestingPrioritizationUrisL();
        		}		

            break;
            }
        case EQueryTypeEvent:
            {
            // add objects header
            items.iEvents.iPtr.iCount = count;
            items.iEvents.iPtr.iOffset = iResults->Position();
            freetextOffset = items.iEvents.iPtr.iOffset + count * sizeof(TMdCEvent);
            break;
            }
        case EQueryTypeRelation:
            {
            // add objects header
            items.iRelations.iPtr.iCount = count;
            items.iRelations.iPtr.iOffset = iResults->Position();
            freetextOffset = items.iRelations.iPtr.iOffset + count * sizeof(TMdCRelation);
            break;
            }
        default:
            {
#ifdef _DEBUG
			User::Panic( _L("MdSFOCr1") , KErrMdEUnknownQueryType );
#endif
			User::Leave( KErrMdEUnknownQueryType );
            }
        }

  	iResults->PositionL( KNoOffset );
	items.SerializeL( *iResults );

    //Set offset to objects/events/relations
    for( TInt i = 0; i < count; i++ )
        {
        switch ( iQueryType )
        	{
			case EQueryTypeObject:
	        	{
	            iResults->PositionL( items.iObjects.iPtr.iOffset
	            		+ i * sizeof(TMdCObject) );
	            freetextOffset = AddItemToContainerL( *iResultRows[i], freetextOffset );
	            break;
	            }
	        case EQueryTypeEvent:
	        	{
	            iResults->PositionL( items.iEvents.iPtr.iOffset
	            		+ i * sizeof(TMdCEvent) );
	            freetextOffset = AddItemToContainerL( *iResultRows[i], freetextOffset );
	            break;
	            }
	        case EQueryTypeRelation:
	        	{
	            iResults->PositionL( items.iRelations.iPtr.iOffset
	            		+ i * sizeof(TMdCRelation) );
	            freetextOffset = AddItemToContainerL( *iResultRows[i], freetextOffset );
			    break;
	            }
	        default:
	        	{
#ifdef _DEBUG
	        	User::Panic( _L("MdSFOCr2") , KErrMdEUnknownQueryType );
#endif
	        	User::Leave( KErrMdEUnknownQueryType );
	        	}
        	}
        }
    }

void CMdSSqlFindOperation::CreateCountL()
    {
    TMdCItemCounts itemCounts;
    itemCounts.iNamespaceDefId = iFindClause->NamespaceDef()->GetId();
    itemCounts.iObjects = 0;
    itemCounts.iEvents = 0;
    itemCounts.iRelations = 0;

    switch( iQueryType )
        {
        case EQueryTypeObject:
            {
            itemCounts.iObjects = iResultCount;
            break;
            }
        case EQueryTypeEvent:
            {
            itemCounts.iEvents = iResultCount;
            break;
            }
        case EQueryTypeRelation:
            {
            itemCounts.iRelations = iResultCount;
            break;
            }
        default:
            {
#ifdef _DEBUG
			User::Panic( _L("MdSFOCr3") , KErrMdEUnknownQueryType );
#endif
			User::Leave( KErrMdEUnknownQueryType );
            }
        }

    itemCounts.SerializeL( *iResults );
    }

void CMdSSqlFindOperation::CreateIdL()
    {
    TMdCItemIds itemIds;
    itemIds.iNamespaceDefId = iFindClause->NamespaceDef()->GetId();
    itemIds.iErrorCode = KErrNone;
    itemIds.iObjectIds.iPtr.iCount = 0;
    itemIds.iObjectIds.iPtr.iOffset = KNoOffset;
    itemIds.iObjectUris.iPtr.iCount = 0;
    itemIds.iObjectUris.iPtr.iOffset = KNoOffset;
    itemIds.iEventIds.iPtr.iCount = 0;
    itemIds.iEventIds.iPtr.iOffset = KNoOffset;
    itemIds.iRelationIds.iPtr.iCount = 0;
    itemIds.iRelationIds.iPtr.iOffset = KNoOffset;

    iResults->PositionL( sizeof(TMdCItemIds) );

    TUint32 count = iResultIds.Count();
    switch( iQueryType )  
        {
        case EQueryTypeObject:
            {
            itemIds.iObjectIds.iPtr.iCount = count;
            itemIds.iObjectIds.iPtr.iOffset = iResults->Position();
            break;
            }
        case EQueryTypeEvent:
            {
            itemIds.iEventIds.iPtr.iCount = count;
            itemIds.iEventIds.iPtr.iOffset = iResults->Position();
            break;
            }
        case EQueryTypeRelation:
            {
            itemIds.iRelationIds.iPtr.iCount = count;
            itemIds.iRelationIds.iPtr.iOffset = iResults->Position();
            break;
            }
        default:
            {
#ifdef _DEBUG
			User::Panic( _L("MdSFOCr4") , KErrMdEUnknownQueryType );
#endif
			User::Leave( KErrMdEUnknownQueryType );
            }
        }

	iResults->PositionL( KNoOffset );
	itemIds.SerializeL( *iResults );

    for( TInt i = 0; i < count; i++ )
        {
	    AddIdToContainerL( iResultIds[i] );
        }
    }

void CMdSSqlFindOperation::CreateDistinctL()
	{
    TMdCItemIds itemIds;
    itemIds.iNamespaceDefId = iFindClause->NamespaceDef()->GetId();
    itemIds.iErrorCode = KErrNone;
    itemIds.iObjectIds.iPtr.iCount = 0;
    itemIds.iObjectIds.iPtr.iOffset = KNoOffset;
    itemIds.iEventIds.iPtr.iCount = 0;
    itemIds.iEventIds.iPtr.iOffset = KNoOffset;
    itemIds.iRelationIds.iPtr.iCount = 0;
    itemIds.iRelationIds.iPtr.iOffset = KNoOffset;

    itemIds.iObjectUris.iPtr.iCount = 0;
    itemIds.iObjectUris.iPtr.iOffset = sizeof(TMdCItemIds);
    
    iResults->PositionL( itemIds.iObjectUris.iPtr.iOffset );
    const TInt resultRowsCount = iResultRows.Count();
	for ( TInt i = 0; i < resultRowsCount; ++i )
		{		
		if ( !iResultRows[i]->Column( 0 ).IsNull() )
			{
			TPtrC16 value = TPtr16((TUint16*)0, 0);
			iResultRows[i]->Column( 0 ).Get( value );
			iResults->InsertL( value );
			++itemIds.iObjectUris.iPtr.iCount;
			}
		}

    iResults->PositionL( KNoOffset );
    itemIds.SerializeL( *iResults );
	}

void CMdSSqlFindOperation::AddToContainerL()
    {
    // if old result buffer exist try to use it
    if( iResults )
    	{
    	// create new if the old result buffer is too short
    	if( iResults->Size() < iMemoryLimit )
    		{
    		delete iResults;
    		iResults = NULL;

    		iResults = CMdCSerializationBuffer::NewL( iMemoryLimit );
    		}
    	// else just move to the begin of the result buffer
    	else
    		{
    		iResults->PositionL( 0 );
    		}
    	}
	else
		{
		iResults = CMdCSerializationBuffer::NewL( iMemoryLimit );
		}

    TInt err;
    switch( iResultMode )  
        {
        case EQueryResultModeCount:
            {
            TRAP( err, CreateCountL() );
            break;
            }
        case EQueryResultModeItem:
            {
            TRAP( err, CreateItemL() );
            break;
            }
        case EQueryResultModeId:
            {
            TRAP( err, CreateIdL() );
          break;
            }
        case EQueryResultModeDistinctValues:
            {
            TRAP( err, CreateDistinctL() );
            break;
            }
        default:
            {
#ifdef _DEBUG
			User::Panic( _L("MdSFOAd3") , KErrMdEUnknownQueryResultMode );
#endif
			User::Leave( KErrMdEUnknownQueryResultMode );
            }
        }
 
	if( err != KErrNone )
		{
		delete iResults;
		iResults = NULL;
		}

   	ConsumeRows();
    }

TMdCOffset CMdSSqlFindOperation::AddObjectToContainerL( RRowData& aRow, TMdCOffset aFreespaceOffset )
    {
    TMdCObject object;
    const TMdCOffset objectOffset = iResults->Position();
    object.iFlags = EMdEObjectFlagNone;

	// Get base objects basic values
    TUint32 serverSideFlags;
    TPtrC16 uri;
    aRow.Column( 0 ).Get( object.iId );
    aRow.Column( 1 ).Get( object.iDefId );
    aRow.Column( 2 ).Get( serverSideFlags );
    aRow.Column( 3 ).Get( object.iMediaId );
    aRow.Column( 4 ).Get( object.iUsageCount );
    aRow.Column( 5 ).Get( object.iGuidHigh );
    aRow.Column( 6 ).Get( object.iGuidLow );
    if (aRow.Column( 7 ).IsNull())
    	{
    	object.iId = KNoId;
    	object.iUri.iPtr.iCount = 0;
    	object.iUri.iPtr.iOffset = KNoOffset;
    	object.SerializeL( *iResults );
    	return aFreespaceOffset;
    	}
    aRow.Column( 7 ).Get( uri );

	// SETTING CLIENT SIDE FLAGS
	// Set not present flag to client side flags 
	// (removed objects are also flaged as not present)
	if( ( serverSideFlags & EMdEObjectFlagNotPresent ) ||
		( serverSideFlags & EMdEObjectFlagRemoved ) )
		{
		object.iFlags |= EMdEObjectFlagNotPresent;
		}

	// Set confidential flag to client side flags
	if( serverSideFlags & EMdEObjectFlagConfidential )
		{
		object.iFlags |= EMdEObjectFlagConfidential;
		}

	// Set freetext flag to client side flags
	if( serverSideFlags & EMdEObjectFlagFreetexts )
		{
		object.iFlags |= EMdEObjectFlagFreetexts;
		}

	// Set placeholder flag to client side flags
	if( serverSideFlags & EMdEObjectFlagPlaceholder )
		{
		object.iFlags |= EMdEObjectFlagPlaceholder;
		
		// object is placeholder, so try add it's URI to harverting 
		// prioritization buffer
		if( iHarvestingPrioritizationCount > 0 )
			{
			iFind.Server().AddHarvestingPrioritizationUriL( uri );

			iHarvestingPrioritizationCount--;

			if( iHarvestingPrioritizationCount == 0 )
				{
				iFind.Server().NotifyHarvestingPrioritizationObserver( KErrNone );
				}
			}
		}
	
	const CMdsObjectDef* objectDef = iFindClause->ObjectDef();
	if( !objectDef )
		{
#ifdef _DEBUG
		User::Panic( _L("MdSFOAd4") , KErrMdEUnknownObjectDef );
#endif		
		User::Leave( KErrMdEUnknownObjectDef );
		}

    // Add base objects basic values to result buffer
    object.iUri.iPtr.iCount = uri.Length();
    object.iUri.iPtr.iOffset = aFreespaceOffset;
    iResults->PositionL( aFreespaceOffset );
    aFreespaceOffset = iResults->InsertL( uri );

	const RPointerArray<CMdsPropertyDef>& propertyFilters = iFindClause->PropertyFilters();

	// No property filters
	if( propertyFilters.Count() <= 0 )
		{
		const TInt allPropertyCount = objectDef->GetAllPropertiesCount();
		object.iProperties.iPtr.iCount = 0;
		object.iProperties.iPtr.iOffset = aFreespaceOffset;

		// Position after property offsets
		aFreespaceOffset += allPropertyCount * sizeof(TMdCProperty);
		
		for( TInt i = 0; i < allPropertyCount; i++ )
			{
			const CMdsObjectDef::TMdsColumnOrder& propertyColumn = 
				objectDef->GetPropertyColumnL( i );

			TColumn& column = aRow.Column( propertyColumn.iColumnId );

			// check if property exists, else continue to with next property
			if( column.IsNull() )
				{
				continue;
				}

			TMdCProperty property;

			property.iPropertyDefId = propertyColumn.iPropertyDef.GetId();
			property.iModFlags = EMdEPropertyModNone;

			const TPropertyType propertyType = propertyColumn.iPropertyDef.GetType();
			switch(propertyType)
				{
				case EPropertyBool:
					{
					TBool value;
					column.Get( value );
					property.iValue.iInt32 = value;
					}
    				break;
    			case EPropertyInt8:
    				{
					TInt32 value32;
					column.Get( value32 );
					property.iValue.iInt32 = value32;
    				}
    				break;
    			case EPropertyUint8:
    				{
					TUint32 value32;
					column.Get( value32 );
					property.iValue.iUint32 = value32;
    				}
    				break;
    			case EPropertyInt16:
    				{
					TInt32 value32;
					column.Get( value32 );
					property.iValue.iInt32 = value32;
    				}
    				break;
    			case EPropertyUint16:
    				{
					TUint32 value32;
					column.Get( value32 );
					property.iValue.iUint32 = value32;
    				}
    				break;
    			case EPropertyInt32:
    				{
					TInt32 value;
					column.Get( value );
					property.iValue.iInt32 = value;
    				}
    				break;
    			case EPropertyUint32:
    				{
					TUint32 value;
					column.Get( value );
					property.iValue.iUint32 = value;
    				}
    				break;
				case EPropertyInt64:
    				{
					TInt64 value;
					column.Get( value );
					property.iValue.iInt64 = value;
    				}
    				break;    				
    			case EPropertyReal32:
    				{
					TReal32 value;
					column.Get( value );
					property.iValue.iReal = value;
    				}
    				break;
    			case EPropertyReal64:
    				{
					TReal64 value;
					column.Get( value );
					property.iValue.iReal = value;
    				}
    				break;
    			case EPropertyTime:
    				{
					TTime value;
					column.Get( value );
					property.iValue.iInt64 = value.Int64();
    				}
    				break;
    			case EPropertyText:
    				{    				
	    			TPtrC16 value;
					column.Get( value );
					if (value.Length() > 0)
						{
						property.iValue.iPtr.iCount = value.Length();
						property.iValue.iPtr.iOffset = aFreespaceOffset;
						iResults->PositionL( aFreespaceOffset );
						aFreespaceOffset = iResults->InsertL( value );
						}
					else
						{
						property.iValue.iPtr.iCount = 0;
						property.iValue.iPtr.iOffset = KNoOffset;
						}
    				}
    				break;
    			default:
#ifdef _DEBUG
					User::Panic( _L("MdSFOAd5") , KErrMdEUnknownPropertyType );
#endif
					User::Leave( KErrMdEUnknownPropertyType );
				}

		    // store property in buffer
			iResults->PositionL( object.iProperties.iPtr.iOffset
					+ object.iProperties.iPtr.iCount * sizeof(TMdCProperty) );
			++object.iProperties.iPtr.iCount;
		    property.SerializeL( *iResults );
			}
		}
	// Property filters
	else
		{
		const TUint32 propertyCount = propertyFilters.Count();
		object.iProperties.iPtr.iCount = 0;
		object.iProperties.iPtr.iOffset = aFreespaceOffset;

		// Position after property offsets
		aFreespaceOffset += propertyCount * sizeof(TMdCProperty);

		for( TInt i = 0; i < propertyCount; i++ )
			{
			CMdsPropertyDef* propDef = propertyFilters[i];

			if( !propDef )
				{
#ifdef _DEBUG
				User::Panic( _L("MdSFOAd6") , KErrMdEUnknownPropertyDef );
#endif
				User::Leave( KErrMdEUnknownPropertyDef );
				}

			TColumn& column = aRow.Column( KBaseObjectBasicValueColumnOffset + i );

			// check if property exists, else continue to with next property
			if( column.IsNull() )
				{
				continue;
				}

			TMdCProperty property;

			property.iPropertyDefId = propDef->GetId();
			property.iModFlags = EMdEPropertyModNone;

			const TPropertyType propertyType = propDef->GetType();
			switch(propertyType)
				{
				case EPropertyBool:
					{
					TBool value;
					column.Get( value );
					property.iValue.iInt32 = value;
					}
					break;
				case EPropertyInt8:
					{
					TInt32 value32;
					column.Get( value32 );
					property.iValue.iInt32 = value32;
					}
					break;
				case EPropertyUint8:
					{
					TUint32 value32;
					column.Get( value32 );
					property.iValue.iUint32 = value32;
					}
					break;
				case EPropertyInt16:
					{
					TInt32 value32;
					column.Get( value32 );
					property.iValue.iInt32 = value32;
					}
					break;
				case EPropertyUint16:
					{
					TUint32 value32;
					column.Get( value32 );
					property.iValue.iUint32 = value32;
					}
					break;
				case EPropertyInt32:
					{
					TInt32 value;
					column.Get( value );
					property.iValue.iInt32 = value;
					}
					break;
				case EPropertyUint32:
					{
					TUint32 value;
					column.Get( value );
					property.iValue.iUint32 = value;
					}
					break;
				case EPropertyInt64:
					{
					TInt64 value;
					column.Get( value );
					property.iValue.iInt64 = value;
					}
					break;    				
				case EPropertyReal32:
					{
					TReal32 value;
					column.Get( value );
					property.iValue.iReal = value;
					}
					break;
				case EPropertyReal64:
					{
					TReal64 value;
					column.Get( value );
					property.iValue.iReal = value;
					}
					break;
				case EPropertyTime:
					{
					TTime value;
					column.Get( value );
					property.iValue.iReal = value.Int64();
					}
					break;
				case EPropertyText:
					{    				
	    			TPtrC16 value;
					column.Get( value );
					if (value.Length() > 0)
						{
						property.iValue.iPtr.iCount = value.Length();
						property.iValue.iPtr.iOffset = aFreespaceOffset;
						iResults->PositionL( aFreespaceOffset );
						aFreespaceOffset = iResults->InsertL( value );
						}
					else
						{
						property.iValue.iPtr.iCount = 0;
						property.iValue.iPtr.iOffset = KNoOffset;
						}
					}
					break;
				default:
#ifdef _DEBUG
					User::Panic( _L("MdSFOAd7") , KErrMdEUnknownPropertyType );
#endif
					User::Leave( KErrMdEUnknownPropertyType );
				}

		    // store property in buffer
			iResults->PositionL( object.iProperties.iPtr.iOffset
					+ object.iProperties.iPtr.iCount * sizeof(TMdCProperty) );
			++object.iProperties.iPtr.iCount;
		    property.SerializeL( *iResults );
			}
		}

	if( FindCriteria().IncludesFreetexts() )
		{		
		// In result row the second lastest column is free text count
		const TInt freeTextCountColumn = aRow.Size() - 2;

		// Get free text count and total free text lenght.
		TUint32 freeTextCount = 0;
		aRow.Column( freeTextCountColumn ).Get( freeTextCount );

		if( freeTextCount > 0 )
			{
			object.iFreeTexts.iPtr.iCount = freeTextCount;
			object.iFreeTexts.iPtr.iOffset = aFreespaceOffset;
			
			// In result row last column is total free text lenght.
			const TInt totalFreeTextsLengthColumn = aRow.Size() - 1;

			TUint32 totalFreeTextsLength = 0;
			aRow.Column( totalFreeTextsLengthColumn ).Get( totalFreeTextsLength );

			// Reserve space for free texts.
			// For every free text possible padding (TUint8), length (TUint16)
			TUint32 freeTextReserve = freeTextCount * CMdCSerializationBuffer::KRequiredSizeForEmptyText;
			// and total free text length * TUint16
			freeTextReserve += totalFreeTextsLength * sizeof( TUint16 );

			aFreespaceOffset += freeTextReserve;
			}
		else
			{
			object.iFreeTexts.iPtr.iCount = 0;
			object.iFreeTexts.iPtr.iOffset = KNoOffset;
			}
		}
	else
		{
		object.iFreeTexts.iPtr.iCount = 0;
		object.iFreeTexts.iPtr.iOffset = KNoOffset;
		}

	iResults->PositionL( objectOffset );
    object.SerializeL( *iResults );

	return aFreespaceOffset;
    }

void CMdSSqlFindOperation::ConsumeRows()
    {
	if( EQueryResultModeId != iResultMode )
		{    
	    for( TInt i = iResultRows.Count() - 1; i >=0; i-- )
	    	{
	    	iResultRows[i]->Close();
	    	}

	    iResultRows.ResetAndDestroy();
		}
	// handle result mode IDs as special case
	else
		{
		iResultIds.Reset();
		}
	}
