/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Observer of ECOM events. 
*
*/



#include "epos_comasuplecomeventwatcher.h"


// ================= MEMBER FUNCTIONS =======================

//-------------------------------------------
// Two phased constructor.
//-------------------------------------------
//
COMASuplEcomEventWatcher* COMASuplEcomEventWatcher::NewL(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId)
	{
	COMASuplEcomEventWatcher* self = new(ELeave) COMASuplEcomEventWatcher(aSuplEcomEventObserver, aInterfaceUid, aImplId);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
//-------------------------------------------
// Constructor.
//-------------------------------------------
//
COMASuplEcomEventWatcher::COMASuplEcomEventWatcher(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId)
 : CActive(EPriorityStandard),
   iSuplEcomEventObserver(aSuplEcomEventObserver),
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
void COMASuplEcomEventWatcher::ConstructL()
	{
	
	REComSession& ecomSession = REComSession::OpenL();
	
	iEComSession = &ecomSession;
	
	TBool isImplPresent = IsImplementationExistL();	
	if(isImplPresent == EFalse)
		{
		User::Leave(KErrNotFound);
		}
		
	iSuplEcomPlugInStatus = EExist ;
	
	}

//-------------------------------------------
// Destructor.
//-------------------------------------------
//
COMASuplEcomEventWatcher::~COMASuplEcomEventWatcher()
	{
	Cancel();
	if(iEComSession)
	iEComSession->Close();
	}

//---------------------------------------------------------------
// Implementation of RunL.
// RunL() calls observer method if there is any uninstallation in
// already instantiated handler plug-in.
//---------------------------------------------------------------
//
void COMASuplEcomEventWatcher::RunL()
	{
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

	// request ecom framework for notification
	iEComSession->NotifyOnChange(iStatus);
	SetActive();	
	}

//-------------------------------------------
// RunError.
//-------------------------------------------
//
TInt COMASuplEcomEventWatcher::RunError(TInt /* aError */)
	{
	
	return KErrNone;	
	}

//-------------------------------------------
// DoCancel.
//-------------------------------------------
//
void COMASuplEcomEventWatcher::DoCancel()
	{
	
	// cancel request for ecom notification
	iEComSession->CancelNotifyOnChange(iStatus);
	}
	
//-------------------------------------------------------------------------------------
// IsImplementationExist.
// This method checks the existance of specific plug-in and returns result accordingly.
// (other items were commented in a header).
//-------------------------------------------------------------------------------------
//
TBool COMASuplEcomEventWatcher::IsImplementationExistL()
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

// NotifyOnPlugInUnInstallation
void COMASuplEcomEventWatcher::NotifyOnPlugInUnInstallation()
	{
	// request ecom framework for notification
	iEComSession->NotifyOnChange(iStatus);
	SetActive();		
	}



// end of file


