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
* Description:  CCLbsSuplTiRequestManager Class Implementation
*
*/

#include <lbs/supltiapi.h>

#include "lbssupltirequestmanager.h"
#include "lbssupltiapirequestor.h"
#include "epos_tiapiimpltrace.h"


_LIT(KTraceFileName,"lbssupltiapirequestmanager.cpp");

// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::CCLbsSuplTiRequestManager
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiRequestManager::CCLbsSuplTiRequestManager(MLbsSuplTiObserver& aObserver):CActive(EPriorityStandard), iObserver(aObserver)
    {
    // No implementation required
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::~CCLbsSuplTiRequestManager
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiRequestManager::~CCLbsSuplTiRequestManager()
    {
    CloseSession();
    iRequestorCollection.Close();
    if(iTrace)
        {
        delete iTrace;
        iTrace = NULL;
        }
   if (IsActive())      
      	Cancel();
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::NewLC
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiRequestManager* CCLbsSuplTiRequestManager::NewLC(MLbsSuplTiObserver& aObserver)
    {
    CCLbsSuplTiRequestManager* self =
            new (ELeave) CCLbsSuplTiRequestManager(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::NewL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiRequestManager* CCLbsSuplTiRequestManager::NewL(MLbsSuplTiObserver& aObserver)
    {
    CCLbsSuplTiRequestManager* self = CCLbsSuplTiRequestManager::NewLC(aObserver);
    CleanupStack::Pop(); // self;
    return self;
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::ConstructL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiRequestManager::ConstructL()
    {
	CActiveScheduler::Add(this);
    iTrace = COMASuplTrace::NewL(); 
    iTrace->Trace(_L("CCLbsSuplTiRequestManager::ConstructL()"), KTraceFileName, __LINE__); 

    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::RequestLocation
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiRequestManager::RequestLocation(const TLbsNetSessionId& aSessionId, const TLbsNetPosRequestOptionsBase& aOptions, const TLbsNetPosRequestMethod& aMethod)
    {
    iTrace->Trace(_L("CCLbsSuplTiRequestManager::RequestLocation()"), KTraceFileName, __LINE__);
    
     if(!iServer.Handle())
    	{
    	iTrace->Trace(_L("Invalid Server Handle. Opening new connection"), KTraceFileName, __LINE__);
    	TInt err = iServer.Connect(iStatus);  
    	if(err != KErrNone)
    	  {
    	  iTrace->Trace(_L("Opening new connection failed"), KTraceFileName, __LINE__);
    	  iObserver.RequestComplete(err,aSessionId);
    	  return;
    	  }
    	iSessionId = aSessionId;
     	iOptions = aOptions;
     	iMethod = aMethod;
    	SetActive();
    	}
    else
    	{
    	iTrace->Trace(_L("Valid Server Handle exist.Using same one..."), KTraceFileName, __LINE__);	
    	iSessionId = aSessionId;
     	iOptions = aOptions;
     	iMethod = aMethod;
    	InitiateLocationRequest();
    	}
    
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::CancelRequest
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CCLbsSuplTiRequestManager::CancelRequest(const TLbsNetSessionId& aSessionId)
    {
    iTrace->Trace(_L("CCLbsSuplTiRequestManager::CancelRequest"), KTraceFileName, __LINE__);
    TInt index = FindSessionId(aSessionId);
    if(index != KErrNotFound)
        {
        iTrace->Trace(_L("session found calling cancel"), KTraceFileName, __LINE__);
        iRequestorCollection[index]->Cancel();
        delete iRequestorCollection[index];
        iRequestorCollection.Remove(index);
        return KErrNone;
        }
    iTrace->Trace(_L("session not found "), KTraceFileName, __LINE__);
    return KErrNotFound;
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::GetPosition
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CCLbsSuplTiRequestManager::GetPosition(const TLbsNetSessionId& aSessionId, TPositionInfoBase& aPositionInfo)
    {
    iTrace->Trace(_L("CCLbsSuplTiRequestManager::GetPosition"), KTraceFileName, __LINE__);
    TInt index = FindSessionId(aSessionId);
    if(index != KErrNotFound)
        {
        iTrace->Trace(_L("session found calling getposition"), KTraceFileName, __LINE__);
        return iRequestorCollection[index]->GetPosition(aPositionInfo);
        }
    iTrace->Trace(_L("session not found found for getposition"), KTraceFileName, __LINE__);
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::CloseSession
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiRequestManager::CloseSession()
    {
    if(iTrace)
        iTrace->Trace(_L("CCLbsSuplTiRequestManager::CloseSession() start"), KTraceFileName, __LINE__);
    iRequestorCollection.ResetAndDestroy(); //cancels all active requests
    iServer.Close();
    if(iTrace)
        iTrace->Trace(_L("CCLbsSuplTiRequestManager::CloseSession() end"), KTraceFileName, __LINE__);
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiRequestManager::FindSessionId
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CCLbsSuplTiRequestManager::FindSessionId(const TLbsNetSessionId& aSessionId)
    {
    TInt count = iRequestorCollection.Count();
    for ( TInt index = 0; index < count; index++ )
        {
        if ( iRequestorCollection[index]->GetSessionId().SessionNum() == aSessionId.SessionNum() )
            {
            return index;
            }
        }
    return KErrNotFound;
    }

void CCLbsSuplTiRequestManager::RunL()
	{
	if(iStatus.Int() == KErrNone) // if server connect was successful
	{
		iTrace->Trace(_L("CCLbsSuplTiRequestManager::RunL called successfully..."), KTraceFileName, __LINE__);
		iTrace->Trace(_L("Putting new request..."), KTraceFileName, __LINE__);
		InitiateLocationRequest();
	}	
	else //opening connection failed
		{
		iTrace->Trace(_L("Opening new connection failed"), KTraceFileName, __LINE__);
    iObserver.RequestComplete(iStatus.Int(),iSessionId);
		}
	}

   // For cancelling Ashnchronous requests
void CCLbsSuplTiRequestManager::DoCancel()
	{
	
	}

   // Override to handle leaves from RunL().
TInt CCLbsSuplTiRequestManager::RunError(TInt aError)
	{
	return aError;
	}
   
void CCLbsSuplTiRequestManager::InitiateLocationRequest()
	{
	if(FindSessionId(iSessionId) == KErrNotFound) // no request exists with this session id so make a new request
		{
		iTrace->Trace(_L("Session Id not found creating new requestor"), KTraceFileName, __LINE__);
		//Create a new requestor object
		CCLbsSuplTiApiRequestor* newRequestor = NULL;
	    // Create new requestor and add it to requestor collection,
		// ownership is transferred to the collection array
		TRAPD(err,newRequestor = CCLbsSuplTiApiRequestor::NewL(iObserver,iServer);
		         iRequestorCollection.AppendL(newRequestor));
		if(err != KErrNone)
			{
			iTrace->Trace(_L("Creating New Requestor Failed"), KTraceFileName, __LINE__);
			iObserver.RequestComplete(err,iSessionId);
			return;
			}

		//ask requestor to make location request
		newRequestor->RequestLocation(iSessionId,iOptions,iMethod);
		}
	else //request was already made with this session id
		{
		iTrace->Trace(_L("Session Id already exits"), KTraceFileName, __LINE__);
		iObserver.RequestComplete(KErrAlreadyExists,iSessionId);
		}
	}
