/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation to fire triggers, both session and startup
*
*/


// INCLUDE FILES
#include <e32std.h>	// For RProcess
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>
#include <lbttriggerfilterbyattribute.h>
#include "lbttriggerfirehandler.h"
#include "lbtserverconsts.h"
#include "lbtglobal.h"
#include "lbtstartuptrigger.h"
#include "lbtcontainer.h"
#include "lbtcontainertriggerentry.h"
#include "lbttriggerfiltercomposite.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtnotificationmap.h"
#include "lbtlogger.h"


// ===================== MEMBER FUNCTIONS =====================

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerFireHandler* CLbtTriggerFireHandler::NewL( CLbtNotificationMap& aNotificationMap,
			    									  CLbtContainer& aContainer )
	{
	FUNC_ENTER("CLbtTriggerFireHandler::NewL");
	CLbtTriggerFireHandler* self = new (ELeave) CLbtTriggerFireHandler(aNotificationMap,
																	   aContainer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::~CLbtTriggerFireHandler
// Destructor
// ---------------------------------------------------------------------------
//
CLbtTriggerFireHandler::~CLbtTriggerFireHandler()
	{
	FUNC_ENTER("CLbtTriggerFireHandler::~CLbtTriggerFireHandler");
	Cancel();
	iFireInfoArray.Reset();
	iTriggerArray.ResetAndDestroy();
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::CLbtTriggerFireHandler
// C++ Default constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerFireHandler::CLbtTriggerFireHandler( CLbtNotificationMap& aNotificationMap,	    									    
	    									    CLbtContainer& aContainer )
	: CActive( EPriorityHigh ), iNotificationMap(aNotificationMap),iContainer(aContainer)
	{
	
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::FireTriggerL
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::FireTriggerL(TLbtTriggerFireInfo aFireInfo)
	{
	FUNC_ENTER("CLbtTriggerFireHandler::FireTriggerL");
	// queue the fire info into the array;
	iFireInfoArray.AppendL(aFireInfo);
	
	if(!IsActive())
		{
		// Add this active object to the active scheduler
		CActiveScheduler::Add(this);
		
		// Fire this trigger
		FireNextTrigger();
		}
	}
	
// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::ConstructL()
	{

	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::RunL
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::RunL()
	{
	FUNC_ENTER("CLbtTriggerFireHandler::RunL");
	// Got the response for List Triggers from container
	if(iStatus.Int() == KErrNone)
		{
        // Notify the client that this trigger is fired.
        NotifyTriggerFired();
        
    	// Start the client process if the trigger is an start up trigger
    	CLbtTriggerEntry* triggerEntry = iTriggerArray[0]->TriggerEntry();

    	// If the trigger is a startup trigger then start the trigger
    	// handling process
    	if(triggerEntry->Type() == CLbtTriggerEntry::ETypeStartup)
    		{
    		StartTriggerHandlingProcess(triggerEntry);
    		}
		}
	else
	    {
        // Nothing to do in case the List Triggers returns error.
        // So we just log this error case.
        LBT_TRACE(KLbtLogError|KLbtLogServerLogic, __FILE__, __LINE__, "Error %d in List Triggers. Hence trigger %d not fired", iStatus.Int(),iFireInfoArray[0].iTriggerId);
	    }

	// Removed the fired trigger from the fire info queue
	iFireInfoArray.Remove(0);
	
	// If there are any more trigger fire requests then fire the next trigger
	if(iFireInfoArray.Count())
		{
		FireNextTrigger();
		}
	else
		{
		// No trigger to be fired, so remove this AO from the 
		// active scheduler queue
		Deque();
		}
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::RunError
// ---------------------------------------------------------------------------
//
TInt CLbtTriggerFireHandler::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::FireNextTrigger
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::FireNextTrigger()
	{
	FUNC_ENTER("CLbtTriggerFireHandler::FireNextTrigger");
	// Check if any trigger is in the process of being fired	
	if(!IsActive())
		{
		// Clear the array before it can be used again
		iTriggerArray.ResetAndDestroy();
		
		// Get the trigger entry from container
		RArray<TLbtTriggerId> triggerId;
		TInt error = triggerId.Append( iFireInfoArray[0].iTriggerId );
		if( error == KErrNone )
		    {
            iContainer.GetTriggers( triggerId, 
                                    iTriggerArray, 
                                    iAOIdentificationNum,
                                    iStatus);
            SetActive();
		    }
		triggerId.Close();		
		}
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::DoCancel()
	{
	FUNC_ENTER("CLbtTriggerFireHandler::DoCancel");
	iContainer.CancelAsyncOperation(iAOIdentificationNum);
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::NotifyTriggerFired
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::NotifyTriggerFired()
    {
    FUNC_ENTER("CLbtTriggerFireHandler::NotifyTriggerFired");
	// Search the message array if the process has registered for 
	// trigger fire notification
	RMessage2 message;
	CLbtContainerTriggerEntry* trigger = iTriggerArray[0];

    // Write the trigger fire info in the message
    TPckg<TLbtTriggerFireInfo> fireInfo(iFireInfoArray[0]);

	while( !iNotificationMap.Retreive(message,trigger->ExtendedTriggerInfo()->OwnerSid(),
                       ELbtNotifyTriggerFired) )
	    {
        TInt retVal = LbtGlobal::Write(message, KParamTriggerFireInfo, fireInfo);
        // Complete the message with the error code.
        LbtGlobal::RequestComplete(message, retVal);
	    }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFireHandler::StartTriggerHandlingProcess
// ---------------------------------------------------------------------------
//
void CLbtTriggerFireHandler::StartTriggerHandlingProcess(
    CLbtTriggerEntry* aTriggerEntry)
    {
    FUNC_ENTER("CLbtTriggerFireHandler::StartTriggerHandlingProcess");
	CLbtStartupTrigger* startupTrigger = 
	                        static_cast<CLbtStartupTrigger*>(aTriggerEntry);
    
	TSecureId secureId;
	TFileName fileName;
	startupTrigger->GetProcessId(fileName, secureId);
	
	RProcess proc;
	TInt retVal = proc.Create(fileName, startupTrigger->CommandLine());
	if(retVal == KErrNone)
		{
		proc.Resume();
		}
	proc.Close();
	}


// end of file
