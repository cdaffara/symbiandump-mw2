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
* Description:  Observer for CSatIconHandler
*
*/



#ifndef MSATICONOBSERVER_H
#define MSATICONOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Observer for CSatIconHandler
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class MSatIconObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        MSatIconObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatIconObserver() {};

    public: // New functions

        /**
        * Notification of getting icon info from SIM.
        */
        virtual void NotifyGetIconInfo() = 0;

        /**
        * Notification of getting icon image.
        */
        virtual void NotifyGetIconInstanceL() = 0;

        /**
        * Notification of getting icon color lookup table.
        */
        virtual void NotifyGetClutL() = 0;

        /**
        * Notification of operation failure.
        * @param aErrorCode Reason of failure.
        */
        virtual void NotifyFailure( TInt aErrorCode ) = 0;
        
        /**
        * Error completion for handler
        * @param aError Received error code
        * @return Result of completion
        */
        virtual TInt CompleteError( TInt aError ) const = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatIconObserver( const MSatIconObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatIconObserver& operator=( const MSatIconObserver& );

    };

#endif      // MSATICONOBSERVER_H

// End of File
