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
* Description:   Provides the interface between SUPL Gateway and SUPL Protocol Handler. 
*				 This class provides the functionality of loading and unloading 
*				 the Protocol handler ECom plug-in.
*
*/


#ifndef C_CSUPLPROTOCOLMANAGERBASE_H
#define C_CSUPLPROTOCOLMANAGERBASE_H


#include <e32base.h>
#include "epos_msuplpacketreceiver.h"
#include <epos_suplterminal.h>

//Forword Declarations
class CSuplSessionBase;
class CSuplCommunicationManager;
class TGeoCellInfo;

/**
 * Constant:    KProtocolManagerInterfaceDefinitionUid
 *
 * Description: UID of this ECOM interface. 
 *
 */
const TUid KProtocolManagerInterfaceDefinitionUid = {0x102073CB};
 

/**
 * Class:       CSUPLProtocolManagerBase
 *
 * Description: Custom ECOM interface definition. This interface is used by
 *              clients to find specific instance and implementation of 
 *				protocol handler Plugin.
 */
 
 
class CSUPLProtocolManagerBase : public CBase, public MSuplPacketReceiver
    {

public :

	/**
	 * For diffentiating request between Terminal & NET initiation 
	 *
	 */
	enum TSuplReqType
	    {
	    ETerminal = 0,
	    ENetwork
	    }; 
   
public: // Constructors and destructor  

    /**
     * Function:     NewL, Two Phase Construction
     *
     * @Description: Wraps ECom object instantitation. Will search for
     *               interface implementation, which matches to given
     *               aOperationName.
     *
     * @Param:       aOperationName, name of requested implementation.
     *
     * @return 		 instance of implementation		
     *
     */     
 	IMPORT_C static CSUPLProtocolManagerBase* NewL(const TDesC8& aOperationName);

    /**
     * Function:     NewL, Two Phase Construction
     *
     * @Description: Wraps ECom object instantitation. Will search for
     *               interface implementation, which matches to given
     *               UID.
     *
     * @Param:       aImplementationUid, UID of requested implementation.
     *
     * @return 		 instance of implementation		
     *
     */     
 	IMPORT_C static CSUPLProtocolManagerBase* NewL(const TUid &aImplementationUid);
 
    /**
     * Function:     CSUPLProtocolManagerBase 
     *
     * @Description: Default constructor
     *
     * @Param:       None
     *
     * @Returns:     None
     *
     */
   	IMPORT_C CSUPLProtocolManagerBase();
   
      /**
     * Function:     SetImplementationUid
     *
     * @Description: Sets Uid of implementation 
     *
     * @Param:       UID of implementation.
     *
     * @Returns:     None
     *
     */
	IMPORT_C virtual void ConstructL(const TUid& aUid); 

    /**
     * Function:     ~CSUPLProtocolManagerBase  
     *
     * @Description: Wraps ECom object destruction. Notifies the ECOM
     *               framework that specific instance is being destroyed.
     */
    IMPORT_C  ~CSUPLProtocolManagerBase();

    /**
     * Function:     ImplementationUid, 
     *
     * @Description: Gives implementation UID
     *
     * @Param:       None.
     *
     * @return 		 Returns implementation UID 		
     *
     */     
  	IMPORT_C TUid ImplementationUid() const;
 
    /**
     * Function:   	 Version 
     *
     * @Description: Gives version of implementation
     *
     * @Param:       None.
     *
     * @return 		 Returns TInt, containing version of implementation 
     *
     */     
  	IMPORT_C TInt Version() const;

public:

    /**
     * Function:     InitializeL, 
     *
     * @Description: Does the asynchonous initilization. 
     *
     * @Param:       aCommManager,Communication Manager
     * @Param:       aInitilizationRequestStatus,for asynchronous initilization of protocol manager
     *
     * @Returns:     None
     *
     */
  	virtual void InitializeL(
  					 CSuplCommunicationManager* aCommManager,
  					 TRequestStatus& aInitilizationRequestStatus) = 0;

    /**
     * Function:     CancelInitialize, 
     *
     * @Description: Does the cancellation of asynchronous initilization. 
     *
     * @Param:       None
     *
     * @Returns:     None
     *
     */
  	virtual void CancelInitialize() = 0;

    /**
     * Function:     DeInitialize, 
     *
     * @Description: Does the de-initilization. 
     *
     * @Param:       aDeInitRequestStatus,for asynchronous de-initilization of protocol manager
     *
     * @Returns:     None
     *
     */
  	virtual void DeInitialize(TRequestStatus& aDeInitRequestStatus) = 0;

    /**
     * Function:     CancelDeInitialize, 
     *
     * @Description: Does the cancellation of asynchronous de-initilization. 
     *
     * @Param:       None
     *
     * @Returns:     None
     *
     */
  	virtual void CancelDeInitialize() = 0;

    /**
     * Function:     HandleSuplMessageL, 
     *
     * @Description: Handles packet which come from SLP in NET initiated case
     *
     * @Param:       aSuplSession , session object on which processing will start
     * @Param:       aStatus, 			On return, the status of the request
     *
     * @Returns:     None
     *
     */ 		
  	virtual void HandleSuplMessageL(
  					 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus,
  					 HBufC8* aMessage) = 0;
  					 
public: 

    /**
     * Function:     CreateNewSession
     *
     * @Description: Creates new SuplSession instance
     *
     * @Param:       None
     *
     * @Returns:     Instance of CSuplSessionBase.
     *
     */
   	virtual CSuplSessionBase* CreateNewSessionL(
   					 TSuplReqType aRequestType, 
   					 TInt aIpcSessionId,
   					 RSuplTerminalSubSession::TSuplServiceType aSuplService) = 0;
   
   	/**
     * Function:     DestroySession
     *
     * @Description: Destoys SuplSession instance
     *
     * @Param:       None
     *
     * @Returns:     TInt, if session is invalid, then returns KErrArgument
     *
     */
   	virtual TInt DestroySession(CSuplSessionBase* aSuplSession ) = 0;
   
    /**
     * Function:     RunSessionL, 
     *
     * @Description: Starts processing on session
     *
     * @Param:       aSuplSession , session object on which processing will start
     * @Param:       aStatus, 			On return, the status of the request
     * @Param:       aAllowedCapabilities , Allowed capabilities of SET
     * @Param:       aRequestID , Request ID of Session
     *
     * @Returns:     None
     */ 		
  	virtual void RunSessionL(
  					 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus,
  					 const TDesC& aHslpAddress, 
  					 TBool aFallBack, 
  					 TBool aFirstReq, 
  					 TInt aAllowedCapabilities, 
  					 TInt aRequestID = 0) = 0;

    /**
     * Function:     RunSessionL, 
     *
     * @Description: Starts processing on session
     *
     * @Param:       aSuplSession , session object on which processing will start
     * @Param:       aStatus, 			On return, the status of the request
     * @Param:       aAllowedCapabilities , Allowed capabilities of SET
     * @Param:       aQop , specifies the quality of position
     * @Param:       aRequestID , Request ID of Session
     *
     * @Returns:     None
     *
     */
  	virtual void RunSessionL(
  					 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus,
  					 const TDesC& aHslpAddress, 
  					 TBool aFallBack, 
  					 TBool aFirstReq, 
  					 TInt aAllowedCapabilities,
  					 TSuplTerminalQop& aQop, 
  					 TInt aRequestID = 0) = 0;

   	/**
     * Function:     CancelRunSession
     *
     * @Description: Cancels the Run Session Request
     *
     * @Param:       Session Object
     *
     * @Returns:     None
     *
     */
   	virtual void CancelRunSession(CSuplSessionBase* aSuplSession) = 0;
    
    /**
     * Function:     RunSuplTriggerSessionL, 
     *
     * @Description: Starts processing on session
     *
     * @Param:       aSuplSession, session object on which processing will start
     * @Param:       aStatus, On return, the status of the request
     * @Param:       aPTrigger, specifies the periodic trigger session
     * @Param:       aHslpAddress, specifies HSLP Server Address to be used 
     *				  	to get position from it
     * @Param:       aFallBack, enables/diables Multiple-SLP Fallback mechanism
     * @Param:       aAllowedCapabilities, Allowed capabilities of SET
     * @Param:       aRequestID, Request ID of Session     
     *
     * @Returns:     None
     *
     */
   	virtual void RunSuplTriggerSessionL(
  					 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus,
  					 TSuplTerminalPeriodicTrigger& aPTrigger,
  					 const TDesC& aHslpAddress, 
  					 TBool aFallBack, 
  					 TInt aAllowedCapabilities,
  					 TInt aRequestID = 0) = 0;

   	/**
     * Function:     CancelSuplTriggerSession
     *
     * @Description: Cancels an in-progress Terminal Initiated 
	 * 				 Periodic Trigger Session Location Request
     *
     * @Param:       Session Object
     *
     * @Returns:     None
     *
     */
   	virtual void CancelSuplTriggerSession(CSuplSessionBase* aSuplSession) = 0;

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
    virtual void NotifyTriggerFiredL(
    				 CSuplSessionBase *const aSuplSession,
  					 TRequestStatus& aStatus, 
        			 TSuplTriggerFireInfo& aFireInfo) = 0;

    
    /**
     * Function:     GetSUPLMessageVersionL
     *
     * @Description: Cancels listening for the trigger fired event
     *
     * @Param:       aMajorVersion,return parameter which will hold version after decoding of message.
     *
     * @Param:       aReceivedMessage,message to be used to get version number.
     *
     * @Returns:     returns KErrNone if recceived message gets decoded successfuly otherwise KErrGeneral.
     *
     */
    virtual TInt GetSUPLMessageVersionL(TInt& aMajorVersion, const TDesC8& aReceivedMessage);
    
    /**
        * Function:     MakeLocationConversionRequestL
        *
        * @Description: Makes Location conversion request
        *
        * @Param:       aSuplSessn,session object on which processing will start. Ownership 
        *               is not transferred.
        *
        * @Param:       aCellInfo,cell information which needs to be converted to geo-coordinates
        *
        * @Param:       aStatus, On return, the status of the request
        * @Returns:     returns None
        */
        
        virtual void MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,
                                                     TGeoCellInfo& aCellInfo,
                                                     TRequestStatus& aStatus ) = 0;
           
       
        /**
         * Function:     CancelLocationConversionRequest
         *
         * @Description: Cancels the conversion Request
         *
         * @Param:       Session Object
         *
         * @Returns:     None
         */
         
         virtual void CancelLocationConversionRequest(CSuplSessionBase* aSuplSession) = 0;

				 
