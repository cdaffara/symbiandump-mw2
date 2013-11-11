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
* Description:   Main server class
*
*/



// INCLUDE FILES

#include <e32base.h>
#include <e32svr.h>
#include <ecom/ecom.h>
#include <connect/sbdefs.h>
#include <centralrepository.h>
#include <featmgr.h>

#include "epos_suplterminalipc.h"
#include "epos_suplnetworkipc.h"
#include "epos_startsuplserver.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplserverprivatecrkeys.h"

#include "epos_csuplserver.h"
#include "epos_csuplserverpanic.h"
#include "epos_csuplipcsession.h"
#include "epos_csuplserverdelayedshutdown.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csuplglobal.h"

#include "epos_csuplnetinitiatedregistry.h"
#include "epos_csuplnetinitiatedsession.h"
#include "epos_mnetinitiatedobserver.h"
#include "epos_csuplserverstartuprequest.h"
#include "epos_csupldeinitrequestor.h"
#include "epos_mconnectobserver.h"

const TSecureId KNetworkGatewaySID = 0x10281D46;
const TSecureId KLocationMonitorSID = 0x102869DF;

// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplserver.cpp");
//#endif

// ================= SERVER'S POLICY =======================

//Definition of the ranges of IPC numbers
const TInt suplServerPolicyRanges[] = 
    {
    0,
    // unsupported area
    ESuplTerminalSubssnOpen,
    ESuplTerminalSubssnClose,
    ESuplTerminalSubssnSyncRunsession,
    ESuplTerminalSubssnAsyncRunsession,
    ESuplTerminalSubssnGetPosition,
    ESuplTerminalSubssnStartPeriodicTrigger,
    ESuplTerminalSubssnStartPeriodicTriggerWithServer,
    ESuplNetworkForwardSuplMsg,
    // unsupported area
    ESuplSubssnCustomCodesBegin,
    // custom request area
    }; 

// Total number of ranges
const TUint suplServerPolicyRangeCount = 
    sizeof(suplServerPolicyRanges) / sizeof(TInt);

// Types of Policies
enum TPolicies
    {
    EPolicyCommDDNeeded = 0
    };

// Specific capability checks
const CPolicyServer::TPolicyElement suplServerPolicyElements[] = 
    {
    // policy EPolicyCommDDNeeded - fail call if Location not present
    {_INIT_SECURITY_POLICY_C1(ECapabilityCommDD), CPolicyServer::EFailClient}
    };

//Policy to implement for each of the above ranges        

const TUint8 suplServerPolicyElementsIndex[suplServerPolicyRangeCount] = 
    {
    CPolicyServer::ENotSupported,           // unsupported
    CPolicyServer::ECustomCheck,	                // subsession open
    CPolicyServer::ECustomCheck,	                // subsession close
    CPolicyServer::ECustomCheck,	                // sync runsession
    CPolicyServer::ECustomCheck,	                // async runsession
    CPolicyServer::ECustomCheck,					// GetPosition
    CPolicyServer::ECustomCheck,					// StartSuplTriggerSession
    CPolicyServer::ECustomCheck,					// StartSuplTriggerSession with server    
    CPolicyServer::EAlwaysPass,				// Forward Message
    CPolicyServer::ECustomCheck,            // custom requests
    };

//Package all the above together into a policy
const CPolicyServer::TPolicy suplServerPolicy =
    {
    CPolicyServer::EAlwaysPass,             // onConnect
    //0,										 onConnect
    suplServerPolicyRangeCount,	            // number of ranges                                   
    suplServerPolicyRanges,	                // ranges array
    suplServerPolicyElementsIndex,	        // elements<->ranges index
    suplServerPolicyElements,		        // array of elements
    };
 
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplServer::CSuplServer(TInt aPriority, const TPolicy &aPolicy, TServerType aServerType)
        : CPolicyServer(aPriority, aPolicy, aServerType)
    {
    // This does not do anything.
    __DECLARE_NAME(_S("CSuplServer"));
    }
    
void ResetAndDestroy(TAny* aArray)
    {
    (static_cast<RImplInfoPtrArray*> (aArray))->ResetAndDestroy();
    }    
    
// EPOC default constructor
void CSuplServer::ConstructL() 
    {    
    DEBUG_TRACE("CSuplServer::ConstructL", __LINE__)
    
    // Settings
    TInt keyValue;
    TInt err = KErrNone;
    CRepository* repository = NULL;
    TRAP(err, repository = CRepository::NewL(KCRUidSuplConfiguration));
    CleanupStack::PushL(repository);
    
    if (err != KErrNone)
        {
        SetErrorCode(err);
        }
	if (err == KErrNone)
		{
	    TInt ret = repository->Get(KSuplConfigurationShutdownTimer, keyValue);	    
	    if (keyValue <= 0)
	        {            
	        DEBUG_TRACE("Invalid value in configuratin file for shutdown timer, using default value of 2 sec", __LINE__)
	    	iServerShutdownDelay = KSuplServerShutdownTimer;
	        }
	    else		
	    	iServerShutdownDelay = keyValue;
		}
	else    	
    	iServerShutdownDelay = KSuplServerShutdownTimer;
	
	CleanupStack::PopAndDestroy(repository);
	
	// Initialize Network Initiated registry
	iSuplNetInitRegistry=CSuplNetInitiatedRegistry::NewL();
			        	
    // Shutdown timer
    iShutdown = CSuplServerDelayedShutdown::NewL(*this);

    // Session manager
    iSessionManager = CSuplSessionManager::NewL();
    iServerStartup = CSuplServerStartupRequest::NewL(this,*iSessionManager);
    iDeInitRequest = CSuplServerDeInitRequest::NewL(*iSessionManager);
    iConnectFlag=FALSE;
    SetErrorCode(KErrNone);
    iNumNetMessages=0;
    iIpcSessionCount = 0;
    StartL(KSuplServerName);
    }

