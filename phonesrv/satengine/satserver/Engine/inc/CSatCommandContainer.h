/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains SAT commands
*
*/



#ifndef C_CSATCOMMANDCONTAINER_H
#define C_CSATCOMMANDCONTAINER_H


#include <e32base.h>
#include "MSatUtils.h"
#include "TSatEventMediator.h"
#include "MSatEventObserver.h"
#include "MSatSystemStateObserver.h"
#include "CSatCommandHandler.h"


class TUSatAPI;
class MSatSSessions;
class MSatShellController;
class MSatSystemState;
class MSatSUiClientHandler;
class CSatEventMonitorContainer;
class MSatSSimSubscriberId;
class CSatSAPChangeObserver;
class CSatBIPUtils;
class CSatMediatorEventProvider;
class CSatProfileChangeObserver;
class CSatMultiModeApi;
class MSatSystemStateChangeNotifier;

const TInt KSatMaxBipApnName = 50;


/**
*  Contains SAT commands and offers accessor API for
*  command handlers.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class CSatCommandContainer : public CBase,
                             public MSatUtils,
                             public MSatEventObserver,
                             public MSatSystemStateObserver
    {
public:  // Constructors and destructor

   /**
    * Two-phased constructor.
    * @param aSessions Accessor for sessions
    * @param aEventMediator Event handler class
    * @param aSatUiHandler UI Client handler class
    */
    static CSatCommandContainer* NewL(
        MSatSSessions& aSessions,
        TSatEventMediator& aEventMediator,
        MSatSUiClientHandler& aSatUiHandler );

    virtual ~CSatCommandContainer();

    /**
     * Starts command handlers.
     *
     * @since S60 v3.0
     */
    void StartCommandHandlersL();

    /**
     * Stops command handlers.
     *
     * @since S60 v3.0
     */
    void StopCommandHandlers();

// from MSatEventObserver

    /**
     * From MSatEventObserver.
     * Event notification.
     *
     * @since S60 v3.0
     * @param  aEvent An event that has been notified by TSatEventMediator
     */
    void Event( TInt aEvent );

    /**
     * From MSatEventObserver.
     * Sends UI Event Notification if UI session has found
     *
     * @since S60 v3.1
     * @param aEvenId, identifies the event
     * @param aEventStatus, status of the event
     * @param aEventError, possible error value
     */
    void NotifyUiEvent(
            TInt aEventId,
            TInt aEventStatus,
            TInt aEventError );

