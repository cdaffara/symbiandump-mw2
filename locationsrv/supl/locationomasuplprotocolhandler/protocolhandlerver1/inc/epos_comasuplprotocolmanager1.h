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
* Description:   Header file for COMASUPLProtocolManager1 implementation
*
*/



#ifndef C_COMASUPLPROTOCOLMANAGER1_H
#define C_COMASUPLPROTOCOLMANAGER1_H

#include <e32cmn.h> 
#include <etel.h> 
#include <etelmm.h> 

//#include "epos_csuplprotocolmanagerbase.h"   // interface
#include "epos_comasuplprotocolmanager.h"
#include "epos_msuplsettingsobserver.h"
#include "epos_comasuplsession.h"
#include "epos_msuplpacketnotifier.h"

#include "epos_momasuplecomeventobserver.h"

#include "epos_csuplsettingsinternal.h"

#include "epos_comasuplasnbase.h"

class COMASuplTrace;
class CSuplCommunicationManager;
class RMobilePhone;
class RTelServer;
class COMASuplSettings;
class COMASuplPosHandlerBase;
class COMASuplSETSessionID;
class COMASuplSLPSessionID;

class COMASuplEcomEventWatcher;
class TSuplTerminalQop;
class CSuplSettings;
class COMASuplTimeoutTimer;
class COMASuplETelNotifier;

/**
 *  MOMASuplSessionObserver observer for session termination operation. 
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1
 */

class MOMASuplSessionObserver
	{
public:
		/**
		* TerminateSession
		* @since S60 v3.1u
		* @param aSession, session which is going to terminate
		* @param aErrorCode, error code for completion of request. 
		*/
		virtual void TerminateSession(CSuplSessionBase *aSession, TInt aErrorCode) = 0;
	};


	/**
	*  An implementation of the COMASUPLProtocolManagerBase definition. 
	*  @lib eposomasuplprotocolhandler.lib
	*  @since S60 v3.1
	*/
 
