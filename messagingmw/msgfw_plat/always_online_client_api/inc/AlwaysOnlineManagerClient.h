/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*     Client interface for always online service
*
*/


#ifndef __ALWAYSONLINEMANAGERCLIENT_H__
#define __ALWAYSONLINEMANAGERCLIENT_H__

#include <e32base.h>
#include <msvapi.h>
#include <AlwaysOnlineManagerCommon.h>


const TInt KDefaultMessageSlots = 4;
//this is needed because we call threadfunction only by dll index
const TInt KThreadFunctionIndex = 3;

enum TAlwaysOnlineServerAPICommands
    {
    EServerAPIBaseCommand = 0,//common command IDs 0 - 999
    EServerAPIBaseCommandStart,
    EServerAPIBaseCommandStop,
    EServerAPIBaseAllowNWOperations = 4,
    EServerAPIBaseSuspendNWOperations = 5,
    EServerAPIBaseCommandAoSchedulerError = 99,
    EServerAPIEmailBaseCommand = 1000, //email specific commands 1000 - 1999
    EServerAPIEmailTurnOff, // 1001 switch off specified mailbox
    EServerAPIEmailTurnOn, // 1002 switch on specified mailbox
    EServerAPIEmailAgentRemove, // 1003 remove specified mailbox
    EServerAPIEmailEMNReceived, // 1004 EMN received for specified mailbox
    EServerAPIEmailUpdateMailWhileConnected, // 1005 acts like imap connection
                                             // except does not reconnect if
                                             // connection dropped
    EServerAPIEmailQueryState, // 1006
    EServerAPIEmailCancelAllAndDisconnect, // 1007
    EServerAPIEmailCancelAllAndDoNotDisconnect, // 1008
    EServerAPIEmailSuspend, // 1009
    EServerAPIEmailContinue, // 1010
    EServerAPIEmailDoNotDisconnect, // 1011
    EServerAPIEmailSessionClosed, // 1012
    EServerAPIEmailDisableAOEmailPlugin, // 1013 disables AOEmailPlugin
    EServerAPIEmailEnableAOEmailPlugin, // 1014 enables AOEmailPlugin

    EServerAPIPecBaseCommand = 2000, //Pec specific commands 2000 - 2999

    EServerAPIImBaseCommand = 3000 // IM specific commands 3000 - 3999

    //for new plugins, add new base command IDs
    };

/**
* CAlwaysOnlineClientSession
*
* Client interface for always online service
*/
class RAlwaysOnlineClientSession : public RSessionBase
    {

public:

    /**
    * Constructor
    * RAlwaysOnlineClientSession
    */
	IMPORT_C RAlwaysOnlineClientSession();

    /**
     * Destructor
     * ~RAlwaysOnlineClientSession
    */
    IMPORT_C ~RAlwaysOnlineClientSession();

    /**
    * Connect
    * @return TInt, KErrNone or system wide error code
    */
	IMPORT_C TInt Connect();

    /**
    * Version
    * @return TVersion always online version number
    */
	IMPORT_C TVersion Version() const;

    /**
    * Sends command with parameter to always online server, connection
    * must be established before calling this method
    * @since Series60 2.6
    * @param aCommand, command Id
    * @param aParameter, possible parameters
    */
    IMPORT_C void RelayCommandL(
        TAlwaysOnlineServerAPICommands aCommand,
        TDes8& aParameter );
    /**
    * Sends command with parameter to always online server, function
    * opens and closes connection
    * @since Series60 2.6
    * @param aCommand, Command to be executed
    * @param aParameter, The message to be delivered
    */
    IMPORT_C void SendSinglePacketL(
        const TAlwaysOnlineServerAPICommands aCommand,
        TDes8&                               aParameter );

    /**
    * Sends command with parameter asynchronically
    * @since Series60 3.2.3
    * @param aCommand, Command to be executed
    * @param aParameter, The message to be delivered
    * @param aStatus, status of callers active object
    */
    IMPORT_C void SendCommandAsync(
        const TAlwaysOnlineServerAPICommands aCommand,
        TDes8& aParameter,
        TRequestStatus& aStatus );

    /**
    * Sends command and receive result in a result parameter
    * @since Series60 3.2.3
    * @param aCommand, Command to be executed
    * @param aParameter, The message to be delivered
    * @param aResultParameter, Writable descriptor contains
    *                          result from ao server
    * @return TInt, KErrNone or system wide error code
    */
    IMPORT_C TInt SendReceiveSync(
            const TAlwaysOnlineServerAPICommands aCommand,
            TDes8& aParameter,
            TDes8& aResultParameter );

    };

#endif
//EOF