public: // from MSuplPacketReceiver

   	/**
     * Function:     HandleSuplPackets
     *
     * @Description: Process Supl message which are comming from SLP
     *
     * @Param:       aPacket, message packet
     *
     * @Returns:     None
     *
     */
   	virtual void HandlePacket(const TDesC8& aPacket, TUint aPortNum) = 0;

   	/**
     * Function:     HandlePacketError
     *
     * @Description: Handles error if any while reading from communication manager
     *
     * @Param:       aError, error code occured while reading from communication manager
     *
     * @Returns:     None
     *
     */
   	virtual void HandlePacketError(TInt aError) = 0;
    
private:

 	/**
     *	By default,  prohibit copy constructor
	 */         
    CSUPLProtocolManagerBase( const CSUPLProtocolManagerBase& );
    
   	/**
     *	By default,  Prohibit assigment operator
     *
	 */
    CSUPLProtocolManagerBase& operator= ( const CSUPLProtocolManagerBase& );
	
    /**
     * Function:     GetVersionL 
     *
     * @Description: Gives version of implementation 
     *
     * @Param:       None
     *
     * @Returns:     TInt , containing version of implementation 
     *
     */
	TInt GetVersionL() const;	
	
   	/**
     * Function:     SetImplementationUid
     *
     * @Description: Sets Uid of implementation 
     *
     * @Param:       UID of implementation.
     *
     * @Returns:     None
     *
     */
	 //void SetImplementationUid(const TUid& aUid); 

private:

    /** 
     * iDtor_ID_Key  Instance identifier key. When instance of an
     *               implementation is created by ECOM framework, the
     *               framework will assign UID for it. The UID is used in
     *               destructor to notify framework that this instance is
     *               being destroyed and resources can be released.
     *
     */     
    TUid iDtor_ID_Key;
    
    /**
     * Uid of Implementation    
     *
     */
    TUid iImplementationUid;
    
    };


#endif // C_CSUPLPROTOCOLMANAGERBASE_H

// End of File
