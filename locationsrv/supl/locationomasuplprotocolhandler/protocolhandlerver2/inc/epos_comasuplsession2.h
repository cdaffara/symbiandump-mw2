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
* Description:   This class provides the the implementation for CSuplSessionBase
*								 to provide the session objects for OMA SUPL Protocol Handler
*
*/



#ifndef C_COMASUPLSESSION2_H
#define C_COMASUPLSESSION2_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <e32cmn.h> 

#include "epos_suplterminalparams.h"
#include "epos_comasupliapchangenotifier.h"
#include "epos_comasupletelnotifier.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplsessionbase.h"
#include "lbs/epos_momasuplobserver.h"
#include "lbs/epos_comasuplpossessionbase.h"

#include "epos_comasuplasnmessagebase.h"
#include "epos_comasuplend.h"
#include "epos_comasupltriggeredstart.h"

#include "epos_momasuplconnobserver.h"
#include "epos_comasupluicompletionobserver.h"
#include "epos_comasupletelcompletionobserver.h"
#include "epos_momasuplprivacyobserver.h"
#include "epos_comasuplcompleteselfrequestor.h"
#include "epos_comasuplstate.h"
#include "epos_omasuplconstants.h"
#include "epos_momasupltimeoutnotifier.h"
#include "epos_tomasuplposmethod.h"
#include "epos_momasupldialogtimeoutnotifier.h"
#include "epos_csuplsettingsinternal.h"
//#include "epos_comasuplprotocolmanager2.h"

#include <lbs/epos_cposnetworkprivacy.h>
#include <lbs/epos_cpossuplprivacyrequestinfo.h>

class RMobilePhone;
class TOMASuplVersion;
class COMASuplSessionID;
class CSuplCommunicationManager;
class COMASuplConnRequestor;
class COMASuplTrace;
class COMASuplSettings;
class COMASuplState;
class COMASuplPosition;
class MOMASuplSessionObserver;
class TPositionInfo;
class COMASuplPosHandlerBase;
class COMASuplPosSessionBase;
class COMASuplSETCapabilities;
class COMASuplPOSRequestor;
class CSuplSettings;
class COMASuplTimeoutTimer;
class COMASuplDialogTimer;

class COMASUPLProtocolManager2;
class COMASuplAsnHandlerBase;

struct TCellIdInfo
    {
	    TInt iMNC;
	    TInt iMCC;
	    TInt iCid;
	    TInt iLac; 
    };

//class COMASuplETelNotifier;
/**
*  SUPL Session Object in OMA SUPL Protocol Handler plug-in
*  This class is responsible for processing and generating 
*  the OMA SUPL protocol messages
*
*  @lib EPosOMASUPLProtocolHandler.lib
*  @since S60 v3.1u
*/

