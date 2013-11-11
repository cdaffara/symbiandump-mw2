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
* Description:  Interface for Publish and suscripe change notifier.
*
*/



#ifndef MSATSYSTEMSTATECHANGENOTIFIER_H
#define MSATSYSTEMSTATECHANGENOTIFIER_H

//  INCLUDES
#include <e32std.h>

// CONSTANTS
// Indicates that the observer doesn't want to specify any value, when it should
// be notified. In this case, the changed value is sent to observer
const TInt KNotifyValueNotSet = -1;

// CLASS DECLARATION

/**
*  Change notifier for Publish & Subscribe.
*
*  @lib SatSystemState
*  @since Series 60 3.0
*/
class MSatSystemStateChangeNotifier
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~MSatSystemStateChangeNotifier() {};

    public: // New functions

        /**
        * Request a notification of a change in system state.
        * The request is standing as long as it is canceled.
        */
        virtual void NotifyChangeL() = 0;

        /**
        * Cancels the notification request.
        */
        virtual void CancelNotify() = 0;
        
        /**
        * Gets the current value.
        * @return Current value for earlier request.
        */
        virtual TInt GetValueInt() { return KNotifyValueNotSet; };

    protected:  // New functions

        /**
        * C++ default constructor.
        */
        MSatSystemStateChangeNotifier() {};

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSystemStateChangeNotifier( const MSatSystemStateChangeNotifier& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSystemStateChangeNotifier& operator=( const MSatSystemStateChangeNotifier& );

    };

#endif      // MSATSYSTEMSTATECHANGENOTIFIER_H

// End of File
