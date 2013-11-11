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
* Description:   Header file for SUPL Connection
*
*/


#ifndef C_CSUPLCONNECTION_H
#define C_CSUPLCONNECTION_H

class CRepository;
// INCLUDES
//#include <?include_file>

#include <e32base.h>
#include <securesocket.h>

#include <ssl.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <ssl_internal.h>
#endif

#if WITH_GBA
#include <GbaClientrequesthandler.h>
#endif /* WITH_GBA */

// CONSTANTS
//const ?type ?constant_var = ?constant;
const TInt KMaxHostAddressLen = 256;
const TInt KErrSuplTimeout =  -32530; 

// Forward Declaration
class CSuplSocketReader;
class MSuplPacketNotifier;
class CSuplConnTrace;
class CSuplSocketTimer;
class CSuplPacketInfo;

// CLASS DECLARATION

/**
*  CSuplConnection is a wrapper around the underlying socket 
*  SUPL Session uses this object to send 
*  
*  This provides the methods to create and destroy for SUPL Connection
*  Provides method to handle incoming packets from the network
*  
*
*  @lib EPosSuplProtocolHandlerPlugin.dll
*  @since Series 60 3.1M //e.g. Series 60 3.0
*/
class CSuplConnection: public CActive, public MSoPskKeyHandler
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSuplConnection* NewL(RSocketServ &aSocketServ, const TDesC &aHostAddress, 
        							 const TUint aPort,const TInt aIAPId,
        							  MSuplPacketNotifier *aNotifier);
        
        //-----------------------------ADDED NEWL TAKING TLS ALSO--------------------------------
        static CSuplConnection* NewL(RSocketServ &aSocketServ, const TDesC &aHostAddress, 
                                             const TUint aPort,const TInt aIAPId,
                                             TBool aTls,TBool aPskTls, 
                                              MSuplPacketNotifier *aNotifier);
                
        //---------------------------------------------------------------------------------------
        
    public: // New functions
        
        /**
        * Connect.
        * @since Series 60 3.1
        * @param TRequestStatus &aRequest
        * @return void *
        */
        IMPORT_C void Connect(TRequestStatus &aStatus);
        
       /**
        * A method that makes Socket Connection.
        * @since Series 60 3.1 
        * @return None
        * @param aPrompt used to show connection prompt using OCC Api's.
        * @param aWlanOnly used to differentiate connection for GPRS and WLAN using OCC Api's
        */
        IMPORT_C void Connect(TRequestStatus &aStatus,TBool aPrompt,TBool aWlanOnly);
            
        

        IMPORT_C void CloseConnection();

        IMPORT_C TInt GetPortNumberUsed();

        /**
        * CancelConnect.
        * @since Series 60 5.0 
        * @param TRequestStatus &aRequest
        * @return void *
        */
        IMPORT_C void CancelConnect(TRequestStatus &aStatus);
        
        /**
        * Send.
        * @since Series 60 3.1
        * @param aPacket packet to be sent
        * @param TRequestStatus &aRequest
        * @return void *
        */
        IMPORT_C void Send(const TDesC8 &aPacket, TRequestStatus &aStatus);
        
        /**
        * CancelSend.
        * @since Series 60 3.1
        * @param aPacket packet to be sent
        * @param TRequestStatus &aRequest
        * @return void *
        */
        IMPORT_C void CancelSend(TRequestStatus &aStatus);
        
        /**
        * ListenToMessages.
        * @since Series 60 3.1
        * @param None
        * @return TInt for error
        */
        IMPORT_C TInt ListenToMessages();
        
        IMPORT_C void CancelListenToMessages();	
        /**
        * Destroy.
        * @since Series 60 3.1
        * @param None
        * @return none
        */
        void Destroy();


		/**
        * RefCount.
        * @since Series 60 3.1
        * @param None
        * @return TInt
        */
        TInt RefCount();
        
        /**
        * IncRefCount.
        * @since Series 60 3.1
        * @param None
        * @return None
        */
        void IncRefCount();
        
        /**
        * DecrRefCount.
        * @since Series 60 3.1
        * @param None
        * @return None
        */
        void DecrRefCount();
        
        /**
        * HostAddress.
        * @since Series 60 3.1
        * @param None
        * @return TDesC
        */
        const TDesC& HostAddress();
        
        /**
        * Port.
        * @since Series 60 3.1
        * @param None
        * @return TUint
        */
        TUint Port();
        
        /**
        * GetLocalIPAddress.
        * @since Series 60 3.1
        * @param None
        * @return TUint
        */
		IMPORT_C TInt GetLocalIPAddress(TInetAddr &aTInetAddress );

    public: // Functions from base classes

    public: // From MSoPskKeyHandler
        void GetPskL(const HBufC8 * aPskIdentityHint, HBufC8 *& aPskIdentity, HBufC8 *& aPskKey);         
        
    public:    
    
		void TimerExpired();
               
    protected:  // New functions
   
	   /**
        * Making Destructor protected so that no class
        * other than CSuplCommunicationManager is able to 
        * delete CSuplConnection.
        */
        virtual ~CSuplConnection();
     
        /*!
  		@enum TConnectionState

  		@discussion Tracks the state of this object through the connection process
 		@value ENotConnected The initial (idle) state
  		@value EConnecting A connect request is pending with the socket server
  		@value EConnected A connection has been established
  		@value EMakingSecureConn Started making the Socket Secure
  		@value ESecureHandShake Client Handshake for the secure connection is in progress
  		@value ELookingUp A DNS lookup request is pending with the socket server
  		@value EFailue Indicates failure in any of the asynchronous operations
  		*/
		enum TConnectionState
		    {
			ENotConnected,
			ERetriveIAP,
	        EConnecting,
	        EConnected,
	        EMakingSecureConn,	        
	        ESecureHandShake,
 	        ELookingUp,
	        EFailure
		    };
		    
		/**
        * A method that makes Socket Connection.
        * @since Series 60 3.1 
        * @param None
        * @return None
        */
        void Connect();
        
        /**
        * A method that makes Socket Connection.
        * @since Series 60 3.1 
        * @param None
        * @return None
        */
        void ConnectIAP();
        
        /**
        * A method that makes Socket Secure.
        * @since Series 60 3.1 
        * @param None
        * @return None
        */
        void MakeSecureConnection();

        void CompleteConnectRequests(TInt err);
        
        void CheckForOutstandingPackets();

        void CompleteConnectCancel(TRequestStatus &aStatus);

    protected:  // Functions from CActive
        
        /**
        * From CActive 
        * To handle request completion
        */
        void RunL();
        
        /**
        * From CActive 
        * To cancel the asynchronous requests
        */
       
        void DoCancel();


    private:

        /**
        * C++ default constructor.
        */
        CSuplConnection(RSocketServ &aSocketServ);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(const TDesC& aHostAddress, const TUint aPort, const TInt aIAPId,
        				MSuplPacketNotifier *aNotifier);

        void SetVariation();
        
        void GeneratePsk_Identity_KeyL(const TDesC8& aVer,const TDesC8& aPassword,
        							  HBufC8 *& aPskIdentity, HBufC8 *& aPskKey);
        							  
        void GeneratePskIdentityL(CRepository* aRepository,TInt aKeyVer,HBufC8 *& aPskIdentity);
        
        void GeneratePskKeyL(CRepository* aRepository,const TDesC8& aPassKey,HBufC8 *& aPskKey,TInt aSLPKeyVersion);
        
        void LogPskKeyInfoL(const HBufC8* aPskIdentityHint, HBufC8* aPskIdentity, HBufC8* aPskKey);
        
        void PrintHex(const TDesC8& aBuffer,TInt aLine);
        
        //-----------------------------ADDED CONSTUCTL TAKES TLS ALSO-----------------------------------
        void ConstructL(const TDesC& aHostAddress, const TUint aPort, const TInt aIAPId,
                                TBool aTls,TBool aPskTls, 
                                MSuplPacketNotifier *aNotifier);
                
        //----------------------------------------------------------------------------------------------

    public:     // Data
        
    protected:  // Data
        
    private:    // Data
    
    	// Socket Server
    	RSocketServ &iSocketServ;
        
        //Socket with which connection is made
        RSocket   iSocket;
        
        // Secure Socket
        CSecureSocket *iTlsSocket;
        
        //Socket Resolver
        RHostResolver iResolver;
        
        // RConnection object
        RConnection	  iConnection;
        
        //Name Entry for Address Resolution
        TNameEntry iNameEntry;
        
        // The HostAddress and port with which this connection object 
        // is associted with
        TInetAddr iAddress;
        
        // A String buffer holding the IP Address in String
        // Format
        TBuf <KMaxHostAddressLen> iHostAddress;
        
        // An unsigned integer hodling the port with which 
        // connection has to be made
        TUint iPort;
        
        // IAP ID
		TInt iIAPId;
        
        // A reference count that indicates whether there are
        // SUPL Sessions still using the SUPL Connection
        TInt iRefCount;
        
        // Connection State
        TConnectionState iState;
        
        // Status sent by Asynchronous service user
        TRequestStatus* iReqStatus;
        
        // Socket Reader
        CSuplSocketReader *iSocketReader;
        
        // Trace Log
        CSuplConnTrace* iTrace;

        // Socket Timer
        CSuplSocketTimer* iTimer;
        // Array of statuses for connection
        RPointerArray<TRequestStatus> iConnArray;

        TPtrC8 iPacket;
        
        CSuplPacketInfo *iPkt;
        
        RPointerArray<CSuplPacketInfo> iPktArray;

#if WITH_GBA
        CGbaClientRequestHandler *iGbaClient;
#endif /* WITH_GBA */

        /* Flag for local variation Support*/
        TBool iVariantEnabled;
        
        TBool iPskTLSUsage;
        
        TBool iStartedListen;
        
        TBool iConnectStarted;

        TBool iSendInProgress;
        
        TBool iFirstPkt;
       
        // Number of clients listening to socket 
        TInt  iListenCount;

        // Number of client connected
        TInt iConnectClients;
        
        TBool iPrompt;
        
        TBool iWlanOnly;
        
       
        
        
        
    };

#endif      // C_CSUPLCONNECTION_H 
            
// End of File
