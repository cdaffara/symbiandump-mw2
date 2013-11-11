/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class constructs SQL search clauses for MdS.*
*/

// INCLUDE FILES
#include <mdequery.h>
#include "mdsfindsqlclause.h"
#include "mdsfindsqlclausedef.h"
#include "mdsclausebuffer.h"
#include "mdcserializationbuffer.h"
#include "mdsquerycriteriadeserialization.h"
#include "mdsobjectdef.h"
#include "mderange.h"
#include "mdsschema.h"
#include "mdsnamespacedef.h"
#include "mdspropertydef.h"
#include "mdcquery.h"
#include "mdeinternalerror.h"

const TInt KQueryBufInitialSize = 512;

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CMdSFindSqlClause* CMdSFindSqlClause::NewL(CMdsSchema& aSchema)
    {
    CMdSFindSqlClause* self = NewLC(aSchema);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CMdSFindSqlClause* CMdSFindSqlClause::NewLC(CMdsSchema& aSchema)
    {
    CMdSFindSqlClause* self = new ( ELeave ) CMdSFindSqlClause(aSchema);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// ~CMdSSqlClause
// ---------------------------------------------------------------------------
//
CMdSFindSqlClause::~CMdSFindSqlClause()
    {
    delete iQueryBuf;
    iQueryBuf = NULL;

    iResultRow.Close();
    iVariables.Close();

    // doesn't own property definitions
	iPropertyFilters.Close();
	
	iFreeText.ResetAndDestroy();
	iFreeText.Close();
	
	if( iSourceObjectDefs )
		{
		iSourceObjectDefs->Close();

		delete iSourceObjectDefs;
		iSourceObjectDefs = NULL;
		}
    }

// ---------------------------------------------------------------------------
// CMdSSqlClause
// ---------------------------------------------------------------------------
//
CMdSFindSqlClause::CMdSFindSqlClause(CMdsSchema& aSchema) : 
    iVariables(), iQueryBuf( NULL ), iSchema( aSchema ), 
    iSearchCriteria( NULL ), iSerializedBuffer( NULL ), 
    iUserLevel( EUserLevelNone ), iNamespaceDef( NULL ), iObjectDef( NULL ), 
    iSourceObjectDef ( NULL ), iSourceObjectDefs( NULL ), 
    iAppendToResultRow( ETrue ), iIncludeFreetexts( ETrue ), 
    iIncludeNotPresent( EFalse ), iNoObjectLocking( ETrue ), iPlaceholdersOnly( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::ConstructL()
    {
    // Construct empty buffer for sql query
    iQueryBuf = CMdsClauseBuffer::NewL( KQueryBufInitialSize );
    iQueryBuf->AppendL( KEmpty );
    }

// ---------------------------------------------------------------------------
// AsText           Returns sql clause
// ---------------------------------------------------------------------------
//
const TDesC& CMdSFindSqlClause::AsTextL() const
    {
    return iQueryBuf->ConstBufferL();
    }

// ---------------------------------------------------------------------------
// Variables        Query variables
// ---------------------------------------------------------------------------
//
RRowData& CMdSFindSqlClause::Variables()
    {
    return iVariables;
    }

// ---------------------------------------------------------------------------
// Variables        Query result row
// ---------------------------------------------------------------------------
//
RRowData& CMdSFindSqlClause::ResultRow()
    {
    return iResultRow;
    }

// ---------------------------------------------------------------------------
// Limit        Query row limit
// ---------------------------------------------------------------------------
//
TUint32 CMdSFindSqlClause::Limit()
	{
	return iSearchCriteria->iLimit;
	}
	
TBool CMdSFindSqlClause::IncludesFreetexts()
	{
	return iIncludeFreetexts;
	}

TQueryType CMdSFindSqlClause::QueryType()
	{
	return (TQueryType)iSearchCriteria->iQueryType;
	}

TQueryResultMode CMdSFindSqlClause::ResultMode()
	{
	return (TQueryResultMode)iSearchCriteria->iQueryResultType;
	}

RPointerArray<HBufC>& CMdSFindSqlClause::QueryFreeText()
	{
	return iFreeText;
	}

TBool CMdSFindSqlClause::NoObjectLocking()
	{
	return iNoObjectLocking;
	}

// ---------------------------------------------------------------------------
// ObjectDef        Query object definition
// ---------------------------------------------------------------------------
//
CMdsObjectDef* CMdSFindSqlClause::ObjectDef()
	{
	return iObjectDef;
	}

RPointerArray<CMdsPropertyDef>& CMdSFindSqlClause::PropertyFilters()
	{
	return iPropertyFilters;
	}

// ---------------------------------------------------------------------------
// NamespaceDef        Query namespace definition
// ---------------------------------------------------------------------------
//
CMdsNamespaceDef* CMdSFindSqlClause::NamespaceDef()
	{
	return iNamespaceDef;
	}

// ---------------------------------------------------------------------------
// CreateL          Creates whole sql clause
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::CreateL( 
	CMdCSerializationBuffer& aSerializedCriteria, 
	TUserLevel aUserLevel )
    {
    iUserLevel = aUserLevel;

    iSerializedBuffer = &aSerializedCriteria;

    iSearchCriteria = CONST_CAST( TMdCSearchCriteria*, 
    		&TMdCSearchCriteria::GetFromBufferL( aSerializedCriteria ) );

	// can freetexts drop from results
	if( ( iSearchCriteria->iOptimizationFlags & EContainsFreetextCondition ) == 0 )
		{
		iIncludeFreetexts = EFalse;
		}

	if( iSearchCriteria->iOptimizationFlags & EContainsNotPresentCondition )
		{
		iIncludeNotPresent = ETrue;
		}

	if( iSearchCriteria->iOptimizationFlags & EContainsObjectLocking )
		{
		iNoObjectLocking = EFalse;
		}
	
	if( iSearchCriteria->iOptimizationFlags & EContainsPlaceholdersOnly )
        {
        iPlaceholdersOnly = ETrue;
        }
	

	// get namespace def from new schema definition
	iNamespaceDef = const_cast<CMdsNamespaceDef*>(iSchema.GetNamespaceByIdL( 
			iSearchCriteria->iNamespaceDefId ));

	if ( iNamespaceDef )
		{
		iNamespaceIdDes.Num( iNamespaceDef->GetId() );
		}
	else
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}

	iObjectDef = NULL;

	if( iSearchCriteria->iObjectDefId != KNoDefId )
		{
		// get object def from new schema definition
		iObjectDef = CONST_CAST( CMdsObjectDef*, 
				iNamespaceDef->GetObjectByIdL( iSearchCriteria->iObjectDefId ) );

		if ( !iObjectDef )
			{
			User::Leave( KErrMdEUnknownObjectDef );
			}
		}

	iSourceObjectDef = NULL;
	iSourceObjectDefs = NULL;

	if ( iSearchCriteria->iObjectDefIds.iPtr.iCount > 0 )
		{
		iSourceObjectDefs = new (ELeave) RArray<TDefId>( 
				iSearchCriteria->iObjectDefIds.iPtr.iCount );
		iSourceObjectDefs->ReserveL( iSearchCriteria->iObjectDefIds.iPtr.iCount );

		iSerializedBuffer->PositionL( iSearchCriteria->iObjectDefIds.iPtr.iOffset );

		for( TUint32 i = 0; i < iSearchCriteria->iObjectDefIds.iPtr.iCount; i++ )
			{
			TDefId objectDefId;
			iSerializedBuffer->ReceiveL( objectDefId );			
			iSourceObjectDefs->AppendL( objectDefId );
			}
		}
	else
		{
		iSourceObjectDef = iObjectDef;
		}

	if( !iSourceObjectDefs )
		{
		// Construct beginning of the select clause 
		// Format: SELECT [column list/count(*)] FROM table name
		AppendSelectStatementL();

		AppendWhereStatementL();

		// if query type is object query and freetext are included
		// "GROUP BY BO.ObjectId"
		if( iIncludeFreetexts )
			{
		    AppendGroupByL();
			}
		}
	else
		{
		const TInt sourceObjectDefsCount = iSourceObjectDefs->Count();
		for( TInt i = 0; i < sourceObjectDefsCount; i++ )
			{
			const TDefId sourceObjectDefId = (*iSourceObjectDefs)[i];
			iSourceObjectDef = CONST_CAST( CMdsObjectDef*, 
					iNamespaceDef->GetObjectByIdL( sourceObjectDefId ) );

			if( i > 0 )
				{
				iAppendToResultRow = EFalse;
				iQueryBuf->AppendL( KUnionAll );
				}

			// Construct beginning of the select clause 
			// Format: SELECT [column list/count(*)] FROM table name
			AppendSelectStatementL();
			
			AppendWhereStatementL();
			    
			// if query type is object query and freetext are included
			// "GROUP BY BO.ObjectId"
			if( iIncludeFreetexts )
				{
			    AppendGroupByL();
				}
			}
		}

    // ORDER BY clause (if such exists)
    AppendOrderByL();

    // LIMIT and OFFSET clauses (if such exists)
    AppendLimitAndOffsetL();

    // SQL clause must end to semicolon
    iQueryBuf->AppendL( KSemicolon );
    }

// ---------------------------------------------------------------------------
// AppendPropertyFiltersL       Appends the property filters
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendPropertyFiltersL( TBool aDistinct )
	{
	if( iSearchCriteria->iPropertyFilters.iPtr.iCount > 0 )
		{
		iSerializedBuffer->PositionL( 
				iSearchCriteria->iPropertyFilters.iPtr.iOffset );

		for ( TUint32 i = 0; i < iSearchCriteria->iPropertyFilters.iPtr.iCount; ++i )
			{
			TDefId id;
			iSerializedBuffer->ReceiveL( id );

			if ( !aDistinct )
				{
				iQueryBuf->AppendL( KComma );
				}

			AppendColumnByPropertyDefIdL( id, KNoDefId, ETrue );
			}

		iQueryBuf->AppendL( KSpace );
		}
	}

// ---------------------------------------------------------------------------
// AppendSelectStatementL       Appends the SELECT statements
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectStatementL()
    {
    switch( iSearchCriteria->iQueryType )
    	{
    	case EQueryTypeObject:
    		AppendSelectForObjectL();
    		AppendFromForObjectL();
	    	break;
	    case EQueryTypeRelation:
	    	AppendSelectForRelationL();
	    	AppendFromForRelationL();
	    	break;
        case EQueryTypeEvent:
	    	AppendSelectForEventL();
  	 		AppendFromForEventL();
	    	break;
	    default:
#ifdef _DEBUG
            User::Panic( _L("MdSFCASS") , KErrMdEUnknownQueryType );
#endif
	    	User::Leave( KErrMdEUnknownQueryType );
    	}
    }

// ---------------------------------------------------------------------------
// AppendSelectCountStatementL        SELECT statements for count query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectCountStatementL()
    {
    // if result rows needs to be grouped return just "1"
    if( iIncludeFreetexts )
		{
		// "SELECT 1 "
		iQueryBuf->AppendL( KSelect1 );
		}
	else
		{
		// "SELECT count(*) "
    	iQueryBuf->AppendL( KSelectCount );
		}

	// expected result count
    if( iAppendToResultRow )
    	{
    	iResultRow.AppendL( TColumn( EColumnUint32 ) );
    	}
    }

// ---------------------------------------------------------------------------
// AppendSelectDistinctStatementL   SELECT statements for distinct value query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectDistinctStatementL()
	{
	// "SELECT DISTINCT "
	iQueryBuf->AppendL( KSelectDistinct );

	const TUint32 position = iSerializedBuffer->Position();

	// "column name" (from property filter)
	AppendPropertyFiltersL( ETrue );

	iSerializedBuffer->PositionL( position );
	}

// ---------------------------------------------------------------------------
// AppendSelectObjectIdStatementL        SELECT statements for object id query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectObjectIdStatementL()
	{
	// "SELECT BO.ObjectId "
	iQueryBuf->AppendL( KSelectObjectIds );

	// expected result object ID
	if( iAppendToResultRow )
		{
		iResultRow.AppendL( TColumn( EColumnTItemId ) );
		}
	}

// ---------------------------------------------------------------------------
// AppendSelectEventAllStatementL        SELECT statements for event query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectAllFromEventStatementL()
	{
	// "SELECT * "
	iQueryBuf->AppendL( KSelectAllFrom );

	if( iAppendToResultRow )
		{
		// expected result Events 
	    iResultRow.AppendL( TColumn( EColumnTItemId ) );
	    iResultRow.AppendL( TColumn( EColumnTItemId ) );
	    iResultRow.AppendL( TColumn( EColumnTDefId ) );
	    iResultRow.AppendL( TColumn( EColumnTime ) );
	    iResultRow.AppendL( TColumn( EColumnDes16 ) );
	    iResultRow.AppendL( TColumn( EColumnDes16 ) );
		}
	}

// ---------------------------------------------------------------------------
// AppendSelectRelationAllStatementL        SELECT statements for Relaltion query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectAllFromRelationStatementL()
	{
	// "SELECT * "
	iQueryBuf->AppendL( KSelectAllFrom );

	if( iAppendToResultRow )
		{
		// expected result Relation
	    iResultRow.AppendL( TColumn( EColumnTItemId ) );
	    iResultRow.AppendL( TColumn( EColumnUint32 ) );
	    iResultRow.AppendL( TColumn( EColumnTDefId ) );
	    iResultRow.AppendL( TColumn( EColumnTItemId ) );
	    iResultRow.AppendL( TColumn( EColumnTItemId ) );
	    iResultRow.AppendL( TColumn( EColumnInt32 ) );
	    iResultRow.AppendL( TColumn( EColumnInt64 ) );
	    iResultRow.AppendL( TColumn( EColumnInt64 ) );
	    iResultRow.AppendL( TColumn( EColumnTime ) );
		}
	}

// ---------------------------------------------------------------------------
// AppendSelectEventItemStatementL        SELECT statements for event id query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectEventItemStatementL()
    {
	// "SELECT Event + NamespaceId.EventId "
	iQueryBuf->AppendL( KSelectEventIds );
	iQueryBuf->AppendL( iNamespaceIdDes );
	iQueryBuf->AppendL( KDotEventId );

	if( iAppendToResultRow )
		{
		// expected result event
	    iResultRow.AppendL( TColumn( EColumnTItemId ) );
		}
    }

// ---------------------------------------------------------------------------
// AppendSelectRealtionItemStatementL        SELECT statements for Realtion id query
// ---------------------------------------------------------------------------
//AppendSelectRelationItemStatementL
void CMdSFindSqlClause::AppendSelectRealtionItemStatementL()
    {
	 //"SELECT Relation.EventId "
	iQueryBuf->AppendL( KSelectRelationIds );
    iQueryBuf->AppendL( iNamespaceIdDes );
    iQueryBuf->AppendL( KDotRelationId );
	
	if( iAppendToResultRow )
		{
		//expected result relation
		iResultRow.AppendL( TColumn( EColumnTItemId ) );
		}
    }

// ---------------------------------------------------------------------------
// AppendSelectObjectItemStatementL    SELECT statements for object item query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectObjectItemStatementL()
	{
	if( iAppendToResultRow )
		{
		// add expected fixed "base" object's columns
		iResultRow.AppendL( TColumn( EColumnTItemId ) ); // object ID
		iResultRow.AppendL( TColumn( EColumnTDefId ) ); // object definition ID
		iResultRow.AppendL( TColumn( EColumnUint32 ) ); // flags
		iResultRow.AppendL( TColumn( EColumnUint32 ) ); // media ID
		iResultRow.AppendL( TColumn( EColumnUint32 ) ); // UsageCount
		iResultRow.AppendL( TColumn( EColumnInt64 ) );  // GUID high
		iResultRow.AppendL( TColumn( EColumnInt64 ) );  // GUID low
		iResultRow.AppendL( TColumn( EColumnDes16 ) );  // URI
		}

	if( iSearchCriteria->iPropertyFilters.iPtr.iCount > 0 )
		{
		// "SELECT BO...."
		iQueryBuf->AppendL( KSelectPropertyFilterBegin );

		const TUint32 position = iSerializedBuffer->Position();

		AppendPropertyFiltersL();

		iSerializedBuffer->PositionL( position );
		}
	else
		{
		// No property filters so get all properties
		
	    if( !iObjectDef )
	        {
	        User::Leave( KErrMdEUnknownObjectDef );
	        }
	
		// "SELECT BO.* "
		iQueryBuf->AppendL( KSelectAllFromBaseObject );
	
		const TInt propCount = iObjectDef->GetAllPropertiesCount();
		TBool addBaseObjectId = EFalse;
		
		if( KBaseObjectDefId != iObjectDef->GetId() )
			{
			if( !iSourceObjectDefs )
				{
				// ",O.* "
				iQueryBuf->AppendL( KAllFromObject );
				}
			else
				{
				// ",O.ObjectId"
				iQueryBuf->AppendL( KComma );
				iQueryBuf->AppendL( KObjectObjectId );

				// no filters, so add all property names
				const TInt lookupObjectPropertyCount = 
					iObjectDef->GetPropertiesCount();
				
				for( TInt i = 0; i < lookupObjectPropertyCount; i++)
					{
					iQueryBuf->AppendL( KComma );
					const CMdsPropertyDef* propDef = iObjectDef->GetProperty( i );
					iQueryBuf->AppendL( propDef->GetName() );
					}
				iQueryBuf->AppendL( KSpace );
				}
			addBaseObjectId = ETrue;
			}

		if (propCount > 0)
			{
			// add base object properties, object ID and the rest properties
			for( TUint32 i = 0; i < propCount; i++ )
				{
				const CMdsPropertyDef& propDef = iObjectDef->GetPropertyColumnL( i ).iPropertyDef;

				// add column for object ID when object definition is not "base" object
				if( addBaseObjectId && KBaseObjectBasicValueColumnOffset < propDef.GetId() )
					{
					if( iAppendToResultRow )
						{
			    	    // add expected column for object ID
						iResultRow.AppendL( TColumn( EColumnTItemId ) ); // object ID
						}
					addBaseObjectId = EFalse;
					}

				if( iAppendToResultRow )
					{
					// add expected columns	from object
					iResultRow.AppendL( TColumn( propDef.GetSqlType() ) );
					}
				}
				
			if( addBaseObjectId )
				{
				if( iAppendToResultRow )
					{
					// add expected column for object ID
					iResultRow.AppendL( TColumn( EColumnTItemId ) ); // object ID
					}
				}
			}
		}

	if( iIncludeFreetexts )
		{
		// ",count(D.Word),sum(length(D.Word)) "
		iQueryBuf->AppendL( KSelectFreeTextCountAndTotalLength );

		if( iAppendToResultRow )
			{
			iResultRow.AppendL( TColumn( EColumnUint32 ) );
			iResultRow.AppendL( TColumn( EColumnUint32 ) );
			}
		}
	}

// ---------------------------------------------------------------------------
// AppendFromForObjectL           FROM statements for object query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendFromForObjectL()
	{
	// "FROM BaseObject" + namespace def id + " AS BO "
	iQueryBuf->AppendL( KFromBaseObject );
	iQueryBuf->AppendL( iNamespaceIdDes );
	iQueryBuf->AppendL( KAsBaseObject );

	// if object definitions is BaseObject
	if( iSourceObjectDef && KBaseObjectDefId != iSourceObjectDef->GetId() )
		{
		// "," + object def's table + namespace def id + " AS O ON BO.ObjectId=O.ObjectId "
        iQueryBuf->AppendL( KComma );

		AppendTableByObjectDefIdL( iSourceObjectDef->GetId() );

        iQueryBuf->AppendL( KAsObjectOnEqual );
		}

	if( iIncludeFreetexts )
		{
		// Append free text join
		// "LEFT JOIN TextSearchDictionary" + namespace def id + " 
		// AS D ON D.WordId IN(SELECT WordId FROM TextSearch" + 
		// namespace def id + " WHERE BO.ObjectId=ObjectId) "
		iQueryBuf->AppendL( KFreeTextLeftJoinBegin );
		iQueryBuf->AppendL( iNamespaceIdDes );
		iQueryBuf->AppendL( KFreeTextLeftJoinMiddle );
		iQueryBuf->AppendL( iNamespaceIdDes );
		iQueryBuf->AppendL( KFreeTextLeftJoinEnd );
		}
	}


// ---------------------------------------------------------------------------
// AppendFromForObjectL           FROM statements for Event query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendFromForEventL()
    {
   	iQueryBuf->AppendL( KFromEvent );
	iQueryBuf->AppendL( iNamespaceIdDes );
   	iQueryBuf->AppendL( KSpace );
    }

// ---------------------------------------------------------------------------
// AppendFromForRelationL           FROM statements for Relation query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendFromForRelationL()
    {
   	iQueryBuf->AppendL( KFromRealtions );
	iQueryBuf->AppendL( iNamespaceIdDes );
	iQueryBuf->AppendL( KSpace );
    }


// ---------------------------------------------------------------------------
// AppendWhereStatementL           WHERE statements for query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendWhereStatementL()
	{
	// move position to begin of the root condition
	iSerializedBuffer->PositionL( iSearchCriteria->iRootCondition );

	const TMdCLogicCondition& condition = TMdCLogicCondition::GetFromBufferL( 
			*iSerializedBuffer );

	if( EConditionTypeLogic == condition.iConditionType )
		{
		// if root condition's child count is more than zero add where statement
		if( condition.iChildConditions.iPtr.iCount > 0 )
			{
			// "WHERE("root condition")"
			iQueryBuf->AppendL( KWhere );

			if( EQueryTypeObject == iSearchCriteria->iQueryType )
				{
				// remove "deleted" objects from result
				iQueryBuf->AppendL( KGetNotFlagAnd );
				iVariables.AppendL( TColumn( EMdEObjectFlagRemoved ) );

				if( EUserLevelDeviceAccess != iUserLevel )
					{
					if( iIncludeNotPresent )
						{
						// If client application doesn't have device capability limit to
						// only non-confidential (confidential flag is not set)
						// "(NOT Flags&?)AND "
						iQueryBuf->AppendL( KGetNotFlagAnd );
						iVariables.AppendL( TColumn( EMdEObjectFlagConfidential ) );
						}
					else
						{
						// If client application doesn't have device capability limit to
						// only non-confidential (confidential flag is not set)
						// and not present objects
						// "(NOT Flags&?)AND "
						iQueryBuf->AppendL( KGetNotFlagAnd );
						iVariables.AppendL( TColumn( EMdEObjectFlagConfidential | EMdEObjectFlagNotPresent ) );
						}
					}
				else
					{
					if( !iIncludeNotPresent )
						{
						// Drop not present objects
						// "(NOT Flags&?)AND "
						iQueryBuf->AppendL( KGetNotFlagAnd );
						iVariables.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
						}
					}
				}

			if ( EQueryTypeRelation == iSearchCriteria->iQueryType )
				{
				// add "(NOT Flags&?)AND " with not present flag
				iQueryBuf->AppendL( KGetNotFlagAnd );
				iVariables.AppendL( TColumn( EMdERelationFlagNotPresent ) );
				// add "(NOT Flags&?)AND " with deleted flag
				iQueryBuf->AppendL( KGetNotFlagAnd );
				iVariables.AppendL( TColumn( EMdERelationFlagDeleted ) );
				}

			iQueryBuf->AppendL( KLeftBracket );

			// move position to begin of the root condition
			iSerializedBuffer->PositionL( iSearchCriteria->iRootCondition );

			ConditionStatementL();

			iQueryBuf->AppendL( KRightBracket );
			}
		else if( EQueryTypeObject == iSearchCriteria->iQueryType )
			{
			// remove "deleted" objects from result
			iQueryBuf->AppendL( KWhereNotFlag );
			iVariables.AppendL( TColumn( EMdEObjectFlagRemoved ) );

			if( EUserLevelDeviceAccess != iUserLevel )
				{
				if( iIncludeNotPresent )
					{
					// If client application doesn't have device capability limit to
					// only non-confidential (confidential flag is not set)
					// "WHERE(NOT Flags&?)"
					iQueryBuf->AppendL( KAnd );
					iQueryBuf->AppendL( KGetNotFlag );
					iVariables.AppendL( TColumn( EMdEObjectFlagConfidential ) );
					}
				else
					{
					// If client application doesn't have device capability limit to
					// only non-confidential (confidential flag is not set)
					// and not present objects
					// "WHERE(NOT Flags&?)"
					iQueryBuf->AppendL( KAnd );
					iQueryBuf->AppendL( KGetNotFlag );
					iVariables.AppendL( TColumn( EMdEObjectFlagConfidential | EMdEObjectFlagNotPresent ) );
					}
				}
			else
				{
				if( !iIncludeNotPresent )
					{
					// Drop not present objects
					// "WHERE(NOT Flags&?)"
					iQueryBuf->AppendL( KAnd );
					iQueryBuf->AppendL( KGetNotFlag );
					iVariables.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
					}
				}
			}
		else if ( EQueryTypeRelation == iSearchCriteria->iQueryType )
			{
			// add "WHERE(NOT Flags&?)" with not present flag
			iQueryBuf->AppendL( KWhereNotFlag );
			iVariables.AppendL( TColumn( EMdERelationFlagNotPresent ) );
			// add "AND (NOT Flags&?)" with deleted flag
			iQueryBuf->AppendL( KAnd );
			iQueryBuf->AppendL( KGetNotFlag );
			iVariables.AppendL( TColumn( EMdERelationFlagDeleted ) );
			}
		}
	else
		{
		User::Leave( KErrMdEUnknownConditionType );
		}
	}

// ---------------------------------------------------------------------------
// AppendSelectForObjectGroupL      SELECT statements for object query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectForObjectL()
    {
    switch( iSearchCriteria->iQueryResultType )
	    {
	    case EQueryResultModeCount:
	        // Format: SELECT count(*) FROM
	        AppendSelectCountStatementL();
	        break;
	    case EQueryResultModeDistinctValues:
	        // Format: SELECT "wanted property's column" FROM
	        AppendSelectDistinctStatementL();
	        break;
	    case EQueryResultModeItem:
	    	AppendSelectObjectItemStatementL();
	    	break;
	    case EQueryResultModeId:
	    	AppendSelectObjectIdStatementL();
	    	break;

	    default:
	    	{
#ifdef _DEBUG
            User::Panic( _L("MdSFCSeO") , KErrMdEUnknownQueryResultMode );
#endif
	    	User::Leave( KErrMdEUnknownQueryResultMode );
	    	}
	    }
    }

// ---------------------------------------------------------------------------
// AppendSelectForRelationGroupL        SELECT statements for relation query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectForRelationL()
    {
    switch( iSearchCriteria->iQueryResultType )
        {
	    case EQueryResultModeCount:
	        // Format: SELECT count(*) FROM
	        AppendSelectCountStatementL();
	        break;
	    case EQueryResultModeItem:
	    	// Format: SELECT * FROM   //  SELECT * FROM Relation
	    	AppendSelectAllFromRelationStatementL();
	    	break;
	    case EQueryResultModeId:
	    	// SELECT Relation.RelationId FROM // SELECT 
	    	AppendSelectRealtionItemStatementL();
	    	break;

	    default:
	    	{
#ifdef _DEBUG
            User::Panic( _L("MdSFCSeR") , KErrMdEUnknownQueryResultMode );
#endif
	    	User::Leave( KErrMdEUnknownQueryResultMode );
	    	}
        }
    }

// ---------------------------------------------------------------------------
// AppendSelectForEventL()       SELECT statements for event query
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendSelectForEventL()
    {
    switch( iSearchCriteria->iQueryResultType )
        {
	    case EQueryResultModeCount:
	        // Format: SELECT count(*) FROM
	        AppendSelectCountStatementL();
	        break;
	    case EQueryResultModeItem:
	    	// Format: SELECT * FROM   //  SELECT * FROM Event
	    	AppendSelectAllFromEventStatementL();
	    	break;
	    case EQueryResultModeId:
	    	// SELECT EventDef.EventId FROM // SELECT EventDef.EventDefId FROM Event
	    	AppendSelectEventItemStatementL();
	    	break;

	    default:
	    	{
#ifdef _DEBUG
            User::Panic( _L("MdSFCSeE") , KErrMdEUnknownQueryResultMode );
#endif	    	
	    	User::Leave( KErrMdEUnknownQueryResultMode );
	    	}
        }
    }

// ---------------------------------------------------------------------------
// ConditionStatementL       Recursive parsing of condition statements
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::ConditionStatementL(TBool aNegated, TAppendIn aAppendIdIn)
    {
    const TMdCCondition& condition = TMdCCondition::GetFromBufferL( *iSerializedBuffer );

    TBool negated = condition.iNegated;

	if( aNegated )
		{
		negated = !negated;
		}

	const TConditionType conditionType = (TConditionType)condition.iConditionType;

	if( aAppendIdIn != EAppendInFalse )
		{
		if ( EConditionTypeLogic != conditionType )
			{
			if ( aAppendIdIn == EAppendInTrue )
				{
				AppendIdInL();
				}
			else if (aAppendIdIn == EAppendInRelationLeft)
				{
				iQueryBuf->AppendL( KRelationLeftObjectIdIn );
				}
			else if (aAppendIdIn == EAppendInRelationRight)
				{
				iQueryBuf->AppendL( KRelationRightObjectIdIn );
				}
			else
				{
#ifdef _DEBUG
				User::Panic( _L("MdSFCCo1") , KErrCorrupt );
#endif
				User::Leave( KErrCorrupt );
				}
			}

		// Add recursive child criteria surrounded by "("child")".
		iQueryBuf->AppendL( KLeftBracket );
		}

	switch( conditionType )
		{
		case EConditionTypeLogic:
			AppendLogicConditionL( negated, aAppendIdIn );
			break;
		case EConditionTypeObject:
			AppendObjectConditionL( negated );
			break;
		case EConditionTypeProperty:
			AppendPropertyConditionL( negated );
			break;
		case EConditionTypePropertyIntRange:
		case EConditionTypePropertyInt64Range:
		case EConditionTypePropertyUintRange:
		case EConditionTypePropertyRealRange:
		case EConditionTypePropertyTimeRange:
			AppendPropertyRangeConditionL( negated );
			break;
		case EConditionTypePropertyText:
			AppendPropertyTextConditionL( negated );
			break;
		case EConditionTypePropertyBool:
			AppendPropertyBoolConditionL( negated );
			break;
		case EConditionTypeRelation:
			AppendRelationConditionL( negated );
			break;
		case EConditionTypeEvent:
			AppendEventConditionL( negated );
			break;
		default: // Unknown condition type
#ifdef _DEBUG
            User::Panic( _L("MdSFCCo2") , KErrMdEUnknownConditionType );
#endif
			User::Leave( KErrMdEUnknownConditionType );
		}

	if( aAppendIdIn )
		{		
		iQueryBuf->AppendL( KRightBracket );
		}
    }

// ---------------------------------------------------------------------------
// AppendObjectId        Help method for appending object ID clause.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendObjectIdL()
	{
	if( KBaseObjectDefId == iObjectDef->GetId() )
		{
		// "BO.ObjectId"
		iQueryBuf->AppendL( KBaseObjectObjectId );
		}
	else
		{
		// "O.ObjectId"
		iQueryBuf->AppendL( KObjectObjectId );
		}
	}

// ---------------------------------------------------------------------------
// AppendIdIn                    			Help method for appending object, 
//											event or relation ID IN clause.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendIdInL()
	{
	switch( iSearchCriteria->iQueryType )
		{
		case EQueryTypeObject:
			// "BO.ObjectId" or "O.ObjectId"
			AppendObjectIdL();
			break;
		case EQueryTypeRelation:
			iQueryBuf->AppendL( KRelationId );
			break;
		case EQueryTypeEvent:
			iQueryBuf->AppendL( KEventId );
			break;
		default:
#ifdef _DEBUG
            User::Panic( _L("MdSFCAII") , KErrMdEUnknownQueryType );
#endif
			User::Leave( KErrMdEUnknownQueryType );
		}

	// " IN"
	iQueryBuf->AppendL( KIn );
	}


// ---------------------------------------------------------------------------
// AppendLogicConditionL        Help method for appending logic condition.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendLogicConditionL(TBool aNegated, TAppendIn aAppendIdIn)
	{
	const TMdCLogicCondition& condition = TMdCLogicCondition::GetFromBufferL( 
			*iSerializedBuffer );

    // Add recursive all children.
    for( TUint32 i = 0; i < condition.iChildConditions.iPtr.iCount; i++)
		{
		// get offset and move position to begin of child condition
		iSerializedBuffer->PositionL( condition.iChildConditions.iPtr.iOffset + 
				CMdCSerializationBuffer::KRequiredSizeForTUint32 * i );

		TUint32 childOffset;
		iSerializedBuffer->ReceiveL( childOffset );

		iSerializedBuffer->PositionL( childOffset );

		// If not first add operator
		if( i > 0 )
			{
			switch( condition.iOperator )
				{
				case ELogicConditionOperatorAnd:
					iQueryBuf->AppendL( KAnd );
					break;
				case ELogicConditionOperatorOr:
					iQueryBuf->AppendL( KOr );
					break;
				default:
#ifdef _DEBUG
					User::Panic( _L("MdSFCALC") , KErrMdEUnknownConditionCompareMethod );
#endif
					User::Leave( KErrMdEUnknownConditionCompareMethod ); 
				}
			}

		ConditionStatementL( aNegated, aAppendIdIn );
		}
	}

// ---------------------------------------------------------------------------
// AppendObjectConditionL        Help method for appending object condition.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendObjectConditionL(TBool aNegated)
	{
	const TMdCObjectCondition& condition = TMdCObjectCondition::GetFromBufferL( 
			*iSerializedBuffer );

	// "SELECT ObjectId FROM BaseObject"namespace def ID" WHERE(...)"
	iQueryBuf->AppendL( KSelectObjectIdFromBaseObject );
	iQueryBuf->AppendL( iNamespaceIdDes );
	iQueryBuf->AppendL( KSpace );
	iQueryBuf->AppendL( KWhere );

	iQueryBuf->AppendL( KLeftBracket );

	// remove "deleted" objects from result
	iQueryBuf->AppendL( KGetNotFlagAnd );
	iVariables.AppendL( TColumn( EMdEObjectFlagRemoved ) );
	
   if( iPlaceholdersOnly )
        {
        // Drop non-placeholder  objects
        // "(Flags&?)AND "
        iQueryBuf->AppendL( KGetFlagAnd );
        iVariables.AppendL( TColumn( EMdEObjectFlagPlaceholder ) );
        }

	if( EUserLevelDeviceAccess == iUserLevel )
		{
		switch( condition.iConfidentialityLevel )
			{
			// Only non-confidential (confidential flag is not set)
			// "(NOT Flags&?)AND "
			case EObjectConditionLevelNormal:
				iQueryBuf->AppendL( KGetNotFlagAnd );
				iVariables.AppendL( TColumn( EMdEObjectFlagConfidential ) );
				break;
			// Only confidential (confidential flag is set)
			// "(Flags&?)AND "
			case EObjectConditionLevelConfidential:
				iQueryBuf->AppendL( KGetFlagAnd );
				iVariables.AppendL( TColumn( EMdEObjectFlagConfidential ) );
				break;

			case EObjectConditionLevelIgnoreConfidentiality:
				// no implementation needed
				break;

		    default:
		    	{
#ifdef _DEBUG
				User::Panic( _L("MdSFCAO1") , KErrMdEUnknownConfidentialityLevel );
#endif
		    	User::Leave( KErrMdEUnknownConfidentialityLevel );
		    	}
			}
		}
	else
		{
		// If client application doesn't have device capability limit to
		// only non-confidential (confidential flag is not set)
		// "(NOT Flags&?)AND "
		iQueryBuf->AppendL( KGetNotFlagAnd );
		iVariables.AppendL( TColumn( EMdEObjectFlagConfidential ) );
		}

	if( ( condition.iFlags & EMdEObjectFlagNotPresent ) == EFalse )
		{
		// Not present flag is not set
		// "NOT(Flags&?)AND "
		iQueryBuf->AppendL( KGetNotFlagAnd );
		iVariables.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
		}

	if( ( condition.iFlags & EMdEObjectFlagPlaceholder ) )
		{
		// Placeholder flag is not set
		// "NOT(Flags&?)AND "
		iQueryBuf->AppendL( KGetNotFlagAnd );
		iVariables.AppendL( TColumn( EMdEObjectFlagPlaceholder ) );
		}

	if( aNegated )
		{
		// "NOT "
		iQueryBuf->AppendL( KNotWithSpace );
		}

	// move position to begin of compare condition
	iSerializedBuffer->PositionL( condition.iCondition );

	switch( condition.iCompareMethod )
		{
		case EObjectConditionCompareNone:
			break;
		case EObjectConditionCompareId:
			{
			TItemId objectId;
			iSerializedBuffer->ReceiveL( objectId );

			iQueryBuf->AppendL( KObjectIdEqual );
			iVariables.AppendL( TColumn( objectId ) );
			}
			break;
		case EObjectConditionCompareIds:
			{
			TInt32 objectIdCount;
			iSerializedBuffer->ReceiveL( objectIdCount );

			if( objectIdCount == 1 )
				{
				TItemId objectId;
				iSerializedBuffer->ReceiveL( objectId );

				iQueryBuf->AppendL( KObjectIdEqual );
				iVariables.AppendL( TColumn( objectId ) );
				}
			else if( objectIdCount > 1 )
				{
				iQueryBuf->AppendL( KObjectIdIn );
				iQueryBuf->AppendL( KLeftBracket );

				TItemId objectId;
				iSerializedBuffer->ReceiveL( objectId );

				iQueryBuf->AppendL( KVariable );
				iVariables.AppendL( TColumn( objectId ) );

				for( TInt32 i = 1; i < objectIdCount; i++ )
					{
					iSerializedBuffer->ReceiveL( objectId );

					iQueryBuf->AppendL( KCommaVariable );
					iVariables.AppendL( TColumn( objectId ) );
					}

				iQueryBuf->AppendL( KRightBracket );
				}
			}
			break;
		case EObjectConditionCompareGuid:
			{
			TInt64 guidHigh;
			iSerializedBuffer->ReceiveL( guidHigh );			
			TInt64 guidLow;
			iSerializedBuffer->ReceiveL( guidLow );			

			iQueryBuf->AppendL( KObjectGuidEqual );
			iVariables.AppendL( TColumn( guidHigh ) );
			iVariables.AppendL( TColumn( guidLow ) );
			}
			break;
		case EObjectConditionCompareObjectDef:
			{
			TDefId objectDefId;
			iSerializedBuffer->ReceiveL( objectDefId );

			iQueryBuf->AppendL( KObjectDefIdEqual );
			iVariables.AppendL( TColumn( objectDefId ) );
			}
			break;
		case EObjectConditionCompareUsageCount:
			{
			iQueryBuf->AppendL( KObjectUsageCount );
			AppendRangeL( EUint32RangeValue );
			}
			break;
		case EObjectConditionCompareUri:
			{
			TPtrC16 uriPtr = iSerializedBuffer->ReceivePtr16L();

			iQueryBuf->AppendL( KObjectUriEqual );
			iVariables.AppendL( TColumn( uriPtr ) );
			}
			break;
		case EObjectConditionCompareUriBeginsWith:
			{
			TPtrC16 uriPtr = iSerializedBuffer->ReceivePtr16L();

			HBufC16* pattern = HBufC16::NewLC( uriPtr.Length() + 1 );  // prepare for uri%

	        TPtr16 modPattern = pattern->Des();

			// add wildcard after URI "uri%"
			modPattern.Append( uriPtr );
			modPattern.Append( KWildcard );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KObjectUriBeginsWith );			
			}
			break;
		 case EObjectConditionCompareUriContains:
            {
            TPtrC16 uriPtr = iSerializedBuffer->ReceivePtr16L();

            HBufC16* pattern = HBufC16::NewLC( uriPtr.Length() + 2 );  // prepare for %uri%

            TPtr16 modPattern = pattern->Des();

            // add wildcard before and after URI "%uri%"
            modPattern.Append( KWildcard );
            modPattern.Append( uriPtr );
            modPattern.Append( KWildcard );

            iVariables.AppendL( TColumn( pattern ) );

            CleanupStack::Pop( pattern );

            iQueryBuf->AppendL( KObjectUriBeginsWith );         
            }
            break;
		case EObjectConditionCompareFreeText:
			{
			TPtrC16 freetextPtr = iSerializedBuffer->ReceivePtr16L();

			HBufC* freetext = freetextPtr.AllocLC();
			iFreeText.AppendL( freetext );
			CleanupStack::Pop( freetext );

			iQueryBuf->AppendL( KObjectFreeTextSearchStart );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchWhere );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchDictionaryWhere );
			iQueryBuf->AppendL( KEqual );
			iQueryBuf->AppendL( KObjectFreeTextSearchEnd );
			iVariables.AppendL( TColumn( freetextPtr ) );
			}
			break;
		case EObjectConditionCompareFreeTextContains:
			{
			TPtrC16 freetextPtr = iSerializedBuffer->ReceivePtr16L();

			HBufC* freetext = freetextPtr.AllocLC();
			iFreeText.AppendL( freetext );
			CleanupStack::Pop( freetext );

			iQueryBuf->AppendL( KObjectFreeTextSearchStart );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchWhere );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchDictionaryWhere );
			
			HBufC16* pattern = HBufC16::NewLC( freetextPtr.Length() + 2 );  // prepare for %freetext%

	        TPtr16 modPattern = pattern->Des();

			// add wildcard before and after freetext "%freetext%"
			modPattern.Append( KWildcard );
			modPattern.Append( freetextPtr );
			modPattern.Append( KWildcard );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KLike );

			iQueryBuf->AppendL( KObjectFreeTextSearchEnd );
			}
			break;
		case EObjectConditionCompareFreeTextBeginsWith:
			{
			TPtrC16 freetextPtr = iSerializedBuffer->ReceivePtr16L();

			HBufC* freetext = freetextPtr.AllocLC();
			iFreeText.AppendL( freetext );
			CleanupStack::Pop( freetext );

			iQueryBuf->AppendL( KObjectFreeTextSearchStart );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchWhere );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchDictionaryWhere );

			HBufC16* pattern = HBufC16::NewLC( freetextPtr.Length() + 1 );  // prepare for freetext%

	        TPtr16 modPattern = pattern->Des();

			// add wildcard after freetext "freetext%"
			modPattern.Append( freetextPtr );
			modPattern.Append( KWildcard );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KLike );

			iQueryBuf->AppendL( KObjectFreeTextSearchEnd );
			}
			break;
		case EObjectConditionCompareFreeTextEndsWith:
			{
			TPtrC16 freetextPtr = iSerializedBuffer->ReceivePtr16L();

			HBufC* freetext = freetextPtr.AllocLC();
			iFreeText.AppendL( freetext );
			CleanupStack::Pop( freetext );

			iQueryBuf->AppendL( KObjectFreeTextSearchStart );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchWhere );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KObjectFreeTextSearchDictionaryWhere );

			HBufC16* pattern = HBufC16::NewLC( freetextPtr.Length() + 1 );  // prepare for %freetext

	        TPtr16 modPattern = pattern->Des();

			// add wildcard before freetext "%freetext"
			modPattern.Append( KWildcard );
			modPattern.Append( freetextPtr );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KLike );

			iQueryBuf->AppendL( KObjectFreeTextSearchEnd );
			}
			break;
		default:
