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
* Description:  CActive derived class for making asynchronous request to TI API
*
*/

#include <lbs/supltiapi.h>
#include <lbscommon.h> // for TPositionModuleInfo

#include "lbssupltiapirequestor.h"
#include "epos_suplterminalconstants.h"
#include "epos_tiapiimpltrace.h"


const TInt KSetBasedBits = 5; // Assisted|Network|Terminal = 101
const TInt KSetAssistedBits = 6;//Assisted|Network|Terminal = 110
const TInt KNetworkBits = 2;//Assisted|Network|Terminal = 010

_LIT(KTraceFileName,"lbssupltiapirequestor.cpp");

// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::CCLbsSuplTiApiRequestor
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiApiRequestor::CCLbsSuplTiApiRequestor(MLbsSuplTiObserver& aObserver,RSuplTerminalServer& aServer) :CActive(EPriorityStandard), // Standard priority
    iServer(aServer),iObserver(aObserver)
    {    
    }

// -----------------------------------------------------------------------------
//  CCLbsSuplTiApiRequestor::NewLC
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiApiRequestor* CCLbsSuplTiApiRequestor::NewLC(MLbsSuplTiObserver& aObserver,RSuplTerminalServer& aServer)
    {
    CCLbsSuplTiApiRequestor* self = new (ELeave) CCLbsSuplTiApiRequestor(aObserver,aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::NewL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiApiRequestor* CCLbsSuplTiApiRequestor::NewL(MLbsSuplTiObserver& aObserver,RSuplTerminalServer& aServer)
    {
    CCLbsSuplTiApiRequestor* self = CCLbsSuplTiApiRequestor::NewLC(aObserver,aServer);
    CleanupStack::Pop(); // self;
    return self;
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::ConstructL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiApiRequestor::ConstructL()
    {
    CActiveScheduler::Add(this); // Add to scheduler    
    iTrace = COMASuplTrace::NewL(); 
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::ConstructL()"), KTraceFileName, __LINE__); 
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::~CCLbsSuplTiApiRequestor
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
CCLbsSuplTiApiRequestor::~CCLbsSuplTiApiRequestor()
    {
    Cancel(); // Cancel any request, if outstanding
    if(iSubSession.SubSessionHandle() != NULL)
        iSubSession.Close(); // Close sub-session
    if(iTrace)
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::~CCLbsSuplTiApiRequestor..."), KTraceFileName, __LINE__); 
        delete iTrace;
        iTrace = NULL;
        }
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::DoCancel
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiApiRequestor::DoCancel()
    {
    iTrace->Trace(_L("CLbsSuplTiApiImplementation::DoCancel()"), KTraceFileName, __LINE__);
    if(iSubSession.SubSessionHandle() != NULL)
        {
        iTrace->Trace(_L("CLbsSuplTiApiImplementation::subsession handle not NULL calling cancel runsession)"), KTraceFileName, __LINE__);
        iSubSession.CancelRunSession();
        iObserver.RequestComplete(KErrCancel,iNetSessionId);
        }
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::RequestLocation
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiApiRequestor::RequestLocation(const TLbsNetSessionId& aSessionId, const TLbsNetPosRequestOptionsBase& aOptions, const TLbsNetPosRequestMethod& aMethod)
    {
    iTrace->Trace(_L("CCLbsSuplTiApiRequestor::RequestLocation()"), KTraceFileName, __LINE__);
    Cancel(); // Cancel any request, just to be sure
    iState = EUninitialized;
    iNetSessionId = aSessionId;
    if(iSubSession.SubSessionHandle() == NULL)
        {
        TInt error = iSubSession.Open( iServer );
        
        //If can't open session signal error to client
        if ( error != KErrNone )
            {
            iStatus = KRequestPending;
            iTrace->Trace(_L("Subsession handle open failed"), KTraceFileName, __LINE__);
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, error );
            return;
            }
        iTrace->Trace(_L("Subsession handle open succeeded"), KTraceFileName, __LINE__);
        }
    
    TInt setCaps = ProcessPosMethods(aMethod);
    if ( setCaps < 0 )
        {
        iStatus = KRequestPending;
        iTrace->Trace(_L("process pos methods failed"), KTraceFileName, __LINE__);
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrArgument );
        return;
        }
    else
    	{
    	TBuf<32> setcapsbuf;
    	setcapsbuf.Copy(_L("Final Set Caps = "));
    	setcapsbuf.AppendNum(setCaps);
    	iTrace->Trace(setcapsbuf,KTraceFileName,__LINE__);
    	}
    //convert TLbsNetPosRequestOptions to TSuplTerminalQop
    TLbsNetPosRequestQuality quality;

    TLbsNetPosRequestOptionsBase& optionsbase = const_cast<TLbsNetPosRequestOptionsBase&>(aOptions);

    TLbsNetPosRequestOptions& options = static_cast<TLbsNetPosRequestOptions&>(optionsbase);

    options.GetRequestQuality(quality);

     if(quality.MinHorizontalAccuracy() != 0)//mandatory value has been set, so use RunSession with Qop
        {
        TSuplTerminalQop qop;
        qop.SetHorizontalAccuracy(quality.MinHorizontalAccuracy());
        
        if(quality.MinVerticalAccuracy() != 0)
            qop.SetVerticalAccuracy(quality.MinVerticalAccuracy());
        
        TInt maxAge = quality.MaxFixAge().Int64()/1000000;
        if(maxAge != 0)
            qop.SetMaxLocationAge(maxAge);
        
        TInt delay = quality.MaxFixTime().Int64()/1000000;
        if(delay != 0)
            qop.SetDelay(delay);
        
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor:Calling RunSession-with qop "), KTraceFileName, __LINE__);
         //use terminal initiation library to make request
         iSubSession.RunSession(iStatus,qop,setCaps,iNetSessionId.SessionNum(),options.NewClientConnected());
         SetActive(); // Tell scheduler a request is active
        }
    else // no qop available so call run session without qop
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor:Calling Run Session-no qop "), KTraceFileName, __LINE__);
        //use terminal initiation library to make request
        iSubSession.RunSession(iStatus,setCaps,iNetSessionId.SessionNum(),options.NewClientConnected());
        SetActive(); // Tell scheduler a request is active
        }

    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::RunL
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
void CCLbsSuplTiApiRequestor::RunL()
    {
    if (iState == EUninitialized)
        {
        iState = EInitialized;
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor :Run Session completed"), KTraceFileName, __LINE__);
        iObserver.RequestComplete(iStatus.Int(),iNetSessionId);
        }
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::RunError
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CCLbsSuplTiApiRequestor::RunError(TInt aError)
    {
    iTrace->Trace(_L("CCLbsSuplTiApiRequestor :RunError called"), KTraceFileName, __LINE__);
    iObserver.RequestComplete(aError,iNetSessionId);
    return aError;
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::GetPosition
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CCLbsSuplTiApiRequestor::GetPosition(TPositionInfoBase& aPositionInfo)
    {
    // if run session was called before calling GetPosition and completed successfully
    if(iState == EInitialized && iStatus.Int() == KErrNone) 
        {
        //Runsession has completed, so call get position and return the position
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor :Calling get position"), KTraceFileName, __LINE__);
        return iSubSession.GetPosition(aPositionInfo);
        }
    else
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::GetPosition returning -1"), KTraceFileName, __LINE__);
        return KErrNotFound; //Check this error code
        }
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::GetSessionId
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TLbsNetSessionId CCLbsSuplTiApiRequestor::GetSessionId()
    {
    return iNetSessionId;
    }
// -----------------------------------------------------------------------------
// CCLbsSuplTiApiRequestor::ProcessPosMethods
// Other items were commented in a header
// -----------------------------------------------------------------------------
//
TInt CCLbsSuplTiApiRequestor::ProcessPosMethods( const TLbsNetPosRequestMethod& aMethod)
    {
    TInt setcaps = 0;
    
    TInt numOfPosMethods = aMethod.NumPosMethods();
    if(numOfPosMethods == 0)
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::ProcessPosMethods pos methods zero"), KTraceFileName, __LINE__);
        return KErrArgument;
        }
    TInt methods = 0;
    for(TInt methodIter = 0; methodIter < numOfPosMethods; methodIter++)
        {
        TLbsNetPosMethod posMethod;
        aMethod.GetPosMethod(methodIter,posMethod);
        methods |= posMethod.PosMode();
        }
    if(methods == KSetBasedBits)
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::ProcessPosMethods Recieved SET Based"), KTraceFileName, __LINE__);
        setcaps |= KGpsSETBased;
        }
    else if(methods == KSetAssistedBits)
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::ProcessPosMethods Recieved SET Assisted"), KTraceFileName, __LINE__);
        setcaps |= KGpsSETAssisted;
        }
    else if(methods == KNetworkBits)
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::ProcessPosMethods Recieved only Network"), KTraceFileName, __LINE__);
        setcaps |= KAutonomousGps|KAFLT |KECID|KEOTD|KOTDOA;
        }
    else
        {
        iTrace->Trace(_L("CCLbsSuplTiApiRequestor::ProcessPosMethods enabling all"), KTraceFileName, __LINE__);
        setcaps |= KGpsSETAssisted | KGpsSETBased | KAutonomousGps|KAFLT |  
                       KECID|KEOTD|KOTDOA;
        }
    return setcaps;
    }
