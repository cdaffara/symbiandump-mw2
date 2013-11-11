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
* Description:   Header file for COMASUPLProtocolManager implementation
*
*/


#ifndef C_COMASUPLPROTOCOLMANAGER_H
#define C_COMASUPLPROTOCOLMANAGER_H

// Include Files
#include <e32cmn.h> 
#include <etel.h> 
#include <etelmm.h> 

#include "epos_csuplprotocolmanagerbase.h"   // interface
#include "epos_comasuplinitilizeobserver.h"
#include "epos_comasupldeinitializer.h"

// Farward Declaration
class COMASuplTrace;
class CSuplCommunicationManager;
class COMASuplCommonConfig;
class TSuplTerminalQop;
class COMASuplInitilizeRequestor;
class COMASuplAsnHandlerBase;

/**
*  An implementation of the COMASUPLProtocolManagerBase definition. 
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1
*/ 
class COMASUPLProtocolManager : public CSUPLProtocolManagerBase,
								public MCompleteInitilization,
								public MCompleteDeInitialization								
    {    
public:
	/**
	 * NewL, Create instance of concrete implementation. 
	 * @since S60 v3.1u		
	 * @param      None
	 * @return    Instance of this class.
	 */
   	IMPORT_C static COMASUPLProtocolManager* NewL();

	/**
	 * ~COMASUPLProtocolManager
	 * @since S60 v3.1u		
	 * @param      None
	 * @return     None
	 */
  	IMPORT_C  ~COMASUPLProtocolManager();

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
  	IMPORT_C virtual void RunSessionL(
  						CSuplSessionBase *const aSuplSession,
  						TRequestStatus& aStatus,
  						const TDesC& aHslpAddress,
  						TBool aFallBack,
  						TBool aFirstReq,
  						TInt aAllowedCapabilities,
  						TSuplTerminalQop& aQop,
  						TInt aRequestID = 0);

	/**
	 * RunSessionL, Starts the SUPL Sesssion for Terminal Initiated Location Request
	 * @since S60 v3.1
	 * @param       aSuplSession , session object on which processing will start
	 * @param       aStatus, 	  On completion, sets status of the request
	 * @param       aAllowedCapabilities , Allowed capabilities of SET
	 * @param       aRequestID , Request ID of Session
	 * @return      None
	 */
  	IMPORT_C virtual void RunSessionL(
  						CSuplSessionBase *const aSuplSession,
  						TRequestStatus& aStatus,
  						const TDesC& aHslpAddress,
  						TBool aFallBack,
  						TBool aFirstReq,
  						TInt aAllowedCapabilities,
  						TInt aRequestID = 0);
	/**
 	 * HandleSuplMessage, Handles session in NET initiated case
	 * @since S60 v3.1
	 * @param       aSuplSession , session object on which processing will start
	 * @param       aStatus, 	  On completion, sets status of the request
	 * @param       aMessage,	message to be decoded.
	 * @return      None
	 */
  	IMPORT_C virtual void HandleSuplMessageL(
  						CSuplSessionBase *const aSuplSession,
  						TRequestStatus& aStatus,
  						HBufC8* aMessage);

	/**
	 * DestroySession, Destroys the specified Supl Session instance
 	 * @since S60 v3.1u
	 * @param      aSuplSession , session object which has to delete
	 * @return     TInt,returns KErrNone if deleted successfully else KErrArgument  
	 */
  	IMPORT_C TInt DestroySession(CSuplSessionBase* aSuplSession);	
  
	/**
	 * InitializeL, Does the asynchonous initilization. 
	 * @since S60 v3.1u
	 * @param      aCommManager,Communication Manager
	 * @param      aInitilizationRequestStatus, TRequestStatus for asynchronous initilization operation
	 * @return     None
	 */
  	IMPORT_C void InitializeL(
  						CSuplCommunicationManager* aCommManager,
  						TRequestStatus& aInitilizationRequestStatus);
  
    /**
     * Function:     CancelInitialize, 
     * @Description: Does the cancellation of asynchonous initilization. 
     * @Param:       None
     * @Returns:     None
     */
  	IMPORT_C  void CancelInitialize();

    /**
     * Function:     DeInitialize, 
     * @Description: Does the de-initilization. 
     * @Param:       aDeInitRequestStatus,for asynchronous de-initilization of protocol manager
     * @Returns:     None
     */
  	IMPORT_C void DeInitialize(TRequestStatus& aDeInitRequestStatus);

    /**
     * Function:     CancelDeInitialize, 
     * @Description: Does the cancellation of asynchonous de-initilization. 
     * @Param:       None
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
  
    /**
     * Function:     RunSuplTriggerSessionL, 
	 * @since S60 v5.2
     * @Description: Starts processing on session
     * @Param:       aSuplSession , session object on which processing will start
     * @Param:       aStatus, On return, the status of the request
     * @Param:       aAllowedCapabilities , Allowed capabilities of SET
     * @Param:       aPTrigger , specifies the periodic trigger session
     * @Param:       aRequestID , Request ID of Session
     *
     * @Returns:     None
     */
  	IMPORT_C virtual void  RunSuplTriggerSessionL(
  					CSuplSessionBase *const aSuplSession,
  					TRequestStatus& aStatus,
  					TSuplTerminalPeriodicTrigger& aPTrigger,
  					const TDesC& aHslpAddress, 
  					TBool aFallBack,
  					TInt aAllowedCapabilities,
  					TInt aRequestID = 0);

	/**
	 * CancelPeriodicTriggerSession,Cancels an in-progress Terminal Initiated 
	 * Periodic Trigger Session Location Request
	 * @since S60 v5.2
     * @Description: Cancels the Triggering Session
	 * @param       aSession, The session on which the request has to be cancelled
	 * @return      None
	 */
  	IMPORT_C virtual void CancelSuplTriggerSession(CSuplSessionBase* aSuplSession);
  
   	/**
     * Function:     NotifyTriggerFired
 	 * @since S60 v5.2
     * @Description: Starts listening for the trigger fired event
     * @Param:       None
     * @Returns:     None
     *
     */
  	IMPORT_C virtual void NotifyTriggerFiredL(
    				 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus, 
        			 TSuplTriggerFireInfo& aFireInfo);

   	/**
     * Function:     SetCommonConfig
	 * @since S60 v5.2
     * @Description: Sets the common configuration data
     * @Param:       aCommmonConfig, common data structure 
     * @Returns:     None
     *
     */
  	IMPORT_C virtual void SetCommonConfig(COMASuplCommonConfig*& aCommmonConfig);

   	/**
     * Function:     GetSUPLMessageVersionL
	 * @since S60 v5.2
     * @Description: Gets the SUPL Message Version
     * @Param:       aMajorVersion, Major Version Number
     * @Param:       aReceivedMessage, Recived Message Data
     * @Returns:     None
     *
     */
	IMPORT_C virtual TInt GetSUPLMessageVersionL(TInt& aMajorVersion, const TDesC8& aReceivedMessage);
	
	/**
     * Function:     CancelLocationConversionRequest
	 * @since S60 v5.2
     * @Description: Cancels the location conversion request
     * @Param:       aSuplSession, session object
     * @Returns:     None
     *
     */
	
	IMPORT_C  void CancelLocationConversionRequest(CSuplSessionBase* aSuplSession);
	    
	/**
     * Function:     MakeLocationConversionRequestL
	 * @since S60 v5.2
     * @Description: issues the location conversion request
     * @Param:       aSuplSession, session object
     * @Param:       aCellInfo, cell information
     * @Param:       aStatus, request status
     * @Returns:     None
     *
     */
	
	IMPORT_C  void MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,
	                                                  TGeoCellInfo& aCellInfo,
	                                                  TRequestStatus& aStatus );
	
