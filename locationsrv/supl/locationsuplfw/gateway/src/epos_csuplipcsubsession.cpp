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
* Description:   Base class for SUPL subsessions
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <s32mem.h>

#include "epos_suplterminalipc.h"
#include "epos_suplterminalerrors.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_suplterminalqop.h"
#include "epos_suplterminal.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csuplglobal.h"
#include "epos_csuplserverpanic.h"
#include "epos_csuplipcsubsession.h"
#include "epos_suplparameters.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_csuplsettingparams.h"

// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplipcsubsession.cpp");
//#endif

// ==================== LOCAL FUNCTIONS ====================

inline TPositionInfo& PositionInfo(HBufC8* aBuffer)
    {
    DEBUG_TRACE("PositionInfo", __LINE__)
        return reinterpret_cast<TPositionInfo&>
        (const_cast<TUint8&>(*aBuffer->Ptr()));
    }

inline TDes& ServerAddress(HBufC8* aBuffer)
    {
    DEBUG_TRACE("ServerAddress", __LINE__)
    return reinterpret_cast<TDes&>
            (const_cast<TUint8&>(*aBuffer->Ptr()));
    }

inline TSuplTriggerFireInfo& TriggerInfo(HBufC8* aBuffer)
    {
    DEBUG_TRACE("TriggerInfo", __LINE__)
        return reinterpret_cast<TSuplTriggerFireInfo&>
        (const_cast<TUint8&>(*aBuffer->Ptr()));
    }

    
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplIPCSubSession::CSuplIPCSubSession()
    {
    }

// EPOC default constructor can leave.
void CSuplIPCSubSession::ConstructL(CSuplSessionManager& aSessnMgr, 
                                    CSUPLProtocolManagerBase::TSuplReqType aReqType,
                                    TInt aIpcSessionId,
                                    RSuplTerminalSubSession::TSuplServiceType aSuplService)
    {
    DEBUG_TRACE("CSuplIPCSubSession::ConstructL", __LINE__)
    iSuplService = aSuplService;    
    iSuplSession = aSessnMgr.CreateNewSessionL(aReqType, aIpcSessionId, aSuplService);
    iSuplSessnReq = CSuplSessionRequest::NewL(aSessnMgr, iSuplSession, this);
    iReqType=0;
    if(!iSuplSession)
    	User::Leave(KErrGeneral);
    }

// Two-phased constructor.
CSuplIPCSubSession* CSuplIPCSubSession::NewLC(CSuplSessionManager& aSessnMgr, 
                                         CSUPLProtocolManagerBase::TSuplReqType aReqType,
                                         TInt aIpcSessionId,
                                         RSuplTerminalSubSession::TSuplServiceType aSuplService)
	{
    DEBUG_TRACE("CSuplIPCSubSession::NewLC", __LINE__)
    CSuplIPCSubSession* self = new (ELeave) CSuplIPCSubSession();
    CleanupClosePushL(*self);
    TInt err=KErrNone;
	TRAP(err, self->ConstructL(aSessnMgr, aReqType, aIpcSessionId, aSuplService));
	if(err!=KErrNone)
		{
		DEBUG_TRACE("CSuplIPCSubSession::Could Not Create Session", __LINE__)
		return NULL;
		}
    return self;	
	}

