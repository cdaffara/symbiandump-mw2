/*
* ==============================================================================
*  Name        : epos_csuplsessionretryq.cpp
*  Part of     : SUPL Framework / SUPL Server
*  Description : Class to handle retries of failed requests. It waits for all the current sessions to end and then
*  				 notifies the failed sessions that they can retry.
*  Version     :
*
*  Copyright (c) 2010 Nokia Corporation.
*  This material, including documentation and any related
*  computer programs, is protected by copyright controlled by
*  Nokia Corporation. All rights are reserved. Copying,
*  including reproducing, storing, adapting or translating, any
*  or all of this material requires the prior written consent of
*  Nokia Corporation. This material also contains confidential
*  information which may not be disclosed to others without the
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/

// INCLUDE FILES
#include <e32base.h>
#include <e32debug.h>

#include "epos_csuplsessionretryq.h"
#include "epos_csuplsessionrequest.h"
#include "epos_csuplglobal.h"

//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplsessionretryq.cpp");
//#endif

// CONSTANTS
const TTimeIntervalSeconds KRecentConnectionInterval(5);
const TTimeIntervalMicroSeconds32 KSmallDelay(2000000);
const TTimeIntervalMicroSeconds32 KReissueRequestDelay(5000000);

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSessionRetryQ::CSuplSessionRetryQ()
    : CTimer(EPriorityNormal)
    {
    DEBUG_TRACE("CSuplSessionRetryQ", __LINE__)
    CActiveScheduler::Add(this);
    }

// Destructor
CSuplSessionRetryQ::~CSuplSessionRetryQ()
    {
    DEBUG_TRACE("~CSuplSessionRetryQ", __LINE__)
    Cancel();
	iSessionRequests.Close();
    }

// Two-phased constructor
CSuplSessionRetryQ* CSuplSessionRetryQ::NewL()
    {
    DEBUG_TRACE("NewL", __LINE__)
    CSuplSessionRetryQ* self = new (ELeave) CSuplSessionRetryQ();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// EPOC default constructor
void CSuplSessionRetryQ::ConstructL()
    {
    DEBUG_TRACE("ConstructL", __LINE__)
    CTimer::ConstructL();
    }


void CSuplSessionRetryQ::AddToQueueL(CSuplSessionRequest& aSessionRequest)
	{
    TBuf<256> msg(_L("AddToQueueL, count = "));
    msg.AppendNum(iSessionRequests.Count()+1);
    SuplGlobal::Trace(msg, KTraceFileName, __LINE__);   
	// if there is no open connection and there is nothing else in the queue then check when it's possible to 
	// re-issue, otherwise just leave in the queue
    if(iSessionRequests.Count() == 0 && iSessionCount == 0)
		{
		TTime now;
		now.UniversalTime();
		// if there was some recent connection made that was closed then ask for re-issue straight away
		if(iSomeSessionEnded && now - KRecentConnectionInterval < iTimeOfLastEndedSession)
			{
			DEBUG_TRACE("Re-issuing request straight away", __LINE__)
			RDebug::Printf("[LBS][SUPL]Re-issuing request straight away");	
			After(KSmallDelay);
			}
		// if there is no previous request then wait a little before re-issuing request
		else
			{
			DEBUG_TRACE("Re-issuing request after a delay", __LINE__)
			RDebug::Printf("[LBS][SUPL]Re-issuing request after a delay");	
			After(KReissueRequestDelay);
			}
		}
	iSessionRequests.AppendL(&aSessionRequest);
	}

void CSuplSessionRetryQ::RemoveFromQueueL(CSuplSessionRequest& aSessionRequest)
	{
	TInt index = 0;
	index = iSessionRequests.Find(&aSessionRequest);
	if(index != KErrNotFound)
		{
		iSessionRequests.Remove(index);
		
	    TBuf<256> msg(_L("Removed from queue, count = "));
	    msg.AppendNum(iSessionRequests.Count());
	    SuplGlobal::Trace(msg, KTraceFileName, __LINE__);   
		
		}
	}

void CSuplSessionRetryQ::SessionStarted()
	{
    ++iSessionCount;

    
    TBuf<256> msg(_L("SessionStarted, session count = "));
    msg.AppendNum(iSessionCount);
    SuplGlobal::Trace(msg, KTraceFileName, __LINE__);   
	}

void CSuplSessionRetryQ::SessionEnded()
	{
	--iSessionCount;
	
    TBuf<256> msg(_L("SessionEnded, session count = "));
    msg.AppendNum(iSessionCount);
    SuplGlobal::Trace(msg, KTraceFileName, __LINE__);   
	
	
	if(iSessionCount == 0 && iSessionRequests.Count() > 0)
		{
		DEBUG_TRACE("Re-issuing request straight away", __LINE__)
		After(KSmallDelay);
		}
	// If a connection just closed remember the time-stamp so later it can be decided when to re-issue
	iSomeSessionEnded = ETrue;
	iTimeOfLastEndedSession.UniversalTime();
	}

// ---------------------------------------------------------
// CSuplSessionRetryQ::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRetryQ::RunL()
    {
    TBuf<256> msg(_L("RunL, session count = "));
    msg.AppendNum(iSessionCount);
    msg.Append(_L(", queue count = "));
    msg.AppendNum(iSessionRequests.Count());
    SuplGlobal::Trace(msg, KTraceFileName, __LINE__);   
   
	// If a connection is already open, wait until it finishes
	if(iSessionCount == 0 && iSessionRequests.Count() > 0)
		{
		DEBUG_TRACE("Re-issuing request now", __LINE__)
		// re-issue request for the first item in the queue
		TRAPD(err, iSessionRequests[0]->ReIssueRequestL());
		while(err!=KErrNone && iSessionRequests.Count() > 1)
			{
			// if for some reason re-issuing doesn't work then just try the next requestor and forget about the first one
			DEBUG_TRACE("Re-issue failed, trying next one in the queue", __LINE__)
			iSessionRequests.Remove(0);
			TRAP(err, iSessionRequests[0]->ReIssueRequestL());
			}
		if(err!=KErrNone)
			{
			DEBUG_TRACE("Re-issue failed", __LINE__)
			}
		iSessionRequests.Remove(0);
		}
    }


