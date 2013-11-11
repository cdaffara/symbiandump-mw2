/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SUPL protocol manager implementation.i.e. OMA SUPL implementation 
*
*/


#include <centralrepository.h>
#include <ecom/ecom.h> 
#include <flogger.h>
#include <CoreApplicationUIsSDKCRKeys.h>

#include "epos_csuplsessionbase.h"
#include "epos_comasuplasnbase.h"
#include "epos_comasuplasnmessagebase.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "lbs/epos_comasuplposhandlerbase.h"
#include "epos_suplconstants.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_suplterminalconstants.h"
	
#include "epos_comasuplprotocolmanager.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_comasuplsettings.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplcommonconfig.h"
#include "epos_comasuplinitilizerequestor.h"

_LIT(KTraceFileName,"SUPL_OMA_PH::EPos_COMASUPLProtocolManager.cpp");

const TInt KMaxIMSILen = 20;

// -----------------------------------------------------------------------------
// ResetAndDestroy()
// Resets & destroys array if leave occurs
// -----------------------------------------------------------------------------
void ResetAndDestroy(TAny* aArray)
	{
	((RImplInfoPtrArray*)aArray)->ResetAndDestroy();
	}

// ============================ MEMBER FUNCTIONS ===============================    
// Create instance of concrete ECOM interface implementation
EXPORT_C COMASUPLProtocolManager* COMASUPLProtocolManager::NewL()
	{
	COMASUPLProtocolManager* self = new(ELeave) COMASUPLProtocolManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	} 
 
// Destructor.
EXPORT_C COMASUPLProtocolManager::~COMASUPLProtocolManager()
	{
    if( !isPH2Shared )
        {
        delete iOMASuplProtocolHandler1;
        }
    
    delete iOMASuplProtocolHandler2;
    iOMASuplProtocolHandler2 = NULL;

	delete iCommonConfig;
	iCommonConfig = NULL;
	
	delete iTrace;	
	iTrace = NULL;
	
	delete iOMASuplAsnHandlerBaseImpl;
	iOMASuplAsnHandlerBaseImpl = NULL;
	
	delete iOMASuplInitilizeRequestor;
	iOMASuplInitilizeRequestor=NULL;
	
	delete iOMASuplDeInitilizer;
	iOMASuplDeInitilizer = NULL;
	
	}

// Constructor
EXPORT_C COMASUPLProtocolManager::COMASUPLProtocolManager() : 
										iTrace(NULL),  
                                        iInitilizationRequestStatus(NULL),
                                        iInitilizationRequestStatus2(NULL),
                                        iDeInitRequestStatus(NULL),
                                        iOMASuplInitilizeRequestor(NULL),
                                        iOMASuplProtocolHandler1(NULL),
                                        iOMASuplProtocolHandler2(NULL),
                                        iCommonConfig(NULL),
                                        iCommManager(NULL),
                                        iIsInitilized(EFalse),
                                        isPH2Shared(EFalse),
                                        iIsPhDeinitialized1(EFalse),
                                        iIsPhDeinitialized2(EFalse)
                                        
    {
	// no implementation required
	}

