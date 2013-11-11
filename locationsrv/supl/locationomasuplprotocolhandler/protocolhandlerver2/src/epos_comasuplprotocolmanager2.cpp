/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


// Includes

#include <centralrepository.h>
#include <commdb.h>
#include <etelmm.h> 
#include <ecom/ecom.h> 
#include <in_sock.h> 
#include <bautils.h>
#include <flogger.h>
#include <CoreApplicationUIsSDKCRKeys.h>

#include "epos_comasuplasnmessagebase.h"
#include "epos_comasuplsessionid.h"
#include "lbs/epos_comasuplposhandlerbase.h"
#include "epos_suplconstants.h"
#include "epos_csuplsettings.h"
#include "epos_comasupletelnotifier.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_suplterminalconstants.h"
	
#include "epos_comasuplprotocolmanager2.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_comasuplcompleteselfrequestor.h"
#include "epos_comasuplecomeventwatcher.h"
#include "epos_comasuplsettings.h"
#include "epos_omasuplconstants.h"
#include "epos_omasuplconfigurationkeys.h"
#include "epos_comasupltrace.h"
#include "epos_comasupltimeouttimer.h"
#include "epos_omasuplconstants.h"
#include "epos_comasuplcommonconfig.h"

// Constants
_LIT(KTraceFileName,"SUPL_OMA_PH::EPos_COMASUPLProtocolManager2.cpp");

const TInt KMaxIMSILen = 20;
const TInt KMaxDelay = 128;
const TInt KDefaultHorizontalAccuracy = 0;
const TInt KQueuedBufferTimeout = 11;


// -----------------------------------------------------------------------------
// ResetAndDestroy()
// Resets & destroys array if leave occurs
// -----------------------------------------------------------------------------
void ResetAndDestroy(TAny* aArray)
	{
	((RImplInfoPtrArray*)aArray)->ResetAndDestroy();
	}

// -----------------------------------------------------------------------------
// FindNearestN
// -----------------------------------------------------------------------------
TInt FindNearestN(TInt n)    
	{
	TInt count = 0;
	TInt originalNumber = n;
	while(n)
		{
		n = n >> 1;
		count++;
		}
		
	TInt previousNumber = count - 1;	
	TReal min,max;
	Math::Pow(min,2,previousNumber);
	Math::Pow(max,2,count);
	TInt avg = (min + max ) / 2;
	if (avg < originalNumber)
		return  max;
	else
		return min;
	}
	
// ============================ MEMBER FUNCTIONS ===============================    
// Create instance of concrete ECOM interface implementation
EXPORT_C COMASUPLProtocolManager2* COMASUPLProtocolManager2::NewL()
	{
	COMASUPLProtocolManager2* self = new(ELeave) COMASUPLProtocolManager2();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	} 
 
// Destructor.
EXPORT_C COMASUPLProtocolManager2::~COMASUPLProtocolManager2()
	{
	/*if(iSettingsLauncher)		
		{
			iSettingsLauncher->CancelLaunch();
		}*/
	
	if(iSuplSettings)
		{
		iSuplSettings->SetUIActive( EFalse );
		}

	DeleteAllSessions();
	iSuplSessions.Reset();
	iSuplSessions.Close();

	//Only attempt to unload the phone if the TSY has actually been initialised
	if(iTsyName.Length() > 0)
		{
	    iTelServer.UnloadPhoneModule(iTsyName);
		}

	
	iQueuedPacket.Close();
	
	delete iTimer;
		
	if(iSuplStorageSettings)
	{
		iSuplStorageSettings->RemoveObserver();
		iSuplStorageSettings->RemoveSessionObserver();
	}
	
	delete iSuplStorageSettings;
    //delete iSettingsLauncher;
    delete iNotifier;
	iMobilePhone.Close(); 
	iTelServer.Close();
	iIMSI.Close();
	iUIRequestArray.Close();	
	delete iOMASuplAsnHandlerBaseImpl;
	iTrace->Trace(_L("At the End of COMASUPLProtocolManager2::~COMASUPLProtocolManager2"), 
							KTraceFileName, __LINE__);
	delete iTrace;	
	}

// Constructor
COMASUPLProtocolManager2::COMASUPLProtocolManager2() : 
								iRunningSessionCount(0),
                                iSessnUIObserver(NULL),
                                iDeInitDone(EFalse),
                                iNotifyStarted(EFalse),
                                iTimeOutDialogPresent(EFalse)
	{
	// no implementation required
	}

// Second phase construction.
void COMASUPLProtocolManager2::ConstructL()
	{
	iTrace = COMASuplTrace::NewL();		    
	iTrace->Trace(_L("COMASUPLProtocolManager2::ConstructL"), KTraceFileName, __LINE__); 
	
	iSuplStorageSettings = CSuplSettingsInternal::NewL();
	iSuplStorageSettings->SetObserverL(*this);
	iSuplStorageSettings->SetSessionObserverL(*this);
	
   	//iSettingsLauncher = COMASuplSettingsLauncher::NewL( *this );
   	
    TRAPD(err,iOMASuplAsnHandlerBaseImpl = COMASuplAsnHandlerBase::NewL(2);) 
    TBuf<64> buf(_L("ASN Plugin 2.0 loaded with error : "));                
    buf.AppendNum(err);
    iTrace->Trace(buf,KTraceFileName, __LINE__);        
    User::LeaveIfError(err);
    
    iIMSI.CreateL( KMaxIMSILen );
		
	iIsPreviouslyDecodingFailed = EFalse;
	iTimer = COMASuplTimeoutTimer::NewL(*this);	
	}
    
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CreateNewSessionL, 
// Creates new session 
// it transfers ownership to callee
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C CSuplSessionBase* COMASUPLProtocolManager2::CreateNewSessionL(
									CSUPLProtocolManagerBase::TSuplReqType aRequestType, 
									TInt aIpcSessionId, 
									RSuplTerminalSubSession::TSuplServiceType aSuplService)
	{
	// Map the type before creating the session object
	COMASuplSession::TOMASuplReqType requestType = COMASuplSession::ESUPL_TERMINAL;
	switch(aRequestType)
		{
		case CSUPLProtocolManagerBase::ENetwork:
			requestType = COMASuplSession::ESUPL_NETWORK;
			break;
		case CSUPLProtocolManagerBase::ETerminal:
			requestType = COMASuplSession::ESUPL_TERMINAL;
		default:	
			break;
		}	
	return CreateNewSessionL(requestType, aIpcSessionId,aSuplService);
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CreateNewSessionL
// An overloaded CreateNewSession to take care of Invalid session ID
// 
// -----------------------------------------------------------------------------
//    
CSuplSessionBase* COMASUPLProtocolManager2::CreateNewSessionL(
									COMASuplSession::TOMASuplReqType aRequestType, 
									TInt aIpcSessionId,
									RSuplTerminalSubSession::TSuplServiceType /*aSuplService*/)
	{  
	iTrace->Trace(_L("Creating New OMA SUPL Session"), KTraceFileName, __LINE__); 
	CSuplSessionBase* SuplSession = COMASuplSession::NewL(
									*iCommManager, 
									iMobilePhone, 
									iSuplSettings,
									iSuplStorageSettings, 
									aRequestType, 
									*this, 
									iPosHandler, 
									iIMSI,
                                    *this, 
                                    aIpcSessionId,
                                    iOMASuplAsnHandlerBaseImpl);
  SuplSession->SetSUPLVersion(2);                                    
	User::LeaveIfError(iSuplSessions.Append(SuplSession));
  	return SuplSession;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::InitializeL 
// Does the connect to ETel. 
// Intilize POS if present 
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C  void COMASUPLProtocolManager2::InitializeL(
			CSuplCommunicationManager* aCommManager,
			TRequestStatus& /*aInitilizationRequestStatus*/)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::InitializeL"), KTraceFileName, __LINE__);

	iCommManager = aCommManager;			

	//Connect to ETel
	User::LeaveIfError(ConnectToETelL());
  
    //ReadSuplUsage();    
	 
	if(iSuplEcomEventWatcher)
		{
	    iTrace->Trace(_L("NotifyOnPlugInUnInstallation starting"), KTraceFileName, __LINE__);	
		iSuplEcomEventWatcher->NotifyOnPlugInUnInstallation();	
		}
		if(!iNotifier)
			{
    		iNotifier = COMASuplETelNotifier::NewL(*this,iMobilePhone);
    	}
	
	iTrace->Trace(_L("ETEL Notifier created"), KTraceFileName, __LINE__);	
	iTrace->Trace(_L("COMASUPLProtocolManager2::InitializeL completed"), KTraceFileName, __LINE__);
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::InitializeL 
// Cancel Initilization of Protocol manager
// 
// -----------------------------------------------------------------------------
//
EXPORT_C  void COMASUPLProtocolManager2::CancelInitialize() 
	{		
	}
   
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::RunSessionL
// Starts the SUPL Sesssion for Terminal Initiated Location Request
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::RunSessionL(
							CSuplSessionBase *const  aSuplSession,
							TRequestStatus& aStatus,
							const TDesC& aHslpAddress, 
							TBool aFallBack,
							TBool aFirstReq,
							TInt aAllowedCapabilities,
							TInt aRequestID)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::RunSessionL"), KTraceFileName, __LINE__);

#ifdef PRINT_MESSAGE
    TBuf<255> tempBuf;
    tempBuf.Append(_L("Server name in Protocol Handler is : "));
    tempBuf.Append(aHslpAddress);

    iTrace->Trace(tempBuf, KTraceFileName, __LINE__);

    
    if (aFallBack)
	    {
    	tempBuf.Copy(_L("Fallback is set as TRUE."));    
    	}
    else
    	{
    	tempBuf.Copy(_L("Fallback is set as False"));
    	}
    iTrace->Trace(tempBuf, KTraceFileName, __LINE__);
#endif	
	iTrace->Trace(_L("COMASUPLProtocolManager2::RunSessionL"), KTraceFileName, __LINE__); 

	if ( iSuplSessions.Find(aSuplSession) == KErrNotFound )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotFound ); 
		return;
		}
		
	CheckForSessionCount();
	iRunningSessionCount++;
	
	COMASuplSession* OMASession =  static_cast<COMASuplSession*>(aSuplSession);
	OMASession->SetConfigurationParameters(
							iUT1_StartTimer,
							iUT2_PosInitTimer, 
							iUT3_PosTimer,
							iPrivacyTimer,
							iSETMode,
                    		iSuplUsage, 
                    		iPersistFailTimer,
                    		iSuplInitTimeOut);
                    		
	OMASession->RunSuplSessionL(
							aStatus,
							aFirstReq, 
							aHslpAddress, 
							aFallBack, 
							aAllowedCapabilities,
							iRunningSessionCount,
							aRequestID);
	}


// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::RunSessionL
// Starts the SUPL Sesssion for Terminal Initiated Location Request,with QoP support
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::RunSessionL(
                            CSuplSessionBase *const aSuplSession,
                            TRequestStatus& aStatus,
                            const TDesC& aHslpAddress, 
                            TBool aFallBack,
                            TBool aFirstReq,
  							TInt aAllowedCapabilities,
  							TSuplTerminalQop& aQop, 
  							TInt aRequestID)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::RunSessionL"), KTraceFileName, __LINE__);
#ifdef PRINT_MESSAGE
    TBuf<255> tempBuf;
    tempBuf.Append(_L("Server name in Protocol Handler is : "));
    tempBuf.Append(aHslpAddress);

    iTrace->Trace(tempBuf, KTraceFileName, __LINE__);
    if (aFallBack)
    	{
    	tempBuf.Copy(_L("Fallback is set as TRUE."));
    	}
    else
    	{
    	tempBuf.Copy(_L("Fallback is set as False"));
    	}
    iTrace->Trace(tempBuf, KTraceFileName, __LINE__);
#endif
    
	
    
	iTrace->Trace(_L("COMASUPLProtocolManager2::RunSessionL"), KTraceFileName, __LINE__); 

	if ( iSuplSessions.Find(aSuplSession) == KErrNotFound )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotFound ); 
		return;
		}
	
	TReal32 horizonatalAccuracy;
	if(aQop.GetHorizontalAccuracy(horizonatalAccuracy) == KErrNotFound)
		{
		aQop.SetHorizontalAccuracy(KDefaultHorizontalAccuracy);
		}

	TReal32 verticalAccuracy;
	if(aQop.GetVerticalAccuracy(verticalAccuracy) == KErrNone)
		{
        if (verticalAccuracy > KOMAMaxVerticalAccuracy)            
		    aQop.SetVerticalAccuracy(KOMAMaxVerticalAccuracy);
		}

	TInt delay;
	
	if(aQop.GetDelay(delay) != KErrNotFound)
		{
		if(delay < 1 )
			{
			TRequestStatus *status = &aStatus; 
			User::RequestComplete(status,KErrArgument); 
			return;
			}
		
		if( delay >  128 )
			{
			aQop.SetDelay(KMaxDelay);
			}
		else
			{
			switch(delay)	
				{
				case 1:
				case 2:
				case 4:
				case 8:
				case 16:
				case 32:
				case 64:
				case 128:
					  break;	
				default:
					{
					aQop.SetDelay(FindNearestN(delay));
					}
				}
			}
		}
		
	CheckForSessionCount();
	iRunningSessionCount++;
	
	COMASuplSession* OMASession =  static_cast<COMASuplSession*>(aSuplSession);
	OMASession->SetConfigurationParameters(
								iUT1_StartTimer,
								iUT2_PosInitTimer, 
								iUT3_PosTimer,
								iPrivacyTimer,
								iSETMode,
                    			iSuplUsage, 
                    			iPersistFailTimer,
                    			iSuplInitTimeOut);
                    			
	OMASession->RunSuplSessionL(
								aStatus,
								aFirstReq, 
								aHslpAddress, 
								aFallBack, 
								aAllowedCapabilities,
								iRunningSessionCount,
								aQop,
								aRequestID);
	
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CheckForSessionCount
// Check for MAximum session count..if it reaches to MAX,it resets to zero
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::CheckForSessionCount()
	{
	if (iRunningSessionCount == KMaxSessionCount)
		iRunningSessionCount = 0;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::ConnectToETelL
// Connects to Telephony server & RMobilePhone
// Returns System wide error,if any occurs
// -----------------------------------------------------------------------------
//
TInt COMASUPLProtocolManager2::ConnectToETelL()
	{
	iTrace->Trace(_L("Start of COMASUPLProtocolManager2::ConnectToETelL"), KTraceFileName, __LINE__);
	TInt err = iTelServer.Connect(); 
	if(err != KErrNone)
		{
		TBuf<8> errrorCode;
		errrorCode.AppendNum(err);
		TBuf<64> msg(_L("Failed to Connect ETel with Reason Code "));
		msg.Append(errrorCode);
		iTrace->Trace(msg,KTraceFileName, __LINE__);
		return err;
		}

	iTrace->Trace(_L("Opening CommsDatabase..."), KTraceFileName, __LINE__);		
	CCommsDatabase* const db = CCommsDatabase::NewL(EDatabaseTypeUnspecified); 
	CleanupStack::PushL(db); 

	TUint32 modemId = 0; 
	db->GetGlobalSettingL(TPtrC(MODEM_PHONE_SERVICES_SMS), modemId); 
	iTrace->Trace(_L("Retriving TSY Name..."), KTraceFileName, __LINE__);		
	CCommsDbTableView* const view = db->OpenViewMatchingUintLC(TPtrC(MODEM), TPtrC(COMMDB_ID), 
																modemId); 
	err = view->GotoFirstRecord(); 
	if(err != KErrNone)
		{
		CleanupStack::PopAndDestroy(2,db);			
		return err;	
		}
	
	view->ReadTextL(TPtrC(MODEM_TSY_NAME), iTsyName); 
	err = iTelServer.LoadPhoneModule(iTsyName);
	
	if(err != KErrNone)
		{
		CleanupStack::PopAndDestroy(2,db);		
		return err;
		}
		
	// For the phone information
	RTelServer::TPhoneInfo info;

	iTelServer.GetPhoneInfo(0, info); 
	iTrace->Trace(_L("Opening RMobilePhone..."), KTraceFileName, __LINE__);		
	err = iMobilePhone.Open(iTelServer, info.iName);
	
	CleanupStack::PopAndDestroy(2,db);					
	if(err != KErrNone)
		{
		return err;	
		}
	
	iTrace->Trace(_L("End of COMASUPLProtocolManager2::ConnectToETelL"), KTraceFileName, __LINE__);		
	return err;
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::DestroySession
// Deletes specified session & removes from array.
// Returns System wide error,if any occurs
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASUPLProtocolManager2::DestroySession(CSuplSessionBase* aSuplSession)	
	{
	// Log
	iTrace->Trace(_L("COMASUPLProtocolManager2::DestroySession"), KTraceFileName, __LINE__);

	// Check if the Session is valid
	TInt index = iSuplSessions.Find(aSuplSession);

	if(KErrNotFound == index)
		{
		iTrace->Trace(_L("Cannot found Supl session."), KTraceFileName, __LINE__);
		return KErrArgument;
		}

	// Remove from Array
	iTrace->Trace(_L("COMASUPLProtocolManager2::DestroySession Remove Session"), KTraceFileName, __LINE__);
	iSuplSessions.Remove(index);

	//Last Session invoking Destroy
	iTrace->Trace(_L("COMASUPLProtocolManager2::DestroySession Destroy Session"), KTraceFileName, __LINE__);

	COMASuplSession* OMASession =  static_cast<COMASuplSession*>(aSuplSession);
    if (OMASession->GetSessionUIFlag())
    	{
	    OMASession->DestroySession();
        iSessnUIObserver = NULL;
    	}            
    else
	    OMASession->DestroySession();

	return KErrNone;
	}
	

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::DeleteAllSessions
// Delets all session & removes from array.
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::DeleteAllSessions()	
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::DeleteAllSessions"), KTraceFileName, __LINE__);
	while(iSuplSessions.Count() > 0)
		{
		DestroySession(iSuplSessions[0]);
		}		
	} 
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::HandlePacket
// Handles Supl packets ,  received from SLP
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::HandlePacket(const TDesC8& aPacket, TUint aPortNum)
	{
	iTrace->Trace(_L("Start of COMASUPLProtocolManager2::HandlePacket"), KTraceFileName, __LINE__);
	
	TRAPD(err,HandleOMASuplPacketsL(aPacket, aPortNum));
	
	if(err!= KErrNone)
		{
		ResetBufferingParameters();
		TBuf<64> msg(_L("HandleOMASuplPacketsL Leaved with error code : "));
		msg.AppendNum(err);
		iTrace->Trace(msg, KTraceFileName, __LINE__); 
		}
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::HandlePacketError
// Handles Supl packet receiving errors
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::HandlePacketError(TInt /*aError*/)
	{
	// Do nothing for now
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::TerminateSession
// Terminate the session specified by the parameter
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::TerminateSession(CSuplSessionBase *aSession, TInt aError)
	{
	// Check if the Session is valid
	TInt index = iSuplSessions.Find(aSession);
	
	if(KErrNotFound == index)
		{
		return;
		}
	
	COMASuplSession *omaSuplSession = static_cast <COMASuplSession *> (aSession);
			
	// Check the type of Session
	if(omaSuplSession->RequestType() == COMASuplSession::ESUPL_TERMINAL)
		{
		// For Terminal initiated session, do not delete the session
		omaSuplSession->SessionEnd();
		
		// Complete the client request
		omaSuplSession->CompleteSession(aError);
		}
	else
		{
		// For Network Initiated session and invalid session, delete the session
		omaSuplSession->SessionEnd();
		// Complete the client request
		omaSuplSession->CompleteSession(aError);
		DestroySession(omaSuplSession);
		}
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::HandleOMASuplPacketsL
// Handles Supl packets ,  received from SLP
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::HandleOMASuplPacketsL(const TDesC8& aPacket, TUint aPortNum)
	{
	RBuf8 Packet;
	TBool isDecodingRequired = ETrue;
	TInt errorCode = KErrNone;
	TInt unConsumedBytes = aPacket.Length();
 	TBuf<64> msg(_L("Length of Received message is  : "));
	msg.AppendNum(unConsumedBytes);
	iTrace->Trace(msg, KTraceFileName, __LINE__); 
	COMASuplAsnMessageBase* decodedAsn = NULL;
	CleanupClosePushL(Packet);
	
	iTimer->StopTimer();	 
				
	if(iIsPreviouslyDecodingFailed) //Previous decoding Failed...
		{
		User::LeaveIfError(iQueuedPacket.ReAlloc(unConsumedBytes + iQueuedPacket.Length()));
		iQueuedPacket.Append(aPacket);
		if( iQueuedPacket.Length() >= iPreviousMessageLength ) // Message length is enough to decode
			{
			isDecodingRequired = ETrue;
			User::LeaveIfError(Packet.ReAlloc(iQueuedPacket.Length()));
			Packet.Append(iQueuedPacket);
			unConsumedBytes  =  iQueuedPacket.Length(); 
			
			msg.Copy(_L("Length of Combined buffer is  : "));
			msg.AppendNum(unConsumedBytes);
			iTrace->Trace(msg, KTraceFileName, __LINE__); 
			}
		else
			{
			isDecodingRequired = EFalse;
			}	
		}
	else
		{
		User::LeaveIfError(Packet.ReAlloc(unConsumedBytes));
		Packet.Append(aPacket);
		}	
		
	while(unConsumedBytes != 0 && errorCode == KErrNone && isDecodingRequired)		
		{
		msg.Copy(_L("Unconsumed bytes are : "));
		msg.AppendNum(unConsumedBytes);
		iTrace->Trace(msg, KTraceFileName, __LINE__); 
					
		TRAPD(trapErr,decodedAsn = iOMASuplAsnHandlerBaseImpl->DecodeL(Packet.Right(unConsumedBytes),errorCode,unConsumedBytes));						
		LogPacket(aPacket,decodedAsn);
		
		if(trapErr!= KErrNone ) //Decoding leaved...clear everything.
			{
			TBuf<64> msg(_L("COMASuplAsnDecoder::DecodeL() Leaved with error code : "));
			msg.AppendNum(trapErr);
			iTrace->Trace(msg, KTraceFileName, __LINE__); 
			ResetBufferingParameters();
			CleanupStack::PopAndDestroy();	//Packet
			return;
			}
		
		if(!decodedAsn) 
			{
			CleanupStack::PopAndDestroy();	//Packet
			return;
			}
			
		if(errorCode == KErrOMASuplDecodeSessionFailed) //Decoding failed...drop packet
			{
			ResetBufferingParameters();
			delete decodedAsn;
			CleanupStack::PopAndDestroy();	//Packet
			iTrace->Trace(_L("Decoding failed..."), KTraceFileName, __LINE__);
			return;			
			}
			
		iPreviousMessageLength = decodedAsn->MessageLength();
		msg.Copy(_L("Message Length of Received SUPL message is  : "));
		msg.AppendNum(iPreviousMessageLength );
		iTrace->Trace(msg, KTraceFileName, __LINE__); 
	  
		if(iPreviousMessageLength <= 0) //Could not found message length...
			{
			ResetBufferingParameters();
			delete decodedAsn;
			CleanupStack::PopAndDestroy();	//Packet
			iTrace->Trace(_L("Negative/Zero message length..."), KTraceFileName, __LINE__);
			return;			
			}
	  
		CleanupStack::PushL(decodedAsn); 
		COMASuplSessionID* retrivedSessionID = NULL;
		decodedAsn->SessionId(retrivedSessionID);
		LogReceivedSessionIdL(retrivedSessionID);
		TBool isInvalidSLPID;
		TBool isInvalidSETID;
		COMASuplSession* suplSession = FindSession(
					retrivedSessionID,
					isInvalidSLPID, 
					isInvalidSETID); 
					
		if((isInvalidSETID && suplSession) || (isInvalidSLPID && suplSession))
			{
			iTrace->Trace(_L("Running invalid session"), KTraceFileName, __LINE__); 
			suplSession->RunInvalidSessionL(decodedAsn);
			ResetBufferingParameters();
			CleanupStack::PopAndDestroy(decodedAsn); // decodedAsn
			CleanupStack::PopAndDestroy();	//Packet
			return;
			}

		if(errorCode == KErrOMASuplShortFrame )	// Newly added code for Short Frame...			
			{
			if(suplSession)
				{
				if(suplSession->CheckProtocolVersionL(decodedAsn)) //Check for version & will send SUPL_END if mismatch
					{
					msg.Copy(_L("ASN returned Short Frame error."));
					iTrace->Trace(msg, KTraceFileName, __LINE__); 
					User::LeaveIfError(iQueuedPacket.ReAlloc(iQueuedPacket.Length() +  aPacket.Length()));
					iQueuedPacket.Append(aPacket);
					iIsPreviouslyDecodingFailed = ETrue;
					iTimer->StartTimer(KQueuedBufferTimeout * KSecond); 	
					}
				else
					{
					ResetBufferingParameters(); //After sending SUPL_END ....clearing all variables needed for TCP Buffering.
					}
					
				CleanupStack::PopAndDestroy(decodedAsn); // decodedAsn
				CleanupStack::PopAndDestroy();	//Packet
				return;
				}
			}
								
		if(suplSession) //Session Found
			{
			if(errorCode == KErrOMASuplMessageLengthMismatch) //Added after Confirmance test cases (invalid message) 
				{
				iTrace->Trace(_L("Received Message with wrong message length..."), KTraceFileName, __LINE__); 
				ResetBufferingParameters();
				suplSession->HandleInvalidMessageLengthL();
				}
			if(errorCode == KErrOMASuplInvalidMessage)	//Checks for invalid message
				{
				iTrace->Trace(_L("Received Invalid Message..."), KTraceFileName, __LINE__); 
				ResetBufferingParameters();
				suplSession->HandleInvalidMessageL(decodedAsn);
				}
				
			if(errorCode == KErrNone)	//Successful decoding of message
				{
				ResetBufferingParameters();
				iTrace->Trace(_L("Message decoded successfully...passing to appropriate session..."), KTraceFileName, __LINE__);
				suplSession->HandleOMASuplMessageL(decodedAsn);
				}
				
			if(errorCode == KErrCompletion)	//Only session id retrived from message,Send error code
				{
				iTrace->Trace(_L("Retrived sessionid.Decoding failed.Handling Invalid Parameter."), KTraceFileName, __LINE__); 
				suplSession->HandleInvalidParameterErrorL(decodedAsn);
				unConsumedBytes = 0; //Drop packet									
				}	
			}
		else  //Session Not Found....Sending SUPL_END with invalidsession
			{
                                //Check if session can be found based on port number
                                suplSession = FindSessionOnPortNum(aPortNum);

                                if (suplSession)
                                    {                                        
						            iTrace->Trace(_L("Running invalid session"), KTraceFileName, __LINE__); 
						            suplSession->RunInvalidSessionL(decodedAsn);
						            ResetBufferingParameters();
						            CleanupStack::PopAndDestroy(decodedAsn); // decodedAsn
						            CleanupStack::PopAndDestroy();	//Packet
						            return;
                                    }
                                else                                        
                                    {                                        
								    ResetBufferingParameters();
								    iTrace->Trace(_L("Received Invalid Session."), KTraceFileName, __LINE__); 
								    ProcessInvalidSessionL(decodedAsn); 
								    unConsumedBytes = 0; //Drop packet									
                                    }
					}
					
				CleanupStack::PopAndDestroy(decodedAsn);
			}
		
		CleanupStack::PopAndDestroy();	//Packet
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::FindSession
// Finds perticuler session from session array.
// 
// -----------------------------------------------------------------------------
//
COMASuplSession* COMASUPLProtocolManager2::FindSession(
								COMASuplSessionID* aRetrivedSessionID,
								TBool& aInvalidSLPId, 
								TBool& aInvalidSETId) const
	{
	
	/* 	
	Algorithm for matching session ID 
	For each session we are going to follow, following preocedure...
	if  session in array haves SET id then we are going to compare SET id  and at the same time if  SLP id 
	also present in session then we will also compare SLP id.
	If both matches then we are going to return session id.
	& if SLP Id does not matches then comparasion will be done for next session.
	If SLP id does not present in session id, in this case we will compare only SET id.
	*/

	aInvalidSLPId = EFalse;
	aInvalidSETId = EFalse;
	COMASuplSession* OMAsuplSession = NULL;
	COMASuplSessionID*  suplSessionID  = NULL;
	
	TInt TotalSession = iSuplSessions.Count();
	 
	COMASuplSETSessionID* retrivedSuplSETID = GetSETSessionID(aRetrivedSessionID);
	COMASuplSLPSessionID* retrivedSuplSLPID = GetSLPSessionID(aRetrivedSessionID);
	iTrace->Trace(_L("COMASUPLProtocolManager2::FindSession Searching in availble Sessions...  "), KTraceFileName, __LINE__);
	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
		{
		OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
		suplSessionID = OMAsuplSession->SuplSessionID();
		COMASuplSETSessionID* suplSETID = GetSETSessionID(suplSessionID);
		LogAvailableSessions(suplSETID,GetSLPSessionID(suplSessionID),cnt);
		
		if(suplSETID)
			{
            iTrace->Trace(_L("FindSession, SET ID Present ...  "), KTraceFileName, __LINE__);
			if(CompareSETSessionID(suplSETID, retrivedSuplSETID))  
				{
				COMASuplSLPSessionID* suplSLPID = GetSLPSessionID(suplSessionID);
					if(suplSLPID)
						{
						if(CompareSLPSessionID(suplSLPID, retrivedSuplSLPID))  											
							{
							iTrace->Trace(_L("COMASUPLProtocolManager2::FindSession Searching Done...Session Found"), KTraceFileName, __LINE__);		
							return OMAsuplSession;
							}
						else
							{
							aInvalidSLPId = ETrue;//means we have received message with incorrect SLP session id.
							//continue; changed after Confirmance test....
							return OMAsuplSession;
							}	
						}
						
				iTrace->Trace(_L("COMASUPLProtocolManager2::FindSession Searching Done...Session Found"), KTraceFileName, __LINE__);				
				return OMAsuplSession;																						
				}
            else 
                {
                iTrace->Trace(_L("FindSession, SET ID comp failed ...  "), KTraceFileName, __LINE__);
				COMASuplSLPSessionID* suplSLPID = GetSLPSessionID(suplSessionID);
				if(suplSLPID)
				    {
                    iTrace->Trace(_L("FindSession, SLP ID present...  "), KTraceFileName, __LINE__);
                    if (CompareSLPSessionID(suplSLPID, retrivedSuplSLPID))
                        {                                    
                        iTrace->Trace(_L("FindSession, SLP ID comp succeeded...  "), KTraceFileName, __LINE__);
				        aInvalidSETId = ETrue;//means we have received message with incorrect SET session id.
				                      //continue; changed after Confirmance test....
				        return OMAsuplSession;
                        }                                
                    }                                
				}
			}
		}
		
	iTrace->Trace(_L("COMASUPLProtocolManager2::FindSession Searching Done...Session Not Found."), KTraceFileName, __LINE__);		
	return NULL;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::FindSessionOnPortNum
// Finds perticuler session from session array.
// 
// -----------------------------------------------------------------------------
//
COMASuplSession* COMASUPLProtocolManager2::FindSessionOnPortNum(TUint aPortNum) const
	{
		iTrace->Trace(_L("COMASUPLProtocolManager2::FindSessionOnPortNum start"), KTraceFileName, __LINE__);		

        COMASuplSession* OMAsuplSession = NULL;
		TInt TotalSession = iSuplSessions.Count();
        TUint portNum=0;
		 
		iTrace->Trace(_L("COMASUPLProtocolManager2::FindSessionOnPortNum Searching in availble Sessions"), KTraceFileName, __LINE__);
		for(TInt cnt = 0; cnt < TotalSession; ++cnt)
			{
				OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
				portNum = OMAsuplSession->GetPortNumUsed();
                if (portNum != 0 && portNum == aPortNum)
                    {                 
                    iTrace->Trace(_L("COMASUPLProtocolManager2::FindSessionOnPortNum Searching Done...Session Found"), KTraceFileName, __LINE__);		
					return OMAsuplSession;
                    }
			}
			
		iTrace->Trace(_L("COMASUPLProtocolManager2::FindSessionOnPortNum Searching Done...Session Not Found."), KTraceFileName, __LINE__);		
		return NULL;
	}
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::GetSETSession
// Retrives COMASuplSETSessionID from COMASuplSessionID.
// 
// -----------------------------------------------------------------------------
//
COMASuplSETSessionID* COMASUPLProtocolManager2::GetSETSessionID(COMASuplSessionID* 
																aOMAsuplSessionID) const
	{
	if(aOMAsuplSessionID)
		{
		COMASuplSETSessionID* suplSETID = NULL;
		aOMAsuplSessionID->SETSessionID(suplSETID);
		return suplSETID;
		}
	else
		return NULL;	
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::GetSLPSession
// Retrives COMASuplSLPSessionID from COMASuplSessionID.
// 
// -----------------------------------------------------------------------------
//
COMASuplSLPSessionID* COMASUPLProtocolManager2::GetSLPSessionID(COMASuplSessionID* 
																aOMAsuplSessionID) const
	{
	if(aOMAsuplSessionID)
		{
		COMASuplSLPSessionID* suplSLPID = NULL;
		aOMAsuplSessionID->SLPSessionID(suplSLPID);
		return suplSLPID;
		}
	else
		return NULL;	
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CompareSETSessionID
// Compares two SET session IDs
// 
// -----------------------------------------------------------------------------
//
TBool COMASUPLProtocolManager2::CompareSETSessionID(COMASuplSETSessionID* aSuplSETID,
												COMASuplSETSessionID* aRetrivedSuplSETID) const
	{	
	if ( aRetrivedSuplSETID && aSuplSETID)
		return *aRetrivedSuplSETID == *aSuplSETID;
	else
		return EFalse;
	}
	 
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CompareSLPSessionID
// Compares two SLP session IDs
// 
// -----------------------------------------------------------------------------
//
TBool COMASUPLProtocolManager2::CompareSLPSessionID(COMASuplSLPSessionID* aSuplSLPID,
												COMASuplSLPSessionID* aRetrivedSuplSLPID) const
	{
	if ( aRetrivedSuplSLPID && aSuplSLPID)
		return *aRetrivedSuplSLPID == *aSuplSLPID;
	else
		return EFalse;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::ProcessInvalidSessionL
// Processes invalid session .. creates new session & sends SUPL_END
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::ProcessInvalidSessionL(COMASuplAsnMessageBase* aDecodedMsg)
	{
	// 1. Create a new session for handling the invalid session case
	CSuplSessionBase* suplSession = NULL;
			
	TRAPD(err, suplSession = CreateNewSessionL(
								COMASuplSession::ESUPL_INVALID_SESSION, 
								0, 
								RSuplTerminalSubSession::ESUPL_2_0))
	
	COMASuplSession* invalidSession = static_cast<COMASuplSession*>(suplSession);
	
	if(KErrNone == err)
		{
		invalidSession->RunInvalidSessionL(aDecodedMsg);
		}
	}
	


// -----------------------------------------------------------------------------
// COMASUPLProtocolManager1::CancelRunSession
// Cancels run session operation if its going on.
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::CancelRunSession(CSuplSessionBase* aSuplSession)
	{
	// Log
	iTrace->Trace(_L("COMASUPLProtocolManager2::CancelRunSession"), KTraceFileName, __LINE__);
	
	// Check if the Session is valid
	TInt index = iSuplSessions.Find(aSuplSession);

	if(KErrNotFound == index)
		{
		iTrace->Trace(_L("Cannot found Supl session."), KTraceFileName, __LINE__);
		return;
		}
		aSuplSession->CancelRunSession();	
		return ;
	}


    
   

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::HandleSuplMessageL
// Informs to client about Initilization of POS ,thro' active object
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C  void  COMASUPLProtocolManager2::HandleSuplMessageL(
									CSuplSessionBase *const aSuplSession,
									TRequestStatus& aStatus,
									HBufC8* aMessage)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::HandleSuplMessageL"), KTraceFileName, __LINE__);
	TInt index = iSuplSessions.Find(aSuplSession);
	TRequestStatus* status =  &aStatus;

	if(KErrNotFound == index)
		{
		iTrace->Trace(_L("Cannot found Supl session."), KTraceFileName, __LINE__);
		if(status)
			User::RequestComplete(status,KErrNotFound);
		return;
		}
	
	if(!aMessage)
		{
		iTrace->Trace(_L("Received packet is Empty."), KTraceFileName, __LINE__);
		if(status)
			User::RequestComplete(status,KErrBadDescriptor);
		return;
		}

#ifdef PRINT_MESSAGE
    RFile file;
	RFs   fs;
	TInt cErr=fs.Connect();
	TInt fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
	if (fErr == KErrNotFound || fErr == KErrPathNotFound)
		{                             
		TBool ret = BaflUtils::FolderExists(fs,_L("c:\\logs\\epos\\"));
		if(!ret)
			{
			fs.MkDirAll(_L("c:\\logs\\epos\\"));
			}
		file.Create(fs, _L("c:\\logs\\epos\\packet.txt"), EFileWrite|EFileShareAny);
		fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
		}

    _LIT8(KMsgType, "SUPL INIT");
    file.Write(KMsgType);
    file.Write(_L8("\r \n"));
	file.Write(*aMessage);
    file.Write(_L8("\r \n"));
	file.Close();
	fs.Close();
#endif

	TInt unConsumedBytes = aMessage->Length();
	COMASuplAsnMessageBase* decodedAsn = NULL;
	TInt errorCode;
	TRAP_IGNORE(decodedAsn = iOMASuplAsnHandlerBaseImpl->DecodeL(aMessage->Right(unConsumedBytes),errorCode,unConsumedBytes));						
	
	if(errorCode == KErrOMASuplDecodeSessionFailed || errorCode == KErrCompletion) //Decoding failed...drop packet
		{
		delete decodedAsn;
		iTrace->Trace(_L("COMASUPLProtocolManager2::HandleSuplMessageL::Decoding failed..."), KTraceFileName, __LINE__);
		if(status)
			User::RequestComplete(status,KErrGeneral);
		return;			
		}

	if(errorCode == KErrNone 	|| errorCode == KErrOMASuplOutOfRangeParameter 
								|| errorCode == KErrOMASuplMessageLengthMismatch
								|| errorCode == KErrOMASuplShortFrame)		
							 	// Successful decoding of message
							 	// KErrOMASuplOutOfRangeParameter added this for supporting 
		 	 					// handling of out of range parameters.
		{
		if(!decodedAsn)
			{
			if(status)
				User::RequestComplete(status,KErrGeneral);
				return;			
			} 
	
		COMASuplAsnMessageBase::TSuplMessageType messageType = decodedAsn->MessageType();
		if(messageType == COMASuplAsnMessageBase::ESUPL_END )
			{
			delete decodedAsn;
			iTrace->Trace(_L("Received SUPL_END thro' Netinitiated Lib...."), KTraceFileName, __LINE__);
			if(status)
				User::RequestComplete(status,KErrGeneral);
			return;			
			}
			
		CleanupStack::PushL(decodedAsn); 
		COMASuplSession* suplSession = static_cast<COMASuplSession*>(aSuplSession);
		if(suplSession)
			{
			CheckForSessionCount();
			iRunningSessionCount++;
			suplSession->SetEncodedSuplInitL(aMessage);
			suplSession->SetConfigurationParameters(iUT1_StartTimer,iUT2_PosInitTimer, iUT3_PosTimer,iPrivacyTimer,iSETMode,
	                        iSuplUsage, iPersistFailTimer,iSuplInitTimeOut);
			suplSession->HandleOMASuplMessageL(decodedAsn,aStatus,iRunningSessionCount,errorCode);
			}
		CleanupStack::PopAndDestroy(decodedAsn);
		}
	else
		{
		delete decodedAsn;
		iTrace->Trace(_L("COMASUPLProtocolManager2::HandleSuplMessageL::Error occured after decoding..."), KTraceFileName, __LINE__);
		if(status)
			User::RequestComplete(status,KErrGeneral);
		}
	}


// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::PlugInUninstalled
// Handles Pos handler unload event
// -----------------------------------------------------------------------------
void COMASUPLProtocolManager2::PlugInUninstalled() // change here --- Subhra
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::PlugInUninstalled"), KTraceFileName, __LINE__);
	
	TInt suplSessionCount = iSuplSessions.Count();
	
	for(TInt loop = 0; loop < suplSessionCount;loop++)
		{
		iTrace->Trace(_L("COMASUPLProtocolManager2::PlugInUninstalled Session retrieved"), KTraceFileName, __LINE__);		
		
		COMASuplSession* suplSession = REINTERPRET_CAST(COMASuplSession*, iSuplSessions[loop]);
		suplSession->PosPlugInUnloaded();
		}
		
	iTrace->Trace(_L("COMASUPLProtocolManager2::PlugInUninstalled removing PosH"), KTraceFileName, __LINE__);		
	
	iTrace->Trace(_L("COMASUPLProtocolManager2::PlugInUninstalled PosH removed"), KTraceFileName, __LINE__);		
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::HandleSuplSettingsChangeL
// notifier for settings changes.
// -----------------------------------------------------------------------------
void COMASUPLProtocolManager2::HandleSuplSettingsChangeL(TSuplSettingsEventType aEvent,TInt /*aSlpId*/)
	{
    if( aEvent == MSuplSettingsObserver::ESuplSettingsEventSuplUsageChange)
        {
        iTrace->Trace(_L("Setting Changed.SUPL Usage changed.. Reeading once again..."), KTraceFileName, __LINE__);
        //ReadSuplUsage();
		}
	else if( aEvent == MSuplSettingsObserver::ESuplSettingsEventCommParameterChange)
		{
		iTrace->Trace(_L("Setting Changed.Communication Parameters changed.. Reeading once again..."), KTraceFileName, __LINE__);
						
		}
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::HandleSuplTriggerStatusChangeL
// notifier for settings changes.
// -----------------------------------------------------------------------------
void COMASUPLProtocolManager2::HandleSuplTriggerStatusChangeL(CSuplSettings::TSuplTriggerStatus aSuplTriggerStatus)
	{
  	iTrace->Trace(_L("Setting Changed.SUPL Trigger Status Changed..."), KTraceFileName, __LINE__);
		if( aSuplTriggerStatus == CSuplSettings::ESuplTriggerOff)
		{
						iTrace->Trace(_L("Setting Changed.SUPL Trigger Status to OFF..."), KTraceFileName, __LINE__);
						TInt TotalSession = iSuplSessions.Count();
						for(TInt cnt = 0; cnt < TotalSession; ++cnt)
							{
									iTrace->Trace(_L("Cancelling Trigger Session..."), KTraceFileName, __LINE__);
									COMASuplSession* OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
	 								OMAsuplSession->CancelTriggerSession();		
							}
		}
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::LogPacket
// Logs
// -----------------------------------------------------------------------------
/*
# if 0
void COMASUPLProtocolManager2::LogPacket(const TDesC8& aPacket,COMASuplAsnMessageBase* decodedAsn)
	{
#ifdef PRINT_MESSAGE
    RFile file;
    RFs   fs;
    TInt cErr=fs.Connect();
    
    TInt fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
	if (fErr == KErrNotFound)
		{                            
		file.Create(fs, _L("c:\\logs\\epos\\packet.txt"), EFileWrite|EFileShareAny);
		fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"), EFileWrite|EFileShareAny);
		}
	
    TInt aPos;
    file.Seek(ESeekEnd, aPos);

	if(decodedAsn)	
		{
        COMASuplAsnMessageBase::TSuplMessageType messageType = decodedAsn->MessageType();

        switch(messageType)
	        {
	        case COMASuplAsnMessageBase::ESUPL_RESPONSE:
                _LIT8(KMsgType1, "SUPL RESPONSE");
                file.Write(KMsgType1);
                break;
            case COMASuplAsnMessageBase::ESUPL_POS:
                _LIT8(KMsgType2, "SUPL POS");
                file.Write(KMsgType2);
                break;
	        case COMASuplAsnMessageBase::ESUPL_END:
                _LIT8(KMsgType3, "SUPL END");
                file.Write(KMsgType3);
		 	    break;
	        case COMASuplAsnMessageBase::ESUPL_AUTH_RESP:
                _LIT8(KMsgType4, "SUPL AUTH RESPONSE");
                file.Write(KMsgType4);
                break;
	        default:
		        break;	
	        }
		}
	else
		{
		_LIT8(KMsgTypeUnknown, "Decoding Failed.");
		file.Write(KMsgTypeUnknown);
		}
    file.Write(_L8("\r \n")); 
    file.Write(aPacket);
    file.Write(_L8("\r \n")); 
    file.Close();
    fs.Close();
#endif                
	
	}
#endif
*/

void COMASUPLProtocolManager2::LogPacket(const TDesC8& 
#if defined(PRINT_MESSAGE)
	aPacket
#endif
	,COMASuplAsnMessageBase* 
#if defined(PRINT_MESSAGE)
	decodedAsn
#endif
	)
	{
	
#ifdef PRINT_MESSAGE
	RFileLogger  iLogger;
	
	TInt err = iLogger.Connect();
		if(err != KErrNone) 
			return;
		
	const TText* hdr1 = _S("Header");
	const TText* mrg1 = _S("End");
    if(decodedAsn)	
		{
        COMASuplAsnMessageBase::TSuplMessageType messageType = decodedAsn->MessageType();

    	switch(messageType)
	        {
	        case COMASuplAsnMessageBase::ESUPL_RESPONSE:
		        {
				_LIT(KMsgType1, "SUPL_RESPONSE.txt");
				iLogger.CreateLog(_L("epos"),KMsgType1,EFileLoggingModeAppend);
				iLogger.HexDump(hdr1, mrg1, aPacket.Ptr(), aPacket.Length() );
				break;
				}	
			case COMASuplAsnMessageBase::ESUPL_POS:
				{
				_LIT(KMsgType2, "SUPL_POS.txt");
				iLogger.CreateLog(_L("epos"),KMsgType2,EFileLoggingModeAppend);
				iLogger.HexDump(hdr1, mrg1, aPacket.Ptr(), aPacket.Length() );
				break;
				}      
			case COMASuplAsnMessageBase::ESUPL_END:
				{
				_LIT(KMsgType3, "SUPL_END.txt");
				iLogger.CreateLog(_L("epos"),KMsgType3,EFileLoggingModeAppend);
				iLogger.HexDump(hdr1, mrg1, aPacket.Ptr(), aPacket.Length() );
				break;
				}          
			case COMASuplAsnMessageBase::ESUPL_AUTH_RESP:
				{
				_LIT(KMsgType4, "SUPL_AUTH_RESPONSE.txt");
				iLogger.CreateLog(_L("epos"),KMsgType4,EFileLoggingModeAppend);
				iLogger.HexDump(hdr1, mrg1, aPacket.Ptr(), aPacket.Length() );
				break;
				}        
	        default:
		        break;	
	        }
		}
	else
		{
		_LIT(KMsgTypeUnknown, "Decode_Failed.txt");
		iLogger.CreateLog(_L("epos"),KMsgTypeUnknown,EFileLoggingModeAppend);
		iLogger.HexDump(hdr1, mrg1, aPacket.Ptr(), aPacket.Length() );
		}
	iLogger.Close();
#endif           
	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::LogAvailableSessions
// Logs
// -----------------------------------------------------------------------------
void COMASUPLProtocolManager2::LogAvailableSessions( COMASuplSETSessionID* 
#if defined(PRINT_MESSAGE)
	aSuplSETID
#endif
	,COMASuplSLPSessionID* 
#if defined(PRINT_MESSAGE)
	aSuplSLPID
#endif
	,TInt 
#if defined(PRINT_MESSAGE)
	aCount
#endif
	) const

	{
#ifdef PRINT_MESSAGE		//This is only for logging purpose.
	
	TBuf<128> msg(_L("Session Ids For Session No : "));
	msg.AppendNum(aCount);
	iTrace->Trace(msg,KTraceFileName, __LINE__); 
	
	if(!aSuplSETID) 
		{
	 	iTrace->Trace(_L("No SET Session ID present..."),KTraceFileName, __LINE__);
		}
	else
		{
	    msg.Copy(_L("Comparing with Session ID : "));
		TBuf<4> id;	
		TInt    SETSessionID;
		TBuf8<64> IPvAddress8;
		TBuf<64>  IPvAddress;
		
		aSuplSETID->SuplSETSessionID(SETSessionID,IPvAddress8);	
		COMASuplSETSessionID::TSuplSETIDType setIDType = aSuplSETID->SuplSETIDType();
		if(setIDType == COMASuplSETSessionID::EIMSI )
			{
			iTrace->Trace(_L("SET ID is of type IMSI"),KTraceFileName, __LINE__);
			iTrace->Trace(_L("IMSI Value :"),KTraceFileName, __LINE__);
			msg.Copy(IPvAddress8);
			}
		else
			{
		    TLex8 lex(IPvAddress8);

		    TChar chr;	
		
		    chr = lex.Get();
		    TUint i = chr;
		    IPvAddress.AppendNum(i);
		    IPvAddress.Append(_L("."));
		
		    chr = lex.Get();
		    i = chr;
		    IPvAddress.AppendNum(i);
		    IPvAddress.Append(_L("."));

		    chr = lex.Get();
		    i = chr;
		    IPvAddress.AppendNum(i);
		    IPvAddress.Append(_L("."));

		    chr = lex.Get();
		    i = chr;
		    IPvAddress.AppendNum(i);
		
		    id.AppendNum(SETSessionID);
		    msg.Append(id);
		    msg.Append(_L("  IPAddress :"));
		    msg.Append(IPvAddress);
			}				

		}
#endif                          
  
#ifdef PRINT_MESSAGE		//This is only for logging purpose.
	if(!aSuplSLPID) 
		{
		iTrace->Trace(_L("No SLP Session ID present..."),KTraceFileName, __LINE__);
		}
	else
		{
		TBuf8<4> sessionId8;
		TBuf<32> sessionId;
		TBuf8<64> ipAddress8(_L8("No Address field in SLP Session ID"));				
		TBuf<64> ipAddress;

		COMASuplSLPAddress* slpIPAddress = NULL;
		aSuplSLPID->SuplSLPSessionID(sessionId8,slpIPAddress);

		if(slpIPAddress)
		    {
		    COMASuplSLPAddress::TSuplSLPAddressType slpAddType = slpIPAddress->SLPAddressType();
		    if(slpAddType == COMASuplSLPAddress::EFqdn)
		        {
		        TBuf<128> msg(_L("Comparing with SLP Session ID : "));
		        HBufC8* fqdn = NULL;
		        slpIPAddress->Fqdn(fqdn);
		        TLex8 lex1(sessionId8);
		        TChar chr;	
		        for(TInt i = 0; i < 4; i++)	
		            {
		            chr = lex1.Get();
		            TUint num = chr;
		            sessionId.AppendNum(num);
		            }
		        msg.Append(sessionId);
		        iTrace->Trace(msg,KTraceFileName, __LINE__); 
		        iTrace->Trace(_L("SLP Address is FQDN"),KTraceFileName, __LINE__); 
		        iTrace->Trace(_L("FQDN Value"),KTraceFileName, __LINE__); 
		        if(fqdn)
		            {	
		            HBufC* fqdnBuffer = NULL; 
		            TInt err;
		            TRAP(err,fqdnBuffer = HBufC::NewL(fqdn->Length()));
		            err = 0;
		            fqdnBuffer->Des().Copy(fqdn->Des());
		            iTrace->Trace(fqdnBuffer->Des(),KTraceFileName, __LINE__);
		            delete fqdnBuffer;
		            fqdnBuffer = NULL; 
		            }
		        }
		    else
		        {
		        TBuf<128> msg(_L("Comparing with SLP Session ID : "));
		        TInt err = slpIPAddress->IPvAddress(ipAddress8);	
		        if(err != KErrNone)
		            {
	                msg.Copy(_L("SLP Address is not set."));
	                iTrace->Trace(msg,KTraceFileName, __LINE__);
		            }	
		        else
		            {                                                        
		            TLex8 lex(ipAddress8);
		            TChar chr;	

		            chr = lex.Get();
		            TUint i = chr;
		            ipAddress.AppendNum(i);
		            ipAddress.Append(_L("."));

		            chr = lex.Get();
		            i = chr;
		            ipAddress.AppendNum(i);
		            ipAddress.Append(_L("."));

		            chr = lex.Get();
		            i = chr;
		            ipAddress.AppendNum(i);
		            ipAddress.Append(_L("."));

		            chr = lex.Get();
		            i = chr;
		            ipAddress.AppendNum(i);

		            TLex8 lex1(sessionId8);
		            for(TInt i = 0; i < 4; i++)	
		                {
		                chr = lex1.Get();
		                TUint num = chr;
		                sessionId.AppendNum(num);
		                }
		            msg.Append(sessionId);
		            msg.Append(_L("  IPAddress : "));
		            msg.Append(ipAddress);
		            iTrace->Trace(msg,KTraceFileName, __LINE__);
		            }							
		        }							
		    }
		}
#endif                   
	}
	
void COMASUPLProtocolManager2::LogReceivedSessionIdL(COMASuplSessionID* 
#if defined(PRINT_MESSAGE)
	aRetrivedSessionID
#endif
	)
	{
	
#ifdef PRINT_MESSAGE		//This is only for logging purpose.                 
	if(! aRetrivedSessionID) 
	return;
	
	TBuf<128> msg(_L("Received Session Id is..."));			
	iTrace->Trace(msg,KTraceFileName, __LINE__); 							
	
    COMASuplSETSessionID* suplSETID = NULL;
    aRetrivedSessionID->SETSessionID(suplSETID);
	
	COMASuplSLPSessionID* suplSLPID = NULL;
	aRetrivedSessionID->SLPSessionID(suplSLPID);
	
	//SET Part			
	if(suplSETID)
		{
		msg.Copy(_L("SET Session ID : "));
		TBuf<8> id;	
		TInt    SETSessionID;
		TBuf8<64> IPvAddress8;
		TBuf<64>  IPvAddress;
		
		suplSETID->SuplSETSessionID(SETSessionID,IPvAddress8);	
		COMASuplSETSessionID::TSuplSETIDType setIDType = suplSETID->SuplSETIDType();
		if(setIDType == COMASuplSETSessionID::EIMSI )
			{
			iTrace->Trace(_L("SET ID is of type IMSI"),KTraceFileName, __LINE__);
			iTrace->Trace(_L("IMSI Value :"),KTraceFileName, __LINE__);
			msg.Copy(IPvAddress8);
			}
		else
			{
			TLex8 lex(IPvAddress8);
		
			TChar chr;	
			
			chr = lex.Get();
			TUint i = chr;
			IPvAddress.AppendNum(i);
			IPvAddress.Append(_L("."));
			
			chr = lex.Get();
			i = chr;
			IPvAddress.AppendNum(i);
			IPvAddress.Append(_L("."));

			chr = lex.Get();
			i = chr;
			IPvAddress.AppendNum(i);
			IPvAddress.Append(_L("."));

			chr = lex.Get();
			i = chr;
			IPvAddress.AppendNum(i);
			
			id.AppendNum(SETSessionID);
			msg.Append(id);
			msg.Append(_L("  IPAddress :"));
			msg.Append(IPvAddress);
			}				
		}
	else
		{
		msg.Copy(_L("SET Session Id is not set."));
		}
		
	//SLP Part			
	if(suplSLPID)
		{
		msg.Copy(_L("SLP Session ID : "));
		TBuf8<4> sessionId8;
		TBuf<32> sessionId;
		TBuf8<64> ipAddress8(_L8("No Address field in SLP Session ID"));				
		TBuf<64> ipAddress;

		COMASuplSLPAddress* slpIPAddress = NULL;
		suplSLPID->SuplSLPSessionID(sessionId8,slpIPAddress);
		
		if(slpIPAddress)
			{
			COMASuplSLPAddress::TSuplSLPAddressType slpAddType = slpIPAddress->SLPAddressType();
			if(slpAddType == COMASuplSLPAddress::EFqdn)
				{
				HBufC8* fqdn = NULL;
				slpIPAddress->Fqdn(fqdn);
				iTrace->Trace(_L("SLP Address is FQDN"),KTraceFileName, __LINE__); 
				iTrace->Trace(_L("FQDN Value"),KTraceFileName, __LINE__); 
				if(fqdn)
					{	
					HBufC* fqdnBuffer = NULL; 
					fqdnBuffer = HBufC::NewL(fqdn->Length());
					fqdnBuffer->Des().Copy(fqdn->Des());
					iTrace->Trace(fqdnBuffer->Des(),KTraceFileName, __LINE__);
					delete fqdnBuffer;
					fqdnBuffer = NULL; 
					}
				}
			else
				{
				TInt err = slpIPAddress->IPvAddress(ipAddress8);	
				if(err != KErrNone)
					{
					msg.Copy(_L("SLP Address is not set."));
					iTrace->Trace(msg,KTraceFileName, __LINE__); 								
					}	
				}							
			}
		
		TLex8 lex(ipAddress8);
		TChar chr;	
		
		chr = lex.Get();
		TUint i = chr;
		ipAddress.AppendNum(i);
		ipAddress.Append(_L("."));
		
		chr = lex.Get();
		i = chr;
		ipAddress.AppendNum(i);
		ipAddress.Append(_L("."));

		chr = lex.Get();
		i = chr;
		ipAddress.AppendNum(i);
		ipAddress.Append(_L("."));

		chr = lex.Get();
		i = chr;
		ipAddress.AppendNum(i);
		
		TLex8 lex1(sessionId8);
		for(TInt i = 0; i < 4; i++)	
			{
			chr = lex1.Get();
			TUint num = chr;
			sessionId.AppendNum(num);
			}
			
		msg.Append(sessionId);
		msg.Append(_L("  IPAddress : "));
		msg.Append(ipAddress);
		iTrace->Trace(msg,KTraceFileName, __LINE__); 								
		}
	else
		{
		msg.Copy(_L("SLP Session Id is not set."));
		iTrace->Trace(msg,KTraceFileName, __LINE__); 								
		}
#endif
	
	}
	
void  COMASUPLProtocolManager2::TimerExpiredL()
	{
	ResetBufferingParameters();
	}
	
void COMASUPLProtocolManager2::ResetBufferingParameters()
	{
	iQueuedPacket.Zero(); //Reset all parameters
	iPreviousMessageLength = 0 ; 
	iIsPreviouslyDecodingFailed = EFalse;
	}
	
	

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::DeInitialize
// 
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::DeInitialize(TRequestStatus& aDeInitRequestStatus)
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::DeInitialize"), KTraceFileName, __LINE__);
	
	iTrace->Trace(_L("COMASUPLProtocolManager2::DeInitialize"), KTraceFileName, __LINE__);
	iDeInitRequestStatus = & aDeInitRequestStatus;
    *iDeInitRequestStatus = KRequestPending;

    
    // If settings UI is not active, complete the request immediately
    if (!iSuplSettings->IsUIActive())
        {                    
	    iTrace->Trace(_L("COMASUPLProtocolManager2::DeInitialize, UI is not active, Completing request"), KTraceFileName, __LINE__);
        User::RequestComplete(iDeInitRequestStatus,KErrNone);
        }
    else
        {            
        iDeInitDone = ETrue;
        }
    }            
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CancelDeInitialize
// 
// 
// -----------------------------------------------------------------------------
//	
void COMASUPLProtocolManager2::CancelDeInitialize()
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::CancelDeInitialize"), KTraceFileName, __LINE__);
    iDeInitDone = EFalse;
    if(iDeInitRequestStatus)
    	User::RequestComplete(iDeInitRequestStatus,KErrCancel);
    }
    
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::GetNetworkModeL
// Logs
// -----------------------------------------------------------------------------
TInt COMASUPLProtocolManager2::GetNetworkModeL()
	{
	TInt networkMode;
	// get network mode
	CRepository* centralRepository = CRepository::NewLC(KCRUidCoreApplicationUIs);
	User::LeaveIfError(centralRepository->Get(KCoreAppUIsNetworkConnectionAllowed, networkMode));
    CleanupStack::PopAndDestroy(centralRepository);
    return networkMode;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::LaunchSuplUsageSettingsUI
// 
// 
// -----------------------------------------------------------------------------
//

/*TInt COMASUPLProtocolManager2::LaunchSuplUsageSettingsUI(MOMASuplUICompletionObserver* aObserver, TBool aRoaming)	
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::LaunchSuplUsageSettingsUI"), KTraceFileName, __LINE__);
    if (iSuplSettings->IsUIActive() && !iTimeOutDialogPresent)
        return KErrInUse;                
    // If timeout dialog is present, close it
    if (iTimeOutDialogPresent)
        {
        iTimeOutDialogPresent = EFalse;
        iSuplSettings->SetUIActive(EFalse);
        iSettingsLauncher->Cancel();
        }                
    iSessnUIObserver = aObserver;            
    iSuplSettings->SetUIActive(ETrue);
    return iSettingsLauncher->LaunchSuplUsageSettingsUI(aRoaming);
	}*/

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::SettingsUsageUICompletedL
// 
// 
// -----------------------------------------------------------------------------
//
/*void COMASUPLProtocolManager2::SettingsUsageUICompletedL(TInt aError)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::SettingsUsageUICompletedL"), KTraceFileName, __LINE__);
    iSuplSettings->SetUIActive(EFalse);

    if (iDeInitDone)
        {
	    iTrace->Trace(_L("COMASUPLProtocolManager2::SettingsUsageUICompletedL, de-init done.  Completing request"), KTraceFileName, __LINE__);
        iDeInitDone = EFalse;
        User::RequestComplete(iDeInitRequestStatus,KErrNone);
        }                
    else
        {
        if (iSessnUIObserver)                
        	iSessnUIObserver->SettingsUsageUICompletedL(aError);
        }

    CheckOutstandingUsageUIRequestsL();
	}*/

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::ReadSuplUsage
// Reads Supl Usage from Central Repository
// 
// -----------------------------------------------------------------------------
//
/*void COMASUPLProtocolManager1::ReadSuplUsage()
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::ReadSuplUsage start"), KTraceFileName, __LINE__);	
	CSuplSettings::TSuplSettingsUsage suplUsage;
	TInt err = iSuplStorageSettings->GetSuplUsage(suplUsage);		
	if(err == KErrNone ) 
		{
			iSuplSettings->SetSUPLUsage(suplUsage);
		}
	iTrace->Trace(_L("COMASUPLProtocolManager2::ReadSuplUsage end"), KTraceFileName, __LINE__);	
	}*/

void COMASUPLProtocolManager2::UpdateAllSubSessnsInSameSession(TInt aIpcSessionId)
    {
	TInt TotalSession = iSuplSessions.Count();
	COMASuplSession* OMAsuplSession = NULL;

	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
		{
			OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
            TInt ipcSessnId = OMAsuplSession->SuplIpcSessionID();

            if (ipcSessnId > 0 && ipcSessnId == aIpcSessionId)
                OMAsuplSession->UpdateSuplUsage();
		}
    }        


void COMASUPLProtocolManager2::SettingsChanged()
    {
	TInt TotalSession = iSuplSessions.Count();
	COMASuplSession* OMAsuplSession = NULL;

	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
		{
			OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
            OMAsuplSession->SettingsChanged();
		}
    }        

void COMASUPLProtocolManager2::CheckOutstandingUsageUIRequestsL()
    {
	TInt TotalSession = iSuplSessions.Count();
	COMASuplSession* OMAsuplSession = NULL;

	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
		{
			OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);

            if (OMAsuplSession->GetSuplUsageFlag())
                {                        
                //OMAsuplSession->StartUsageDialogLaunchL();
                break;
                }
		}
    }        

void COMASUPLProtocolManager2::CheckForRoaming()
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::CheckForRoaming"), KTraceFileName, __LINE__);
    if (iRoamingCheckStarted)
        return;                
    iNotifier->CheckForRoaming();            
    iRoamingCheckStarted = ETrue;
    }

void COMASUPLProtocolManager2::NotifyCurrentNetworkChange()
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::NotifyCurrentNetworkChange"), KTraceFileName, __LINE__);
    if (iNotifyStarted)
        return;            
    iNotifier->NotifyCurrentNetworkChange();        
    iNotifyStarted = ETrue;            
    }

void COMASUPLProtocolManager2::CancelNotify()
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::CancelNotify"), KTraceFileName, __LINE__);
    // Cancel only none of the the sessions currently requested
    // for network change notification
    TInt TotalSession = iSuplSessions.Count();
    TInt notifyCount=0;
	COMASuplSession* OMAsuplSession = NULL;

	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
	    {
	    OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
        if (OMAsuplSession->IsEtelNotifySet())
            {                        
            notifyCount++;
            break;            
            }
        }            
    if (notifyCount == 0)
        {            
        iNotifier->Cancel();            
        iNotifyStarted = EFalse;
        }
    }

void COMASUPLProtocolManager2::HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw)
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::HandleCurrentNetworkChangeL"), KTraceFileName, __LINE__);
	TInt TotalSession = iSuplSessions.Count();
	COMASuplSession* OMAsuplSession = NULL;

	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
	    {
	    OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
        if (OMAsuplSession->IsEtelNotifySet())
            {                        
            OMAsuplSession->HandleCurrentNetworkChangeL(aErrorCode, aHomeNw);
            break;
            }
        }            
    }            

void COMASUPLProtocolManager2::HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw)
    {
	iTrace->Trace(_L("COMASUPLProtocolManager2::HandleRoamingCheckCompleteL"), KTraceFileName, __LINE__);
    iRoamingCheckStarted = EFalse;
	TInt TotalSession = iSuplSessions.Count();
	COMASuplSession* OMAsuplSession = NULL;

	for(TInt cnt = 0; cnt < TotalSession; ++cnt)
	    {
	    OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
        if (OMAsuplSession->IsEtelRoamingSet())
            {                        
            OMAsuplSession->HandleRoamingCheckCompleteL(aErrorCode, aHomeNw);
            break;
            }
        }            
    }            

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::LaunchSuplDialogTimeoutUI
// 
// 
// -----------------------------------------------------------------------------
//
/*TInt COMASUPLProtocolManager2::LaunchSuplDialogTimeoutUI(MOMASuplUICompletionObserver* aObserver )
    {
    iTrace->Trace(_L("COMASUPLProtocolManager2::LaunchTimeOutUI"), KTraceFileName, __LINE__);
    
    iSessnUIObserver = aObserver;            
    if (iSuplSettings->IsUIActive())
        {
        iSuplSettings->SetUIActive(EFalse);
        iSettingsLauncher->Cancel();
        }                

    iSuplSettings->SetUIActive(ETrue);
    iTimeOutDialogPresent = ETrue;
    return iSettingsLauncher->LaunchSessionTimeOutDialog();		
    }*/

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::SettingsTimeOutUICompletedL
// 
// 
// -----------------------------------------------------------------------------
//
/*void COMASUPLProtocolManager2::SettingsTimeOutUICompletedL(TInt aError)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::SettingsTimeOutUICompletedL"), KTraceFileName, __LINE__);
    iSuplSettings->SetUIActive(EFalse);
    iTimeOutDialogPresent = EFalse;

    if (iDeInitDone)
        {
	    iTrace->Trace(_L("COMASUPLProtocolManager2::SettingsTimeOutUICompletedL, Completing request"), KTraceFileName, __LINE__);
        iDeInitDone = EFalse;
        User::RequestComplete(iDeInitRequestStatus,KErrNone);
        }                
    else
        {
        if (iSessnUIObserver)                
        	iSessnUIObserver->SettingsTimeOutUICompletedL(aError);
        }
	}*/

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::GetLastUsedAccessPoint
// 
// 
// -----------------------------------------------------------------------------
//
TInt COMASUPLProtocolManager2::GetLastUsedAccessPoint(TDes& /*aLastlyUsedAccessPoint*/,TUint32& /*iIAPId*/)
	{
	//return iSettingsLauncher->GetIAPName(aLastlyUsedAccessPoint,iIAPId);
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::PrintHex
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::PrintHex(const TDesC8& aBuffer,TInt aLine)
	{
	TBuf<128> buffer;
	TBuf<8> buff;
	_LIT16(KFormat1,"%02x");
	TInt len = aBuffer.Length();
	for(TInt i = 0 ; i <len; i++)
		{
		buff.Zero();
		buff.Format(KFormat1,aBuffer[i]);
		iTrace->Trace(buff, KTraceFileName, aLine);
		}
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::RunSuplTriggerSessionL
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::RunSuplTriggerSessionL(
  					CSuplSessionBase *const aSuplSession,
  					TRequestStatus& aStatus,
  					TSuplTerminalPeriodicTrigger& aPTrigger,
                    const TDesC& aHslpAddress, 
                    TBool aFallBack,
  					TInt aAllowedCapabilities,
  					TInt aRequestID
  					)
	{	
	iTrace->Trace(_L("COMASUPLProtocolManager2::RunSuplTriggerSessionL"), 
									KTraceFileName, __LINE__);
	aStatus = KRequestPending;    
	if ( iSuplSessions.Find(aSuplSession) == KErrNotFound )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotFound ); 
		return;
		}
		
	CheckForSessionCount();
	iRunningSessionCount++;
	
	COMASuplSession* OMASession =  static_cast<COMASuplSession*>(aSuplSession);
	
	OMASession->SetConfigurationParameters(
						iUT1_StartTimer,
						iUT2_PosInitTimer, 
						iUT3_PosTimer,
						iPrivacyTimer,
						iSETMode,
                    	iSuplUsage, 
                    	iPersistFailTimer,
                    	iSuplInitTimeOut);
                    	
	OMASession->RunTriggerSessionL(
						aStatus, 
						aPTrigger,
						aHslpAddress,
						aFallBack,
						aAllowedCapabilities,
						iRunningSessionCount,
						aRequestID);
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::CancelPeriodicTriggerSession
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::CancelSuplTriggerSession(CSuplSessionBase* aSuplSession)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::CancelTriggerSession"), KTraceFileName, __LINE__);

	// Check if the Session is valid
	TInt index = iSuplSessions.Find(aSuplSession);

	if(KErrNotFound == index)
		{
		iTrace->Trace(_L("Cannot found Supl session."), KTraceFileName, __LINE__);
		return;
		}
	aSuplSession->CancelTriggerSession();
	return ;
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::NotifyTriggerFiredL
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASUPLProtocolManager2::NotifyTriggerFiredL(
    				 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus, 
        			 TSuplTriggerFireInfo& aFireInfo)
	{
	iTrace->Trace(_L("COMASUPLProtocolManager2::NotifyTriggerFiredL"), KTraceFileName, __LINE__);
	
	aStatus = KRequestPending;    
	if ( iSuplSessions.Find(aSuplSession) == KErrNotFound )
	    {
	    iTrace->Trace(_L("Cannot find Supl session.Completing with KErrNotFound"), KTraceFileName, __LINE__);
	    TRequestStatus *status = &aStatus; 
	    User::RequestComplete(status,KErrNotFound );	    
	    return;
	    }
    COMASuplSession* OMASession =  static_cast<COMASuplSession*>(aSuplSession);
	OMASession->NotifyTriggerFiredL(aStatus, aFireInfo);
	}

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager2::InitilizationCompletedL
// Informs to client about Initilization of POS ,thro' active object
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::InitilizationCompletedL(TInt /*aError*/)
	{

	}
	
// -----------------------------------------------------------------------------
// COMASUPLProtocolManager::DeInitilizationCompletedL
// Informs to client about Initilization of POS ,thro' active object
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::DeInitilizationCompletedL(TInt /*aError*/)
	{

	}		

// -----------------------------------------------------------------------------
// COMASUPLProtocolManager1::SetCommoniCommonConfig
// 
// 
// -----------------------------------------------------------------------------
//
void COMASUPLProtocolManager2::SetCommonConfig(COMASuplCommonConfig*& aCommmonConfig)
    {
    iSuplSettings = aCommmonConfig->iSuplSettings;
    iPosHandler = aCommmonConfig->iPosHandler;
    iIMSI = aCommmonConfig->iIMSI;
    iUT1_StartTimer = aCommmonConfig->iUT1_StartTimer;
    iUT2_PosInitTimer = aCommmonConfig->iUT2_PosInitTimer;
    iUT3_PosTimer = aCommmonConfig->iUT3_PosTimer;
    iPrivacyTimer = aCommmonConfig->iPrivacyTimer;
    iSETMode = aCommmonConfig->iSETMode;
    iSuplUsage = aCommmonConfig->iSuplUsage;
    iSuplInitTimeOut = aCommmonConfig->iSuplInitTimeOut;
    iPersistFailTimer = aCommmonConfig->iPersistFailTimer;
    }
 
 // -----------------------------------------------------------------------------
// COMASUPLProtocolManager1::HandleSuplSessionChangeL
// 
// 
// -----------------------------------------------------------------------------
//   
void COMASUPLProtocolManager2::HandleSuplSessionChangeL(MSuplSessionObserver::TSuplSessionEventType aEvent,TInt64 aSessionId)
	{
		 					TBuf<64> buf(_L("Received Notification : "));                
    					buf.AppendNum(aEvent);
    					iTrace->Trace(buf,KTraceFileName, __LINE__); 
    					
						if(aEvent != MSuplSessionObserver::ESuplSessionDBDeleteEvent)
								return;
								
					    buf.Copy(_L("Received Notification to cancel trigger id : "));                
    					buf.AppendNum(aSessionId);
    					iTrace->Trace(buf,KTraceFileName, __LINE__);        
			
					
					TInt TotalSession = iSuplSessions.Count();
					for(TInt cnt = 0; cnt < TotalSession; ++cnt)
							{
									COMASuplSession* OMAsuplSession = static_cast<COMASuplSession*>(iSuplSessions[cnt]);
									if(OMAsuplSession->AreYouHandlingThisSession(aSessionId))
									{
											buf.Copy(_L("Session Found to cancel trigger id : "));                
											buf.AppendNum(aSessionId);
											iTrace->Trace(buf,KTraceFileName, __LINE__);      
											OMAsuplSession->CancelTriggerSession();		
											break;
									}
							}
	}    
	
// end of file
