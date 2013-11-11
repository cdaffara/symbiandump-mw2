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
* Description:  Monitors for USB events
*
*/


#include <e32base.h>
#include <f32file.h>
#include <e32property.h>
#include <driveinfo.h>
#include <mpxpskeywatcher.h>
#include <coreapplicationuisdomainpskeys.h>
#include <UsbWatcherInternalPSKeys.h>
#include <usbpersonalityids.h>
#include <mtpprivatepskeys.h>
#include <mpxlog.h>
#include "mpxusbeventhandler.h"

// CONSTANTS
const TUint KUsbAllStates = 0xFFFFFFFF;

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CMPXUsbEventHandler::CMPXUsbEventHandler( MMPXSystemEventObserver& aObserver ) :
                                          CActive( EPriorityStandard ),
                                          iWasMtp  ( EFalse ),
                                          iObserver( aObserver ),
                                          iUsbManConnected( EFalse )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXUsbEventHandler::ConstructL()
    {
    MPX_DEBUG1(_L("CMPXUsbEventHandler::ConstructL <---"));
    iPSKeyWatcher = CMPXPSKeyWatcher::NewL( KPSUidUsbWatcher,
                                            KUsbWatcherSelectedPersonality,
                                            this );

    //for checking MTP status key whether a client has started
    iMTPKeyWatcher = CMPXPSKeyWatcher::NewL( KMtpPSUid, 
                                             KMtpPSStatus,
                                             this );

    // Connect to usbman
    ConnectUsbMan();
    MPX_DEBUG1(_L("CMPXUsbEventHandler::ConstructL --->"));
    }


// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXUsbEventHandler* CMPXUsbEventHandler::NewL
                                        ( MMPXSystemEventObserver& aObserver )
    {
    CMPXUsbEventHandler* self = CMPXUsbEventHandler::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXUsbEventHandler* CMPXUsbEventHandler::NewLC
                                        ( MMPXSystemEventObserver& aObserver )

    {
    CMPXUsbEventHandler* self = new( ELeave ) CMPXUsbEventHandler( aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXUsbEventHandler::~CMPXUsbEventHandler()
    {
    Cancel();
    if ( iUsbManConnected )
        {
        iUsbMan.Close();
        }
    delete iPSKeyWatcher;
    delete iMTPKeyWatcher;
    }

// ---------------------------------------------------------------------------
// Poll for any ongoing USB / MTP event
// ---------------------------------------------------------------------------
//
void CMPXUsbEventHandler::PollStatus()
    {
    TInt value;
    iPSKeyWatcher->GetValue( value );
    if( (value==KUsbPersonalityIdMS) ||
        (value==KUsbPersonalityIdMTP) ||
        (value==KUsbPersonalityIdPCSuiteMTP) )
        {
        HandlePSEvent( TUid::Uid(0), 0 );
        }
    }

// ---------------------------------------------------------------------------
// Handle the PS key event
// ---------------------------------------------------------------------------
//
void CMPXUsbEventHandler::HandlePSEvent( TUid aUid, TInt aKey )
    {
    TRAP_IGNORE( DoHandlePSEventL(aUid,aKey) );
    }

// ---------------------------------------------------------------------------
// Handle the PS key event
// ---------------------------------------------------------------------------
//
void CMPXUsbEventHandler::DoHandlePSEventL( TUid /*aUid*/, TInt /*aKey*/ )
    {
    MPX_FUNC("CMPXUsbEventHandler::DoHandlePSEvent()");
    if ( !iUsbManConnected )
        {
        ConnectUsbMan();
        }
    
    // Use the default MMC drive
    TInt removableDrive( EDriveE );
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
        DriveInfo::EDefaultRemovableMassStorage,
        removableDrive ) );

    // Handle the Key event
    TInt value;
    iPSKeyWatcher->GetValue( value );

    // Determine if we are currently in MTP / Mass Storage mode
    //
    MPX_DEBUG2("CMPXUsbEventHandler::DoHandlePSEvent -- get usb value %i", value);
	MPX_DEBUG2("CMPXUsbEventHandler::DoHandlePSEvent -iState=%d", iState);
    if( (value == KUsbPersonalityIdMS) && 
        ( iDeviceState == EUsbDeviceStateAddress ||
          iDeviceState == EUsbDeviceStateConfigured ) )
        {
        // Switch mode
        if( iState == KUsbPersonalityIdMTP )
            {
            MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - MTP End");
            iObserver.HandleSystemEventL( EUSBMTPEndEvent, removableDrive );
            }
        MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - USB Start");
        removableDrive = -1;

        // Notify the state change (may happen more than once)
        iObserver.HandleSystemEventL( EUSBMassStorageStartEvent, removableDrive );
        iState = KUsbPersonalityIdMS;
        }
    else if( ( value == KUsbPersonalityIdMTP || value == KUsbPersonalityIdPCSuiteMTP ) &&
             ( iDeviceState == EUsbDeviceStateAddress || iDeviceState == EUsbDeviceStateConfigured ) )
        {
        if( iState == KUsbPersonalityIdMS )
            {
            MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - USB End");
            iObserver.HandleSystemEventL( EUSBMassStorageEndEvent, removableDrive );
            }
        iMTPKeyWatcher->GetValue(value);
        MPX_DEBUG2("CMPXUsbEventHandler::DoHandlePSEvent -- get MTP value %i", value);
		
        if (value == EMtpPSStatusActive)
            {
            MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - MTP Start");
            iObserver.HandleSystemEventL( EUSBMTPStartEvent, removableDrive );
            iState = KUsbPersonalityIdMTP;
            }
        else if( value != EMtpPSStatusReadyToSync )
            {
            MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - MTP Not Active");
            iObserver.HandleSystemEventL( EUSBMTPNotActiveEvent, removableDrive );
            iState = KUsbPersonalityIdMTP;
            }
        }
    // Make sure MTP and MS flags are OFF
    //
    else if( value != KUsbPersonalityIdMTP &&
             value != KUsbPersonalityIdMS &&
             value != KUsbPersonalityIdPCSuiteMTP )
        {
        // Signal end of event
        if( iState == KUsbPersonalityIdMS )
            {
            MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - USB End");
            iObserver.HandleSystemEventL( EUSBMassStorageEndEvent, removableDrive );
            }
        else if( iState == KUsbPersonalityIdMTP )
            {
            MPX_DEBUG1("CMPXUsbEventHandler::DoHandlePSEvent - MTP End");
            iObserver.HandleSystemEventL( EUSBMTPEndEvent, removableDrive );
            }
        iState = KUsbWatcherSelectedPersonalityNone;
        }
    }

// ---------------------------------------------------------------------------
// CMPXUsbEventHandler::RunL
// ---------------------------------------------------------------------------
//
void CMPXUsbEventHandler::RunL()
    {
    MPX_FUNC("CMPXUsbEventHandler::RunL()");
    TInt status( iStatus.Int() );
    
    MPX_DEBUG2("CMPXUsbEventHandler::RunL status=%d", status );
    if ( status != KErrCancel && status != KErrServerTerminated )
        {
        iUsbMan.DeviceStateNotification( KUsbAllStates, iDeviceState, iStatus );
        SetActive();
        }
    
    if ( status == KErrNone )
        {
        MPX_DEBUG2("CMPXUsbEventHandler::RunL - DeviceState = %d", iDeviceState);
        HandlePSEvent( TUid::Uid(0), 0 );
        }
    }

// ---------------------------------------------------------------------------
// CMPXUsbEventHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CMPXUsbEventHandler::DoCancel()
    {
    MPX_FUNC("CMPXUsbEventHandler::DoCancel()");
    if ( iUsbManConnected )
        {
        iUsbMan.DeviceStateNotificationCancel();
        }
    }

// ---------------------------------------------------------------------------
// CMPXUsbEventHandler::ConnectUsbMan
// If error, default iDeviceState to EUsbDeviceStateConfigured so this would not
// block usb event mode change.
// ---------------------------------------------------------------------------
void CMPXUsbEventHandler::ConnectUsbMan()
    {
    MPX_FUNC("CMPXUsbEventHandler::ConnectUsbMan()");
    if ( iUsbMan.Connect() == KErrNone )
        {
        iUsbManConnected = ETrue;
        // get device state
        TInt err = iUsbMan.GetDeviceState( iDeviceState );
        if ( err )
            {
            iDeviceState = EUsbDeviceStateUndefined;
            }
        
        // start active object
        if ( !IsActive() )
            {
            iUsbMan.DeviceStateNotification( KUsbAllStates, iDeviceState, iStatus );
            SetActive();
            }
        }
    else
        {
        iDeviceState = EUsbDeviceStateConfigured;
        }
    }
// END OF FILE