// Two-phased constructor
CSuplServer* CSuplServer::NewL()
    {
    DEBUG_TRACE("CSuplServer::NewL", __LINE__)
    FeatureManager::InitializeLibL();
	TBool fsupport;

 	// Feature flag check provided for Product profile bits specific variation
 	// If the SUPL feature flag is not defined then the SUPL Server exits 
 	// and returns KErrNotSupported
	fsupport = FeatureManager::FeatureSupported(/* KFeatureIdSuplFramework */ 1583);
    FeatureManager::UnInitializeLib();
	if (!fsupport)
        {
        DEBUG_TRACE("CSuplServer::NewL, SUPL is not enabled.  Exiting....", __LINE__)
        User::Leave(KErrNotSupported);
        }   

    DEBUG_TRACE("CSuplServer::NewL, SUPL is enabled.", __LINE__)
    CSuplServer* self = new (ELeave) CSuplServer(EPriority, suplServerPolicy);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CSuplServer::~CSuplServer()
    {
    DEBUG_TRACE("CSuplServer::~CSuplServer", __LINE__)
    delete iServerStartup;
    delete iDeInitRequest; 
    delete iShutdown;
    delete iSessionManager;
    delete iSuplNetInitRegistry;    
    }

// ---------------------------------------------------------
// CSuplServer::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplServer::RunError(TInt aError)
    {
    DEBUG_TRACE("CSuplServer::RunError", __LINE__)
    if (!Message().IsNull())
        {
        Message().Complete(aError);
        }
    ReStart();
    return KErrNone;
    }

// ---------------------------------------------------------
// CSuplServer::NewSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSession2* CSuplServer::NewSessionL(
        const TVersion& aVersion, 
        const RMessage2& aMessage) const
    {
    DEBUG_TRACE("NewSessionL", __LINE__)
    if (iErrorCode != KErrNone)
    	{
    	iShutdown->Start(iServerShutdownDelay);
    	User::Leave(KErrGeneral);
    	}
    	
    // Check we're the right version
    TVersion version(KSuplMajorVersionNumber,
                     KSuplMinorVersionNumber,
                     KSuplBuildVersionNumber);
    if (!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(KErrNotSupported);
        }

    //if secure id is not equal to KSuplWapSecureId or KSuplWapSecureId, then perform capability check
    if ((aMessage.SecureId() != KSuplWapSecureId && aMessage.SecureId() != KNetworkGatewaySID && aMessage.SecureId() != KLocationMonitorSID))
    	{
        if (!aMessage.HasCapability(ECapabilityCommDD))
            {
            DEBUG_TRACE("CSuplServer::Invalid UID", __LINE__)	
            User::Leave(KErrPermissionDenied);
            }    	
    	}
    	
	if(iConnectFlag==FALSE) 
		{		
		TUid pluginUid;
    	pluginUid.iUid = 0;
		if(!iServerStartup->IsActive())
			{	
    		pluginUid.iUid = 270562255; 
			iServerStartup->MakeSuplServerStartupRequestL(iSessionManager,pluginUid,*(const_cast<CSuplServer*>(this)));
			}
		}
    // Make new session
    CSuplIPCSession* newSession = 
        CSuplIPCSession::NewL(*(const_cast<CSuplServer*>(this)), *iSessionManager, iIpcSessionCount);

    return newSession;
    }

// ---------------------------------------------------------
// CSuplServer::IncreamentIpcSessionCount
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::IncreamentIpcSessionCount() 
    {
    iIpcSessionCount++;
    }

// ---------------------------------------------------------
// CSuplServer::CustomSecurityCheckL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPolicyServer::TCustomResult CSuplServer::CustomSecurityCheckL(
        const RMessage2& aMessage, 
        TInt& /*aAction*/, 
        TSecurityInfo& /*aMissing*/)
    {
    DEBUG_TRACE("CustomSecurityCheck", __LINE__)

    CPolicyServer::TCustomResult result = CPolicyServer::EPass;

    static _LIT_SECURITY_POLICY_S0(allowNetworkGatewayPolicy, KNetworkGatewaySID);
    TBool isNetworkGateway = allowNetworkGatewayPolicy().CheckPolicy(aMessage); 

    static _LIT_SECURITY_POLICY_S0(allowSuplWapPolicy, KSuplWapSecureId);
    TBool isSuplWap = allowSuplWapPolicy().CheckPolicy(aMessage); 
    
    static _LIT_SECURITY_POLICY_S0(allowlocationserverPolicy, KLocationMonitorSID);
       TBool islocationserver = allowlocationserverPolicy().CheckPolicy(aMessage); 
       
       //Check if the request was made by one of the allowed processes else check for required capability
       if (!isNetworkGateway && !isSuplWap && !islocationserver)
        {
        if (!aMessage.HasCapability(ECapabilityCommDD))
            {
            DEBUG_TRACE("CSuplServer::Invalid UID", __LINE__)   
            result = CPolicyServer::EFail;          
            }
        else
            result = CPolicyServer::EPass;
        }
    
    return result;
    }

