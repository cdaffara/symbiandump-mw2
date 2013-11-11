/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Positioning Indicator Helper Client Library
*
*/



#ifndef POS_IND_HELPER_SERVER_H
#define POS_IND_HELPER_SERVER_H


#include <e32base.h>

#include "posindicatorlogger.h"

/**
 * A handle to Positioning Indicator Helper Server session. 
 *
 * This is generally the first interface class used by all client applications.
 * It is used to make the primary connection to the Positioning Indicator Helper Server. 
 * After the primary connection has been established, its handle is passed as a 
 * parameter of the Open methods of @p RPosIndicatorHelper to create a subsession.
 *
 *  @lib posindicatorhelper.lib
 *
 */
class RPosIndicatorHelperServer : public RSessionBase
    {
public:
    /**
     * Constructor for RPosIndicatorHelperServer
     */
     RPosIndicatorHelperServer();

    /**
     * Creates a session with the Positioning Indicator Helper Server. 
     * This method must be called before any subsession can be opened. 
     * @see RPosIndicatorHelper.
     *
     * @panic EPosIndicatorHelperServerHandleNotClosed Try to connect using an already opened handle. 
     *
     * @return KErrNone, if successful. KErrNotSupported if Positioning Indicator Helper
     * Server is not present in the system. Otherwise one of the other standard
     * Symbian error code (for example KErrNoMemory, KErrServerBusy, etc. ) 
     * is returned.
     */
     TInt Connect();

    /**
     * Closes a session with the Positioning Indicator Helper Server.
     * Before the connection to Positioning Indicator Helper Server is closed, the 
     * client application must ensure that all subsessions is closed. 
     */
     void Close();

    /**
     * Obtains the client side version number of Positioning Indicator Helper Server.
     *
     * @return The client side version number.
     */
     TVersion Version() const;   
     
     /**
      * Start pos indicator helper server.
      */
     TInt StartServer();

#if defined(__WINSCW__)
    TInt StartLbsRootProcess();
#endif
    
private: // data
    /**
     * Unused variable for future expansion. 
     */
    TAny* iReserved;
    };


#endif //POS_IND_HELPER_SERVER_H
