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
* Description:  Notifies the changes in publish and suscripe properties.
*
*/



#ifndef CSATPSCHANGENOTIFIER_H
#define CSATPSCHANGENOTIFIER_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "MSatSystemStateChangeNotifier.h"

// FORWARD DECLARATION
class MSatSystemStateObserver;

// CLASS DECLARATION

/**
*  Notifies the changes in publish and suscripe properties.
*
*  @lib SatSystemState
*  @since Series 60 3.0
*/
class CSatPSChangeNotifier : public CActive,
                             public MSatSystemStateChangeNotifier
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aPropertyKey This is the P&S key.
        * @param aPropertyValue Value of the P&S key.
        * @param aObserver A class that is notified when value changes.
        * @param aNotifyValue Defines the value that causes the notification.
        */
        static CSatPSChangeNotifier* NewL(
            const TUid& aPropertyKey,
            TUint aPropertyValue,
            MSatSystemStateObserver& aObserver,
            TInt aNotifyValue = -1 );

        /**
        * Destructor.
        */
        virtual ~CSatPSChangeNotifier();

    public: // Functions from base classes

        /**
        * From MSatSystemStateChangeNotifier Request a notification of a
        * change in system state.
        */
        void NotifyChangeL();

        /**
        * From MSatSystemStateChangeNotifier Cancels the notification request.
        */
        void CancelNotify();
               
        /**
        * From MSatSystemStateChangeNotifier Gets the current value.
        * @return Current value for earlier request.
        */
        TInt GetValueInt();

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive.
        */
        void RunL();

    private: // New methods

        /**
        * Makes the request and activates the active object.
        */
        void Activate();

    private:

        /**
        * C++ default constructor.
        * @param aPropertyKey This is the P&S key.
        * @param aPropertyValue Value of the P&S key.
        * @param aObserver A class that is notified when value changes.
        * @param aNotifyValue Defines the value that causes the notification.
        */
        CSatPSChangeNotifier(
            const TUid& aPropertyKey,
            TUint aPropertyValue,
            MSatSystemStateObserver& aObserver,
            TInt aNotifyValue );

    private:    // Data

        // Change observer.
        MSatSystemStateObserver& iObserver;

        // Publish & suscripe handle.
        RProperty iPSClient;

        // Publish&Suscripe property key
        TUid iPSKey;

        // Publish&Suscripe property value
        TUint iPSValue;

        // Value of the property when observer should be notified
        // If this is not set, the changed value is sent to observer
        TInt iNotifyValue;

    };

#endif      // CSATPSCHANGENOTIFIER_H

// End of File
