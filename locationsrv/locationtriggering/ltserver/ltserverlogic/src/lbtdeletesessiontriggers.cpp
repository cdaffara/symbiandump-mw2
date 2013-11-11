/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Object operation for deletiong session triggers.
*
*/


// INCLUDE FILES
#include "lbtglobal.h"
#include "lbtdeletesessiontriggers.h"
#include "lbtcontainer.h"
#include "lbtnotificationmap.h"
#include "lbttriggerchangeevent.h"
#include "lbtcontainerupdatefilter.h"
#include "lbtserverconsts.h"
#include "lbtdeletesessiontriggerobserver.h"
#include "lbtlogger.h"

// ===================== MEMBER FUNCTIONS =====================

// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::NewLC
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtDeleteSessionTriggers* CLbtDeleteSessionTriggers::NewL(MLbtDeleteSessionTriggerObserver& aObserver,
															CLbtContainer& aContainer,CLbtNotificationMap& aNotificationMap)
	{
	FUNC_ENTER("CLbtDeleteSessionTriggers::NewL");
	CLbtDeleteSessionTriggers* deleteSessionTriggers=new( ELeave ) CLbtDeleteSessionTriggers(aObserver,aContainer,aNotificationMap);
	CleanupStack::PushL(deleteSessionTriggers);
	deleteSessionTriggers->ConstructL();
	CleanupStack::Pop(deleteSessionTriggers);
	return(deleteSessionTriggers);
	}


// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::CLbtDeleteSessionTriggers
// C++ Default constructor
// ---------------------------------------------------------------------------
//
CLbtDeleteSessionTriggers::CLbtDeleteSessionTriggers(MLbtDeleteSessionTriggerObserver& aObserver,CLbtContainer& aContainer,
							CLbtNotificationMap& aNotificationMap):CActive( EPriorityStandard ),iObserver(aObserver),
							iContainer(aContainer),iNotificationMap(aNotificationMap)
	{
	FUNC_ENTER("CLbtDeleteSessionTriggers::CLbtDeleteSessionTriggers");
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtDeleteSessionTriggers::ConstructL()
	{
	
	}

// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::~CLbtDeleteSessionTriggers
// Destructor
// ---------------------------------------------------------------------------
//
CLbtDeleteSessionTriggers::~CLbtDeleteSessionTriggers()
	{
	FUNC_ENTER("CLbtDeleteSessionTriggers::~CLbtDeleteSessionTriggers");
	Cancel();
	}


// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtDeleteSessionTriggers::DoCancel()
	{
	FUNC_ENTER("CLbtDeleteSessionTriggers::DoCancel");
	iContainer.CancelAsyncOperation(iAOIdentificationNum);
	}



// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::DeleteSessionTriggers
// ---------------------------------------------------------------------------
//
void CLbtDeleteSessionTriggers::DeleteSessionTriggers(CLbtContainerUpdateFilter* aContainerFilter)
	{
	FUNC_ENTER("CLbtDeleteSessionTriggers::DeleteSessionTriggers");
	iContainer.DeleteTriggers(aContainerFilter,iAOIdentificationNum,iStatus);
	SetActive();
	}


// ---------------------------------------------------------------------------
// CLbtDeleteSessionTriggers::RunL
// ---------------------------------------------------------------------------
//
void CLbtDeleteSessionTriggers::RunL()
	{
	FUNC_ENTER("CLbtDeleteSessionTriggers::RunL");
	// If there are no triggers to be deleted,DeleteTriggers method will return error code
	// other than KErrNone in which case management library should not be notified.

	if(iStatus.Int()==KErrNone)
		{
		RMessage2 message;
		TInt retValue=iNotificationMap.Retreive(message,ELbtNotifyTriggerChangeEvent,
						CLbtServerLogicBase::TLbtManagementLibrary);
		
		if(retValue==KErrNone)
			{
			TLbtTriggerChangeEvent event;
			event.iEventType = ELbtTriggerChangeEventMultiple;
		
			//Send notification to management library
			TPckg<TLbtTriggerChangeEvent> eventChange(event);
			if(LbtGlobal::Write(message,KParamChangeEvent,eventChange)==KErrNone)
				{
				LbtGlobal::RequestComplete(message,KErrNone);
				}
			}
		}
	iObserver.HandleDeleteSessionTriggersClosureL(this);	
	}

// end of file

