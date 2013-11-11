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
* Description:  Accessors for different kind of sessions in SatServer
*
*/



#ifndef MSATSSESSIONS_H
#define MSATSSESSIONS_H

//  INCLUDES
#include <e32std.h>
#include "MSatRefreshSubSession.h"

// FORWARD DECLARATION
class MSatUiSession;

// CLASS DECLARATION

/**
*  Accessors for SatServer sessions.
*
*  @lib SatServer
*  @since Series 60 3.0
*/
class MSatSSessions
    {
    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatSSessions() {};

        /**
        * Destructor.
        */
        virtual ~MSatSSessions() {};

    public: // New functions

        /**
        * Gives ui session interface. May be NULL.
        * @return Interface for ui session
        */
        virtual MSatUiSession* UiSession() = 0;

        /**
        * Gives all refresh sub sessions.
        * @return Pointer array of MSatRefreshSubSessions
        */
        virtual const RPointerArray<MSatRefreshSubSession>&
            RefreshSubSessions() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSSessions( const MSatSSessions& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSSessions& operator=( const MSatSSessions& );

    };

#endif      // MSATSSESSIONS_H

// End of File