#ifdef _DEBUG
			User::Panic( _L("MdSFCAO2") , KErrMdEUnknownConditionCompareMethod );
#endif
			User::Leave( KErrMdEUnknownConditionCompareMethod );
		}

	iQueryBuf->AppendL( KRightBracket );
	}

// ---------------------------------------------------------------------------
// AppendRangeL                    Help method for appending range.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendRangeL(TRangeValueType aRangeValueType)
	{
	TInt32 rangeTypeValue;
	iSerializedBuffer->ReceiveL( rangeTypeValue );
	const TMdERangeType rangeType = (TMdERangeType)rangeTypeValue;

	TMdCValueUnion minValue;
	iSerializedBuffer->ReceiveL( minValue );

	TMdCValueUnion maxValue;
	iSerializedBuffer->ReceiveL( maxValue );

	TInt64 minIntegerValue = KMaxTInt64;
	TInt64 maxIntegerValue = 0;

	switch( aRangeValueType )
		{
		case EInt32RangeValue:
			minIntegerValue = maxValue.iInt32;
			maxIntegerValue = maxValue.iInt32;
			break;
		case EUint32RangeValue:
			minIntegerValue = maxValue.iUint32;
			maxIntegerValue = maxValue.iUint32;
			break;
		case EInt64RangeValue:
			minIntegerValue = maxValue.iInt64;
			maxIntegerValue = maxValue.iInt64;
			break;
		default:		
		    break;
		}
	
	switch( rangeType )
		{
		case EMdERangeTypeAny:
			break;
		case EMdERangeTypeEqual:
			iQueryBuf->AppendL( KEqual );
			iVariables.AppendL( TColumn( minIntegerValue ) );
			break;
		case EMdERangeTypeNotEqual:
			iQueryBuf->AppendL( KNotEqual );
			iVariables.AppendL( TColumn( minIntegerValue ) );
			break;
		case EMdERangeTypeLess:
			iQueryBuf->AppendL( KLess );
			iVariables.AppendL( TColumn( maxIntegerValue ) );
			break;
		case EMdERangeTypeLessOrEqual:
			iQueryBuf->AppendL( KLessOrEqual );
			iVariables.AppendL( TColumn( maxIntegerValue ) );
			break;
		case EMdERangeTypeGreater:
			iQueryBuf->AppendL( KGreater );
			iVariables.AppendL( TColumn( minIntegerValue ) );
			break;
		case EMdERangeTypeGreaterOrEqual:
			iQueryBuf->AppendL( KGreaterOrEqual );
			iVariables.AppendL( TColumn( minIntegerValue ) );
			break;
		case EMdERangeTypeBetween:
			iQueryBuf->AppendL( KBetween );
			iVariables.AppendL( TColumn( minIntegerValue ) );
			iVariables.AppendL( TColumn( maxIntegerValue ) );
			break;
		case EMdERangeTypeNotBetween:
			iQueryBuf->AppendL( KNotBetween );
			iVariables.AppendL( TColumn( minIntegerValue ) );
			iVariables.AppendL( TColumn( maxIntegerValue ) );
			break;
		default:
#ifdef _DEBUG
			User::Panic( _L("MdSFCARa") , KErrMdEUnknownRangeType );
#endif
			User::Leave( KErrMdEUnknownRangeType );
		}
	}

