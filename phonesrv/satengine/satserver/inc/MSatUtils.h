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
* Description:  Utility methods for SAT command handling.
*
*/



#ifndef M_MSATUTILS_H
#define M_MSATUTILS_H


#include <e32std.h>
#include <etelsat.h>
#include "MSatRefreshSubSession.h"
#include "SatSOpcodes.h"
#include "msatasynctosync.h"

class MUiSessionLaunchObserver;
class MSatApi;
class MSatUiSession;
class MSatEventObserver;
class MSatSystemState;
class MSatShellController;
class MSatSUiClientHandler;
class MSatCommand;
class CSatClientServiceReq;
class MSatEventMonitorContainer;
class MSatEventMonitorUtils;
class MSatSSimSubscriberId;
class MSatBIPUtils;
class MSatBIPEventNotifier;
class RMobilePhone;
class RMmCustomAPI;
class MSatMediatorEventProvider;
class MSatMultiModeApi;

/**
 *  Utility methods for SAT command handling.
 *
 *  @lib SatServer
 *  @since S60 v3.0
 */
class MSatUtils
    {

public:

    /**  For notifying SAT commands/events */
    enum TSatEvent
        {
        /**  Originator:  CSetUpMenuHandler */
        /**  Listeners:   None */
        ESetUpMenuExecuting,

        /**  Originator:  CSimSessionEndHandler */
        /**  Listeners:   CSetUpMenuHandler */
        ESimSessionEndExecuting,

        /**  Originator:  CSelectItemHandler */
        /**  Listeners:   None */
        ESelectItemExecuting,

        /**  Originator:  CSetUpCallHandler */
        /**  Listeners:   None */
        ESetUpCallExecuting,

        /**  Originator:  CDisplayTextHandler */
        /**  Listeners:   None */
        EDisplayTextExecuting,

        /**  Originator:  CLaunchBrowserHandler */
        /**  Listeners:   None */
        ELaunchBrowserExecuting,

        /**  Originator:  CGetInputHandler */
        /**  Listeners:   None */
        EGetInputExecuting,

        /**  Originator:  CGetInkeyHandler */
        /**  Listeners:   None */
        EGetInkeyExecuting,

        /**  Originator:  CSetUpEventListHandler */
        /**  Listeners:   None */
        ESetUpEventListExecuting,

        /**  Originator:  CSetUpIdleModeTextHandler */
        /**  Listeners:   None */
        ESetUpIdleModeTextExecuting,

        /**  Originator:  CSendDtmfHandler */
        /**  Listeners:   None */
        ESendDtmfExecuting,

        /**  Originator:  CSendSsHandler */
        /**  Listeners:   None */
        ESendSsExecuting,

        /**  Originator:  CSendUssdHandler */
        /**  Listeners:   None */
        ESendUssdExecuting,

        /**  Originator:  CPlayToneHandler */
        /**  Listeners:   None */
        EPlayToneExecuting,

        /**  Originator:  CProvideLocalInfoHandler */
        /**  Listeners:   None */
        ELocalInfoExecuting,

        /**  Originator:  CRefreshHandler */
        /**  Listeners:   CClearScreenHandler */
        ERefreshExecuting,

        /**  Originator:  CCallControlHandler */
        /**  Listeners:   CSetUpCallHandler */
        /**               CSendSsHandler */
        /**               CSendUssdHandler */
        ECallControlExecuting,

        /**  Originator:  CSendSmHandler */
        /**  Listeners:   None */
        ESendSmExecuting,

        /**  Originator:  CMoSmControlHandler */
        /**  Listener:    CSendSmHandler */
        EMoSmControlExecuting,

        /**  Originator:  CMoSmControlHandler */
        /**  Listener:    CSendSmHandler */
        EMoSmControlDone,

        /**  Originator:  CCallControlHandler */
        /**  Listeners:   CSetUpCallHandler */
        /**               CSendSsHandler */
        /**               CSendUssdHandler */
        ECallControlDone,

        /**  Originator:  CSatSSession */
        /**  Listeners:   CSatCommandContainer */
        ESatUiClosed,

        /**  Originator:  CSatSSession */
        /**  Listeners:   CSetUpMenuHandler */
        ESatUiLaunched,

        /**  originators: CSendSmHandler */
        /**  Listeners:   CSatSUiSubSession */
        ESmsSent,

        /**  Originators: CSetUpCallHandler */
        /**               CSelectItemHandler */
        /**  Listeners    CSimSessionEndHandler */
        ESessionTerminatedByUser,

        /**  Originators: CSatCommandHandler */
        /**  Listeners:   CSimSessionEndHandler */
        EDestroySimSessionEndTimer,

        /**  Originators: CSatCommandHandler */
        /**  Listeners:   CSimSessionEndHandler */
        EDelaySimSessionEnd,

        /**  Originators: CDisplayTextHandler */
        /**  Listeners:   CSetUpMenuHandler */
        ESetUpMenuNeeded,

        /**  Originators: CCallControlHandler */
        /**               CMoSmControlHandler*/
        /**  Listeners:   CSimSessionEndHandler */
        ESimSessionEndCallBack,

        /**  Originators: CSatIconSubSession */
        /**  Listeners:   CSetUpIdleModeTextHandler */
        EIdleModeResponseSuccess,

        /**  Originators: CSatIconSubSession */
        /**  Listeners:   CSetUpIdleModeTextHandler */
        EIdleModeResponseSuccessNoIcon,

        /**  Originators: CSatIconSubSession */
        /**  Listeners:   CSetUpIdleModeTextHandler */
        EIdleModeResponseBeyondCapabilities,

        /**  Originators: CSatIconSubSession */
        /**  Listeners:   CSetUpIdleModeTextHandler */
        EIdleModeResponseUnableToProcess,

        /**  Originator:  CSatSRefreshSubSession. */
        /**  Listeners:   CRefreshHandler */
        ERefreshAllowed,

        /**  Originator:  CSatSRefreshSubSession. */
        /**  Listeners:   CRefreshHandler */
        ERefreshNotAllowed,

        /**  Originator:  CSatSRefreshSubSession. */
        /**  Listeners:   CRefreshHandler */
        ERefreshEFRead,

        /**  Originator:  CSatSRefreshSubSession. */
        /**  Listeners:   CRefreshHandler */
        ERefreshAdditionalEFRead,

        /**  Originators: CSatSSimSubscriberId */
        /**  Listeners:   CSatCommandContainer */
        ECmccSimDetected,

        /**  Originators: CSatCommandContainer */
        /**  Listeners:   CSetUpMenuHandler */
        EApplNameUpdated,

        /**  Originators: COpenChannelHandler */
        /**  Listeners:   CReceiveDataHandler */
        /**               CSendDataHandler */
        EOpenChannelExecuting,

        /**  Originators: CCloseChannelHandler */
        /**  Listeners:   None */
        ECloseChannelExecuting,

        /**  Originators: CGetChannelStatusHandler */
        /**  Listeners:   None */
        EGetChannelStatusExecuting,

        /**  Originators: CSendDataHandler */
        /**  Listeners:   None */
        ESendDataExecuting,

        /**  Originators: CReceiveDataHandler */
        /**  Listeners:   None */
        EReceiveDataExecuting,

        /**  Originators: CDisplayTextHandler */
        /**  Listeners:   CSimSessionEndHandler */
        ESustainedTextInDisplay,

        /**  Originators: CDisplayTextHandler */
        /**  Listeners:   CSimSessionEndHandler */
        ESustainedTextRemoved,

        /**  Originators: COpenChannelHandler */
        /**  Listeners:   CSimSessionEndHandler */
        EDataChannelsActive,

        /**  Originators: CCloseChannelHandler */
        /**  Listeners:   CSimSessionEndHandler */
        EDataChannelsInactive,

        /**  Originators: CSendDataHandler */
        /**               CReceiveDataHandler */
        /**  Listeners:   CSatSEventMoniotorContainer */
        EBipCommandExecuting,
        EBipCommandDoneExecuting,

        /**  Originators: CSatSUiSubSession */
        /**  Listeners:   Command handlers */
        ECommandCancelled,
        ECancelledUsingEndKey,

        /**  Originators: CRefreshHandler */
        /**  Listeners:   CSetUpIdleModeTextHandler */
        ESimInitDone,

        /**  Originators: CRefreshRequiredHandler */
        /**  Listeners:   CSetUpMenuHandler */
        /**               CSetUpIdleModeTextHandler */
        /**               CSetUpEventListHandler */
        ESimResetCalled,

        /**  Originators: CRefreshHandler */
        /**  Listeners:   CSetUpIdleModeTextHandler */
        /**               CSetUpEventListHandler */
        ESimResetCancelled,

        /**  Originator:  CSatUiSubSession */
        /**  Listeners:   CLaunchBrowserHandler */
        ESetUpMenuRequested,

        /** Originator:  CSatClientServiceReq */
        /** Listeners:   CSatSUiClientHandler */
        EBringSatUiToForeGround,

        /** Originator:  CSatSAPChangeObserver */
        /** Listeners:   CSetUpMenuHandler */
        EAddSatUiCalled,
        ERemoveSatUiCalled

        };


    /**
     * Gives USAT API.
     *
     * @since S60 v3.0
     * @return Reference to USAT API wrapper.
     */
    virtual MSatApi& USatAPI() = 0;

    /**
     * Gives reference to system state interface
     *
     * @since S60 v3.0
     * @return Reference to system state interface.
     */
    virtual MSatSystemState& SystemState() = 0;

    /**
     * Gives reference to MSatSUiClientHandler
     *
     * @since S60 v3.0
     * @return Pointer to sat ui handler.
     */
    virtual MSatSUiClientHandler& SatUiHandler() = 0;

    /**
     * Registers observer for event. Same observer will not be added
     * more than once for one event.
     *
     * @since S60 v3.0
     * @param aObserver Pointer of observer instance. Panic will occur if
     * aObserver is NULL.
     * @param aEvent Event, which will cause the notification of observer.
     */
    virtual void RegisterL(
            MSatEventObserver* aObserver,
            TSatEvent aEvent ) = 0;

    /**
     * Notifies event to all registered observers.
     *
     * @since S60 v3.0
     * @param aEvent Event to be notified.
     */
    virtual void NotifyEvent( TSatEvent aEvent ) = 0;

    /**
     * From MSatEventObserver.Sends UI Event Notification,
     * if UI session is found
     *
     * @since S60 v3.1
     * @param aEvenId, identifies the event
     * @param aEventStatus, status of the event
     * @param aEventError, possible error value
     */
    virtual void NotifyUiEvent(
            TInt aEventId,
            TInt aEventStatus,
            TInt aEventError ) = 0;

    /**
     * Unregisters the event notification.
     *
     * @since S60 v3.0
     * @param aObserver Observer of the event
     * @param aEvent Event to be unregistered.
     */
    virtual void UnregisterEvent(
            MSatEventObserver* aObserver,
            TSatEvent aEvent ) = 0;

    /**
     * Removes observer from event observers
     *
     * @since S60 v3.0
     * @param aObserver Observer to removed
     */
    virtual void UnregisterEventObserver(
            MSatEventObserver* aObserver ) = 0;

    /**
     * Gives number of executing commands currently.
     * In practise there can be only one executing proactive
     * command, but envelopes can be executing same time.
     *
     * @since S60 v3.0
     * @return Number of executing command handlers.
     */
    virtual TInt NumberOfExecutingCommandHandlers() = 0;

    /**
     * Gets the sim application name.
     *
     * @since S60 v3.0
     * @return Sat application name.
     */
    virtual const TDesC& SatAppName() const = 0;

    /**
     * Gets the name for the APN
     *
     * @since S60 v3.0
     * @return Sat application name.
     */
    virtual const TDesC& BipApnName() const = 0;
    
    /**
     * Restores default SIM Application name.
     *
     * @since S60 v3.1
     */
    virtual void RestoreSatAppNameL() = 0;

    /**
     * Sets the sim application name.
     *
     * @since S60 v3.0
     * @param aName New name of Sat application
     */
    virtual void SetSatAppNameL( const TDesC& aName ) = 0;

    /**
     * Gets all refresh subsession.
     * @return Pointer array of Refresh Sub sessions.
     */
    virtual const RPointerArray<MSatRefreshSubSession>&
            RefreshSubSessions() = 0;

    /**
     * Registers new service request handler
     *
     * @since S60 v3.0
     * @param aRequest A Request this service request listens
     * @param aResponse A Response for this service request
     * @param aCommand A Command handler for request
     */
    virtual void RegisterServiceRequestL(
            TSatServerRequest aRequest,
            TSatServerRequest aResponse,
            MSatCommand* aCommand ) = 0;

    /**
     * Gets all registered service requests
     *
     * @since S60 v3.0
     * @return Pointer array of registered Client Service requests
     */
    virtual CArrayPtrFlat<CSatClientServiceReq>*
            ServiceRequests() = 0;

    /**
     * Gives pointer to event monitor container
     *
     * @since S60 v3.0
     * @return Pointer to MSatEventMonitorContainer API
     */
    virtual MSatEventMonitorContainer* EventMonitors() = 0;

    /**
     * Gives reference to subscriber id.
     *
     * @since S60 v3.0
     * @return SIM subscriber id API reference
     */
    virtual MSatSSimSubscriberId& SimSubscriberId() = 0;

    /**
     * Gives reference to BIP Utils interface.
     *
     * @since S60 v3.0
     * @return Reference to BIP Utils interface.
     */
    virtual MSatBIPUtils& BipUtils() = 0;

    /**
     * Gives reference to BIP Event notifier interface.
     *
     * @since S60 v3.0
     * @return Reference to BIP Event notifier interface.
     */
    virtual MSatBIPEventNotifier& BipEventNotifier() = 0;

    /**
     * Gives reference to active RMmCustomApi session
     *
     * @since S60 v3.1
     * @return Reference to active RMmCustomApi server session
     */
    virtual RMmCustomAPI* CustomApi() = 0;
    

    /**
     * Gives reference to MSatMultiModeApi
     *
     * @since S60 v5.0
     * @return Reference to MSatMultiModeApi
     */
    virtual MSatMultiModeApi& MultiModeApi() = 0;

    /**
     * Sends SAT Mediator event to Mediator Event Provider interface.
     *
     * @since S60 v3.1
     * @param aData packaged data to mediator event consumer
     * @return KErrNone or error code.
     */
    virtual TInt RaiseSatEvent( const TDesC8& aData ) = 0;

    /**
     * Checks Cover UI support for SAT Mediator Event.
     *
     * @since S60 v3.1
     * @return ETrue if supported.
     */
    virtual TBool CoverUiSupported() = 0;

    /**
     * Gives pointer to MSatAsyncToSync interface.
     *
     * @since S60 v5.0.1
     * @return a MSatAsyncToSync Pointer to CreateAsyncToSyncHelper object.
     */
    virtual MSatAsyncToSync* CreateAsyncToSyncHelper() = 0;

protected:

    MSatUtils() {};

    virtual ~MSatUtils() {};

private:

    /**
     * Prohibit copy constructor if not deriving from CBase.
     *
     * @since S60 v3.0
     * @param const reference to MSatUtils.
     */
    MSatUtils( const MSatUtils& );

    /**
     * Prohibit assigment operator if not deriving from CBase.
     *
     * @since S60 v3.0
     * @param const reference to MSatUtils.
     */
    MSatUtils& operator=( const MSatUtils& );

    };

#endif // M_MSATUTILS_H

