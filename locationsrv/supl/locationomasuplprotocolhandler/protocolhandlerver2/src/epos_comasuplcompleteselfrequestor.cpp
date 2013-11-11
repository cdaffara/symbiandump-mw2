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
* Description:   Class for doing asynchronous service for relinquish control.
*
*/



#include "epos_comasuplcompleteselfrequestor.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::COMASuplCompleteSelfRequestor
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplCompleteSelfRequestor::COMASuplCompleteSelfRequestor(MCompleteSelfRequest& aObserver):
    				 CActive( EPriorityStandard ),iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplCompleteSelfRequestor::ConstructL()
    {
    	CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplCompleteSelfRequestor* COMASuplCompleteSelfRequestor::NewL(MCompleteSelfRequest& aObserver)
    {
	    COMASuplCompleteSelfRequestor* self =  new ( ELeave ) 
	    									   COMASuplCompleteSelfRequestor(aObserver);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::~COMASuplCompleteSelfRequestor
// -----------------------------------------------------------------------------
//
COMASuplCompleteSelfRequestor::~COMASuplCompleteSelfRequestor()
    {
    	Cancel();
    }

// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::CompleteSelf
// -----------------------------------------------------------------------------
//
void COMASuplCompleteSelfRequestor::CompleteSelf()
    {
    		iCompStatus = ESUPL_PACKET_RECEIVE;
			TRequestStatus* pStat = &iStatus;
			User::RequestComplete(pStat, KErrNone);
			SetActive();
    }
// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::CompleteInvalidSession
// -----------------------------------------------------------------------------
//
void COMASuplCompleteSelfRequestor::CompleteInvalidSession()
    {	
    		iCompStatus = ESUPL_INVALID_SESSIONID;
			TRequestStatus* pStat = &iStatus;
			User::RequestComplete(pStat, KErrNone);
			SetActive();
    }
  
// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::CompleteInvalidSession
// -----------------------------------------------------------------------------
//
void COMASuplCompleteSelfRequestor::CompleteInvalidSLPSession()
	{
	   		iCompStatus = ESUPL_INVALID_SLP_SESSIONID;
			TRequestStatus* pStat = &iStatus;
			User::RequestComplete(pStat, KErrNone);
			SetActive();

	}
// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::RunL
// -----------------------------------------------------------------------------
//
void COMASuplCompleteSelfRequestor::RunL()
    {
    	switch(iCompStatus)
    	{
    		case ESUPL_PACKET_RECEIVE:
    			iObserver.RequestCompletedL();	
    			break;
    		case ESUPL_INVALID_SESSIONID:
    			iObserver.StartInvalidSessionL();
    			break;
    		case ESUPL_INVALID_SLP_SESSIONID:
				iObserver.HandleInvalidSLPSession();
				break;
    	}
    	
    }

// -----------------------------------------------------------------------------
// COMASuplCompleteSelfRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplCompleteSelfRequestor::DoCancel()
    {    
    	
    }

//  End of File