// Destructor
CSuplIPCSubSession::~CSuplIPCSubSession()
    {    
    DEBUG_TRACE("CSuplIPCSubSession::~CSuplIPCSubSession", __LINE__)
    delete iSuplSessnReq;
    if( iPositionBuffer )
    delete iPositionBuffer;
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::ServiceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::ServiceL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSubSession::ServiceL", __LINE__)
    
    switch (aMessage.Function())
        {
		case ESuplTerminalSubssnSyncRunsession:
			{	
			iReqType=ESuplTerminalSubssnSyncRunsession;
			iMessage=aMessage;
			HandleRunSessionL(aMessage);
			}
			break;			
		case ESuplTerminalSubssnAsyncRunsession:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnAsyncRunsession;
			HandleRunSessionL(aMessage);
			}
			break;
		case ESuplTerminalSubssnAsyncRunsessionWithQop:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnAsyncRunsessionWithQop;
			HandleRunSessionL(aMessage);
			}
			break;
		case ESuplTerminalSubssnSyncRunsessionWithQop:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnSyncRunsessionWithQop;
			HandleRunSessionL(aMessage);	        	
			}
			break;
		case ESuplTerminalSubssnSyncRunsessionWithServernameQop:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnSyncRunsessionWithServernameQop;
			HandleRunSessionL(aMessage);
			}
			break;
		case ESuplTerminalSubssnASyncRunsessionWithServernameQop:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnASyncRunsessionWithServernameQop;
			HandleRunSessionL(aMessage);
			}
			break;
		case ESuplTerminalSubssnSyncRunsessionServernameWithoutQop:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnSyncRunsessionServernameWithoutQop;
			HandleRunSessionL(aMessage);
			}
			break;
		case ESuplTerminalSubssnASyncRunsessionServernameWithoutQop:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnASyncRunsessionServernameWithoutQop;
			HandleRunSessionL(aMessage);
			}
			break;
			
		case ESuplTerminalSubssnGetServerAddress:
			HandleGetServerAddressL(aMessage);
			break;         
			
		case ESuplTerminalSubssnGetPosition:
			HandleGetPositionL(aMessage);
			break;			
			
		case ESuplTerminalSubssnCancelRunsession:
			HandleRunSessionCancelRequestL(aMessage);
			break;
			
		case ESuplTerminalSubssnStartPeriodicTrigger:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnStartPeriodicTrigger;
			HandlePeriodicTriggerSessionL(aMessage);
			}
			break;        	

        case ESuplTerminalSubssnStartPeriodicTriggerWithServer:
			{
			iMessage=aMessage;
			iReqType=ESuplTerminalSubssnStartPeriodicTriggerWithServer;
        	HandlePeriodicTriggerSessionWithServernameL(aMessage);
			}
        	break;

		case ESuplTerminalSubssnStopTriggerSession:
			{
			iReqType = ESuplTerminalSubssnStopTriggerSession;			
			HandleStopTriggerringSessionL(aMessage);			
			break;
			}
        case ESuplTerminalSubssnNotifyTriggerFired:
        	{
			iMessage=aMessage;
        	iReqType = ESuplTerminalSubssnNotifyTriggerFired;
        	HandleNotifyTriggerFiredL(aMessage);
        	break;
        	}
        case ESuplTerminalSubssnLocationConversion:
                    {
                    iMessage=aMessage;
                    iReqType = ESuplTerminalSubssnLocationConversion;
                    HandleLocationConversionL(aMessage);
                    break;
                    }
                            
                case ESuplTerminalSubssnCancelLocationConversion:
                    {
                    HandleCancelLocationConversionL(aMessage);
                    break;
                    }  
		default:
			User::Leave(KErrNotSupported);
        }
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::NotifyServerShutdown
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::NotifyServerShutdown()
    {
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::DestroySession
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSuplIPCSubSession::DestroySession(CSuplSessionManager& aSessnMgr)
	{
	DEBUG_TRACE("CSuplIPCSubSession::DestryoSession", __LINE__)
	TInt errCode =	aSessnMgr.DestroySession(iSuplSession);
	}

// ---------------------------------------------------------
// CSuplIPCSubSession::HandleRunSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSuplIPCSubSession::HandleRunSessionL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::HandleRunSessionL", __LINE__)

	if (iSuplSessnReq->IsActive())
		{
		SuplGlobal::RequestComplete(aMessage, KErrAlreadyExists);
		}

    TInt setCaps = aMessage.Int0();
    TBool firstReq = EFalse;            
    
    if (setCaps & 0x1000 )
        firstReq = ETrue;                    

    setCaps &= 0x00FF;
    
	if(iReqType==ESuplTerminalSubssnAsyncRunsession||iReqType==ESuplTerminalSubssnSyncRunsession)	
		iSuplSessnReq->MakeSuplSessionRequestL(iSuplSession, aMessage.Int0(), aMessage.Int1(), firstReq);
	else 
	if(iReqType==ESuplTerminalSubssnSyncRunsessionWithQop)
		{
    	HBufC8* qopBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);
    	
		TUint bufferSize = qopBuf->Size();

		TSuplTerminalQop& qop = reinterpret_cast
    	<TSuplTerminalQop&>(const_cast<TUint8&>(*qopBuf->Ptr()));
    		    	
		iSuplSessnReq->MakeSuplSessionRequestL(iSuplSession, aMessage.Int0(), aMessage.Int1(),qop, firstReq);	
		CleanupStack::PopAndDestroy(qopBuf);		
		}
	else
	if(iReqType==ESuplTerminalSubssnAsyncRunsessionWithQop)
		{
		HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);
    		    	
    	TSuplTerminalQop& qop1=reinterpret_cast<TSuplTerminalQop&>
        	(const_cast<TUint8&>(*clientBuf->Ptr()));
        	
        iSuplSessnReq->MakeSuplSessionRequestL(iSuplSession, aMessage.Int0(), aMessage.Int1(),qop1, firstReq);	
    		    	
    	CleanupStack::PopAndDestroy(clientBuf);    		
		}
	else
	if(iReqType==ESuplTerminalSubssnSyncRunsessionWithServernameQop||iReqType==ESuplTerminalSubssnASyncRunsessionWithServernameQop)
		{
		HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);
    		    	
		TSuplParameters& suplStruct=reinterpret_cast<TSuplParameters&>
        (const_cast<TUint8&>(*clientBuf->Ptr()));
                		
        iSuplSessnReq->MakeSuplSessionRequestL(iSuplSession, suplStruct.HslpAddress, suplStruct.FallBack, aMessage.Int0(), aMessage.Int1(),suplStruct.qop, firstReq);	
    		    	
    	CleanupStack::PopAndDestroy(clientBuf);    		
		}
	else
	if(iReqType==ESuplTerminalSubssnSyncRunsessionServernameWithoutQop||iReqType==ESuplTerminalSubssnASyncRunsessionServernameWithoutQop)
		{
		HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);
    		    	
		TSuplParameters& suplStruct1=reinterpret_cast<TSuplParameters&>
        (const_cast<TUint8&>(*clientBuf->Ptr()));        
        	        
        iSuplSessnReq->MakeSuplSessionRequestL(iSuplSession, suplStruct1.HslpAddress, suplStruct1.FallBack, aMessage.Int0(), aMessage.Int1(), firstReq);	
    		    	
    	CleanupStack::PopAndDestroy(clientBuf);    		
		}		
	}

