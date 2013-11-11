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
* Description:  Interface for Ui sessions
*
*/



#ifndef MSATUISESSION_H
#define MSATUISESSION_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
#include "SatSOpcodes.h"

// FORWARD DECLARATION
class MSatCommand;

// CLASS DECLARATION

/**
*  Ui sub session interface for commands.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class MSatUiSession
    {
    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatUiSession() {};

        /**
        * Destructor.
        */
        virtual ~MSatUiSession() {};

    public: // New functions

        /**
        * Closes the ui session.
        */
        virtual void CloseUiSession() = 0;

        /**
        * Clears the screen.
        */
        virtual void ClearScreen() = 0;
        
        /**
        * Ui Event
        */
        virtual void UiEventNotification( 
            TInt aEventId,
            TInt aEventStatus,
            TInt aEventError ) = 0;

        /**
        * Notification that command data is available.
        * @param aCmdData Data to be sent to client.
        * @param aCmdRsp Response from Client is written to this.
        * @param aRequest Request, where the data is sent.
        */
        virtual void SendCommand(
            TDesC8* aCmdData,
            TDes8* aCmdRsp,
            TSatServerRequest aRequest ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatUiSession( const MSatUiSession& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatUiSession& operator=( const MSatUiSession& );

    };

#endif      // MSATUISESSION_H

// End of File