protected: // Construction
	/**
	 * COMASUPLProtocolManager,Perform the first phase of two phase construction
	 * @since S60 v3.1u
	 * @param       None
	 * @return      None
	 */
	IMPORT_C COMASUPLProtocolManager();

private: 
	/**
     * ConstructL,Perform the second phase construction of a COMASUPLProtocolManager object.
     * @since S60 v3.1u
     * @param       None
     * @return      None
     */
    void ConstructL();

private : //For internal use only       
	/**
	 * CompleteInitilization,Completes TRequestStatus used for initilization.
	 * @since S60 v3.1u
	 * @param       aError, error code by which we are going to complete TRequestStatus.
	 * @return      None
	 */
	void CompleteInitilization(TInt aError);  														
	
	
	void DeInitilizationCompletedL(TInt aError);
	
	/**
	 * GetNetworkModeL,
	 * @since 		S60 v5.2
	 * @param      	None
	 * @return     	TInt,returns KErrNone 
	 */
	TInt GetNetworkModeL();
	
	/**
	 * LoadOMASUPLPluginsL, Loads supl protocolhandler plugins
	 * @since 		S60 v5.2
	 * @param      	None
	 * @return     	None
	 */
	void LoadOMASUPLPluginsL();
	
	/**
	 * GetSuplVersion, Gets Supl Version
	 * @since 		S60 v5.2
	 * @param      	None
	 * @return     	None
	 */
	TInt GetSuplVersion(TInt& aMajor,const TPtrC8& aSuplAsnMessage );
	
    
private:	//From MCompleteInitilization

    /**
     * InitilizationCompletedL, Gets called after initilization gets completed with error or without error.
     * @since S60 v3.1u          
     * @param      aError,Error code if any happen while initilization.
     * @return     None.
     */
	void InitilizationCompletedL(TInt aError);
	
private: // Data Members

	//Tracing utility
	COMASuplTrace* iTrace;

	// Asynchronous request status for Initilization
	TRequestStatus* iInitilizationRequestStatus; 
	TRequestStatus* iInitilizationRequestStatus2;
	TRequestStatus* iDeInitRequestStatus; 

    COMASuplInitilizeRequestor* iOMASuplInitilizeRequestor;
	
    CSUPLProtocolManagerBase* iOMASuplProtocolHandler1;

    CSUPLProtocolManagerBase* iOMASuplProtocolHandler2;
    
    COMASuplCommonConfig*     iCommonConfig;
    
    //Communication Manager
	CSuplCommunicationManager* iCommManager;
	
	COMASuplAsnHandlerBase* iOMASuplAsnHandlerBaseImpl; 
	
	COMASuplDeInitilizer* iOMASuplDeInitilizer;
	
    TInt iSuplService;
    // Indicates if the Protocol Manager was initialized or not
    TBool iIsInitilized;
    
    TBool isPH2Shared;
    
    TBool iIsPhDeinitialized1;
    TBool iIsPhDeinitialized2;
       
	};

#endif // C_COMASUPLPROTOCOLMANAGER_H