// ---------------------------------------------------------------------------
// AppendPropertyConditionL      Help method for appending property condition.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendPropertyConditionL(TBool aNegated)
	{
	const TMdCPropertyCondition& condition = 
		TMdCPropertyCondition::GetFromBufferL( *iSerializedBuffer );
	
	// "SELECT ObjectId FROM "
	iQueryBuf->AppendL( KSelectObjectIdFrom );

	TDefId objectDefId = condition.iObjectDefId;

	// get object def ID for source object def if 
	// condition is not from "Object" def and 
	// (not multi object query or (multi object query and WHERE to search from 
	// doesn't match with WHAT to search))
	if( KBaseObjectDefId != objectDefId && ( !iSourceObjectDefs || 
			( iSourceObjectDefs && iSourceObjectDef && iObjectDef != iSourceObjectDef ) ) )
		{
		objectDefId = iSourceObjectDef->GetId();
		}
	AppendTableByObjectDefIdL( objectDefId );

	// "WHERE(...)"
	iQueryBuf->AppendL( KSpace );
	iQueryBuf->AppendL( KWhere );

	iQueryBuf->AppendL( KLeftBracket );

	AppendColumnByPropertyDefIdL( condition.iPropertyDefId );

	if( aNegated )
		{
		// If negated search objects without specified property

		// " ISNULL"
		iQueryBuf->AppendL( KIsNull );
		}
	else
		{
		// " NOTNULL"
		iQueryBuf->AppendL( KNotNull );
		}

	iQueryBuf->AppendL( KRightBracket );
	}

