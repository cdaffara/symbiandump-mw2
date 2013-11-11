
/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Location Trigger Manager monitoring
*
*/
#ifndef LBTTRIGGERMANAGER_H_
#define LBTTRIGGERMANAGER_H_

#include <lbtmanager.h>
#include <e32def.h>
#include <f32file.h>

NONSHARABLE_CLASS( CLocationTriggerManager ) :
public CActive
    {
    
    public:

        /*
         * Default constructor for CLocationTriggerManager class.
         */
        CLocationTriggerManager( );
        
        /*
         * Default destructor.
         */
        ~CLocationTriggerManager();
        
        /*
         * Intialises the lbt server and opens sessions to it.
         * The observer for management trigger changes is also started here.
         */
        void ConstructL();
        
        /*
         * Two phase constructor for CLocationTriggerManager
         */
        static CLocationTriggerManager* NewL( );

        /*
         * Two phase constructor for CLocationTriggerManager
         */
        static CLocationTriggerManager* NewLC( );
        
        /*
         * Derived from CActive
         */
        void RunL();
        
        /*
         * Derived from CActive
         */
        void DoCancel();
        
        /*
         * Derived from CActive
         */
        TInt RunError( TInt aError );
        
        /*
         * Start listening to events from Management UI
         */
        void StartManagerObserver();
        
    public :
        
        /*
         * Handle to LBT Server
         */
        RLbtServer iLbtServer;    

        /*
         * Handle to LBT Manager for monitoring trigger deletion
         */
        RLbtManager iLbtManager;
        
    };

#endif /* LBTTRIGGERMANAGER_H_ */
