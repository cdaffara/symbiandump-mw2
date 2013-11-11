/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CLbtPsyChangeListner class
*
*/


#include "lbtpsychangelistner.h"
#include "lbtpsychangeobserver.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::NewL
// CLbtPsyChangeListner instantiation method
// -----------------------------------------------------------------------------
//
CLbtPsyChangeListner* CLbtPsyChangeListner::NewL( MLbtPsyChangeObserver& aObserver )
    {
    CLbtPsyChangeListner* self = new( ELeave ) CLbtPsyChangeListner( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::CLbtPsyChangeListner
// Default constructor
// -----------------------------------------------------------------------------
//
CLbtPsyChangeListner::CLbtPsyChangeListner( MLbtPsyChangeObserver& aObserver ):
                                        CActive( EPriorityNormal ),iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CLbtPsyChangeListner::ConstructL()
    {
    User::LeaveIfError( iPosServer.Connect() );
    }

    
// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::~CLbtPsyChangeListner
// Destructor
// -----------------------------------------------------------------------------
//
CLbtPsyChangeListner::~CLbtPsyChangeListner()
    {
    FUNC_ENTER("CLbtPsyChangeListner::~CLbtPsyChangeListner");
    if( IsActive() )
        {
        Cancel();
        }
    iPosServer.Close();
    }

// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::StartToListen
// 
// -----------------------------------------------------------------------------
//
void CLbtPsyChangeListner::StartToListen( TPositionModuleId aPosID )
    {
    LOG("CLbtPsyChangeListner::StartToListenL");
    if( IsActive() )
        {
        Cancel();
        }
    TUid defaultID = { 0xFFFFFFFF };
    
    // If we are interested in change in all PSY, KPositionNullModuleId should be passed
    // to location framework.
    if( aPosID == defaultID )
        {
        iPosID = KPositionNullModuleId;
        }
    else
        {
        iPosID = aPosID;
        }
    iModuleStatusEvent.SetRequestedEvents(TPositionModuleStatusEvent::EEventAll);
    iPosServer.NotifyModuleStatusEvent( iModuleStatusEvent,iStatus,iPosID );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::RunL
// 
// -----------------------------------------------------------------------------
//
void CLbtPsyChangeListner::RunL()
    {
    LOG("CLbtPsyChangeListner::RunL");
    LOG1("Status:%d",iStatus.Int());
    if( iStatus.Int() == KErrNone )
        {
        TPositionModuleStatus moduleStatus;
        iModuleStatusEvent.GetModuleStatus( moduleStatus );
        LOG1("ModuleStatus:%d",moduleStatus.DeviceStatus());
        if( ( moduleStatus.DeviceStatus() == TPositionModuleStatus::EDeviceReady ) ||
            ( moduleStatus.DeviceStatus() == TPositionModuleStatus::EDeviceActive ) ||
            ( moduleStatus.DeviceStatus() == TPositionModuleStatus::EDeviceInactive ) ||
            ( moduleStatus.DeviceStatus() == TPositionModuleStatus::EDeviceStandBy) )
            {
            iObserver.HandlePsyChangeEvent( iModuleStatusEvent.ModuleId() );
            }
        else
            {
            iModuleStatusEvent.SetRequestedEvents(TPositionModuleStatusEvent::EEventAll);
            iPosServer.NotifyModuleStatusEvent( iModuleStatusEvent,iStatus,iPosID );
            SetActive();
        	}  
        }
    }
    
// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::DoCancel
// 
// -----------------------------------------------------------------------------
//    
void CLbtPsyChangeListner::DoCancel()
    {
    FUNC_ENTER("CLbtPsyChangeListner::DoCancel");
    iPosServer.CancelRequest( EPositionServerNotifyModuleStatusEvent );
    }
    
// End of file
