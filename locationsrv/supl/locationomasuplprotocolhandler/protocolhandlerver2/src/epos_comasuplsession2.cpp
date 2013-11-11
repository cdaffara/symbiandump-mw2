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
* Description:   Implementation of Session Object in the OMA SUPL Protocol Handler
*
*/


 


#include <barsc2.h>
#include <barsread2.h>
#include <bautils.h>
#include <networking/dnd_err.h>

#include <e32def.h>
#include <etelmm.h> 
#include <centralrepository.h>
#include <CoreApplicationUIsSDKCRKeys.h> // for network mode (offline)


#include <lbspositioninfo.h>
#include <lbsfieldids.h>

#include "epos_suplterminal.h"
#include "epos_suplterminalconstants.h"
#include "epos_suplterminalqop.h"

#include "epos_comasuplnotification.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_csuplconnection.h"
#include "lbs/epos_comasuplpossessionbase.h"
#include "lbs/epos_comasuplposhandlerbase.h"
#include "lbs/epos_comasuplinforequestlist.h"
#include "epos_comasupllocationid.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplresponse.h"
#include "epos_comasuplpos.h"
#include "epos_comasuplstart.h"
#include "epos_comasupltriggeredstart.h"
#include "epos_comasuplposinit.h"
#include "epos_comasuplauthresponse.h"
#include "epos_comasupltriggeredresponse.h"
#include "epos_comasupltriggeredstop.h"
#include "epos_comasuplauthrequest.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_comasuplvelocity.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "lbs/epos_eomasuplposerrors.h"
#include "epos_comasuplinit.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"

#include "epos_comasuplprotocolmanager2.h"
#include "epos_comasuplsettings.h"
#include "epos_comasupletelnotifier.h"
#include "epos_comasuplsession2.h"

#include "epos_comasuplconnrequestor.h"
#include "epos_comasuplstate.h"
#include "epos_comasuplstartstate.h"
#include "epos_comasupltriggerstartstate.h"
#include "epos_comasuplposinitstate.h"
#include "epos_comasuplresponsestate.h"
#include "epos_comasupltriggerresponsestate.h"
#include "epos_comasuplendstate.h"
#include "epos_comasuplreportstate.h"
#include "epos_comasupltriggerstopstate.h"
#include "epos_comasuplposstate.h"
#include "epos_comasuplinitstate.h"
#include "epos_comasuplposrequestor.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"
#include "epos_resourceutils.h"
#include "epos_comasupltimeouttimer.h"
#include "epos_comasupldialogtimer.h"
#include "epos_tomasuplposmethod.h"
#include "epos_omasuplconfigurationkeys.h"
#include "epos_csuplsettingparams.h"
#include "epos_comasuplasnbase.h"
#include "epos_csuplsettingsconstants.h"


_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplSession2.cpp");

//Multiplying factor for conversion of ellipsoid to circle
//formula used sqrt(-2*natural log(1-Confidence))
// actual value = 1.0107676525947896431381113653917 

//const TReal	MultiplyFactorOneSigma = 1.01076765; 
const TInt KMaxIntervalTime = 10;
const TInt KMaxStartTime = 10;

// ============================ MEMBER FUNCTIONS ===============================

//Constructor
COMASuplSession::COMASuplSession(RMobilePhone& aMobilePhone ,
																TOMASuplReqType aRequestType,
																COMASuplPosHandlerBase *aPosHandler,
																MOMASuplSessionObserver& aSessionObserver,
																COMASuplSettings*& aSettings,
																COMASUPLProtocolManager2 &aProtoMgr,
																TInt aIpcSessionId,
																COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
																
																iMobilePhone(aMobilePhone),
																iRequestType(aRequestType),
																iPosHandler(aPosHandler),	
																iHSLPAddress(aSettings->SLPAddress()),
																iEncodedSuplInit(NULL),
																iSessionObserver(aSessionObserver),
																iSuplSettings(aSettings),
																iProtocolManager(aProtoMgr),
																iUIFlag(EFalse),
																iSessionIDFlag(EFalse),
																iRoaming(EFalse),
																iChanged(EFalse),
																iIpcSessionId(aIpcSessionId),
																iNwInitError(EFalse),
																iPersistFail(EFalse),
																iCapsFail(EFalse),
																iUsageDialog(EFalse),
																iUsageHomeNW(EFalse),
																iEtelNotify(EFalse),
																iEtelRoamingCheck(EFalse),
																iIapDialogShown(EFalse),
																iIapDlgTimerExpired(EFalse),
                                                                iTriggerSession(EFalse),
																iOMASuplAsnHandlerBaseImpl(aOMASuplAsnHandlerBase),
                                                                iTrgTimer(EFalse),
                                                                iStartTimerFlag(EFalse),
                                                                iTrgCancelRequest(EFalse),
                                                                iClientNotify(EFalse),
                                                                iDisconnected(EFalse)
																
    { 
    }
 
//2 nd Phase construction
 void COMASuplSession::ConstructL(CSuplCommunicationManager &aCommManager,
			                      CSuplSettingsInternal* aSettingsStorage,
 								  TDes& aIMSI)
     {
     iTrace = COMASuplTrace::NewL();	

     iTrace->Trace(_L("COMASuplSession::ConstructL"), KTraceFileName, __LINE__); 


     iConnRequestor = COMASuplConnRequestor::NewL(aCommManager,iProtocolManager,KOMASuplHSLPPort,*this);
     TBuf<64> msg;
     msg.Copy(_L("Port Number is :"));
     msg.AppendNum(KOMASuplHSLPPort);
     iTrace->Trace(msg, KTraceFileName, __LINE__); 


     iSuplStorageSettings = aSettingsStorage;

     iSuplSessionId = COMASuplSessionID::NewL();
     iCompleteSelfRequestor = COMASuplCompleteSelfRequestor::NewL(*this);

     iSETCapabilities = COMASuplSETCapabilities::NewL();

     //All caps are on...for NET initiated case
     iAllowedCapabilities = KGpsSETAssisted | KGpsSETBased | KAutonomousGps| KAFLT | KECID | KEOTD | KOTDOA | KCID; 

     //Ownership transfer to iSuplSessionId
     COMASuplSETSessionID* SETSessionId  = COMASuplSETSessionID::NewL();

     iSuplSessionId->SetSETSessionID(SETSessionId);

     //Ownership transfer to iSuplSessionId
     iSuplSessionId->SetSLPSessionID(NULL); 	

     iSuplVersion.SetSuplVersion(KSuplMajorVersion,KSuplMinorVersion,KSuplRevision);

     if( aIMSI.Length() )
         {
         User::LeaveIfError(iIMSI.Create( aIMSI ));    
         }

     if(iPosHandler)
         {
         iTrace->Trace(_L("Creating POSSession..."), KTraceFileName, __LINE__); 
         iPOSSession = iPosHandler->CreateNewSessionL(this);
         //Don't move this statement else where
         iOMASuplPOSRequestor = COMASuplPOSRequestor::NewL(this,iPOSSession);
         }
     else
         {
         iTrace->Trace(_L("No POS plugin exists..."), KTraceFileName, __LINE__); 				
         }


     iTimer = COMASuplTimeoutTimer::NewL(*this);


     iIapNotifier = COMASuplIapNotifier::NewL(*this);


     iDialogTimer = COMASuplDialogTimer::NewL(*this);

     iIsQoPPresent = EFalse;	
     iIsFirstPOSMessage = ETrue;
     isTimeoutDialogTimerStarted = EFalse;

     iDiffTime = 0;
     iSessionCompletionCode = KErrCompletion;
	 iClientName.CreateL(256);
        iRequestorName.CreateL(256);
        iRequestorName.Zero();
        iNetworkPrivacy 	= CPosNetworkPrivacy::NewL();
        iRequestorId = 0;
     iTrace->Trace(_L("End of COMASuplSession::ConstructL"), KTraceFileName, __LINE__); 
     }

//
// -----------------------------------------------------------------------------
// COMASuplSession::NewL
// Creates new instance of COMASuplSession 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 COMASuplSession* COMASuplSession::NewL(CSuplCommunicationManager& aCommManager,
																				RMobilePhone& aMobilePhone,
																				COMASuplSettings*& aSettings,
																				CSuplSettingsInternal* aSettingsStorage,
																				TOMASuplReqType  aRequestType,
																				MOMASuplSessionObserver& aSessionObserver,
																				COMASuplPosHandlerBase *aPosHandler,
																				TDes& aIMSI,
																				COMASUPLProtocolManager2 &aProtoMgr,
																				TInt aIpcSessionId,
																				COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase)
     { 	
     COMASuplSession* self = new( ELeave ) 
     COMASuplSession( aMobilePhone,aRequestType, aPosHandler, 
             aSessionObserver, aSettings, aProtoMgr, aIpcSessionId,aOMASuplAsnHandlerBase);
     CleanupStack::PushL( self );
     self->ConstructL( aCommManager, aSettingsStorage, aIMSI );
     CleanupStack::Pop(self);
     return self;
     }
    
// -----------------------------------------------------------------------------
// COMASuplSession::DestroySession
// This method will delete this session..since only protocol manager will delete session object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::DestroySession()
     {
     iTrace->Trace(_L("COMASuplSession::DestroySession"), KTraceFileName, __LINE__); 

     //This is for sending SUPL_END if session is destroed while RunSession is going on.
     if(iRequestType == ESUPL_TERMINAL && iSuplSessionState!= ESUPL_TERMINATED)
         {
         CancelRunSession();
         SessionEnd();
         CompleteSession(KErrCancel);
         }

     delete this;
     }

// -----------------------------------------------------------------------------
// COMASuplSession::PosPlugInUnloaded
// Handles Pos handler unload
// -----------------------------------------------------------------------------
void COMASuplSession::PosPlugInUnloaded() 
    {
    iTrace->Trace(_L("COMASuplSession::PosPlugInUnloaded"), KTraceFileName, __LINE__); 															

    // Cancel the Session
    CancelSession();

    // Delete the POS Requestor
    delete iOMASuplPOSRequestor;
    iOMASuplPOSRequestor = NULL;

    // Delete the POS Session
    delete iPOSSession;
    iPOSSession = NULL;

    if(iSuplSessionState == ESUPL_SEND)
        {
        CompleteSession(KErrCompletion);
        }
    else
        {
        //Close Connection : bug Fix.
        iConnRequestor->CloseConnection();
        iPortNum = 0;
        iSessionObserver.TerminateSession(this, KErrCompletion);	
        }
    }

	
//Desrtuctor    
 COMASuplSession::~COMASuplSession()
     {
     if(iTrace)
         iTrace->Trace(_L("Start ~COMASuplSession::COMASuplSession"), KTraceFileName, __LINE__); 

     CleanUp();

     delete iEncodedBuffer;
     delete iSuplState;
     delete iOMASuplPOSRequestor;
     delete iConnRequestor;
     delete iSuplSessionId; 
     delete iCompleteSelfRequestor;
     delete iDecodedAsnMessage;
     delete iSETCapabilities;
     delete iPosition;
     delete iEncodedSuplInit;
     delete iPOSSession;
     iPOSSession = NULL;
     delete iTimer;
     delete iIapNotifier;
     iIMSI.Close();
     delete iDialogTimer;
     iClientName.Close();
        iRequestorName.Close();
        delete iSuplReqInfo;
        delete iNetworkPrivacy;

     if(iTrace)
         iTrace->Trace(_L("End of ~COMASuplSession::COMASuplSession"), KTraceFileName, __LINE__); 

     delete iTrace;
     }

// -----------------------------------------------------------------------------
// COMASuplSession::Initialize
// This method initializes the OMA SUPL Session Object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::InitializeL(TInt aRequestID)
     {
     iSuplSessionState = ESUPL_INITIALIZED;
     iRequestID = aRequestID;
     if(iPOSSession)
         {
         iTrace->Trace(_L("COMASuplSession::InitializeL of POS"), KTraceFileName, __LINE__); 
         iOMASuplPOSRequestor->SetObserver(this);
         TInt err = iOMASuplPOSRequestor->InitilizePOSSessionL(aRequestID);
         if(err != KErrNone)
             {
             iTrace->Trace(_L("Initilization Failed ..."), KTraceFileName, __LINE__); 
             CompleteSession(KErrNotReady);
             }
         }
     else
         {
         iTrace->Trace(_L("InitializeL, calling OperationCompleteL"), KTraceFileName, __LINE__); 
         OperationCompleteL(KErrNone);	
         }
     }

// -----------------------------------------------------------------------------
// COMASuplSession::CleanUp
// Perfomrs clean up operations on the OMA SUPL Session Object
// This releases any resource held by the object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::CleanUp()
     {
    	if(iTrace)
     iTrace->Trace(_L("COMASuplSession::CleanUp"), KTraceFileName, __LINE__); 

     if(iSuplState)
         {
         if(iSuplSessionState != ESUPL_SESSION_ERROR)
             {
             iSuplState->CancelOperation();
             }
         }

     iConnRequestor->Cancel();

     delete iEncodedBuffer;
     iEncodedBuffer = NULL;
     delete iDecodedAsnMessage; 
     iDecodedAsnMessage = NULL;
	 RemoveActiveSession();
		if(iTrace)
     iTrace->Trace(_L("End of COMASuplSession::CleanUp"), KTraceFileName, __LINE__); 
     }


