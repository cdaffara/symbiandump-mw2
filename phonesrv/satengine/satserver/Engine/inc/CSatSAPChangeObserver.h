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
* Description:  Observes SIM Access Profile status changes.
*
*/



#ifndef CSATSAPCHANGEOBSERVER_H
#define CSATSAPCHANGEOBSERVER_H

//  INCLUDES
#include <e32base.h>
#include "MSatSystemStateObserver.h"

// FORWARD DECLARATION
class CSatCommandContainer;
class MSatSystemStateChangeNotifier;

// CLASS DECLARATION

/**
*  Observes SIM Access Profile status change and informs CSatCommandContriner
*  of a possible change.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class CSatSAPChangeObserver : public CBase,
                              public MSatSystemStateObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aContainer Container of command handlers.
        */
        static CSatSAPChangeObserver* NewL( CSatCommandContainer& aContainer );

        /**
        * Destructor.
        */
        virtual ~CSatSAPChangeObserver();

    public: // From base classes

        /**
        * From MSatSystemStateObserver Notifies that system state changed.
        * @param aValue The new value that has changed.
        */
        void StateChanged( const TInt aValue );

    private:

        /**
        * C++ default constructor.
        * @param aContainer Container of command handlers.
        */
        CSatSAPChangeObserver( CSatCommandContainer& aContainer );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Data

        // Command container.
        CSatCommandContainer& iContainer;

        // System state notifier.
        MSatSystemStateChangeNotifier* iStateNotifier;

    };

#endif      // CSATSAPCHANGEOBSERVER_H

// End of File
