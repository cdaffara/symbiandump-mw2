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
* Description:  Interface for refresh subsession.
*
*/



#ifndef MSATREFRESHSUBSESSION_H
#define MSATREFRESHSUBSESSION_H

//  INCLUDES
#include <etelsat.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class MSatCommand;
class MSatReplyObserver;

// CLASS DECLARATION

/**
*  Interface for refresh subsession.
*
*  @lib SatEngine.lib
*  @since Series S60 3.0
*/
class MSatRefreshSubSession
    {
    protected:

        /**
        * C++ default constructor.
        */
        MSatRefreshSubSession() {};

        /**
        * Destructor.
        */
        virtual ~MSatRefreshSubSession() {};

    public: // New functions

        /**
        * Query for allowing the refresh
        * @param aRefresh Refresh data.
        */
        virtual void QueryClient( const RSat::TRefreshV2& aRefresh ) = 0;

        /**
        * Refresh event.
        * @param aRefresh Refresh data.
        */
        virtual void NotifyClient( const RSat::TRefreshV2& aRefresh ) = 0;

        /**
        * Refresh cancel event.
        */
        virtual void NotifyClientCancel() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatRefreshSubSession( const MSatRefreshSubSession& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatRefreshSubSession& operator=( const MSatRefreshSubSession& );

    };

#endif      // MSATREFRESHSUBSESSION_H

// End of File
