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
* Description:  API for getting refresh notifications.
*
*/



#ifndef RSATREFRESH_H
#define RSATREFRESH_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
#include <msatrefreshobserver.h>

// FORWARD DECLARATIONS
class RSatSession;
class CSatAllowRefreshMonitor;
class CSatRefreshMonitor;

// CLASS DECLARATION

/**
*  Sub-session for getting refresh notifies.
*
*  @lib SatClient.lib
*  @since Series 60 2.6
*/
class RSatRefresh : public RSubSessionBase
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        * @param aObserver Observer, which is notified about
        * refresh.
        */
        IMPORT_C RSatRefresh( MSatRefreshObserver& aObserver );

    public: // New functions

        /**
        * Opens the sub-session.
        * @param aSession Opens the sub-session.
        */
        IMPORT_C void OpenL( const RSatSession& aSession );

        /**
        * Closes the sub-session. Cancels
        * the notification if active.
        */
        IMPORT_C void Close();

        /**
        * Starts the waiting for refresh events.
        */
        IMPORT_C void NotifyFileChangeL();

        /**
        * Starts the waiting for refresh events.
        * @param aObservedFiles List of files to be observed for refresh.
        */
        IMPORT_C void NotifyFileChangeL(
            const TSatRefreshFiles& aObservedFiles );

        /**
        * Indicates client's refresh actions complete.
        * @param aAdditionEFRead Client has read additional file(s) from those
        *                        implicitly defined by refresh.
        */
        IMPORT_C void RefreshEFRead( TBool aAdditionEFRead ) const;

        /**
        * Cancels the notification.
        */
        IMPORT_C void Cancel() const;

        /**
        * Gets the notification of refresh query.
        * @param aType Refresh type.
        * @param aChangingFiles List of files to be changed if any.
        * @param aStatus Request status.
        */
        void NotifyAllowRefresh(
            TPckg<TSatRefreshType>& aType,
            TPckg<TSatRefreshFiles>& aChangingFiles,
            TRequestStatus& aStatus ) const;

        /**
        * Sends response for refresh quary.
        * @param aAllow ETrue, refresh is allowed, EFalse refresh not allowed.
        */
        void AllowRefreshL( TBool aAllow ) const;

        /**
        * Cancels the notification.
        * @param aStatus Request status.
        */
        void CancelNotifyAllowRefresh( TInt aError = KErrNone ) const;

        /**
        * Gets the notification of refresh.
        * @param aType Refresh type.
        * @param aFiles List of changed files if any.
        * @param aStatus Request status.
        */
        void NotifyRefresh(
            TPckg<TSatRefreshType>& aType,
            TPckg<TSatRefreshFiles>& aFiles,
            TRequestStatus& aStatus ) const;

        /**
        * Cancels the notification.
        */
        void CancelNotifyRefresh() const;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        RSatRefresh( const RSatRefresh& );

        // Prohibit assigment operator if not deriving from CBase.
        RSatRefresh& operator=( const RSatRefresh& );

    private:

        // Observer of refresh events.
        MSatRefreshObserver& iObserver;

        // Active object to monitor refresh query.
        CSatAllowRefreshMonitor* iAllowMonitor;

        // Active object to monitor refresh.
        CSatRefreshMonitor* iRefreshMonitor;

        //lint -e{1510} Desstruction of objects has been taken care
    };

#endif      // RSATREFRESH_H

// End of File
