/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Main class for server side sessions
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <centralrepository.h>

#include "epos_suplterminalipc.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_suplnetworkipc.h"

//#ifdef _DEBUG
//#include "epos_cpostrace.h"
//#endif

#include "epos_csuplserver.h"
#include "epos_csuplsubsessionregistry.h"
#include "epos_csuplglobal.h"
#include "epos_csuplipcsubsession.h"
#include "epos_csuplipcsession.h"
#include "epos_csuplnetinitiatedsession.h"
#include "epos_csuplnetinitiatedregistry.h"
#include "epos_csuplconnecttimer.h"
#include "epos_mconnecttimerobserver.h"
#include "epos_csuplsessionmanager.h"


// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplipcsession.cpp");
//#endif

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplIPCSession::CSuplIPCSession(CSuplServer& aServer, CSuplSessionManager& aSessnMgr):
	iSuplServer(aServer),
	iSessionManager(aSessnMgr)
    {      
    }

// Destructor
CSuplIPCSession::~CSuplIPCSession()
    {
    DEBUG_TRACE("CSuplIPCSession::~CSuplIPCSession", __LINE__)
    delete iSubSessionRegistry;
	delete iConnectTimer;
    // The flag iServerShutdown prevents calling the server or its members if
    // it is already destroyed.
    if (!iServerShutdown)
        {
        if (iDecrementSessions)
            {
            iSuplServer.DecrementSessions();
            }
        }
    }

// Two-phased constructor
CSuplIPCSession* CSuplIPCSession::NewL(CSuplServer& aServer, CSuplSessionManager& aSessnMgr, TInt aIpcSessionId)
    {
    DEBUG_TRACE("CSuplIPCSession::NewL", __LINE__)
    CSuplIPCSession* self = new (ELeave)
        CSuplIPCSession(aServer, aSessnMgr);
    CleanupStack::PushL(self);
    self->ConstructL(aIpcSessionId);
    CleanupStack::Pop(self);
    return self;
    }

// EPOC default constructor
void CSuplIPCSession::ConstructL(TInt aIpcSessionId)
    {
    DEBUG_TRACE("CSuplIPCSession::ConstructL", __LINE__)
    iSubSessionRegistry = CSuplSubsessionRegistry::NewL();
	iConnectTimer = CSuplConnectTimer::NewL(this);
    // This call will also cancel any server shutdown in progress.
    iSuplServer.IncrementSessions();
    iIpcSessionId = aIpcSessionId;
    iDecrementSessions = ETrue;
    
    }
    
// ---------------------------------------------------------
// CSuplIPCSession::ForwardMessageL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::ForwardMessageL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSession::ForwardMessageL", __LINE__)

	if (aMessage.SecureId() != KSuplWapSecureId)
	    if (!aMessage.HasCapability(ECapabilityCommDD))
	     	{
	     	DEBUG_TRACE("CSuplServer::Invalid UID", __LINE__)
	     	RequestComplete(aMessage, KErrPermissionDenied);
	     	}
    TInt dataSize = aMessage.GetDesLength(0);
    if(dataSize>0)
    	{
    	TRAPD(err,iSuplServer.HandleNewNetSessionL(iSessionManager,aMessage));
    	if(err != KErrNone)
    	    {
            RequestComplete(aMessage,err);
    	    }
    	}
    else
    	{
        RequestComplete(aMessage,KErrArgument);
    	}
	}

// ---------------------------------------------------------
// CSuplIPCSession::ServiceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::ServiceL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSession::ServiceL", __LINE__)
    iMessage = aMessage;
	switch (aMessage.Function())
        {
        case ESuplTerminalSubssnOpen:
        	{
     	    TInt service = aMessage.Int0();
     	    
     	    switch( service )
     	    	{
     	    	case 1:
     	    		{
     	    		iSuplService = RSuplTerminalSubSession::ESUPL_1_0;	
     	    		break;
     	    		}
     	    	case 2:
     	    		{
     	    		iSuplService = RSuplTerminalSubSession::ESUPL_2_0;	
     	    		break;
     	    		}
     	    	default:
     	    		{
     	    		iSuplService = RSuplTerminalSubSession::ESUPL_1_0;	
     	    		break;
     	    		}
     	    	}

           	TBool connect=iSuplServer.GetConnectFlag(); 
           	TInt connectError=iSessionManager.GetConnectError();
            if (connect && connectError==KErrNone)
                CreateSubSessionL(aMessage, CSUPLProtocolManagerBase::ETerminal, iSuplService );
            else
                {                	
                if(connect==FALSE && connectError==KErrNone)
                	{
                	iConnectCount = 0;
                	iReqType = ETerminalConnectOpen;
                	iConnectTimer->Start(250000);
                	}
                else
                	RequestComplete(aMessage,KErrGeneral);
                }                        
            break;
        	}
	    case ESuplTerminalSubssnClose:
            {
            CloseSubsessionL(aMessage);            	
            }
		    break;
        case ESuplNetworkForwardSuplMsg:
            {
           	TBool connect=iSuplServer.GetConnectFlag(); 
           	TInt connectError=iSessionManager.GetConnectError();
            if (connect && connectError==KErrNone)
        	    ForwardMessageL(aMessage);
            else
                {                
                if(!connect && connectError==KErrNone)
                 	{
                	iConnectCount = 0;
                 	iReqType = ENetworkConnectForwardMessage;
                 	iConnectTimer->Start(250000);	
                 	}
                else
                	RequestComplete(aMessage,KErrGeneral);
                }                        
		    break;
            }
        default:
            ForwardToSubSessionL(aMessage);
		    break;
        }
    }