class COMASUPLProtocolManager1 : public COMASUPLProtocolManager,
								public MOMASuplSessionObserver,
								public MSuplEcomEventObserver,
								public MSuplSettingsObserver,
								public MOMASuplTimeOutNotifier,
                                public MOMASuplETelNotifierObserver
    {
    
public:

	/**
	* NewL, Create instance of concrete implementation. 
	* @since S60 v3.1u		
	* @param      None
	* @return    Instance of this class.
	*/
   	IMPORT_C static COMASUPLProtocolManager1* NewL();

	/**
	* ~COMASUPLProtocolManager1
	* @since S60 v3.1u		
	* @param      None
	* @return     None
	*/
  	IMPORT_C  ~COMASUPLProtocolManager1();

	/**
	* CreateNewSessionL, Creates new SuplSession instance
	* @since S60 v3.1u
	* @param      aRequestType, used for differentiation between SET & NET cases
	* @return     Instance of CSuplSessionBase.
	*/
  	IMPORT_C virtual CSuplSessionBase* CreateNewSessionL( 
										CSUPLProtocolManagerBase::TSuplReqType aRequestType, 
										TInt aIpcSessionId,
										RSuplTerminalSubSession::TSuplServiceType aSuplService);
  
	/**
	* RunSessionL, Starts the SUPL Sesssion for Terminal Initiated Location Request
	* @since S60 v3.1
	* @param       aSuplSession , session object on which processing will start
	* @param       aStatus, 	  On completion, sets status of the request
	* @param       aAllowedCapabilities , Allowed capabilities of SET
	* @Param:      aQop,  specifies the quality of position
	* @param       aRequestID , Request ID of Session
	* @return      None
	*/
  	IMPORT_C  virtual void  RunSessionL(CSuplSessionBase *const aSuplSession,TRequestStatus& aStatus, const TDesC& aHslpAddress, TBool aFallBack, TBool aFirstReq,
  										TInt aAllowedCapabilities,TSuplTerminalQop& aQop, TInt aRequestID = 0);

	/**
	* RunSessionL, Starts the SUPL Sesssion for Terminal Initiated Location Request
	* @since S60 v3.1
	* @param       aSuplSession , session object on which processing will start
	* @param       aStatus, 	  On completion, sets status of the request
	* @param       aAllowedCapabilities , Allowed capabilities of SET
	* @param       aRequestID , Request ID of Session
	* @return      None
	*/
  	IMPORT_C  virtual void  RunSessionL(CSuplSessionBase *const aSuplSession,TRequestStatus& aStatus,const TDesC& aHslpAddress, TBool aFallBack,
  										TBool aFirstReq, TInt aAllowedCapabilities, TInt aRequestID = 0);
	/**
	* HandleSuplMessage, Handles session in NET initiated case
	* @since S60 v3.1
	* @param       aSuplSession , session object on which processing will start
	* @param       aStatus, 	  On completion, sets status of the request
	* @param       aMessage,	message to be decoded.
	* @return      None
	*/
  	IMPORT_C  virtual void  HandleSuplMessageL(CSuplSessionBase *const aSuplSession,TRequestStatus& aStatus,HBufC8* aMessage);

	/**
	* DestroySession, Destroys the specified Supl Session instance
	* @since S60 v3.1u
	* @param      aSuplSession , session object which has to delete
	* @return     TInt,returns KErrNone if deleted successfully else KErrArgument  
	*/
  	IMPORT_C  TInt  DestroySession(CSuplSessionBase* aSuplSession);	
  
	/**
	* InitializeL, Does the asynchonous initilization. 
	* @since S60 v3.1u
	* @param      aCommManager,Communication Manager
	* @param      aInitilizationRequestStatus, TRequestStatus for asynchronous initilization operation
	* @return     None
	*/
  	IMPORT_C  void InitializeL(CSuplCommunicationManager* aCommManager,TRequestStatus& aInitilizationRequestStatus);
  
     /**
     * Function:     CancelInitialize, 
     * @Description: Does the cancellation of asynchonous initilization. 
     * @Param:       None
     * @Returns:     None
     */
  	IMPORT_C  void CancelInitialize();

     /**
     * Function:     DeInitialize, 
     *
     * @Description: Does the de-initilization. 
     *
     * @Param:       aDeInitRequestStatus,for asynchronous de-initilization of protocol manager
     *
     * @Returns:     None
     */
  	IMPORT_C void DeInitialize(TRequestStatus& aDeInitRequestStatus);

     /**
     * Function:     CancelDeInitialize, 
     *
     * @Description: Does the cancellation of asynchonous de-initilization. 
     *
     * @Param:       None
     *
     * @Returns:     None
     */
  	IMPORT_C void CancelDeInitialize();
  
	/**
	* HandleSuplPackets,Process Supl message which are comming from SLP
	* @since S60 v3.1u
	* @param       aPacket, message packet
	* @return      None
	*/
              IMPORT_C void HandlePacket(const TDesC8& aPacket, TUint aPortNum);

	/**
	* HandlePacketError,Process Supl message receiving Errors 
	* @since S60 v3.1u
	* @param      aError, the Error Code
	* @return     None
	*/
  	IMPORT_C virtual void HandlePacketError(TInt aError);
  
	/**
	* CancelRunSession,Cancels an in-progress Terminal Initiated Location Request
	* @param       aSession, The session on which the request has to be cancelled
	* @return      None
	*/
  	IMPORT_C void CancelRunSession(CSuplSessionBase* aSuplSession);
	
	  IMPORT_C   COMASuplSession* FindSessionOnPortNum(TUint aPortNum) const ;
  
     /**
     * Function:     RunSuplTriggerSessionL, 
     *
     * @Description: Starts processing on session
     *
     * @Param:       aSuplSession , session object on which processing will start
     * @Param:       aStatus, On return, the status of the request
     * @Param:       aAllowedCapabilities , Allowed capabilities of SET
     * @Param:       aPTrigger , specifies the periodic trigger session
     * @Param:       aRequestID , Request ID of Session
     *
     * @Returns:     None
     */
  	void  RunSuplTriggerSessionL(
  					CSuplSessionBase *const aSuplSession,
  					TRequestStatus& aStatus,
  					TSuplTerminalPeriodicTrigger& aPTrigger,
  					const TDesC& aHslpAddress, 
  					TBool aFallBack,
  					TInt aAllowedCapabilities,
  					TInt aRequestID = 0
  		);

   	/**
     * Function:     NotifyTriggerFired
     *
     * @Description: Starts listening for the trigger fired event
     *
     * @Param:       None
     *
     * @Returns:     None
     *
     */
  	void NotifyTriggerFiredL(
    				 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus, 
        			 TSuplTriggerFireInfo& aFireInfo
    	);
 
public : // From base class, MOMASuplSessionObserver

	/**
	* TerminateSession,Terminate the SUPL Session specified by the paramerter
	* @since S60 v3.1u
	* @param       aSession, the SUPL Session to be terminated
	* @param       aErrorCode,error code by which session completes
	* @return      None
	*/
	virtual void TerminateSession(CSuplSessionBase *aSession, TInt aErrorCode);
	
public: 

	// For setting UI observer    
	//TInt LaunchSettingsUI(MOMASuplUICompletionObserver* aObserver,const TDesC& aForHslp);
	//TInt LaunchSuplUsageSettingsUI(MOMASuplUICompletionObserver* aObserver, TBool aRoaming);
	//TInt LaunchSuplDialogTimeoutUI(MOMASuplUICompletionObserver* aObserver );
	void UpdateAllSubSessnsInSameSession(TInt aIpcSessionId);

	void SettingsChanged();

	TInt GetLastUsedAccessPoint(TDes& aLastlyUsedAccessPoint,TUint32& iIAPId);
	/** 
	* This callback method is used to notify the client about 
	* the completion of UI launch
	* @param aError - Error during Launch
	* @return None
	*/
	void SettingsUICompletedL(TInt /*aError*/) {};

	/** 
	* This callback method is used to notify the client about 
	* the completion of UI launch
	* @param aError - Error during Launch
	* @return None
	*/
	void SettingsUsageUICompletedL(TInt /*aError*/) {};

	/** 
	* This callback method is used to notify the client about 
	* the completion of UI completion
	* @param aError - Error during Launch
	* @return None
	*/
	void SettingsTimeOutUICompletedL(TInt /*aError*/) {};
	
public: //From base class MSuplSettingsObserver

	/**
	* HandleSuplSettingsChangeL,Observeed function, gets called.
	* @since S60 v3.1
	* @param       aEvent,TSuplSettingsEventType, type of event.
	* @return      None
	*/
    virtual void HandleSuplSettingsChangeL(TSuplSettingsEventType aEvent,TInt aSlpId);                 

	/**
	* Handles Pos handler unload event
	* @since S60 v3.1u
	* @param       None
	* @param       None
	* @return      None
	*/
	void PlugInUninstalled();  
	
public: // Construction

    /**
     * COMASUPLProtocolManager1,Perform the first phase of two phase construction
     * @since S60 v3.1u
     * @param       None
     * @return      None
     */
     COMASUPLProtocolManager1();

    /**
     * ConstructL,Perform the second phase construction of a COMASUPLProtocolManager1 object.
     * @since S60 v3.1u
     * @param       None
     * @return      None
     */
    void ConstructL();

	/**
	* CreateNewSessionL,An overloaded version that takes the COMASuplSession::TOMASuplReqType
	*     			This additional method is required to take of invalid session ID
	* @since S60 v3.1u
	* @param       None
	* @return      None
	*/
    //virtual 
	CSuplSessionBase* CreateNewSessionL(
    						COMASuplSession::TOMASuplReqType aRequestType, 
    						TInt aIpcSessionId,
    						RSuplTerminalSubSession::TSuplServiceType aSuplService
    );
    
    
private:
    /**
     * InitilizationCompletedL, Gets called after initilization gets completed with error or without error.
     * @since S60 v3.1u          
     * @param      aError,Error code if any happen while initilization.
     * @return     None.
     */
	void InitilizationCompletedL(TInt aError);
	
	void DeInitilizationCompletedL(TInt aError);
												
private : //For internal use only       

	
	/**
	* CheckForSessionCount, Check Max session count,if its reaches to max then resets to Zero.
	* @since S60 v3.1u     
	* @param      None
	* @return     None
	*/
	void CheckForSessionCount();

	/**
	* ConnectToETelL, Connects to Telephony server & RMobilePhone
	* @since S60 v3.1u     
	* @param      None, 
	* @return     TInt, returns system wide error code
	*/
	TInt  ConnectToETelL();

	/**
	* DeleteAllSessions, Deletes all sessions,if any session is remaining in array
	* @since S60 v3.1u     
	* @param      None, 
	* @return     None
	*/
	void DeleteAllSessions();	
	
	/**
	* HandleOMASuplPackets, Handles OMA Packets,passes to approriate session
	* @since S60 v3.1u     
	* @param      aPacket, Packet received from gateway...(i.e. thro' Communication Manager)
	* @return     None
	*/
	void HandleOMASuplPacketsL(const TDesC8& aPacket, TUint aPortNum);

	/**
	* FindSession,finds session in sessions array,using decoded message 
	* @since S60 v3.1u     
	* @param      aRetrivedSessionID,Session which has to be find in sessions array. 
	* @return     CSuplSessionBase*, session which found in sessions array,if any.
	*/
	COMASuplSession* FindSession(COMASuplSessionID* aRetrivedSessionID,TBool& aInvalidSLPId, TBool& aInvalidSETId) const;

	/**
	*  GetSETSessionID, Retrives SET ID from COMASuplSessionID
	* @since S60 v3.1u
	* @param       aOMAsuplSessionID,Session ID. 
	* @return      COMASuplSETSessionID, SET ID retrived from aOMAsuplSessionID.
	*/
	COMASuplSETSessionID* GetSETSessionID(COMASuplSessionID* aOMAsuplSessionID) const;

	/**
	* GetSLPSessionID, Retrives SLP ID from COMASuplSessionID
	* @since S60 v3.1u     
	* @param      aOMAsuplSessionID,Session ID. 
	* @return     COMASuplSLPSessionID, SLP ID retrived from aOMAsuplSessionID.
	*/
	COMASuplSLPSessionID* GetSLPSessionID(COMASuplSessionID* aOMAsuplSessionID) const;
	
	/**
	* CompareSETSessionID, Compares two SET ID of two Sessions.
	* @since S60 v3.1u     
	* @param      aSuplSETID, SET Session ID. 
	* @param      aRetrivedSuplSETID, SET Session ID. 
	* @return     TBool, if SET Session id matches returns ETrue else EFalse
	*/
	TBool CompareSETSessionID(COMASuplSETSessionID* aSuplSETID,
							  COMASuplSETSessionID* aRetrivedSuplSETID) const;
	
    /**
     * CompareSLPSessionID, Compares two SLP ID of two Sessions.
     * @since S60 v3.1u          
     * @param      aSuplSLPID, SLP Session ID. 
     * @param      aRetrivedSuplSLPID, SLP Session ID. 
     * @return     TBool, if matches returns ETrue else EFalse
     */
	TBool CompareSLPSessionID(COMASuplSLPSessionID* aSuplSLPID,
							 COMASuplSLPSessionID* aRetrivedSuplSLPID) const;	

    /**
     * ProcessInvalidSessionL, Processes invalid session, if any comes
     * @since S60 v3.1u          
     * @param      aRetrivedSessionID,SessionID of SuplSession. 
     * @return     None.
     */
	void ProcessInvalidSessionL(COMASuplAsnMessageBase* aDecodedMsg);

    
    /**
     * ReadSuplUsage, Reads OMA Supl Usage.
     * @since S60 v3.1u          
     * @param      None. 
     * @return     None.
     */
   //void ReadSuplUsage();
	
    
    /**
     * ResetBufferingParameters, 
     * @since S60 v3.1          
     * @param      None. 
     * @return     None
     */
	void ResetBufferingParameters();
	
    /**
     * GetNetworkModeL, 
     * @since S60 v3.1          
     * @param      None. 
     * @return     None
     */
	TInt GetNetworkModeL();

	// For Logging purpose
	void LogPacket(const TDesC8& aPacket,COMASuplAsnMessageBase* decodedAsn);
	void LogAvailableSessions( COMASuplSETSessionID* aSuplSETID,COMASuplSLPSessionID* aSuplSLPID,TInt aTotalCount) const;
	void LogReceivedSessionIdL(COMASuplSessionID* aRetrivedSessionID);
	void PrintHex(const TDesC8& aBuffer,TInt aLine);
  void CheckOutstandingUsageUIRequestsL();

public: // MOMASuplTimeOutNotifier

	/**
	* Timeout Notifier Method.
	* @since Series 60 3.1M
	* @param None
	* @return None
	*/
	virtual void TimerExpiredL();

public:  // For ETEL requests
    void CheckForRoaming();
    void NotifyCurrentNetworkChange();
    void CancelNotify();

public:  // MOMASuplETelNotifierObserver
    void HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw);
    void HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw);  
    
