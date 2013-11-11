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
* Description:  Active object to monitor the refresh event.
*
*/



#ifndef CSATREFRESHMONITOR_H
#define CSATREFRESHMONITOR_H

//  INCLUDES
#include <e32base.h>
#include <msatrefreshobserver.h>

// FORWARD DECLARATIONS
class RSatRefresh;

// CLASS DECLARATION

/**
*  Active object for monitoring the refresh event.
*
*  @lib SatClient
*  @since 2.6
*/
class CSatRefreshMonitor : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aRefresh Refresh sub-session.
        * @param aObserver Observer for refresh event.
        */
        static CSatRefreshMonitor* NewL(
            RSatRefresh& aRefresh,
            MSatRefreshObserver& aObserver );

        /**
        * Destructor.
        */
        virtual ~CSatRefreshMonitor();

    public: // New functions

        /**
        * Starts to monitor the refresh event.
        * @since 2.6
        */
        void Start();

    protected: // Functions from Base classes.

        /**
        * From CActive.
        */
        void RunL();

        /**
        * From CActive.
        */
        void DoCancel();

    private:

        /**
        * C++ default constructor.
        * @param aRefresh Refresh sub-session.
        * @param aObserver Observer for refresh event.
        */
        CSatRefreshMonitor(
            RSatRefresh& aRefresh,
            MSatRefreshObserver& aObserver );

    private:    // Data
        // Observer
        MSatRefreshObserver& iObserver;

        // Refresh sub session.
        RSatRefresh& iRefresh;

        // Changed files
        TSatRefreshFiles iChangedFiles;

        // Changed files package.
        TPckg<TSatRefreshFiles> iChangedFilesPckg;

        // Refresh type.
        TSatRefreshType iType;

        // Refresh type package.
        TPckg<TSatRefreshType> iTypePckg;
    };

#endif      // CSATREFRESHMONITOR_H

// End of File
