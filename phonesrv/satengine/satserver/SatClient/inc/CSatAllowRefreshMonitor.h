/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Waits for the refresj query notification
*
*/



#ifndef CSATALLOWREFRESHMONITOR_H
#define CSATALLOWREFRESHMONITOR_H

//  INCLUDES
#include <e32base.h>
#include <msatrefreshobserver.h>

// FORWARD DECLARATIONS
class RSatRefresh;

// CLASS DECLARATION

/**
*  Active object to wait the refresh query notification.
*
*  @lib SatClient.lib
*  @since 2.6
*/
class CSatAllowRefreshMonitor : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aObserver Observer for refresh query.
        * @param aRefresh Refresh sub-session.
        */
        static CSatAllowRefreshMonitor* NewL(
            MSatRefreshObserver& aObserver,
            RSatRefresh& aRefresh );

        /**
        * Destructor.
        */
        virtual ~CSatAllowRefreshMonitor();

    public: // New functions

        /**
        * Set files to be observed.
        * @since 3.0
        * @param aObservedFiles List of files to be observed for refresh.
        */
        void ObserveFiles( const TSatRefreshFiles& aObservedFiles );

        /**
        * Starts waiting for the refresh query.
        */
        void Start();

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        * @param aStatus Request status.
        */
        TInt RunError( TInt aError );

        /**
        * From CActive
        */
        void DoCancel();

    private:

        /**
        * C++ default constructor.
        */
        CSatAllowRefreshMonitor(
            MSatRefreshObserver& aObserver,
            RSatRefresh& aRefresh );

    private:

        // Notified when query comes.
        MSatRefreshObserver& iObserver;

        // Refresh sub-sesion.
        RSatRefresh& iRefresh;

        // Changing files
        TSatRefreshFiles iChangingFiles;

        // Package of chaning files.
        TPckg<TSatRefreshFiles> iChangingFilesPckg;

        // Observed files
        TSatRefreshFiles iObservedFiles;

        // Refresh type.
        TSatRefreshType iType;

        // Refresh type package.
        TPckg<TSatRefreshType> iTypePckg;

    };

#endif      // CSATALLOWREFRESHMONITOR_H

// End of File
