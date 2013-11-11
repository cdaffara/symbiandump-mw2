/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements geotagging 3AM timer
*
*/

#include <hwrmpowerstatesdkpskeys.h>
#include "clocationgeotagtimerao.h"
#include "locationmanagerdebug.h"

//Time at which the geotagging should be triggered( 3.00 AM )
const TInt GEOTAGGING_TIME_IN_HOURS = 3;
//Hour specified in minutes
const TInt HOUR_VALUE_IN_MINUTES = 60;
//Hour specified in seconds
const TInt HOUR_VALUE_IN_SECONDS = 3600;

// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::CLocationGeoTagTimerAO
// --------------------------------------------------------------------------
//
CLocationGeoTagTimerAO::CLocationGeoTagTimerAO(CMdESession& aMdeSession,
        MGeoTaggerObserver& aObserver):
            CTimer( EPriorityLow ),
            iGeoTagger(NULL),
            iMdeSession(aMdeSession),
            iObserver(aObserver)
{

}

// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::~CLocationGeoTagTimerAO
// --------------------------------------------------------------------------
//
CLocationGeoTagTimerAO::~CLocationGeoTagTimerAO()
    {
    LOG ("CLocationGeoTagTimerAO::~CLocationGeoTagTimerAO(), begin");
    if(iGeoTagger)
        {
        delete iGeoTagger;
        iGeoTagger = NULL;
        }
    LOG ("CLocationGeoTagTimerAO::~CLocationGeoTagTimerAO(), end");
    }

// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::NewL
// --------------------------------------------------------------------------
//
CLocationGeoTagTimerAO* CLocationGeoTagTimerAO::NewL(CMdESession& aMdeSession,
                                MGeoTaggerObserver& aObserver)
    {
    CLocationGeoTagTimerAO* self = 
            new( ELeave ) CLocationGeoTagTimerAO(aMdeSession, aObserver);
       CleanupStack::PushL( self );
       self->ConstructL();
       CleanupStack::Pop(); //self
       
       return self;
    }


// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::ConstructL
// --------------------------------------------------------------------------
//
void CLocationGeoTagTimerAO::ConstructL()
    {
    LOG ("CLocationGeoTagTimerAO::ConstructL(), begin");
	CActiveScheduler::Add(this);
    CTimer::ConstructL();
    LOG ("CLocationGeoTagTimerAO::ConstructL(), end");
    }

// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::StartTimer
// --------------------------------------------------------------------------
//
void CLocationGeoTagTimerAO::StartTimer()
    {
    LOG ("CLocationGeoTagTimerAO::StartTimer(), begin");
    if(!IsActive())
        {
        TTime hometime;
        hometime.HomeTime();
        
        //Get the current time in Hour,Minute, Second
        TDateTime  currentDateTime = hometime.DateTime();
        TInt currentHr = currentDateTime.Hour(); 
        TInt currentMin = currentDateTime.Minute();
        TInt currentSec = currentDateTime.Second();
        
        //3 AM in seconds
        TInt targetTimeInSeconds = GEOTAGGING_TIME_IN_HOURS * HOUR_VALUE_IN_SECONDS;
        TInt timeDifference = 0;
        
        //Find the time difference in seconds between current time to 3.00 AM 
        //Either on same day or next day.
        if ( currentHr < GEOTAGGING_TIME_IN_HOURS )
        {
           timeDifference = targetTimeInSeconds - 
                    ( ( currentHr * HOUR_VALUE_IN_SECONDS  ) + ( currentMin * HOUR_VALUE_IN_MINUTES ) + currentSec );
        }
        else
        {
           timeDifference = ( 24 * HOUR_VALUE_IN_SECONDS - ( 
                    ( currentHr * HOUR_VALUE_IN_SECONDS ) + ( currentMin * HOUR_VALUE_IN_MINUTES ) + currentSec ) )  +
                    targetTimeInSeconds ;
        }
        
        //Add the time difference to current time to set the target time ( 3.00 AM )
        TTimeIntervalSeconds interval( timeDifference );
        TTime timeToSet;
        timeToSet.HomeTime();
        timeToSet+= interval;
        
        
         At( timeToSet );
        }
     LOG ("CLocationGeoTagTimerAO::StartTimer(), end");
     }


// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::RunL
// --------------------------------------------------------------------------
//
void CLocationGeoTagTimerAO::RunL( )
    {
    LOG ("CLocationGeoTagTimerAO::RunL(), begin");
    TInt status = iStatus.Int();
    LOG1 ("Timedout error - %d", status);
   
    switch( status )
        {
        case KErrNone:
            {
            // If battery is low, skip geotagging in the background
            // to save power especially if there is a lot of images
            // to be handled
            if( IsLowBattery() )
                {
                StartTimer();
                break;
                }
            //Trigger the reverse geocoding and start the timer again
            //Create the instance of geotagger class
            if(iGeoTagger != NULL)
                {
                delete iGeoTagger;
                iGeoTagger = NULL;
                }
            iGeoTagger = CGeoTagger::NewL( this, NULL );
            iGeoTagger->CreateGeoTagsL();
            LOG ("Started 3:00 AM geotagging.");
            break;
            }
        default:
            StartTimer();
            break;      
       }
    LOG ("CLocationGeoTagTimerAO::RunL(), end");
    }
    
// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::GeoTaggingCompleted
// --------------------------------------------------------------------------
//
void CLocationGeoTagTimerAO::GeoTaggingCompleted(  const TInt aError )
    {    
    LOG ("CLocationGeoTagTimerAO::GeoTaggingCompleted(), begin");
    StartTimer();
    iObserver.GeoTaggingCompleted(aError);
    LOG ("CLocationGeoTagTimerAO::GeoTaggingCompleted(), end");
    }

// --------------------------------------------------------------------------
// CLocationGeoTagTimerAO::PendingGeoTagReqComplete
// --------------------------------------------------------------------------
//
void CLocationGeoTagTimerAO::PendingGeoTagReqComplete(  const TInt aError )
    {    
    LOG ("CLocationGeoTagTimerAO::PendingGeoTagReqComplete()");
    // do nothing.
    iObserver.PendingGeoTagReqComplete(aError);
    }

// ----------------------------------------------------------------------------
// CLocationGeoTagTimerAO::GetCurrentRegisterNw()
// ---------------------------------------------------------------------------- 
RMobilePhone::TMobilePhoneNetworkInfoV2& CLocationGeoTagTimerAO::GetCurrentRegisterNw()
    {
    LOG( "CLocationGeoTagTimerAO::GetCurrentRegisterNw ,begin" );
    return iObserver.GetCurrentRegisterNw();
    }


// ----------------------------------------------------------------------------
// CLocationGeoTagTimerAO::IsRegisteredAtHomeNetwork()
// ---------------------------------------------------------------------------- 
TBool CLocationGeoTagTimerAO::IsRegisteredAtHomeNetwork()
    {
    LOG( "CLocationGeoTagTimerAO::IsRegisteredAtHomeNetwork" );
    return iObserver.IsRegisteredAtHomeNetwork();
    }

// ----------------------------------------------------------------------------
// CLocationGeoTagTimerAO::GetHomeNetworkInfo()
// ----------------------------------------------------------------------------
const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        CLocationGeoTagTimerAO::GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag)
    {
    LOG( "CLocationGeoTagTimerAO::GetHomeNetworkInfo" );
    return iObserver.GetHomeNetworkInfo(aHomeNwInfoAvailableFlag);
    }

// ----------------------------------------------------------------------------
// CLocationGeoTagTimerAO::IsLowBattery()
// ----------------------------------------------------------------------------
TBool CLocationGeoTagTimerAO::IsLowBattery()
    {
    LOG("CLocationGeoTagTimerAO::IsLowBattery()");
    RProperty batteryProperty;
    TInt batteryStatus;

    TInt error = batteryProperty.Get(KPSUidHWRMPowerState, KHWRMBatteryStatus, batteryStatus);
    LOG1("CLocationGeoTagTimerAO::IsLowBattery() - battery status %d", batteryStatus );
    if( error != KErrNone || batteryStatus == EBatteryStatusOk )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

// End of file