// from MSatUtils
    /**
     * From MSatUtils.
     * Gives USAT API wrapper.
     *
     * @since S60 v3.0
     * @return USAT API wrapper reference
     */
    MSatApi& USatAPI();

    /**
     * From MSatUtils.
     * Gives reference to system state.
     *
     * @since S60 v3.0
     * @return SAT System state API reference
     */
    MSatSystemState& SystemState();

    /**
     * From MSatUtils.
     * Gives pointer to MSatSUiClientHandler
     *
     * @since S60 v3.0
     * @return SAT UI handler API reference
     */
    MSatSUiClientHandler& SatUiHandler();

    /**
     * From MSatUtils.
     * Registers observer for event. Same observer will not
     * be added more than once for one event.
     *
     * @since S60 v3.0
     * @param aObserver Pointer of observer instance. Panic will occur if
     * aObserver is NULL.
     * @param aEvent Event, which will cause the notification of observer.
     */
    void RegisterL(
            MSatEventObserver* aObserver,
            TSatEvent aEvent );

    /**
     * From MSatUtils.
     * Notifies all the listeners who are listening for aEvent.
     *
     * @since S60 v3.0
     * @param aEvent Event to be notified.
     */
    void NotifyEvent( TSatEvent aEvent );

    /**
     * From MSatUtils.
     * Unregisteres the event notification.
     *
     * @since S60 v3.0
     * @param aObserver An event observer that is unregistering event
     * @param aEvent An event that is unregistered from aObserver parameter
     */
    void UnregisterEvent(
            MSatEventObserver* aObserver,
            TSatEvent aEvent );

    /**
     * From MSatUtils.
     * Removes observer from event observers
     *
     * @since S60 v3.0
     * @param aObserver Observer to removed
     */
    void UnregisterEventObserver( MSatEventObserver* aObserver );

    /**
     * From MSatUtils.
     * Number of executing commands.
     *
     * @since S60 v3.0
     * @return Number of currently executing command handlers
     */
    TInt NumberOfExecutingCommandHandlers();

    /**
     * From MSatUtils.
     * Gets the sim application name.
     *
     * @since S60 v3.0
     * @return SAT Application name
     */
    const TDesC& SatAppName() const;

    /**
     * From MSatUtils.
     * Gets the name for the APN.
     *
     * @since S60 v3.0
     * @return BIP Apn name
     */
    const TDesC& BipApnName() const;
    
    /**
     * From MSatUtils.
     * Restores default SIM Application name.
     *
     * @since S60 v3.1
     */
    void RestoreSatAppNameL();

    /**
     * From MSatUtils.
     * Sets the sim application name.
     *
     * @since S60 v3.0
     * @param aName New name of Sat application
     */
    void SetSatAppNameL( const TDesC& aName );

    /**
     * From MSatUtils
     * Gets an array of refresh subsessions
     *
     * @since S60 v3.0
     * @return Pointer array containing refresh subsessions
     */
    const RPointerArray<MSatRefreshSubSession>& RefreshSubSessions();

    /**
     * From MSatUtils.
     * Registers new service request handler
     *
     * @since S60 v3.0
     * @param aRequest A Request this service request listens
     * @param aResponse A Response for this service request
     * @param aCommand A Command handler for request
     */
    void RegisterServiceRequestL(
            TSatServerRequest aRequest,
            TSatServerRequest aResponse,
            MSatCommand* aCommand );

    /**
     * From MSatUtils Gets all registered service requests
     *
     * @since S60 v3.0
     * @return Pointer array of registered Client Service requests
     */
    CArrayPtrFlat<CSatClientServiceReq>* ServiceRequests();

    /**
     * From MSatUtils.
     * Gives pointer to event monitor container
     *
     * @since S60 v3.0
     * @return Pointer to MSatEventMonitorContainer API
     */
    MSatEventMonitorContainer* EventMonitors();

    /**
     * From MSatUtils.
     * Gives reference to subscriber id.
     *
     * @since S60 v3.0
     * @return SIM subscriber id API reference.
     */
    MSatSSimSubscriberId& SimSubscriberId();

    /**
     * From MSatUtils.
     * Gives reference to BIP Utils interface.
     *
     * @since S60 v3.0
     * @return Reference to BIP Utils interface.
     */
    MSatBIPUtils& BipUtils();

    /**
     * From MSatUtils.
     * Gives reference to BIP Event notifier interface.
     *
     * @since S60 v3.0
     * @return Reference to BIP Event notifier interface.
     */
    MSatBIPEventNotifier& BipEventNotifier();

    /**
     * Gives reference to MSatMultiModeApi
     *
     * @since S60 v5.0
     * @return Reference to MSatMultiModeApi
     */
    MSatMultiModeApi& MultiModeApi() ;

    /**
     * From MSatUtils.
     * Gives reference to active RMmCustomApi session
     *
     * @since S60 v3.1
     * @return Reference to active RMmCustomApi server session
     */
    RMmCustomAPI* CustomApi();

    /**
     * From MSatUtils.
     * Raises SAT Mediator event
     *
     * @since S60 v3.1
     * @param aData event data to Mediator Event consumer.
     * @return KErrNone or error code.
     */
    TInt RaiseSatEvent( const TDesC8& aData );

    /**
     * From MSatUtils.
     * Checks if Cover UI is supported by SAT
     *
     * @since S60 v3.1
     * @return ETrue if Cover UI is supported.
     */
    TBool CoverUiSupported();

    /**
     * From MSatUtils.
     * Gives pointer to MSatAsyncToSync interface.
     *
     * @since S60 v5.0.1
     * @return a MSatAsyncToSync Pointer to CreateAsyncToSyncHelper object.
     */
    MSatAsyncToSync* CreateAsyncToSyncHelper();

