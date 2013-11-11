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
* Description:  Common session for accessing SatServer services.
*
*/

#ifndef RSATSESSION_H
#define RSATSESSION_H

#include <e32std.h>
#include <e32base.h>

/**
 * Definitions of SAT API versions.
 */ 
#define SIM_ATK_SERVICE_API_V1

/**
* Handles the connection to SatServer. This class
* is used with the sub-sessions.
*
* @lib SatClient
* @since Series60 2.6
*/
class RSatSession : public RSessionBase
    {

    public: // New functions

        /**
        * C++ default constructor.
        */
        IMPORT_C RSatSession();

        /**
        * Opens session to SatServer
        */
        IMPORT_C void ConnectL();

        /**
        * The client API version.
        */
        IMPORT_C TVersion Version() const;

    private:

        /**
        * Prohibit copy constructor if not deriving from CBase.
        */
        RSatSession( const RSatSession& );

        /**
        * Prohibit assigment operator if not deriving from CBase.
        */
        RSatSession& operator=( const RSatSession& );

    };

#endif      // RSATSESSION_H
