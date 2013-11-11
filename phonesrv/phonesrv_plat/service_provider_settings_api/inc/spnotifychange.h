/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service provider settings change notifer header file
*
*/



#ifndef CSPNOTIFYCHANGE_H
#define CSPNOTIFYCHANGE_H

#include <e32base.h>
#include <e32property.h>
#include "spdefinitions.h"

class MSPNotifyChangeObserver;
class CSpsBufferedPublisher;

/**
 *  Service provider settings change notifer class.
 *
 *  Client uses this class to issue the change notify request
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CSPNotifyChange ): public CActive
    {
    public: // Constructor and destructor
    
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPNotifyChange* NewL( MSPNotifyChangeObserver& aObserver );

        /**
        * Destructors.
        */
        IMPORT_C virtual ~CSPNotifyChange();

    public: 
        /**
         * Issue notify service change rquest.
         *
         * @since S60 V3.2
         * @param aServiceIds The service ID array of monitored service entries
         */
        IMPORT_C void NotifyChangeL( const RIdArray& aServiceIds );

        /**
         * Cancel notify change.
         *
         * @since S60 V3.2
         */
        IMPORT_C void NotifyChangeCancel();

    protected: // from CActive

        void DoCancel();

        void RunL();
        
        TInt RunError( TInt aError );

        void Subscribe();
    private:

        /**
        * C++ default constructor.
        */
        CSPNotifyChange( MSPNotifyChangeObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:

        /**
         * Publish & Subscribe connection
         */
        RProperty       iProperty;

        /**
         * Observer object
         */
        MSPNotifyChangeObserver& iObserver;
        
        /**
         * Id array
         */
        RIdArray        iIdArray;
        
        /**
         * Contains buffered P&S data
         */
        CSpsBufferedPublisher* iData;
    };

#endif // CSPNOTIFYCHANGE_H