class COMASuplSession : public CSuplSessionBase,
						public MOMASuplConnObserver,
                        public MOMASuplUICompletionObserver,
						public MCompleteSelfRequest,
						public MOMASuplMsgStateObserver,
						public MOMASuplObserver,
						public MOMASuplPrivacyObserver,
						public MOMASuplTimeOutNotifier,
                        public MOMASuplEtelCompletionObserver,
                        public MOMASuplIapChangeNotifierObserver,
                        public MOMASuplDialogTimeOutNotifier
{

  public:  // Constructors and destructor
    
	    // State Enums
	  enum TOMASuplSessionState
			{
				ESUPL_INITIALIZED = 0x001,
				ESUPL_CONNECTING,
				ESUPL_GENERATE,
				ESUPL_ENCODE,
				ESUPL_SEND,
				ESUPL_RECEIVED,
				ESUPL_SESSION_ERROR,
				ESUPL_TERMINATED
			};
			
			enum TOMASuplMsgType
			{
				ESUPL_START,
				ESUPL_RESPONSE,
				ESUPL_INIT,
				ESUPL_POSINIT,
				ESUPL_POS,
				ESUPL_END,
				ESUPL_AUTH_REQ,
				ESUPL_AUTH_RESP,
                ESUPL_TRIG_START,
                ESUPL_TRIG_RESPONSE,
                ESUPL_TRIG_STOP,
                ESUPL_REPORT
			};
			
			// Request Type enum
			enum TOMASuplReqType
			{
				ESUPL_NETWORK,
				ESUPL_TERMINAL,
				ESUPL_INVALID_SESSION,
				ESUPL_NETWORK_INVALID_SESSION
			};
        
	public:  // Constructors and destructor        

			/**
			* NewL,Two-phased constructor.
			* @since S60 v3.1u
			* @param aCommManager,communication manager for sending packet 
			*						to SLP CSuplCommunicationManager 
			* @param aMobilePhone,RMobilePhone
			* @param aSettings ,settings for holding HSLP IP address & IAP Id.
			* @param aRequestType,Type of request SET , NET or Invalid session
			* @param aSessionObserver,observer for this class.
			* @return None
			*/
		    static COMASuplSession* NewL(
		    							CSuplCommunicationManager& aCommManager,
										RMobilePhone& aMobilePhone,
										COMASuplSettings*& aSettings,
										CSuplSettingsInternal* aSettingsStorage,
										TOMASuplReqType aRequestType,
										MOMASuplSessionObserver &aSessionObserver,
										COMASuplPosHandlerBase *aPosHandler,
										TDes& aIMSI,
										COMASUPLProtocolManager2 &aProtoMgr,
										TInt aIpcSessionId,
										COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase
                                    );
	private:        

			/**
			* Destructor.
			*/
			 ~COMASuplSession();

	public:

			/**
			* Delete Session Explicitly.
			* @since  S60 v3.1u
			* @param  None
			* @return None
			*/
			void DestroySession();
			/**
	        * Handles Pos handler unload.
	        * @since  S60 v3.1u
	        * @param  None
	        * @return None
	        */         
	         void PosPlugInUnloaded();

	public: //For all Observers
	
	        /**
	        * OperationComplete Method, Completion of any Async operation
	        * @since S60 v3.1u
	        * @param aErrorCode, Error code,if any
	        * @return None
	        */
			void OperationCompleteL(TInt aErrorCode);
			
			/**
	        * GetPositionComplete Method, Completion of get position opertaion from Pos Module
	        * @since S60 v3.1u
	        * @param aErrorCode, Error code,if any
	        * @return None
	        */
			void GetPositionCompleteL(TInt aError);
			
        	/** 
            * This callback method is used to notify the client about 
            * the completion of UI launch
            * @param aError - Error during Launch
            * @return None
            */
        	void SettingsUICompletedL(TInt aError);
		
            /** 
            * This callback method is used to notify the client about 
            * the completion of UI
            * @return None
            */
					void SettingsUsageUICompletedL(TInt /*aError*/) {};
			
            /** 
            * This callback method is used to notify the client about 
            * the completion of UI 
            * @return None
            */
			void SettingsTimeOutUICompletedL(TInt aError);
			
	public: 			 
			/**
			* InitializeL Method.
			* @since S60 v3.1u
			* @param aRequestID, Request Identification number.
			* @return None
			*/
			virtual void InitializeL(TInt aRequestID);

			/**
			* CleanUp Method.
			* @since S60 v3.1
			* @param None
			* @return None
			*/
			virtual void CleanUp();

			/**
			* RunSuplSession Method.
			* @since S60 v3.1
			* @param aStatus, aSynchronous service request   
			* @param aAllowedCapabilities, Capabilities of SET
			* @param aSessionIdSeed, seed value of session
			* @param aRequestID, Request Identification number.
			* @return None
			*/
			virtual void RunSuplSessionL(
								TRequestStatus& aStatus, 
								TBool aFirstReq, 
								const TDesC& aHslpAddress, 
								TBool aFallBack, 
								TInt aAllowedCapabilities, 
								TInt aSessionIdSeed, 
								TInt aRequestID = 0);
										 
			/**
			* RunSessionL, Starts the SUPL Sesssion for Terminal Initiated Location Request
			* @since S60 v3.1
			* @param       aStatus, 	  On completion, sets status of the request
			* @param       aAllowedCapabilities , Allowed capabilities of SET
			* @param 	   aSessionIdSeed, seed value of session
			* @Param:      aQop,  specifies the quality of position
			* @param       aRequestID , Request ID of Session
			* @return      None
			*/
		    virtual void  RunSuplSessionL(
		    					TRequestStatus& aStatus, 
		    					TBool aFirstReq,
		                        const TDesC& aHslpAddress, 
		                        TBool aFallBack,
		  						TInt aAllowedCapabilities,
		  						TInt aSessionIdSeed,
		  						TSuplTerminalQop& aQop, 
		  						TInt aRequestID = 0);
										 
			/**
			* RunInvalidSessionL Method
			* @since S60 v3.1u
			* @param aSessionId, the invalid session Id received by COMASuplProtocolManager
			* @return None
			*/	 
			virtual void RunInvalidSessionL(COMASuplAsnMessageBase* aDecodedMsg);


			/**
			* RunTriggerSessionL Method.
			* @since S60 v3.1
			* @param aStatus, aSynchronous service request   
			* @param aAllowedCapabilities, Capabilities of SET
			* @param aSessionIdSeed, seed value of session
			* @param aRequestID, Request Identification number.
			* @return None
			*/
			virtual void RunTriggerSessionL(
								TRequestStatus& aStatus, 
                                TSuplTerminalPeriodicTrigger& aTriggerParams,
			                    const TDesC& aHslpAddress, 
			                    TBool aFallBack,
                                TInt aAllowedCapabilities, 
							 	TInt aSessionIdSeed, 
							 	TInt aRequestID = 0);

			/**
			* NotifyTriggerFiredL Method.
			* @since S60 v3.1
			* @param aStatus, aSynchronous service request   
			* @param aAllowedCapabilities, Capabilities of SET
			* @param aSessionIdSeed, seed value of session
			* @param aRequestID, Request Identification number.
			* @return None
			*/
			virtual void NotifyTriggerFiredL(
								TRequestStatus& aStatus, 
                                TSuplTriggerFireInfo& aFireInfo);
			/**
			* CancelTriggerSession Method.
			* @since S60 v9.2TB
			* @return None
			*/
            void CancelTriggerSession();
			/**
			* HandleInvalidSLPSession Method
			* @since S60 v3.1u
			* @none
			* @return None
			*/	 
			void HandleInvalidSLPSession();

			/**
			* GetPosition Method.
			* @since S60 v3.1u
			* @param aSuplPosInfo,
			* @return TInt 
			*/
			virtual TInt GetPosition(TPositionInfo& aSuplPosInfo);        									 

			virtual TInt GetServerAddressL(TDes& ahslpAddress);
			/**
			* GetPosition Method.
			* @since S60 v3.1u
			* @param aSuplPosInfo,HPositionGenericInfo
			* @return TInt 
			*/
			virtual TInt GetPosition(HPositionGenericInfo& aSuplPosInfo);        									 
			/**
			* SetEncodedSuplInitL Method.
			* @since S60 v3.1u
			* @param aEncodedSuplInit A HBufC pointer holding the
			* encoded SUPL INIT packet
			* @return none
			*/
			void SetEncodedSuplInitL(HBufC8 *aEncodedSuplInit);       									 

		    void HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw);

		    void HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw);
            void HandleIapSettingsChangeL(TInt aErrorCode);
			/**
			* SuplIpcSessionID Method. Returns IPC session ID 
			* @since S60 v3.2
			* @param None
			* @return None
			*/
			TInt SuplIpcSessionID();

			/**
			* UpdateSuplUsage Method.  Updates SUPL usage 
			* @since S60 v3.2
			* @param None
			* @return None
			*/
			void UpdateSuplUsage();

			/**
			* SettingsChanged Method.  Updates flag to indicate that
            * settings has changed 
			* @since S60 v3.2
			* @param None
			* @return None
			*/
            void SettingsChanged();
			
    protected:  // New functions
        
    protected:  //COMASuplCompleteSelfRequestor
    
			/**
			* RequestCompleted Method.,Called after self complete completed
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
	    	void RequestCompletedL();
    	
    private:
			/**
			* C++ default constructor.
			* @since S60 v3.1u
			* @param aMobilePhone,RMobilePhone 
			* @param aRequestType,Type of request SET , NET or Invalid session
			* @param aSessionObserver,observer for this class.
			* @return None
			*/
			COMASuplSession(RMobilePhone& aMobilePhone,
											TOMASuplReqType aRequestType,
											COMASuplPosHandlerBase *aPosHandler,
											MOMASuplSessionObserver& aSessionObserver,
											COMASuplSettings*& aSettings,
											COMASUPLProtocolManager2 &aProtoMgr,
											TInt aIpcSessionId,
											COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase
                            );
		
			/**
			* By default Symbian 2nd phase constructor is private.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void ConstructL(CSuplCommunicationManager &aCommManager, 
			                CSuplSettingsInternal* aSettingsStorage,
			                TDes& aIMSI);
	private:
	
			/**
			* GetLocalIPAddress Method. Reads Local IP address
			* @since S60 v3.1u
			* @param None,
			* @return TUint32, IPAddress in TUint32 form 
			*/
			TInt GetLocalIPAddress();	

			/**
			* UpdateSuplSessionIDL Method,Updates SLP sessionId,which got from SLP
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void UpdateSuplSessionIDL();	

			/**
			* UpdateSLPSessionID Method,Updates SLP sessionId,which got from SLP
			* @since S60 v3.1u
			* @param aSessionID,SLP Session id by which current sessions id to update.
			* @return None
			*/
			void UpdateSLPSessionIDL(COMASuplSessionID *aSessionID);

			/**
			* UpdateSLPSessionID Method,Updates SLP sessionId,which got from SLP
			* @since S60 v3.1u
			* @param aSessionID,SLP Session id by which current sessions id to update.
			* @return None
			*/
			void UpdateSETSessionIDL(COMASuplSessionID *aSessionID);

			/**
			* CreateCloneMessageL Method,Creates Clone of COMASuplAsnMessageBase
			* @since S60 v3.1u
			* @param @aDecodedAsnMessage,Clone to be create COMASuplAsnMessageBase
			* @return None
			*/
			void CreateCloneMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage);

			/**
			* GenerateSuplMessage Method,generates SUPL Message
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplMessageL();

			/**
			* GenerateSuplStartL Method,generates SUPL_START Message
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplStartL();		

			/**
			* GenerateSuplTriggerStartL Method,generates SUPL_TRIGGER_START Message
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplTriggerStartL();		

			/**
			* GenerateSuplTriggerStopL Method,generates SUPL_TRIGGER_STOP Message
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplTriggerStopL();		
			/**
			* GenerateSuplPosInitL Method,generates SUPL_POS_INIT Message
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplPosInitL();		
		
			/**
			* GenerateSuplEndL Method,generates SUPL_END Message
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplEndL();				
				
			/**
			* GenerateSuplPosL Method,generates SUPL_POS Message
			* @since Series 60 3.1u
			* @param None
			* @return None
			*/
			void GenerateSuplPosL();				

			/**
			* ProcessSuplResponseL Method,processes SUPL_RESPONCE Message
			* @since S60 v3.1u
			* @param None
			* @return Message to be sent to SLP
			*/
			void ProcessSuplResponseL();		

			/**
			* ProcessSuplEndL Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void ProcessSuplEndL();		

			/**
			* ProcessSuplPosL Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			**/
			void ProcessSuplPosL();

			/**
			* ProcessSuplInitL Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			**/
			void ProcessSuplInitL();
			
			/**
			* ProcessSuplAuthResponceL Method.
			* @since S60 v3.1
			* @param None
			* @return None
			**/
			void ProcessSuplAuthResponceL();

			/**
			* ProcessSuplTriggerResponseL Method,processes SUPL_TRIGGER_RESPONSE Message
			* @since S60 v3.1u
			* @param None
			* @return Message to be sent to SLP
			*/
			void ProcessSuplTriggerResponseL();		

			/**
			* ProcessSuplReportL Method,processes SUPL_REPORT Message
			* @since S60 v3.1u
			* @param None
			* @return Message to be sent to SLP
			*/
			void ProcessSuplReportL();		

			/**
			* ProcessSuplTriggerStopL Method,processes SUPL_TRIGGER_STOP Message
			* @since S60 v3.1u
			* @param None
			* @return Message to be sent to SLP
			*/
			void ProcessSuplTriggerStopL();		

			/**
			* CancelAllOperations Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void CancelAllOperations();		

			/**
			* CopySETCapabilities Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void CopySETCapabilities();	

			/**
			* ConnectionRequired Method,Checks for need of connection
			* @since S60 v3.1u
			* @param None
			* @return True if neeeded
			*/
			TBool ConnectionRequired();
			/**
			* CancelAllOperations Method.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void CancelSession();		

			/**
			* ValidatePosition Method.
			* @since S60 v3.1u
			* @param aPosition of type COMASuplPosition
			* @return KErrNone if position is valid, KErrArgument otherwise.
			*/
            TInt ValidatePosition(COMASuplPosition *aPosition);
            
			/**
			* GetNetworkModeL Method.
			* @since S60 v3.1
			* @param None
			* @return 0 if phone is in offline mode , 1 for online mode
			*/
			TInt GetNetworkModeL();
			
			/**
			* CheckForSuplUsageL Method.  Checks for SUPL Usage in SUPL settings
			* @since S60 v3.2
			* @param None
			* @return None
			*/
			void CheckForSuplUsageL();

			/**
			* CheckForTimerExpiry Method.  Checks for timer expiry 
			* @since S60 v3.2
			* @param None
			* @return None
			*/
            void CheckForTimerExpiry();

			/**
			* CheckForPreviousResultL Method.  Checks for the previous request result
			* @since S60 v3.2
			* @param None
			* @return None
			*/
			void CheckForPreviousResultL();
		
	private: //Error Handling functions

			/**
			* HandleSuplError Method. Handles all the errors occurs while processing session.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleSuplErrorL(TInt aErr);	

			/**
			* HandleSuplError Method. Handles all the errors occurs while generating message for session.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleGenerationErrorL(TInt aErr);

			/**
			* HandleSuplError Method. Handles all the errors occurs while initilization of session.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleInitilizationErrorL(TInt aErr);

			/**
			* HandleSuplError Method. Handles all the errors occurs while connection to SLP.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleConnectionErrorL(TInt aErr);

			/**
			* HandleSuplError Method. Handles all the errors occurs while encoding message.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleEncodingErrorL(TInt aErr);

			/**
			* HandleSuplError Method. Handles all the errors occurs while sending packet to SLP.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleSendErrorL(TInt aErr);
				
			/**
			* HandleSuplError Method. Handles all the errors occurs while receiving packet.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandlePacketsErrorL(TInt aErr);

			/**
			* HandleSuplInitErrorL Method. Handles all the errors during processing of SUPL_INIT.
			* @since S60 v3.1u
			* @param aErr,error code 
			* @return None,
			*/
			void HandleSuplInitErrorL(TInt aErr);

			/**
			* StartInvalidSession Method,creates invalid session.
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void StartInvalidSessionL();		
			
	public: //Handle error 
	
			/* HandleInvalidParameterErrorL Method. 
			* @since S60 v3.1u
			* @param aDecodedAsnMessage, Decoded ASN message, received from SLP  
			* @return None
			*/
			void HandleInvalidParameterErrorL(COMASuplAsnMessageBase* aDecodedAsnMessage);
			
			/* HandleWrongSuplVersion Method. 
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void HandleWrongSuplVersionL();
			
			/* HandleInvalidMessageL Method. 
			* @since S60 v3.1
			* @param None
			* @return None
			*/
			void HandleInvalidMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage);
			
			/* HandleInvalidMessageLengthL Method. 
			* @since S60 v3.1
			* @param None
			* @return None
			*/
			void HandleInvalidMessageLengthL();
	public:
	
			/**
			* SetSETSessionID Method,Sets SET SessionID of Session
			* @since S60 v3.1u
			* @param aSessionId, unique no to assign to SET session id.
			* @return None
			*/
			void SetSETSessionID(TInt aSessionId);

			/**
			* SuplSessionID Method. returns SessionID of Session
			* @since S60 v3.1u
			* @param None,
			* @return SETSessionID of session
			*/
			COMASuplSessionID* SuplSessionID() const;

			/**
			* HandleOMASuplMessage Method. 
			* @since S60 v3.1u
			* @param aDecodedAsnMessage, Decoded ASN message, received from SLP  
			* @return None
			*/
			void HandleOMASuplMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage);
			
			/**
			* HandleOMASuplMessage Method. 
			* @since S60 v3.1u
			* @param aDecodedAsnMessage, Decoded ASN message, received from SLP  
			* @return None
			*/
			void HandleOMASuplMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage,TRequestStatus& aStatus,TInt aSeedValue,TInt aErrorCode);

			/**
			* RequestType Method. 
			* @since S60 v3.1u
			* @param None
			* @return Two possible values - Network Initiated Request, Terminal Initiated Request
			*/
			COMASuplSession::TOMASuplReqType RequestType();

			/**
			* SessionEnd Method,Ends the session
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void SessionEnd();
				
			/**
			* CompleteSession Method.
			* @since S60 v3.1u
			* @param aCompletionCode, Request completion code
			* @return None
			*/
			void CompleteSession(TInt aCompletionCode);
			
			/**
			* CancelRunSession Method,cancels session
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			virtual void CancelRunSession();
			
			/**
			* SetConfigurationParameters Method,Sets different setting from CR
			* @since S60 v3.1u
			* @param aUT1_StartTimer, SUPL_START Timer value
			* @param aUT2_PosInitTimer, SUPL_POS_INIT Timer value
			* @param aUT3_PosTimer, SUPL_POS Timer value
			* @param aPrivacyTimer, privacy timer value
			* @param aSLPMode, Mode of SLP Proxy Non Proxy
            * @param aSuplUsage, SUPL Usage mode
            * @param aPersistFailTimer, Timer value for persistent failure
			* @return None
			*/
			void SetConfigurationParameters(TInt& aUT1_StartTimer,TInt& aUT2_PosInitTimer,
											TInt& aUT3_PosTimer,  TInt& aPrivacyTimer, 
											TInt& aSLPMode,
                                            TInt& aSuplUsage,
                                            TInt& aPersistFailTimer,
                                            TInt& aSuplInitTimeOut);
			/**
			* CloseAllOperations Method,Closes session
			* @since S60 v3.1
			* @param None
			* @return None
			*/
			void CloseAllOperations();
			
	public://MOMASuplObserver			

			/**
			* SuplVersion Method.
			* @since S60 v3.1u
			* @param None
			* @return TReal,version of protocol
			*/
			TReal SuplVersion();
			
			/**
			* TerminateSession Method,terminates session
			* @since S60 v3.1u
			* @param None
			* @return None
			*/
			void TerminateSession();
		
	public:	//MOMASuplPrivacyObserver
			/**
			* PrivacyCheckCompleteL Method.
			* @since S60 3.1u
			* @param aErrorCode, error code - Specifying whether accepted or rejected
			* @return None
			*/ 
			void PrivacyCheckCompleteL(TInt aErrorCode);
			
	public:
			/**
			* CheckProtocolVersion, Checks protocol version
			* @since S60 v3.1          
			* @param      None. 
			* @return     True if version of received SUPL message is matching with supported version.
			*/
			TBool CheckProtocolVersionL(COMASuplAsnMessageBase* aDecodedAsn);
    private: // UI
			/**
			* SessionUIFlag, sets session UI flag 
			* @since S60 v3.2          
			* @param      None. 
			* @return     None 
			*/
			void SetSessionUIFlag(TBool aValue);
			
  public://UI
            TBool GetSessionUIFlag();            
            
	public://Active Sessions
	
						TBool AreYouHandlingThisSession(TInt64& aActiveSessionId);
						
	private: //internal			

			/**
			* SetPOSMsgPluginState Method,Sets state of POS Session
			* @since S60 v3.1u
			* @param aSuplMsgState,state of message
			* @return None
			*/
			void SetPOSMsgPluginState(COMASuplPosSessionBase::TOMASuplMsgState aSuplMsgState);
			
			/**
			* IsLastPOSMessage Method,Checks for last message from plugin
			* @since S60 v3.1u
			* @param aSuplMsgState,state of message
			* @return None
			*/
			TBool IsLastPOSMessage();
			
			/**
			* IsSegmentationDone Method,Checks segmentation
			* @since S60 v3.1
			* @param None
			* @return TBool,ETrue it is done by posstate class
			*/
			TBool IsSegmentationDone();
			
			/**
			* IsGenerationInProgress Method,checks about generation status of message
			* @since S60 v3.1u
			* @param None
			* @return TBool
			*/
			TBool IsGenerationInProgress();
			
			/**
			* LogEndTime Method,logs end  time, if in Qop , delay is present
			* @since S60 v3.1
			* @param None
			* @return None
			*/
			void LogEndTime();
			
			/**
			* SetIMSIL Method, Updates Session ID with IMSI if specified
			* @since S60 v3.1
			* @param None
			* @return TInt, error code 
			*/
			void SetIMSI();
			
			/**
			* IsIMSIVariant Method,checks if IMSI needs to be included in SUPL message
			* @since S60 v3.1u
			* @param None
			* @return TBool
			*/
			TBool IsIMSIVariant();
			
			/**
			* GetRemainingPosPayloadL Method,Retrives if any pos payload is remainig dutin POS_INIT message
			* @since S60 v3.1
			* @param None
			* @return Pospayload which needs to send
			*/
			COMASuplPosPayload* GetRemainingPosPayloadL();
			
			/**
			* IsWholeMessageSent LMethod,
			* @since S60 v3.1
			* @param None
			* @return ETrue 
			*/
			TBool IsWholeMessageSentL();

			/**
			* IsUIDisplayedL Method,
			* @since S60 v3.1
			* @param None
			* @return TBool 
			*/
            TBool IsUIDisplayedL();
            /**
			* SetApproriateSETSessionId  Method,Sets the appropriate session id
			* @since S60 v3.2
			* @param None
			* @return TInt 
			*/
            TInt SetApproriateSETSessionId();

            /**
			* CheckForPersistentFailure Method, checks for persistent failure
			* @since S60 v3.2
			* @param None
			* @return TInt 
			*/
            void CheckForPersistentFailure(TInt aErrorCode);

            
			/**
			* Timeout Notifier Method.
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/
			virtual void TimerExpiredL();
            
			/**
			* Dialog Timeout Notifier Method.
			* @since Series 60 9.1 TB
			* @param None
			* @return None
			*/
            virtual void DialogTimerExpiredL();
    public:  // Usage dialog
            void SetSuplUsageFlag();
            void ReSetSuplUsageFlag();
            TBool GetSuplUsageFlag();
            void StartUsageDialogLaunchL();
            TBool IsEtelNotifySet();
            TBool IsEtelRoamingSet();
			TUint GetPortNumUsed();
            void StartPeriodicTriggerTimer();
            void GetPeriodicTriggerParams();
            void StartTriggerPosSessionL();
            
	 private:
	 
	 	void StoreCurrentCellIDL(HPositionGenericInfo& aSuplPosInfo);		
	 	TInt ServerAddressCheckForSuplInitL();
	 	
	 private://active session
	     void InsertActiveSessionL();
	     void RemoveActiveSession();
	     void UpdateActiveTriggerSession();
	     
	     
	 		 
	private:  		//APE CENTRIC	 		  
	
	void GenerateRequestID();		//APE CENTRIC	 		
	//void ReadRequestIDFeatureFlagL(); //APE CENTRIC 
	
	void LaunchNotifyDialogL();     // Vijay for UI reqmt
	
	
	private:    // Data


            COMASuplIapNotifier* iIapNotifier;
    
			// Asynchronous request 
			TRequestStatus* iRunRequestStatus; 

			// Request ID of SuplSession
			TInt iRequestID;

			//Supl Version
			TOMASuplVersion iSuplVersion;

			//SET capabilities
			COMASuplSETCapabilities* iSETCapabilities;
			
			//Allowed  capabilities
			TInt iAllowedCapabilities;
			
			//State of Supl Session
			TOMASuplSessionState iSuplSessionState;

			// Message Type
			TOMASuplMsgType iSuplMsgType;

			//Session ID
			COMASuplSessionID* iSuplSessionId; 

			// Connection Requestor
			COMASuplConnRequestor *iConnRequestor;

			//Connected RMobilePhone session
			RMobilePhone& iMobilePhone;
			
			//Type of Request SET OR NET
			TOMASuplReqType iRequestType;

			//Encoded Buffer
			HBufC8* iEncodedBuffer; 

			//Trace Utility
			COMASuplTrace* iTrace;

			//Unique Identifier, holding id of SET session.
			TInt iSETSessionUniqueId;

			//Decoded ASN message
			COMASuplAsnMessageBase* iDecodedAsnMessage;

			//Self Request Completor
			COMASuplCompleteSelfRequestor* iCompleteSelfRequestor;

			//State holder
			COMASuplState *iSuplState;

			// Position for Terminal Initiated Cases
			COMASuplPosition* iPosition;

			//Error Code for SUPL END
			COMASuplEnd::TOMASuplStatusCode iErrorStatusCode;
			
			//POS Handler
			COMASuplPosHandlerBase* iPosHandler;

			//POS Session 
			COMASuplPosSessionBase* iPOSSession; 

			//POS Requestor
			COMASuplPOSRequestor* iOMASuplPOSRequestor;

        	// Reference to CSuplSettingsInternal
        	CSuplSettingsInternal* iSuplStorageSettings;

            //Trigger type for NI trigger
            COMASuplTriggeredStart::TOMASuplTrigType iTriggerType;
			
			// HSLP Address
			const TDesC8& iHSLPAddress;	
			
			// This buffer is used only in case we get a SUPL INIT
			// packet from the SLP. This is used to compute the
			// VER of the Encoded SUPL INTI packet
			HBufC8* iEncodedSuplInit;
			
			// Container to store IMSI
			RBuf iIMSI;
			
			// Session Observer for TerminateSession
			MOMASuplSessionObserver &iSessionObserver;

        	// Reference to OMA Supl Settings
        	COMASuplSettings*& iSuplSettings;
			
            // Reference to protocol manager to invoke settings UI
            COMASUPLProtocolManager2 &iProtocolManager;
			
			//UT1 Timer for SUPL_START
			TInt iUT1_StartTimer;

			//UT2 Timer for SUPL_POS_INIT
			TInt iUT2_PosInitTimer;

			//UT3 Timer for SUPL_POS
			TInt iUT3_PosTimer;

			//Privacy Timer
			TInt iPrivacyTimer;

			//SET Mode 
			TInt iSETMode;
			
            //SUPL Usage configuration
            TInt iSuplUsage;

            // Timer for persistent failure
            TInt iPersistFailTimer;

			TBool iIsQoPPresent;
			
			//Received QoP from Client Lib
			TSuplTerminalQop iClientQop;
			
						
			TTime iSessionStartTime;
			
			TTime iSessionEndTime;
			//Is POS message is First one or Not.
			TBool iIsFirstPOSMessage;

            // UI Flag
            TBool iUIFlag;
            
	      	TBool iSessionIDFlag;
	            
			//Decoded ASN message
			COMASuplAsnMessageBase* iDecodedNETAsnMessage;

            //Structure to hold trigger info
            TSuplTriggerFireInfo* iTrgFireInfo;

			TInt iNETErrorCode;
            TInt iRequestId;
			
			//Timeout Timer
			COMASuplTimeoutTimer* iTimer;

			COMASuplDialogTimer* iDialogTimer;
            //Roaming indicator
            TBool iRoaming;

            //Previous request result
            TInt  iPrevUsageResult;
            //Change flag to track if any change has happened
            TInt    iChanged;

            TInt  iIpcSessionId;

            TBool iNwInitError;

            TInt iNwInitCompletionCode;

            TBool iPersistFail;

            TInt iPrevReqResult;

			TInt iFailedCaps;

            TBool iCapsFail;

            TTime iFailTime;
            TBool iUsageDialog;

            TBool iUsageHomeNW;

            TOMASuplPosMethod iPosMethod;
            
            TBool iEtelNotify;

            TBool iEtelRoamingCheck;

            TCellIdInfo iCurrentCellId;

            TBool isTimeoutDialogTimerStarted;
            TInt iSuplInitTimeOut;
            TTime iDlgStartTime;
            TTimeIntervalSeconds iDiffTime;
            TBool iIapDialogShown;
            TBool iIapDlgTimerExpired;
            HBufC8* iHslpAddress;
            
            TInt iSessionCompletionCode;
            TInt iNetworkType;

            TBool iTriggerSession;
            
            COMASuplAsnHandlerBase* iOMASuplAsnHandlerBaseImpl; 

            TSuplTerminalPeriodicTrigger iTriggerParams;

            //Periodic trigger parameters
            TUint iNumFixes;
            TUint iInterval;
            TUint iStartTime;
            TUint iStartTimeCopy;
            TBool iTrgTimer;
            TBool iStartTimerFlag;
            TBool iTrgCancelRequest;
            TUint iTotalNoOfFixes; 
            TBool iClientNotify;
          //  TInt iActiveSessionId;
            RBuf iClientName;
			TBool iDisconnected;
            
			RBuf iRequestorName;
            CPosNetworkPrivacy*	iNetworkPrivacy;  //Vijay for UI reqmt
            TBool iPrivacyOverride;               //Vijay for UI reqmt
            TInt iGSMRequestId;										//Vijay for UI reqmt
            TBool iIsNotificationOnly;            //Vijay for UI reqmt
            CPosSUPLPrivacyRequestInfo* iSuplReqInfo;//Vijay for UI reqmt
            
            TInt iRequestorId;//Vijay for UI reqmt
            
            TBool iRequestIDFeatureFlag;//APE Centric
			
			TUint iPortNum;
    };

#endif      // C_COMASUPLSESSION_H
            
// End of File