// ---------------------------------------------------------
// CSuplIPCSession::NotifyServerShutdown
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::NotifyServerShutdown()
    {
    DEBUG_TRACE("CSuplIPCSession::NotifyServerShutdown", __LINE__)
    iServerShutdown = ETrue;
    TInt nrOfSubSessions = iSubSessionRegistry->Count();
    for (TInt i = 0; i < nrOfSubSessions; i++)
        {
        CSuplIPCSubSession * subSession = 
            static_cast <CSuplIPCSubSession *> 
            (iSubSessionRegistry->SubSessionFromIndex(i));
        if (subSession)
			{
			subSession->NotifyServerShutdown();
			}
        }
    }

// ---------------------------------------------------------
// CSuplIPCSession::CloseSubsession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::CloseSubsessionL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSession::CloseSubsessionL", __LINE__)
    
    TInt handle = aMessage.Int3();

	CSuplIPCSubSession* subSession =
        iSubSessionRegistry->SubSessionFromHandleL(aMessage.Int3());
        
	subSession->DestroySession(iSessionManager);
	            
    iSubSessionRegistry->CloseSubSession(handle);
	RequestComplete(aMessage, KErrNone);

    DEBUG_TRACE("CSuplIPCSession::CloseSubsessionCompletes", __LINE__)
    }

// ---------------------------------------------------------
// CSuplIPCSession::CreateSubSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::CreateSubSessionL(
                const RMessage2& aMessage,
                CSUPLProtocolManagerBase::TSuplReqType aReqType,
                RSuplTerminalSubSession::TSuplServiceType aSuplService
            )
	{
    DEBUG_TRACE("CSuplIPCSession::CreateSubSessionL", __LINE__)
    // Create SUPL session object
    CSuplIPCSubSession* subSession = CSuplIPCSubSession::NewLC(
    									iSessionManager, 
    									aReqType, 
    									iIpcSessionId,
    									aSuplService);
	if(!subSession)
		{ 
		CleanupStack::PopAndDestroy();
		RequestComplete(aMessage, KErrGeneral);
		return;
		}
    TInt subSessionHandle = iSubSessionRegistry->AddInstanceL(subSession);
    CleanupStack::Pop(subSession); // Now owned by registry

    // Set the client subsession identifier.
    TPckg<TInt> handlePackage(subSessionHandle);
    TInt err = SuplGlobal::Write(aMessage, 3, handlePackage);
    if (err)
        {
        iSubSessionRegistry->CloseSubSession(subSessionHandle);
        User::Leave(err);
        }
    RequestComplete(aMessage, KErrNone);
	}

// ---------------------------------------------------------
// CSuplIPCSession::ForwardToSubSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::ForwardToSubSessionL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSession::ForwardtoSubSessionL", __LINE__)
    CSuplIPCSubSession* subSession =
        iSubSessionRegistry->SubSessionFromHandleL(aMessage.Int3());

    if (!subSession)
		{
        User::Leave(KErrNotSupported);
        }

    subSession->ServiceL(aMessage); 
    }

// ---------------------------------------------------------
// CSuplIPCSession::RequestComplete
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::RequestComplete(const RMessage2& aMessage, TInt aCompleteCode)
    {
	DEBUG_TRACE("CSuplIPCSession::RequestComplete", __LINE__)
    if (!aMessage.IsNull())
        {
        aMessage.Complete(aCompleteCode);
        }
    }

// ---------------------------------------------------------
// CSuplIPCSession::RequestComplete
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSession::CompleteConnectTimerL()
	{
	DEBUG_TRACE("CSuplIPCSession::CompleteConnectTimerL", __LINE__)
	TBool connect=iSuplServer.GetConnectFlag();
	
    if (connect)
    	{
        if (iReqType == ETerminalConnectOpen)        
        	{
            CreateSubSessionL(iMessage, CSUPLProtocolManagerBase::ETerminal, iSuplService);
        	}                
        else if (iReqType == ENetworkConnectForwardMessage)
        	{
            ForwardMessageL(iMessage);
        	}                
        return;
    	} 
    TInt connectError=iSessionManager.GetConnectError();
    if(connectError!=KErrNone)
    	{
    	RequestComplete(iMessage,KErrGeneral);
    	return;
    	}
    iConnectCount++;
    if (iConnectCount == 10)
    	{
        RequestComplete(iMessage,KErrServerBusy);
    	return;	
    	}            
    iConnectTimer->Start(250000);
	}

// ---------------------------------------------------------
// CSuplIPCSession::ProtocolHUnloaded
//
// (other items were commented in a header).
// ---------------------------------------------------------
//   
void CSuplIPCSession::ProtocolHUnloaded() 
	{
	DEBUG_TRACE("CSuplIPCSession::ProtocolHUnloaded", __LINE__)
	TInt subSessioncount = iSubSessionRegistry->Count();
	
	for(TInt loop=0; loop < subSessioncount;loop++)
		{
		DEBUG_TRACE("CSuplIPCSession::ProtocolHUnloaded Iterating", __LINE__)
		CSuplIPCSubSession* subSession = iSubSessionRegistry->SubSessionFromIndex(loop);
		subSession->ProtocolHUnloaded();
		}
	}

//  End of File
