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
* Description:  Command interface for use of ui session.
*
*/



#ifndef MSATCOMMAND_H
#define MSATCOMMAND_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Command interface. Offers accessors for command id and command data.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatCommand
    {
    public: // Enums

        enum TSatCmdId
            {
            ESetUpMenuCmdId,
            ESetUpCallCmdId,
            ESelectItemCmdId,
            ESimSessionEndCmdId,
            ESendSmCmdId,
            ELaunchBrowserCmdId,
            EDisplayTextCmdId,
            EGetInputCmdId,
            EGetInkeyCmdId,
            ECallControlCmdId,
            EMoSmControlCmdId,
            ESetUpEventListCmdId,
            ESetUpIdleModeTextCmdId,
            ESendDtmfCmdId,
            ESendSsCmdId,
            ESendUssdCmdId,
            EPlayToneCmdId,
            ERefreshCmdId
            };

    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatCommand() {};

        /**
        * Destructor.
        */
        virtual ~MSatCommand() {};

    public: // New functions

        /**
        * Response from client.
        */
        virtual void ClientResponse() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatCommand( const MSatCommand& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatCommand& operator=( const MSatCommand& );

    };

#endif      // MSATCOMMAND_H

// End of File
