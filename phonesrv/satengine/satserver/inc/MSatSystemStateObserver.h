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
* Description:  Observer for system state changes.
*
*/



#ifndef MSATSYSTEMSTATEOBSERVER_H
#define MSATSYSTEMSTATEOBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Observer for system state changes.
*
*  @lib SatSystemState.lib
*  @since Series 60 3.0
*/
class MSatSystemStateObserver
    {
    public: // New functions

        /**
        * Notifies that system state changed.
        */
        virtual void StateChanged() {};

        /**
        * Notifies that system state changed.
        * @param aValue The new value that has changed.
        */
        virtual void StateChanged( const TInt /*aValue*/ ) {};

        /**
        * Notifies that system state changed.
        * @param aValue The new value that has changed.
        */
        virtual void StateChanged( const TReal& /*aValue*/ ) {};

        /**
        * Notifies that system state changed.
        * @param aValue The new value that has changed.
        */
        virtual void StateChanged( const TDesC8& /*aValue*/ ) {};

        /**
        * Notifies that system state changed.
        * @param aValue The new value that has changed.
        */
        virtual void StateChanged( const TDesC16& /*aValue*/ ) {};

    protected:  // New functions

        /**
        * Destructor.
        */
        virtual ~MSatSystemStateObserver() {};

        /**
        * C++ default constructor.
        */
        MSatSystemStateObserver() {};

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSystemStateObserver( const MSatSystemStateObserver& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSystemStateObserver& operator=( const MSatSystemStateObserver& );

    };

#endif      // MSATSYSTEMSTATEOBSERVER_H

// End of File
