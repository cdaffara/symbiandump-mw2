/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer of events.
*
*/



#ifndef MSATEVENTOBSERVER_H
#define MSATEVENTOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Interface for observing events.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatEventObserver
    {
    protected: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatEventObserver() {};

        /**
        * Destructor.
        */
        virtual ~MSatEventObserver() {};

    public: // New functions

        /**
        * Notification of event.
        * @param aEvent Identifies the event.
        */
        virtual void Event( TInt aEvent ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatEventObserver( const MSatEventObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatEventObserver& operator=( const MSatEventObserver& );

    };

#endif      // MSATEVENTOBSERVER_H

// End of File
