/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A class responsible for geotagging of media files in background.
*
*/

#include <e32cmn.h> 
#include <lbserrors.h>
#include <lbssatellite.h>
#include <ecom.h>

#include "locationmanagerdebug.h"
#include "mdeconstants.h"
#include "mdesession.h"
#include "cinternalgeotagger.h"

using namespace MdeConstants;

_LIT ( KCountry, "country:");   // country:india 

const TReal64 KZeroLatLon (0.000000 );

// --------------------------------------------------------------------------
// CInternalGeoTagger::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CInternalGeoTagger* CInternalGeoTagger::NewL( CMdESession* aMdeSession,
            MGeoTaggerObserver* aObserver )
    {
    CInternalGeoTagger* self = 
            new (ELeave) CInternalGeoTagger( aMdeSession, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
        
// --------------------------------------------------------------------------
// CInternalGeoTagger::CInternalGeoTagger
// --------------------------------------------------------------------------
//  
CInternalGeoTagger::CInternalGeoTagger( CMdESession* aMdeSession,
        MGeoTaggerObserver* aObserver )
                        : iMdeSession(aMdeSession),
                        iLocationQuery(NULL),
                        iRelationQuery(NULL),
                        iTagQuery(NULL),
                        iIndex(0),
                        iSessionReady( EFalse ),
                        iObserver( aObserver ),
                        iASW(NULL),
                        iTagPendingHandlerFlag(EFalse)
#ifdef LOC_GEOTAGGING_CELLID
						,iGeoConverter(NULL)
#endif						
#ifdef LOC_REVERSEGEOCODE
                        ,iTagCreator(NULL)
                        ,iRevGeocoderPlugin(NULL)
#endif						
    {
    iMdeSessionOwnFlag = (iMdeSession == NULL);
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::ConstructL
// --------------------------------------------------------------------------
//    
void CInternalGeoTagger::ConstructL()
    {
    LOG("CInternalGeoTagger::ConstructL ,begin");
    if(iMdeSession != NULL)
        {
		LOG("Not owning mds session");
        iMdeSessionOwnFlag = EFalse;
        //object is not owning iMdeSession
        iSessionReady = ETrue;
#ifdef LOC_REVERSEGEOCODE
        iTagCreator = CTagCreator::NewL();
        
        iTagCreator->SetSession( iMdeSession );
#endif //LOC_REVERSEGEOCODE
        }
    else
        {
        iMdeSessionOwnFlag = ETrue;
        iASW = new (ELeave) CActiveSchedulerWait();
        iMdeSession = CMdESession::NewL( *this );
        //for creating tags
#ifdef LOC_REVERSEGEOCODE
        iTagCreator = CTagCreator::NewL();
#endif
        
        iASW->Start();
        delete iASW;
        iASW = NULL;
        }
    
    //for reverse geocoding (geo-tagging)
    
#ifdef LOC_REVERSEGEOCODE
    	
		TInt pluginerr = KErrNone;
    TRAP(pluginerr,iRevGeocoderPlugin = CReverseGeoCoderPlugin::NewL());
    		
   if(pluginerr == KErrNone)
   	{
		iRevGeocoderPlugin->AddObserverL(*this);
	 	}


#endif //LOC_REVERSEGEOCODE

    LOG("CInternalGeoTagger::ConstructL ,end");
    }
    
// --------------------------------------------------------------------------
// CInternalGeoTagger::~CInternalGeoTagger
// --------------------------------------------------------------------------
//    
EXPORT_C CInternalGeoTagger::~CInternalGeoTagger()
    {  
    LOG("CInternalGeoTagger::~CInternalGeoTagger ,begin");
#ifdef LOC_GEOTAGGING_CELLID
    if(iGeoConverter)
        {
        delete iGeoConverter;
        iGeoConverter = NULL;
        }
#endif    
	if(iRelationQuery)
		{
		iRelationQuery->RemoveObserver(*this);
		iRelationQuery->Cancel();
		delete iRelationQuery;
		iRelationQuery = NULL;
		}
	if(iLocationQuery)
		{
		iLocationQuery->RemoveObserver(*this);
		iLocationQuery->Cancel();
		delete iLocationQuery;
		iLocationQuery = NULL;
		}
	if(iTagQuery)
		{
		iTagQuery->RemoveObserver(*this);
		iTagQuery->Cancel();
		delete iTagQuery;
		iTagQuery = NULL;
		}	
#ifdef LOC_REVERSEGEOCODE
    delete iTagCreator;
    iTagCreator = NULL;
    delete iRevGeocoderPlugin;
    iRevGeocoderPlugin = NULL;

#endif //LOC_REVERSEGEOCODE
    if(iMdeSessionOwnFlag)
        {
    	delete iMdeSession;
    	iMdeSession = NULL; 
        delete iASW;
        iASW = NULL;
        }
    LOG("CInternalGeoTagger::~CInternalGeoTagger ,end");
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::CreateGeoTags
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::CreateGeoTagsL( const TConnectionOption aOption )
    {  
    LOG("CInternalGeoTagger::CreateGeoTagsL ,begin");
    if ( !IsSessionReady() )
        {
        User::Leave( KErrNotReady );
        }
    
    iConnectionOption = aOption;
    iIndex = 0;
    GetAllLocationsL();
    
    LOG("CInternalGeoTagger::CreateGeoTagsL ,end");
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::HandleSessionOpened
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::HandleSessionOpened(CMdESession& /*aSession*/, TInt aError)
    {
    LOG("CInternalGeoTagger::HandleSessionOpened ,begin");
    if ( iASW && iASW->IsStarted() )
        {
        iASW->AsyncStop();
        }
    
    if ( KErrNone == aError )
        {
        iSessionReady = ETrue;
        
#ifdef LOC_REVERSEGEOCODE
        iTagCreator->SetSession( iMdeSession );
#endif
        }
    else 
        {
        delete iMdeSession;
        iMdeSession = NULL;
        }
    LOG("CInternalGeoTagger::HandleSessionOpened ,end");
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::HandleSessionError
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::HandleSessionError(CMdESession& /*aSession*/, TInt /*aError*/)
    {
    LOG("CInternalGeoTagger::HandleSessionError ,begin");
    if (iASW && iASW->IsStarted() )
        {
        iASW->AsyncStop();
        }
    iSessionReady = EFalse;

	delete iMdeSession;
	iMdeSession = NULL;

    LOG("CInternalGeoTagger::HandleSessionError ,end");
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::IsSessionReady
// --------------------------------------------------------------------------
//
TBool CInternalGeoTagger::IsSessionReady()
    {
    return iSessionReady;
    }


// --------------------------------------------------------------------------
// CInternalGeoTagger::GetRelationObjectsL()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::GetRelationObjectsL(TItemId aLocID)
	{
    LOG("CInternalGeoTagger::GetRelationObjectsL ,begin");
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	CMdEObjectDef& imageObjDef = namespaceDef.GetObjectDefL( Image::KImageObject );
	
	if(iRelationQuery)
		{
		iRelationQuery->RemoveObserver(*this);
		iRelationQuery->Cancel();
		delete iRelationQuery;
    	iRelationQuery = NULL;
		}
    
    iRelationQuery = iMdeSession->NewRelationQueryL( namespaceDef, this );
    User::LeaveIfNull( iRelationQuery );

    iRelationQuery->SetResultMode( EQueryResultModeItem );
    
    // both left and right condition must match
    CMdERelationCondition& filterCond = iRelationQuery->Conditions().
    		AddRelationConditionL( ERelationConditionSideRight );

    // left one must be any image object.
    filterCond.LeftL().AddObjectConditionL( imageObjDef );

    // right one must be this location object 
    filterCond.RightL().AddObjectConditionL( aLocID );

    iRelationQuery->FindL(); // results to a call to HandleQueryCompleted()
    LOG("CInternalGeoTagger::GetRelationObjectsL ,end");
	}

// --------------------------------------------------------------------------
// CInternalGeoTagger::HandleQueryNewResults()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::HandleQueryNewResults(CMdEQuery& /*aQuery*/, TInt /*aFirstNewItemIndex*/, 
		TInt /*aNewItemCount*/)
	{
	LOG("CInternalGeoTagger::HandleQueryNewResults ,begin");
	}

// --------------------------------------------------------------------------
// CInternalGeoTagger::HandleQueryCompleted()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::HandleQueryCompleted(CMdEQuery& aQuery, TInt aError)
    {
    LOG("CInternalGeoTagger::HandleQueryCompleted ,begin");

    TInt count = aQuery.Count();
    LOG1("CInternalGeoTagger::HandleQueryCompleted count: %d", count);
        
    if ( &aQuery == iLocationQuery )
        {
        if ( count )
            {
            iLocationId = iLocationQuery->ResultId( iIndex++ );
            TRAP_IGNORE( GetRelationObjectsL( iLocationId ) );
            }
        else 
            {
            iIndex = 0;
            iObserver->GeoTaggingCompleted( aError );
            if(iTagPendingHandlerFlag)
                {
                iTagPendingHandlerFlag = EFalse;
                iObserver->PendingGeoTagReqComplete(KErrNone);
                }
            }  
        }
    else if ( &aQuery == iRelationQuery )
        {
        //if it is a relation query, then update gps info of location object.
     
        if ( iRelationQuery->Count() )
            {
            TInt err = KErrNone;
            TRAP(err, StartGeoTaggingL( ) );              
            if(err != KErrNone)
                {
                IterateNextLocation();
                }
            }
        else
            {
            IterateNextLocation();
            }
        
        }
		
    else if ( &aQuery == iTagQuery )
        {
                       
        if ( LocationTagExists() )  //nothing to do for this location
            {
            IterateNextLocation();  //Go through next location
            }
        else   //do reverse geocoding
            {          
            if(iTagPendingHandlerFlag)
                {
                iIndex = 0;
                iTagPendingHandlerFlag = EFalse;
                iObserver->PendingGeoTagReqComplete(KErrNotFound);
                return;
                }

            //Find the address for this geo-coordinate
            if(GPSInfoExists())
                {
                // GPS info exist go for reverse geo coding.
#ifdef LOC_REVERSEGEOCODE
                if(iRevGeocoderPlugin)
    			    {
                    TRAP_IGNORE( 
    				iRevGeocoderPlugin->GetAddressByCoordinateL
    				   ( iLocationData.iPosition, iConnectionOption ) );
    				}
    			else
#endif //LOC_REVERSEGEOCODE
    			    {
                    IterateNextLocation();  //Go through next location
    			    }
    			}
    		else
    		    {
        		if ( iLocationData.iNetworkInfo.iCellId > 0 && 
        				iLocationData.iNetworkInfo.iCountryCode.Length() > 0 &&
        				iLocationData.iNetworkInfo.iNetworkId.Length() > 0 )
        			{
        			// valid n/w information.. go for cell id based geo coding.
#ifdef LOC_GEOTAGGING_CELLID
                    TInt err = KErrNone;
                    if(iGeoConverter == NULL)
                       {
                       TRAP(err, iGeoConverter = CGeoConverter::NewL(*this));
                       }
                   
                    if(err == KErrNone && iGeoConverter != NULL)
                       {
                       // there is no field to store type of network. 
                       // but it's safe to take decission based on area code.
                       if(iLocationData.iNetworkInfo.iLocationAreaCode > 0)
                           {
                           LOG("Valid areacode. Treat as GSM n/w");
                           iLocationData.iNetworkInfo.iAccess = CTelephony::ENetworkAccessGsm;
                           }
                       else
                           {
                           LOG("Areacode is 0. Treat as 3G n/w");
                           iLocationData.iNetworkInfo.iAccess = CTelephony::ENetworkAccessUtran;
                           }
                       TRAP(err, iGeoConverter->ConvertL(iLocationData.iNetworkInfo);)
                       }
                    LOG1("Error - %d", err);
                   if(err != KErrNone)
                       {
                       HandleConversionError(err);	
                       }
#else
                    IterateNextLocation();  //Go through next location
#endif                    
        			}
        		else
        		    {
                    IterateNextLocation();  //Go through next location
        		    }
    		    }
            }
        
        }

    LOG("CInternalGeoTagger::HandleQueryCompleted ,end");
    }


// --------------------------------------------------------------------------
// CInternalGeoTagger::GPSInfoExists()
// --------------------------------------------------------------------------
//
TBool CInternalGeoTagger::GPSInfoExists()
    {
    LOG("CInternalGeoTagger::GPSInfoExists");
    TBool gpsInfoExists = EFalse;
    if ( !Math::IsNaN( iLocationData.iPosition.Latitude() ) && 
                !Math::IsNaN( iLocationData.iPosition.Longitude() ) &&
                KZeroLatLon != iLocationData.iPosition.Latitude() && 
                KZeroLatLon != iLocationData.iPosition.Longitude() )  //lat, long is there
        {
        
		LOG("Valid lat/lon\n");
        gpsInfoExists = ETrue;
        }
    return gpsInfoExists;
    }


// --------------------------------------------------------------------------
// CInternalGeoTagger::StartGeoTagging()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::StartGeoTaggingL( )
    {       
    LOG("CInternalGeoTagger::StartGeoTaggingL ,start");
    GetLocationInfoL();
    
    CMdERelation& relation = iRelationQuery->Result( 0 );
    
    GetTagsL( relation.LeftObjectId() );

	LOG("CInternalGeoTagger::StartGeoTaggingL ,end");
    }
// --------------------------------------------------------------------------
// CInternalGeoTagger::GetLocationInfoL()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::GetLocationInfoL()
    {
    LOG("CInternalGeoTagger::GetLocationInfoL, begin");

    CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();

    CMdEObjectDef& locationObjectDef = namespaceDef.GetObjectDefL( Location::KLocationObject );
    
    CMdEPropertyDef& latitudeDef = locationObjectDef.GetPropertyDefL( 
                                        Location::KLatitudeProperty );
    CMdEPropertyDef& longitudeDef = locationObjectDef.GetPropertyDefL( 
                                        Location::KLongitudeProperty );
    CMdEPropertyDef& cellIdDef = locationObjectDef.GetPropertyDefL( 
                                        Location::KCellIdProperty );
    CMdEPropertyDef& lacCodeDef = locationObjectDef.GetPropertyDefL( 
                                        Location::KLocationAreaCodeProperty );
    CMdEPropertyDef& countryCodeDef = locationObjectDef.GetPropertyDefL( 
                                        Location::KCountryCodeProperty );
    CMdEPropertyDef& networkCodeDef = locationObjectDef.GetPropertyDefL( 
                                        Location::KNetworkCodeProperty );
    
    CMdEObject* locationObject = NULL;
    CMdEProperty* latProp = NULL;
    CMdEProperty* lonProp = NULL;
    CMdEProperty* cellProp = NULL;
    CMdEProperty* lacProp = NULL;
    CMdEProperty* countryProp = NULL;
    CMdEProperty* networkProp = NULL;
       
    locationObject = iMdeSession->GetObjectL( iLocationId, locationObjectDef );
    
    locationObject->Property( latitudeDef, latProp, 0 );
    locationObject->Property( longitudeDef, lonProp, 0 );
    if ( latProp && lonProp )
        {
        iLocationData.iPosition.SetCoordinate( latProp->Real64ValueL(), 
                                               lonProp->Real64ValueL() );
        }
    else
        {
        iLocationData.iPosition.SetCoordinate( KZeroLatLon, 
                                               KZeroLatLon );
        }
    
    locationObject->Property( cellIdDef, cellProp, 0 );
    locationObject->Property( lacCodeDef, lacProp, 0 );
    locationObject->Property( countryCodeDef, countryProp, 0 );
    locationObject->Property( networkCodeDef, networkProp, 0 );
    if ( cellProp && countryProp && networkProp)
        {
        iLocationData.iNetworkInfo.iCellId = cellProp->Uint32ValueL();
        iLocationData.iNetworkInfo.iCountryCode = countryProp->TextValueL();
        iLocationData.iNetworkInfo.iNetworkId = networkProp->TextValueL();
        }
    else
        {
        // set to invalid values.
        iLocationData.iNetworkInfo.iCellId = 0;
        iLocationData.iNetworkInfo.iAccess = CTelephony::ENetworkAccessUnknown;
        iLocationData.iNetworkInfo.iLocationAreaCode = 0;
        iLocationData.iNetworkInfo.iAreaKnown = EFalse;
        iLocationData.iNetworkInfo.iCountryCode.Zero();
        iLocationData.iNetworkInfo.iNetworkId.Zero();
        }

    iLocationData.iNetworkInfo.iAreaKnown = EFalse;
    if(lacProp)
        {
        iLocationData.iNetworkInfo.iLocationAreaCode = lacProp->Uint32ValueL();
        if(iLocationData.iNetworkInfo.iLocationAreaCode > 0)
            {
            iLocationData.iNetworkInfo.iAreaKnown = ETrue;
            }
        }
    delete locationObject;
	LOG("CInternalGeoTagger::GetLocationInfoL ,end");
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::LocationTagExists()
// --------------------------------------------------------------------------
//
TBool CInternalGeoTagger::LocationTagExists()
    {
    LOG("CInternalGeoTagger::LocationTagExists ,begin");
                           
    TBool tagFound = EFalse;
    TInt error = KErrNone;
    const TInt count = iTagQuery->Count();
                
    for ( TInt i = 0; i < count; i++ )
        {
        CMdERelation& relation =  static_cast<CMdERelation&>( iTagQuery->ResultItem( i ) );
                    
        TItemId tagId = relation.RightObjectId(); 
        CMdEObject* object = NULL;
        TRAP_IGNORE( object = iMdeSession->GetObjectL( tagId ) );
        if(object)
            {
            error = object->Uri().Find( KCountry );
            delete object;
            if ( error == KErrNone )
                {
                i = count;
                tagFound = ETrue;
                break;
                }
            }
        }
                
           
	LOG("CInternalGeoTagger::LocationTagExists ,end");
    return tagFound;

    }


// --------------------------------------------------------------------------
// CInternalGeoTagger::IterateNextLocation()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::IterateNextLocation()
    {
    LOG("CInternalGeoTagger::IterateNextLocation ,begin");
                                   
    if (iIndex < iLocationQuery->Count())
        {
        iLocationId = iLocationQuery->ResultId( iIndex++ );
        TRAP_IGNORE( GetRelationObjectsL( iLocationId ) );
        }
    else
        {
        if ( iObserver )
            {
            iIndex = 0;
            TInt error = KErrNone;
            iObserver->GeoTaggingCompleted( error );
            if(iTagPendingHandlerFlag)
                {
                iTagPendingHandlerFlag = EFalse;
                iObserver->PendingGeoTagReqComplete(KErrNone);
                }
            }
        }
	LOG("CInternalGeoTagger::IterateNextLocation ,end");
    }

// --------------------------------------------------------------------------
// CInternalGeoTagger::GetAllLocationsL()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::GetAllLocationsL()
    {
    LOG("CInternalGeoTagger::GetAllLocationsL ,begin");

    CMdENamespaceDef& defaultNamespaceDef = iMdeSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& locObjDef = defaultNamespaceDef.GetObjectDefL( 
                               MdeConstants::Location::KLocationObject );
    CMdEPropertyDef& cellIdPropDef = locObjDef.GetPropertyDefL( 
                               MdeConstants::Location::KCellIdProperty );

	if(iLocationQuery)
		{
		iLocationQuery->RemoveObserver(*this);
		iLocationQuery->Cancel();
		delete iLocationQuery;
		iLocationQuery = NULL;
		}
    // query objects with object definition "Image"
    iLocationQuery = iMdeSession->NewObjectQueryL( defaultNamespaceDef, locObjDef, this );
    
    //Define the result order
    iLocationQuery->AppendOrderRuleL(TMdEOrderRule(cellIdPropDef, ESortAscending));
     
    iLocationQuery->SetResultMode( EQueryResultModeId );
    iLocationQuery->FindL();

  LOG("CInternalGeoTagger::GetAllLocationsL ,end");
    }


// --------------------------------------------------------------------------
// CInternalGeoTagger::GetTagsL()
// Get tags related to this image/video object
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::GetTagsL( TItemId aImageID )
    {
    LOG("CInternalGeoTagger::GetTagsL ,begin");
    CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& tagObjectDef = namespaceDef.GetObjectDefL( Tag::KTagObject );
        
	if(iTagQuery)
		{
		iTagQuery->RemoveObserver(*this);
		iTagQuery->Cancel();
		delete iTagQuery;
        iTagQuery = NULL;
		}	
        
    iTagQuery = iMdeSession->NewRelationQueryL( namespaceDef, this );
    User::LeaveIfNull( iTagQuery );

    iTagQuery->SetResultMode( EQueryResultModeItem );
        
    // both left and right condition must match
    CMdERelationCondition& filterCond = iTagQuery->Conditions().
    AddRelationConditionL( ERelationConditionSideRight );

    // left one must be this image object.
    filterCond.LeftL().AddObjectConditionL( aImageID );

    // right one must be tag object 
    filterCond.RightL().AddObjectConditionL( tagObjectDef );

    iTagQuery->FindL(); // results to a call to HandleQueryCompleted()
    LOG("CInternalGeoTagger::GetTagsL ,end");
    }

#ifdef LOC_GEOTAGGING_CELLID	

// --------------------------------------------------------------------------
// CInternalGeoTagger::UpdateGPSInfoL()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::UpdateGPSInfoL(const TLocality& aPosition)
	{
	
    LOG("CInternalGeoTagger::UpdateGPSInfoL ,start");
    if(Math::IsNaN( aPosition.Latitude()) ||
        Math::IsNaN( aPosition.Longitude()) )
        {
        LOG("Not a valid location info.");
        LOG("CInternalGeoTagger::UpdateGPSInfoL ,end");
        return;
        }
    
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	CMdEObjectDef& locObjDef = namespaceDef.GetObjectDefL( Location::KLocationObject );
	CMdEObject* location = iMdeSession->OpenObjectL(iLocationId, locObjDef);
	CleanupStack::PushL( location );
	
	CMdEPropertyDef& propLatDef = locObjDef.GetPropertyDefL( Location::KLatitudeProperty );
	CMdEPropertyDef& propLongDef = locObjDef.GetPropertyDefL( Location::KLongitudeProperty );
	CMdEPropertyDef& propAltDef = locObjDef.GetPropertyDefL( Location::KAltitudeProperty );
	CMdEPropertyDef& qualityDef = locObjDef.GetPropertyDefL( Location::KQualityProperty );

	if (location->PropertyCount(propLatDef) == 0)
		{
		location->AddReal64PropertyL(propLatDef, aPosition.Latitude() ); //iLatitude
		}
	if (location->PropertyCount(propLongDef) == 0)
		{
		location->AddReal64PropertyL(propLongDef, aPosition.Longitude() ); //iLongitude
		}
	if (location->PropertyCount(propAltDef) == 0 &&
        !Math::IsNaN( aPosition.Altitude()) )
		{
		location->AddReal64PropertyL(propAltDef, aPosition.Altitude());
		}
    LOG1( "Updating quality - %d", aPosition.HorizontalAccuracy());
	if (location->PropertyCount(qualityDef) == 0 )
		{
		location->AddReal32PropertyL(qualityDef, aPosition.HorizontalAccuracy());
		}
	
	CMdEProperty* modProp = NULL;
	CMdEObjectDef& objImageDef = namespaceDef.GetObjectDefL( Image::KImageObject );
	CMdEPropertyDef& propModifiedDef = objImageDef.GetPropertyDefL( Object::KLastModifiedDateProperty );
	location->Property( propModifiedDef, modProp, 0 );
	if ( modProp )
		{
		TTime timestamp( 0 );
		timestamp.UniversalTime();
		modProp->SetTimeValueL( timestamp );
		}
	// commit to DB
	iMdeSession->CommitObjectL(*location);
	CleanupStack::PopAndDestroy( location );

    // update the relation
    const TInt count = iRelationQuery->Count();
    LOG1("Relation count - %d", count);
    for ( TInt i = 0; i < count; i++ )
        {
        CMdERelation& relation = iRelationQuery->Result( i );
        // update relation timestamp, composer will then update exif data   
        
        TTime timestamp( 0 );
        timestamp.UniversalTime();
        relation.SetLastModifiedDate( timestamp );     
        iMdeSession->UpdateRelationL( relation );
        }
    LOG("CInternalGeoTagger::UpdateGPSInfoL ,end");
	}

// --------------------------------------------------------------------------
// CInternalGeoTagger::ConversionCompletedL()
// --------------------------------------------------------------------------
//

void CInternalGeoTagger::ConversionCompletedL( const TInt aError, 
                    TLocality& aPosition )
    {
    LOG1("CInternalGeoTagger::ConversionCompletedL, begin. Error - %d", aError);
    if(aError == KErrNone)
        {
        iLocationData.iPosition.SetCoordinate
                ( aPosition.Latitude(), aPosition.Longitude(), aPosition.Altitude());
        iLocationData.iQuality = aPosition.HorizontalAccuracy();
        UpdateGPSInfoL(aPosition);
#ifdef LOC_REVERSEGEOCODE
        if(iRevGeocoderPlugin)
		    {
           	iRevGeocoderPlugin->GetAddressByCoordinateL
			   ( iLocationData.iPosition, iConnectionOption ) ;
			}
		else
#endif //LOC_REVERSEGEOCODE
		    {
            IterateNextLocation();  //Go through next location
		    }
       }
	else
	    {
        IterateNextLocation();  //Go through next location
	    }
    LOG("CInternalGeoTagger::ConversionCompletedL, end");
    }


//-----------------------------------------------------------------
// CInternalGeoTagger::HandleConversionError
//------------------------------------------------------------------
//
void CInternalGeoTagger::HandleConversionError(TInt aError)
	{
    LOG("CInternalGeoTagger::HandleConversionError, begin");
    ARG_USED(aError);
    IterateNextLocation();  //Go through next location
    LOG("CInternalGeoTagger::HandleConversionError, end");
	}

#endif //LOC_GEOTAGGING_CELLID

// --------------------------------------------------------------------------
// CInternalGeoTagger::PendingGeoTagsL()
// Is there any untagged photos with location info?.
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::PendingGeoTagsL( TBool aTagInProgress )
    {
    LOG("CInternalGeoTagger::PendingGeoTagsL ,begin");
    iIndex = 0;
    if ( aTagInProgress )
        {
        //Tagging is going through location trail, so photo does not need to do tagging now
        LOG("Geo tagging in progress.\n");
        iObserver->PendingGeoTagReqComplete( KErrInUse );
        }
    else
        {
        if(!iTagPendingHandlerFlag && iIndex <= 0)
            {
            LOG("Processing the request.\n");
            // tag pending request is not going on
            iTagPendingHandlerFlag = ETrue;
            CreateGeoTagsL();
            }
        else
            {
            // req osn progress, don't do anything.
            LOG("Request in progress.\n");
            iObserver->PendingGeoTagReqComplete( KErrInUse );
            }
        }
    LOG("CInternalGeoTagger::PendingGeoTagsL ,end");
    }


#ifdef LOC_REVERSEGEOCODE
// --------------------------------------------------------------------------
// CInternalGeoTagger::AddressInfo()
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::AddressInfoL( const TItemId aCountryTagId, const TItemId aCityTagId )
    {
    LOG("CInternalGeoTagger::AddressInfoL ,begin");
                               
    const TInt count = iRelationQuery->Count();
    
    for ( TInt i = 0; i < count; i++ )
        {
        CMdERelation& relation = iRelationQuery->Result( i );
        TItemId imageId = relation.LeftObjectId(); 
        
        if ( !GPSInfoExists() )   //to update EXIF
            {
            // update relation timestamp, composer will then update exif data   
            TTime timestamp( 0 );
            timestamp.UniversalTime();
            relation.SetLastModifiedDate( timestamp );     
            iMdeSession->UpdateRelationL( relation );
            }
        
        iTagCreator->AttachTagsL( imageId, aCountryTagId, aCityTagId  );
        }

   LOG("CInternalGeoTagger::AddressInfoL ,end");
    }


// --------------------------------------------------------------------------
// CInternalGeoTagger::ReverseGeocodeComplete()
//  Get address details like street, city, state, etc.
// --------------------------------------------------------------------------
//
void CInternalGeoTagger::ReverseGeocodeComplete( TInt& aErrorcode, MAddressInfo& aAddressInfo )
    {
    LOG("CInternalGeoTagger::ReverseGeocodeComplete ,begin");
    TItemId countryTagId(0);
    TItemId cityTagId(0);
    
    // create country and city tags
    if( aErrorcode == KErrNone )
        {
        TPtrC countryPtr( aAddressInfo.GetCountryName() ); 
        TPtrC cityPtr( aAddressInfo.GetCity() );
        
        TRAP_IGNORE( iTagCreator->CreateLocationTagsL( countryPtr, countryTagId, cityPtr, cityTagId ) );
		
        TRAP_IGNORE(AddressInfoL( countryTagId, cityTagId ));
        
        IterateNextLocation();
        }
    else
        {
        LOG1("Reverse geocode err - %d", aErrorcode);

        IterateNextLocation();
        }
   
    
    LOG("CInternalGeoTagger::ReverseGeocodeComplete ,end");
    }

// ----------------------------------------------------------------------------
// CInternalGeoTagger::GetCurrentRegisterNw()
// ---------------------------------------------------------------------------- 
RMobilePhone::TMobilePhoneNetworkInfoV2& CInternalGeoTagger::GetCurrentRegisterNw()
    {
    LOG( "CInternalGeoTagger::GetCurrentRegisterNw ,begin" );
    return iObserver->GetCurrentRegisterNw();
    }

// ----------------------------------------------------------------------------
// CInternalGeoTagger::IsRegisteredAtHomeNetwork()
// ---------------------------------------------------------------------------- 
TBool CInternalGeoTagger::IsRegisteredAtHomeNetwork()
    {
    LOG( "CInternalGeoTagger::IsRegisteredAtHomeNetwork" );
    return iObserver->IsRegisteredAtHomeNetwork();
    }

// ----------------------------------------------------------------------------
// CInternalGeoTagger::GetHomeNetworkInfo()
// ----------------------------------------------------------------------------
const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        CInternalGeoTagger::GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag)
    {
    LOG( "CInternalReverseGeocode::GetHomeNetworkInfo" );
    return iObserver->GetHomeNetworkInfo(aHomeNwInfoAvailableFlag);
    }

#endif //LOC_REVERSEGEOCODE

// End of file