// ---------------------------------------------------------------------------
// ObjectDefForPropertyCondition        Helper method for getting object 
//                                      definition for property condition.
// ---------------------------------------------------------------------------
//
TDefId CMdSFindSqlClause::ObjectDefForPropertyCondition(TDefId aObjectDefId)
	{
	// if object def is not base object def
	if( iSourceObjectDef && KBaseObjectDefId != aObjectDefId )
		{
		const CMdsObjectDef* parent = iSourceObjectDef->GetParent();

		// check if property's object def belongs to 
		// any of source object def's parents
		while( parent )
			{
			// if property's object def belongs to 
			// any parent, use source object def's ID
			if( aObjectDefId == parent->GetId() )
				{
				return iSourceObjectDef->GetId();
				}

			// get parent's parent
			parent = parent->GetParent();
			}
		}

	return aObjectDefId;
	}

// ---------------------------------------------------------------------------
// AppendPropertyRangeConditionL        Help method for appending property 
//                                      range condition.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendPropertyRangeConditionL(TBool aNegated)
	{
	const TMdCPropertyCondition& condition = 
		TMdCPropertyCondition::GetFromBufferL( *iSerializedBuffer );

	// "SELECT ObjectId FROM "
	iQueryBuf->AppendL( KSelectObjectIdFrom );

	TDefId objectDefId = ObjectDefForPropertyCondition( 
			condition.iObjectDefId );

	AppendTableByObjectDefIdL( objectDefId );

	// "WHERE(...)"
	iQueryBuf->AppendL( KSpace );
	iQueryBuf->AppendL( KWhere );

	iQueryBuf->AppendL( KLeftBracket );

	if( aNegated )
		{
		// "NOT "
		iQueryBuf->AppendL( KNotWithSpace );
		}

	AppendColumnByPropertyDefIdL( condition.iPropertyDefId, objectDefId );

	// move position to begin of compare condition
	iSerializedBuffer->PositionL( condition.iCondition );
	
	TInt32 rangeTypeValue;
	iSerializedBuffer->ReceiveL( rangeTypeValue );
	const TMdERangeType rangeType = (TMdERangeType)rangeTypeValue;

	TMdCValueUnion minValue;
	iSerializedBuffer->ReceiveL( minValue );

	TMdCValueUnion maxValue;
	iSerializedBuffer->ReceiveL( maxValue );

	TBool integerValue = ETrue;
	TInt64 minIntegerValue = 0;
	TInt64 maxIntegerValue = 0;

	// get correct value and value's type
	switch( condition.iConditionType )
	{
		case EConditionTypePropertyIntRange:
			minIntegerValue = minValue.iInt32;
			maxIntegerValue = maxValue.iInt32;
			break;
		case EConditionTypePropertyUintRange:
			minIntegerValue = minValue.iUint32;
			maxIntegerValue = maxValue.iUint32;
			break;
		case EConditionTypePropertyInt64Range:
		case EConditionTypePropertyTimeRange:
			minIntegerValue = minValue.iInt64;
			maxIntegerValue = maxValue.iInt64;
			break;
		case EConditionTypePropertyRealRange:
			integerValue = EFalse;
			break;
		default:
#ifdef _DEBUG
			User::Panic( _L( "MdSFCAR1" ), KErrMdEUnknownConditionType );
#endif
			User::Leave( KErrMdEUnknownConditionType );
			break;
	};
	
	switch( rangeType )
		{
		case EMdERangeTypeAny:
			break;
		case EMdERangeTypeEqual:
			iQueryBuf->AppendL( KEqual );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( minIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( minValue.iReal ) );
				}
			break;
		case EMdERangeTypeNotEqual:
			iQueryBuf->AppendL( KNotEqual );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( minIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( minValue.iReal ) );
				}

			break;
		case EMdERangeTypeLess:
			iQueryBuf->AppendL( KLess );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( maxIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( maxValue.iReal ) );
				}
			break;
		case EMdERangeTypeLessOrEqual:
			iQueryBuf->AppendL( KLessOrEqual );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( maxIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( maxValue.iReal ) );
				}
			break;
		case EMdERangeTypeGreater:
			iQueryBuf->AppendL( KGreater );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( minIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( minValue.iReal ) );
				}
			break;
		case EMdERangeTypeGreaterOrEqual:
			iQueryBuf->AppendL( KGreaterOrEqual );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( minIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( minValue.iReal ) );
				}
			break;
		case EMdERangeTypeBetween:
			iQueryBuf->AppendL( KBetween );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( minIntegerValue ) );
				iVariables.AppendL( TColumn( maxIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( minValue.iReal ) );
				iVariables.AppendL( TColumn( maxValue.iReal ) );
				}
			break;
		case EMdERangeTypeNotBetween:
			iQueryBuf->AppendL( KNotBetween );
			if( integerValue )
				{
				iVariables.AppendL( TColumn( minIntegerValue ) );
				iVariables.AppendL( TColumn( maxIntegerValue ) );
				}
			else
				{
				iVariables.AppendL( TColumn( minValue.iReal ) );
				iVariables.AppendL( TColumn( maxValue.iReal ) );
				}
			break;
		default:
