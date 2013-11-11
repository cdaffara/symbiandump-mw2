/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpPositionRequest class implementation
*
*/


#include <aknwaitdialog.h>
#include <coemain.h>
#include <aknnotewrappers.h>

#include <lbspositioninfo.h>

#include "mnrppositionrequest.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpPositionRequest::CMnrpPositionRequest(
    MMnrpPositionObserver& aObserver )
    : 
    CActive( CActive::EPriorityStandard ), 
    iObserver( aObserver ),
    iState( EIdle )
    {
    TRealX nan;
    nan.SetNaN();
    iSpeed = iHeading = iCourse = iMagneticHeading = iMagneticCourse = nan;
    
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpPositionRequest::ConstructL( const TDesC& aRequestorName )
    {
    const TInt KBufSize = 8192;
    iGenericPosition = HPositionGenericInfo::NewL( KBufSize );
    
    // Connect to the location server
    User::LeaveIfError( iLocationServer.Connect() );

    // Open the default positioner
    User::LeaveIfError( iPositioner.Open( iLocationServer ) );

    // Set this application as location requestor
    User::LeaveIfError( iPositioner.SetRequestor(
        CRequestor::ERequestorService,
        CRequestor::EFormatApplication,
        aRequestorName ) );

    // Set default options
    TPositionUpdateOptions updateOptions;

    const TInt K30Seconds = 30000000;
    TTimeIntervalMicroSeconds timeOut( K30Seconds );

    updateOptions.SetUpdateTimeOut( timeOut );
    SetOptionsL( updateOptions );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CMnrpPositionRequest* CMnrpPositionRequest::NewL(
    const TDesC& aAppName, 
    MMnrpPositionObserver& aObserver )
    {
    CMnrpPositionRequest* self =
        new (ELeave) CMnrpPositionRequest( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aAppName );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CMnrpPositionRequest::~CMnrpPositionRequest()
    {
    Cancel();
    iPositioner.Close();
    iLocationServer.Close();
    delete iGenericPosition;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpPositionRequest::FetchNewPosition( TBool aLastKnownLocationAllowed )
    {
    Cancel();
    // Request location
    iState = EAcquiringDefaultLocation;
    iLastKnownLocationAllowed = aLastKnownLocationAllowed;
    ExecuteNextStep();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpPositionRequest::GetPosition( TPositionInfo& aPositionInfo )
    {
    aPositionInfo = iPositionInfo;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal32 CMnrpPositionRequest::Speed() 
    { 
    return iSpeed;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal32 CMnrpPositionRequest::Heading() 
    { 
    return iHeading; 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal32 CMnrpPositionRequest::MagneticHeading() 
    { 
    return iMagneticHeading; 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal32 CMnrpPositionRequest::Course() 
    {
    return iCourse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TReal32 CMnrpPositionRequest::MagneticCourse()
    { 
    return iMagneticCourse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpPositionRequest::DoCancel()
    {
    // Cancel location request
    if (iState == EAcquiringDefaultLocation)
        {
        iPositioner.CancelRequest( EPositionerNotifyPositionUpdate );
        }
    else if (iState == EAcquiringLastKnownLocation)
        {
        iPositioner.CancelRequest( EPositionerGetLastKnownPosition );
        }
    iState = EIdle;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpPositionRequest::RunL()
    {
    if ( iStatus != KErrNone &&
         iState == EAcquiringDefaultLocation && 
         iLastKnownLocationAllowed )
        {
        // Fetching current location failed. Try last known location.
        iState = EAcquiringLastKnownLocation;
        ExecuteNextStep();
        }
    else
        {
        // Operation finished, notify observer
        iState = EIdle;
        SaveContainer( *iGenericPosition );
        iObserver.HandlePositionRequestCompletedL( iStatus.Int() );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CMnrpPositionRequest::RunError(TInt /*aError*/)
    {
    // Ignore possible errors.
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpPositionRequest::ExecuteNextStep()
    {
    iStatus = KRequestPending;

    switch (iState)
        {
        case EAcquiringDefaultLocation:
            InitContainer( *iGenericPosition );
            iPositioner.NotifyPositionUpdate( *iGenericPosition, iStatus);
            break;
        case EAcquiringLastKnownLocation:
            iPositioner.GetLastKnownPosition( iPositionInfo, iStatus );
            break;
        default:    
            // this case should never happen, see RunL
            return;
        }

    SetActive();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpPositionRequest::SetOptionsL( const TPositionUpdateOptionsBase& aOptions )
    {
    User::LeaveIfError( iPositioner.SetUpdateOptions( aOptions ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpPositionRequest::InitContainer( HPositionGenericInfo& aGenericInfo )
    {
    aGenericInfo.ClearPositionData();
    
    aGenericInfo.SetRequestedField( EPositionFieldHorizontalSpeed );
    aGenericInfo.SetRequestedField( EPositionFieldTrueCourse );
    aGenericInfo.SetRequestedField( EPositionFieldMagneticCourse );
    aGenericInfo.SetRequestedField( EPositionFieldHeading );
    aGenericInfo.SetRequestedField( EPositionFieldMagneticHeading );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpPositionRequest::SaveContainer( const HPositionGenericInfo& aGenericInfo )
    {
    TRealX nan;
    nan.SetNaN();
    iSpeed = iHeading = iCourse = iMagneticHeading = iMagneticCourse = nan;
    
    aGenericInfo.GetValue( EPositionFieldHorizontalSpeed, iSpeed );
    aGenericInfo.GetValue( EPositionFieldTrueCourse, iCourse );
    aGenericInfo.GetValue( EPositionFieldMagneticCourse, iMagneticCourse );
    aGenericInfo.GetValue( EPositionFieldHeading, iHeading );
    aGenericInfo.GetValue( EPositionFieldMagneticHeading, iMagneticHeading );
    
    iPositionInfo = *iGenericPosition;
    }