// ---------------------------------------------------------
// CSuplIPCSubSession::HandleGetPositionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::HandleGetPositionL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::HandleGetPositionL", __LINE__)
	TInt err;
	if (iSuplSession)
		{
		delete iPositionBuffer;
		iPositionBuffer = NULL;	
		HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage, 0);
		CleanupStack::Pop(clientBuf);
		iPositionBuffer = clientBuf;
		TPositionInfo& info = PositionInfo(iPositionBuffer);
		TUint32 classType = info.PositionClassType();
		TUint32 generic =  EPositionGenericInfoClass;

		if (classType & generic)
			{
	  		HPositionGenericInfo& hGeneric = static_cast <HPositionGenericInfo&> (info);
	  		err=iSuplSession->GetPosition(hGeneric);
	   		}
		else
			{
	   		err=iSuplSession->GetPosition(info);
			}
		}
	else
		err = KErrNotFound;

	CompleteGetPositionRequest(aMessage, err);
 	}

// ---------------------------------------------------------
// CSuplIPCSubSession::HandleGetServerAddressL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::HandleGetServerAddressL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSubSession::HandleGetServerAddressL", __LINE__)
    TInt err;
    if (iSuplSession)
        {
        delete iPositionBuffer;
        iPositionBuffer = NULL; 
        HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage, 0);
        CleanupStack::Pop(clientBuf);
        iPositionBuffer = clientBuf;
        TDes& info = ServerAddress(iPositionBuffer);
       
       // Please check this call... Need Tbuf16 as a parameter
        err = iSuplSession->GetServerAddressL(info);
                  
        CompleteGetServerAddress(aMessage, err);        
        }
    else
        err = KErrNotFound;        
 	}
 
