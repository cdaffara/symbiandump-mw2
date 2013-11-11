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
* Description:   Header file for SUPL Communication Manager
*
*/


#ifndef C_CSUPLCOMMUNICATIONMANAGER_H
#define C_CSUPLCOMMUNICATIONMANAGER_H

#include <e32std.h>
#include <es_sock.h>

#include "epos_msuplpacketnotifier.h"

// CONSTANTS
//const ?type ?constant_var = ?constant;
const TInt KMaxSessionPerConn = 1;


// Forward Declaration
class CSuplConnection;
class MSuplPacketReceiver;
class CSuplConnTrace;

// CLASS DECLARATION

class MSuplConnectionMonitor
	{
public:
	virtual void ConnectionOpened() = 0;
	virtual void ConnectionClosed() = 0;
	};

/**
*  CSuplCommunicationManager is the Singleton that provides SUPL Connection 
*  to be used by Protocol Handler
*  
*  This provides the methods to create and destroy for SUPL Connection
*  Provides method to handle incoming packets from the network
*  
*
*  @lib EPosSuplProtocolHandlerPlugin.dll
*  @since Series 60 3.1M //e.g. Series 60 3.0
*/
class CSuplCommunicationManager : public CBase, public MSuplPacketNotifier
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSuplCommunicationManager* NewL(MSuplConnectionMonitor& aConnMonitor);
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CSuplCommunicationManager();

    public: // New functions
        
        /**
        * Initialize.
        * @since Series 60 3.1M
        * @param None
        * @return TInt. If sucessful, returns KErrNone. 
        * For unsuccessful cases, it returns a System wide
        * error code
        */
        IMPORT_C TInt Initialize();
        
        /**
        * CreateConnection.
        * @since Series 60 3.1M
        * @param aHostAddress HostAddress with which connection has to
        * be established
        * @return CSuplConnection *
        */
        IMPORT_C CSuplConnection *CreateConnectionL(const TDesC &aHostAddress, 
        											const TUint aPort,
        											const TInt aIAPId);
 
        //---------------------------------------ADDITIONAL CREATECONNECTIONL METHOD(TAKES TLS ALSO)-----------------------------

        IMPORT_C CSuplConnection *CreateConnectionL(const TDesC &aHostAddress, 
        											TBool aTls, 
        											TBool aPskTls, 
        											const TUint aPort,
                                                    const TInt aIAPId);
        //------------------------------------------------------------------------------------------------------
        
        /**
        * DestroyConnection.
        * @since Series 60 3.1M
        * @param CSuplConnection *
        * @return none
        */
        IMPORT_C TInt DestroyConnection(CSuplConnection *aConnection);
        
        /**
        * SetPacketReceiver.
        * @since Series 60 3.1M
        * @param MSuplPacketReceiver
        * @return none
        */
        IMPORT_C void SetPacketReceiver(MSuplPacketReceiver *aReceiver);

    public: // Functions from base classes

         /**
        * From MSuplPacketNotifier - To handle received packets fromk the
        * Network.
        * @since Series 60 3.1U
        * @param aPacket - The packet received from the network
        * @return None
 		*/
        void HandlePacket(const TDesC8 &aPacket, TUint aPortNo);
        
        /**
        * From MSuplPacketNotifier - To handle any errors while trying to
        * read packets from the network
        * @since Series 60 3.1U
        * @param aError - The Error value 
        * @return None
 		*/        
        void HandlePacketError (const TInt aError);
        
    protected:  // New functions
        
        /**
        * FindConnection.
        * @since Series 60 3.1M
        * @param aHostAddress
        * @param aPort
        * @return CSuplConnection *
        */
        CSuplConnection *FindConnection( const TDesC &aHostAddress, const TUint aPort );
        
        /**
        * FindConnection.
        * @since Series 60 3.1M
        * @param aConnection
        * @return TInt
        */
        TInt FindConnection( const CSuplConnection *aConnection );
        
        
        /**
        * RemoveConnection.
        * @since Series 60 3.1M
        * @param CSuplConnection *
        * @return none
        */
        void RemoveConnection(TInt aIndex);
        
        /**
        * DestroyAllConnections.
        * @since Series 60 3.1M
        * @param none
        * @return none
        */
        void DestroyAllConnections();


    protected:  // Functions from base classes
        
        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CSuplCommunicationManager(MSuplConnectionMonitor& aConnMonitor);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        
    public:     // Data
        
    
    protected:  // Data
       
    private:    // Data
        // A Pointer Array holding the CSuplConnection pointers
        RPointerArray<CSuplConnection> iConnArray;
        
        // RSocketServ 
        RSocketServ iSocketServ;
        
        // If Connected ??
        TBool iServerConnected;
        
        // Packet Receiver
        MSuplPacketReceiver *iPktReceiver;
        
        // Trace Log
        CSuplConnTrace* iTrace;
        
        MSuplConnectionMonitor& iConnMonitor;
    };

#endif      // C_CSUPLCOMMUNICATIONMANAGER_H   
            
// End of File
