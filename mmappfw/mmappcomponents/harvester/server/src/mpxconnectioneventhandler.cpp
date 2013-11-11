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
#ifdef RD_MULTIPLE_DRIVE
#include <driveinfo.h>
#endif //RD_MULTIPLE_DRIVE
#include <mpxpskeywatcher.h>
#include <coreapplicationuisdomainpskeys.h>
#include <UsbWatcherInternalPSKeys.h>
#include <usbpersonalityids.h>
#include <mtpprivatepskeys.h>
#include <mpxlog.h>
#include "mpxconnectioneventhandler.h"

// CONSTANTS
const TUint KUsbAllStates = 0xFFFFFFFF;

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CMPXConnectionEventHandler::CMPXConnectionEventHandler( MMPXSystemEventObserver& aObserver ) :
                                          CActive( EPriorityStandard ),
                                          iObserver( aObserver ),
                                          iState ( EMPXConnectionNone ),
                                          iUsbManConnected( EFalse )
                                          
                                          
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXConnectionEventHandler::ConstructL()
    {
    MPX_DEBUG1(_L("CMPXConnectionEventHandler::ConstructL <---"));
    iUSBKeyWatcher = CMPXPSKeyWatcher::NewL( KPSUidUsbWatcher,
                                            KUsbWatcherSelectedPersonality,
                                            this );

    //for checking MTP status key whether a client has started
    iMTPKeyWatcher = CMPXPSKeyWatcher::NewL( KMtpPSUid, 
                                             KMtpPSStatus,
                                             this );

    // Connect to usbman
    ConnectUsbMan();
    MPX_DEBUG1(_L("CMPXConnectionEventHandler::ConstructL --->"));
    }


// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXConnectionEventHandler* CMPXConnectionEventHandler::NewL
                                        ( MMPXSystemEventObserver& aObserver )
    {
    CMPXConnectionEventHandler* self = CMPXConnectionEventHandler::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXConnectionEventHandler* CMPXConnectionEventHandler::NewLC
                                        ( MMPXSystemEventObserver& aObserver )

    {
    CMPXConnectionEventHandler* self = new( ELeave ) CMPXConnectionEventHandler( aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXConnectionEventHandler::~CMPXConnectionEventHandler()
    {
    Cancel();
    if ( iUsbManConnected )
        {
        iUsbMan.Close();
        }
    delete iUSBKeyWatcher;
    delete iMTPKeyWatcher;
    }

// ---------------------------------------------------------------------------
// Poll for any ongoing USB / MTP event
// ---------------------------------------------------------------------------
//
void CMPXConnectionEventHandler::PollStatus()
    {
    // only call back on connection state != EMPXConnectionNone
    if (iState == EMPXConnectionMassStorage)
        {
        TRAP_IGNORE( DoMSStartEventL() );
        }
    else if (iState == EMPXConnectionMTPActive)
        {
        TRAP_IGNORE( DoMTPStartEventL() );
        }
    else if (iState == EMPXConnectionMTPIdle)
        {
        TRAP_IGNORE( DoMTPNotActiveEventL() );
        }
    }

// ---------------------------------------------------------------------------
// Handle the PS key event
// ---------------------------------------------------------------------------
//
void CMPXConnectionEventHandler::HandlePSEvent( TUid aUid, TInt aKey )
    {
    TRAP_IGNORE( DoHandlePSEventL(aUid,aKey) );
    }

// ---------------------------------------------------------------------------
// Handle the PS key event
// ---------------------------------------------------------------------------
//
void CMPXConnectionEventHandler::DoHandlePSEventL( TUid /*aUid*/, TInt /*aKey*/ )
    {
    MPX_FUNC("CMPXConnectionEventHandler::DoHandlePSEvent()");
    if ( !iUsbManConnected )
        {
        ConnectUsbMan();
        }
    
    // Handle the Key event
    TInt usbStatus;
    iUSBKeyWatcher->GetValue( usbStatus );
    
    TInt mtpStatus;
    TInt err = iMTPKeyWatcher->GetValue(mtpStatus);
    
    MPX_DEBUG3("CMPXConnectionEventHandler::DoHandlePSEventL, usbStatus = %d, mtpStatus = %d", usbStatus, mtpStatus);
    MPX_DEBUG2("CMPXConnectionEventHandler::DoHandlePSEventL, iState = %d", iState);
    MPX_DEBUG4("CMPXConnectionEventHandler::DoHandlePSEventL, iDeviceState = %d, EUsbDeviceStateAddress(%d), EUsbDeviceStateConfigured(%d)", iDeviceState, EUsbDeviceStateAddress, EUsbDeviceStateConfigured);
        
    // events from lower level is not causing multiple callback on the same type
    // usb is really only connected if went through Address or Configured state
    if( (usbStatus == KUsbPersonalityIdMS) && (iDeviceState == EUsbDeviceStateAddress || iDeviceState == EUsbDeviceStateConfigured))
        {
        if ((iState == EMPXConnectionMTPIdle) || (iState == EMPXConnectionMTPActive))
            {
            MPX_DEBUG1("CMPXConnectionEventHandler::DoHandlePSEvent - MTP End");
            iObserver.HandleSystemEventL( EUSBMTPEndEvent, -1 );
            iState = EMPXConnectionNone;
            }
        
        if (iState != EMPXConnectionMassStorage)
            {
            DoMSStartEventL();
            }
        }
    else if (iState == EMPXConnectionMassStorage)
        {
        if (usbStatus != KUsbPersonalityIdMS)
            {
            MPX_DEBUG1("CMPXConnectionEventHandler::DoHandlePSEvent - USB MassStorage End");
            iObserver.HandleSystemEventL( EUSBMassStorageEndEvent, -1 );
            iState = EMPXConnectionNone;
            }
        }
    
    // after MassStorage End, it is possible that MTP is still connected
    if (iState != EMPXConnectionMassStorage)
        {
		if (err == KErrNone)
		    {
            if ((mtpStatus == EMtpPSStatusUninitialized) && (iState != EMPXConnectionNone))
                {
                MPX_DEBUG1("CMPXConnectionEventHandler::DoHandlePSEvent - MTP End");
                iObserver.HandleSystemEventL( EUSBMTPEndEvent, -1 );
                iState = EMPXConnectionNone;
                }
            else if ((mtpStatus == EMtpPSStatusActive) && (iState != EMPXConnectionMTPActive)
                    && ((usbStatus == KUsbPersonalityIdMTP) || (usbStatus == KUsbPersonalityIdPCSuiteMTP))) // only trigger MusicPlayer fully block and RAM Drive if USB MTP/PCSuiteMTP is connected
                {
                DoMTPStartEventL();
                }
            else if ((mtpStatus == EMtpPSStatusReadyToSync) && (iState != EMPXConnectionMTPIdle) && (iState != EMPXConnectionMTPActive))
                {
                DoMTPNotActiveEventL();
                }
            }
        else
            {
            // mtpwatcher can err out because the mtp ps key would only be defined and default after first use,
            // and this logic here might hit before that
            MPX_DEBUG2("CMPXConnectionEventHandler::DoHandlePSEvent - MTPWatcher err = %d", err);
            }
        }
    }

void CMPXConnectionEventHandler::DoMSStartEventL()
    {
    MPX_DEBUG1("CMPXConnectionEventHandler::DoMSStartEventL - USB MassStorage Start");
    iObserver.HandleSystemEventL( EUSBMassStorageStartEvent, -1 );
    iState = EMPXConnectionMassStorage;
    }

void CMPXConnectionEventHandler::DoMTPStartEventL()
    {
    MPX_DEBUG1("CMPXConnectionEventHandler::DoMTPStartEventL - MTP Start");
    iObserver.HandleSystemEventL( EUSBMTPStartEvent, -1 );
    iState = EMPXConnectionMTPActive;
    }

void CMPXConnectionEventHandler::DoMTPNotActiveEventL()
    {
    MPX_DEBUG1("CMPXConnectionEventHandler::DoMTPNotActiveEventL - MTP Not Active");
    iObserver.HandleSystemEventL( EUSBMTPNotActiveEvent, -1 );
    iState = EMPXConnectionMTPIdle;
    }

// ---------------------------------------------------------------------------
// CMPXConnectionEventHandler::RunL
// ---------------------------------------------------------------------------
//
void CMPXConnectionEventHandler::RunL()
    {
    MPX_FUNC("CMPXConnectionEventHandler::RunL()");
    TInt status( iStatus.Int() );
    
    MPX_DEBUG2("CMPXConnectionEventHandler::RunL status=%d", status );
    if ( status != KErrCancel && status != KErrServerTerminated )
        {
        iUsbMan.DeviceStateNotification( KUsbAllStates, iDeviceState, iStatus );
        SetActive();
        }
    
    if ( status == KErrNone )
        {
        MPX_DEBUG2("CMPXConnectionEventHandler::RunL - DeviceState = %d", iDeviceState);
        HandlePSEvent( TUid::Uid(0), 0 );
        }
    }

// ---------------------------------------------------------------------------
// CMPXConnectionEventHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CMPXConnectionEventHandler::DoCancel()
    {
    MPX_FUNC("CMPXConnectionEventHandler::DoCancel()");
    if ( iUsbManConnected )
        {
        iUsbMan.DeviceStateNotificationCancel();
        }
    }

// ---------------------------------------------------------------------------
// CMPXConnectionEventHandler::ConnectUsbMan
// If error, default iDeviceState to EUsbDeviceStateConfigured so this would not
// block usb event mode change.
// ---------------------------------------------------------------------------
void CMPXConnectionEventHandler::ConnectUsbMan()
    {
    MPX_FUNC("CMPXConnectionEventHandler::ConnectUsbMan()");
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

