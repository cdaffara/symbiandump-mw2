/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Listener class for Ecom events
*
*/



#include "epos_csuplecomeventwatcher.h"


// ================= MEMBER FUNCTIONS =======================

//-------------------------------------------
// Two phased constructor.
//-------------------------------------------
//
CSuplEcomEventWatcher* CSuplEcomEventWatcher::NewL(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId)
	{
	CSuplEcomEventWatcher* self = new(ELeave) CSuplEcomEventWatcher(aSuplEcomEventObserver, aInterfaceUid, aImplId);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
//-------------------------------------------
// Constructor.
//-------------------------------------------
//
CSuplEcomEventWatcher::CSuplEcomEventWatcher(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId)
 : CActive(EPriorityStandard),
   iSuplEcomEventObserver(aSuplEcomEventObserver),
   iEComSession(NULL),
   iPlugInInterfaceUid(aInterfaceUid),
   iPlugInImplUid(aImplId),
   iSuplEcomPlugInStatus(ENotExist) 
	{
	CActiveScheduler::Add(this);
	}

//-------------------------------------------
// ConstructL.
//-------------------------------------------
//
void CSuplEcomEventWatcher::ConstructL()
	{
	
	REComSession& ecomSession = REComSession::OpenL();
	
	iEComSession = &ecomSession;	
		
	TBool isImplPresent = IsImplementationExistL();	
	if(isImplPresent == EFalse)
		{
		User::Leave(KErrNotFound);
		}
		
	iSuplEcomPlugInStatus = EExist ;	
	//NotifyOnPlugInUnInstallation();
	}

//-------------------------------------------
// Destructor.
//-------------------------------------------
//
CSuplEcomEventWatcher::~CSuplEcomEventWatcher()
	{
	Cancel();
	if(iEComSession)
	iEComSession->Close();
	}

// NotifyOnPlugInUnInstallation
void CSuplEcomEventWatcher::NotifyOnPlugInUnInstallation()
	{
	// request ecom framework for notification
	iEComSession->NotifyOnChange(iStatus);
	SetActive();		
	}

//---------------------------------------------------------------
// Implementation of RunL.
// RunL() calls observer method if there is any uninstallation in
// already instantiated handler plug-in.
//---------------------------------------------------------------
//
void CSuplEcomEventWatcher::RunL()
	{
	// request ecom framework for notification
	NotifyOnPlugInUnInstallation();
	
	// check plug-in unload
	TBool isImplPresent = IsImplementationExistL();	
		
	if(isImplPresent == EFalse)	
		{
		if(iSuplEcomPlugInStatus == EExist)
			{
			iSuplEcomPlugInStatus = ENotExist ;
			// here to inform observer
			iSuplEcomEventObserver.PlugInUninstalled();	
			}
		}	

	}

//-------------------------------------------
// RunError.
//-------------------------------------------
//
TInt CSuplEcomEventWatcher::RunError(TInt /*aError*/)
	{
	
	return KErrNone;	
	}

//-------------------------------------------
// DoCancel.
//-------------------------------------------
//
void CSuplEcomEventWatcher::DoCancel()
	{
	if(iEComSession)
		{		
		// cancel request for ecom notification
		iEComSession->CancelNotifyOnChange(iStatus);
		}
	}
	

//-------------------------------------------------------------------------------------
// IsImplementationExist.
// This method checks the existance of specific plug-in and returns result accordingly.
// (other items were commented in a header).
//-------------------------------------------------------------------------------------
//
TBool CSuplEcomEventWatcher::IsImplementationExistL()
	{
	TBool retVal = EFalse;
    RImplInfoPtrArray implInfoArray;
    
	REComSession::ListImplementationsL(iPlugInInterfaceUid, implInfoArray);
	TInt implementationCount = implInfoArray.Count();
	
	for(TInt loop=0; loop < implementationCount; loop++)
		{
		if(implInfoArray[loop]->ImplementationUid() == iPlugInImplUid)
			{
			retVal = ETrue;
			break;
			}
		}
	
	implInfoArray.ResetAndDestroy();
	
    return retVal;
	}
//End of File



