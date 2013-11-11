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
* Description:  Factory class that provides SAT SystemState objects
*                outside of this DLL
*
*/



#ifndef TSATSYSTEMSTATEFACTORY_H
#define TSATSYSTEMSTATEFACTORY_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MSatSystemState;
class MSatSystemStateChangeNotifier;
class MSatSystemStateObserver;
class MSatMultiModeApi;

// CLASS DECLARATION

/**
*  Factory class.
*
*  @lib SatSystemState.lib
*  @since Series 60 3.0
*/
class TSatSystemStateFactory
    {
    public:  // Factory methods

        /**
        * Creates MSatSystemState object.
        * @param aPhone Reference to MSatMultiModeApi object.
        * @return Interface to SatSystemnState
        */
        IMPORT_C static MSatSystemState* CreateSystemStateL(
            MSatMultiModeApi& aPhone );

        /**
        * Creates idle mode change notifier.
        * @param aObserver Observer, which is notifed when change happens.
        * @return Interface to PSChangeNotifier listening Idle mode changes.
        */
        IMPORT_C static MSatSystemStateChangeNotifier*
            CreateIdleModeChangeNotifierL(
                MSatSystemStateObserver& aObserver );

        /**
        * Creates SIM status change notifier.
        * @param aObserver Observer, which is notifed when change happens.
        * @return Interface to PSChangeNotifier listening SIM status changes.
        */
        IMPORT_C static MSatSystemStateChangeNotifier*
            CreateSimStatusChangeNotifierL(
                MSatSystemStateObserver& aObserver );

        /**
        * Creates SIM access profile state change observer.
        * @param aObserver Observer, which is notifed when change happens.
        * @return Interface CenRepChangeNotifier listening SAP status.
        */
        IMPORT_C static MSatSystemStateChangeNotifier*
            CreateSIMAccessProfileChangeNotifierL(
                MSatSystemStateObserver& aObserver );

        /**
        * Creates Startup state change observer.
        * @param aObserver Observer, which is notifed when change happens.
        * @return Interface PSChangeNotifier listening startup status.
        */
        IMPORT_C static MSatSystemStateChangeNotifier*
            CreateStartupChangeNotifierL(
                MSatSystemStateObserver& aObserver );

        /**
        * Creates language selection observer.
        * @param aObserver Observer, which is notifed when change happens.
        * @return Interface to CenRepChangeNotifier listening language
        * selections.
        */
        IMPORT_C static MSatSystemStateChangeNotifier*
            CreateLanguageSelectionChangeNotifierL(
                MSatSystemStateObserver& aObserver );
                
        /**
        * Creates Profile state change observer.
        * @param aObserver Observer, which is notifed when change happens.
        * @return Interface to CenRepChangeNotifier listening profile
        * status.
        */
        IMPORT_C static MSatSystemStateChangeNotifier*
            CreateProfileChangeNotifierL(
                MSatSystemStateObserver& aObserver );
                
    };

#endif      // TSATSYSTEMSTATEFACTORY_H

// End of File