// -----------------------------------------------------------------------------
// COMASuplSession::RequestType
// Returns the type of request associated with the session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSession::TOMASuplReqType COMASuplSession::RequestType()
    {
    return iRequestType; //SET OR NET
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SessionEnd
// Cleans up the session and completes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::SessionEnd()
     {	
     iSuplSessionState = ESUPL_TERMINATED;

     // Invoke SessionEnd on the POS Session
     iTrace->Trace(_L("COMASuplSession::SessionEnd"), KTraceFileName, __LINE__); 
     // CleanUp
     CleanUp();
     if(iPOSSession)
         {
         iPOSSession->SessionEnd();
         iTrace->Trace(_L("POS Session...COMASuplSession::SessionEnd."), KTraceFileName, __LINE__); 
         }
     }   
// -----------------------------------------------------------------------------
// COMASuplSession::RunSuplSession
// Starts the SUPL Sesssion for Terminal Initiated Location Request
// The call to this method will result in the generation of SUPL START 
// and sending of the SUPL START packet to the SLP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::RunSuplSessionL(TRequestStatus& aStatus, TBool aFirstReq, const TDesC& aHslpAddress, 
                                       TBool aFallBack, TInt aAllowedCapabilities,
 									  TInt aSessionIdSeed,TInt aRequestID)
     {
     iRunRequestStatus = & aStatus;
     *iRunRequestStatus = KRequestPending;

     iRequestID = aRequestID;

     iTrace->Trace(_L("Start COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__); 

     // Log Session Id
     TBuf<64> id;
     id.Append(_L("Session  Id is "));
     id.AppendNum(aSessionIdSeed);
     iTrace->Trace(id,KTraceFileName, __LINE__); 

     // Log Capability from client
     id.Copy(_L("Received Capability is "));
     id.AppendNum(aAllowedCapabilities);
     iTrace->Trace(id,KTraceFileName, __LINE__); 

     iSETSessionUniqueId = aSessionIdSeed;


     TInt networkMode = 1;
     networkMode = GetNetworkModeL();

     if ( networkMode == ECoreAppUIsNetworkConnectionNotAllowed )
         {
         id.Copy(_L("The device is in OFFLINE mode."));
         iTrace->Trace(id,KTraceFileName, __LINE__); 
         iSessionObserver.TerminateSession(this, KErrGeneral);
         return;
         }
     else
         {
         id.Copy(_L("The device is in ON LINE mode."));
         iTrace->Trace(id,KTraceFileName, __LINE__); 
         }

     if(aAllowedCapabilities == 0)
         {
         iAllowedCapabilities = KGpsSETAssisted | KGpsSETBased | KAutonomousGps| KAFLT | KECID | KEOTD | KOTDOA | KCID; 
         }
     else
         {
         iAllowedCapabilities = aAllowedCapabilities;
         }

     //Ownership transfer to iSuplSessionId
     iSuplSessionId->SetSLPSessionID(NULL); 	

     iConnRequestor->SetDefaultParametersL(aHslpAddress,aFallBack);

     if (aFirstReq)
         CheckForSuplUsageL();
     else
         CheckForPreviousResultL();        



// Clear  Position....
     delete iPosition;
     iPosition = NULL;


     iTrace->Trace(_L("End of COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__); 
     }

// -----------------------------------------------------------------------------
// COMASuplSession::RunSuplSession
// Starts the SUPL Sesssion for Terminal Initiated Location Request, with QoP
// The call to this method will result in the generation of SUPL START 
// and sending of the SUPL START packet to the SLP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
  void COMASuplSession::RunSuplSessionL(TRequestStatus& aStatus, TBool aFirstReq,
                                        const TDesC& aHslpAddress, TBool aFallBack,
  										TInt aAllowedCapabilities,TInt aSessionIdSeed,
  										TSuplTerminalQop& aQop, TInt aRequestID)
      {


      iClientQop = aQop;

      TInt delay;
      if(iClientQop.GetDelay(delay) != KErrNotFound)
          {
          iSessionStartTime.HomeTime();
          }

      iIsQoPPresent = ETrue;	
      RunSuplSessionL(aStatus,aFirstReq, aHslpAddress, aFallBack, aAllowedCapabilities,aSessionIdSeed,aRequestID);
      }

// -----------------------------------------------------------------------------
// CSuplSession::RunInvalidSessionL
// Starts the SUPL Sesssion for Invalid Session ID 
// Completes Self so that protocol Manager can process other stuff 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void COMASuplSession::RunInvalidSessionL(COMASuplAsnMessageBase* aDecodedMsg)
    {

    // Check if the message type is SUPL END
    COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedMsg->MessageType();

    if(messageType == COMASuplAsnMessageBase::ESUPL_END)
        {
        return;
        }

    COMASuplSessionID* retrivedSessionID = NULL;
    aDecodedMsg->SessionId(retrivedSessionID);

    // Set the SessionId.
    UpdateSLPSessionIDL(retrivedSessionID);
    UpdateSETSessionIDL(retrivedSessionID);

    //Set default slp to Conn Requestor...		
    iConnRequestor->UseDefaultServerL();

    // Complete Self
    iCompleteSelfRequestor->CompleteInvalidSession();

    }

// -----------------------------------------------------------------------------
// CSuplSession::StartInvalidSessionL
// Starts the SUPL Sesssion for Invalid Session ID 
// Generate SUPL End with error state set ti Invalid Session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void COMASuplSession::StartInvalidSessionL()
    {
    iTrace->Trace(_L("Start COMASuplSession::StartInvalidSessionL"), KTraceFileName, __LINE__); 
    iErrorStatusCode = COMASuplEnd::EInvalidSessionId;
    GenerateSuplEndL();
    }

// -----------------------------------------------------------------------------
// CSuplSession::GenerateSuplMessage
// This method generates an appropriate SUPL Message based on the state
// of the SUPL object
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::GenerateSuplMessageL()
     {
     switch(iSuplSessionState)
         {
         //case ESUPL_INITIALIZED:
         case ESUPL_CONNECTING:
             {
             if(iRequestType == ESUPL_TERMINAL )
                 {
                 if (iTriggerSession)
                     {   
                     if (iTrgCancelRequest)                                    
                         {                                    
                         iTrgCancelRequest = EFalse;                                        
                         iErrorStatusCode = COMASuplEnd::EVer2SessionStopped;
                         GenerateSuplTriggerStopL();	
                         }
                     else if (iSuplMsgType == ESUPL_TRIG_RESPONSE || iSuplMsgType == ESUPL_REPORT)
                         {                                    
                         GenerateSuplPosInitL();
                         }
                     else
                         {                                    
                         GenerateSuplTriggerStartL();
                         }
                     }
                 else                        
                     GenerateSuplStartL();	
                 }
             if(iRequestType == ESUPL_NETWORK )
                 {
                 if (iTriggerSession)
                     {                                
                     if (iTrgCancelRequest)                                    
                         {                                    
                         iTrgCancelRequest = EFalse;                                        
                         iErrorStatusCode = COMASuplEnd::EVer2SessionStopped;
                         GenerateSuplTriggerStopL();	
                         }
                     else if (iSuplMsgType == ESUPL_TRIG_RESPONSE || iSuplMsgType == ESUPL_REPORT)
                         {                                    
                         GenerateSuplPosInitL();	
                         }
                     else                                    
                         {                                    
                         GenerateSuplTriggerStartL();
                         }
                     }
                 else                        
                     GenerateSuplPosInitL();	
                 }
             if(iRequestType == ESUPL_INVALID_SESSION )
                 {
                 GenerateSuplEndL();	
                 }

             break;
             }
         case ESUPL_GENERATE:
             {
             switch(iSuplMsgType)
                 {
                 case ESUPL_RESPONSE:
                 case ESUPL_TRIG_RESPONSE:
                 case ESUPL_REPORT:
                 case ESUPL_INIT:
                     GenerateSuplPosInitL();
                     break;
                 case ESUPL_END:
                     GenerateSuplEndL();
                     break;
                 case ESUPL_POS:
                     GenerateSuplPosL();	
                     break;
							case ESUPL_TRIG_STOP:
								 GenerateSuplTriggerStopL();	
								 break;
                 default:
                     // Error
                     break;
                 }
             break;		
             }
         }
     }

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplStart
// Generate SUPL START from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void COMASuplSession::GenerateSuplStartL()			
    {
    iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplStartL"), KTraceFileName, __LINE__);

    // Free the existing state if any
    delete iSuplState;
    iSuplState = NULL;

    iSuplState = COMASuplStartState::NewL(iMobilePhone, iUT1_StartTimer,iOMASuplPOSRequestor,iOMASuplAsnHandlerBaseImpl,iAllowedCapabilities);
    iSuplState->SetMsgStateObserver(this);
    if(iIsQoPPresent)
        {
        COMASuplStartState* startSuplState =  static_cast <COMASuplPosInitState *>(iSuplState);
        startSuplState->SetQop(iClientQop);
        }

    TInt err = iSuplState->GenerateMessageL();
    iSuplMsgType = ESUPL_START;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    }  

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplTriggerStart
// Generate SUPL TRIGGER START from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void COMASuplSession::GenerateSuplTriggerStartL()			
    {
    iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplTriggerStartL"), KTraceFileName, __LINE__);

    // Free the existing state if any
    delete iSuplState;
    iSuplState = NULL;

    iSuplState = COMASuplTriggerStartState::NewL(iMobilePhone, iUT1_StartTimer,iOMASuplPOSRequestor,iOMASuplAsnHandlerBaseImpl,iAllowedCapabilities, iTriggerParams, iRequestType);
    iSuplState->SetMsgStateObserver(this);
    if(iIsQoPPresent)
        {
        COMASuplTriggerStartState* startSuplState =  static_cast <COMASuplTriggerStartState *>(iSuplState);
        startSuplState->SetQop(iClientQop);
        }

    TInt err = iSuplState->GenerateMessageL();
    iSuplMsgType = ESUPL_TRIG_START;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    }  
	 

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplPosInitL
// Generate SUPL POS INIT from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplPosInitL()			
    {
    iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplPosInitL"), KTraceFileName, __LINE__);

    // Free the existing state if any
    iTrace->Trace(_L("Deleting SUPL State"), KTraceFileName, __LINE__);
    delete iSuplState;
    iSuplState = NULL;

    iTrace->Trace(_L("Setting SUPL State to NULL - successful "), KTraceFileName, __LINE__);
    if(iRequestType == ESUPL_NETWORK)
        {
        iTrace->Trace(_L("iRequestType == ESUPL_NETWORK is true"), KTraceFileName, __LINE__);
        TBuf<256> serverAddress;
        HBufC8 *hslpAdress = HBufC8::NewL(256);
        iConnRequestor->GetUsedServerAddress(serverAddress);
        hslpAdress->Des().Copy(serverAddress);
        CleanupStack::PushL(hslpAdress);
        if (iTriggerSession)
            {                    
            iSuplState = COMASuplPosInitState::NewL(iSETCapabilities, iMobilePhone, iUT2_PosInitTimer,
                    iOMASuplPOSRequestor,iAllowedCapabilities, iRequestType, iPosMethod, iOMASuplAsnHandlerBaseImpl);
            }
        else
            {                    
            iSuplState = COMASuplPosInitState::NewL(iSETCapabilities, iMobilePhone, iUT2_PosInitTimer,
                    iOMASuplPOSRequestor,iAllowedCapabilities, iRequestType, iPosMethod, iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit, hslpAdress);
            }

        if(hslpAdress)
            {
            CleanupStack::PopAndDestroy(hslpAdress);
            }
        }
    else
        {
        TBuf<128> msg(_L("COMASuplSession::GenerateSuplPosInitL() without last 2 params : "));
        msg.AppendNum(iPosMethod);
        iTrace->Trace(msg, KTraceFileName, __LINE__);
        iSuplState = COMASuplPosInitState::NewL(iSETCapabilities, iMobilePhone, iUT2_PosInitTimer,
                iOMASuplPOSRequestor,iAllowedCapabilities,iRequestType, iPosMethod,iOMASuplAsnHandlerBaseImpl);

        }
    iSuplState->SetMsgStateObserver(this);

    //If trigger session, set flag
    if (iTriggerSession)
        {                    
        COMASuplPosInitState* state =  static_cast <COMASuplPosInitState *>(iSuplState);
        state->SetTriggerSessionFlag(ETrue);
        }                    

    iSuplState->GenerateMessageL();

    iSuplMsgType = ESUPL_POSINIT;

    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    }   

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplPosL
// Generate SUPL POS from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplPosL()			
    {
    iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplPosL"), KTraceFileName, __LINE__);

    if(COMASuplState::ESUPL_POS != iSuplState->GetSuplState()) // Check if existing is of POS type.
        {
        COMASuplPosPayload* remainingPosPayload = GetRemainingPosPayloadL();
        delete iSuplState;
        iSuplState = NULL;
        iSuplState = COMASuplPosState::NewL(iPOSSession,iUT3_PosTimer,iSETCapabilities,iOMASuplAsnHandlerBaseImpl);					

        COMASuplPosState* state =  static_cast <COMASuplPosState *>(iSuplState);
        state->SetRemainingPosPayloadFromSuplPosInitL(remainingPosPayload);//ownership transfered to POS state class
        }

    iSuplState->SetMsgStateObserver(this);
    iSuplMsgType = ESUPL_POS;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    COMASuplPosState* posState =  static_cast <COMASuplPosState*>(iSuplState);
    if(!(posState->IsGenerationInProgress()))
        {
        iSuplSessionState = ESUPL_ENCODE;
        TInt err = iSuplState->GenerateMessageL();
        if(err != KErrNone) 
            {
            iTrace->Trace(_L("COMASuplSession::GenerateSuplPosL Failed..."), KTraceFileName, __LINE__); 
            OperationCompleteL(err);//Error handling.
            }
        }

    }   

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplEndL
// Generate SUPL END from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplEndL()			
    {
    iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplEnd..."), KTraceFileName, __LINE__);

    // Free the existing state if any
    delete iSuplState;
    iSuplState = NULL;

    if(iRequestType == ESUPL_INVALID_SESSION )
        {
        iSuplSessionState = ESUPL_INITIALIZED;
        }
    else 							
        {
        iSuplSessionState = ESUPL_ENCODE;	//Normal End
        }

    iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
    iSuplState->SetMsgStateObserver(this);
    iSuplMsgType = ESUPL_END;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    iSuplState->GenerateMessageL();
    }   

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateTriggerStopL
// Generate SUPL END from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplTriggerStopL()			
    {
    iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplTriggerStopL..."), KTraceFileName, __LINE__);

    // Free the existing state if any
    delete iSuplState;
    iSuplState = NULL;

    iSuplSessionState = ESUPL_ENCODE;	//Normal End

    iSuplState = COMASuplTriggerStopState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
    iSuplState->SetMsgStateObserver(this);
    iSuplMsgType = ESUPL_TRIG_STOP;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    iSuplState->GenerateMessageL();
    }   

// -----------------------------------------------------------------------------
//COMASuplSession::CompleteSession
//Informs to client about completion of location request, thro' active object
 void COMASuplSession::CompleteSession(TInt aCompletionCode)
     {	
     iTrace->Trace(_L("COMASuplSession::CompleteSession start"), KTraceFileName, __LINE__);
     if(iRunRequestStatus)
         {

         TTime currentTime;
         currentTime.HomeTime();
         iSessionCompletionCode = aCompletionCode;

         TBuf<64> errorCode;
         errorCode.Append(_L("Completing Session with error code : "));
         errorCode.AppendNum(aCompletionCode);
         iTrace->Trace(errorCode, KTraceFileName, __LINE__); 

         //Cell filtering is only for non-trigger requests    
         if (!iTriggerSession)    
         	iConnRequestor->UpdateNetInfo(currentTime,iCurrentCellId.iMCC,iCurrentCellId.iMNC,iCurrentCellId.iCid,iCurrentCellId.iLac,iNetworkType,aCompletionCode);

         User::RequestComplete(iRunRequestStatus,aCompletionCode);
         }
     }

// -----------------------------------------------------------------------------
// COMASuplSession::OperationComplete
// Gets control when any of the asynchronous requests are completed
// This is a very important method and drives the OMS SUPL
// State Machine
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::OperationCompleteL(TInt aErrorCode)
    {
    iTrace->Trace(_L("COMASuplSession::OperationCompleteL"), KTraceFileName, __LINE__);
    TBuf<128> msg;

    msg.Copy(_L("State is "));
    msg.AppendNum(iSuplSessionState);

    iTrace->Trace(msg, KTraceFileName, __LINE__);

    if(KErrNone != aErrorCode)
        {
        msg.Copy(_L("Actual Error Code : "));
        msg.AppendNum(aErrorCode);
        iTrace->Trace(msg, KTraceFileName, __LINE__); 

        if (iSuplSessionState == ESUPL_CONNECTING)
            CheckForPersistentFailure(aErrorCode);                    
        HandleSuplErrorL(aErrorCode);
        return;
        }

    TInt err = KErrNone;

    switch(iSuplSessionState)
        {
        case ESUPL_INITIALIZED:
            {
            if (iRequestType == ESUPL_NETWORK) 
                {
                iIapDialogShown = ETrue;                                                
                TInt delay ;
                iTrace->Trace(_L("Dialog timer not started earlier and request is NI, starting now"), KTraceFileName, __LINE__);
                if (iDiffTime.Int() == 0)
                    {                                                
                    COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                    TOMASuplQop qop;
                    TInt retVal = suplInit->Qop(qop);

                    qop.Delay(delay);
                    //if delay is > 7, it is not valid
                    if (delay > 7)
                        delay = 0;                                
                    if(retVal == KErrNone && delay > 0)
                        {
                        TReal delayReal;
                        Math::Pow(delayReal, 2, (TReal)delay);
                        delay = (TInt) delayReal;
                        msg.Copy(_L("Delay present in message, value is = "));
                        msg.AppendNum(delay);
                        iTrace->Trace(msg, KTraceFileName, __LINE__); 
                        isTimeoutDialogTimerStarted = ETrue;
                        }
                    else
                        {
                        if (iSuplInitTimeOut > 0)
                            {                                                        
                            iTrace->Trace(_L("Delay not present in message"), KTraceFileName, __LINE__);
                            msg.Copy(_L("Delay value in CR is"));
                            msg.AppendNum(iSuplInitTimeOut);
                            iTrace->Trace(msg, KTraceFileName, __LINE__);
                            isTimeoutDialogTimerStarted = ETrue;
                            delay = iSuplInitTimeOut * KSecond;
                            }
                        else
                            {
                            iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                            }    
                        }
                    }
                else
                    {
                    msg.Copy(_L("Time diff used, diff = "));
                    msg.AppendNum(iDiffTime.Int());
                    iTrace->Trace(msg, KTraceFileName, __LINE__);
                    isTimeoutDialogTimerStarted = ETrue;	
                    delay = iDiffTime.Int();
                    }                                                
                    if (iSuplMsgType == ESUPL_TRIG_RESPONSE || iSuplMsgType == ESUPL_REPORT || iDisconnected)
                    {
                    iTrace->Trace(_L("Calling CreateConnectionL() for trigger"), KTraceFileName, __LINE__);
                    TRAP( err, iConnRequestor->CreateConnectionL(delay, ETrue) );
                    }
                else                
                    {
                    iTrace->Trace(_L("Calling CreateConnectionL()"), KTraceFileName, __LINE__);
                    TRAP( err, iConnRequestor->CreateConnectionL(delay, EFalse) );
                    }
                }
            else //terminal initiated case
                {
                if (iSuplMsgType == ESUPL_TRIG_RESPONSE || iSuplMsgType == ESUPL_REPORT || iDisconnected)
                    {
                    iTrace->Trace(_L("Calling CreateConnectionL() for trigger"), KTraceFileName, __LINE__);
                    iDisconnected = EFalse;
                    TRAP( err, iConnRequestor->CreateConnectionL(ETrue) );
                    }                                                
                else
                    {
                    iTrace->Trace(_L("Calling CreateConnectionL()"), KTraceFileName, __LINE__);
                    TRAP( err, iConnRequestor->CreateConnectionL(EFalse) );
                    }                                                
                }
            if(KErrNone != err)
                {
                HandleSuplErrorL(err);
                break;
                }
            iSuplSessionState = ESUPL_CONNECTING;
            break;
            }

        case ESUPL_CONNECTING:
        		{
            //Retrieve the portnumber being used
            iPortNum = iConnRequestor->GetPortNumber();

            if(iRequestType == ESUPL_INVALID_SESSION) 
                {
                iTrace->Trace(_L("Handling Invalid Session..."), KTraceFileName, __LINE__); 
                // ::POS::
                delete iEncodedBuffer;
                iEncodedBuffer = NULL;
                iSuplSessionState = ESUPL_ENCODE;
                iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
                if(KErrNone == err)
                    {
                    iConnRequestor->SendPacket(*iEncodedBuffer);
                    iSuplSessionState = ESUPL_SEND;
                    }
                }
            else
                {
                GenerateSuplMessageL();
                if (iSuplMsgType != ESUPL_TRIG_STOP)
				    iSuplSessionState = ESUPL_GENERATE;
                }
            break;
            }

        case ESUPL_SEND:
            {
            // Clean up Encoding buffer before filling it up
            delete iEncodedBuffer;
            iEncodedBuffer = NULL;

            if(IsWholeMessageSentL())
                {
                SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplSent);
                } 

            if(  (iSuplMsgType == ESUPL_START || iSuplMsgType == ESUPL_POSINIT) && iRequestType == ESUPL_TERMINAL) // Manish : Added this to support Cell filtering...
                {
                COMASuplStartState* SuplStartState =  static_cast <COMASuplStartState *>(iSuplState);
                SuplStartState->GetCurrentCellID(iCurrentCellId,iNetworkType);

#ifdef PRINT_MESSAGE
                
                msg.Copy(_L("After receiving from SUPL START/SUPL POS INIT"));
                iTrace->Trace(msg,KTraceFileName, __LINE__); 

                msg.Copy(_L("cid="));
                msg.AppendNum(iCurrentCellId.iCid);
                iTrace->Trace(msg,KTraceFileName, __LINE__); 

                msg.Copy(_L("Lac="));
                msg.AppendNum(iCurrentCellId.iLac);
                iTrace->Trace(msg,KTraceFileName, __LINE__);

                msg.Copy(_L("MCC="));
                msg.AppendNum(iCurrentCellId.iMCC);
                iTrace->Trace(msg,KTraceFileName, __LINE__);

                msg.Copy(_L("MNC="));
                msg.AppendNum(iCurrentCellId.iMNC);
                iTrace->Trace(msg,KTraceFileName, __LINE__); 
#endif

                }

            if(iPOSSession && iSuplMsgType == ESUPL_POSINIT) //Added while doing confirmance testing...
                {
                COMASuplPosInitState* SuplPosInitState =  static_cast <COMASuplPosInitState *>(iSuplState);
                if(!(SuplPosInitState->IsPosPayloadPresent()))
                    {
                    iTrace->Trace(_L("StartTimer for SUPL_POSINIT..."), KTraceFileName, __LINE__); 								
                    iTimer->StartTimer(iUT2_PosInitTimer * KSecond); 	
                    }
                else
                    {
                    iTrace->Trace(_L("Timer for SUPL_POSINIT not started ..since POS payload is present..."), KTraceFileName, __LINE__); 								
                    }
                }
            else if (iSuplMsgType == ESUPL_TRIG_STOP)
                {
                iTrace->Trace(_L("StartTimer for SUPL_TRIGGER_STOP..."), KTraceFileName, __LINE__); 
                iTimer->StartTimer(iUT2_PosInitTimer * KSecond); 	
                }                    
            else
                {
                iSuplState->StartTimer();
                }

            if(iSuplMsgType == ESUPL_END)
                {
                //Close Connection : bug Fix.
                iConnRequestor->CloseConnection();
				iPortNum = 0;
				RemoveActiveSession();
                // Invoke Terminate Session
                if (iTriggerSession && iRequestType == COMASuplSession::ESUPL_TERMINAL)
                    {                            
                    iTrace->Trace(_L("COMASuplSession::OperationCompleted...SUPL_END send for trigger..."), KTraceFileName, __LINE__); 
                    iSessionObserver.TerminateSession(this, KErrNone);
                    }
                else
                    {                            
                    iTrace->Trace(_L("COMASuplSession::OperationCompleted...SUPL_END send for Error/cancellation..."), KTraceFileName, __LINE__); 
                    iSessionObserver.TerminateSession(this, KErrCompletion);
                    }
                break;
                }

            if(	iPOSSession && 
                    (iSuplMsgType == ESUPL_POSINIT || (iSuplMsgType == ESUPL_POS  && (!IsLastPOSMessage() || IsSegmentationDone()) ) ))
                {
                iTrace->Trace(_L("Generating POS ..."), KTraceFileName, __LINE__); 
                iSuplMsgType = ESUPL_POS;
                iSuplSessionState = ESUPL_GENERATE;
                GenerateSuplMessageL();
                }
            break;
            }

        case ESUPL_GENERATE:
            {
            // Set the appropriate state
            if(iRequestType == ESUPL_INVALID_SESSION) 
                {
                iTrace->Trace(_L("Handling Invalid Session..."), KTraceFileName, __LINE__); 
                // ::POS::
                delete iEncodedBuffer;
                iEncodedBuffer = NULL;
                iSuplSessionState = ESUPL_ENCODE;
                iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
                if(KErrNone == err)
                    {
                    iConnRequestor->SendPacket(*iEncodedBuffer);
                    iSuplSessionState = ESUPL_SEND;
                    }
                }
            else
                {
                delete iEncodedBuffer;
                iEncodedBuffer = NULL;
                if(ConnectionRequired())				
                    {
                    iTrace->Trace(_L("ConnectionRequired returned TRUE"), KTraceFileName, __LINE__);
                    err = SetApproriateSETSessionId();
                    iSuplSessionState = ESUPL_ENCODE;
                    CopySETCapabilities();//Copy this before and then & with Allowed Capabilities
                    iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
                    iConnRequestor->SendPacket(*iEncodedBuffer);
                    iSuplSessionState = ESUPL_SEND;
                    iConnRequestor->ListenToMessages();
                    }
                else
                    {
                    iTrace->Trace(_L("ConnectionRequired returned False"), KTraceFileName, __LINE__);
                    GenerateSuplMessageL();
                    iSuplSessionState = ESUPL_ENCODE;
                    }
                }

            if(KErrNone != err )
                {
                HandleSuplErrorL(err);
                }
            break;
            }

        case ESUPL_ENCODE:
            {
            // Clean up Encoding buffer before filling it up
            delete iEncodedBuffer;
            iEncodedBuffer = NULL;

            iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
            if(KErrNone == err)
                {
                iConnRequestor->SendPacket(*iEncodedBuffer);
                iSuplSessionState = ESUPL_SEND;
                }
            else
                {
                HandleSuplErrorL(err);
                }
            break;
            }

        case ESUPL_RECEIVED: break;

        default:
            {                             
            HandleSuplErrorL(KErrOMAUnknownState);
            break;
            }
        }
    }
// -----------------------------------------------------------------------------
// COMASuplSession::ConnectionRequired
// Determines of connection should be created
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplSession::ConnectionRequired()
    {
    iTrace->Trace(_L("COMASuplSession::ConnectionRequired"), KTraceFileName, __LINE__);
    TBuf<128> logBuf;
    logBuf.Copy(_L("Trigger session "));
    if (iTriggerSession)
        logBuf.Append(_L("True"));
    else
        logBuf.Append(_L("False"));
    logBuf.Append(_L("Request type "));
    logBuf.AppendNum(iRequestType);
    logBuf.Append(_L("Message type "));
    logBuf.AppendNum(iSuplMsgType);
    iTrace->Trace(logBuf, KTraceFileName, __LINE__);

        {                
        if (iTriggerSession && iRequestType == ESUPL_TERMINAL && iSuplMsgType == ESUPL_POSINIT)
            return ETrue;
        }

    if(iRequestType == ESUPL_INVALID_SESSION )
        {
        return ETrue;
        }
    if(iRequestType == ESUPL_TERMINAL && (iSuplMsgType == ESUPL_START || iSuplMsgType == ESUPL_TRIG_START))
        {
        return ETrue;
        }
    else
        {
        if(iRequestType == ESUPL_NETWORK)
            {
            if(iSuplMsgType == ESUPL_POSINIT || iSuplMsgType == ESUPL_END || iSuplMsgType == ESUPL_TRIG_START)
                {
                return ETrue;
                }
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::ValidatePosition
// Validates position estimate 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplSession::ValidatePosition(COMASuplPosition *aPosition)
    {
    TOMASuplUtcTime utcTime;
    TOMASuplPositionEstimate pos;

    aPosition->GetPosition(utcTime, pos);        

    TOMASuplPositionEstimate::TOMASuplLatitudeSign  latSign;
    TInt               latitude;
    TInt               longitude;
    pos.GetPositionEstimate(latSign,latitude,longitude);	

    // Set Position Estimate
    if(latitude < KLatitudeMin || latitude > KLatitudeMax)
        {
        return KErrArgument;
        }
    if(longitude < KLongitudeMin || longitude > KLongitudeMax)
        {
        return KErrArgument;
        }

    // Retrieve optional parametrs of Position Estimate
    // Retrieve Uncertainity
    TOMASuplUncertainty uncertainty;
    TInt error = pos.GetUncertainty(uncertainty);
    if(!error)
        {
        TInt uncertSemiMajor;
        TInt uncertSemiMinor;
        TInt orientMajorAxis;
        uncertainty.GetUncertainty(uncertSemiMajor,uncertSemiMinor,orientMajorAxis);
        if(uncertSemiMajor < KUncertSemiMajorMin || uncertSemiMajor > KUncertSemiMajorMax)
            {
            return KErrArgument;
            }
        if (uncertSemiMinor < KUncertSemiMinorMin || uncertSemiMinor > KUncertSemiMinorMax)
            {
            return KErrArgument;
            }
        if (orientMajorAxis < KOrientMajorAxisMin  || orientMajorAxis > KOrientMajorAxisMax)
            {
            return KErrArgument;
            }
        }

    TInt confidence;
    error = pos.GetConfidence(confidence);
    if(!error)
        {
        if(confidence < KConfidenceMin || confidence > KConfidenceMax)
            {
            return KErrArgument;
            }
        }
    // Retrieve AltitudeInfo
    TOMASuplAltitudeInfo altitudeInfo;
    error = pos.GetAltitudeInfo(altitudeInfo);
    if(!error)
        {
        TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altDirection;
        TInt altitude;
        TInt altUncert;
        altitudeInfo.GetAltitudeInfo(altDirection,altitude,altUncert);
        if (altitude < KAltitudeMin  || altitude > KAltitudeMax)
            {
            return KErrArgument;
            }
        if (altUncert < KAltitudeUncertMin || altUncert > KAltitudeUncertMax)
            {
            return KErrArgument;
            }
        }

    return KErrNone;
    }        

// -----------------------------------------------------------------------------
// COMASuplSession::GetPositionComplete
// Handles any errors that may occur during protocol processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GetPositionCompleteL(TInt aError)
    {
    if (!iTriggerSession)
        iConnRequestor->CloseConnection();
		
		iPortNum = 0;

    if (iTriggerSession)
        {    
        TPositionInfo position;        
        GetPosition(position);
        if( iTrgFireInfo )
	        {
	        iTrgFireInfo->Set(position);
	        }
        if (iNumFixes == 0)
            {                    
            iTrace->Trace(_L("GetPositionComplete, last fix generation SUPL END"), KTraceFileName, __LINE__);
            iSuplSessionState = ESUPL_GENERATE;
            iSuplMsgType = ESUPL_END;
            GenerateSuplMessageL();
            }
        else
            {                    
            iTrace->Trace(_L("GetPositionComplete, Completing session"), KTraceFileName, __LINE__);
            CompleteSession(KErrNone);
            }                    
        }                
    else
        {                

        if(aError || iPosition->Status())
            {
            delete iPosition;
            iPosition = NULL;
            // Invoke SessionTerminate
            iTrace->Trace(_L("Could not get position from POS Msg Plugin..."), KTraceFileName, __LINE__);
            iSessionObserver.TerminateSession(this, KErrNone);
            }
        else
            {
            TInt err;
            err = ValidatePosition(iPosition);                
            if (err != KErrNone)
                {
                delete iPosition;
                iPosition = NULL;
                iSessionObserver.TerminateSession(this, KErrCompletion);	
                }
            else                
                iSessionObserver.TerminateSession(this, KErrNone);	
            }	
        }	
    }



// -----------------------------------------------------------------------------
// COMASuplSession::HandleSuplErrorL
// Handles any errors that may occur during protocol processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleSuplErrorL(TInt aErr)	
    {
    // All Possible errors -
    // 1. Packet generation failure
    //    SUPL START - Complete the session with aErr
    //    SUPL POS INIT - Send SUPL END with appropriate Error code
    //    SUPL END - Complete Session with aErr and terminate the session
    //    SUPL POS - Will be handled later
    // 2. Secure Socket Connection failure
    //    Complete the Session with aErr. Terminate the supl session
    // 3. Local IP Address retrieval failure
    //    Complete the session with aErr. Terminate the supl session
    // 4. Packet Encoding error
    //    SUPL START - Complete Session with aErr
    //    SUPL POS INIT - Generate SUPL END and send to SLP
    //    SUPL POS - Will be handled later
    //    SUPL END - Complete session with aErr and terminate the session
    // 5. Packet Send Error
    //    Complete the session with aErr and terminate the session
    // 6. Unknown State Error
    //    SUPL START - Complete Session with invalid state
    //    SUPL RESPONSE - Send SUPL END with appropriate error
    //    SUPL POS INIT - Send SUPL END with appropriate error
    //    SUPL POS - Will be handled later
    //    SUPL END - Complete session with invalid state and terminate the session
    //    SUPL INTI - Will be handled later
    // 7. Message Processing error
    //    SUPL RESPONSE - Send SUPL END with appropriate error code
    //    SUPL POS - Will be handled later
    //    SUPL END - Complete session with aErr and terminate the session
    // 8. Time out Handling
    //    SUPL START - Send SUPL END with appropriate error code
    //    SUPL POS INIT - Send SUPL END with appropriate error code
    //    SUPL END - There should be no timer started in the first place

    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplError);	
    iNumFixes = 0; //Making 0 will stop all further request done by client after errors
	RemoveActiveSession();//Remove session as there is error in session...
    switch(iSuplSessionState) 
        {

        //***************************************************************************************				 		

        case ESUPL_INITIALIZED: //Initilization error
            {
            aErr = KErrNotReady;
            iTrace->Trace(_L("HandleInitilizationError..."), KTraceFileName, __LINE__); 
            HandleInitilizationErrorL(aErr);
            break;	
            }

            //***************************************************************************************				 

        case ESUPL_CONNECTING:  //Error while retriving Local IP address
            {
            aErr = KErrGeneral;
            iTrace->Trace(_L("HandleConnectionError..."), KTraceFileName, __LINE__); 
            HandleConnectionErrorL(aErr);
            break;					
            }

            //***************************************************************************************				 

        case ESUPL_ENCODE:   //Encoding failure error
            {

            iTrace->Trace(_L("HandleEncodingError..."), KTraceFileName, __LINE__); 
            HandleEncodingErrorL(aErr);
            break;
            }

            //***************************************************************************************				 				

        case ESUPL_GENERATE : //Generation error
            {
            aErr = KErrGeneral;
            iTrace->Trace(_L("HandleGenerationError..."), KTraceFileName, __LINE__); 
            HandleGenerationErrorL(aErr);
            break;
            }						

            //***************************************************************************************				 				

        case ESUPL_SEND:	//Timeout & packet send error
            {
            iTrace->Trace(_L("HandleSendError..."), KTraceFileName, __LINE__); 
            HandleSendErrorL(aErr);
            break;
            }

            //***************************************************************************************				 								 

			case ESUPL_RECEIVED:	//Error while getting wrong message from SLP
				{
					iTrace->Trace(_L("HandlePacketsError..."), KTraceFileName, __LINE__); 					
                    if (iSuplMsgType == ESUPL_REPORT)
                        HandleSendErrorL(aErr);
                    else                            
					    HandlePacketsErrorL(aErr);	
					break;		
				}
				
//***************************************************************************************				 								 				

        default:
            {
            TBuf<64> msg(_L("Unknown ERROR occured..."));
            msg.AppendNum(aErr);
            iTrace->Trace(msg, KTraceFileName, __LINE__); 					
            break;	
            } 

            //***************************************************************************************				 								 				
        }

    }
	
// -----------------------------------------------------------------------------
// ReverseString
// Reverse the string
// -----------------------------------------------------------------------------
//
void ReverseString(TDes8& aLocalIPAddress,TInt aLength)
    {
    TBuf8<1> temp1;
    TBuf8<1> temp2;
    for(TInt i =0 ;i < aLength / 2; ++i)
        {
        temp1.Zero();
        temp2.Zero();
        temp1.Append(aLocalIPAddress[i]); 
        temp2.Append(aLocalIPAddress[aLength - 1 - i]); 
        aLocalIPAddress.Replace(i,1,temp2);
        aLocalIPAddress.Replace(aLength - 1 - i,1,temp1);
        }
    }
	
// -----------------------------------------------------------------------------
// COMASuplSession::GetLocalIPAddress
// Gets Local IP address of SET
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplSession::GetLocalIPAddress()	
    {
    iTrace->Trace(_L("COMASuplSession::GetLocalIPAddress"), KTraceFileName, __LINE__); 

    if (iSessionIDFlag)
        {
        return KErrNone;
        }                

    TInetAddr address;
    TInt err = iConnRequestor->GetLocalIPAddress(address);  
    if (err != KErrNone) 
        {
        return err;	
        }

    if ( address.Family() == KAfInet) //This routine will convert IPV4 address in TBuf8<4>
        {
        TUint32 ipAddress = address.Address();
        TPtr8 ptrIPAddress(reinterpret_cast<TUint8*>(&ipAddress), 
                sizeof(ipAddress), sizeof(ipAddress));
        TBuf8<KIP4AddressLength> localIPAddress; 
        localIPAddress.Append(ptrIPAddress);
        COMASuplSETSessionID* suplSETSessionId = NULL;
        iSuplSessionId->SETSessionID(suplSETSessionId);
        ReverseString(localIPAddress,KIP4AddressLength);
        suplSETSessionId->SetSuplSETSessionIDIPv4(iSETSessionUniqueId,
                localIPAddress);
        //#if _DEBUG													  
        TBuf<64> msg;
        TBuf<16> ipaddress;
        address.Output(ipaddress);
        msg.Copy(_L("Its IP4 Address. Local IP Address is "));
        msg.Append(ipaddress);
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        //#endif	        
        }

    if ( address.Family() == KAfInet6 )  // This routine will convert IPV6 address in TBuf8<16>
        {
        TBuf8<KIP6AddressLength> localIP6Address;
        const TIp6Addr &ip6Addr = address.Ip6Address();
        for(TInt i = 0;i < 4; ++i )
            {
            TUint32 ipAddress = ip6Addr.u.iAddr32[i]; 
            TPtr8 ptrIPAddress(reinterpret_cast<TUint8*>(&ipAddress), 
                    sizeof(ipAddress), sizeof(ipAddress));
            TBuf8<KIP4AddressLength> localIPAddress;
            localIPAddress.Append(ptrIPAddress);
            localIP6Address.Append(localIPAddress);
            }

        COMASuplSETSessionID* suplSETSessionId = NULL;
        iSuplSessionId->SETSessionID(suplSETSessionId);
        ReverseString(localIP6Address,KIP6AddressLength);
        suplSETSessionId->SetSuplSETSessionIDIPv6(iSETSessionUniqueId,
                localIP6Address);
        //#if _DEBUG													  
        TBuf<64> msg;
        TBuf<16> ipaddress;
        address.Output(ipaddress);
        msg.Copy(_L("Its IP6 Address. Local IP Address is "));
        msg.Append(ipaddress);
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        //#endif	        	    																	
        }

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// COMASuplSession::SetEncodedSuplInitL
// Sets the Encoded SUPL INIT. This is required
// to compute the VER in SUPL POS INIT
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::SetEncodedSuplInitL(HBufC8 *aEncodedSuplInit)	 
    {
    TInt len = aEncodedSuplInit->Length();
    if(iEncodedSuplInit)
        {
        delete iEncodedSuplInit;
        iEncodedSuplInit = NULL;
        }

    iEncodedSuplInit = HBufC8::NewL(len);
    *iEncodedSuplInit = *aEncodedSuplInit;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SuplSessionID
// Returns SessionID of Session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSessionID* COMASuplSession::SuplSessionID() const	 
    {
    return iSuplSessionId;
    }
	
// -----------------------------------------------------------------------------
// COMASuplSession::GetServerAddressL
// Gets the Server Name and returns it back to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt COMASuplSession::GetServerAddressL(TDes& ahslpAddress)
    {
    if(iSessionCompletionCode == KErrNone)
        {
        iConnRequestor->GetUsedServerAddress(ahslpAddress);
        iTrace->Trace(_L("Session has suceeded...returning address"), KTraceFileName, __LINE__); 
        iTrace->Trace(ahslpAddress, KTraceFileName, __LINE__); 
        return KErrNone;
        }
    else
        {
        iTrace->Trace(_L("Session has failed...returning Error"), KTraceFileName, __LINE__); 
        return KErrNotFound;
        }
    }
    
// -----------------------------------------------------------------------------
// COMASuplSession::GetPosition
// Gets the Position from the SUPL framework and returns it back to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt COMASuplSession::GetPosition(TPositionInfo& aSuplPosInfo)	
    {
    TBool aPositionQualityError=TRUE;
    iTrace->Trace(_L("COMASuplSession::GetPosition(TPositionInfo)"), KTraceFileName, __LINE__); 
    if(!iPosition)	
        {
        iTrace->Trace(_L("COMASuplSession::GetPosition ... Position is Empty"), KTraceFileName, __LINE__); 
        return KErrNotFound;	
        }

    TPosition posInfo;
    TOMASuplUtcTime UtcTime;
    TOMASuplPositionEstimate PosEstimate;
    TDateTime TimeStamp;
    //coverity[var_decl]
    TInt ZoneCode,Zone,altitude,AltitudeUncertainty, HorizontalAccuracy;
    TOMASuplAltitudeInfo AltitudeInfo;
    TInt latitude,longitude;
    TOMASuplPositionEstimate::TOMASuplLatitudeSign LatSign;
    TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altitudeDirection;
    TOMASuplUncertainty Uncertainty;
    TInt UncertaintySemiMajor, UncertaintySemiMinor, OrientationMajorAxis;
    TReal UncertaintySemiMajorReal, UncertaintySemiMinorReal, AltitudeUncertaintyReal;
    TInt confidence;

    iPosition->GetPosition(UtcTime,PosEstimate);
    UtcTime.GetUtcTime(TimeStamp,ZoneCode,Zone);
    TInt altErr = PosEstimate.GetAltitudeInfo(AltitudeInfo);
    PosEstimate.GetPositionEstimate(LatSign,latitude,longitude);
    TInt err = PosEstimate.GetConfidence(confidence);
    altitude = 0;
    if (altErr == KErrNone)
        AltitudeInfo.GetAltitudeInfo(altitudeDirection,altitude,AltitudeUncertainty);

    TTime time(TimeStamp);
    posInfo.SetTime(time);

    if(LatSign == TOMASuplPositionEstimate::ESouth )
        {
        latitude = latitude * -1;
        }

    // Conversion of latitude and longitude
    TReal64 newLat, newLong;
    newLat = latitude;
    newLong = longitude;
            //Conversion as per GAD and OMA Spec
    newLat = (newLat/8388608)*90;
    newLong = (newLong/16777216)*360;

    if (altErr == KErrOMASuplParamNotSet)
        posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong);
    else                    
        posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong,(float)altitude);
    PosEstimate.GetUncertainty(Uncertainty);
    Uncertainty.GetUncertainty(UncertaintySemiMajor,UncertaintySemiMinor,
            OrientationMajorAxis);
    TBuf<64> msg;
    msg.Copy(_L("Uncertainty Semi major"));
    msg.AppendNum(UncertaintySemiMajor);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Uncertainty Semi Minor"));
    msg.AppendNum(UncertaintySemiMinor);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Orientation to Major axis "));
    msg.AppendNum(OrientationMajorAxis);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Altitude "));
    msg.AppendNum(altitude);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Altitude uncertainty "));
    //coverity[uninit_use_in_call] 
    msg.AppendNum(AltitudeUncertainty);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 

    // Convert uncertainty to meters
    TReal pwrMjr, pwrMnr, pwrAlt;
            Math::Pow(pwrMjr, 1.1, UncertaintySemiMajor);
            Math::Pow(pwrMnr, 1.1, UncertaintySemiMinor);
    Math::Pow(pwrAlt, 1.070000, AltitudeUncertainty); 
    UncertaintySemiMajorReal = 10 * (pwrMjr -1);
    UncertaintySemiMinorReal = 10 * (pwrMnr -1);
    AltitudeUncertaintyReal = 10 * (pwrAlt -1);

    TReal MultiplyFactor;

    // Default set accuracy values to NaN
    TRealX nan;
    nan.SetNaN();

    posInfo.SetHorizontalAccuracy(nan);
    posInfo.SetVerticalAccuracy(nan);
    if (err == KErrOMASuplParamNotSet||confidence<=0 || confidence>=100)
        {

TReal32 horizAccr;
horizAccr = KUncertaintyConversionFactor * ( KCEP68ConversionFactor * (UncertaintySemiMajorReal + UncertaintySemiMinorReal ) );
posInfo.SetHorizontalAccuracy(horizAccr);
if (AltitudeUncertaintyReal > 0)
    posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);

        }
    else
        {                    

        TReal lnResult,a_Sigma,b_Sigma;
        Math::Ln(lnResult,(1-confidence/100.0));
        Math::Sqrt(MultiplyFactor,(-2*lnResult));	

        	        a_Sigma=UncertaintySemiMajorReal/MultiplyFactor;
        	
        	        b_Sigma=UncertaintySemiMinorReal/MultiplyFactor;
        	
        	        HorizontalAccuracy=0.7542*(a_Sigma+b_Sigma);

        posInfo.SetHorizontalAccuracy(HorizontalAccuracy);
        TReal32 clientHorAcc;
        if(iClientQop.GetHorizontalAccuracy(clientHorAcc)!=KErrNotFound)
            {
            if(HorizontalAccuracy<clientHorAcc)
                {
                aPositionQualityError=FALSE;	
                }
        }

        /* Calculation for vertical accuracy */
        /* If confidence is 68, then vertical accuracy = altitude uncertainty */
        if (altitude > 0 && altErr == KErrNone)
            {                    
            if (confidence == 68 && AltitudeUncertaintyReal > 0)
                posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);
            else
                {
                TReal32 chiValue=0, verticalAccuracy=0;                        
                if (confidence < 6)
                    {
                    chiValue = chisquare[1];
                    }
                else
                    {
                    chiValue = chisquare[confidence/5];
                    }                
                verticalAccuracy = AltitudeUncertaintyReal/chiValue;
                if (verticalAccuracy > 0)
                    {                                
                    posInfo.SetVerticalAccuracy(verticalAccuracy);
                    TReal32 clientVerAcc;
                    if(iClientQop.GetVerticalAccuracy(clientVerAcc)!=KErrNotFound)
                        {
                        if(verticalAccuracy<clientVerAcc)
                            {
                            aPositionQualityError=FALSE;	
                            }
                        }
                    }
                }
            }                    
        }                    

    TInt aClientTime;  
    if(iClientQop.GetDelay(aClientTime)!=KErrNotFound)
        {
        TTimeIntervalSeconds aDelay;

        iSessionEndTime.SecondsFrom(iSessionStartTime,aDelay);

        if(aClientTime< aDelay.Int())
            aPositionQualityError=FALSE;	
        }

    /*Set the position to TPositionInfo*/
    aSuplPosInfo.SetPosition(posInfo);			

    if(aPositionQualityError!=TRUE)
        return KSuplPositionQualityLoss;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::GetPosition
// Gets the Position from the SUPL framework and returns it back to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt COMASuplSession::GetPosition(HPositionGenericInfo& aSuplPosInfo )
    {
    TBool aPositionQualityError=TRUE;
    iTrace->Trace(_L("COMASuplSession::GetPosition(HPositionGenericInfo)"), KTraceFileName, __LINE__); 
    if(!iPosition)	
        {
        iTrace->Trace(_L("COMASuplSession::GetPosition ... Position is Empty"), KTraceFileName, __LINE__); 
        return KErrNotFound;	
        }

    TRAP_IGNORE(StoreCurrentCellIDL(aSuplPosInfo));

    TPosition posInfo;
    TOMASuplUtcTime UtcTime;
    TOMASuplPositionEstimate PosEstimate;
    TDateTime TimeStamp;
    //coverity[var_decl]
    TInt ZoneCode,Zone,altitude,AltitudeUncertainty, HorizontalAccuracy;
    TOMASuplAltitudeInfo AltitudeInfo;
    TInt latitude,longitude;
    TOMASuplPositionEstimate::TOMASuplLatitudeSign LatSign;
    TOMASuplAltitudeInfo::TOMASuplAltitudeDirection AltitudeDirection;
    TOMASuplUncertainty Uncertainty;
    TInt UncertaintySemiMajor, UncertaintySemiMinor, OrientationMajorAxis;
    TReal UncertaintySemiMajorReal, UncertaintySemiMinorReal, AltitudeUncertaintyReal;
    TInt confidence;

    iPosition->GetPosition(UtcTime,PosEstimate);
    UtcTime.GetUtcTime(TimeStamp,ZoneCode,Zone);
    TInt altErr = PosEstimate.GetAltitudeInfo(AltitudeInfo);
    PosEstimate.GetPositionEstimate(LatSign,latitude,longitude);
    TInt err = PosEstimate.GetConfidence(confidence);
    altitude = 0;
    if (altErr == KErrNone)
        AltitudeInfo.GetAltitudeInfo(AltitudeDirection,altitude,AltitudeUncertainty);
    PosEstimate.GetUncertainty(Uncertainty);
    Uncertainty.GetUncertainty(UncertaintySemiMajor,UncertaintySemiMinor,
            OrientationMajorAxis);
    // Convert uncertainty to meters
    TReal pwrMjr, pwrMnr, pwrAlt;
            Math::Pow(pwrMjr, 1.1, UncertaintySemiMajor);
            Math::Pow(pwrMnr, 1.1, UncertaintySemiMinor);
    //coverity[uninit_use] 
    Math::Pow(pwrAlt, 1.070000, AltitudeUncertainty); 
    UncertaintySemiMajorReal = 10 * (pwrMjr -1);
    UncertaintySemiMinorReal = 10 * (pwrMnr -1);
    AltitudeUncertaintyReal = 10 * (pwrAlt -1);

    TReal MultiplyFactor;

    TBuf<64> msg;
    msg.Copy(_L("Uncertainty Semi major"));
    msg.AppendNum(UncertaintySemiMajor);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Uncertainty Semi Minor"));
    msg.AppendNum(UncertaintySemiMinor);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Orientation to Major axis "));
    msg.AppendNum(OrientationMajorAxis);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Altitude "));
    msg.AppendNum(altitude);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
    msg.Copy(_L("Altitude uncertainty "));
    msg.AppendNum(AltitudeUncertainty);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 

    // Default set accuracy values to NaN
    TRealX nan;
    nan.SetNaN();

    posInfo.SetHorizontalAccuracy(nan);
    posInfo.SetVerticalAccuracy(nan);

    if (err == KErrOMASuplParamNotSet||confidence<=0 || confidence>=100)
        {
        iTrace->Trace(_L("Confidance Not Set"), KTraceFileName, __LINE__); 

        TReal32 horizAccr;
        horizAccr = KUncertaintyConversionFactor * ( KCEP68ConversionFactor * (UncertaintySemiMajorReal + UncertaintySemiMinorReal ) );
        posInfo.SetHorizontalAccuracy(horizAccr);
        if (AltitudeUncertaintyReal > 0)
            posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);
        }
    else
        {
        iTrace->Trace(_L("Confidance is Set"), KTraceFileName, __LINE__); 

        TReal lnResult,a_Sigma,b_Sigma;
        Math::Ln(lnResult,(1-confidence/100.0));
        Math::Sqrt(MultiplyFactor,(-2*lnResult));	

	        	    a_Sigma=UncertaintySemiMajorReal/MultiplyFactor;
	        	    b_Sigma=UncertaintySemiMinorReal/MultiplyFactor;

	        	    HorizontalAccuracy=0.7542*(a_Sigma+b_Sigma);

        posInfo.SetHorizontalAccuracy(HorizontalAccuracy);
        TReal32 clientHorAcc;
        if(iClientQop.GetHorizontalAccuracy(clientHorAcc)!=KErrNotFound)
            {
            if(HorizontalAccuracy<clientHorAcc)
                {
                aPositionQualityError=FALSE;	
                }
            }


        /* If confidence is 68, then vertical accuracy = altitude uncertainty */
        if (altitude > 0 && altErr == KErrNone)
            {                    
            if (confidence == 68 && AltitudeUncertaintyReal > 0)
                posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);
            else
                {
                TReal32 chiValue=0, verticalAccuracy=0;                        
                if (confidence < 6)
                    {
                    chiValue = chisquare[1];
                    }
                else
                    {
                    chiValue = chisquare[confidence/5];
                    }                
                verticalAccuracy = AltitudeUncertaintyReal/chiValue;
                if (verticalAccuracy > 0)
                    {                            
                    posInfo.SetVerticalAccuracy(verticalAccuracy);
                    TReal32 clientVerAcc;
                    if(iClientQop.GetVerticalAccuracy(clientVerAcc)!=KErrNotFound)
                        {
                        if(verticalAccuracy<clientVerAcc)
                            {
                            aPositionQualityError=FALSE;	
                            }
                        else
                            aPositionQualityError=FALSE;	
                        }
                    }
                }                    
            }                    
        }

    TTime time(TimeStamp);
    posInfo.SetTime(time);

    if(LatSign == TOMASuplPositionEstimate::ESouth )
        {
        latitude = latitude * -1;
        }

    // Conversion of latitude and longitude
    TReal64 newLat, newLong;
    newLat = latitude;
    newLong = longitude;
    newLat = (newLat/8388608)*90;
    newLong = (newLong/16777216)*360;
    if (altErr == KErrOMASuplParamNotSet)
        posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong);
    else                    
        posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong,(float)altitude);

    /*Set the position to TPositionInfo*/
    aSuplPosInfo.SetPosition(posInfo);			

    /* Set GenInfo related fields */
    TOMASuplVelocityType    VelType;
    COMASuplHorizVelocity*  HorizVel;
    COMASuplVelocity*       SuplVel;
    TUint16 HorSpeed;
    TUint16 Bearing;
    TUint8 VertDirection;
    TUint8 VertSpeed;
    TUint8 UncertSpeed;
    TUint8 HorUncertSpeed,VerUncertSpeed;

    iTrace->Trace(_L("Before iPosition->GetVelocity"), KTraceFileName, __LINE__); 
    TInt ret = iPosition->GetVelocity(SuplVel);
    iTrace->Trace(_L("After iPosition->GetVelocity"), KTraceFileName, __LINE__); 
    if (ret == KErrNone)
        {

        VelType = SuplVel->VelType();

        HorizVel = SuplVel->Velocity();

        switch(VelType)
            {
            case EHorizVelocity:
                {
                iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizVelocity"), KTraceFileName, __LINE__); 
                aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                HorizVel->GetHorizVel(Bearing, HorSpeed);
                aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizVelocity"), KTraceFileName, __LINE__); 
                break;                        
                }
            case EHorizAndVertVelocity:
                {
                iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertVelocity"), KTraceFileName, __LINE__); 
                COMASuplHorizAndVertVelocity* horizVertVel = (COMASuplHorizAndVertVelocity*)HorizVel;
                horizVertVel->GetHorizAndVertVel(Bearing, HorSpeed, VertDirection,
                        VertSpeed);
                aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                aSuplPosInfo.SetRequestedField(EPositionFieldVerticalSpeed);
                aSuplPosInfo.SetValue(EPositionFieldVerticalSpeed, (TReal32)VertSpeed);
                iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertVelocity"), KTraceFileName, __LINE__); 
                break;
                }
            case EHorizUncertVelocity:
                {
                iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizUncertVelocity"), KTraceFileName, __LINE__); 
                COMASuplHorizUncertVelocity* horizUncertVel = (COMASuplHorizUncertVelocity*)HorizVel;
                aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                horizUncertVel->GetHorizUncertVel(Bearing, HorSpeed, UncertSpeed); 
                aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeedError);
                aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeedError, (TReal32)UncertSpeed);
                iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizUncertVelocity"), KTraceFileName, __LINE__); 
                break;
                }
            case EHorizAndVertUncertVelocity:
                {
                iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertUncertVelocity"), KTraceFileName, __LINE__); 
                COMASuplHorizAndVertUncertVelocity* horizVertUncertVel = (COMASuplHorizAndVertUncertVelocity*)HorizVel;
                aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                horizVertUncertVel->GetHorizVertUncertVel(Bearing, HorSpeed,
                        VertDirection,
                        VertSpeed,HorUncertSpeed,VerUncertSpeed); 
                aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeedError);
                aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeedError, (TReal32)HorUncertSpeed);
                aSuplPosInfo.SetRequestedField(EPositionFieldVerticalSpeed);
                aSuplPosInfo.SetValue(EPositionFieldVerticalSpeed, (TReal32)VertSpeed);
                aSuplPosInfo.SetRequestedField(EPositionFieldVerticalSpeedError);
                aSuplPosInfo.SetValue(EPositionFieldVerticalSpeedError, (TReal32)VerUncertSpeed);
                iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertUncertVelocity"), KTraceFileName, __LINE__); 
                break;
                }
            }
        }
    TInt aClientTime;  
    if(iClientQop.GetDelay(aClientTime)!=KErrNotFound)
        {
        TTimeIntervalSeconds aDelay;
        iSessionEndTime.SecondsFrom(iSessionStartTime,aDelay);

        if(aClientTime< aDelay.Int())
            aPositionQualityError=FALSE;	
        }

    if(aPositionQualityError==FALSE)
        return KSuplPositionQualityLoss;   
    return KErrNone;
    }

	
