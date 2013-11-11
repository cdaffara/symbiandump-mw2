/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides implementation for CCallInformation -class.
*
*/


#include <e32base.h>
#include <callinformation.h>    //Call Information Mediator API

#include "CCallInformationImpl.h"
#include "ccallinfoiter.h"
#include "mcallinformationobserver.h"

bool GlobalEmptyCallList;
bool GlobalTriggerSignal;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCallInformationImpl::CCallInformationImpl()
    {
        
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCallInformationImpl* CCallInformationImpl::NewL()
    {
    CCallInformationImpl* self = new (ELeave) CCallInformationImpl( );  
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallInformationImpl::~CCallInformationImpl()
    {
    delete iInfos;
    delete iCallIter;   
    
    delete iInfosInState;    
    delete iCallStateIter;   
    }


// ---------------------------------------------------------------------------
// Saves the observer call information changes.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::NotifyCallInformationChanges(
    MCallInformationObserver& aObserver )
    {
    iObserver = &aObserver;
    return;
    }
    
// ---------------------------------------------------------------------------
// Removes the observer.
// ---------------------------------------------------------------------------
//
void CCallInformationImpl::CancelNotification( )
    {
    iObserver = NULL;
    return;
    }


// ---------------------------------------------------------------------------
// Returns info about all calls. 
// ---------------------------------------------------------------------------
//
CCallInfoIter& CCallInformationImpl::GetCallsL( )
    {    
    iInfos = CCallInfos::NewL();  
    
    if (!GlobalEmptyCallList)
        {
        TCallInfo callInfo;
            int index = 0;
            unsigned int serviceId = 1;
            callInfo.iIndex = index;
            callInfo.iServiceId = serviceId;
            callInfo.iDirection= CCPCall::EMobileOriginated;
            callInfo.iType = CCPCall::ECallTypeCSVoice;
            callInfo.iState = CCPCall::EStateDialling;
            iInfos->AddL(callInfo);
            
            index++;
            serviceId++;
            callInfo.iIndex = index;
            callInfo.iServiceId = serviceId;
            callInfo.iDirection= CCPCall::EMobileTerminated;
            callInfo.iType = CCPCall::ECallTypeCSVoice;
            callInfo.iState = CCPCall::EStateConnected;
            iInfos->AddL(callInfo);
        }
    
    iCallIter = CCallInfoIter::NewL( *iInfos );
                      
    return *iCallIter;            
    }

// ---------------------------------------------------------------------------
// Returns information whether call in given state exist. 
// ---------------------------------------------------------------------------
//
TBool CCallInformationImpl::IsCallInState( CCPCall::TCallState aState ) const
    {
    bool value(false);
    if (aState == CCPCall::EStateConnected)
        {
        value = true;
        }
    
    if( GlobalTriggerSignal )
        {
        iObserver->CallInformationChanged();
        GlobalTriggerSignal=false;
        }
    
    return value;
    }