// Second phase construction.
void COMASUPLProtocolManager::ConstructL()
	{
    iTrace = COMASuplTrace::NewL();	
	LoadOMASUPLPluginsL();
	iTrace->Trace(_L("COMASUPLProtocolManager::ConstructL"), KTraceFileName, __LINE__);

	iCommonConfig = COMASuplCommonConfig::NewL();
	iOMASuplInitilizeRequestor = COMASuplInitilizeRequestor::NewL(
										iCommonConfig->iPosHandler,
										*this,
										iCommonConfig->iSuplSettings);
	
	iOMASuplDeInitilizer = COMASuplDeInitilizer::NewL(*this);
	
	TRAPD(err,iOMASuplAsnHandlerBaseImpl = COMASuplAsnHandlerBase::NewL(1));
    
    if( KErrNone != err )
        {
        TRAP(err,iOMASuplAsnHandlerBaseImpl = COMASuplAsnHandlerBase::NewL(2));
        }
	}
    
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CreateNewSessionL, 
// Creates new session 
// it transfers ownership to callee
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C CSuplSessionBase* COMASUPLProtocolManager::CreateNewSessionL(
										CSUPLProtocolManagerBase::TSuplReqType aRequestType, 
										TInt aIpcSessionId, 
										RSuplTerminalSubSession::TSuplServiceType aSuplService)
	{
    if(!iIsInitilized) // if its not initilized, then leave with KErrSuplUnInitilized
	    User::LeaveIfError(KErrOMASuplUnInitilized);
    
    if( aSuplService == RSuplTerminalSubSession::ESUPL_1_0 )
	    {
        if(iOMASuplProtocolHandler1)
            {
            return iOMASuplProtocolHandler1->CreateNewSessionL(
            							aRequestType, 
            							aIpcSessionId, 
            							aSuplService);
            }
        return NULL;
	    }
		
	else if( aSuplService == RSuplTerminalSubSession::ESUPL_2_0 )
	    {
	    if(iOMASuplProtocolHandler2)
	        {
	        return iOMASuplProtocolHandler2->CreateNewSessionL(
	        							aRequestType, 
	        							aIpcSessionId, 
	        							aSuplService);
	        }
        return NULL;
	    }
	else
	    {
        return NULL;
	    }
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::InitializeL 
// Does the connect to ETel. 
// Intilize POS if present 
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C  void COMASUPLProtocolManager::InitializeL(
										CSuplCommunicationManager* aCommManager,
										TRequestStatus& aInitilizationRequestStatus)
	{
	iInitilizationRequestStatus = &aInitilizationRequestStatus;
    *iInitilizationRequestStatus = KRequestPending;
	if(iIsInitilized) 
		{
		CompleteInitilization(KErrNone);
		return;	
		}

    iCommManager = aCommManager;			
	iIsInitilized = EFalse;		    
	   
	if(!iCommManager) 
		{
		CompleteInitilization(KErrArgument);
		return;	
		}
	
	iOMASuplInitilizeRequestor->DoInitilizationL();
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CancelInitialize 
// Cancel Initilization of Protocol manager
// 
// -----------------------------------------------------------------------------
//
EXPORT_C  void COMASUPLProtocolManager::CancelInitialize() 
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::CancelInitialize"), 
										KTraceFileName, __LINE__);
	
	CompleteInitilization(KErrCancel);
	iOMASuplInitilizeRequestor->Cancel();		
	}
   
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::RunSessionL
// Starts the SUPL Sesssion for Terminal Initiated Location Request
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::RunSessionL(
											CSuplSessionBase *const  aSuplSession,
											TRequestStatus& aStatus,
											const TDesC& aHslpAddress, 
											TBool aFallBack,
											TBool aFirstReq,
											TInt aAllowedCapabilities,
											TInt aRequestID)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::RunSessionL"), KTraceFileName, __LINE__);
	
	aStatus = KRequestPending;
	TRequestStatus *status = &aStatus; 
	
	if( iIsInitilized )
		{
			if( aSuplSession->GetSUPLVersion() == 1 )
			{
				iOMASuplProtocolHandler1->RunSessionL(      
																						aSuplSession,
                                            aStatus,
                                            aHslpAddress,
                                            aFallBack,
                                            aFirstReq,
                                            aAllowedCapabilities,
                                            aRequestID);
			}    
		else if( aSuplSession->GetSUPLVersion() == 2 )
			{
				iOMASuplProtocolHandler2->RunSessionL(      
																						aSuplSession,
                                            aStatus,
                                            aHslpAddress,
                                            aFallBack,
                                            aFirstReq,
                                            aAllowedCapabilities,
                                            aRequestID);
			}
		}
	else
	    {
	    User::RequestComplete(status,KErrOMASuplUnInitilized); 
	    }
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::RunSessionL
// Starts the SUPL Sesssion for Terminal Initiated Location Request,with QoP support
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::RunSessionL(
                            CSuplSessionBase *const aSuplSession,
                            TRequestStatus& aStatus,
                            const TDesC& aHslpAddress, 
                            TBool aFallBack,
                            TBool aFirstReq,
  							TInt aAllowedCapabilities,
  							TSuplTerminalQop& aQop, 
  							TInt aRequestID)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::RunSessionL"), KTraceFileName, __LINE__);
	
	aStatus = KRequestPending;
	
	TRequestStatus *status = &aStatus; 
    if( iIsInitilized )
    	{
    		if( aSuplSession->GetSUPLVersion() == 1 )
        {
        iOMASuplProtocolHandler1->RunSessionL(
                                            aSuplSession,
                                            aStatus,
                                            aHslpAddress,
                                            aFallBack,
                                            aFirstReq,
                                            aAllowedCapabilities,
                                            aQop,
                                            aRequestID);
        }  
        else if( aSuplSession->GetSUPLVersion() == 2 )  
        {
        	iOMASuplProtocolHandler2->RunSessionL(
                                            aSuplSession,
                                            aStatus,
                                            aHslpAddress,
                                            aFallBack,
                                            aFirstReq,
                                            aAllowedCapabilities,
                                            aQop,
                                            aRequestID);	
        }
      }
    else
        {
        User::RequestComplete(status,KErrOMASuplUnInitilized); 
        }
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::DestroySession
// Deletes specified session & removes from array.
// Returns System wide error,if any occurs
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASUPLProtocolManager::DestroySession(CSuplSessionBase* aSuplSession)	
	{
	iTrace->Trace(_L("Start of COMASUPLProtocolManager::DestroySession"), KTraceFileName, __LINE__);
	// Forward to PH1 first
	TInt err = KErrNone;
	if(iOMASuplProtocolHandler1)
	    {
	    err = iOMASuplProtocolHandler1->DestroySession(aSuplSession);
	    }
	// If session is not found in PH1 then forward it to PH2 else just return the error 
	if( KErrArgument == err && iOMASuplProtocolHandler2)
        {
        return iOMASuplProtocolHandler2->DestroySession(aSuplSession);
        }
	return err;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::HandlePacket
// Handles Supl packets ,  received from SLP
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::HandlePacket(const TDesC8& aPacket, TUint aPortNum)
	{
	iTrace->Trace(_L("Start of COMASUPLProtocolManager::HandlePacket"), 
											KTraceFileName, __LINE__);
	
	TInt major;
	TInt errorCode = GetSuplVersion(major,aPacket);
	if(errorCode != KErrNone )		
		{			
		HandlePacketError(errorCode);
		return;
		}
	
	//Work out what SUPL Protocol Hanlder version should be used
	if( major == 1 )
		{
		//SUPL v1
		if(iOMASuplProtocolHandler1)
		    {
		    iOMASuplProtocolHandler1->HandlePacket(aPacket,aPortNum);
		    }
		}
	else if ( major == 2 )
		{
		//SUPL v2
		if(iOMASuplProtocolHandler2)
            {
            iOMASuplProtocolHandler2->HandlePacket(aPacket,aPortNum);
            }
		}
	else
		{
		//Use either of the Protocol Handlers to handle the packet 
		if (iOMASuplProtocolHandler1)
    		{
    		iOMASuplProtocolHandler1->HandlePacket(aPacket,aPortNum);
    		}
		else if(iOMASuplProtocolHandler2)
            {
            iOMASuplProtocolHandler2->HandlePacket(aPacket,aPortNum);
            } 
		}			
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::HandlePacketError
// Handles Supl packet receiving errors
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::HandlePacketError(TInt /*aError*/)
	{
	// Do nothing for now
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CancelRunSession
// Cancels run session operation if its going on.
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::CancelRunSession(CSuplSessionBase* aSuplSession)
	{
	// Log
	iTrace->Trace(_L("COMASUPLProtocolManager::CancelRunSession"), KTraceFileName, __LINE__);
	if(iOMASuplProtocolHandler1)
	    {
	    iOMASuplProtocolHandler1->CancelRunSession(aSuplSession);
	    }
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CompleteInitilization
// Completes initilization AO with error or without error.
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager::CompleteInitilization(TInt aCompletionCode)
	{	
	if(iInitilizationRequestStatus)
		User::RequestComplete(iInitilizationRequestStatus,aCompletionCode);
	
	iTrace->Trace(_L("COMASUPLProtocolManager::InitializeL"), KTraceFileName, __LINE__);
	if(iOMASuplProtocolHandler1)
	    {
	    TRAP_IGNORE(iOMASuplProtocolHandler1->InitializeL(iCommManager,*iInitilizationRequestStatus2));
	    }
	if(iOMASuplProtocolHandler2)
	    {
	    TRAP_IGNORE(iOMASuplProtocolHandler2->InitializeL(iCommManager,*iInitilizationRequestStatus));
	    }
	iTrace->Trace(_L("InitializeL completed"), KTraceFileName, __LINE__);	
	}
    
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::InitilizationCompletedL
// Informs to client about Initilization of POS ,thro' active object
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager::InitilizationCompletedL(TInt aError)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::InitilizationCompletedL Start"), 
													KTraceFileName, __LINE__);
	TInt networkMode  = GetNetworkModeL();
	
	if ( networkMode == ECoreAppUIsNetworkConnectionNotAllowed 
			&& aError == KErrGeneral)
		{
		aError = KErrNone;
		}

	if(aError == KErrNone)
		{   
		iCommonConfig->iIMSI.Close();
	    iCommonConfig->iIMSI.Create( KMaxIMSILen );
	    aError = iOMASuplInitilizeRequestor->GetIMSI( iCommonConfig->iIMSI );

	    if( aError == KErrNotSupported || aError == KErrNone 
	    					|| (networkMode == ECoreAppUIsNetworkConnectionNotAllowed 
	    						&& aError == KErrNotReady))
	        {
	        iIsInitilized = ETrue;	
	        aError = KErrNone;
	        }
		}
	
	TBuf<64> buf(_L("COMASUPLProtocolManager::InitilizationCompletedL Error : "));
	buf.AppendNum(aError);
	iTrace->Trace(buf,KTraceFileName, __LINE__);
	
	SetCommonConfig(iCommonConfig);
	CompleteInitilization(aError);
	iTrace->Trace(_L("COMASUPLProtocolManager::InitilizationCompletedL End"), 
						KTraceFileName, __LINE__);
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::HandleSuplMessageL
// Informs to client about Initilization of POS ,thro' active object
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void COMASUPLProtocolManager::HandleSuplMessageL(
											CSuplSessionBase *const aSuplSession,
											TRequestStatus& aStatus,HBufC8* aMessage)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::HandleSuplMessageL"), KTraceFileName, __LINE__);
	
	TRequestStatus* status =  &aStatus;

	if(!aMessage)
		{
		iTrace->Trace(_L("Received packet is Empty."), KTraceFileName, __LINE__);
		if(status)
			User::RequestComplete(status,KErrBadDescriptor);
		return;
		}

	//Work out what SUPL Protocol Hanlder version should be used
	TInt major = aSuplSession->GetSUPLVersion();	
	if( major == 1 )
		{
		//SUPL v1
		if(iOMASuplProtocolHandler1)
		    {
		    iOMASuplProtocolHandler1->HandleSuplMessageL(aSuplSession,aStatus,aMessage);
		    }
		else if (iOMASuplProtocolHandler2)
				{
				iOMASuplProtocolHandler2->HandleSuplMessageL(aSuplSession,aStatus,aMessage);
				}
		}
	else if( major == 2 )
		{
		//SUPL v2
		if(iOMASuplProtocolHandler2)
		    {
		    iOMASuplProtocolHandler2->HandleSuplMessageL(aSuplSession,aStatus,aMessage);
		    }
		else if (iOMASuplProtocolHandler1)
				{
				iOMASuplProtocolHandler1->HandleSuplMessageL(aSuplSession,aStatus,aMessage);
				}
		}
	else
		{
		//Use either of the Protocol Handlers (if they exist) to handle the packet 
		if (iOMASuplProtocolHandler1)
    		{
    		iOMASuplProtocolHandler1->HandleSuplMessageL(aSuplSession,aStatus,aMessage);
    		}
		else if(iOMASuplProtocolHandler2)
		    {
		    iOMASuplProtocolHandler2->HandleSuplMessageL(aSuplSession,aStatus,aMessage);
		    }    	
		}			
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::DeInitialize
// 
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager::DeInitialize(TRequestStatus& aDeInitRequestStatus)
    {
	iTrace->Trace(_L("COMASUPLProtocolManager::DeInitialize"), KTraceFileName, __LINE__);
	iDeInitRequestStatus = &aDeInitRequestStatus;
	
	if(iOMASuplProtocolHandler1 && !iIsPhDeinitialized1)
	    {
	    iTrace->Trace(_L("COMASUPLProtocolManager::DeInitialize ph1 issued"), KTraceFileName, __LINE__);
	    iIsPhDeinitialized1 = ETrue;
	    iOMASuplDeInitilizer->Deinitialize(iOMASuplProtocolHandler1);
	    }
	else
		{
		iTrace->Trace(_L("COMASUPLProtocolManager::DeInitialize ph1 not present"), KTraceFileName, __LINE__);
		TRAP_IGNORE(DeInitilizationCompletedL( KErrNotFound )); //ph1 is not present.
		}
	
    }            
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CancelDeInitialize
// 
// 
// -----------------------------------------------------------------------------
//	
void COMASUPLProtocolManager::CancelDeInitialize()
    {
	iTrace->Trace(_L("COMASUPLProtocolManager::CancelDeInitialize"), KTraceFileName, __LINE__);
	if(iOMASuplProtocolHandler1 && iIsPhDeinitialized1 )
        {
        iTrace->Trace(_L("COMASUPLProtocolManager::CancelDeInitialize ph1 issued"), KTraceFileName, __LINE__);
        iOMASuplProtocolHandler1->CancelDeInitialize();
        }
	    
	if(iOMASuplProtocolHandler2 && iIsPhDeinitialized2 )
        {
        iTrace->Trace(_L("COMASUPLProtocolManager::CancelDeInitialize ph2 issued"), KTraceFileName, __LINE__);
        iOMASuplProtocolHandler2->CancelDeInitialize();
        }	
    }
   
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::GetNetworkModeL
// Logs
// -----------------------------------------------------------------------------
TInt COMASUPLProtocolManager::GetNetworkModeL()
	{	
	TInt networkMode;
	// get network mode
	CRepository* centralRepository = CRepository::NewLC(KCRUidCoreApplicationUIs);
	User::LeaveIfError(centralRepository->Get(KCoreAppUIsNetworkConnectionAllowed, networkMode));
    CleanupStack::PopAndDestroy(centralRepository);
    return networkMode;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::LoadOMASUPLPluginsL
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager::LoadOMASUPLPluginsL()
    {
	iTrace->Trace(_L("COMASUPLProtocolManager::LoadOMASUPLPlugins"), KTraceFileName, __LINE__); 
	
    const TUid KOMASUPLProtocolInterfaceUid 		= {0x102073CB};
    
    const TUid KOMASuplProtocolImplementationUid1 	= {0x2002678D};
    
    const TUid KOMASuplProtocolImplementationUid2 	= {0x2002678E};
    
    RImplInfoPtrArray pluginArray;
    
    REComSession& EComServerSession = REComSession::OpenL();
    
    CleanupStack::PushL(TCleanupItem(*ResetAndDestroy, &pluginArray));
    REComSession::ListImplementationsL(KOMASUPLProtocolInterfaceUid, pluginArray);
    
    if(pluginArray.Count() > 0)    
        {         
        TRAPD(err1,iOMASuplProtocolHandler1 = 
        				CSUPLProtocolManagerBase::NewL(KOMASuplProtocolImplementationUid1));
        				
        TBuf<64> buf(_L("OMA SUPL 1.0 Plugin loaded with error : "));
        buf.AppendNum(err1);
        iTrace->Trace(buf,KTraceFileName, __LINE__);        	

        TRAPD(err2,iOMASuplProtocolHandler2 = 
        				CSUPLProtocolManagerBase::NewL(KOMASuplProtocolImplementationUid2));
        				
        buf.Copy(_L("OMA SUPL 2.0 Plugin loaded with error : "));
        buf.AppendNum(err2);
        iTrace->Trace(buf,KTraceFileName, __LINE__);

        // if PH1 is not available but PH2 is,then use PH2 whenever there is reference to PH1.
        if( KErrNone != err1 && KErrNone == err2 )
            {
            isPH2Shared = ETrue;
            iOMASuplProtocolHandler1 = iOMASuplProtocolHandler2;
            }
        }
    else
        {
        iTrace->Trace(_L("No Plugins Found for OMA Implementation..."), KTraceFileName, __LINE__);
        User::LeaveIfError(KErrArgument);
        }   
        
    CleanupStack::PopAndDestroy(1);
    EComServerSession.Close();
    }

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::RunSuplTriggerSessionL
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::RunSuplTriggerSessionL(
  					CSuplSessionBase *const aSuplSession,
  					TRequestStatus& aStatus,
  					TSuplTerminalPeriodicTrigger& aPTrigger,
  					const TDesC& aHslpAddress, 
  					TBool aFallBack,
  					TInt aAllowedCapabilities,
  					TInt aRequestID
  					)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::RunSuplTriggerSessionL"), KTraceFileName, __LINE__);
	aStatus = KRequestPending;
	TRequestStatus *status = &aStatus; 
	if( iIsInitilized && iOMASuplProtocolHandler2 )
        {
        iOMASuplProtocolHandler2->RunSuplTriggerSessionL(
                aSuplSession,
                aStatus,
                aPTrigger,
                aHslpAddress,
                aFallBack,
                aAllowedCapabilities,
                aRequestID);           
        }    
	else
	    {
	    User::RequestComplete(status,KErrOMASuplUnInitilized); 
	    }	
	} 

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CancelTriggerringSession
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::CancelSuplTriggerSession(CSuplSessionBase* aSuplSession)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::CancelSuplTriggerSession"), KTraceFileName, __LINE__);
	if(iOMASuplProtocolHandler2)
	    {
	    iOMASuplProtocolHandler2->CancelSuplTriggerSession(aSuplSession);
	    }
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::NotifyTriggerFired
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::NotifyTriggerFiredL(
    				 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus, 
        			 TSuplTriggerFireInfo& aFireInfo)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::NotifyTriggerFired"), KTraceFileName, __LINE__);
	if(iOMASuplProtocolHandler2)
        {
        iOMASuplProtocolHandler2->NotifyTriggerFiredL(aSuplSession,aStatus,aFireInfo);
        }
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::SetCommonConfig
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager::SetCommonConfig(COMASuplCommonConfig*& aCommmonConfig)
    {
    if(iOMASuplProtocolHandler1)
        {
        COMASUPLProtocolManager* ph1= (COMASUPLProtocolManager*) (iOMASuplProtocolHandler1);
        ph1->SetCommonConfig(aCommmonConfig);
        }
    
    if(iOMASuplProtocolHandler2)
        {
        COMASUPLProtocolManager* ph2= (COMASUPLProtocolManager*) (iOMASuplProtocolHandler2);
        ph2->SetCommonConfig(aCommmonConfig);
        }
    }
    
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::GetSuplVersion
// 
// 
// -----------------------------------------------------------------------------
//
TInt COMASUPLProtocolManager::GetSuplVersion(TInt& aMajor,const TPtrC8& aSuplAsnMessage )
    {     
    if( !iOMASuplAsnHandlerBaseImpl )
        {
        return KErrNotFound;
        }
    TOMASuplVersion suplVersion;
    TInt err1;
    TRAP_IGNORE(suplVersion = iOMASuplAsnHandlerBaseImpl->GetSuplMessageVersionL(
    										err1,
    										aSuplAsnMessage));//Ignore err2                         
              
    if( err1 != KErrNone )      
        {           
        return err1;
        } 

    TInt minor,servInd;
    suplVersion.SuplVersion(aMajor,minor,servInd);
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::GetSUPLMessageVersionL
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASUPLProtocolManager::GetSUPLMessageVersionL(
											TInt& aMajorVersion, 
											const TDesC8& aReceivedMessage)    
	{
	return GetSuplVersion(aMajorVersion,aReceivedMessage);
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::MakeLocationConversionRequestL
// 
// 
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASUPLProtocolManager::MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,TGeoCellInfo& aCellInfo,TRequestStatus& aStatus )
    {
     iTrace->Trace(_L("COMASUPLProtocolManager::MakeLocationConversionRequestL"), KTraceFileName, __LINE__);
     if(iOMASuplProtocolHandler1)
        {
         iTrace->Trace(_L("COMASUPLProtocolManager::MakeLocationConversionRequestL if condition true"), KTraceFileName, __LINE__);
        COMASUPLProtocolManager* ph1= (COMASUPLProtocolManager*) (iOMASuplProtocolHandler1);
        ph1->MakeLocationConversionRequestL(aSuplSessn,aCellInfo,aStatus);
        };
    }

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::CancelLocationConversionRequest
// 
// 
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASUPLProtocolManager::CancelLocationConversionRequest(CSuplSessionBase* aSuplSession)
    {
     if(iOMASuplProtocolHandler1)
        {
        COMASUPLProtocolManager* ph1= (COMASUPLProtocolManager*) (iOMASuplProtocolHandler1);
        ph1->CancelLocationConversionRequest(aSuplSession);
        };
    }

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::DeInitilizationCompletedL
// 
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager::DeInitilizationCompletedL(TInt /*aError*/)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager::DeInitilizationCompletedL"), KTraceFileName, __LINE__);
	iIsPhDeinitialized1 = EFalse;
	if( !iIsPhDeinitialized2 )
		{
		if( iOMASuplProtocolHandler2 )
		    {
		    iTrace->Trace(_L("COMASUPLProtocolManager::DeInitilizationCompletedL ph2 issued"), KTraceFileName, __LINE__);
		    iIsPhDeinitialized2 = ETrue;
		    iOMASuplDeInitilizer->Deinitialize(iOMASuplProtocolHandler2);
		    return;
		    }
		}
	iTrace->Trace(_L("COMASUPLProtocolManager::DeInitilizationCompletedL complete"), KTraceFileName, __LINE__);
	iIsPhDeinitialized2 = EFalse; //Reset the flag
	User::RequestComplete( iDeInitRequestStatus,KErrNone );
	}

// end of file
