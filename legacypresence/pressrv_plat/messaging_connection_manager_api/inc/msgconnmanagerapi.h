/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Connection manager
*
*/



#ifndef __MSGCONNMANAGERAPI__
#define __MSGCONNMANAGERAPI__

#include "msgbearerobscallback.h"

//Define the CommDb parameters which the Conn Manager must support
enum TSupportedCommDbRecords
    {
    EMsgProxyAddress = 0,
    EMsgProxyPort
    };

//FORWARD DECLARATIONS
class TInetAddr;
class RConnection;
class RSocketServ;
class MMsgBearerObsCallback;

class MMsgConnManager 
    {
    public:  // New functions

        /**
        * Start a connection. This asynchronous request completes
        * as soon as a connection has been successfully opened or
        * an error condition has been detected.
        *
        * Completes with:
        *
        *    - KErrNone if a connection has been succesfully opened
        *    - KErrNotReady if client has called StopConnection()
        *      and attempts to start a new connection before the previous
        *      one has been disconnected.
        *    - KErrAlreadyExists if a client attempts to open a connection
        *      while one is already being opened
        *    - KErrTimedOut if the specified timeout value expires
        *
        * @param aStatus Request status of the client.
        */  
        virtual void StartConnection( TRequestStatus& aStatus ) = 0;
    
        /**
        * Stop a connection, synchronous version
        */  
        virtual void StopConnection() = 0;
        
        /**
        * Stop a connection. This asynchronous request completes
        * as soon as a connection has been closed. Note that it 
        * takes a few seconds for the connection to really wear out.
        * Thus, when a client calls this function, it should really
        * mean what it is saying, because a new connection CANNOT BE 
        * OPENED before a previous one has been deactivated.
        *
        * The request may complete with
        *    - KErrNone if connection closing is successful
        *    - KErrAbort if call takes place while connection initiation
        *      is ongoing or if there is no active connection that would
        *      require closing. If a connection initiation is ongoing
        *      client should use CancelStartL() method instead.
        *
        * @param aStatus Request status of the client
        */  
        virtual void StopConnection( TRequestStatus& aStatus ) = 0;
        
        /**
        * Cancel StartConnection(). The method may leave with
        *   - KErrAbort if the TRequestStatus object the client
        *     supplied is not active, hence, there is no pending
        *     request that would require cancelling.
        * In normal cases - when RConnection::Start() or its 
        * progress notifications are pending - ConnMan completes
        * the client's request with the "error" KErrCancel.
        *
        * @return void
        */  
        virtual void CancelStartL() = 0;

        /**
        * Set the ID of the Access Point to connect to. Note that this
        * function is effective only when there is no active connection
        * the start of which has been initiated by the client. In other
        * words, the ID of the desired Access Point can be changed only
        * BEFORE calling StartConnection().
        * 
        * May leave with
        *    - KErrNotFound if the Access Point ID does not exist
        *
        * @param TInt aAccessPointID ID of the Accee Point to use
        */
        virtual void SetAccessPointIDL( const TInt aAccessPointID ) = 0;

        /**
        * Returns a reference to the active socket server.
        *
        * @return RSocketServ& An opened socket server
        */
        virtual RSocketServ& SocketSession() = 0;

        /**
        * Returns a reference to the currently open connection
        *
        * @return RConnection& An opened connection
        */
        virtual RConnection& Connection() = 0;

        /**
        * Returns the number of active connections currently open
        *
        * @return TInt Number of active connections on the device
        */
        virtual TInt NumberOfActiveConns() = 0;

        /**
        * Read data from the Comms Database
        *
        * @param TMsgSupportedCommDbRecords aParameter The type of the CommDb record
        * @return HBufC* Value of the requested record
        */
        virtual HBufC* ReadFromCommsDbLC( const TSupportedCommDbRecords aParameter ) = 0;

        /**
        * Add an object to the queue of listeners. The object to be added
        * must inplement the interface MMsgBearerObsCallback in order
        * to receive events from the system agent. It is important to note
        * that the call to the notification handler (HandleBearerEventL())
        * takes place inside the RunL() method of this Connection Manager,
        * so the listening object MUST return the control to the Manager
        * AS SOON AS POSSIBLE in order not to clog the scheduler.
        *
        * @param MMsgBearerObserverCallback* aClient A subscribing client
        * @return void
        */
        virtual void AddEventSubscriberL( MMsgBearerObsCallback* aClient ) = 0;
        
        /**
        * Remove an object from the queue of listeners. It is not necessary
        * to call this method at deletion time, as the destructor destroys
        * the subscriber queue altogether. If, however, a listener object is
        * destroyed (or is likely to be) before an instance of Connection Manager,
        * it is mandatory to remove the listener from the queue prior to deleting
        * the listener.
        *
        * @param MMsgBearerObsCallback* aClient The client to be removed
        * @return void
        */
        virtual void RemoveEventSubscriber( MMsgBearerObsCallback* aClient ) = 0;

        /**
        * Returns the state of this connection manager:
        *
        *    - ETrue, if a connection is available
        *    - EFalse otherwise; bearer suspended, StartConnection() failed etc.
        *
        * A component that wishes to send/receive data is encouraged
        * to call this function before opening a socket, or reading
        * from or writing to one. This way it can be assured that
        * no component prompts up that annoying IAP dialog.
        *
        * @return TBool Is the connection OK
        */
        virtual TBool Status() const = 0;

        /**
        * Destructor. Must be called when the services of the
        * manager are no longer needed. This is merely a
        * wrapper to the C++ destructor.
        *
        * It is highly advisable that a client of this API
        * ALWAYS Destroy()s the instance it owns only after
        * it has disposed of all other connection-related
        * components at its command. This is because the other
        * components may still be dependent on the RConnection
        * and RSocketServ instances provided by this object.
        *
        * @return void
        */
        virtual void Destroy() = 0;
    };

/**
* Creates a new Connection Manager
* @param aDefaultAccessPoint ID of the default Access Point
*/     
IMPORT_C MMsgConnManager* NewMsgConnManagerL( const TInt aDefaultAccessPoint );

#endif


// End of File