// ---------------------------------------------------------
// CSuplServer::IncrementSessions
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::IncrementSessions()
    {
    DEBUG_TRACE("IncrementSessions", __LINE__)

    iNumSessions++;
    iShutdown->Cancel();

	if(iDeInitRequest->IsActive())
        iDeInitRequest->Cancel();
    }

// ---------------------------------------------------------
// CSuplServer::DecrementSessions
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::DecrementSessions()
    {    
    DEBUG_TRACE("DecrementSessions", __LINE__)

    // This panic handles an internal error.
    __ASSERT_DEBUG(iNumSessions > 0,
        DebugPanic(EPosSuplServerPanicSessionsCountInconsistency));
    --iNumSessions;
    if (iNumSessions == 0 && iNumNetMessages == 0)
        {
        //iConnectFlag=FALSE;
        iShutdown->Start(iServerShutdownDelay);
        }
    }
    
// ---------------------------------------------------------
// CSuplServer::HandleNewNetSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::HandleNewNetSessionL(CSuplSessionManager& aSessionManager,const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplServer::HandleNewNetSession", __LINE__)
	
	HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage, 0);
	CSuplNetInitiatedSession* session;
	session=CSuplNetInitiatedSession::NewLC(aSessionManager,CSUPLProtocolManagerBase::ENetwork,this,clientBuf);
	CleanupStack::Pop(session);
	iSuplNetInitRegistry->AddInstanceL(session);
	iNumNetMessages++;
	iShutdown->Cancel();
	CleanupStack::PopAndDestroy(clientBuf);
	session->ForwardMessageL(aMessage);
	}

// ---------------------------------------------------------
// CSuplServer::CompleteForwardMessageL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	     
void CSuplServer:: CompleteForwardMessageL(TInt aHandle)
	{	
	DEBUG_TRACE("CSuplServer::CompleteForwardMessageL", __LINE__)
	CSuplNetInitiatedSession* session = iSuplNetInitRegistry->SubSessionFromHandleL(aHandle);
	if(session)
		session->DestroySession(iSessionManager);
	
	iSuplNetInitRegistry->RemoveInstance(aHandle);
	--iNumNetMessages;
	if (iNumSessions == 0 && iNumNetMessages == 0)
    	{
    	iShutdown->Start(iServerShutdownDelay);
    	}		
	}
	
// ---------------------------------------------------------
// CSuplServer::CompleteConnect
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	     
void CSuplServer::CompleteConnect(TInt aHandle)		
	{
	DEBUG_TRACE("CSuplServer::CompleteConnect", __LINE__)
	
	if(aHandle==KErrNone)
		iConnectFlag=TRUE;
	else
		{
		iConnectFlag=FALSE;		
		}	
	}

// ---------------------------------------------------------
// CSuplServer::GetConnectFlag
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	     
TBool CSuplServer::GetConnectFlag()
	{
	DEBUG_TRACE("CSuplServer::GetConnectFlag", __LINE__)
	return iConnectFlag;
	}

// ---------------------------------------------------------
// CSuplServer::PlugInUnloaded
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::PlugInUninstalled() 
	{	
	DEBUG_TRACE("CSuplServer::PlugInUninstalled Start", __LINE__)
	iSessionIter.SetToFirst();
	CSuplIPCSession* session = NULL;
	CSession2* baseSession = NULL;
	
	while((baseSession = iSessionIter++) != NULL)
		{	 
		DEBUG_TRACE("CSuplServer::PlugInUninstalled Iterating", __LINE__)	   
		session = REINTERPRET_CAST(CSuplIPCSession*, baseSession) ;
		session->ProtocolHUnloaded();
		}	
	
	// request Session Manager to unload protocol handler plug-in
	iSessionManager->ProtocolHUnloaded();
	DEBUG_TRACE("CSuplServer::PlugInUninstalled End", __LINE__)	
	}
	
// ---------------------------------------------------------
// CSuplServer::SetErrorCode
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::SetErrorCode(TInt aErrorCode)
    {
    iErrorCode = aErrorCode;
    }

// ---------------------------------------------------------
// CSuplServer::GetErrorCode
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplServer::GetErrorCode()
    {
    return iErrorCode;
    }    

// ---------------------------------------------------------
// CSuplServer::DeInitialize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplServer::DeInitialize()
    {
	if(!iDeInitRequest->IsActive())
        iDeInitRequest->DeInitialize();
    }      
      
//  End of File