// -----------------------------------------------------------------------------
// COMASuplSession::HandleOMASuplMessage
// Handles OMA Supl messages & starts processing according to state of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleOMASuplMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage,TRequestStatus& aStatus,TInt aSessionIdSeed,TInt aErrorCode)
    {
    delete iSuplState;
    iSuplState = NULL;

    iSETSessionUniqueId = aSessionIdSeed;
    iRunRequestStatus = & aStatus;
    *iRunRequestStatus = KRequestPending;

    TInt networkMode = 1;
    networkMode = GetNetworkModeL();

    TBuf<64> msg;
    if ( networkMode == ECoreAppUIsNetworkConnectionNotAllowed )
        {
        msg.Copy(_L("The device is in OFFLINE mode."));
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        iSessionObserver.TerminateSession(this, KErrGeneral);
        return;
        }
    else
        {
        msg.Copy(_L("The device is in ON LINE mode."));
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        }

        /*CSuplSettings::TSuplSettingsUsage usage = iSuplSettings->SUPLUsage();

    if (usage == CSuplSettings::ESuplUsageDisabled)
        {                
        msg.Copy(_L("SUPL Usage is disabled"));
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        iSessionObserver.TerminateSession(this, KErrGeneral);	
        return;
            }*/

    COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();

    if(messageType == COMASuplAsnMessageBase::ESUPL_INIT && aErrorCode == KErrNone)
        {
        HandleOMASuplMessageL(aDecodedAsnMessage);
        }
    else //Other messages than SUPL_INIT.... Send End
        {
        CreateCloneMessageL(aDecodedAsnMessage);		
        UpdateSuplSessionIDL();
 
        iSuplSessionState = ESUPL_INITIALIZED;
        iSuplMsgType = ESUPL_END;

        if((aErrorCode == KErrCompletion && messageType == COMASuplAsnMessageBase::ESUPL_INIT) ||
                (aErrorCode == KErrOMASuplOutOfRangeParameter && messageType == COMASuplAsnMessageBase::ESUPL_INIT) ||
                (aErrorCode == KErrOMASuplMessageLengthMismatch && messageType == COMASuplAsnMessageBase::ESUPL_INIT) ||
                (aErrorCode == KErrOMASuplShortFrame && messageType == COMASuplAsnMessageBase::ESUPL_INIT))	    			
            {
            iErrorStatusCode = COMASuplEnd::EUnexpectedDataValue;
            if(aErrorCode == KErrOMASuplMessageLengthMismatch)
                {
                iErrorStatusCode = COMASuplEnd::EProtocolError;
                }
            if(aErrorCode == KErrOMASuplShortFrame)
                {
                iErrorStatusCode = COMASuplEnd::EDataMissing;
                }
            TInt len = iHSLPAddress.Length();
            HBufC8 *hslpAdress = NULL;
            if(len > 0)
                {
                hslpAdress = HBufC8::NewL(iHSLPAddress.Length());
                hslpAdress->Des().Copy(iHSLPAddress);
                }

            else if(len ==0)
                {
                CServerParams* serverParams = CServerParams::NewL();
                CleanupStack::PushL(serverParams);
    
                if (iSuplStorageSettings->GetDefaultServer(serverParams) == KErrNotFound )
                    {
                    iTrace->Trace(_L("Length of HSLP Address is = 0, passing the HSLP generated frm IMSI"), KTraceFileName, __LINE__);
                    hslpAdress = HBufC8::NewL(iSuplSettings->SLPAddressfromImsi().Length());
                    hslpAdress->Des().Copy(iSuplSettings->SLPAddressfromImsi());	
                    }
                else
                    {
                    iTrace->Trace(_L("Sending End with ver for Default HSLP"), KTraceFileName, __LINE__);
                    TInt64 slpId;
                    TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
                    HBufC* serverAddr = HBufC::NewL(KMaxHSLPAddrLen);
                    HBufC* iapName = HBufC::NewL(KMaxIAPLen);
                    CleanupStack::PushL(serverAddr);
                    CleanupStack::PushL(iapName);
                    serverParams->Get(slpId,serverAddr->Des(),iapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
                    hslpAdress = HBufC8::NewL(serverAddr->Length());
                    hslpAdress->Des().Copy(*serverAddr);
    
                    CleanupStack::PopAndDestroy(iapName);
                    CleanupStack::PopAndDestroy(serverAddr);
                    }
                CleanupStack::PopAndDestroy(serverParams);
                }
            delete iSuplState;
            iSuplState=NULL;
            CleanupStack::PushL(hslpAdress);
            iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress);		
            CleanupStack::PopAndDestroy(hslpAdress);
            }
        else
            {
            iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
            iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
            }	

        SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
        iSuplState->SetMsgStateObserver(this);
        iSuplState->GenerateMessageL();
        iSuplSessionState = ESUPL_GENERATE; 
        }	
    }