public:  // COMASUPLProtocolManager
    void SetCommonConfig(COMASuplCommonConfig*& aCommmonConfig);

public:
    /*
     * Method used to make conversion request for retrieving position of a location id.
     * 
     * @since S60 v5.2
     * @param aSuplSessn session object on which processing will start,ownership is not transferred
     * @param aCellInfo contains cell id information and cell id type
     * @param aStatus On return, the status of the request
     * 
     */
    void MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,
                                                      TGeoCellInfo& aCellInfo,               
                                                      TRequestStatus& aStatus );
    /*
     * Method used to make cancel a conversion request for retrieving position of a location id,,ownership is not transferred.
     * 
     * @since S60 v5.2
     * @param aSuplSessn session object which is to be cancelled
     * 
     */
    void CancelLocationConversionRequest(CSuplSessionBase* aSuplSession);
	
public : // from MSuplSettingsObserver
    virtual void HandleSuplTriggerStatusChangeL(
                   CSuplSettings::TSuplTriggerStatus aSuplTriggerStatus );
           
private: // Data Members

	//Holds all session objects created by CreateNewSession
	RPointerArray<CSuplSessionBase> iSuplSessions;

	//No of Sessions created
	TInt iRunningSessionCount;

	//Communication Manager
	CSuplCommunicationManager* iCommManager;

	//Tracing utility
	COMASuplTrace* iTrace;

	// Etel Server
	RTelServer iTelServer;

	//For access to a mobile phone
	RMobilePhone iMobilePhone; 

	//TSYName
	TFileName iTsyName; 

	// OMA Supl Settings
	COMASuplSettings* iSuplSettings;

	//Pos Handler
	COMASuplPosHandlerBase *iPosHandler;

 
   COMASuplETelNotifier* iNotifier;

    // Session Observer for UI completion
    MOMASuplUICompletionObserver* iSessnUIObserver;

	// Asynchronous request status for De-Initilization
	TRequestStatus* iDeInitRequestStatus; 

    TBool iDeInitDone;
	
	//	UT1 Timer for SUPL_START
	TInt iUT1_StartTimer;
	
	//	UT2 Timer for SUPL_POS_INIT
	TInt iUT2_PosInitTimer;
	
	//	UT3 Timer for SUPL_POS
	TInt iUT3_PosTimer;
	
	//Privacy Timer
	TInt iPrivacyTimer;
	
	//SET Mode
	TInt iSETMode;

    //SUPL Usage configuration
    TInt iSuplUsage;
 
   // Timer for persistent failure
    TInt iPersistFailTimer;

	//For ECom unload event
	COMASuplEcomEventWatcher* iSuplEcomEventWatcher;
	
	//SUPLSettings
	CSuplSettingsInternal *iSuplStorageSettings;
	
	// Container to store IMSI value
	RBuf    iIMSI;
	
	RBuf8 iQueuedPacket;
	
	TInt iPreviousMessageLength;
	
	TBool iIsPreviouslyDecodingFailed;
	
	//Timeout Timer
	COMASuplTimeoutTimer* iTimer;

    TBool iNotifyStarted;
    TBool iRoamingCheckStarted;
    TInt iSuplInitTimeOut;
    TBool iTimeOutDialogPresent;

    RPointerArray<MOMASuplUICompletionObserver> iUIRequestArray;
    
    TBuf<256> iCurrentlyUsedHslp;
    
    COMASuplAsnHandlerBase* iOMASuplAsnHandlerBaseImpl; 

};

#endif // C_COMASUPLPROTOCOLMANAGER1_H
