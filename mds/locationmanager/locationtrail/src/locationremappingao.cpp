/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Remap location data to location objects*
*/

#include "locationremappingao.h"
#include "mdequery.h"
#include "mdccommon.h"
#include "mdeconstants.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h" 
#include "mdeobjectquery.h"

using namespace MdeConstants;


// --------------------------------------------------------------------------
// CLocationRemappingAO::NewL()
// --------------------------------------------------------------------------
//
CLocationRemappingAO* CLocationRemappingAO::NewL()
	{
	LOG( "CLocationRemappingAO::NewL" ); // DEBUG INFO
	
	CLocationRemappingAO* self = new (ELeave) CLocationRemappingAO();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::ConstructL()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::ConstructL()
	{
	LOG( "CLocationRemappingAO::ConstructL" ); // DEBUG INFO
	CActiveScheduler::Add( this );
	iMdEClient = NULL;
	
	TRAPD(err, ReadTimeFromCenRepL());
	if (err)
		{
		LOG( "CLocationRemappingAO::ConstructL, Can not read timelimit value from CenRep");
		iTimeLimit = TTimeIntervalSeconds( KRemappingTime );
		}
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::~CLocationRemappingAO()
// --------------------------------------------------------------------------
//
CLocationRemappingAO::~CLocationRemappingAO()
	{
	LOG( "CLocationRemappingAO::~CLocationRemappingAO" ); // DEBUG INFO

	StopRemapping();
	Cancel();
	
	iObjects.ResetAndDestroy();
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::CLocationRemappingAO()
// --------------------------------------------------------------------------
//
CLocationRemappingAO::CLocationRemappingAO() : CActive( CActive::EPriorityStandard ),
	iState ( EIdle ),
	iContinue( EFalse ),
	iTimed( EFalse ),
	iRequested( EFalse ),
	iStartTime( 0 ),
	iEndTime( 0 ),
	iTimeLimit( 0 )
	{
	// No implementation required
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::InitialiseL()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::InitialiseL(CMdESession* aMdEClient)
	{
	LOG( "CLocationRemappingAO::Initialise start" ); // DEBUG INFO
	iMdEClient = aMdEClient;
	
	// namespace defaults
	iNamespaceDef = &iMdEClient->GetDefaultNamespaceDefL();
	
	// media object definitions
    iObjImageDef = &iNamespaceDef->GetObjectDefL( Image::KImageObject );
    iObjVideoDef = &iNamespaceDef->GetObjectDefL( Video::KVideoObject );
    iPropDateDef = &iObjImageDef->GetPropertyDefL( Object::KCreationDateProperty );
    iPropModifiedDef = &iObjImageDef->GetPropertyDefL( Object::KLastModifiedDateProperty );
	
    // location object definitions
	iObjLocationDef = &iNamespaceDef->GetObjectDefL( Location::KLocationObject );
	iPropLatDef = &iObjLocationDef->GetPropertyDefL( Location::KLatitudeProperty );
    iPropLongDef = &iObjLocationDef->GetPropertyDefL( Location::KLongitudeProperty );
	iPropAltDef = &iObjLocationDef->GetPropertyDefL( Location::KAltitudeProperty );
	
	iContainsLocationRelDef = &iNamespaceDef->GetRelationDefL( Relations::KContainsLocation );
	
	LOG( "CLocationRemappingAO::Initialise end" );
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::Append()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::Append( TRemapItem aItem )
	{
	iRemapItems.Append( aItem );
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::ResetQueue()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::ResetQueue()
	{
	iRemapItems.Reset();
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::ItemsInQueue()
// --------------------------------------------------------------------------
//
TBool CLocationRemappingAO::ItemsInQueue()
	{
	return iRemapItems.Count() > 0;
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::CheckQueue()
// --------------------------------------------------------------------------
//
TBool CLocationRemappingAO::CheckQueue()
	{
	LOG( "CLocationRemappingAO::CheckQueue - start" );
	TBool create( EFalse );
	TInt index = iRemapItems.Count() - 1;
	TTime startTime( 0 );
	startTime.UniversalTime();
	startTime = startTime - iTimeLimit;
	RArray<TItemId> removeLocations; 
	
	for( TInt i = index; i >= 0; i-- )
		{
		if( iRemapItems[i].iLocationId == 0 )
			{
			create = ETrue;
			}
		
		if( iRemapItems[i].iTime < startTime )
			{
			LOG( "CLocationRemappingAO::CheckQueue - old item found" );
			if ( iRemapItems[i].iLocationId != 0 )
				{
				// save old location id 
				TInt err = removeLocations.Find( iRemapItems[i].iLocationId );
				if ( err == KErrNotFound )
					{
					removeLocations.Append( iRemapItems[i].iLocationId );
					}
				}
			iRemapItems.Remove( i );
			create = ETrue;
			}
		}
	
	// clear all "old" location id's from remap items
	if( removeLocations.Count() < 0 )
		{
		for( TInt i = iRemapItems.Count() - 1; i >=0; i-- )
			{
			TInt err = removeLocations.Find( iRemapItems[i].iLocationId );
			if ( err != KErrNotFound )
				{
				LOG( "CLocationRemappingAO::CheckQueue - old item found" );
				iRemapItems[i].iLocationId = 0;
				}
			}
		}
	
	removeLocations.Close();
	LOG( "CLocationRemappingAO::CheckQueue - end" );
	return create;
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::StopRemapping()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::StopRemapping()
	{
	LOG( "CLocationRemappingAO::StopRemapping" ); // DEBUG INFO
	NextState(EIdle);
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::StartRemappingObjects()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::StartRemappingObjects( const TLocationData& aLocationData )
	{
	iLocationData = aLocationData;
	LOG( "CLocationRemappingAO::StartRemappingObjects" ); // DEBUG INFO
	TInt count = iRemapItems.Count();
	if ( count == 0 )
		{
		LOG("CLocationRemappingAO::StartRemappingObjects No need to remap location objects");
		return;
		}
	NextState( ERemapObjects );
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::NextState()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::NextState(TMappingState aState)
	{
	LOG1( "CLocationRemappingAO::NextState - state: %d", aState ); // DEBUG INFO
	iState = aState;
	// Request complete if not idling
	if (iState != EIdle)
		{
		TRequestStatus* pStatus = &iStatus;
		User::RequestComplete( pStatus, KErrNone );
		SetActive();
		}	
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::DoCancel()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::DoCancel()
	{
	LOG( "CLocationRemappingAO::DoCancel" ); // DEBUG INFO
	// back to idle
	NextState(EIdle);
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::RunError()
// --------------------------------------------------------------------------
//
TInt CLocationRemappingAO::RunError( TInt aError )
	{
	if (aError != KErrNone)
		{
		LOG1( "CLocationRemappingAO::RunError with error code: %d", aError ); // DEBUG INFO
		}
	// back to idle
	NextState(EIdle);
	return KErrNone;
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::RunL()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::RunL()
	{
	LOG1( "CCameraTrailMonitorAO::RunL iStatus: %d", iStatus.Int() ); // DEBUG INFO
	
	switch(iState)
		{
		case EIdle:			
			{
			LOG( "CLocationRemappingAO::RunL() - EIdle" );			
			break;
			}

		case ERemapObjects:
			{
			// start remapping object data if Location Object request succeed
			RemapObjectsL();
			NextState(ECommitObjects);
			break;			
			}

		case ECommitObjects:
			{
			// commit location object data
			CommitObjectsL();
			NextState( EIdle );
			break;
			}
			
		default:		
			{
			User::Leave( KErrUnknown );		
			break;
			}
		}
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::RemapObjectsL()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::RemapObjectsL()
	{	
	LOG( "CLocationRemappingAO::RemapObjects - start" ); // DEBUG INFO
	// remap location data to location objects
	TInt count = iRemapItems.Count();
	
	for( TInt i = 0 ; i < count; i++ )
		{
		TInt err = iObjectIds.Find( iRemapItems[i].iLocationId );
		if ( err == KErrNotFound )
			{
			iObjectIds.Append( iRemapItems[i].iLocationId );
			}
		}

	count = iObjectIds.Count();
	for (TInt i = 0; i < count; i++)
		{
		CMdEObject* location = iMdEClient->OpenObjectL(iObjectIds[i], *iObjLocationDef);
		
		if(location)
		    {
            CleanupStack::PushL( location );
		
            if (location->PropertyCount(*iPropLatDef) == 0)
                {
                location->AddReal64PropertyL(*iPropLatDef, iLocationData.iPosition.Latitude());
                LOG( "CLocationRemappingAO::RemapObjects - wrote latitude" );
                }
            if (location->PropertyCount(*iPropLongDef) == 0)
                {
                location->AddReal64PropertyL(*iPropLongDef, iLocationData.iPosition.Longitude());
                LOG( "CLocationRemappingAO::RemapObjects - wrote longitude" );
                }
            if (location->PropertyCount(*iPropAltDef) == 0)
                {
                location->AddReal64PropertyL(*iPropAltDef, iLocationData.iPosition.Altitude());
                LOG( "CLocationRemappingAO::RemapObjects - wrote altitude" );
                }
            CMdEProperty* modProp = NULL;
            location->Property( *iPropModifiedDef, modProp, 0 );
            if ( modProp )
                {
                TTime timestamp( 0 );
                timestamp.UniversalTime();
                modProp->SetTimeValueL( timestamp );
                }
            iObjects.AppendL(location);
            CleanupStack::Pop( location );
            
          }
		}
	count = iRemapItems.Count();
	LOG1("CLocationRemappingAO::RemapObjectsL - updating relations, count:%d", count);
	// update relation timestamp, composer will then update exif data
	for( TInt i = 0; i < count; i++ )
		{
		CMdERelation* relation = NULL;
		relation = iMdEClient->GetRelationL( iRemapItems[i].iRelationId );
		
		if(relation)
		    {
		    CleanupStack::PushL(relation);
            TTime timestamp( 0 );
            timestamp.UniversalTime();
            relation->SetLastModifiedDate( timestamp );
    	
            iMdEClient->UpdateRelationL( *relation );
		    CleanupStack::PopAndDestroy(relation);
		    }
		}
	
	LOG("CLocationRemappingAO::RemapObjectsL - relations updated");
	
	iObjectIds.Reset();
	ResetQueue();
	
	LOG( "CLocationRemappingAO::RemapObjects - end" );
	}


// --------------------------------------------------------------------------
// CLocationRemappingAO::CommitObjectsL()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::CommitObjectsL()
	{
	LOG( "CLocationRemappingAO::CommitObjects" ); // DEBUG INFO
	// commit location objects
	if( iObjects.Count() > 0 )
		{
		iMdEClient->CommitObjectsL(iObjects);
		iObjects.ResetAndDestroy();
		}	
	}
	    	
// --------------------------------------------------------------------------
// CLocationManagerServer::ReadCenRepValueL
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::ReadTimeFromCenRepL()
	{
	LOG( "CLocationRemappingAO::ReadTimeFromCenRepL(), begin" );
	CRepository* repository = CRepository::NewLC( KRepositoryUid ); 
	TInt value( 0 );
	User::LeaveIfError( repository->Get(KRemappingTimeKey, value) );
	CleanupStack::PopAndDestroy(repository);

	LOG1( "CLocationRemappingAO::ReadTimeFromCenRepL(), Using timelimit value:%d seconds", value );
	iTimeLimit = TTimeIntervalSeconds( value );

    LOG( "CLocationRemappingAO::ReadTimeFromCenRepL(), end" );   
	}

// --------------------------------------------------------------------------
// CLocationRemappingAO::UpdateRelationsL()
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::UpdateRelationsL( TItemId aLocationId )
	{ 
	LOG("CLocationRemappingAO::UpdateRelationsL - start");
	CMdENamespaceDef& namespaceDef = iMdEClient->GetDefaultNamespaceDefL();
	
	// "contains" relation definition
	CMdERelationDef& containsRelDef = namespaceDef.GetRelationDefL( 
			Relations::KContainsLocation );

	TInt count = iRemapItems.Count() - 1;
	for( TInt i = count; i >= 0; i-- )
		{
		if( iRemapItems[i].iLocationId == 0 )
			{
			if( iRemapItems[i].iRelationId == 0 )
				{
				CMdERelation* relationObject = iMdEClient->NewRelationLC( containsRelDef,
						iRemapItems[i].iObjectId, aLocationId, 0 );
				iMdEClient->AddRelationL( *relationObject );
				CleanupStack::PopAndDestroy( relationObject );
				LOG("new relation created");
				}
			else
				{
				CMdERelation* relationObject = iMdEClient->GetRelationL( iRemapItems[i].iRelationId );
                CleanupStack::PushL(relationObject);
				relationObject->SetRightObjectIdL( aLocationId );
				iMdEClient->UpdateRelationL( *relationObject );
                CleanupStack::PopAndDestroy(relationObject);
				LOG("old relation updated");
				}
			iRemapItems.Remove( i );
			}
		}
	LOG("CLocationRemappingAO::UpdateRelationsL - end");
	}

#ifdef LOC_REVERSEGEOCODE
// --------------------------------------------------------------------------
// CLocationRemapping::AttachGeoTagsL
// --------------------------------------------------------------------------
//
void CLocationRemappingAO::AttachGeoTagsL( CTagCreator *aTagCreator,
                                         const TItemId aCountryTagId, const TItemId aCityTagId )
    { 
    LOG("CLocationRemapping::AttachGeoTagsL - start");
    
    TInt count = iObjectIds.Count() - 1;
    for( TInt i = count; i >= 0; i-- )
        {
        aTagCreator->AttachTagsL( iObjectIds[i], aCountryTagId, aCityTagId );
        }
    
    iObjectIds.Reset();
    
    LOG("CLocationRemapping::AttachGeoTagsL - end");
    }

#endif //LOC_REVERSEGEOCODE


// --------------------------------------------------------------------------
// TRemapItem constructor
// --------------------------------------------------------------------------
//
TRemapItem::TRemapItem() :
	iObjectId( 0 ),
	iLocationId( 0 ),
	iRelationId( 0 ),
	iTime( 0 )
	{
	
	}

// End of file