// -----------------------------------------------------------------------------
// COMASuplSession::HandleOMASuplMessage
// Handles OMA Supl messages & starts processing according to state of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleOMASuplMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage)
    {
    CreateCloneMessageL(aDecodedAsnMessage);
    if(CheckProtocolVersionL(aDecodedAsnMessage)) //Checks version with supported version
        {
        COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();

        if(messageType != COMASuplAsnMessageBase::ESUPL_POS)
            {
            iCompleteSelfRequestor->CompleteSelf(); //This will result in call of RequestCompleted()
            }
        else
            {
            RequestCompletedL();	
            }
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::RequestCompleted
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::RequestCompletedL()
    {
    //Start Next Processing......	
    iTrace->Trace(_L("COMASuplSession::RequestCompleted...Retriving message type"), KTraceFileName, __LINE__); 									

    COMASuplAsnMessageBase::TSuplMessageType messageType = iDecodedAsnMessage->MessageType();

    switch(messageType)
        {
        case COMASuplAsnMessageBase::ESUPL_RESPONSE:
            {
            iSuplSessionState = ESUPL_RECEIVED;
            iTrace->Trace(_L("Received Message is of SUPL_RESPONSE type..."), KTraceFileName, __LINE__); 									
            CancelAllOperations();
            if(iSuplMsgType == ESUPL_START)
                {
                iSuplMsgType = ESUPL_RESPONSE; 
                ProcessSuplResponseL();
                }
            else
                {
                iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
                // Send SUPL End with Protocol Error
                HandleSuplErrorL(KErrOMASuplProtocolError);				 		
                }
            break;

            }
        case COMASuplAsnMessageBase::ESUPL_POS: 
            {
            iTrace->Trace(_L("Received Message is of SUPL_POS type..."), KTraceFileName, __LINE__); 									
            if(iOMASuplPOSRequestor) //
                {
                //if(((iSuplMsgType == ESUPL_POS || iSuplMsgType == ESUPL_POSINIT)) && !IsLastPOSMessage())
                if(((iSuplMsgType == ESUPL_POS || iSuplMsgType == ESUPL_POSINIT))) //Changed for SUPL_POS after UT3
                    {
                    iSuplMsgType = ESUPL_POS; 
                    iTimer->StopTimer();	 // Added by Manish
                    iSuplState->StopTimer();
                    ProcessSuplPosL();
                    }
                else
                    {
                    iSuplSessionState = ESUPL_RECEIVED; 
                    CancelAllOperations();
                    iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
                    // Send SUPL End with Protocol Error
                    HandleSuplErrorL(KErrOMASuplProtocolError);				 		
                    }
                }
            else // POS is not there
                {
                iSuplSessionState = ESUPL_RECEIVED; 
                CancelAllOperations();
                iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
                // Send SUPL End with Protocol Error
                HandleSuplErrorL(KErrOMASuplProtocolError);				 		
                }	
            break;
            }
        case COMASuplAsnMessageBase::ESUPL_END:
            {
            iSuplSessionState = ESUPL_RECEIVED;
            iTrace->Trace(_L("Received Message is of SUPL_END type..."), KTraceFileName, __LINE__); 									
            CancelAllOperations();
            //Close Connection--- Bug fix
            iConnRequestor->CloseConnection();
			iPortNum = 0;
			//Remove entry from Settings to inform UI that trigger session has stop
			RemoveActiveSession();
            if(IsGenerationInProgress()) //Check is generation going on
                {
                // Invoke SessionTerminate
                iSessionObserver.TerminateSession(this, KErrCompletion);
                break;
                }

            if(iSuplMsgType != ESUPL_END)
                {
                iSuplMsgType = ESUPL_END; //::POS::
                ProcessSuplEndL();		
                }						
            break;
            }

        case COMASuplAsnMessageBase::ESUPL_INIT:	 //::POS::
            {
            iTrace->Trace(_L("Received Message is of ESUPL_INIT type..."), KTraceFileName, __LINE__); 									
            iSuplSessionState = ESUPL_RECEIVED;
            CancelAllOperations();
            iSuplMsgType = ESUPL_INIT; //::POS::
            ProcessSuplInitL();
            break;
            }

        case COMASuplAsnMessageBase::ESUPL_AUTH_REQ:
        case COMASuplAsnMessageBase::ESUPL_AUTH_RESP:
            {
            iTrace->Trace(_L("Received Message is of ESUPL_AUTH_RESPONCE/ESUPL_AUTH_REQ type..."), KTraceFileName, __LINE__); 									
            iSuplSessionState = ESUPL_RECEIVED;
            CancelAllOperations();
            iSuplMsgType = ESUPL_AUTH_RESP; 
            ProcessSuplAuthResponceL();
            break;
            }
        case COMASuplAsnMessageBase::ESUPL_TRIG_RESPONSE:
            {
            iSuplSessionState = ESUPL_RECEIVED;
            iTrace->Trace(_L("Received Message is of SUPL_TRIGGER_RESPONSE type..."), KTraceFileName, __LINE__); 									
            CancelAllOperations();
            if(iSuplMsgType == ESUPL_TRIG_START)
                {
                iSuplMsgType = ESUPL_TRIG_RESPONSE; 
                ProcessSuplTriggerResponseL();
                //Start timer without waiting for clientNotify
				if (iRequestType == ESUPL_TERMINAL)
                	StartPeriodicTriggerTimer();
                }
            else
                {
                iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
                // Send SUPL End with Protocol Error
                HandleSuplErrorL(KErrOMASuplProtocolError);				 		
                }
            break;

            }

        case COMASuplAsnMessageBase::ESUPL_REPORT:
            {
            iSuplSessionState = ESUPL_RECEIVED;
            iTrace->Trace(_L("Received Message is of SUPL_REPORT type..."), KTraceFileName, __LINE__); 									
            CancelAllOperations();

            if (iInterval > KMaxIntervalTime)
                iConnRequestor->CloseConnection();

            if(IsGenerationInProgress()) //Check is generation going on
                {
                // Invoke SessionTerminate
                if (iRequestType != ESUPL_NETWORK)
                    iSessionObserver.TerminateSession(this, KErrCompletion);
                break;
                }

            //if(iSuplMsgType != ESUPL_REPORT)
            if(iSuplMsgType == ESUPL_POS || iSuplMsgType == ESUPL_POSINIT)
                {
                iSuplMsgType = ESUPL_REPORT; //::POS::
                ProcessSuplReportL();		
            	if (iNumFixes == 0 && iRequestType == ESUPL_NETWORK)
                   {                                        
				   iTrace->Trace(_L("StartTimer for SUPL_TRIGGER_REPORT..."), KTraceFileName, __LINE__); 
		           iSuplState->SetMsgStateObserver(this);
				   iSuplState->StartTimer(); 	
                   }
                                                                            
				}
            else
                {
                iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__);                                  
                // Send SUPL End with Protocol Error
                HandleSuplErrorL(KErrOMASuplProtocolError);                     
                }
            /*	if(iSuplMsgType != ESUPL_REPORT)
							{
								iSuplMsgType = ESUPL_REPORT; //::POS::
								ProcessSuplReportL();		
							}			*/			
            break;
            }
        case COMASuplAsnMessageBase::ESUPL_TRIG_STOP:
            {
            iSuplSessionState = ESUPL_RECEIVED;
            iTrace->Trace(_L("Received Message is of SUPL_TRIGGER_STOP type..."), KTraceFileName, __LINE__); 									
            CancelAllOperations();

            if(iSuplMsgType != ESUPL_TRIG_STOP)
                {
                iSuplMsgType = ESUPL_TRIG_STOP; //::POS::
                ProcessSuplTriggerStopL();		
                }						
            break;
            }
        default:
            {
            iSuplSessionState = ESUPL_RECEIVED;	
            iTrace->Trace(_L("Received Message is of UNKNOWN type..."), KTraceFileName, __LINE__); 									
            CancelAllOperations();
            HandleSuplErrorL(KErrOMASuplProtocolError);
            break;	

            }
        }
    }


// -----------------------------------------------------------------------------
// COMASuplSession::CancelAllOperations
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::CancelAllOperations()
    {
    if(iSuplState)
        {
        iSuplState->CancelOperation();
        }

    iTrace->Trace(_L("iConnRequestor->Cancel..."), KTraceFileName, __LINE__); 									
    iConnRequestor->Cancel();
    iTrace->Trace(_L("iCompleteSelfRequestor->Cancel..."), KTraceFileName, __LINE__); 									
    iCompleteSelfRequestor->Cancel();

    if(iOMASuplPOSRequestor)
        iOMASuplPOSRequestor->Cancel(); 

    iTrace->Trace(_L("iTimer->StopTimer..."), KTraceFileName, __LINE__); 									
    iTimer->StopTimer();	   
    }

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplResponse
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplResponseL()
    {
    iTrace->Trace(_L("Processing SUPL Response..."), KTraceFileName, __LINE__); 							

    // Recreate SUPL STATE
    delete iSuplState;
    iSuplState = NULL;

    iSuplState = COMASuplResponseState::NewL();
    iSuplState->SetVersion(iSuplVersion);
    TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	
    iSuplMsgType = ESUPL_RESPONSE;

    iPosMethod = static_cast<COMASuplResponseState *>(iSuplState)->GetPosMethod();

    TBuf<128> msg(_L("ProcessSuplResponseL() got iPosMethod: "));
    msg.AppendNum(iPosMethod);
    iTrace->Trace(msg, KTraceFileName, __LINE__);
    if(KErrNone == err)
        {
        iSuplSessionState = ESUPL_GENERATE;
        UpdateSuplSessionIDL();
        OperationCompleteL(KErrNone);	
        }
    else
        {
        HandleSuplErrorL(err);
        }

    }

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplPosL
// Process SUPL POS
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplPosL()
    {
    iTrace->Trace(_L("Processing SUPL Pos..."), KTraceFileName, __LINE__); 							
    TInt error = KErrNone;

    // Recreate SUPL STATE if the previous state was not ESUPL_POS
    if(iSuplState)
        {
        if(COMASuplState::ESUPL_POS != iSuplState->GetSuplState())
            {
            delete iSuplState;
            iSuplState = NULL;

            iSuplState = COMASuplPosState::NewL(iPOSSession,iUT3_PosTimer,iSETCapabilities,iOMASuplAsnHandlerBaseImpl);					
            }

        iSuplState->SetVersion(iSuplVersion);
        error = iSuplState->ProcessMessageL(iDecodedAsnMessage);

        //This is for generating SUPL_POS, after UT3 timer started & received SUPL_POS.

        //!iIsFirstPOSMessage this is for putting new request if we
        //gets SUPL_POS even if first POS message from SET failed to generate data 
        if( KErrNone == error && ( IsLastPOSMessage() || !iIsFirstPOSMessage )) 
            {
            iTrace->Trace(_L("Generating POS ... After starting UT3 OR After failing of Generation of first POS message..."), KTraceFileName, __LINE__); 
            iSuplMsgType = ESUPL_POS;
            iSuplSessionState = ESUPL_GENERATE;
            GenerateSuplMessageL();
            iSuplSessionState = ESUPL_ENCODE;
            }
        }

    if(KErrNone != error)
        {				
        TBuf<64> msg(_L("Error while Processing SUPL Pos is : "));
        msg.AppendNum(error);
        iTrace->Trace(msg, KTraceFileName, __LINE__); 
        HandleSuplErrorL(error);
        }

    }
	
// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplEndL
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplEndL()
    {
    iTrace->Trace(_L("COMASuplSession::ProcessSuplEndL..."), KTraceFileName, __LINE__); 							

    // Recreate SUPL STATE
    delete iSuplState;
    iSuplState = NULL;

    iSuplState = COMASuplEndState::NewL(COMASuplEnd::EUnspecified,iOMASuplAsnHandlerBaseImpl);
    iSuplState->SetVersion(iSuplVersion);
    TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	

    LogEndTime(); //Log end time if QoP & delay in Qop is present.

    if(KErrOMASuplVersionNotMatching != err) //KErrNone == err)..changed for new requirement...
        // Even if there is status code in SUPL_END ... still process SUPL_END 	
        {
        if(iRequestType == COMASuplSession::ESUPL_TERMINAL)
            {
            // if it is a trigger request, complete the
            // request immediately as we don't expect position
            // information in SUPL_END
            if (iTriggerSession)
                {
                // Invoke SessionTerminate
                iTriggerSession = EFalse; // reset the flag
                iSessionObserver.TerminateSession(this, KErrNone);
                }	
            else
                {                           
                // Delete Position
                delete iPosition;
                iPosition = NULL;

                COMASuplEndState *currState = static_cast <COMASuplEndState *>(iSuplState);
                COMASuplEnd::TOMASuplStatusCode statusCode = currState->GetStatusCode();

                if (statusCode == COMASuplEnd::EPosMethodMismatch)
                    {                                
                    iFailedCaps = iAllowedCapabilities;
                    iCapsFail = ETrue;
                    }                                    

                // Retrieve Position
                COMASuplPosition *currPosition = currState->GetPosition();
                if(currPosition)  //Position is in SUPL_END
                    {
                    iPosition =static_cast <COMASuplPosition *>(currPosition->CloneL());	

                    if(KErrNone == err)
                        {
                        iTrace->Trace(_L("Received SUPL_END Without Status Code"), KTraceFileName, __LINE__);
                        // Invoke SessionTerminate
                        iSessionObserver.TerminateSession(this,KErrNone);
                        }
                    else  // Position & status code is present...thats why completing with KErrCompletion.
                        {
                        iTrace->Trace(_L("Received SUPL_END With Status Code"), KTraceFileName, __LINE__);
                        // Invoke SessionTerminate
                        // Changed error code as location Fw uses position if it completes with +ve value. //KErrCompletion
                        iSessionObserver.TerminateSession(this,KSuplPositionPresent);
                        }
                    }
                else
                    {
                    if(iOMASuplPOSRequestor)
                        {
                        iPosition = COMASuplPosition::NewL();
                        iOMASuplPOSRequestor->SetObserver(this);
                        err = iOMASuplPOSRequestor->GetPositionL(iPosition);
                        if(err != KErrNone)
                            {
                            delete iPosition;
                            iPosition = NULL;
                            // Invoke SessionTerminate
                            iSessionObserver.TerminateSession(this, err);	
                            }
                        }
                    else //Position && iOMASuplPOSRequestor are not there
                        {
                        // Invoke SessionTerminate
                        iSessionObserver.TerminateSession(this, KErrNone);
                        }	

                    }
                } 
            } 
        else
            {
            // Invoke SessionTerminate //Close session
            iSessionObserver.TerminateSession(this, KErrNone);	
            }				  	 
        }
    else
        {
        //  Handle Error
        HandleSuplErrorL(err);
        }
    }


// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplInitL
// Process SUPL INIT
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplInitL()
    {
    iTrace->Trace(_L("COMASuplSession::ProcessSuplInitL..."), KTraceFileName, __LINE__); 							

    TInt ret = ServerAddressCheckForSuplInitL();
    if(ret != KErrNone)
        {
        HandleSuplErrorL(KErrGeneral);
        return;
        }

    // Recreate SUPL STATE
    delete iSuplState;
    iSuplState = NULL;
    UpdateSuplSessionIDL();
    iSuplState = COMASuplInitState::NewL(iPrivacyTimer,iHSLPAddress,*this,iSETMode,iNetworkPrivacy);
    iSuplState->SetVersion(iSuplVersion);
    static_cast<COMASuplInitState*>(iSuplState)->SetSessionId(iSETSessionUniqueId);
    TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);//will result in PrivacyCheckCompleteL

        if (static_cast<COMASuplInitState*>(iSuplState)->IsTriggerSession())
            {                            
            iTriggerSession = ETrue;                                
            iTriggerType = static_cast<COMASuplInitState*>(iSuplState)->GetTriggerType();
            }

		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	
		
		iPosMethod  = static_cast<COMASuplInitState*>(iSuplState)->GetPosMethod();
		
		TBuf<128> msg(_L("COMASuplSession::ProcessSuplInitL() got iPosMethod : "));
		msg.AppendNum(iPosMethod);
		iTrace->Trace(msg, KTraceFileName, __LINE__);
		if(KErrOMASuplNoPosition == err || KErrSuplServiceNotSupported == err || 
                        KErrOMANonProxyModeNotSupported == err )
			{
				HandleSuplInitErrorL(err);
				return;
			}
			
		if(KErrNone != err)
			{
				HandleSuplErrorL(err);
			}
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplAuthResponceL
// Process SUPL AUTH  RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplAuthResponceL()
    {
    // No need to create state class object for now....
    // if needed in future ... we can write it...since it will add more code which is 
    //equivalent to current implementation.

    delete iSuplState;
    iSuplState = NULL;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	// inform to POS
    HandleSuplErrorL(KErrOMASuplProtocolError);
    }

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplTriggerResponse
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplTriggerResponseL()
    {
    iTrace->Trace(_L("Processing SUPL Trigger Response..."), KTraceFileName, __LINE__); 							

    // Recreate SUPL STATE
    if (iSuplState)
        {                
        iTrace->Trace(_L("Deleting SUPL state..."), KTraceFileName, __LINE__); 							
        delete iSuplState;
        iSuplState = NULL;
        }

    iTrace->Trace(_L("Creating trigger Response..."), KTraceFileName, __LINE__); 							
    iSuplState = COMASuplTriggerResponseState::NewL();
    iTrace->Trace(_L("Setting version..."), KTraceFileName, __LINE__); 							
    iSuplState->SetVersion(iSuplVersion);
    iTrace->Trace(_L("Calling ProcessMessageL..."), KTraceFileName, __LINE__); 							
    TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	
    iSuplMsgType = ESUPL_TRIG_RESPONSE;

    iPosMethod = static_cast<COMASuplTriggerResponseState *>(iSuplState)->GetPosMethod();

    TBuf<128> msg(_L("ProcessSuplTriggerResponseL() got iPosMethod: "));
    msg.AppendNum(iPosMethod);
    iTrace->Trace(msg, KTraceFileName, __LINE__);

    //If no error, complete request
    if (iRequestType == ESUPL_NETWORK)
        {
            //iTrace->Trace(_L(" Calling COMASuplSession2::LaunchNotifyDialogL..."), KTraceFileName, __LINE__);
            	//LaunchNotifyDialogL();
							//Inform UI that ,trigger session has started...
								//InsertActiveSessionL();
        TBool trgFlag = static_cast<COMASuplTriggerResponseState *>(iSuplState)->GetTriggerParamsFlag();

        // If it is a NI request, read trigger params                    
        if (trgFlag)
            {                    
            TOMASuplTriggerParams trgParams = static_cast<COMASuplTriggerResponseState *>(iSuplState)->GetTriggerParams();
            TOMASuplPeriodicParams perTrgParams;
            trgParams.GetPeriodicParams(perTrgParams);

                perTrgParams.GetSuplPeriodicParams((TInt32&)iNumFixes, (TInt32&)iInterval);
                perTrgParams.GetStartTimeSet((TInt32&)iStartTime);
                iTotalNoOfFixes = iNumFixes;
                iStartTimeCopy = iStartTime;

                //Close connection
                if (iInterval > KMaxIntervalTime || iStartTime > KMaxStartTime)
				    iConnRequestor->CloseConnection();
                //For NI request, we don't get notify request
                StartPeriodicTriggerTimer();
                iClientNotify = ETrue;
            }
		    else
			    {
				//HandleSuplErrorL(err);
				iErrorStatusCode = COMASuplEnd::EDataMissing;
				iSuplSessionState = ESUPL_GENERATE;
				iSuplMsgType = ESUPL_END;
				GenerateSuplMessageL();
			    }
            }
        else        
            {                
		    if(KErrNone == err)
			    {
    	        if(iRunRequestStatus)
    		        {
   		            User::RequestComplete(iRunRequestStatus,KErrNone);
                    }
                GetPeriodicTriggerParams();
                //Inform UI that ,trigger session has started...
								InsertActiveSessionL();
                //Close connection
                if (iInterval > KMaxIntervalTime || iStartTime > KMaxStartTime)
				    iConnRequestor->CloseConnection();
                }
		    else
			    {
				HandleSuplErrorL(err);
			    }
            }
        //Update session ID received in trigger response
	    UpdateSuplSessionIDL();
	       //Inform UI that ,trigger session has started...
	      //  InsertActiveSessionL();


    }

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplReportL
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplReportL()
    {
    iTrace->Trace(_L("COMASuplSession::ProcessSuplReportL..."), KTraceFileName, __LINE__); 							

    // Recreate SUPL STATE
    delete iSuplState;
    iSuplState = NULL;
		iTrace->Trace(_L("After state delete ProcessSuplReportL..."), KTraceFileName, __LINE__); 							
		TBuf<32> tempBuf;
		tempBuf.Append(_L("Timer Value:"));
		tempBuf.AppendNum(iUT2_PosInitTimer);
		iTrace->Trace(tempBuf, KTraceFileName, __LINE__); 
    iSuplState = COMASuplReportState::NewL(iUT2_PosInitTimer, iOMASuplAsnHandlerBaseImpl);
    iSuplState->SetVersion(iSuplVersion);
    TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	

    LogEndTime(); //Log end time if QoP & delay in Qop is present.

    //Decrement number of fixes requested	
    iNumFixes--;
    
    
    if(iRequestType == COMASuplSession::ESUPL_TERMINAL)
        {
        iTrace->Trace(_L("ProcessSuplReportL..., terminal request"), KTraceFileName, __LINE__); 							
        // Delete Position
        delete iPosition;
        iPosition = NULL;
        UpdateActiveTriggerSession(); //Update UI for trigger info...
        COMASuplReportState *currState = static_cast <COMASuplReportState *>(iSuplState);
        // Retrieve Position
        COMASuplPosition *currPosition = currState->GetPositionL();
        if(currPosition)  //Position is in SUPL_REPORT for network based positioning
            {
            iTrace->Trace(_L("ProcessSuplReportL..., Position present"), KTraceFileName, __LINE__);
            iPosition = static_cast <COMASuplPosition *>(currPosition->CloneL());	
            TPositionInfo position;        
            GetPosition(position);
            if (iNumFixes == 0)
                {                    
                iTrace->Trace(_L("ProcessSuplReportL, last fix generation SUPL END"), KTraceFileName, __LINE__);
                iSuplSessionState = ESUPL_GENERATE;
                iSuplMsgType = ESUPL_END;
                GenerateSuplMessageL();
                }
            else
                {                    
                if (iTotalNoOfFixes != iNumFixes || iClientNotify)//if this is the first fix and client notify is true then complete the request
                    {                        
                    iTrace->Trace(_L("ProcessSuplReportL, Completing session"), KTraceFileName, __LINE__);
                    if( iTrgFireInfo )
	                    {
	                    iTrgFireInfo->Set(position);
	                    }
                    CompleteSession(KErrNone);
                    }
                }
            }
        else  //Position is with POS Plugin for MS-BASED case
            {
            if(iOMASuplPOSRequestor)
                {
                iPosition = COMASuplPosition::NewL();
                iOMASuplPOSRequestor->SetObserver(this);
                err = iOMASuplPOSRequestor->GetPositionL(iPosition);
                if(err != KErrNone)
                    {
                    delete iPosition;
                    iPosition = NULL;
                    // Invoke SessionTerminate
                    if (iRequestType != ESUPL_NETWORK)
                        iSessionObserver.TerminateSession(this, err);	
                    }
                }
            else //Position && iOMASuplPOSRequestor are not there
                {
                // Invoke SessionTerminate
                if (iRequestType != ESUPL_NETWORK)
                    {			
                    if (iNumFixes == 0)
                        {                    
                        iTrace->Trace(_L("ProcessSuplReportL, last fix generation SUPL END"), KTraceFileName, __LINE__);
                        iSuplSessionState = ESUPL_GENERATE;
                        iSuplMsgType = ESUPL_END;
                        GenerateSuplMessageL();
                        }
                    else
                        {                            
                        iTrace->Trace(_L("ProcessSuplReportL...,  completing session"), KTraceFileName, __LINE__);
                        iSessionObserver.TerminateSession(this, KErrNone);
                        }
                    }
                }	
            }
        //Start timer without waiting for client's Notify request
        if (iNumFixes != 0)
            StartPeriodicTriggerTimer();
        } 

    //For NI request, we don't get notify request
    if (iNumFixes != 0 && iRequestType == ESUPL_NETWORK)
        {            
        iClientNotify = ETrue;
        StartPeriodicTriggerTimer();
        LaunchNotifyDialogL();
        //UpdateActiveTriggerSession(); //Update UI for trigger info...
    		}
    		
    if (iNumFixes == 0 && iRequestType == ESUPL_NETWORK)
        {            
        LaunchNotifyDialogL();
        }
}

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplTriggerStopL
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplTriggerStopL()
    {
    iTrace->Trace(_L("COMASuplSession::ProcessSuplTriggerStopL..."), KTraceFileName, __LINE__); 							

    // Recreate SUPL STATE
    delete iSuplState;
    iSuplState = NULL;
    iTrace->Trace(_L("After state delete ProcessSuplTriggerStopL..."), KTraceFileName, __LINE__); 							

    iSuplState = COMASuplTriggerStopState::NewL(iOMASuplAsnHandlerBaseImpl);
    iSuplState->SetVersion(iSuplVersion);
    TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	

    LogEndTime(); //Log end time if QoP & delay in Qop is present.

        // For all requests, send SUPL END                    
        {            
        iErrorStatusCode = COMASuplEnd::EVer2SessionStopped;
        GenerateSuplEndL();
        }
    }
// -----------------------------------------------------------------------------
// COMASuplSession::UpdateSuplSessionIDL
//  Update SLP Sesssion ID with rececived slp id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::UpdateSuplSessionIDL()	
    {
    iTrace->Trace(_L("COMASuplSession::UpdateSuplSessionIDL"), KTraceFileName, __LINE__); 		
    COMASuplSessionID* retrivedSessionID = NULL;

    if(!iDecodedAsnMessage)
        return;

    iDecodedAsnMessage->SessionId(retrivedSessionID);
    UpdateSLPSessionIDL(retrivedSessionID);
    }

// -----------------------------------------------------------------------------
// COMASuplSession::UpdateSessionID
// Update SLP Sesssion ID with received session ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::UpdateSLPSessionIDL(COMASuplSessionID *aSessionID)
    {
    iTrace->Trace(_L("Start of COMASuplSession::UpdateSLPSessionIDL"), KTraceFileName, __LINE__); 		
    if(aSessionID)
        {
        COMASuplSLPSessionID* suplSLPID = NULL;
        aSessionID->SLPSessionID(suplSLPID);

        if(suplSLPID)
            {
            TBuf8<KSLPSessionID> sessionId;
            COMASuplSLPSessionID* slpNewSessionId  = COMASuplSLPSessionID::NewL();		
            CleanupStack::PushL(slpNewSessionId);
            COMASuplSLPAddress* slpNewIPAddress = COMASuplSLPAddress::NewL();
            CleanupStack::Pop(slpNewSessionId);

            COMASuplSLPAddress* slpIPAddress = NULL;
            suplSLPID->SuplSLPSessionID(sessionId,slpIPAddress);
            slpNewSessionId->SetSuplSLPSessionID(sessionId,slpNewIPAddress);

            if(slpIPAddress)
                {	
                COMASuplSLPAddress::TSuplSLPAddressType addressType = slpIPAddress->SLPAddressType();
                if(addressType == COMASuplSLPAddress::EFqdn)
                    {
                    HBufC8* fqdn = NULL;
                    TInt err = slpIPAddress->Fqdn(fqdn);
                    if(err == KErrNone)
                        {
                        //Ownership transfer towords COMASuplSLPAddress
                        HBufC8* newFqdn = fqdn->AllocL();
                        slpNewIPAddress->SetFqdn(newFqdn);
                        }
                    }

                if(  addressType == COMASuplSLPAddress::EIPv4Address )	
                    {
                    TBuf8<KIPv4Address> ipAddress;
                    TInt err = slpIPAddress->IPvAddress(ipAddress);
                    if(err == KErrNone)
                        slpNewIPAddress->SetIPv4Address(ipAddress);
                    }

                if(addressType == COMASuplSLPAddress::EIPv6Address )	
                    {
                    TBuf8<KIPv6Address> ipAddress;
                    TInt err = slpIPAddress->IPvAddress(ipAddress);
                    if(err == KErrNone)
                        slpNewIPAddress->SetIPv6Address(ipAddress);
                    }
                }

            iSuplSessionId->SetSLPSessionID(slpNewSessionId);	
            }

        ///////////log //////////////////

#if defined(_DEBUG)  //This is only for logging purpose. 
        iTrace->Trace(_L("After UpdateSuplSessionIDL"), KTraceFileName, __LINE__); 		
        TBuf<128> msg; 
        COMASuplSETSessionID* suplSETID = NULL;
        iSuplSessionId->SETSessionID(suplSETID);

        COMASuplSLPSessionID* suplSlpID = NULL;
        iSuplSessionId->SLPSessionID(suplSlpID);

        //SET Part			
        if(suplSETID)
            {
            msg.Copy(_L("SET Session ID : "));
            TBuf<4> id;	
            TInt      SETSessionID;
            TBuf8<32> IPvAddress8;
            TBuf<32>  ipAddress;
            suplSETID->SuplSETSessionID(SETSessionID,IPvAddress8);	

            TLex8 lex(IPvAddress8);
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

            id.AppendNum(SETSessionID);
            msg.Append(id);
            msg.Append(_L("  IPAddress/IMSI :"));
            msg.Append(ipAddress);
            }
        else
            {
            msg.Copy(_L("SET Session Id is not set."));
            }

        iTrace->Trace(msg,KTraceFileName, __LINE__); 							

        //SLP Part			
        if(suplSlpID)
            {
            msg.Copy(_L("SLP Session ID : "));
            TBuf8<4> sessionId8;
            TBuf<32> sessionId;
            TBuf8<64> ipAddress8(_L8("No Address field in SLP Session ID"));				
            TBuf<64> ipAddress;

            COMASuplSLPAddress* slpIPAddress = NULL;
            suplSlpID->SuplSLPSessionID(sessionId8,slpIPAddress);

            if(slpIPAddress)
                {
                COMASuplSLPAddress::TSuplSLPAddressType addrType = slpIPAddress->SLPAddressType();
                TInt err;
                HBufC8 *fqdnAddr;

                if (addrType == COMASuplSLPAddress::EFqdn)
                    {                                    
                    iTrace->Trace(_L("SLP Address is of type FQDN"),KTraceFileName, __LINE__); 								
                    err = slpIPAddress->Fqdn(fqdnAddr);	
                    }
                else
                    {
                    iTrace->Trace(_L("SLP Address is of type IPv*"),KTraceFileName, __LINE__); 								
                    err = slpIPAddress->IPvAddress(ipAddress8);	
                    }                                    

                if(err != KErrNone)
                    {
                    msg.Copy(_L("SLP Address is not set."));
                    iTrace->Trace(msg,KTraceFileName, __LINE__); 								
                    return;
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
            for(i = 0; i < 4; ++i)	
                {
                chr = lex1.Get();
                TUint num = chr;
                sessionId.Append(chr);
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
        ////////////////////////////////			
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::UpdateSETSessionID
// Update SET Sesssion ID with received session ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::UpdateSETSessionIDL(COMASuplSessionID *aSessionID)
    {

    iTrace->Trace(_L("Start of COMASuplSession::UpdateSETSessionIDL"), KTraceFileName, __LINE__); 		
    if(aSessionID)
        {
        COMASuplSETSessionID* retrivedSETSessionId = NULL;
        aSessionID->SETSessionID(retrivedSETSessionId);

        if(retrivedSETSessionId)
            {
            COMASuplSETSessionID::TSuplSETIDType addressType = retrivedSETSessionId->SuplSETIDType();

            if(addressType == COMASuplSETSessionID::EIPv4Address)
                {
                TInt sessionUniqueNo;
                TBuf8<KIP4AddressLength>  IPvAddress;
                retrivedSETSessionId->SuplSETSessionID(sessionUniqueNo,IPvAddress);

                COMASuplSETSessionID* suplSETSessionId = NULL;
                iSuplSessionId->SETSessionID(suplSETSessionId);
                suplSETSessionId->SetSuplSETSessionIDIPv4(sessionUniqueNo,IPvAddress);
                iSessionIDFlag = ETrue;
                }
            else if(addressType == COMASuplSETSessionID::EIPv6Address)
                {
                TInt sessionUniqueNo;
                TBuf8<KIP6AddressLength>  IPvAddress;
                retrivedSETSessionId->SuplSETSessionID(sessionUniqueNo,IPvAddress);

                COMASuplSETSessionID* suplSETSessionId = NULL;
                iSuplSessionId->SETSessionID(suplSETSessionId);
                suplSETSessionId->SetSuplSETSessionIDIPv6(sessionUniqueNo,IPvAddress);
                iSessionIDFlag = ETrue;
                }
            else    // IMSI
                {
                TInt sessionUniqueNo;
                TBuf8<KIMSILength>  IMSI;
                retrivedSETSessionId->SuplSETSessionID(sessionUniqueNo, IMSI);

                COMASuplSETSessionID* suplSETSessionId = NULL;
                iSuplSessionId->SETSessionID(suplSETSessionId);
                suplSETSessionId->SetSuplSETSessionIDIMSI(sessionUniqueNo, IMSI);
                iSessionIDFlag = ETrue;
                }
            }
        } 
    iTrace->Trace(_L("End of COMASuplSession::UpdateSETSessionIDL"), KTraceFileName, __LINE__); 			
    }

// -----------------------------------------------------------------------------
// COMASuplSession::CreateCloneMessageL
// Creates Clone of COMASuplAsnMessageBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::CreateCloneMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage)	
    {		
    iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL"), KTraceFileName, __LINE__); 		
    delete iDecodedAsnMessage; 
    iDecodedAsnMessage = NULL;

    COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();
    TInt error;
    switch(messageType)
        {
        case COMASuplAsnMessageBase::ESUPL_START:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_START"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplStart::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;
        case COMASuplAsnMessageBase::ESUPL_RESPONSE:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_RESPONSE"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplResponse::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;
        case COMASuplAsnMessageBase::ESUPL_POSINIT:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_POSINIT"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplPosInit::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;	                
        case COMASuplAsnMessageBase::ESUPL_POS:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : SUPL_POS"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplPos::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;

        case COMASuplAsnMessageBase::ESUPL_END:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_END"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplEnd::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;
        case COMASuplAsnMessageBase::ESUPL_INIT:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_INIT"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplInitVer2::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;
        case COMASuplAsnMessageBase::ESUPL_AUTH_RESP:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_AUTH_RESP"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplAuthResponse::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;
        case COMASuplAsnMessageBase::ESUPL_AUTH_REQ:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_AUTH_RESP"), KTraceFileName, __LINE__); 		
            iDecodedAsnMessage = COMASuplAuthRequest::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;

        case COMASuplAsnMessageBase::ESUPL_TRIG_START:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_TRIG_START"), KTraceFileName, __LINE__);
            iDecodedAsnMessage = COMASuplTriggeredStart::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;

        case COMASuplAsnMessageBase::ESUPL_TRIG_RESPONSE:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL:ESUPL_TRIG_RESPONSE"), KTraceFileName, __LINE__);
            iDecodedAsnMessage = COMASuplTriggeredResponse::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;

        case COMASuplAsnMessageBase::ESUPL_TRIG_STOP:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL:ESUPL_TRIG_STOP"), KTraceFileName, __LINE__);
            iDecodedAsnMessage = COMASuplTriggeredStop::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;

        case COMASuplAsnMessageBase::ESUPL_REPORT:
            iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL:ESUPL_REPORT"), KTraceFileName, __LINE__);
            iDecodedAsnMessage = COMASuplReport::NewL(); 
            iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
            break;
        default:
            break;	
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::CancelSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::CancelSession()
    {
    iTrace->Trace(_L("COMASuplSession::CancelSession"), KTraceFileName, __LINE__); 		
    CancelAllOperations();
    iConnRequestor->CancelReadOperation();

    switch(iSuplSessionState)
        {
        case ESUPL_INITIALIZED:
        case ESUPL_CONNECTING:
        case ESUPL_SESSION_ERROR:
            {
            iSuplSessionState = ESUPL_SESSION_ERROR;
            break;
            }

        case ESUPL_RECEIVED:
            {	
            if (iSuplMsgType == ESUPL_TRIG_RESPONSE)                            
                {
                }
            else
                {                        
                iSuplSessionState = ESUPL_SESSION_ERROR;
                if(iSuplMsgType == ESUPL_END)
                    {
                    if(iOMASuplPOSRequestor)
                        iOMASuplPOSRequestor->Cancel();
                    }
                }
            break;
            }

        case ESUPL_GENERATE:
        case ESUPL_ENCODE:
            {
            if(iSuplMsgType == ESUPL_START)	//No need to send SUPL_END to SLP
                {
                iSuplSessionState = ESUPL_SESSION_ERROR;
                break;
                }
            }
        case ESUPL_SEND:
            {
            iErrorStatusCode = COMASuplEnd::EUnspecified;
            iSuplSessionState = ESUPL_GENERATE;
            iSuplMsgType = ESUPL_END;
            if(iOMASuplPOSRequestor) //Reset observer
                {
                iOMASuplPOSRequestor->SetObserver(NULL);	
                }
            TRAP_IGNORE(GenerateSuplMessageL());				
            //iSuplSessionState = ESUPL_SESSION_ERROR; Cancel Bug
            break;
            }
        default: break;
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::CancelRunSession()
    {
    CancelSession();
    if(iSuplSessionState == ESUPL_SEND)
        {
        CompleteSession(KErrCancel);
        }
    else
        {
        //Close Connection : bug Fix.
        iConnRequestor->CloseConnection();
        iPortNum = 0;
        iSessionObserver.TerminateSession(this, KErrCancel);	
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::HandleGenerationErrorL
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::HandleGenerationErrorL(TInt aErr)
    {
    TBool cancelSendRequestor  = ETrue;
    if(iSuplState)
        {
        switch(iSuplState->GetSuplState())
            {
            case COMASuplState::ESUPL_START:
                {
                break;
                }
            case COMASuplState::ESUPL_POS:
            case COMASuplState::ESUPL_POS_INIT:
                {
                iErrorStatusCode = COMASuplEnd::EUnspecified;
                if(aErr == KErrSuplposMethodMismatch)
                    iErrorStatusCode = COMASuplEnd::EPosProtocolMismatch;//COMASuplEnd::EPosMethodMismatch;
                cancelSendRequestor = EFalse;
                iSuplSessionState = ESUPL_GENERATE;
                iSuplMsgType = ESUPL_END;
                GenerateSuplMessageL();
                break;		
                }
            case COMASuplState::ESUPL_END:
                {
                //Close Connection : bug Fix.
                iConnRequestor->CloseConnection();
                    iPortNum = 0;
                iSessionObserver.TerminateSession(this, aErr);
                cancelSendRequestor = EFalse;
                break;
                }
            default: break;
            }
        }

    if(cancelSendRequestor)
        {
        CancelAllOperations();
        CleanUp();
        CompleteSession(aErr);
        }

    }
// -----------------------------------------------------------------------------
// COMASuplSession::HandleInitilizationErrorL
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//		
void COMASuplSession::HandleInitilizationErrorL(TInt aErr)
    {
    CancelAllOperations();
    CleanUp();
    CompleteSession(aErr);
    }
	
// -----------------------------------------------------------------------------
// COMASuplSession::HandleConnectionErrorL
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::HandleConnectionErrorL(TInt aErr)
    {
    CancelAllOperations();
    //Close Connection
    iConnRequestor->CloseConnection();
        iPortNum = 0;
    iSessionObserver.TerminateSession(this, aErr);
    }
	
// -----------------------------------------------------------------------------
// COMASuplSession::HandleEncodingErrorL
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::HandleEncodingErrorL(TInt aErr)
    {
    TBool cancelSendRequestor  = ETrue;
    if(iSuplState)
        {
        iErrorStatusCode = COMASuplEnd::EUnspecified;					

        if(aErr == KErrOMASETCapMisMatch )
            {
            iErrorStatusCode = COMASuplEnd::EPosMethodMismatch;
            iFailedCaps = iAllowedCapabilities;
            iCapsFail = ETrue;
            }

        if(aErr == KErrSuplposMethodMismatch)
            {
            iErrorStatusCode = COMASuplEnd::EPosProtocolMismatch;
            }
        if(aErr == KErrOMAManParamMissing)
            {
            iErrorStatusCode = COMASuplEnd::EDataMissing;
            }

        if(aErr == KErrArgument)
            {
            iErrorStatusCode = COMASuplEnd::EUnexpectedDataValue;
            }

        if(aErr == KErrOMASuplPosInfo || aErr == KErrOMASuplPosInActive || aErr == KErrOMASuplDataMissing)
            {
            iErrorStatusCode = COMASuplEnd::EDataMissing;
            }

        switch(iSuplState->GetSuplState())
            {
            case COMASuplState::ESUPL_START:break;
            case COMASuplState::ESUPL_POS: 
                {
                //This is for,even though we gets error(no pos payload data) for first POS message from plugin
                // Just ignore it ... Don't put new pending request for POS message plugin.
                if(iIsFirstPOSMessage && ( aErr == KErrOMAManParamMissing || aErr ==  KErrOMASuplParamNotSet || aErr == KErrOMASuplPosInfo))
                    {
                    iTrace->Trace(_L("Failed to Generate First messsage from POS...still continueing.."), KTraceFileName, __LINE__); 		
                    iIsFirstPOSMessage = EFalse;
                    return;
                    }
                }
			//Comment to ignore coverity missing break error
			//coverity[MISSING_BREAK  :FALSE]
    		       
            case COMASuplState::ESUPL_POS_INIT:
                {
                cancelSendRequestor = EFalse;
                iSuplSessionState = ESUPL_GENERATE;
                iSuplMsgType = ESUPL_END;
                GenerateSuplMessageL();
                break;			
                }

            case COMASuplState::ESUPL_END:
                {
                iConnRequestor->CloseConnection();
                    iPortNum = 0;
                iSessionObserver.TerminateSession(this, KErrGeneral);
                return;
                }

            default: break;
            }
        }

    if(cancelSendRequestor)
        {
        CancelAllOperations();
        CleanUp();
        CompleteSession(KErrGeneral);
        }

    }
// -----------------------------------------------------------------------------
// COMASuplSession::HandleSendErrorL
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//		
void COMASuplSession::HandleSendErrorL(TInt aErr)
    {
    // if error code is KErrDisconnected, then close connection
    // and set the state accordingly            
    if (aErr == KErrDisconnected)
        {
		//Close Connection
		iConnRequestor->CloseConnection();
		iSessionObserver.TerminateSession(this, aErr);
        iDisconnected = ETrue;
        }
    else
        {                
    TBool cancelSendRequestor  = ETrue;
    if(aErr == KErrTimedOut)				
        {
        iErrorStatusCode = COMASuplEnd::EUnspecified;
        cancelSendRequestor = EFalse;
        iSuplSessionState = ESUPL_GENERATE;
        iSuplMsgType = ESUPL_END;
        GenerateSuplMessageL();				
        }

    if(aErr == KErrSuplposMethodMismatch)				 
        {
        iErrorStatusCode = COMASuplEnd::EPosProtocolMismatch;
        cancelSendRequestor = EFalse;
        iSuplSessionState = ESUPL_GENERATE;
        iSuplMsgType = ESUPL_END;
        GenerateSuplMessageL();				
        }

    aErr = KErrCompletion;

    if(cancelSendRequestor)
        {
        CancelAllOperations();
        CleanUp();
        CompleteSession(aErr);
		}
    }

    }
// -----------------------------------------------------------------------------
// COMASuplSession::HandlePacketsErrorL
// HandlePacketsErrorL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//		
void COMASuplSession::HandlePacketsErrorL(TInt aErr)
    {
    TBool cancelSendRequestor  = ETrue;

    if(iSuplMsgType == ESUPL_INIT)
        {
        HandleSuplInitErrorL(aErr);
        return;
        }

    if(aErr == KErrSuplEndProtocolError )
        {
        aErr = KErrCompletion;
        switch(iSuplMsgType)
            {
            case ESUPL_START: 
            case ESUPL_RESPONSE:
            case ESUPL_END:
                {
                //Close Connection
                iConnRequestor->CloseConnection();
                iPortNum = 0;
                iSessionObserver.TerminateSession(this, aErr);
                break;
                }
            case ESUPL_REPORT: 
                {
                //Close Connection
                iConnRequestor->CloseConnection();
                break;
                }
            default: break;
            }
        }

    if(aErr == KErrOMASuplProtocolError )
        {
        aErr = KErrCompletion;
        switch(iSuplMsgType)
            {
            case ESUPL_START: 
            case ESUPL_INIT:
            case ESUPL_POS:
            case ESUPL_RESPONSE:
            case ESUPL_POSINIT:
            case ESUPL_TRIG_START:    
            case ESUPL_AUTH_RESP:
                {
                iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
                cancelSendRequestor = EFalse;
                iSuplSessionState = ESUPL_GENERATE;
                iSuplMsgType = ESUPL_END;
                GenerateSuplMessageL();				
                break;			
                }

            case ESUPL_END:
                {
                //Close Connection
                iConnRequestor->CloseConnection();
                iPortNum = 0;
                iSessionObserver.TerminateSession(this, aErr);
                break;
                }

            default: break;
            }
        }

    if( aErr == KErrOMANonProxyModeNotSupported) //aErr == KErrOMASuplVersionNotMatching ||
        {
        iErrorStatusCode = COMASuplEnd::ENonProxyModeNotSupported;

        aErr = KErrCompletion;

        if(iSuplState)
            {
            switch(iSuplState->GetSuplState())
                {
                case COMASuplState::ESUPL_RESPONSE:
                    {
                    cancelSendRequestor = EFalse;
                    iSuplSessionState = ESUPL_GENERATE;
                    iSuplMsgType = ESUPL_END;
                    GenerateSuplMessageL();
                    break;
                    }
                default: break;
                }
            }
        }

    if(cancelSendRequestor)
        {
        CancelAllOperations();
        CleanUp();
        CompleteSession(aErr);
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::HandleSuplInitErrorL
// Handle Init related error
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleSuplInitErrorL(TInt aErr)
    {
    iTrace->Trace(_L("Start COMASuplSession::HandleSuplInitError"), KTraceFileName, __LINE__); 

    if(aErr == KErrOMAInvalidHSLPAddress)
        {
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError HSLP Address is not matching..."), KTraceFileName, __LINE__);
        //Close Connection : bug Fix.
        iConnRequestor->CloseConnection();
                iPortNum = 0;
        iSessionObserver.TerminateSession(this, KErrCompletion);
        return;
        }

    TBool setStatusCode = ETrue;


    if(aErr == KErrSuplInvalidSessionID)
        {
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Received SET Session ID"), KTraceFileName, __LINE__);
        iErrorStatusCode = COMASuplEnd::EInvalidSessionId;	

        COMASuplSessionID* retrivedSessionID = NULL;
        iDecodedAsnMessage->SessionId(retrivedSessionID);

        // Set the SessionId.
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Update SET Session ID"), KTraceFileName, __LINE__);
        UpdateSETSessionIDL(retrivedSessionID);

        }

    if(aErr == KErrOMANonProxyModeNotSupported)
        {
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Received Nonproxymode data"), KTraceFileName, __LINE__);
        iErrorStatusCode = COMASuplEnd::ENonProxyModeNotSupported;	
        }

    if(KErrAccessDenied == aErr || KErrOMASuplDenied == aErr)
        {
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError KErrAccessDenied|| KErrOMASuplDenied"), KTraceFileName, __LINE__);
        iErrorStatusCode = COMASuplEnd::EConsentDeniedByUser;	
        }

    if(aErr == KErrOMASuplNoPosition || KErrOMASuplInformationOnly == aErr)
        {
        //Supl End with No Position
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError KErrOMASuplNoPosition || KErrOMASuplInformationOnly"), KTraceFileName, __LINE__);
        setStatusCode = EFalse;
        }

    if(KErrOMASuplAccepted == aErr)
        {
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError EConsentGrantedByUser"), KTraceFileName, __LINE__);
        iErrorStatusCode = COMASuplEnd::EConsentGrantedByUser;	
        }

    if (KErrSuplServiceNotSupported == aErr)    
        {
        iTrace->Trace(_L("COMASuplSession::HandleSuplInitError E"), KTraceFileName, __LINE__);
        iErrorStatusCode = COMASuplEnd::EVer2ServiceNotSupported;	
        }
    TInt len = iHSLPAddress.Length();
    HBufC8 *hslpAdress = NULL;
    if(len > 0)
        {
        hslpAdress = HBufC8::NewL(iHSLPAddress.Length());
        hslpAdress->Des().Copy(iHSLPAddress);
        }

    else if(len ==0)
        {
		CServerParams* serverParams = CServerParams::NewL();
		CleanupStack::PushL(serverParams);
		
		if (iSuplStorageSettings->GetDefaultServer(serverParams) == KErrNotFound )
			{
	        iTrace->Trace(_L("HSLP generated frm IMSI"), KTraceFileName, __LINE__);
	        hslpAdress = HBufC8::NewL(iSuplSettings->SLPAddressfromImsi().Length());
	        CleanupStack::PushL(hslpAdress);
	        hslpAdress->Des().Copy(iSuplSettings->SLPAddressfromImsi());
	        CleanupStack::Pop(hslpAdress);	
			}
		else
			{
			iTrace->Trace(_L("Default HSLP"), KTraceFileName, __LINE__);
			TInt64 slpId;
			TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
			HBufC* serverAddr = HBufC::NewL(KMaxHSLPAddrLen);
			HBufC* iapName = HBufC::NewL(KMaxIAPLen);
			CleanupStack::PushL(serverAddr);
			CleanupStack::PushL(iapName);
			serverParams->Get(slpId,serverAddr->Des(),iapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
			hslpAdress = HBufC8::NewL(serverAddr->Length());
			hslpAdress->Des().Copy(*serverAddr);
		
			CleanupStack::PopAndDestroy(iapName);
			CleanupStack::PopAndDestroy(serverAddr);
			}
		CleanupStack::PopAndDestroy(serverParams);

        }
    CleanupStack::PushL(hslpAdress);

    delete iSuplState;
    iSuplState = NULL;

    if(setStatusCode)
        {
        iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress);		
        }
    else
        {
        iSuplState = COMASuplEndState::NewL(iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress );
        } 		

    CleanupStack::PopAndDestroy(hslpAdress);

    iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Send SUPL_END for SUPL_INIT"), KTraceFileName, __LINE__);

	
    iSuplSessionState = ESUPL_INITIALIZED;
    iSuplState->SetMsgStateObserver(this);
    iSuplMsgType = ESUPL_END;
    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
    iSuplState->GenerateMessageL();
    iSuplSessionState = ESUPL_GENERATE;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidParameterErrorL
// Handles error occured related invalid parameter.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleInvalidParameterErrorL(COMASuplAsnMessageBase* aDecodedAsnMessage)
    {
    iTrace->Trace(_L("COMASuplSession::HandleInvalidParameterErrorL"), KTraceFileName, __LINE__);
    if(CheckProtocolVersionL(aDecodedAsnMessage)) //Checks version with supported version
        {
        COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();
        CancelAllOperations();	
        if(messageType == COMASuplAsnMessageBase::ESUPL_END)
            {
            //Close Connection : bug Fix.
            iConnRequestor->CloseConnection();
            iPortNum = 0;
            iSessionObserver.TerminateSession(this, KErrCompletion);
            }
        else
            {

            iErrorStatusCode = COMASuplEnd::EUnexpectedDataValue;

            if((messageType == COMASuplAsnMessageBase::ESUPL_POSINIT || 
                    messageType == COMASuplAsnMessageBase::ESUPL_START || 
                    messageType == COMASuplAsnMessageBase::ESUPL_RESPONSE) 
                    && (iRequestType == ESUPL_NETWORK))
                {
                iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
                }

            if(iRequestType == ESUPL_TERMINAL )					//Protocol errors
                {
                if( !	(iSuplMsgType == ESUPL_START && messageType == COMASuplAsnMessageBase::ESUPL_RESPONSE)
                        || 				
                        (iSuplMsgType == ESUPL_POSINIT && messageType == COMASuplAsnMessageBase::ESUPL_POS)
                )		

                    {
                    iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
                    }
                }

            iSuplSessionState = ESUPL_GENERATE;
            iSuplMsgType = ESUPL_END;
            GenerateSuplMessageL();				
            }
        }	
    }
// -----------------------------------------------------------------------------
// COMASuplSession::TerminateSession
// Terminates session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::TerminateSession()
    {
    iTrace->Trace(_L("COMASuplSession::TerminateSession"), KTraceFileName, __LINE__);
    if(iSuplSessionState != ESUPL_SESSION_ERROR)
        {
        CancelRunSession();
        }
    else
        {
        SessionEnd();
        CompleteSession(KErrCancel);
        }
    return;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SuplVersion
// returns SUPL Version
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal COMASuplSession::SuplVersion() 
    {
    return KSuplPOSPluginVersion;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SetPOSMsgPluginState
// Informs Protocol state to POS if present
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::SetPOSMsgPluginState(COMASuplPosSessionBase::TOMASuplMsgState aSuplMsgState)
    {
    if(iPOSSession != NULL)
        {
        iTrace->Trace(_L("Setting State of POS Msg Plugin"), KTraceFileName, __LINE__); 															

        COMASuplPosSessionBase::TOMASuplMsg SuplMsg = COMASuplPosSessionBase::EOMASuplStart;

        switch(iSuplMsgType) 
            {
            case ESUPL_START:
                SuplMsg = COMASuplPosSessionBase::EOMASuplStart;	
                break;
            case ESUPL_RESPONSE:
                SuplMsg = COMASuplPosSessionBase::EOMASuplResponse;	
                break;			
            case ESUPL_INIT:
                SuplMsg = COMASuplPosSessionBase::EOMASuplInit;	
                break;			
            case ESUPL_POSINIT:
                SuplMsg = COMASuplPosSessionBase::EOMASuplPosInit;	
                break;			
            case ESUPL_POS:
                SuplMsg = COMASuplPosSessionBase::EOMASuplPos;	
                break;			
            case ESUPL_END:
                SuplMsg = COMASuplPosSessionBase::EOMASuplEnd;				
                break;			
            default:			
                break;
            }

        iPOSSession->SetSuplState(SuplMsg,aSuplMsgState); 
        }

    }
// -----------------------------------------------------------------------------
// COMASuplSession::IsLastPOSMessage
// Checks about last POS message with POS handler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TBool COMASuplSession::IsLastPOSMessage()
    {
    if( iPOSSession && iSuplState)
        {
        COMASuplPosState* state =  static_cast <COMASuplPosState *>(iSuplState);
        TBool ret = state->IsLastMessage();
        return ret;
        }
    else 
        {
        return EFalse;	
        }
    }
// -----------------------------------------------------------------------------
// COMASuplSession::IsSegmentedDone()
// Checks about segmentation of pospayload
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TBool COMASuplSession::IsSegmentationDone()
    {
    if( iPOSSession && iSuplState)
        {
        COMASuplPosState* state =  static_cast <COMASuplPosState *>(iSuplState);
        TBool ret = state->IsSegmentationDone();
        return ret;
        }
    else 
        {
        return EFalse;	
        }
    }


// -----------------------------------------------------------------------------
// COMASuplSession::CopySETCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::CopySETCapabilities()	 
    {
    if(iSuplMsgType == ESUPL_START)
        {
        COMASuplStartState *startState = static_cast <COMASuplStartState *>(iSuplState);
        COMASuplSETCapabilities *setCaps = startState->Capabilities();

        TOMASuplPosTechnology posTech;
        COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
        TOMASuplPosProtocol posProtocol;

        setCaps->GetSETCapabilities(posTech,prefMethod,posProtocol);

        iSETCapabilities->SetSETCapabilities(posTech,prefMethod, posProtocol);
        }

    if (iTriggerSession && iSuplMsgType == ESUPL_TRIG_START)
        {
        COMASuplTriggerStartState *trgStartState = static_cast <COMASuplTriggerStartState *>(iSuplState);
        COMASuplSETCapabilities *setCaps = trgStartState->Capabilities();

        TOMASuplPosTechnology posTech;
        COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
        TOMASuplPosProtocol posProtocol;

        setCaps->GetSETCapabilities(posTech,prefMethod,posProtocol);

        iSETCapabilities->SetSETCapabilities(posTech,prefMethod, posProtocol);
        }

    if(iSuplMsgType == ESUPL_POSINIT && iRequestType == ESUPL_NETWORK && !iTriggerSession)
        {
        TOMASuplPosTechnology posTechnology;
        COMASuplSETCapabilities::TOMASuplPrefMethod    prefMethod;
        TOMASuplPosProtocol   posProtocol;

        TBool  GpsSETAssisted 	= EFalse;
        TBool  GpsSETBased 		= EFalse;
        TBool  AutonomousGps 	= EFalse;
        TBool  FLT 			 	= EFalse;
        TBool  eOTD 		 	= EFalse;
        TBool  oTDOA 		 	= EFalse;

        TBool TIA801 = EFalse;
        TBool RRLP = EFalse;
        TBool RRC =  EFalse;

        posTechnology.SetPosTechnology(GpsSETAssisted,GpsSETBased,AutonomousGps,FLT,
                eOTD,oTDOA);
        posProtocol.SetPosProtocol(TIA801,RRLP,RRC);
        prefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
        iSETCapabilities->SetSETCapabilities(posTechnology,prefMethod,posProtocol);	
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::PrivacyCheckCompleteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::PrivacyCheckCompleteL(TInt aCompletionCode)
    {
    if(aCompletionCode == KErrNone)
        {
        iTrace->Trace(_L("COMASuplSession::PrivacyCheckCompleteL No Error"), KTraceFileName, __LINE__);
				static_cast<COMASuplInitState*>(iSuplState)->GetRequestorIDAndName(iRequestID,iRequestorName);  
				static_cast<COMASuplInitState*>(iSuplState)->GetPrivacyStatus(iPrivacyOverride); 
				static_cast<COMASuplInitState*>(iSuplState)->GetSuplPrivacyRequestInfo(iSuplReqInfo);
				
        iSuplSessionState = ESUPL_INITIALIZED;
        iSuplMsgType = ESUPL_POSINIT;
        if (iSuplUsage >= 2)
            {                        
            COMASuplInit* suplInit = static_cast <COMASuplInit *>(iDecodedAsnMessage);
            COMASuplNotification* notif;
            TInt error	= suplInit->Notification(notif);
            COMASuplNotification::TOMASuplNotificationType notifType = notif->SuplNotification();
            if (notifType == COMASuplNotification::ENotificationOnly)
                {                            
				        						InitializeL(iRequestID); //APE Centric 
                }
            else
                {                            
                        //iRequestID = 0;               //APE Centric 
                CheckForSuplUsageL();                            
                }
            }
        else
				    InitializeL(iRequestID);
        }
    else
        {
        iTrace->Trace(_L("COMASuplSession::PrivacyCheckCompleteL with Error "), KTraceFileName, __LINE__);
        if (iSuplUsage >= 2)                    
            {
            iNwInitError = ETrue;                            
            iNwInitCompletionCode = aCompletionCode;
            CheckForSuplUsageL();                            
            }
        else
            HandleSuplInitErrorL(aCompletionCode);
        }	
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SetConfigurationParameters...sets configuration parameters
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::SetConfigurationParameters(TInt& aUT1_StartTimer,TInt& aUT2_PosInitTimer,
 	 											TInt& aUT3_PosTimer,TInt& aPrivacyTimer, 
												TInt& aSETMode,
                                                TInt& aSuplUsage,
                                                TInt& aPersistFailTimer,
                                                TInt& aSuplInitTimeOut)
    {
    iUT1_StartTimer = aUT1_StartTimer;
    iUT2_PosInitTimer = aUT2_PosInitTimer;
    iUT3_PosTimer = aUT3_PosTimer;
    iPrivacyTimer = aPrivacyTimer;
    iSETMode = aSETMode;
    iSuplUsage = aSuplUsage;
    iPersistFailTimer = aPersistFailTimer; 
    iSuplInitTimeOut = aSuplInitTimeOut;
    }
	
// -----------------------------------------------------------------------------
// COMASuplSession::IsGenerationInProgress...check about status of generation of message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TBool COMASuplSession::IsGenerationInProgress()
    {
    if( iSuplMsgType == ESUPL_POSINIT && iSuplState)
        {
        COMASuplPosInitState* state =  static_cast <COMASuplPosInitState *>(iSuplState);
        TBool ret = state->IsGenerationInProgress();
        return ret;
        }
    else 
        {
        return EFalse;	
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::LogEndTime...log the end time if delay is present in QoP
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::LogEndTime()
	{
		if(!iIsQoPPresent) 
			{
				return;
			}
			
		TInt delay;
		if(iClientQop.GetDelay(delay) != KErrNotFound)
		{
			iSessionEndTime.HomeTime();
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::SetIMSIL
// Updates IMSI value in SET Session ID
// -----------------------------------------------------------------------------
void COMASuplSession::SetIMSI()
    {

        {
	    iTrace->Trace(_L("COMASuplSession::SetIMSIL Start"), KTraceFileName, __LINE__);
	    TInt length = iIMSI.Length();
        TBuf8<KIMSILength> imsi;	    
        TUint8 val;
        TInt index = 0;
      
        while ( index <= length )
            {
            val = 0;
            if (index == length)
                    {
                        val = 15 << 4;
                        val |= 15;
                        imsi.Append(val);
                        break;
                    }
            const TUint16& t = iIMSI[index];
            TInt8 i = t - 48;
            if ( ++index != length )
                {
                const TUint16& h = iIMSI[index];
    			TInt8 j = h - 48;
                val = j << 4;
                val |= i;
                }
            else
                {
                val = 15 << 4;
                val |= i;
                }
    		
            imsi.Append(val);
            index++;
            }
       
        if (!iSessionIDFlag)
        {                
        COMASuplSETSessionID* suplSETSessionId = NULL;
		iSuplSessionId->SETSessionID( suplSETSessionId );
		suplSETSessionId->SetSuplSETSessionIDIMSI( iSETSessionUniqueId, imsi );
        }
        
        iTrace->Trace(_L("COMASuplSession::SetIMSI End"), KTraceFileName, __LINE__);
        }
    }
			
// -----------------------------------------------------------------------------
// COMASuplSession::IsIMSIVariant
// Checks to determine if IMSI needs to be included in the SUPL message
// -----------------------------------------------------------------------------
TBool COMASuplSession::IsIMSIVariant()
    {
    if(iIMSI.Length() && 
    (
    (iRequestType == ESUPL_TERMINAL && iSuplMsgType == ESUPL_START) || 
    (iRequestType == ESUPL_TERMINAL && iSuplMsgType == ESUPL_TRIG_START) ||
    (iRequestType == ESUPL_NETWORK && iSuplMsgType == ESUPL_TRIG_START && iTriggerSession) ||
    (iRequestType == ESUPL_TERMINAL && iSuplMsgType == ESUPL_POSINIT && iTriggerSession) ||
    (iRequestType == ESUPL_NETWORK && iSuplMsgType == ESUPL_POSINIT) || 
    (iRequestType == ESUPL_NETWORK && iSuplMsgType == ESUPL_END)
    ))
        {
        return ETrue;
        }
    else
        {
        return EFalse;    
        }
    }


// -----------------------------------------------------------------------------
// COMASuplSession::SettingsUICompleted
// 
// -----------------------------------------------------------------------------

void COMASuplSession::SettingsUICompletedL(TInt /*aError*/)
	{
		iTrace->Trace(_L("COMASuplSession::SettingsUICompleted ..."), KTraceFileName, __LINE__);

	}


// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidMessageL
// Sends SUPL_END with status code with wrong message
// -----------------------------------------------------------------------------
void COMASuplSession::HandleInvalidMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage)
    {

    if(CheckProtocolVersionL(aDecodedAsnMessage)) //Checks version with supported version 
        {
        iTrace->Trace(_L("COMASuplSession::HandleInvalidMessageL...Sending SUPL_END for invalid message"), KTraceFileName, __LINE__);
        iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
        iSuplSessionState = ESUPL_GENERATE;
        iSuplMsgType = ESUPL_END;
        GenerateSuplMessageL();				
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidMessageLengthL
// Sends SUPL_END with status code with protocol error
// -----------------------------------------------------------------------------
void COMASuplSession::HandleInvalidMessageLengthL()
	{
			iTrace->Trace(_L("COMASuplSession::HandleInvalidMessageLengthL...Sending SUPL_END for invalid message length"), KTraceFileName, __LINE__);
			iErrorStatusCode = COMASuplEnd::EProtocolError;
			iSuplSessionState = ESUPL_GENERATE;
			iSuplMsgType = ESUPL_END;
			GenerateSuplMessageL();				
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::CheckProtocolVersion
// Returns true if its matching else false
// -----------------------------------------------------------------------------
TBool COMASuplSession::CheckProtocolVersionL(COMASuplAsnMessageBase* aDecodedAsn)
    {
    TOMASuplVersion version;
    COMASuplSessionID* SessionID = NULL;
    aDecodedAsn->MessageBase(version,SessionID);

    TInt recMajor,recMinor,recServInd;
    version.SuplVersion(recMajor,recMinor,recServInd);		

    if(recMajor == KSuplMajorVersion )
        {
        return  ETrue;
        } 
    else
        {
        COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsn->MessageType();
        if(messageType == COMASuplAsnMessageBase::ESUPL_END) 
            {
            iTrace->Trace(_L("Received message is SUPL_END...with wrong Version.Terminating session"), KTraceFileName, __LINE__); 
            iSessionObserver.TerminateSession(this, KErrCompletion);	
            }
        else
            {
            iTrace->Trace(_L("Received message with wrong version."), KTraceFileName, __LINE__); 

            delete iSuplState;
            iSuplState = NULL;
            iSuplMsgType = ESUPL_END;
            iErrorStatusCode = COMASuplEnd::EVersionNotSupported;	
            SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	

            if(iRequestType == ESUPL_NETWORK )
                {
                if (messageType == COMASuplAsnMessageBase::ESUPL_INIT)    
                   {                                
                	iSuplSessionState = ESUPL_INITIALIZED;
                	TInt len = iHSLPAddress.Length();
                	HBufC8 *hslpAdress = NULL;
                	if(len > 0)
                    	{
                    	hslpAdress = HBufC8::NewL(iHSLPAddress.Length());
                    	hslpAdress->Des().Copy(iHSLPAddress);
                    	}
                	else if(len ==0)
                    	{
						CServerParams* serverParams = CServerParams::NewL();
						CleanupStack::PushL(serverParams);
					
						if (iSuplStorageSettings->GetDefaultServer(serverParams) == KErrNotFound )
							{
	                    	iTrace->Trace(_L("Length of HSLP Address is = 0, passing the HSLP generated frm IMSI"), KTraceFileName, __LINE__);
	                    	hslpAdress = HBufC8::NewL(iSuplSettings->SLPAddressfromImsi().Length());
	                    	hslpAdress->Des().Copy(iSuplSettings->SLPAddressfromImsi());	
							}
							else
							{
							iTrace->Trace(_L("Sending End with ver for Default HSLP"), KTraceFileName, __LINE__);
							TInt64 slpId;
            				TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
							HBufC* serverAddr = HBufC::NewL(KMaxHSLPAddrLen);
							HBufC* iapName = HBufC::NewL(KMaxIAPLen);
							CleanupStack::PushL(serverAddr);
							CleanupStack::PushL(iapName);
							serverParams->Get(slpId,serverAddr->Des(),iapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
							hslpAdress = HBufC8::NewL(serverAddr->Length());
							hslpAdress->Des().Copy(*serverAddr);
			
							CleanupStack::PopAndDestroy(iapName);
							CleanupStack::PopAndDestroy(serverAddr);
							}
						CleanupStack::PopAndDestroy(serverParams);
                    	}

			    delete iSuplState;
			    iSuplState = NULL;
                CleanupStack::PushL(hslpAdress);
                ServerAddressCheckForSuplInitL();
                iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress);		
                CleanupStack::PopAndDestroy(hslpAdress);

                // Set the SessionId.
                iTrace->Trace(_L("COMASuplSession::CheckProtocolVersionL Update SLP Session ID"), KTraceFileName, __LINE__);
                UpdateSLPSessionIDL(SessionID);

                iTrace->Trace(_L("COMASuplSession::CheckProtocolVersionL Update SET Session ID"), KTraceFileName, __LINE__);
                UpdateSETSessionIDL(SessionID);
                iSuplState->SetMsgStateObserver(this);
                iSuplState->GenerateMessageL();
                iSuplSessionState = ESUPL_GENERATE;
                            }
                        else
                            {                                
					        iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);		
						    UpdateSLPSessionIDL(SessionID);
						    UpdateSETSessionIDL(SessionID);
						    iSuplState->SetMsgStateObserver(this);
                            iRequestType = ESUPL_INVALID_SESSION;
						    iSuplSessionState = ESUPL_GENERATE;
						    iSuplState->GenerateMessageL();
                            }

                } 
            else 							
                {
                iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
                iSuplSessionState = ESUPL_ENCODE;
                iSuplState->SetMsgStateObserver(this);
                UpdateSLPSessionIDL(SessionID);
                UpdateSETSessionIDL(SessionID);
                iSuplState->GenerateMessageL();
                }
            }	

        return EFalse;
        }	

    }

// -----------------------------------------------------------------------------
// COMASuplSession::GetRemainingPosPayloadL
// Returns remaining pos payload
// -----------------------------------------------------------------------------
COMASuplPosPayload* COMASuplSession::GetRemainingPosPayloadL()
    {
    if( iSuplState && COMASuplState::ESUPL_POS_INIT == iSuplState->GetSuplState())
        {
        COMASuplPosInitState* state =  static_cast <COMASuplPosInitState *>(iSuplState);
        return state->GetRemainingPospayLoadL(); 
        }
    else
        {
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::IsWholeMessageSent
// Is whole message get sent
// -----------------------------------------------------------------------------
TBool COMASuplSession::IsWholeMessageSentL()
    {
    COMASuplState::TSuplMessageType msgType = iSuplState->GetSuplState();
    switch(msgType)
        {
        case COMASuplState::ESUPL_POS_INIT:
            {
            COMASuplPosPayload* remainingPosPayload = GetRemainingPosPayloadL();
            if(remainingPosPayload )
                return EFalse;
            else
                return ETrue;
            }

        case COMASuplState::ESUPL_POS:
            {
            return !IsSegmentationDone();
            }
        default : //For Start , End
            {
            return ETrue;	
            } 
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::GetNetworkModeL()
// Checks wheather phone is online or Offline mode...
// -----------------------------------------------------------------------------

TInt COMASuplSession::GetNetworkModeL()
	{
	
	TInt networkMode;
	// get network mode
	CRepository* centralRepository = CRepository::NewLC(KCRUidCoreApplicationUIs);
	User::LeaveIfError(centralRepository->Get(KCoreAppUIsNetworkConnectionAllowed, networkMode));
    CleanupStack::PopAndDestroy(centralRepository);
	return networkMode;
	}

// -----------------------------------------------------------------------------
// COMASuplSession::SetSessionUIFlag
// Sets/Resets the Session UI flag 
// -----------------------------------------------------------------------------
void COMASuplSession::SetSessionUIFlag(TBool aValue)
    {
    iUIFlag = aValue;
    }        

// -----------------------------------------------------------------------------
// COMASuplSession::GetSessionUIFlag
// Returns Session UI flag 
// -----------------------------------------------------------------------------
TBool COMASuplSession::GetSessionUIFlag()
    {
    return iUIFlag;
    }    



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// COMASuplSession::CloseAllOperations
// Close all operations...from here we will stop all processing releated with session and will 
// not send SUPL_END 
// -----------------------------------------------------------------------------
void COMASuplSession::CloseAllOperations()
    {
    	iCompleteSelfRequestor->CompleteInvalidSLPSession();
    }    
    
// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidSLPSessionL
// Relinquish control to Communication manager....
// 
// -----------------------------------------------------------------------------
void COMASuplSession::HandleInvalidSLPSession()
	{
    iTrace->Trace(_L("Cancelling All operations..since received invalid SLP session..."), KTraceFileName, __LINE__); 
    iConnRequestor->CancelReadOperation();
		CancelAllOperations();
		iConnRequestor->CloseConnection();
        iPortNum = 0;
		iSessionObserver.TerminateSession(this, KErrCompletion);//// Invoke Terminate Session
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::IsUIDisplayedL
// Checks whether UI is displayed or not previously
// 
// -----------------------------------------------------------------------------
	
TBool COMASuplSession::IsUIDisplayedL()	
	{
	/*
	TBool retValue = EFalse;
	RBuf HSLPIPAddress;
	RBuf IAP_Name;
	RBuf Imsi;
	CSuplSettings::TSuplSettingsChangedBy changedBy;
				
	HSLPIPAddress.CreateL(KMaxIPAddressLength);
	CleanupClosePushL(HSLPIPAddress);
			
	IAP_Name.CreateL(KMaxIapNameLength);
	CleanupClosePushL(IAP_Name);
				
	Imsi.CreateL(KMaxImsiLength);
	CleanupClosePushL(Imsi);
		
	TInt error = iSuplStorageSettings->Get(HSLPIPAddress, IAP_Name, Imsi, changedBy); 
				
	if(!error && (IAP_Name.Length() != 0))
	{
		if (IAP_Name.Compare(_L("-")) == 0)			
			retValue = ETrue;
	}
	CleanupStack::PopAndDestroy(3);	
	return retValue;
	*/
	return EFalse;
	}


// -----------------------------------------------------------------------------
// COMASuplSession::SetSETSessionIdL
// Sets the proper SET Session id depending on the type of Varient.
// 
// -----------------------------------------------------------------------------

TInt COMASuplSession::SetApproriateSETSessionId()
    {
    TInt err = KErrNone;
    if( IsIMSIVariant() )
        {
        SetIMSI();  
        }
    else
        {
        err = GetLocalIPAddress();
        }	

    return err;	
    }

// -----------------------------------------------------------------------------
// COMASuplSession::TimerExpiredL
// Checks whether UI is displayed or not previously
// 
// -----------------------------------------------------------------------------
void COMASuplSession::TimerExpiredL()
    {
    iTrace->Trace(_L("Timer Expired for SUPL_POSINIT"), KTraceFileName, __LINE__); 
    OperationCompleteL(KErrTimedOut);
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SettingsUsageUICompletedL
// 
// -----------------------------------------------------------------------------
/*void COMASuplSession::SettingsUsageUICompletedL(TInt aError)
    {
    iUIFlag = EFalse;
    TBuf<64> msg;	
    msg.Copy(_L("SUPL Usage UI completed with ... "));
    msg.AppendNum(aError);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 

    iPrevUsageResult = aError;                    



    if (aError == KErrNone)
        {
        iProtocolManager.UpdateAllSubSessnsInSameSession(iIpcSessionId);                

        if (isTimeoutDialogTimerStarted)
            {                
            iTrace->Trace(_L("COMASuplSession::SettingsUsageUICompletedL, stopping timer "), KTraceFileName, __LINE__);
            isTimeoutDialogTimerStarted = EFalse;                    
            iDialogTimer->StopTimer();

            //Compute how log the dialog was on
            TTime nowTime;
            nowTime.HomeTime();
            nowTime.SecondsFrom(iDlgStartTime, iDiffTime);	
            }
        if (iNwInitError)
            {
            iNwInitError = EFalse;                    
            HandleSuplInitErrorL(iNwInitCompletionCode);                             
            }                
        else
            InitializeL(iRequestID); 
        }            
    else if (aError == KErrCompletion)
        CheckForSuplUsageL();
    else 
        {
        TBuf<64> msg;	
        msg.Copy(_L("Error after SUPL Usage Popup... "));
        msg.AppendNum(aError);
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        iSessionObserver.TerminateSession(this, KErrGeneral);	
        return;
        }
	}*/

// -----------------------------------------------------------------------------
// COMASuplSession::CheckForSuplUsageL
// 
// -----------------------------------------------------------------------------
void COMASuplSession::CheckForSuplUsageL()
    {
    iTrace->Trace(_L("COMASuplSession::CheckForSuplUsageL Start"), KTraceFileName, __LINE__); 

    /*if (iSuplUsage >= 1)
        {            
        CSuplSettings::TSuplSettingsUsage usage = iSuplSettings->SUPLUsage();
        if (usage == CSuplSettings::ESuplUsageDisabled)
            {            
            iTrace->Trace(_L("CheckForSuplUsageL, SUPL disabled"), KTraceFileName, __LINE__); 
            iSessionObserver.TerminateSession(this, KErrGeneral);	
            }
        else if (usage == CSuplSettings::ESuplUsageAutomatic)
            { */           
            if (iNwInitError)
                {
                iNwInitError = EFalse;                    
                HandleSuplInitErrorL(iNwInitCompletionCode);                             
                }                
            else
            	{
            		if (iRoaming)                
                    HandleRoamingCheckCompleteL(KErrNone, EFalse);                    
                else
                    {                        
                    iEtelRoamingCheck = ETrue;
                    iProtocolManager.CheckForRoaming();
                    }
	            //InitializeL(iRequestID); 
	          	}
            /*}

        else if (usage == CSuplSettings::ESuplUsageHomeAutomatic || usage == CSuplSettings::ESuplUsageAlwaysAsk)
            {
            if (iRoaming)                
                HandleRoamingCheckCompleteL(KErrNone, EFalse);                    
            else
                {                        
                iEtelRoamingCheck = ETrue;
                iProtocolManager.CheckForRoaming();
                }
            }            
        }            
    else
	        InitializeL(iRequestID); */

    }        

// -----------------------------------------------------------------------------
// COMASuplSession::CheckForTimerExpiry
// 
// -----------------------------------------------------------------------------
void COMASuplSession::CheckForTimerExpiry()
    {
    if (iPersistFail)
        {            
        TTime nowTime;
        nowTime.HomeTime();
        TTimeIntervalMicroSeconds diff = nowTime.MicroSecondsFrom(iFailTime);	

        if (diff > (iPersistFailTimer * KSecond) )
            iChanged = ETrue;            
        }
    }        
// -----------------------------------------------------------------------------
// COMASuplSession::CheckForPreviousResultL
// 
// -----------------------------------------------------------------------------
void COMASuplSession::CheckForPreviousResultL()
    {
    CheckForTimerExpiry();            
    if (iChanged)
        {
        iProtocolManager.CancelNotify();
        iEtelNotify = EFalse;
        iPersistFail = EFalse;
        iCapsFail = EFalse;
        iChanged = EFalse;
        CheckForSuplUsageL();
        return;
        }

    // Check for previous request result
    if (iPrevUsageResult == KErrNone && !iPersistFail && !iCapsFail)
        {            
	    InitializeL(iRequestID); 
        }
    else if (iCapsFail && iAllowedCapabilities != iFailedCaps)
        {            
	    InitializeL(iRequestID); 
        }
    else
        {            
	
        if (iPrevUsageResult != KErrNone)
	        iSessionObserver.TerminateSession(this, iPrevUsageResult);	
        else if (iPersistFail)
	        iSessionObserver.TerminateSession(this, iPrevReqResult);	
        else if (iCapsFail)
	        iSessionObserver.TerminateSession(this, KErrGeneral);	
        }
    }        

void COMASuplSession::HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw)
    {
    iTrace->Trace(_L("COMASuplSession::HandleCurrentNetworkChangeL"), KTraceFileName, __LINE__); 
    iEtelNotify = EFalse;
    if (aErrorCode == KErrNone)
        {            
        iChanged = ETrue;
        if (!aHomeNw)
            {            
            iRoaming = ETrue;            
            }
        else
            iRoaming = EFalse;                
        }
    }            

void COMASuplSession::HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw)
    {
    iTrace->Trace(_L("COMASuplSession::HandleRoamingCheckCompleteL"), KTraceFileName, __LINE__); 
    iEtelRoamingCheck = EFalse;            
    if (aErrorCode == KErrNone)            
        {            
        iUsageHomeNW = aHomeNw;   
        iConnRequestor->UpdateSLPListForHomeUsage(iUsageHomeNW);             
            //CSuplSettings::TSuplSettingsUsage usage = iSuplSettings->SUPLUsage();

            /*if (usage == CSuplSettings::ESuplUsageAlwaysAsk)
            {
            TInt err;                
            if (!aHomeNw)
                {                            
                iTrace->Trace(_L("Invoking LaunchSuplUsageSettingsUI, user is roaming"), KTraceFileName, __LINE__);
                err = iProtocolManager.LaunchSuplUsageSettingsUI(this, ETrue);        
                }
            else
                {                            
                iTrace->Trace(_L("Invoking LaunchSuplUsageSettingsUI, user is not roaming"), KTraceFileName, __LINE__);
                err = iProtocolManager.LaunchSuplUsageSettingsUI(this, EFalse);        
                }

            if(KErrNone != err && KErrInUse == err)
                {
                iTrace->Trace(_L("Dialog in use, setting flag"), KTraceFileName, __LINE__);
                SetSuplUsageFlag();                            
                }
            if( KErrNone == err  && iRequestType == ESUPL_NETWORK )
                {
                iTrace->Trace(_L("Network Session , starting timer"), KTraceFileName, __LINE__);
                COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                TOMASuplQop qop;
                TInt retVal = suplInit->Qop(qop);
                TInt delay;
                qop.Delay(delay);
                //if delay is > 7, it is not valid
                if (delay > 7)
                    delay = 0;                                
                if(retVal == KErrNone && delay > 0)
                    {
                    TReal delayReal;
                    Math::Pow(delayReal, 2, (TReal)delay);
                    delay = (TInt) delayReal;
                    TBuf<128> msg(_L("Delay present in message, value is = "));
                    msg.AppendNum(delay);
                    iTrace->Trace(msg, KTraceFileName, __LINE__); 
                    isTimeoutDialogTimerStarted = ETrue;
                    iDialogTimer->StartTimer(delay);
                    }  	
                else
                    {
                    if (iSuplInitTimeOut > 0)
                        {                                                        
                        TBuf<256> msg(_L("Delay value in CR is"));
                        msg.AppendNum(iSuplInitTimeOut);
                        iTrace->Trace(msg, KTraceFileName, __LINE__); 
                        iDialogTimer->StartTimer(iSuplInitTimeOut * KSecond);
                        isTimeoutDialogTimerStarted = ETrue;
                        }
                    else
                        iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                    }
                iDlgStartTime.HomeTime();
                }
            else if (KErrNone != err)
                {
                iTrace->Trace(_L("Invoking HandleSuplErrorL"), KTraceFileName, __LINE__);
                HandleSuplErrorL(err);
                }

                }*/
            //else if (usage == CSuplSettings::ESuplUsageHomeAutomatic) 
            //{                
            //if (!aHomeNw)
              //  {                
                //TInt err = KErrNone;                
                //if (!iSuplSettings->IsUIActive() )
                    {
                    //iTrace->Trace(_L("Invoking LaunchSuplUsageSettingsUI, user is roaming"), KTraceFileName, __LINE__);
                    //err = iProtocolManager.LaunchSuplUsageSettingsUI(this, ETrue);        

                if(iSuplMsgType == ESUPL_INIT)
                    {
                    iTrace->Trace(_L("Dialog timer started"), KTraceFileName, __LINE__);

                    COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                    TOMASuplQop qop;
                    TInt retVal = suplInit->Qop(qop);
                    TInt delay;
                    qop.Delay(delay);
                    //if delay is > 7, it is not valid
                    if (delay > 7)
                        delay = 0;                                
                    if(retVal == KErrNone && delay > 0)
                        {
                        TReal delayReal;
                        Math::Pow(delayReal, 2, (TReal)delay);
                        delay = (TInt) delayReal;
                        TBuf<128> msg(_L("Delay present in message, value is = "));
                        msg.AppendNum(delay);
                        iTrace->Trace(msg, KTraceFileName, __LINE__); 
                        isTimeoutDialogTimerStarted = ETrue;
                        iDialogTimer->StartTimer(delay);
                        }  	
                    else
                        {
                        if (iSuplInitTimeOut > 0)
                            {                                                        
                            isTimeoutDialogTimerStarted = ETrue;
                            iDialogTimer->StartTimer(iSuplInitTimeOut * KSecond);
                            }
                        else
                            iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                        }
                    }
                }


              /*  if(!iSuplSettings->IsUIActive() && KErrNone != err)
                    {
                    SetSuplUsageFlag();                            
                    }
                else
                    {
                    iTrace->Trace(_L("Invoking HandleSuplErrorL"), KTraceFileName, __LINE__);
                    HandleSuplErrorL(err);
                    }*/
                
                       
                                
                if (iNwInitError)
                    {
                    iNwInitError = EFalse;                    
                    HandleSuplInitErrorL(iNwInitCompletionCode);                             
                    }                
                else
	                InitializeL(iRequestID); 
                                                
        }
    else
        {
        iSessionObserver.TerminateSession(this, KErrGeneral);	
        }                
    }            
TInt COMASuplSession::SuplIpcSessionID() 
    {
    return iIpcSessionId;
    }    

void COMASuplSession::UpdateSuplUsage()
    {        
    iPrevUsageResult = KErrNone;            
    }    

void COMASuplSession::SettingsChanged()
    {
    iChanged = ETrue;
    }    

void COMASuplSession::HandleIapSettingsChangeL(TInt aErrorCode)
    {
    iTrace->Trace(_L("COMASuplSession::HandleIapSettingsChangeL"), KTraceFileName, __LINE__);
    if (aErrorCode >= 0)
        iChanged = ETrue;
    }        

void COMASuplSession::CheckForPersistentFailure(TInt aErrorCode)
    {
    if (iRequestType == ESUPL_TERMINAL)            
        {            
        if (
                (aErrorCode <= KErrDndNameNotFound && aErrorCode >= KErrDndServerUnusable) || // DNS errors
                (aErrorCode <= KErrGprsAndNonGprsServicesNotAllowed && aErrorCode >= KErrGprsFeatureNotSupported) || // GPRS errors
                (aErrorCode <= KErrSSLNoSharedCipher && aErrorCode >= KErrSSLDllLeave) || // TLS errors
                (aErrorCode == KErrCouldNotConnect)                            // Server does not exist
            )
            {
            iPersistFail = ETrue;
            iPrevReqResult = KErrGeneral;

            iFailTime.HomeTime();

            iTrace->Trace(_L("COMASuplSession::CheckForPersistentFailure, listening to network change"), KTraceFileName, __LINE__);
            iEtelNotify = ETrue;
            iProtocolManager.NotifyCurrentNetworkChange();

            iTrace->Trace(_L("COMASuplSession::CheckForPersistentFailure, listening to IAP change"), KTraceFileName, __LINE__);
            if (!iIapNotifier->IsActive())
                iIapNotifier->NotifyIapSettingsChange();
            }            
        }            
    }        
void COMASuplSession::SetSuplUsageFlag()
    {
    iUsageDialog = ETrue;            
    }        

void COMASuplSession::ReSetSuplUsageFlag()
    {
    iUsageDialog = EFalse;            
    }        

TBool COMASuplSession::GetSuplUsageFlag()
    {
    return iUsageDialog;
    }        

/*void COMASuplSession::StartUsageDialogLaunchL()
    {
    iTrace->Trace(_L("COMASuplSession::StartUsageDialogLaunchL"), KTraceFileName, __LINE__);
    ReSetSuplUsageFlag();            
    iTrace->Trace(_L("COMASuplSession::StartUsageDialogLaunchL, flag reset done"), KTraceFileName, __LINE__);

    CSuplSettings::TSuplSettingsUsage usage = iSuplSettings->SUPLUsage();
    if (usage == CSuplSettings::ESuplUsageAlwaysAsk)
        {
        TInt err;                
        if (!iUsageHomeNW)
            {                            
            iTrace->Trace(_L("Invoking LaunchSuplUsageSettingsUI, user is roaming"), KTraceFileName, __LINE__);
            err = iProtocolManager.LaunchSuplUsageSettingsUI(this, ETrue);        

            if(iRequestType == ESUPL_NETWORK && err == KErrNone)
                {
                iTrace->Trace(_L("starting Dialog timer "), KTraceFileName, __LINE__);

                COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                TOMASuplQop qop;
                TInt retVal = suplInit->Qop(qop);
                TInt delay;
                qop.Delay(delay);
                //if delay is > 7, it is not valid
                if (delay > 7)
                    delay = 0;                                
                if(retVal == KErrNone && delay > 0)
                    {
                    TReal delayReal;
                    Math::Pow(delayReal, 2, (TReal)delay);
                    delay = (TInt) delayReal;
                    TBuf<128> msg(_L("Delay present in message, value is = "));
                    msg.AppendNum(delay);
                    iTrace->Trace(msg, KTraceFileName, __LINE__); 
                    isTimeoutDialogTimerStarted = ETrue;
                    iDialogTimer->StartTimer(delay);
                    }  	
                else
                    {
                    if (iSuplInitTimeOut > 0)
                        {                                                        
                        iTrace->Trace(_L("Delay not present in message"), KTraceFileName, __LINE__);
                        TBuf<256> msg(_L("Delay value in CR is "));
                        msg.AppendNum(iSuplInitTimeOut);
                        iDialogTimer->StartTimer(iSuplInitTimeOut * KSecond);
                        isTimeoutDialogTimerStarted = ETrue;
                        }
                    else
                        iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                    }
                }
            }
        else
            {                            
            iTrace->Trace(_L("Invoking LaunchSuplUsageSettingsUI, user is not roaming"), KTraceFileName, __LINE__);
            err = iProtocolManager.LaunchSuplUsageSettingsUI(this, EFalse);        

            if( iRequestType == ESUPL_NETWORK && err == KErrNone)
                {    
                iTrace->Trace(_L("starting Dialog timer "), KTraceFileName, __LINE__);

                COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                TOMASuplQop qop;
                TInt retVal = suplInit->Qop(qop);
                TInt delay;
                qop.Delay(delay);
                //if delay is > 7, it is not valid
                if (delay > 7)
                    delay = 0;                                
                if(retVal == KErrNone && delay > 0)
                    {
                    TReal delayReal;
                    Math::Pow(delayReal, 2, (TReal)delay);
                    delay = (TInt) delayReal;
                    TBuf<128> msg(_L("Delay present in message, value is = "));
                    msg.AppendNum(delay);
                    iTrace->Trace(msg, KTraceFileName, __LINE__); 
                    isTimeoutDialogTimerStarted = ETrue;
                    iDialogTimer->StartTimer(delay);
                    }  	
                else
                    {
                    if (iSuplInitTimeOut > 0)
                        {                                                        
                        iTrace->Trace(_L("Delay not present in message"), KTraceFileName, __LINE__);
                        TBuf<256> msg(_L("Delay value in CR is"));
                        msg.AppendNum(iSuplInitTimeOut);
                        iDialogTimer->StartTimer(iSuplInitTimeOut * KSecond);
                        isTimeoutDialogTimerStarted = ETrue;
                        }
                    else
                        iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                    }
                }
            }

        if(KErrNone != err)
            {                        
            iTrace->Trace(_L("Invoking HandleSuplErrorL"), KTraceFileName, __LINE__);
            HandleSuplErrorL(err);
            }
        }
    else if (usage == CSuplSettings::ESuplUsageHomeAutomatic) 
        {                
        if (!iUsageHomeNW)
            {                
            TInt err;                
            iTrace->Trace(_L("Invoking LaunchSuplUsageSettingsUI, user is roaming"), KTraceFileName, __LINE__);
            err = iProtocolManager.LaunchSuplUsageSettingsUI(this, ETrue);        

            if(KErrNone != err)
                {
                iTrace->Trace(_L("Invoking HandleSuplErrorL"), KTraceFileName, __LINE__);
                HandleSuplErrorL(err);
                }

            if(iRequestType == ESUPL_NETWORK && err == KErrNone)
                {    
                iTrace->Trace(_L("starting Dialog timer "), KTraceFileName, __LINE__);

                COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                TOMASuplQop qop;
                TInt retVal = suplInit->Qop(qop);
                TInt delay;
                qop.Delay(delay);
                //if delay is > 7, it is not valid
                if (delay > 7)
                    delay = 0;                                
                if(retVal == KErrNone && delay > 0)
                    {
                    TReal delayReal;
                    Math::Pow(delayReal, 2, (TReal)delay);
                    delay = (TInt) delayReal;
                    TBuf<128> msg(_L("Delay present in message, value is = "));
                    msg.AppendNum(delay);
                    iTrace->Trace(msg, KTraceFileName, __LINE__); 
                    isTimeoutDialogTimerStarted = ETrue;
                    iDialogTimer->StartTimer(delay);
                    }  	
                else
                    {
                    if (iSuplInitTimeOut > 0)
                        {                                                        
                        iTrace->Trace(_L("Delay not present in message"), KTraceFileName, __LINE__);
                        TBuf<256> msg(_L("Delay value in CR is"));
                        msg.AppendNum(iSuplInitTimeOut);
                        iDialogTimer->StartTimer(iSuplInitTimeOut * KSecond);
                        isTimeoutDialogTimerStarted = ETrue;
                        }
                    else
                        iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                    }
                }
            }
        else            
            {                
            if (iNwInitError)
                {
                iNwInitError = EFalse;                    
                HandleSuplInitErrorL(iNwInitCompletionCode);                             
                }                
            else
                InitializeL(iRequestID); 
            }                
        }                
    }*/

TBool COMASuplSession::IsEtelNotifySet()
    {
    iTrace->Trace(_L("COMASuplSession::IsEtelNotifySet"), KTraceFileName, __LINE__); 
    return iEtelNotify;            
    }

TBool COMASuplSession::IsEtelRoamingSet()
    {
    iTrace->Trace(_L("COMASuplSession::IsEtelRoamingSet"), KTraceFileName, __LINE__); 
    return iEtelRoamingCheck;
    }        

// -----------------------------------------------------------------------------
// COMASuplSession::DialogTimerExpiredL
// Checks whether UI is displayed or not previously
// 
// -----------------------------------------------------------------------------
void COMASuplSession::DialogTimerExpiredL()
    {
    if (iTrgTimer)         
        {
        iTrgTimer = EFalse;
        if (iStartTimerFlag)
            {
            iStartTimerFlag = EFalse;                    
            StartPeriodicTriggerTimer();
            }
        else        
            {                
            iTrace->Trace(_L("Timer Expired for trigger session"), KTraceFileName, __LINE__); 
            StartTriggerPosSessionL();                
            }            
        }            
    else
        {            
        iTrace->Trace(_L("Timer Expired for SUPL Dialog"), KTraceFileName, __LINE__); 


    //if (!iIapDialogShown)
	   // iProtocolManager.LaunchSuplDialogTimeoutUI(this);
   // else
       // iIapDlgTimerExpired = ETrue;            

        }
    return;	
    }

// -----------------------------------------------------------------------------
// COMASuplSession::SettingsTimeOutUICompletedL
// 
// -----------------------------------------------------------------------------
void COMASuplSession::SettingsTimeOutUICompletedL(TInt /*aError*/)
    {

	  //  TBuf<64> msg;	
	  //  msg.Copy(_L("SUPL Timeout UI completed with ... "));
	  //  msg.AppendNum(aError);
	  //  iTrace->Trace(msg,KTraceFileName, __LINE__); 
	  //  iSessionObserver.TerminateSession(this, KErrGeneral);	
    return;

    }

// -----------------------------------------------------------------------------
// COMASuplSession::StoreCurrentCellIDL
// Stores current cell id in CR
// 
// -----------------------------------------------------------------------------
void COMASuplSession::StoreCurrentCellIDL(HPositionGenericInfo& /*aSuplPosInfo*/)
    {
    iTrace->Trace(_L("Calling StoreCurrentCellIDL"),KTraceFileName, __LINE__);
    TPckg<TCellIdInfo> cellPkg(iCurrentCellId);

#ifdef PRINT_MESSAGE
    TBuf<64> msg;
    msg.Copy(_L("cid=:"));
    msg.AppendNum(iCurrentCellId.iCid);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 

    msg.Copy(_L("Lac=:"));
    msg.AppendNum(iCurrentCellId.iLac);
    iTrace->Trace(msg,KTraceFileName, __LINE__);

    msg.Copy(_L("MCC=:"));
    msg.AppendNum(iCurrentCellId.iMCC);
    iTrace->Trace(msg,KTraceFileName, __LINE__);

    msg.Copy(_L("MNC=:"));
    msg.AppendNum(iCurrentCellId.iMNC);
    iTrace->Trace(msg,KTraceFileName, __LINE__); 
#endif

    CRepository* ConfigurationCenRepSession = CRepository::NewL(KCRUidOMASuplConfiguration);
    CleanupStack::PushL(ConfigurationCenRepSession);
    User::LeaveIfError(ConfigurationCenRepSession->Set(KOMASuplCurrentCellid, cellPkg));

    CleanupStack::PopAndDestroy(ConfigurationCenRepSession);
    }

// -----------------------------------------------------------------------------
// COMASuplSession::ServerAddressCheckForSuplInitL()
// Stores current cell id in CR
// 
// -----------------------------------------------------------------------------
TInt COMASuplSession::ServerAddressCheckForSuplInitL()
    {
    TBuf<256> serverName;
    TBuf<64> msg;

    COMASuplSLPAddress* slpAddress = NULL;
    COMASuplInit* SuplInit = static_cast <COMASuplInit *>(iDecodedAsnMessage);
    TInt err = SuplInit->SLPAddress(slpAddress);

    if(err == KErrOMASuplParamNotSet)
        {
        msg.Copy(_L("SLP address is NOT PRESENT in SUPL_INIT."));
        iTrace->Trace(msg,KTraceFileName, __LINE__); 

        //Set default slp to Conn Requestor...		
        iConnRequestor->UseDefaultServerL();
        return KErrNone;
        }

    COMASuplSLPAddress::TSuplSLPAddressType addressType = slpAddress->SLPAddressType();
    if(addressType == COMASuplSLPAddress::EFqdn)
        {
        HBufC8* fqdn;
        TInt err = slpAddress->Fqdn(fqdn);
        serverName.Copy(fqdn->Des());
        msg.Copy(_L("SLP address as FQDN is PRESENT in SUPL_INIT:"));
        msg.Append(serverName);
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        }
    else
        {
        TBuf8<256> IPslpaddress;
        slpAddress->IPvAddress(IPslpaddress);
        serverName.Copy(IPslpaddress);
        msg.Copy(_L("SLP address as IP is PRESENT in SUPL_INIT:"));
        msg.Append(serverName);
        iTrace->Trace(msg,KTraceFileName, __LINE__); 

        }

    TBool isExist = iSuplStorageSettings->IsSlpExists(serverName);
    if(isExist)
        {
        //Set to Conn Requestor...
        iConnRequestor->UseServerL(serverName);
        return KErrNone;
        }
    else
        {
        msg.Copy(_L("Dropping Packet.SLP absent in server List:"));
        msg.Append(serverName);
        iTrace->Trace(msg,KTraceFileName, __LINE__); 
        return KErrNotFound;
        }	

    }

void COMASuplSession::RunTriggerSessionL(
								TRequestStatus& aStatus, 
                                TSuplTerminalPeriodicTrigger& aTriggerParams,
			                    const TDesC& aHslpAddress, 
			                    TBool aFallBack,
                                TInt aAllowedCapabilities, 
							 	TInt aSessionIdSeed, 
							 	TInt aRequestID)
    {
    iRunRequestStatus = & aStatus;
    *iRunRequestStatus = KRequestPending;

    iRequestID = aRequestID;

    iTrace->Trace(_L("Start COMASuplSession::RunTriggerSessionL"), KTraceFileName, __LINE__); 
    	
    // Log Session Id
    TBuf<64> id;
    id.Append(_L("Session  Id is "));
    id.AppendNum(aSessionIdSeed);
    iTrace->Trace(id,KTraceFileName, __LINE__); 
    	
    // Log Capability from client
    id.Copy(_L("Received Capability is "));
    id.AppendNum(aAllowedCapabilities);
    iTrace->Trace(id,KTraceFileName, __LINE__); 
    	
    iSETSessionUniqueId = aSessionIdSeed;

    TInt networkMode = 1;
    networkMode = GetNetworkModeL();
		
    if ( networkMode == ECoreAppUIsNetworkConnectionNotAllowed )
        {
        id.Copy(_L("The device is in OFFLINE mode."));
        iTrace->Trace(id,KTraceFileName, __LINE__); 
        iSessionObserver.TerminateSession(this, KErrGeneral);
        return;
        }
    else
        {
        id.Copy(_L("The device is in ON LINE mode."));
        iTrace->Trace(id,KTraceFileName, __LINE__); 
        }

    if(aAllowedCapabilities == 0)
        {
        iAllowedCapabilities = KGpsSETAssisted | KGpsSETBased | KAutonomousGps| KAFLT | KECID | KEOTD | KOTDOA | KCID; 
        }
    else
        {
        iAllowedCapabilities = aAllowedCapabilities;
        }

    //Ownership transfer to iSuplSessionId
    iSuplSessionId->SetSLPSessionID(NULL); 	

    //Set flag to indicate that it is a triggering session
    iTriggerSession = ETrue;

    //Store trigger parameters
    iTriggerParams = aTriggerParams;

	iConnRequestor->SetDefaultParametersL(aHslpAddress,aFallBack);

    CheckForSuplUsageL();

    // Clear  Position....
    delete iPosition;
    iPosition = NULL;
	    
    iTrace->Trace(_L("End of COMASuplSession::RunTriggerSessionL"), KTraceFileName, __LINE__); 
    }

void COMASuplSession::GetPeriodicTriggerParams()
    {
    iTrace->Trace(_L("COMASuplSession::GetPeriodicTriggerParams"),
            KTraceFileName, __LINE__);
    TInt ret = iTriggerParams.Get(iNumFixes, iInterval, iStartTime);
    if (KErrNone == ret)
        {
        iTotalNoOfFixes = iNumFixes;
        iStartTimeCopy = iStartTime;
        }
    }

void COMASuplSession::StartPeriodicTriggerTimer()
    {
    if (iNumFixes != 0)
        {            
        iTrace->Trace(_L("COMASuplSession::StartPeriodicTriggerTimer"), KTraceFileName, __LINE__); 
        //Set flag to indicate timer for trigger
        iTrgTimer = ETrue;            
        if (iStartTime != 0)
            {                
            iDialogTimer->StartTimer(iStartTime * KSecond);
            iStartTime = 0;
            iStartTimerFlag = ETrue;
            }
        else
            {
            if(iTotalNoOfFixes == iNumFixes)
                {
                iDialogTimer->StartTimer(0);
                }
            else
                {
                iDialogTimer->StartTimer(iInterval * KSecond);
                }   
            }
        }
    }

void COMASuplSession::StartTriggerPosSessionL()
    {
    iTrace->Trace(_L("COMASuplSession::StartTriggerPosSessionL"), KTraceFileName, __LINE__); 

    if (iClientNotify || iTotalNoOfFixes == iNumFixes)
        {            
        //Check if fix already exists with the POS Plug-in
        //in which case whole of POS session is not required.

        iClientNotify = EFalse;                            
        if (iInterval > KMaxIntervalTime || iDisconnected || iStartTime > KMaxStartTime)
            {                
		
			iTrace->Trace(_L("Setting state to INITIALIZED"), KTraceFileName, __LINE__);       
            iStartTimeCopy = 0;                    
	        iSuplSessionState = ESUPL_INITIALIZED;
            }
        else            
            {                
			iTrace->Trace(_L("Setting state to CONNECTING"), KTraceFileName, __LINE__);       
	        iSuplSessionState = ESUPL_CONNECTING;
            }

	    OperationCompleteL(KErrNone);	
        }
    else
        {            
        //Reset number of fixes to 0 so that if notify 
        //request comes after cancel has started, it will
        //get completed with error                
        iNumFixes = 0;                
        CancelTriggerSession();            
        }
    }

void COMASuplSession::NotifyTriggerFiredL(TRequestStatus& aStatus, TSuplTriggerFireInfo& aFireInfo)
    {
    iTrace->Trace(_L("COMASuplSession::NotifyTriggerFiredL"), KTraceFileName, __LINE__); 
	iRunRequestStatus = & aStatus;
	*iRunRequestStatus = KRequestPending;
    iTrgFireInfo = &aFireInfo;
    iClientNotify = ETrue;
    if (iNumFixes == 0)
        {
            CompleteSession(KErrGeneral);
        }

    }        

void COMASuplSession::CancelTriggerSession()
    {
    // Check if trigger session is in progress.  If not trigger session, silently discard.
    if (!iTriggerSession)
        return;

    // If the trigger session is over and SUPL END is already sent,
    // then also silently discard 
    if (iSuplMsgType == ESUPL_END)  
        return;

    // If timer is running, stop timer and send SUPL TRIGGER STOP.
    // Else check if POS session in progress and send SUPL TRIGGER STOP
    if (iDialogTimer->IsTimerStarted())
        {            
        iDialogTimer->StopTimer();
        }

    iTrgCancelRequest = ETrue;                
    // Check if the triggering POS session in progress
    if (iSuplMsgType == ESUPL_POSINIT || iSuplMsgType == ESUPL_POS || iInterval <= 10)  
        {            
	    iSuplSessionState = ESUPL_CONNECTING;
        }
    else
        {                
        // If POS session is not in progress, need to check for interval and progress
	    iSuplSessionState = ESUPL_INITIALIZED;
        }

	TRAP_IGNORE(OperationCompleteL(KErrNone));
		

    }        
// -----------------------------------------------------------------------------
// COMASuplSession::InsertActiveSessionL
// 
// 
// -----------------------------------------------------------------------------
void COMASuplSession::InsertActiveSessionL()
    {
    				iTrace->Trace(_L("Inserting New Trigger Session into DB..."), KTraceFileName, __LINE__); 
						CTriggerParams::TRequestType requestType;
						if(iRequestType == ESUPL_TERMINAL)
						{
							requestType = CTriggerParams::ETerminalInitiated;
						}
						else
						{
							requestType = CTriggerParams::ENetworkInitiated;
						}	

            CTriggerParams* activeSessionParam = CTriggerParams::NewLC();
            TTime endTime;
            endTime.HomeTime();
            TTimeIntervalSeconds period;
            period = iNumFixes * iInterval;
            endTime = endTime + period;
            activeSessionParam->Set(iSETSessionUniqueId,iRequestorName,ETrue,ETrue,CTriggerParams::EPeriodic,requestType,iNumFixes,iInterval);
            activeSessionParam->SetTriggerEndTime(endTime);
            
            iTrace->Trace(_L("Session Id : "), KTraceFileName, __LINE__);
            TBuf<64> sessionId;
            sessionId.AppendNum(iSETSessionUniqueId);
            iTrace->Trace(sessionId, KTraceFileName, __LINE__);
            
            TInt err=iSuplStorageSettings->InsertSessionRecord(activeSessionParam);
            CleanupStack::PopAndDestroy(activeSessionParam);
    }

// -----------------------------------------------------------------------------
// COMASuplSession::RemoveActiveSession
//  
// 
// -----------------------------------------------------------------------------
void COMASuplSession::RemoveActiveSession()
    {
    			if (iTrace)
    			iTrace->Trace(_L("Removing Trigger Session from DB..."), KTraceFileName, __LINE__); 
    			//may need to reset the iActiveSessionId to 0 so that self deletion event will not come to same session...
    			TInt err = iSuplStorageSettings->DeleteSessionRecord(iSETSessionUniqueId);
    }
    
void COMASuplSession::UpdateActiveTriggerSession()
{
		iTrace->Trace(_L("Updating Trigger Session into DB..."), KTraceFileName, __LINE__); 
		iTrace->Trace(_L("Session Id : "), KTraceFileName, __LINE__);
    TBuf<64> sessionId;
    sessionId.AppendNum(iSETSessionUniqueId);
    iTrace->Trace(sessionId, KTraceFileName, __LINE__);
		
    TInt err = iSuplStorageSettings->SetOutstandingTriggers(iSETSessionUniqueId,iNumFixes);
}    
    
// -----------------------------------------------------------------------------
// COMASuplSession::AreYouHandlingThisSession
//   
// 
// -----------------------------------------------------------------------------
TBool COMASuplSession::AreYouHandlingThisSession(TInt64& aActiveSessionId)
    {
    			//We will consider only  Network Initiated case,as there will be cancellation of only network initiated trigger only, nor SET initiated.
    			//return ((aActiveSessionId == iSETSessionUniqueId) && (iRequestType == ESUPL_NETWORK));
    			return (aActiveSessionId == iSETSessionUniqueId);
    }


void COMASuplSession::LaunchNotifyDialogL()
{
	
    iTrace->Trace(_L("COMASuplSession2::LaunchNotifyDialogL..."), KTraceFileName, __LINE__); 
	
	if (!iPrivacyOverride && iTotalNoOfFixes == (iNumFixes+1))
	{
		iTrace->Trace(_L("COMASuplSession2::LaunchNotifyDialogL...inside if"), KTraceFileName, __LINE__);
		// Synchronous call 
		if (iSuplReqInfo)
				iTrace->Trace(_L("iSuplReqInfo is not null"), KTraceFileName, __LINE__);
		
		if (iNetworkPrivacy)
				iTrace->Trace(_L("iNetworkPrivacy is not null"), KTraceFileName, __LINE__);
				
				//Inform UI that ,trigger session has started...
		InsertActiveSessionL();
		TInt privacyId;
		TRAP_IGNORE(iNetworkPrivacy->NotifyLocationRequestL(*iSuplReqInfo, privacyId); )
		iTrace->Trace(_L(" Calling InsertActiveSessionL"), KTraceFileName, __LINE__);
         
	}
	else
	if (!iPrivacyOverride)
	{
		iTrace->Trace(_L("Launch Notify dialog second if condition"), KTraceFileName, __LINE__);
		CTriggerParams* aParamValues = CTriggerParams::NewL();
		iSuplStorageSettings->GetTriggerParams(iSETSessionUniqueId,aParamValues);
		TInt64 sessionId;
    TBool notificationPresent;
    TBool triggerNotificationStatus;
    CTriggerParams::TTriggerType triggerType;
    CTriggerParams::TRequestType requestType;
    TUint64 outstandingTrigger;
    TUint64 interval;
    TTime endTime;

	HBufC* sessionName = HBufC::NewL( KMaxSessionNameLength );
	
		aParamValues->Get(sessionId,sessionName->Des(),notificationPresent,triggerNotificationStatus,
                           triggerType,requestType,outstandingTrigger,interval);
		aParamValues->GetTriggerEndTime(endTime);                                                            
                           //triggerNotificationStatus = ETrue;
	
		UpdateActiveTriggerSession(); //Update UI for trigger info...
		
    if (triggerNotificationStatus)
    {
    	iTrace->Trace(_L("calling notifylocreqL in second if"), KTraceFileName, __LINE__);
    	if (iSuplReqInfo)
				iTrace->Trace(_L("iSuplReqInfo is not null"), KTraceFileName, __LINE__);
		
		if (iNetworkPrivacy)
				iTrace->Trace(_L("iNetworkPrivacy is not null"), KTraceFileName, __LINE__);
				
				
				
    	TRAP_IGNORE(iNetworkPrivacy->NotifyLocationRequestL(*iSuplReqInfo, iRequestorId);)
    	
    }
    
    delete sessionName;
    delete aParamValues;
    
                                                  
	}
	
}

TUint COMASuplSession::GetPortNumUsed()
{
	return iPortNum;
}

//End of File