// from MSatSystemStateObserver

    /**
     * Notifies that system state changed.
     * @param aValue The new value that has changed.
     *
     * @since S60 v3.1
     */
    void StateChanged( const TInt aValue );

private:

    /**
     * Private Constructor
     *
     * @since S60 v3.0
     * @param aSessions Accessor for sessions
     * @param aEventMediator Event handler class
     * @param aSatUiHandler UI Client handler class
     */
    CSatCommandContainer( MSatSSessions& aSessions,
            TSatEventMediator& aEventMediator,
            MSatSUiClientHandler& aSatUiHandler );

    void ConstructL();

    /**
     * Reads sat application name from the resource.
     *
     * @since S60 v3.0
     * @param aResourceId Selects name of the item to be read.
     */
    void CreateSatAppNameL( const TDesC& aResourceId );

    /**
     * Gives pointer to BIP Utils class.
     *
     * @since S60 v3.0
     * @return Pointer to BIP Utils class.
     */
    CSatBIPUtils* CreateAndGetBIPUtils();

    /**
     * Gives pointer to SatMediatorEventProvider class.
     *
     * @since S60 v3.1
     * @return Pointer to SatMediatorEventProvider class.
     */
    CSatMediatorEventProvider* CreateAndGetSatMediatorEventProvider();

    /**
     * Starts command handlers.
     *
     * @since S60 v3.1
     */
    void StartImportantCommandHandlersL();

    /**
     * Checks current startup state and starts rest of command handlers.
     * @param aValue Current startup state to check.
     *
     * @since S60 v3.1
     */    
    void CheckStartupState( const TInt aValue );

private: // Data

    /**
     * Default name of the SAT Application, used when creating new APN
     */
    TBuf<KSatMaxBipApnName>  iSatBipName;

    /**
     * Indicates special SIM to be currently used,
     */
    TBool iIsCmccSim;

    
    /**
     * Access the encapsuled ETel MM api by CSatMultiModeApi
     * Own
     */
    CSatMultiModeApi* iMultiModeApi;

    /**
     * Accessor for sessions.
     */
    MSatSSessions& iSessions;

    /**
     * Event mediator.
     */
    TSatEventMediator& iEventMediator;

    /**
     * UI Client handler API reference.
     */
    MSatSUiClientHandler& iSatUiHandler;

    /**
     * USat api.
     * Own.
     */
    TUSatAPI* iUSatAPI;

    /**
     * Command handlers.
     * Own.
     */
    CArrayPtrFlat<CSatCommandHandler>* iCmdHandlers;

    /**
     * Registered Client request handlers.
     * Own.
     */
    CArrayPtrFlat<CSatClientServiceReq>* iServiceReqs;

    /**
     * Name of the Sat application.
     * Own.
     */
    HBufC* iSatAppName;

    /**
     * System state interface.
     * Own.
     */
    MSatSystemState* iSystemState;

    /**
     * Event monitor container interface
     * Own.
     */
    CSatEventMonitorContainer* iEventMonitors;

    /**
     * SAP change observer / notifier
     * Own.
     */
    CSatSAPChangeObserver* iSapObserver;

    /**
     * Subscriber id interface.
     * Own.
     */
    MSatSSimSubscriberId* iSimSubscriberId;

    /**
     * BIP Utils class
     * Own.
     */
    CSatBIPUtils* iBipUtils;

    /**
     * pointer to SAT Mediator event provider
     * Own.
     */
    CSatMediatorEventProvider* iSatMediatorEvent;

    /**
     * Pointer to Profile state observer
     * Own.
     */
    CSatProfileChangeObserver* iProfileObserver;

    /**
     * Boolean indicating is Startup ongoing
     */
    TBool iStartupPhase;

    /**
     * Pointer to startup change observer
     */
    MSatSystemStateChangeNotifier* iStartupChangeObserver;

    };

#endif // C_CSATCOMMANDCONTAINER_H
