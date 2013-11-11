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
* Description: Network registration status handler
*
*/


#include "nwregistrationstatushandler.h"
#include "locationmanagerdebug.h"


// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::CNwRegistrationStatusHandler()
// ---------------------------------------------------------------------------- 
CNwRegistrationStatusHandler::CNwRegistrationStatusHandler(RMobilePhone& aMobilePhone):
        CActive(EPriorityStandard ),
        iMobilePhone(aMobilePhone),
        iRegistrationStatus(RMobilePhone::ERegistrationUnknown),
        iState(ERegStatusOptStateNone)
    {
    }

// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::~CNwRegistrationStatusHandler()
// ---------------------------------------------------------------------------- 
CNwRegistrationStatusHandler::~CNwRegistrationStatusHandler()
    {
    LOG("CNwRegistrationStatusHandler::~CNwRegistrationStatusHandler");
    Cancel();
    }

// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::NewL()
// ---------------------------------------------------------------------------- 
CNwRegistrationStatusHandler* CNwRegistrationStatusHandler::NewL(RMobilePhone& aMobilePhone)
    {
    LOG("CNwRegistrationStatusHandler::NewL ,begin");
    CNwRegistrationStatusHandler* self = new( ELeave ) CNwRegistrationStatusHandler(aMobilePhone);
       CleanupStack::PushL( self );
       self->ConstructL();
       CleanupStack::Pop(); //self
       
       return self;
    }

// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::ConstructL()
// ---------------------------------------------------------------------------- 
void CNwRegistrationStatusHandler::ConstructL()
    {
    LOG("CNwRegistrationStatusHandler::ConstructL ,begin");
	CActiveScheduler::Add(this);
    iMobilePhone.GetNetworkRegistrationStatus(iStatus, iRegistrationStatus);
    iState = ERegStatusOptStateGet;
    SetActive();
	LOG("CNwRegistrationStatusHandler::ConstructL ,end");
    }


// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::GetNetworkRegistrationStatus()
// ---------------------------------------------------------------------------- 
RMobilePhone::TMobilePhoneRegistrationStatus CNwRegistrationStatusHandler::GetNetworkRegistrationStatus() const
    {
    return iRegistrationStatus;
    }


// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::StartNotifier()
// ---------------------------------------------------------------------------- 
void CNwRegistrationStatusHandler::StartNotifier()
    {
	LOG("CNwRegistrationStatusHandler::StartNotifier ,begin");
    if(!IsActive())
        {
        iState = ERegStatusOptStateNotify;
        iMobilePhone.NotifyNetworkRegistrationStatusChange(iStatus, iRegistrationStatus);
        SetActive();
        }
	else
	    {
        // already active.
        LOG("N/W registration status handler already active");
		}
	LOG("CNwRegistrationStatusHandler::StartNotifier ,end");
    }
	
// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::RunL()
// ---------------------------------------------------------------------------- 
void CNwRegistrationStatusHandler::RunL( )
    {
    LOG("CNwRegistrationStatusHandler::RunL");
    LOG1("Status - %d", iStatus.Int());
    StartNotifier();
    }
    
// ----------------------------------------------------------------------------
// CNwRegistrationStatusHandler::DoCancel()
// ---------------------------------------------------------------------------- 
void CNwRegistrationStatusHandler::DoCancel( )
    {
    LOG("CNwRegistrationStatusHandler::DoCancel");
    switch(iState)
        {
        case ERegStatusOptStateGet:
            {
            iMobilePhone.CancelAsyncRequest(EMobilePhoneGetNetworkRegistrationStatus);
            break;
            }
        case ERegStatusOptStateNotify:
            {
            iMobilePhone.CancelAsyncRequest(EMobilePhoneNotifyNetworkRegistrationStatusChange);
            break;
            }
        default:
            // execution shouldn't come over here
            LOG1("CNwRegistrationStatusHandler::DoCancel, in wrong state - %d", iState);
            break;
        }            
    }


// End of file

