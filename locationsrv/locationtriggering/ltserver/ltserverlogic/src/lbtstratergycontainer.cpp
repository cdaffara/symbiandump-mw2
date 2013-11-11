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
*				 the stratergy ecom plugin
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <ecom.h>
#include <implementationinformation.h>
#include <e32property.h>
#include "lbtstratergycontainer.h"
#include "lbtstatuspskeys.h"
#include "lbtstrategybase.h"
#include "lbttriggeringsupervisionobserver.h"
#include "lbtlogger.h"

// ======== LOCAL FUNCTIONS =========
// static cleanup function
static void RImpleInfoPtrArrayCleanup( TAny* aArray )
	{
	static_cast<RImplInfoPtrArray*>( aArray )->ResetAndDestroy();
	}

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtStratergyContainer* CLbtStratergyContainer::NewL()
	{
	FUNC_ENTER("CLbtStratergyContainer::NewL");
	CLbtStratergyContainer* self = new ( ELeave ) CLbtStratergyContainer;
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop();
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtStratergyContainer::ConstructL()
	{
	FUNC_ENTER("CLbtStratergyContainer::ConstructL");
	iEComSession = &(REComSession::OpenL());
	iStratergy = NULL;
	
	User::LeaveIfError( iProperty.Attach(KPSUidLbtStatusInformation, 
                                         KLbtLocationTriggeringSupervisionStatus,
                                         EOwnerThread) );
  }

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::~CLbtStratergyContainer
// ---------------------------------------------------------------------------
//
CLbtStratergyContainer::~CLbtStratergyContainer()
	{
	FUNC_ENTER("CLbtStratergyContainer::~CLbtStratergyContainer");
	delete iStratergy;
	Cancel();
	if(iEComSession)
		{
		iEComSession->Close();
		}
	
    // This has to be called to free resources by
    // ECom Framework. Failing this call memory is leaked
	REComSession::FinalClose();
	iProperty.Close();
	}
	
// ---------------------------------------------------------------------------
// CLbtStratergyContainer::CLbtStratergyContainer
// ---------------------------------------------------------------------------
//
CLbtStratergyContainer::CLbtStratergyContainer()
    : CActive(EPriorityStandard)
	{
	FUNC_ENTER("CLbtStratergyContainer::CLbtStratergyContainer");
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::LoadStratergyL
// ---------------------------------------------------------------------------
//
void CLbtStratergyContainer::LoadStratergyL(MLbtTriggeringSupervisionObserver* aPtr)
	{
	FUNC_ENTER("CLbtStratergyContainer::LoadStratergyL");
	RImplInfoPtrArray implArray;
	REComSession::ListImplementationsL(KLocAcquisitionStrategyInterfaceUid, implArray);	
	if(implArray.Count() != 1)
		{
		implArray.ResetAndDestroy();
		// Leave if there are more than one or no 
		// implementation of stratergy
		User::Leave(KErrNotFound);
		}
	TCleanupItem arrayCleanup( RImpleInfoPtrArrayCleanup, &implArray );
	CleanupStack::PushL(arrayCleanup);
	CImplementationInformation* info = implArray[0];
	iStratergy = CLbtStrategyBase::NewL(aPtr);
	iVersion = info->Version();
	CleanupStack::PopAndDestroy(); // implArray
	
	iProperty.Set(KPSUidLbtStatusInformation, 
                  KLbtLocationTriggeringSupervisionStatus, 
                  ELbtLocationTriggeringSupervisionSuccessful);
                                     
  // request ecom framework for ecom change notification
	iEComSession->NotifyOnChange(iStatus);	
	SetActive();
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::Stratergy
// ---------------------------------------------------------------------------
//
CLbtStrategyBase* CLbtStratergyContainer::Stratergy()
	{
	FUNC_ENTER("CLbtStratergyContainer::Stratergy");
	return iStratergy;
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::RunL
// ---------------------------------------------------------------------------
//
void CLbtStratergyContainer::RunL()
	{
	FUNC_ENTER("CLbtStratergyContainer::RunL");
	// request ecom framework for notification
	iEComSession->NotifyOnChange(iStatus);
	SetActive();
	
	RImplInfoPtrArray implArray;
	REComSession::ListImplementationsL(KLocAcquisitionStrategyInterfaceUid, implArray);
	CleanupClosePushL(implArray);

	CImplementationInformation* info = implArray[0];
	
	// If stratergy is already loaded, and if its
	// dll has changed then reload it
	if(info->Version() != iVersion && iStratergy )
	    {
	    // this will unload stratergy
	    delete iStratergy;
	    iStratergy = NULL;
	    // Load the dll again
	    iStratergy = CLbtStrategyBase::NewL(this);
	    iVersion = info->Version();
        }
    CleanupStack::PopAndDestroy(); // implArray
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::RunError
// ---------------------------------------------------------------------------
//
TInt CLbtStratergyContainer::RunError(TInt /*aError*/)
	{
	FUNC_ENTER("CLbtStratergyContainer::RunError");
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtStratergyContainer::DoCancel()
	{
	FUNC_ENTER("CLbtStratergyContainer::DoCancel");
	if(iEComSession)
		{
		// cancel request for ecom notification
		// Note that although this takes a iStatus parameter, this is a
		// synchronous call.
		iEComSession->CancelNotifyOnChange(iStatus);
		}
	}

// ---------------------------------------------------------------------------
// CLbtStratergyContainer::UnloadStratergy
// ---------------------------------------------------------------------------
//
void CLbtStratergyContainer::UnloadStratergy()
    {
    FUNC_ENTER("CLbtStratergyContainer::UnloadStratergy");
    // Deleting the interface will unload 
    // Stratergy
    delete iStratergy;
    iStratergy = NULL;
    
    iProperty.Set(KPSUidLbtStatusInformation, 
                  KLbtLocationTriggeringSupervisionStatus, 
                  ELbtLocationTriggeringSupervisionNotActive);
                                     
    // Cancel ECom upgrade notification
    Cancel();
    }

// end of file
