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
* Description:  This file implements the Location triggering container
*                interface
*
*/


#include <e32std.h>
#include <s32strm.h>
#include <s32mem.h>

#include <lbt.h>
#include <lbterrors.h>
#include "lbtserverconsts.h"
#include "lbtmanager.h"
#include "lbttriggeringsystemmanagementsettings.h"
#include "lbtterminalptrholder.h"


// ---------------------------------------------------------
// RLbtManager::RLbtManager
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C RLbtManager::RLbtManager()
	{
	TRAP_IGNORE( iPtrHolder = CLbtSubSessnPtrHolder::NewL(2,0) );
	}

// ---------------------------------------------------------
// RLbtManager::~RLbtManager
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C RLbtManager::~RLbtManager()
	{
	delete iPtrHolder;
	iPtrHolder = NULL;
	}	

// ---------------------------------------------------------
// RLbtManager::Open
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C  TInt RLbtManager::Open( RLbtServer& aServer )
    {
    __ASSERT_ALWAYS(aServer.Handle() != 0,
	User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	__ASSERT_ALWAYS(! SubSessionHandle() , User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
    TInt	ret = CreateSubSession(aServer, ELbtMgmtSubSessionOpen, TIpcArgs());
    return ret;   
	};

// ---------------------------------------------------------
// RLbtManager::SetTriggeringSystemSettingsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RLbtManager::SetTriggeringSystemSettingsL(
            const TLbtTriggeringSystemManagementSettings& aSettings,TLbtManagementSettingsMask aMask)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	TIpcArgs args;
    TPtrC8 ptr(
		reinterpret_cast<const TUint8*>(&aSettings),
		sizeof(TLbtTriggeringSystemManagementSettings));
		
	args.Set(KParamSettings,&ptr);
	args.Set(KParamMask,aMask);		
	
	User::LeaveIfError(SendReceive(ELbtSetTriggeringSettings,args));
    }

// ---------------------------------------------------------
// RLbtManager::SetTriggeringSystemSettingsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C void RLbtManager::SetTriggeringMechanismStateL(
            TLbtTriggeringMechanismState aState )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	
	TIpcArgs args;
	TLbtTriggeringSystemManagementSettings settings;
	settings.SetTriggeringMechanismState(aState);
	 
	TPtrC8 ptr(
		reinterpret_cast<const TUint8*>(&settings),
		sizeof(TLbtTriggeringSystemManagementSettings));
		args.Set(KParamSettings,&ptr);
	 
	iMask = ETriggeringMechanismState;
	 
	args.Set(KParamMask,iMask);
    User::LeaveIfError(SendReceive(ELbtSetTriggeringSettings,args));
    }

// ---------------------------------------------------------
// RLbtManager::GetTriggeringSystemStatusL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
 EXPORT_C void RLbtManager::GetTriggeringSystemStatusL(
            TLbtTriggeringSystemStatus& aTriggeringSystemStatus)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	TIpcArgs args;

    TPtr8 ptr(
		reinterpret_cast< TUint8*>(&aTriggeringSystemStatus),
		sizeof(TLbtTriggeringSystemStatus));
		args.Set(KParamStatus,&ptr);
	
	User::LeaveIfError(SendReceive(ELbtGetTriggeringSysStatus,args));
    }

// ---------------------------------------------------------
// RLbtManager::NotifyTriggeringSystemStatusChange
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
EXPORT_C void RLbtManager::NotifyTriggeringSystemStatusChange(
            TRequestStatus& aStatus)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    SendReceive(ELbtNotifyTriggeringSysStatusChange,aStatus);
    }

// ---------------------------------------------------------
// RLbtManager::GetTriggeringSystemSettingsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//   
EXPORT_C void RLbtManager::GetTriggeringSystemSettingsL( 
            TLbtTriggeringSystemSettings& aSetting )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	TIpcArgs args;

    TPtr8 ptr(
		reinterpret_cast< TUint8*>(&aSetting),
		sizeof(TLbtTriggeringSystemManagementSettings));
		args.Set(KParamSettings,&ptr);
	User::LeaveIfError(SendReceive(ELbtGetTriggeringSysSetting,args));
    }

// ---------------------------------------------------------
// RLbtManager::CancelNotifyTriggeringSystemStatusChange
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C void RLbtManager::CancelNotifyTriggeringSystemStatusChange()
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    SendReceive(ELbtCnclNotifyTriggeringSysStatusChange);
    }
    
// ---------------------------------------------------------
// RLbtManager::NotifyTriggeringSystemSettingChange
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
EXPORT_C void RLbtManager::NotifyTriggeringSystemSettingChange( 
            TLbtTriggeringSystemManagementSettings& aManagementSettings,
            TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
	TIpcArgs args;

    iPtrHolder->Ptr(0).Set(
		reinterpret_cast< TUint8*>(&aManagementSettings),
		sizeof(TLbtTriggeringSystemManagementSettings),
		sizeof(TLbtTriggeringSystemManagementSettings));
		args.Set(0,&iPtrHolder->Ptr(0));
	SendReceive( ELbtNotifyTriggeringSysSettingChange,args,aStatus );
	}

// ---------------------------------------------------------
// RLbtManager::GetTriggerManagerUiInfoL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//  
EXPORT_C void RLbtManager::GetTriggerManagerUiInfoL(
            RArray < TLbtTriggerManagerUiInfo >& /*aManagerUiList*/ )
    {
    
    }


// end of file

