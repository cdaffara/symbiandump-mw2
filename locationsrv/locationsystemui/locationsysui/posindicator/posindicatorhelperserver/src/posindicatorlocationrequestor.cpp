/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of location requestor class.
*
*/

#include "posindicatorlocationrequestor.h"
#include "posindicatorserverconst.h"
#include "posindicatorlogger.h"

#include <epos_poslandmarkserialization.h>
#include <ecom.h>

_LIT( KServer,"PositionIndicatorServer" );
static const TInt KMaxUpdateAge = 10;
static const TInt KUpdateTimeOut = 5000000;

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::NewL
// -----------------------------------------------------------------------------
CPosIndicatorLocationRequestor* CPosIndicatorLocationRequestor::NewL()
    {
    FUNC("CPosIndicatorLocationRequestor::NewL");
    CPosIndicatorLocationRequestor* self = new ( ELeave ) 
                                        CPosIndicatorLocationRequestor;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::~CPosIndicatorLocationRequestor
// -----------------------------------------------------------------------------
CPosIndicatorLocationRequestor::~CPosIndicatorLocationRequestor()
    {
    FUNC("CPosIndicatorLocationRequestor::~CPosIndicatorLocationRequestor");
    
    // Cancel the current operation.
    Cancel();
    
    iPositionRequestQueue.Close();
    iAddressRequestQueue.Close();
    iPositioner.Close();
    iPosServer.Close();    
    delete iReverseGeoCodeInterface;
    iReverseGeoCodeInterface = NULL;
    delete iLandmark;
    iLandmark = NULL;
    delete iLandmarkPackage;
    iLandmarkPackage = NULL;
    // Release all landmark resources
    ReleaseLandmarkResources();
    
    // Release all ECOM resources
    REComSession::FinalClose();	
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::CPosIndicatorLocationRequestor
// -----------------------------------------------------------------------------
CPosIndicatorLocationRequestor::CPosIndicatorLocationRequestor():CActive( 
                                                              EPriorityStandard )
    {
    FUNC("CPosIndicatorLocationRequestor::CPosIndicatorLocationRequestor");
    CActiveScheduler::Add( this );
    }
 
// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::ConstructL
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::ConstructL()
    {
    FUNC("CPosIndicatorLocationRequestor::ConstructL");
    iOperation = ENone;
    
    User::LeaveIfError( iPosServer.Connect() );
    
    TInt error = iPositioner.Open( iPosServer );
    
    User::LeaveIfError( iPositioner.SetRequestor( 
                        CRequestor::ERequestorService,
                        CRequestor::EFormatApplication,
                        KServer ) );

    TPositionUpdateOptions updateops;
    updateops.SetUpdateTimeOut(TTimeIntervalMicroSeconds(KUpdateTimeOut));
    User::LeaveIfError( iPositioner.SetUpdateOptions( updateops ));
 
    iReverseGeoCodeInterface = CPosReverseGeocodeInterface::NewL( *this );
    
    iLandmark = CPosLandmark::NewL();
    
    iLandmarkPackage = NULL;
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::GetCurrentPosition
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::GetCurrentPosition( const RMessage2&
                                                         aMessage )
    { 
    FUNC("CPosIndicatorLocationRequestor::GetCurrentPosition");
    // If location fix is already available and this is waiting for address info,
    // reply the message with the fix available
    if( iOperation == ECurrentAddress )
        {
        FUNC("iOperation == ECurrentAddress");
        TPckg<TPositionSatelliteInfo> posSatPckg( iPositionInfo );
        TInt error = aMessage.Write( KParamCurrentPosition,posSatPckg );
        aMessage.Complete( error );
        }
    else 
        {
        FUNC("iOperation != ECurrentAddress");
        // Store client's message
        iPositionRequestQueue.Append( aMessage );   
        // If the requestor is idle without doing any operation, request
        // current location.
        if( iOperation == ENone )
            {
            FUNC("iOperation == ENone");
            iOperation = ELastKnownPosition;
            
            // Last known position.
            iPositioner.GetLastKnownPosition( iLastKnownInfo,iStatus );
            SetActive();
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::GetCurrentAddressInfoSizeL
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::GetCurrentAddressInfoSizeL( const RMessage2& 
                                                                 aMessage )
    {
    FUNC("CPosIndicatorLocationRequestor::GetCurrentAddressInfoSizeL");
    // Store client's message
    iAddressRequestQueue.Append( aMessage );
    
    // If the requestor is idle without doing any operation, request
    // address information.
    if( iOperation == ENone )
        {
        iOperation = ECurrentAddress;
        // Clear previous landmark information.
        iLandmark->RemoveLandmarkAttributes( CPosLandmark::EAllAttributes );
        TPosition position;
        iPositionInfo.GetPosition( position );
        iLandmark->SetPositionL( position );
        iReverseGeoCodeInterface->GetAddressByCoordinateL( *iLandmark );
        }
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::GetCurrentAddressInfoL
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::GetCurrentAddressInfoL( const RMessage2& 
                                                             aMessage )
    {
    FUNC("CPosIndicatorLocationRequestor::GetCurrentAddressInfoL");
    TInt error = aMessage.Write( KParamCurrentAddress,*iLandmarkPackage );
    aMessage.Complete( error );
    }


// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::RunL
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::RunL()
    {
    FUNC("CPosIndicatorLocationRequestor::RunL");
    LOG("Error Status : ", iStatus.Int());
    switch( iOperation )
        {
        case ELastKnownPosition:
            {
                // Check the time stamp of the last fix, if the fix is 10 sec
                // old, request for the new fix.
                TPosition position;
                iLastKnownInfo.GetPosition( position );
                TTime timeOfFix = position.Time();
                TTime currentTime;
                currentTime.HomeTime();
                timeOfFix += TTimeIntervalSeconds( KMaxUpdateAge );
                if( iStatus.Int() == KErrNone && timeOfFix < currentTime && 
                	!(Math::IsNaN( position.Latitude() ) || Math::IsNaN( position.Longitude() ) ) )
                    {
                    // Set the last knowm position to position info to send it to
                    // client.
                    iPositionInfo.SetPosition( position );
                    }
                else
                    {
                    iOperation = ECurrentPosition;
                 
                    // Request current position.
                    iPositioner.NotifyPositionUpdate( iPositionInfo,iStatus );
                    SetActive();
                    return;
                    }
            	}
        case ECurrentPosition:
            {
            // Reset operation id
            iOperation = ENone;
            // Pack the satellite position information 
            TPckg<TPositionSatelliteInfo> posSatPckg( iPositionInfo );
            TInt count = iPositionRequestQueue.Count();
            for( TInt i=0;i<count;++i )
                {
                TInt error = iStatus.Int();
                if( error == KErrNone )
                    {
                    error = iPositionRequestQueue[i].Write( KParamCurrentPosition,posSatPckg );
                    }
                iPositionRequestQueue[i].Complete( error );
                }
            // All the messages in the position request queue are completed. Hence reset the list.
            iPositionRequestQueue.Reset();
            break;
            }
        case ECurrentAddress:
            {
            FUNC("ECurrentAddress");
            // Reset operation id
            iOperation = ENone;
            // Pack landmarks information
            delete iLandmarkPackage;
			iLandmarkPackage = NULL;
            iLandmarkPackage = PosLandmarkSerialization::PackL( *iLandmark );
            TInt count = iAddressRequestQueue.Count();
            for( TInt i=0;i<count;i++ )
                {
                TInt error = iStatus.Int();
                if( error == KErrNone )
                    {
                    TPckg<TInt> size( iLandmarkPackage->Size() );
                    error = iAddressRequestQueue[i].Write( KParamCurrentAddressSize,size );
                    }
                iAddressRequestQueue[i].Complete( error );
                }
            // All the messages in the address request queue are completed. Hence reset the list.
            iAddressRequestQueue.Reset();
            break;
            }

        default:
            {
            FUNC("- CPosIndicatorLocationRequestor::RunL default case");
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::CancelRequest
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::CancelRequest( const RMessage2& aMessage )
    {
    // Iterate through the outstanding request and check whether there are 
    // outstanding request from the subsession from which cancel call is 
    // received.
    RMessage2 outStandingMessage;
    TBool outStandingFlag = EFalse;
    TInt count = iPositionRequestQueue.Count();
    for( TInt i=0;i<count;++i )
        {
        if( iPositionRequestQueue[i].SecureId() == aMessage.SecureId() &&
            iPositionRequestQueue[i].Int3() == aMessage.Int3() )
            {
            outStandingMessage = iPositionRequestQueue[i];
            iPositionRequestQueue.Remove(i);
            outStandingFlag = ETrue;
            break;
            }
        }
    
    // If the outstanding request is not found in the position request queue,
    // search address request queue.
    if( !outStandingFlag )
        {   
        count = iAddressRequestQueue.Count();
        for( TInt i=0;i<count;++i )
            {
            if( iAddressRequestQueue[i].SecureId() == aMessage.SecureId() &&
                iAddressRequestQueue[i].Int3() == aMessage.Int3() )
                {
                outStandingMessage = iAddressRequestQueue[i];
                iAddressRequestQueue.Remove(i);
                outStandingFlag = ETrue;
                break;
                }
            }
        }
    
    // If the both position and address request queue are zero, cancel outstanding request
    if( !iAddressRequestQueue.Count() &&
        !iPositionRequestQueue.Count() )
        {
        Cancel();
        }
    if( outStandingFlag )
        {
        outStandingMessage.Complete( KErrCancel );
        }
    // Complete cancel request.
    aMessage.Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::DoCancel
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::DoCancel()
    {
    FUNC("CPosIndicatorLocationRequestor::DoCancel");
    switch( iOperation )
        {
        case ELastKnownPosition:
            {
            iPositioner.CancelRequest( RPositionerSubSessionBase::EReqGetLastKnownPosition );
            break;
            }
        case ECurrentPosition:
            {
            iPositioner.CancelRequest( RPositionerSubSessionBase::EReqNotifyPositionUpdate );
            break;
            }    
        case ECurrentAddress:    
            {
            iReverseGeoCodeInterface->Cancel();
            break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosIndicatorLocationRequestor::ReverseGeocodeComplete
// -----------------------------------------------------------------------------
void CPosIndicatorLocationRequestor::ReverseGeocodeComplete( TInt aErrorcode )
    {
    FUNC("CPosIndicatorLocationRequestor::ReverseGeocodeComplete");
    // Here we self complete our active to release the control from this 
    // call back method.
    iStatus = KRequestPending;
    SetActive();
    TRequestStatus* statusPtr = &iStatus;
    User::RequestComplete( statusPtr,aErrorcode );
    }

// End of file.