//----------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------
// CSuplIPCSubSession::HandleRunSessionCancelRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::HandleRunSessionCancelRequestL(const RMessage2& aMessage) 
    {
    DEBUG_TRACE("CSuplIPCSubSession::HandleCancelRequestL", __LINE__)

    if (!iSuplSessnReq->IsActive()
            ||!(iReqType==ESuplTerminalSubssnAsyncRunsession 
                    || iReqType==ESuplTerminalSubssnAsyncRunsessionWithQop 
                    || iReqType==ESuplTerminalSubssnASyncRunsessionWithServernameQop 
                    || iReqType==ESuplTerminalSubssnASyncRunsessionServernameWithoutQop))
        {
        User::Leave(KErrNotFound);
        }
    iSuplSessnReq->CancelRunSessionRequest();
    SuplGlobal::RequestComplete(aMessage, KErrNone);
    CompleteRunSession(KErrCancel);
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::CompleteRunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::CompleteRunSession(TInt aReason)
    {
    DEBUG_TRACE("CSuplIPCSubSession::CompleteRunSession", __LINE__)
    if (!iMessage.IsNull())
        {
        iMessage.Complete(aReason);
        }
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::CompleteTriggerRunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::CompleteTriggerRunSession(TInt aReason)
    {
    DEBUG_TRACE("CSuplIPCSubSession::CompleteTriggerRunSession", __LINE__)
    if (!iMessage.IsNull())
        {
        iMessage.Complete(aReason);
        }
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::CompleteGetPositionRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::CompleteGetPositionRequest(
	const RMessage2& aMessage, 
	TInt aReason)
    {
	DEBUG_TRACE("CSuplIPCSubSession::CompleteGetPositionRequest", __LINE__)
	if (aReason == KErrNone)
		{
		TInt err = PackPositionData(aMessage);
		// err - client cannot receive result data
		//CompleteRunSession((err != KErrNone) ? err : aReason);
		if (err != KErrNone)
			aMessage.Complete(err);
		else
			aMessage.Complete(aReason);
		}
	else
		{
		//CompleteRunSession(aReason);
		aMessage.Complete(aReason);
		}
	delete iPositionBuffer;
	iPositionBuffer = NULL;	
	iReqType=0;   
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::CompleteTriggerFiredNotifyRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::CompleteTriggerFiredNotifyRequest(TInt aReason)
    {
	DEBUG_TRACE("CSuplIPCSubSession::CompleteGetPositionRequest", __LINE__)
    if (aReason == KErrNone)                    
        PackTriggerData(iMessage);            

    if (!iMessage.IsNull())
        {
        iMessage.Complete(aReason);
        }
    delete iPositionBuffer;
    iPositionBuffer = NULL; 
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::CompleteGetServerAddress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplIPCSubSession::CompleteGetServerAddress(const RMessage2& aMessage, TInt aReason)
    {
	DEBUG_TRACE("CSuplIPCSubSession::CompleteGetServerAddress", __LINE__)
	if (aReason == KErrNone)
	    {
	    TInt err = PackServerAddressData(aMessage);
	    // err - client cannot receive result data
	    //CompleteRunSession((err != KErrNone) ? err : aReason);
	    if (err != KErrNone)
	        aMessage.Complete(err);
	    else
	        aMessage.Complete(aReason);
	    }
	else
	    {
	    //CompleteRunSession(aReason);
	    aMessage.Complete(aReason);
	    }
    delete iPositionBuffer;
    iPositionBuffer = NULL; 
    iReqType=0;   
    }
    
//------------------------------------------------------------------------------------------------
// ---------------------------------------------------------
// CSuplIPCSubSession::PackPositionData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
TInt CSuplIPCSubSession::PackPositionData(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::PackPositionData", __LINE__)
    PositionInfo(iPositionBuffer);
    TPtr8 ptrToBuffer = iPositionBuffer->Des();
    return SuplGlobal::Write(aMessage, 0, ptrToBuffer);
    }
 
// ---------------------------------------------------------
// CSuplIPCSubSession::PackServerAddressData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplIPCSubSession::PackServerAddressData(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSubSession::PackServerAddressData", __LINE__)

    ServerAddress(iPositionBuffer);
    TPtr8 ptrToBuffer = iPositionBuffer->Des();
    return SuplGlobal::Write(aMessage, 0, ptrToBuffer);
    }
    
//---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------
// CSuplIPCSubSession::PackTriggerData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
TInt CSuplIPCSubSession::PackTriggerData(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::PackTriggerData", __LINE__)

    TriggerInfo(iPositionBuffer);
    TPtr8 ptrToBuffer = iPositionBuffer->Des();
    return SuplGlobal::Write(aMessage, 2, ptrToBuffer);
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::ProtocolHUnloaded
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSuplIPCSubSession::ProtocolHUnloaded() //change here --- Subhra
	{
	DEBUG_TRACE("CSuplIPCSubSession::ProtocolHUnloaded", __LINE__)
	iSuplSessnReq->Cancel();
	iSuplSessnReq->ProtocolHUnloaded();
	CompleteRunSession(KErrCompletion);
	iSuplSession = NULL;
	}

// ---------------------------------------------------------
// CSuplIPCSubSession::HandlePeriodicTriggerSessionWithServernameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSuplIPCSubSession::HandlePeriodicTriggerSessionWithServernameL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::HandlePeriodicTriggerSessionWithServernameL", __LINE__)
	
    if (!aMessage.HasCapability(ECapabilityCommDD))
        {
        DEBUG_TRACE("HandlePeriodicTriggerSessionL, Client doesnt have CommDD capability", __LINE__)	
		SuplGlobal::RequestComplete(aMessage, KErrPermissionDenied);
        }
            
	if (iSuplSessnReq->IsActive())
		{
	    DEBUG_TRACE("HandlePeriodicTriggerSessionWithServernameL, session active", __LINE__)
		SuplGlobal::RequestComplete(aMessage, KErrAlreadyExists);
		}

    TInt setCaps = aMessage.Int0();
    TInt requestId = aMessage.Int1();
    
	DEBUG_TRACE("HandlePeriodicTriggerSessionWithServernameL, copying client buffer", __LINE__)
	HBufC8* clientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);
	DEBUG_TRACE("HandlePeriodicTriggerSessionWithServernameL, copy client buffer complete", __LINE__)
		    	
	TSuplParameters& suplStruct=reinterpret_cast<TSuplParameters&>
    (const_cast<TUint8&>(*clientBuf->Ptr()));
    
    iSuplSessnReq->MakeSuplSessionTriggerringRequestL(
    					iSuplSession, 
    					suplStruct.PeriodicTrigger, 
    					suplStruct.HslpAddress, 
    					suplStruct.FallBack,
    					setCaps, 
    					requestId);
		    	
	CleanupStack::PopAndDestroy(clientBuf);	   
	}
	
// ---------------------------------------------------------
// CSuplIPCSubSession::HandlePeriodicTriggerSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSuplIPCSubSession::HandlePeriodicTriggerSessionL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::HandlePeriodicTriggerSessionL", __LINE__)
	
    if (!aMessage.HasCapability(ECapabilityCommDD))
        {
        DEBUG_TRACE("HandlePeriodicTriggerSessionL, Client doesnt have CommDD capability", __LINE__)	
		SuplGlobal::RequestComplete(aMessage, KErrPermissionDenied);
        }
            
	if (iSuplSessnReq->IsActive())
		{
	    DEBUG_TRACE("HandlePeriodicTriggerSessionL, session active", __LINE__)
		SuplGlobal::RequestComplete(aMessage, KErrAlreadyExists);
		}

    TInt setCaps = aMessage.Int0();
    TInt requestId = aMessage.Int1();
    
	DEBUG_TRACE("HandlePeriodicTriggerSessionL, copying client buffer", __LINE__)
	HBufC8* pTriggerBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);        	
	DEBUG_TRACE("HandlePeriodicTriggerSessionL, copy client buffer complete", __LINE__)
	TUint bufferSize = pTriggerBuf->Size();
	
	TSuplTerminalPeriodicTrigger& pTrigger = reinterpret_cast
		<TSuplTerminalPeriodicTrigger&>(const_cast<TUint8&>(*pTriggerBuf->Ptr()));    
    	
	iSuplSessnReq->MakeSuplSessionTriggerringRequestL(
						iSuplSession, 
						pTrigger,
						setCaps, 
						requestId
						);	
						
	CleanupStack::PopAndDestroy(pTriggerBuf);	    
	}
	
// ---------------------------------------------------------
// CSuplIPCSubSession::HandleStopTriggerringSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSuplIPCSubSession::HandleStopTriggerringSessionL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::HandleStopTriggerringSessionL", __LINE__)
	
    if (!iSuplSessnReq->IsActive()||(iReqType!=ESuplTerminalSubssnStopTriggerSession))
        {
        User::Leave(KErrNotFound);
        }
    iSuplSessnReq->CancelTriggerringRequest();
    SuplGlobal::RequestComplete(aMessage, KErrNone);
    CompleteTriggerRunSession(KErrCancel);
	}
	