#ifdef _DEBUG
			User::Panic( _L( "MdSFCAR2" ), KErrMdEUnknownRangeType );
#endif
			User::Leave( KErrMdEUnknownRangeType );
		}

	iQueryBuf->AppendL( KRightBracket );
	}

// ---------------------------------------------------------------------------
// AppendPropertyTextConditionL            Help method for appending property 
//                                         text condition.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendPropertyTextConditionL(TBool aNegated)
	{
	const TMdCPropertyCondition& condition = 
		TMdCPropertyCondition::GetFromBufferL( *iSerializedBuffer );

	// "SELECT ObjectId FROM "
	iQueryBuf->AppendL( KSelectObjectIdFrom );

	TDefId objectDefId = ObjectDefForPropertyCondition( 
			condition.iObjectDefId );

	AppendTableByObjectDefIdL( objectDefId );

	// "WHERE(...)"
	iQueryBuf->AppendL( KSpace );
	iQueryBuf->AppendL( KWhere );

	iQueryBuf->AppendL( KLeftBracket );

	if( aNegated )
		{
		// "NOT "
		iQueryBuf->AppendL( KNotWithSpace );
		}

	AppendColumnByPropertyDefIdL( condition.iPropertyDefId );

	// move position to begin of compare condition
	iSerializedBuffer->PositionL( condition.iCondition );

	TUint32 compareMethodValue;
	iSerializedBuffer->ReceiveL( compareMethodValue );
	const TTextPropertyConditionCompareMethod compareMethod = 
		(TTextPropertyConditionCompareMethod)compareMethodValue;

	TPtrC16 textPtr = iSerializedBuffer->ReceivePtr16L();

	switch( compareMethod )
		{
		case ETextPropertyConditionCompareEquals:
			iQueryBuf->AppendL( KEqual );
			iVariables.AppendL( TColumn( textPtr ) );
			break;
		case ETextPropertyConditionCompareContains:
			{
			HBufC16* pattern = HBufC16::NewLC( textPtr.Length() + 2 );  // prepare for %text%

	        TPtr16 modPattern = pattern->Des();

			// add wildcard before and after text "%text%"
			modPattern.Append( KWildcard );
			modPattern.Append( textPtr );
			modPattern.Append( KWildcard );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KLike );
			}
			break;
		case ETextPropertyConditionCompareBeginsWith:
			{
			HBufC16* pattern = HBufC16::NewLC( textPtr.Length() + 1 );  // prepare for text%

	        TPtr16 modPattern = pattern->Des();

			// add wildcard after text "text%"
			modPattern.Append( textPtr );
			modPattern.Append( KWildcard );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KLike );
			}
			break;
		case ETextPropertyConditionCompareEndsWith:
			{
			HBufC16* pattern = HBufC16::NewLC( textPtr.Length() + 1 );  // prepare for %text

	        TPtr16 modPattern = pattern->Des();

			// add wildcard before text "%text"
			modPattern.Append( KWildcard );
			modPattern.Append( textPtr );

			iVariables.AppendL( TColumn( pattern ) );

			CleanupStack::Pop( pattern );

			iQueryBuf->AppendL( KLike );
			}
			break;
		default:
#ifdef _DEBUG
			User::Panic( _L( "MdSFCATC" ), KErrMdEUnknownConditionCompareMethod );
#endif
			User::Leave( KErrMdEUnknownConditionCompareMethod );
		}

	iQueryBuf->AppendL( KRightBracket );
	}

// ---------------------------------------------------------------------------
// AppendPropertyBoolConditionL            Help method for appending property 
//                                         bool condition.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendPropertyBoolConditionL(TBool aNegated)
	{
	const TMdCPropertyCondition& condition = 
		TMdCPropertyCondition::GetFromBufferL( *iSerializedBuffer );

	// "SELECT ObjectId FROM "
	iQueryBuf->AppendL( KSelectObjectIdFrom );

	TDefId objectDefId = ObjectDefForPropertyCondition( 
			condition.iObjectDefId );

	AppendTableByObjectDefIdL( objectDefId );

	// "WHERE(...)"
	iQueryBuf->AppendL( KSpace );
	iQueryBuf->AppendL( KWhere );

	iQueryBuf->AppendL( KLeftBracket );

	if( aNegated )
		{
		// "NOT "
		iQueryBuf->AppendL( KNotWithSpace );
		}

	AppendColumnByPropertyDefIdL( condition.iPropertyDefId );

	// move position to begin of compare condition
	iSerializedBuffer->PositionL( condition.iCondition );
	
	TBool boolValue;
	iSerializedBuffer->ReceiveL( boolValue );

	iQueryBuf->AppendL( KEqual );
	iVariables.AppendL( TColumn( boolValue ) );

	iQueryBuf->AppendL( KRightBracket );
	}

// ---------------------------------------------------------------------------
// AppendEventConditionL                   Help method for appending event 
//                                         condition for object query.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendEventConditionL(TBool aNegated)
	{
	const TMdCEventCondition& condition = TMdCEventCondition::GetFromBufferL( 
			*iSerializedBuffer );

	if( EQueryTypeEvent == iSearchCriteria->iQueryType )
		{
		// "SELECT EventId FROM Event"
		iQueryBuf->AppendL( KSelectEventIdFromEvent );
		}
	else
		{
		// "SELECT ObjectId FROM Event"
		iQueryBuf->AppendL( KSelectObjectIdFromEvent );
		}

	// ""namespace def ID" WHERE"
	iQueryBuf->AppendL( iNamespaceIdDes );
	iQueryBuf->AppendL( KSpace ); 
	iQueryBuf->AppendL( KWhere );

	// " NOT"
	if( aNegated )
		{
		iQueryBuf->AppendL( KSpaceWithNot );
		}

	iQueryBuf->AppendL( KLeftBracket );

	TBool firstEventCondition = ETrue;

	if( condition.iEventId != KNoId )
		{
		firstEventCondition = EFalse;

		// "EventId="eventId""
		iQueryBuf->AppendL( KEventIdEqual );
		iVariables.AppendL( TColumn( condition.iEventId ) );
		}

	if( condition.iEventDefId != KNoDefId )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		// "EventDefId="eventDefId""
		iQueryBuf->AppendL( KEventDefIdEqual );
		iVariables.AppendL( TColumn( condition.iEventDefId ) );
		}

	if( condition.iCreationTimeRange != KNoOffset )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		// "TimeStamp"
		iQueryBuf->AppendL( KEventTimeStamp );
		iSerializedBuffer->PositionL( condition.iCreationTimeRange );		
		AppendRangeL( EInt64RangeValue );
		}

	if( condition.iObjectCondition != KNoOffset )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		iQueryBuf->AppendL( KEventObjectIdIn );
		iSerializedBuffer->PositionL( condition.iObjectCondition );		
		iQueryBuf->AppendL( KLeftBracket );
		ConditionStatementL( EFalse, EAppendInFalse );
		iQueryBuf->AppendL( KRightBracket );
		}

	if( condition.iSourceCondition != KNoOffset )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		iSerializedBuffer->PositionL( condition.iSourceCondition );		
		ConditionStatementL( EFalse, EAppendInFalse );
		}

	if( condition.iParticipantCondition != KNoOffset )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		iSerializedBuffer->PositionL( condition.iParticipantCondition );		
		ConditionStatementL( EFalse, EAppendInFalse );
		}

	// check event condition has URI comparsion
	if( EEventConditionCompareNone != condition.iCompareMethod
			&& KNoOffset != condition.iUriCondition )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		switch( condition.iCompareMethod )
			{
			case EEventConditionCompareSourceURI:
	        	// "Source="URI""
	        	iQueryBuf->AppendL( KEventSourceEqual );
	        	break;

	        case EEventConditionCompareParticipantURI:
	        	// "Participant="URI""
		        iQueryBuf->AppendL( KEventParticipantEqual );
				break;

	        default:
#ifdef _DEBUG
	        	User::Panic( _L( "MdSFCAEC" ), KErrMdEUnknownConditionCompareMethod );
#endif
	        	User::Leave( KErrMdEUnknownConditionCompareMethod );
			}

		iSerializedBuffer->PositionL( condition.iUriCondition );
		TPtrC16 eventUri = iSerializedBuffer->ReceivePtr16L();
		iVariables.AppendL( TColumn( eventUri ) );		
		}

	iQueryBuf->AppendL( KRightBracket );
	}

