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
* Description:   Header file for Socket Read
*
*/


#ifndef C_CSUPLSOCKETREADER_H
#define C_CSUPLSOCKETREADER_H

// INCLUDES

#include <e32base.h>
#include <e32des8.h>
#include <securesocket.h>

// CONSTANTS
const TInt KMaxReadBuffer = 200;

// Forward Declaration
class CSuplConnTrace;
class MSuplPacketNotifier;

// CLASS DECLARATION

/**
*  CSuplSocketReader is a class to read packets from the 
*  remote host
*  
*  Provides an active object to perform SocketRead operation
*  
*
*  @lib EPosSuplProtocolHandlerPlugin.dll
*  @since Series 60 3.1 //e.g. Series 60 3.0
*/
class CSuplSocketReader: public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.- For TLS
        */
        static CSuplSocketReader* NewL(CSecureSocket *aSocket,
        								 MSuplPacketNotifier *aNotifier,
        								 TBool aVariantEnabled);
        
        /**
        * Two-phased constructor.- For without TLS
        */
        static CSuplSocketReader* NewL(RSocket& aSocket,
        								 MSuplPacketNotifier *aNotifier,
        								 TBool aVariantEnabled);
        
        
    public: // New functions
        
        /**
        * Send.
        * @since Series 60 3.1M
        * @param aPacket packet to be sent
        * @param TRequestStatus &aRequest
        * @return void *
        */
        void IssueRead();

        void SetPortNum(TUint aPortNum);
        
        /**
        * Destructor
        */
        virtual ~CSuplSocketReader();
        
        
    public: // Functions from base classes

        
    protected:  // New functions
   
      
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
        * C++ default constructor.-  For TLS
        */
        CSuplSocketReader(CSecureSocket *aSocket, 
        				  MSuplPacketNotifier *aNotifier,
        				  TBool aVariantEnabled);
        				  
        /**
        * C++ default constructor.- For without TLS
        */				  
        CSuplSocketReader(RSocket& aSocket,
        				  MSuplPacketNotifier *aNotifier,
        				  TBool aVariantEnabled);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    public:     // Data
    
    protected:  // Data

    private:    // Data
    
    	// Socket to be used for reading
    	CSecureSocket *iSecureSocket;
    	
    	RSocket& iSocket;
    	
    	RSocket iDummySocket;
    	
        
        

        // Buffer to hold read data        
        RBuf8 iReadBuffer;
        
        // Buffer to hold additional data when the data size is
        // more than 2K
        RBuf8 iAdditionalReadBuffer;
        
        // A Boolean to indicate if its a re-read
        TBool iReRead;
        
        /*! @var iMsgLength of data read is written here */
    	TSockXfrLength          iMsgLength;
    	
    	// Packet Notifier
    	MSuplPacketNotifier *iReadNotifier;
    	
    	 // Trace Log
        CSuplConnTrace* iTrace;
        
        //Local variation Flag
        TBool iVariantEnabled;
    
        //Port number used
        TUint iPortNum;    
    };

#endif      // C_CSUPLSOCKETREADER_H   
            
// End of File