// ---------------------------------------------------------
// CSuplIPCSubSession::HandleNotifyTriggerFiredL
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 	
void CSuplIPCSubSession::HandleNotifyTriggerFiredL(const RMessage2& aMessage)
	{
	DEBUG_TRACE("CSuplIPCSubSession::HandlerNotifyTriggerFired", __LINE__)

	if (iSuplSessnReq->IsActive())
		{
		SuplGlobal::RequestComplete(aMessage, KErrAlreadyExists);
		}

	HBufC8* fireInfoBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,2);        	
	TUint bufferSize = fireInfoBuf->Size();
    
	delete iPositionBuffer;
	iPositionBuffer = NULL;	

    iPositionBuffer = fireInfoBuf;    

	TSuplTriggerFireInfo& fireInfo = reinterpret_cast
		<TSuplTriggerFireInfo&>(const_cast<TUint8&>(*fireInfoBuf->Ptr()));    
    	
	iSuplSessnReq->NotifyTriggerFiredRequestL(iSuplSession,fireInfo);				
						
	CleanupStack::Pop(fireInfoBuf);	    		
	}
// ---------------------------------------------------------
// CSuplIPCSubSession::HandleLocationConversionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//  
void CSuplIPCSubSession::HandleLocationConversionL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSubSession::HandleLocationConversionL", __LINE__)

    if (iSuplSessnReq->IsActive())
        {
        SuplGlobal::RequestComplete(aMessage, KErrAlreadyExists);
        }

    if(iReqType==ESuplTerminalSubssnLocationConversion)
        {
        HBufC8* cellBuf = SuplGlobal::CopyClientBuffer8LC(aMessage,0);
                
        TGeoCellInfo& cellInfo = reinterpret_cast
        <TGeoCellInfo&>(const_cast<TUint8&>(*cellBuf->Ptr()));
                            
        iSuplSessnReq->MakeLocationConversionRequestL( iSuplSession,cellInfo );
        
        CleanupStack::PopAndDestroy(cellBuf);        
        }
         
    }

// ---------------------------------------------------------
// CSuplIPCSubSession::HandleCancelLocationConversionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//  
void CSuplIPCSubSession::HandleCancelLocationConversionL(const RMessage2& aMessage)
    {
    DEBUG_TRACE("CSuplIPCSubSession::HandleCancelLocationConversionL", __LINE__)

    if (!iSuplSessnReq->IsActive()||(iReqType!=ESuplTerminalSubssnLocationConversion))
        {
        User::Leave(KErrNotFound);
        }
    iSuplSessnReq->CancelLocationConversionRequest();
    SuplGlobal::RequestComplete(aMessage, KErrNone);
    CompleteRunSession(KErrCancel);            
    }	
// End of File
