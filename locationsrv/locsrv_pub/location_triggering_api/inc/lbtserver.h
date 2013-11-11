/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  location triggering server client library
*
*/



#ifndef LBTSERVER_H
#define LBTSERVER_H


#include <e32base.h>

/**
 * A handle to Location Triggering Server session. 
 *
 * This is generally the first interface class used by all client applications.
 * It is used to make the primary connection to the Location Triggering Server. 
 * After the primary connection has been established, its handle is passed as a 
 * parameter of the Open methods of @p RLbt to create a subsession.
 *
 *  @lib lbt.lib
 *
 *  @since S60 5.1
 */
class RLbtServer : public RSessionBase
    {
public:
    /**
     * Constructor for RLbtServer
     */
    IMPORT_C RLbtServer();

    /**
     * Creates a session with the Location Triggering Server. 
     * This method must be called before any subsession can be opened. 
     * @see RLbt.
     *
     * @panic ELbtServerHandleNotClosed Try to connect using an already opened handle. 
     *
     * @return KErrNone, if successful. KErrNotSupported if Location Triggers
     * Server is not present in the system. Otherwise one of the other standard
     * Symbian error code (for example KErrNoMemory, KErrServerBusy, etc. ) 
     * is returned.
     */
    IMPORT_C TInt Connect();

    /**
     * Closes a session with the Location Triggering Server.
     * Before the connection to Location Triggering Server is closed, the 
     * client application must ensure that all subsessions is closed. 
     */
    IMPORT_C void Close();

    /**
     * Obtains the client side version number of Location Triggering Server.
     *
     * @return The client side version number.
     */
    IMPORT_C TVersion Version() const;
    
    /**
     * Symbian 2nd phase constructor.
     */    
    void ConstructL();

private: // data
    /**
     * Unused variable for future expansion. 
     */
    TAny* iReserved;
    };


#endif //LBTSERVER_H