// ---------------------------------------------------------------------------
// AppendRelationConditionL                Help method for appending relation 
//                                         condition for object query.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendRelationConditionL(TBool aNegated)
	{
	const TMdCRelationCondition& condition = 
			TMdCRelationCondition::GetFromBufferL( *iSerializedBuffer );

	if( ( ERelationConditionSideLeft == condition.iObjectSide ) || 
		( ERelationConditionSideRight == condition.iObjectSide ) )
		{
		if( EQueryTypeRelation == iSearchCriteria->iQueryType )
			{
			// "SELECT RelationId FROM Relations"
			iQueryBuf->AppendL( KSelectRelationIdFromRelation );
			}
		else
			{
			if( ERelationConditionSideLeft == condition.iObjectSide )
				{
				// "SELECT LeftObjectId FROM Relations"
				iQueryBuf->AppendL( KSelectLeftObjectIdFromRelation );
				}
			else
				{
				// "SELECT RightObjectId FROM Relations"
				iQueryBuf->AppendL( KSelectRightObjectIdFromRelation );
				}
			}

		// ""namespace def ID" "
		iQueryBuf->AppendL( iNamespaceIdDes );
		iQueryBuf->AppendL( KSpace );
		
		// "WHERE(NOT Flags&?)"
		iQueryBuf->AppendL( KWhereNotFlag );
		iVariables.AppendL( TColumn( 
				EMdERelationFlagNotPresent | 
				EMdERelationFlagDeleted | 
				EMdERelationFlagGarbageDeleted ) );

		if( condition.iLeftObjectCondition != KNoOffset )
			{
			// "AND "
			iQueryBuf->AppendL( KAnd );

			// "NOT"
			if( aNegated )
			{
				iQueryBuf->AppendL( KNot );
			}

			iQueryBuf->AppendL( KLeftBracket );

			iQueryBuf->AppendL( KLeftBracket );
			// append relation condition and left object condition
			AppendRelationSideConditionL( ETrue, condition, 
					ETrue, condition.iLeftObjectCondition );
			iQueryBuf->AppendL( KRightBracket );

			// append right object condition if it exists
			if( condition.iRightObjectCondition != KNoOffset )
				{
				// "AND "
				iQueryBuf->AppendL( KAnd );

				iQueryBuf->AppendL( KLeftBracket );
				// append only right object condition
				AppendRelationSideConditionL( EFalse, condition, 
						EFalse, condition.iRightObjectCondition );
				iQueryBuf->AppendL( KRightBracket );
				}

			iQueryBuf->AppendL( KRightBracket );
			}
		else
			{
			// append right object condition if it exists
			if( condition.iRightObjectCondition != KNoOffset )
				{
				// "AND "
				iQueryBuf->AppendL( KAnd );
				
				// "NOT"
				if( aNegated )
				{
					iQueryBuf->AppendL( KNot );
				}

				iQueryBuf->AppendL( KLeftBracket );
				// append relation condition and right object condition
				AppendRelationSideConditionL( ETrue, condition, 
						EFalse, condition.iRightObjectCondition );
				iQueryBuf->AppendL( KRightBracket );
				}
			}
		}
	else if( ERelationConditionSideEither == condition.iObjectSide )
		{
		// relation query
		if( EQueryTypeRelation == iSearchCriteria->iQueryType )
			{
			// "SELECT RelationId FROM Relations"namespace def ID" "
			iQueryBuf->AppendL( KSelectRelationIdFromRelation );
			iQueryBuf->AppendL( iNamespaceIdDes );
			iQueryBuf->AppendL( KSpace );
		
			// "WHERE(NOT Flags&?)"
			iQueryBuf->AppendL( KWhereNotFlag );
			iVariables.AppendL( TColumn( 
					EMdERelationFlagNotPresent | 
					EMdERelationFlagDeleted | 
					EMdERelationFlagGarbageDeleted ) );
			
			// append left object condition if it exists
			if( condition.iLeftObjectCondition != KNoOffset )
				{
				// "AND "
				iQueryBuf->AppendL( KAnd );

				// "NOT"
				if( aNegated )
					{
					iQueryBuf->AppendL( KNot );
					}

				iQueryBuf->AppendL( KLeftBracket );

				iQueryBuf->AppendL( KLeftBracket );
				// append relation condition and left object condition
				AppendRelationSideConditionL( ETrue, condition, 
						ETrue, condition.iLeftObjectCondition );
				iQueryBuf->AppendL( KRightBracket );

				// append right object condition if it exists
				if( condition.iRightObjectCondition != KNoOffset )
					{
					// "OR "
					iQueryBuf->AppendL( KOr );

					iQueryBuf->AppendL( KLeftBracket );
					// append only right object condition
					AppendRelationSideConditionL( EFalse, condition, 
							EFalse, condition.iRightObjectCondition );
					iQueryBuf->AppendL( KRightBracket );
					}

				iQueryBuf->AppendL( KRightBracket );
				}
			// append right object condition if it exists
			else if( condition.iRightObjectCondition != KNoOffset )
				{
				// "AND "
				iQueryBuf->AppendL( KAnd );

				// "NOT"
				if( aNegated )
					{
					iQueryBuf->AppendL( KNot );
					}

				iQueryBuf->AppendL( KLeftBracket );
				// append relation condition and right object condition
				AppendRelationSideConditionL( ETrue, condition, 
						EFalse, condition.iRightObjectCondition );
				iQueryBuf->AppendL( KRightBracket );
				}
			// append only relation conditions
			else 
				{
				// "AND "
				iQueryBuf->AppendL( KAnd );

				// "NOT"
				if( aNegated )
					{
					iQueryBuf->AppendL( KNot );
					}

				iQueryBuf->AppendL( KLeftBracket );
				// append relation conditions
				AppendRelationSideConditionL( ETrue, condition, 
						EFalse, KNoOffset ); // no object condition
				iQueryBuf->AppendL( KRightBracket );
				}
			}
		// object or event query
		else
			{
			if( condition.iLeftObjectCondition != KNoOffset )
				{
				// "SELECT LeftObjectId FROM Relations"namespace def ID" "
				iQueryBuf->AppendL( KSelectLeftObjectIdFromRelation );
				iQueryBuf->AppendL( iNamespaceIdDes );
				iQueryBuf->AppendL( KSpace );

				// "WHERE(NOT Flags&?)"
				iQueryBuf->AppendL( KWhereNotFlag );
				iVariables.AppendL( TColumn( 
						EMdERelationFlagNotPresent | 
						EMdERelationFlagDeleted | 
						EMdERelationFlagGarbageDeleted ) );

				// "AND "
				iQueryBuf->AppendL( KAnd );
				
				// "NOT"
				if( aNegated )
					{
					iQueryBuf->AppendL( KNot );
					}

				iQueryBuf->AppendL( KLeftBracket );
				AppendRelationSideConditionL( ETrue, condition, 
						ETrue, condition.iLeftObjectCondition );
				iQueryBuf->AppendL( KRightBracket );

				if( condition.iRightObjectCondition != KNoOffset )
					{
					// " UNION "
					iQueryBuf->AppendL( KUnion );

					// "SELECT RightObjectId FROM Relations"namespace def ID" "
					iQueryBuf->AppendL( KSelectRightObjectIdFromRelation );
					iQueryBuf->AppendL( iNamespaceIdDes );
					iQueryBuf->AppendL( KSpace );
					
					// "WHERE(NOT Flags&?)"
					iQueryBuf->AppendL( KWhereNotFlag );
					iVariables.AppendL( TColumn( 
							EMdERelationFlagNotPresent | 
							EMdERelationFlagDeleted | 
							EMdERelationFlagGarbageDeleted ) );

					// "AND "
					iQueryBuf->AppendL( KAnd );
					
					// "NOT"
					if( aNegated )
						{
						iQueryBuf->AppendL( KNot );
						}

					iQueryBuf->AppendL( KLeftBracket );
					AppendRelationSideConditionL( ETrue, condition, 
							EFalse, condition.iRightObjectCondition );
					iQueryBuf->AppendL( KRightBracket );
					}
				}
			else
				{
				// "SELECT RightObjectId FROM Relations"namespace def ID" "
				iQueryBuf->AppendL( KSelectRightObjectIdFromRelation );
				iQueryBuf->AppendL( iNamespaceIdDes );
				iQueryBuf->AppendL( KSpace );

				// "WHERE(NOT Flags&?)"
				iQueryBuf->AppendL( KWhereNotFlag );
				iVariables.AppendL( TColumn( 
						EMdERelationFlagNotPresent | 
						EMdERelationFlagDeleted | 
						EMdERelationFlagGarbageDeleted ) );

				// "AND "
				iQueryBuf->AppendL( KAnd );
				
				// "NOT"
				if( aNegated )
					{
					iQueryBuf->AppendL( KNot );
					}

				iQueryBuf->AppendL( KLeftBracket );
				AppendRelationSideConditionL( ETrue, condition, 
						EFalse, condition.iRightObjectCondition );
				iQueryBuf->AppendL( KRightBracket );
				}
			}
		}
	else
		{
		// unknown relation condition side
#ifdef _DEBUG
		User::Panic( _L("MdSFCARC") , KErrCorrupt );
#endif
		User::Leave( KErrCorrupt );
		}
	}

void CMdSFindSqlClause::AppendRelationSideConditionL(
		TBool aRelationConditions, 
		const TMdCRelationCondition& aRelationCondition, 
		TBool aLeftSide, TUint32 aRelationObjectConditionOffset)
	{
	TBool firstEventCondition = ETrue;

	if( aRelationConditions )
		{
		if( aRelationCondition.iRelationId != KNoId )
			{
			firstEventCondition = EFalse;
	
			// "RelationId="relationID""
			iQueryBuf->AppendL( KRelationIdEqual );
			iVariables.AppendL( TColumn( aRelationCondition.iRelationId ) );
			}
	
		if( aRelationCondition.iRelationIds.iPtr.iCount > 0 && 
				aRelationCondition.iRelationIds.iPtr.iOffset != KNoOffset )
			{
			if( firstEventCondition == EFalse )
				{
				iQueryBuf->AppendL( KSpace );
				iQueryBuf->AppendL( KAnd );
				}
			firstEventCondition = EFalse;
	
			// "RelationId IN("relationIDs")"
			iQueryBuf->AppendL( KRelationIdIn );
	
			iQueryBuf->AppendL( KLeftBracket );
	
			iSerializedBuffer->PositionL( 
					aRelationCondition.iRelationIds.iPtr.iOffset );
	
			TItemId relationId;
			iSerializedBuffer->ReceiveL( relationId );
			iQueryBuf->AppendL( KVariable );
			iVariables.AppendL( TColumn( relationId ) );
	
			for( TInt32 i = 1; i < aRelationCondition.iRelationIds.iPtr.iCount; i++ )
				{
				iSerializedBuffer->ReceiveL( relationId );
				iQueryBuf->AppendL( KCommaVariable );
				iVariables.AppendL( TColumn( relationId ) );
				}
	
			iQueryBuf->AppendL( KRightBracket );
			}
		
		if( aRelationCondition.iRelationDefId != KNoDefId )
			{
			if( firstEventCondition == EFalse )
				{
				iQueryBuf->AppendL( KSpace );
				iQueryBuf->AppendL( KAnd );
				}
			firstEventCondition = EFalse;
	
			// "RelationDefId="relationDefId""
			iQueryBuf->AppendL( KRelationDefIdEqual );
			iVariables.AppendL( TColumn( aRelationCondition.iRelationDefId ) );
			}
	
		if( aRelationCondition.iGuid != KNoOffset )
			{
			if( firstEventCondition == EFalse )
				{
				iQueryBuf->AppendL( KSpace );
				iQueryBuf->AppendL( KAnd );
				}
			firstEventCondition = EFalse;
	
			iSerializedBuffer->PositionL( aRelationCondition.iGuid );
			
			TInt64 guidHigh;
			iSerializedBuffer->ReceiveL( guidHigh );
			TInt64 guidLow;
			iSerializedBuffer->ReceiveL( guidLow );
			
			// "RelationGuidHigh="relationGuidHigh" AND 
			// RelationGuidLow="relationGuidLow""
			iQueryBuf->AppendL( KRelationGuidEqual );
			iVariables.AppendL( TColumn( guidHigh ) );
			iVariables.AppendL( TColumn( guidLow ) );
			}
	
		if( aRelationCondition.iParameterRange != KNoOffset )
			{
			if( firstEventCondition == EFalse )
				{
				iQueryBuf->AppendL( KSpace );
				iQueryBuf->AppendL( KAnd );
				}
			firstEventCondition = EFalse;
	
			// "Parameter"
			iQueryBuf->AppendL( KRelationParameter );
			iSerializedBuffer->PositionL( aRelationCondition.iParameterRange );
			AppendRangeL( EInt32RangeValue );
			}
	
		if( aRelationCondition.iLastModifiedDateRange != KNoOffset )
			{
			if( firstEventCondition == EFalse )
				{
				iQueryBuf->AppendL( KSpace );
				iQueryBuf->AppendL( KAnd );
				}
			firstEventCondition = EFalse;
	
			// "LastModifiedDate"
			iQueryBuf->AppendL( KRelationLastModifiedDate );
			iSerializedBuffer->PositionL( aRelationCondition.iLastModifiedDateRange );		
			AppendRangeL( EInt64RangeValue );
			}
		}

	// object condition on left or right side
	if( aRelationObjectConditionOffset != KNoOffset )
		{
		if( firstEventCondition == EFalse )
			{
			iQueryBuf->AppendL( KSpace );
			iQueryBuf->AppendL( KAnd );
			}
		firstEventCondition = EFalse;

		iSerializedBuffer->PositionL( aRelationObjectConditionOffset );

		if ( aLeftSide )
			{
			ConditionStatementL( EFalse, EAppendInRelationLeft );
			}
		else
			{
			ConditionStatementL( EFalse, EAppendInRelationRight );
			}
		}

	// if there was no any condition add "empty" true condition
	if( firstEventCondition )
		{
		iQueryBuf->AppendL( KEmptyTrue );
		}
	}

