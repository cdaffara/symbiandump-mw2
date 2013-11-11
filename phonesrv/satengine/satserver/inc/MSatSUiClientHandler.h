/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MSatSUiClientHandler provides functions for some UI activity
*                like LaunchUi
*
*/



#ifndef MSATSUICLIENTHANDLER_H
#define MSATSUICLIENTHANDLER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATION
class MSatUiSession;
class MSatShellController;

// CLASS DECLARATION

/**
*  Interface provides some activity conserning UI client
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class MSatSUiClientHandler
    {

     protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatSUiClientHandler() {};

        /**
        * Destructor.
        */
        virtual ~MSatSUiClientHandler() {};

    public: // New functions

        /**
        * Gives pointer to ui session interface. If pointer is NULL, then
        * ui session is not available.
        * @return Pointer to ui session interface.
        */
        virtual MSatUiSession* UiSession() = 0;

        /**
        * Launches the SAT UI Client.
        */
        virtual void LaunchSatUiL() = 0;

        /**
        * Tells if user has started the ui client.
        * @return ETrue if user has launched the ui client.
        */
        virtual TBool UiLaunchedByUser() = 0;

        /**
        * Gives pointer to MSatShellController.
        * @return Pointer to sat shell controller.
        */
        virtual MSatShellController& ShellController() = 0;

        /**
        * Tells is UI closing
        * @return Boolean value indicating is UI closing
        */
        virtual TBool IsUiClosing() const = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSUiClientHandler( const MSatSUiClientHandler& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSUiClientHandler& operator=( const MSatSUiClientHandler& );
    };

#endif      // MSATSUICLIENTHANDLER_H

// End of File
