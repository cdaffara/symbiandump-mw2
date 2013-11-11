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
* Description:  Client-Server communication
*
*/


#ifndef __EPOS_SUPLNETWORK_H__
#define __EPOS_SUPLNETWORK_H__

#include <e32std.h>

// Forward declarations

/**
 * This class is used by clients to forward SUPL messages received
 * from the network.  This class contains methods to establish connection to SUPL 
 * server, to forward messages received from network to SUPL server and to close 
 * connection to terminal resident SUPL server. 
 * Client should close connection to SUPL server when connection is no longer
 * required.
 */

class RSuplNetworkServer : public RSessionBase
	{
public:
    /**
     * Constructor for RSuplNetworkServer
     */
	IMPORT_C RSuplNetworkServer();

     /**
     * Creates a connection with the SUPL server. If there are no other
     * connection when this is called, the method will start the server.
     * If client tries to connect again with the same instance before 
     * closing previous connection, it panics with panic code 
     * ESuplNWServerHandleNotClosed.
     *
     * @return following error codes 
     *      - KErrNone when a link has been successfully established with SUPL Server.
     *      - KErrNotFound if server executable is not found to start a SUPL server
     *      - KErrNotSupported if version of client API is not compatible with SUPL Server or if SUPL feature is not enabled in the terminal.
     *      - KErrPermissionDenied if client does not have CommDD capability
     */
	IMPORT_C TInt Connect();

    /**
     * Forwards message to SUPL server.  Before using this method, client
     * should have established connection using Connect() method. If the 
     * connection is not established, it panics with panic code ESuplServerNWBadHandle. 
     *
     * @param [IN] aSuplMsg SUPL message received from the network. 
     * @return one of the following error codes 
     *         - KErrNone if forwarding message is completed sucessfully.
     *         - KErrNoMemory if there is no memory or if the memory available is not enough to hold the message received.
     *         - KErrArgument if the message passed is NULL. 
     *         - KErrNotReady if SUPL server is not ready. 
     *         - KErrServerTerminated if server got terminated due to some error.
     *         - KErrNotSupported if SUPL is not enabled
     *         - KErrGeneral for all other errors.
     */
	IMPORT_C TInt ForwardMessage(
            const TDesC8& aSuplMsg
    );
    
    /**
     * Closes a session with the SUPL server.
     */
	IMPORT_C void Close();

    /**
     * Obtains the current version number of the SUPL Network Initiation API.
     *
     * @return the version of the client API.
     */
	IMPORT_C TVersion Version() const;

private:
	void ConstructL();
	void Destruct();
	static TInt StartServer();

private:
	/** Unused variable for future expansion. */
	TAny* iReserved;
	};


#endif // __EPOS_SUPLNETWORK_H__