// ---------------------------------------------------------------------------
// AppendTableByObjectDefIdL               Help method for searching object 
//                                         definition's table, based on 
//                                         object definition's ID, and 
//                                         appending those to buffer.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendTableByObjectDefIdL(TDefId aObjectDefId)
	{
	const CMdsObjectDef* objectDef = iNamespaceDef->GetObjectByIdL( aObjectDefId );

	if ( !objectDef )
		{
#ifdef _DEBUG
		User::Panic( _L("MdSFCAT1") , KErrMdEUnknownObjectDef );
#endif
		User::Leave( KErrMdEUnknownObjectDef );
		}

	iQueryBuf->AppendL( objectDef->GetName() );
	iQueryBuf->AppendL( iNamespaceIdDes );
	}

// ---------------------------------------------------------------------------
// AppendColumnByPropertyDefIdL            Help method for searching property 
//                                         definition's column, based on 
//                                         property definition's ID, and 
//                                         appending those to buffer.
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendColumnByPropertyDefIdL(TDefId aPropertyDefId, 
		TDefId aObjectDefId, TBool aResult)
	{
	CMdsPropertyDef* propDef = NULL;

	if( aObjectDefId == KNoDefId )
		{
		if ( !iObjectDef )
			{
			const CMdsObjectDef* baseObjectDef = iNamespaceDef->GetObjectByIdL(
					KBaseObjectDefId );
	
			// base object definition must always exist
			if ( !baseObjectDef )
				{
#ifdef _DEBUG
				User::Panic( _L("MdSFCAC1") , KErrMdEUnknownObjectDef );
#endif				
				User::Leave( KErrMdEUnknownObjectDef );
				}

			propDef = CONST_CAST( CMdsPropertyDef*, 
					baseObjectDef->GetPropertyByIdL( aPropertyDefId ) );

			// if property definition doesn't exist in base object definition 
			// search from whole namespace
			if ( !propDef )
				{
				propDef = CONST_CAST( CMdsPropertyDef*, 
						iNamespaceDef->GetPropertyL( aPropertyDefId ) );
				}
			}
		else
			{
			propDef = CONST_CAST( CMdsPropertyDef*, 
					iObjectDef->GetPropertyByIdL( aPropertyDefId ) );
			}
		}
	else
		{
		const CMdsObjectDef* objDef = iNamespaceDef->GetObjectByIdL( 
				aObjectDefId );
		propDef = CONST_CAST( CMdsPropertyDef*, 
				objDef->GetPropertyByIdL( aPropertyDefId ) );
		}

	if( propDef )
		{
		iQueryBuf->AppendL( propDef->GetName() );

		if( aResult )
			{
			if( iAppendToResultRow )
				{
				// expected result for property's type
			    iResultRow.AppendL( TColumn( propDef->GetSqlType() ) );

			    iPropertyFilters.AppendL( propDef );
				}
			}
		}
	// object definition must always exist for given property definition ID
	else
		{
#ifdef _DEBUG
		User::Panic( _L("MdSFCAC2") , KErrMdEUnknownObjectDef );
#endif
		User::Leave( KErrMdEUnknownObjectDef );
		}
	}

// ---------------------------------------------------------------------------
// AppendGroupByL           Appends Group BY statements
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendGroupByL()
	{
	// if query type is object query
	if ( EQueryTypeObject == iSearchCriteria->iQueryType )
		{
		switch( iSearchCriteria->iQueryResultType )
			{
			case EQueryResultModeItem:
			case EQueryResultModeId:
                {
                // "GROUP BY BO.ObjectId "
                iQueryBuf->AppendL( KGroupByObjectId );
                }
                break;
			case EQueryResultModeCount:
				{
				}
				break;

			default:
#ifdef _DEBUG
				User::Panic( _L("MdSFCAGB") , KErrMdEUnknownQueryResultMode );
#endif
				User::Leave( KErrMdEUnknownQueryResultMode );
			}
		}
	}

// ---------------------------------------------------------------------------
// AppendOrderByL           Appends ORDER BY statements
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendOrderByL()
    {
    if( iSearchCriteria->iQueryResultType == EQueryResultModeCount )
        {
        // No need to sort or group if only one count is returned
        return;
        }
    
	if( iSearchCriteria->iOrderRules.iPtr.iCount > 0 )
		{
		iQueryBuf->AppendL( KOrderBy );

		iSerializedBuffer->PositionL( iSearchCriteria->iOrderRules.iPtr.iOffset );

		// Add recursive all children.
	    for( TUint32 i = 0; i < iSearchCriteria->iOrderRules.iPtr.iCount; i++)
			{
			if( i > 0 )
				{
				iQueryBuf->AppendL( KComma );
				}

			const TMdCOrderRule& orderRule = TMdCOrderRule::GetFromBufferL( 
					*iSerializedBuffer );

			switch( iSearchCriteria->iQueryType )
				{
				case EQueryTypeObject:
					switch( orderRule.iOrderRule )
						{
						case EOrderRuleTypeProperty:
							AppendColumnByPropertyDefIdL( orderRule.iPropertyDefId );
							break;

						case EOrderRuleTypeItemID:
							iQueryBuf->AppendL( KBaseObjectObjectId );
							break;

						case EOrderRuleTypeObjectURI:
							_LIT( KQueryBaseObjectURI, "BO.URI");
							iQueryBuf->AppendL( KQueryBaseObjectURI );
							break;

						case EOrderRuleTypeObjectDef:
							_LIT( KQueryBaseObjectDefId, "BO.ObjectDefId");
							iQueryBuf->AppendL( KQueryBaseObjectDefId );
							break;

						case EOrderRuleTypeUsageCount:
							_LIT( KQueryBaseObjectUsageCount, "BO.UsageCount");
							iQueryBuf->AppendL( KQueryBaseObjectUsageCount );
							break;

						default:
							User::Leave( KErrNotSupported );
						}
					break;

				case EQueryTypeRelation:
					switch( orderRule.iOrderRule )
						{
						case EOrderRuleTypeItemID:
							iQueryBuf->AppendL( KRelationId );
							break;

						case EOrderRuleTypeLastModifiedDate:
							iQueryBuf->AppendL( KRelationLastModifiedDate );
							break;

						case EOrderRuleTypeParameterValue:
							iQueryBuf->AppendL( KRelationParameter );
							break;

						case EOrderRuleTypeLeftObjectID:
							_LIT( KQueryRelationLeftObjectId, "LeftObjectId");
							iQueryBuf->AppendL( KQueryRelationLeftObjectId );
							break;

						case EOrderRuleTypeRightObjectID:
							_LIT( KQueryRelationRightObjectId, "RightObjectId");
							iQueryBuf->AppendL( KQueryRelationRightObjectId );
							break;

						case EOrderRuleTypeRelationDef:
							_LIT( KQueryRelationDef, "RelationDefId");
							iQueryBuf->AppendL( KQueryRelationDef );
							break;

						default:
							User::Leave( KErrNotSupported );
						}
					break;

				case EQueryTypeEvent:
					switch( orderRule.iOrderRule )
						{
						case EOrderRuleTypeItemID:
							iQueryBuf->AppendL( KEventId );
							break;

						case EOrderRuleTypeCreationTime:
							iQueryBuf->AppendL( KEventTimeStamp );
							break;

						case EOrderRuleTypeSourceURI:
							_LIT( KQueryEventSource, "Source");
							iQueryBuf->AppendL( KQueryEventSource );
							break;

						case EOrderRuleTypeParticipantURI:
							_LIT( KQueryEventParticipant, "Participant");
							iQueryBuf->AppendL( KQueryEventParticipant );
							break;

						case EOrderRuleTypeTargetObjectID:
							_LIT( KQueryEventObjectId, "ObjectId");
							iQueryBuf->AppendL( KQueryEventObjectId );
							break;

						case EOrderRuleTypeEventDef:
							_LIT( KQueryEventDef, "EventDefId");
							iQueryBuf->AppendL( KQueryEventDef );
							break;

						default:
							User::Leave( KErrNotSupported );
						}
					break;

				default:
#ifdef _DEBUG
					User::Panic( _L("MdSFCAOB") , KErrMdEUnknownQueryType );
#endif
					User::Leave( KErrMdEUnknownQueryType );
				}

			if ( !orderRule.iCaseSensitive )
				{
				iQueryBuf->AppendL( KNoCaseSensitive );
				}

			if( orderRule.iAscending )
				{
				iQueryBuf->AppendL( KAsc );
				}
			else
				{
				iQueryBuf->AppendL( KDesc );
				}
			}
		
		iQueryBuf->AppendL( KSpace );				
		}
    }

// ---------------------------------------------------------------------------
// AppendLimitAndOffsetL            LIMIT and OFFSET statements
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::AppendLimitAndOffsetL()
    {
    if ( iSearchCriteria->iOffset > 0 )
    	{
    	// 10 is maximum length of text format TUint32
    	iQueryBuf->ReserveSpaceL( iQueryBuf->ConstBufferL().Length() + 
    			2 * 10 );

    	iQueryBuf->AppendL( KLimit );
    	iQueryBuf->BufferL().AppendNum( iSearchCriteria->iLimit );
    	iQueryBuf->AppendL( KComma );
    	iQueryBuf->BufferL().AppendNum( iSearchCriteria->iOffset );
    	}
    else if ( iSearchCriteria->iLimit != KMdEQueryDefaultMaxCount )
    	{
    	// 10 is maximum length of text format TUint32
    	iQueryBuf->ReserveSpaceL( iQueryBuf->ConstBufferL().Length() + 
    			10 );
    	
    	iQueryBuf->AppendL( KLimit );
    	iQueryBuf->BufferL().AppendNum( iSearchCriteria->iLimit );
    	}
    }

// ---------------------------------------------------------------------------
// CopyVariablesL   Copies number of variables from variable table to the
//                  end of the variable table
// ---------------------------------------------------------------------------
//
void CMdSFindSqlClause::CopyVariablesL( TInt aStart, TInt aEnd )
    {
    TColumn column;
    
    for ( TInt i(aStart); i<aEnd; ++i )
        {
        column = iVariables.Column(i);
        
        switch( column.Type() )
            {
            case EColumnBool:
                {
                TBool v=0;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnInt32:
                {
                TInt32 v=0;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnUint32:
                {
                TUint32 v=0;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnTime:
                {
                TTime v(0);
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnInt64:
                {
                TInt64 v=0;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnReal32:
                {
                TReal32 v=0;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnReal64:
                {
                TReal64 v=0;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            case EColumnDes16:
                {
                TPtrC16 v = TPtr16((TUint16*)0, 0); //KNullPtr16;
                column.Get( v );
                iVariables.AppendL( TColumn( v ) );
                break;
                }
            default:
#ifdef _DEBUG
				User::Panic( _L("MdSFCCoV") , KErrMdEUnknownPropertyType );
#endif            	
                User::Leave( KErrMdEUnknownPropertyType );
            }
        }
    }
