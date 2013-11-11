/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*     Alwaysonline manager declaration file
*
*/


#ifndef __ALWAYSONLINEMANAGER_H__
#define __ALWAYSONLINEMANAGER_H__

// INCLUDES
#include <e32base.h>
#include <AlwaysOnlineManagerClient.h>
#include <msvapi.h>
#include <rconnmon.h>
#include <MuiuMsvSingleOpWatcher.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

#include "AlwaysOnlineEComInterface.h"
#include "AlwaysOnlineStatusQueryInterface.h"
#include "AlwaysOnlineManagerDiskSpaceObserver.h"

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS
class CAlwaysOnlineManagerServer;
class CAOCenRepControl;

// CLASS DECLARATION

/**
* CAlwaysOnlineManager
*
* This class contains the plugin related logic in always online server
* @since s60 2.1
*/
class CAlwaysOnlineManager :
    public CBase,
    public MAlwaysOnlineStatusQueryInterface,
    public MConnectionMonitorObserver,
    public MMsvSessionObserver,
    public MMsvSingleOpWatcher,
    public MCenRepNotifyHandlerCallback
    {
    public:


        /**
        * NewL();
        * @since Series60 2.6
        * @param CAlwaysOnlineManagerServer*, pointer to server
        * @return CAlwaysOnlineManager*, self
        */
        static CAlwaysOnlineManager* NewL( CAlwaysOnlineManagerServer* aServer );

        /**
        * Destructor
        * ~CAlwaysOnlineManager();
        */
        ~CAlwaysOnlineManager();

        /**
        * DoStartL();
        * @since Series60 2.6
        */
        void DoStartL();

        /**
        * RoamingStatus()
        * @since Series60 2.6
        * @param TInt, query id
        * @return TAny*, returned value for a query. Real type depends on
        * the query. See documentation
        */
        TAny* QueryStatusL( TInt aQuery );


        /**
        * HandleOfflineEventL
        * Sends event to plugins to inform them about changes between
        * offline/normal mode
        * @since Series60 2.6
        * @param TInt event id
        */
        void HandleOfflineEventL( TInt aEvent );


        /**
        * From connmon
        * @since Series60 2.6
        * @param CConnMonEventBase
        */
        void EventL( const CConnMonEventBase &aConnMonEvent );

        /**
        * HandleDiskSpaceEventL()
        * Handle event from disk space observer
        * @since Series60 2.6
        * @param TInt, event id
        */
        void HandleDiskSpaceEventL( TInt aEvent );

    private: // From base classes

        /**
        * From MCenRepNotifyHandlerCallback
        * Handles the incoming notifications of key changes
        * @since Series60 3.0
        * @param aId, Key that has changed
        * @param aNewValue, New value of the key
        */
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );

        /**
        * From MCenRepNotifyHandlerCallback
        * Handles the notifier errors
        * @since Series60 3.0
        * @param aId, Key that has changed
        * @param aNewValue, New value of the key
        */
        void HandleNotifyError(
            TUint32 aId,
            TInt aError,
            CCenRepNotifyHandler* aHandler );

        /**
        * HandleSessionEventL
        * @since Series60 2.6
        * @param TMsvSessionEvent, session event
        * @param TAny* parameter 1
        * @param TAny* parameter 2
        * @param TAny* parameter 3
        */
        void HandleSessionEventL(TMsvSessionEvent aEvent,
            TAny* aArg1,
            TAny* /*aArg2*/,
            TAny* /*aArg3*/);

    public:

        /**
        * BroadcastClientCommandL
        * @since Series60 2.6
        */
        void BroadcastClientCommandL( TAlwaysOnlineServerAPICommands aCommand, TDes8& aParameter );

        /**
        * OpCompleted()
        * From MMsvSingleOpWatcher
        * @since Series60 2.6
        * @param CMsvSingleOpWatcher& opwatcher which completed
        * @param TInt, completion code
        */
        virtual void OpCompleted(
            CMsvSingleOpWatcher& aOpWatcher,
            TInt aCompletionCode );
    private:

        /**
        * C++ constructor
        * CAlwaysOnlineManager();
        */
        CAlwaysOnlineManager();

        /**
        * ConstructL()
        * @since Series60 2.6
        * @param CAlwaysOnlineManagerServer*, server pointer
        */
        void ConstructL( CAlwaysOnlineManagerServer* aServer );

        /**
        * LoadPluginsL();
        * @since Series60 2.6
        */
        void LoadPluginsL();

        /**
        * StartPluginsL();
        * @since Series60 2.6
        */
        void StartPluginsL();

        /**
        * Check network status and report it to plugins;
        * @since Series60 2.6
        */
        void CheckNetworkStatusAndSetObserverL();

        /**
        * Takes action based on registration status
        * @since Series60 2.6
        * @param TInt, status id
        */
        void HandleRegistrationStatusL( TInt aStatus );

        /**
        * Sends command to all plugins
        * @since Series60 3.2.3
        * @param TManagerServerCommands, command for plugin
        * @param TDes8&, parameter
        */
        void BroadcastCommandDirectL(
            TManagerServerCommands aCommand,
            TDes8& aParameter );

        /**
        *
        * @since Series60 2.6
        */
        void InvokeCommandHandlerL( const TManagerServerCommands aCommand,
            TDes8& aParameter,
            TInt aIndex );

        /**
        * Queries all the started plugins, whether they want to be shutdown or not.
        * @since Series60 3.1
        */
        void QuerySuicideL();

        /**
        * Handles starting of plugin, which UID is inside parameter.
        * @since Series60 3.1
        * @param aParameter, UID of plugin
        */
        void HandleStartPluginL( const TDes8& aParameter );

        /**
        * Handles stopping of plugin, which UID is inside parameter.
        * @since Series60 3.1
        * @param aParameter, UID of plugin
        */
        void HandleStopPluginL( const TDes8& aParameter );

        /**
        * Handles stopping of plugin with given implementation UID.
        * @since Series60 3.1
        * @param aPluginImplementationUid, implementation UID of plugin 
        *        (implementation_uid in .rss). 
        *        NOTE: There can be multiple instances of same plugin 
        *        implementation.
        */
        void StopPluginL( const TUid& aPluginImplementationUid );

        /**
        * Unloads the plugin with given instance UID and sets it as disabled.
        * @since Series60 3.1
        * @param aPluginInstanceUid, UID of plugin instance
        */
        void DisablePlugin( const TUid& aPluginInstanceUid );

        /**
        * Handles the final progress of an operation.
        * @since Series60 3.1
        * @param aProgress, progress from operation
        */
        void HandleOpCompletionL( const TDesC8& aProgress );

        /**
        * Handles the parsing of final progress.
        * @since Series60 3.1
        * @param aProgress, final progress of an operation
        * @param aUid, UID of plugin
        * @param aCommand, command executed in operation
        * @param aResult, final result of an operation
        */
        TBool ParseProgressL( 
            const TDesC8& aProgress, 
            TUid& aUid,
            TInt& aCommand,
            TDes8& aResult );

    //data
    private:
        TBool                           iOffline;
        TInt                            iNetworkStatus;
        TBool                           iPluginsLoaded;
        CArrayFixFlat<CAlwaysOnlineEComInterface*>* iPluginArray;
        CArrayFixFlat<TUid>*            iDisabledPluginUidsArray;
        CAlwaysOnlineManagerServer*     iServer;
        CAlwaysOnlineDiskSpaceObserver* iDiskSpaceObserver;
        RFs                             iRfs;
        CMsvSession*                    iSession;
        // Array to store the operation
        CMsvSingleOpWatcherArray*       iOperations;
        TInt                            iLastDiskEvent;
        // Used to listen network state changes
        RConnectionMonitor              iConMon;
        // Handle all the cenrep actions
        CAOCenRepControl*               iCenRepControl;
    };

#endif
//EOF
