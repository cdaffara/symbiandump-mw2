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
* Description:  This class is responsible for loading,upgrading and holding 
*				 the server logic ecom plugin
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <ecom.h>
#include <implementationinformation.h>
#include <e32property.h>
#include "lbtserverlogiccontainer.h"
#include "lbtserverconsts.h"
#include "lbtserverlogicbase.h"
#include "lbtstatuspskeys.h"
#include "lbtlogger.h"

// ======== LOCAL FUNCTIONS =========
// static cleanup function
static void RImpleInfoPtrArrayCleanup( TAny* aArray )
	{
	static_cast<RImplInfoPtrArray*>( aArray )->ResetAndDestroy();
	}

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtServerLogicContainer* CLbtServerLogicContainer::NewL()
	{
	FUNC_ENTER("CLbtServerLogicContainer::NewL");	
	CLbtServerLogicContainer* self = new ( ELeave ) CLbtServerLogicContainer;
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop();
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtServerLogicContainer::ConstructL()
	{
	FUNC_ENTER("CLbtServerLogicContainer::ConstructL");	
	iEComSession = &(REComSession::OpenL());
	iLogicInterface = NULL;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::~CLbtServerLogicContainer
// ---------------------------------------------------------------------------
//
CLbtServerLogicContainer::~CLbtServerLogicContainer()
	{
	FUNC_ENTER("CLbtServerLogicContainer::~CLbtServerLogicContainer");	
    TRAP_IGNORE(UnloadServerLogicL());
	if(iEComSession)
		{
		iEComSession->Close();
		}
    // This is to free up resources held by the EComSession.
    // If FinalClose() is not called, then during the shutdown of server
    // UHEAP_MARKEND will generate a panic in debug builds.
    // Although the server is never shutdown as of now, this is for safe
    // programming.
    REComSession::FinalClose();
	}
	
// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::CLbtServerLogicContainer
// ---------------------------------------------------------------------------
//
CLbtServerLogicContainer::CLbtServerLogicContainer()
    : CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::LoadServerLogicL
// ---------------------------------------------------------------------------
//
void CLbtServerLogicContainer::LoadServerLogicL()
	{
	FUNC_ENTER("CLbtServerLogicContainer::LoadServerLogicL");		
	RImplInfoPtrArray implArray;
	REComSession::ListImplementationsL(KServerLogicInterfaceUid, implArray);	
	if(implArray.Count() != 1)
		{
		LOG1("Server Logic Implementation count is %d",implArray.Count());
		implArray.ResetAndDestroy();
		iEComSession->NotifyOnChange(iStatus);
		SetActive();
		// Leave if there are more than one or no 
		// implementation of server logic.
		User::Leave(KErrNotFound);
		}
	TCleanupItem arrayCleanup( RImpleInfoPtrArrayCleanup, &implArray );
	CleanupStack::PushL(arrayCleanup);
	CImplementationInformation* info = implArray[0];
	iLogicInterface = CLbtServerLogicBase::NewL(info->ImplementationUid());
	iVersion = info->Version();
	CleanupStack::PopAndDestroy(); // implArray
	
	// request ecom framework for ecom change notification
	iEComSession->NotifyOnChange(iStatus);	
	SetActive();
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::ServerLogic
// ---------------------------------------------------------------------------
//
CLbtServerLogicBase* CLbtServerLogicContainer::ServerLogic()
	{
	return iLogicInterface;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::RunL
// ---------------------------------------------------------------------------
//
void CLbtServerLogicContainer::RunL()
	{
	FUNC_ENTER("CLbtServerLogicContainer::RunL");		
	// request ecom framework for notification
	iEComSession->NotifyOnChange(iStatus);
	SetActive();
	
	RImplInfoPtrArray implArray;
	REComSession::ListImplementationsL(KServerLogicInterfaceUid, implArray);
	CleanupClosePushL(implArray);
	if(implArray.Count() != 1)
		{
		LOG1("CLbtServerLogicContainer::RunL: Server Logic Implementation count is %d",implArray.Count());
		CleanupStack::PopAndDestroy(); // implArray
		// Leave if there are more than one or no 
		// implementation of server logic
		User::Leave(KErrGeneral);
		}
	
	CImplementationInformation* info = implArray[0];
	if( iVersion == info->Version() )
	    {
	    LOG("No change in server logic dll version");
	    CleanupStack::PopAndDestroy(); // implArray
	    return;
	    }
	LOG("Loading Server Logic in CLbtServerLogicContainer::RunL");	
	
	// If server logic is already loaded, and if its
	// dll has changed then reload it
	if( iLogicInterface )
	    {
	    // this will unload server logic
	    delete iLogicInterface;
	    iLogicInterface = NULL;	    
        }
	// Load the dll again
    iLogicInterface = CLbtServerLogicBase::NewL(info->ImplementationUid());
    iVersion = info->Version();
    
    CleanupStack::PopAndDestroy(); // implArray
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::RunError
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogicContainer::RunError(TInt /*aError*/)
	{
	FUNC_ENTER("CLbtServerLogicContainer::RunError");		
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtServerLogicContainer::DoCancel()
	{
	FUNC_ENTER("CLbtServerLogicContainer::DoCancel");		
	if(iEComSession)
		{
		// cancel request for ecom notification
		// Note that although this takes a iStatus parameter, this is a
		// synchronous call.
		iEComSession->CancelNotifyOnChange(iStatus);
		}
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicContainer::UnloadServerLogicL
// ---------------------------------------------------------------------------
//
void CLbtServerLogicContainer::UnloadServerLogicL()
    {
    FUNC_ENTER("CLbtServerLogicContainer::UnloadServerLogicL");		
    // Deleting the logic interface will unload 
    // server logic
    delete iLogicInterface;
    iLogicInterface = NULL;
    
    // Cancel ECom upgrade notification
    Cancel();
    }

// end of file
