/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles threads / plugins 
*
*/


#ifndef CCCEPLUGINMANAGER_H
#define CCCEPLUGINMANAGER_H

#include <e32base.h>
#include <e32property.h> // P&S support

#include "mccpplugindeathobserver.h" // MCCPPluginDeathObserver
#include "mccespsobserver.h" // MCCESPSObserver

class CConvergedCallProvider;
class MCCPObserver;
class CCCEPlugin;
class CCCECallContainer;
class MCCEObserver;
class MCCPCallObserver;
class MCCPEmergencyCall;
class MCCPDTMFProvider;
class MCCPDTMFObserver;
class CCCETransferController;
class CCCESPSettingsHandler;

/**
 *  CCE Plugin manager
 *
 *  Creates/Deletes and update current plugins.
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEPluginManager): public CActive,
    public MCCPPluginDeathObserver,
    public MCCESPSObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aCallContainer Reference to call container
     * @param aTransferController Reference to transfer controller
     */
    static CCCEPluginManager* NewL( 
        CCCECallContainer& aCallContainer,
        CCCETransferController& aTransferController );

    /**
     * Destructor.
     */
    virtual ~CCCEPluginManager();

public:

    /**
     * Sets observer
     *
     * @since S60 v3.2
     * @param aProviders 
     */
    void SetObserver( const MCCEObserver& aObserver );

    /**
     * TODO
     *
     * @since S60 v3.2
     * @param aProviders 
     */
    void SetEmergencyCallObserver(MCCPCallObserver& aEmergencyCallObserver);

    /**
     * Returns pointer to primary emergencycall
     *
     * @since S60 v3.2
     * @return Pointer to emergency call.
     */
    MCCPEmergencyCall* PrimaryEmergencyCall();
    
    /**
     * Returns Uid of primary emergencycall
     *
     * @since S60 v3.2
     * @return Implementation Uid.
     */
    TUid PrimaryEmergencyCallUid() const;
    
    /**
     * Returns plugins with emergency call attribute.
     *
     * @since S60 v3.2
     * @return Array of emergency call capable plugins.
     */
    RArray<TUid>& AlternativeEmergencyPlugins();

    /**
     * Returns plugin with desired service id
     *
     * @since S60 v3.2
     * @param aServiceId Used for searching
     * @return Pointer to plugin. NULL if not found.
     */
    CConvergedCallProvider* GetPluginL( TUint32 aServiceId );

   /**
    * Returns plugin with desired implementation id
    *
    * @since S60 v3.2
    * @param aType Used for searching
    * @return Pointer to plugin. NULL if not found.
    */
    CConvergedCallProvider* GetPluginL( const TUid& aType );
 
   /**
    * Remove unneeded plugin with desired implementation id
    *
    * @since S60 v3.2
    * @param aType Used for searching
    */
    void RemovePlugin( const TUid& aType );
   
   /**
    * Returns ETrue if plugin can be relesed if idle
    *
    * @since S60 v3.2
    * @return TBool EFalse If plugin is permanently in memory
    */
    TBool ReleaseWhenIdle( const TUid aType );
    
   /**
    * Returns dtmf interface for desired provider
    *
    * @since S60 v3.2
    * @param aObserver Used for setting interface observer
    * @param aImplementationUid Used for searching
    * @return Reference to dtmf provider or leave.
    */
    MCCPDTMFProvider& DtmfProviderL(
        const MCCPDTMFObserver& aObserver,
        TUid aImplementationUid ) const;

private:

// from base class MCCESPSObserver
    /** @see MCCESPSObserver */
    void ServiceEnabledL( TUid aImplementationUid );

    /** @see MCCESPSObserver */
    void ServiceDisabledL( TUid aImplementationUid );

// from base class MCCPPluginDeathObserver

    /** @see MCCPPluginDeathObserver::CCPPluginDiedEvent */
    void CCPPluginDiedEvent(TUid aPluginId, TInt aDeathType, TInt aReason);
    
    /** @see MCCPPluginDeathObserver::CCPPluginInitialisationFailed */
    void CCPPluginInitialisationFailed(TUid aPluginUid, TInt aError);
    
// from base class CActive

    /**
     * This will be runned when incoming call monitor activates
     *
     * @since S60 v3.2
     */
    void RunL();

    /**
     * Cancels incoming call monitor
     *
     * @since S60 v3.2
     */
    void DoCancel(); 

private:

    /**
     * This is called after phone application is constructed 
     * or else causes phone not to boot.
     *
     * @since S60 v3.2
     * @param aPluginManager 
     * @return KErrNone if successful
     *         ( KErrNone always or else there is loop possiple ).
     */
    static TInt DoAfterBoot( TAny* aPluginManager );
 
    /**
     * This is called when plugins are cheduled to be removed
     *
     * @since S60 v3.2
     * @param aPluginManager 
     * @return KErrNone if successful
     *         ( KErrNone always or else there is posiibility to loop
     */
    static TInt RemovePlugins( TAny* aPluginManager );
    

    /**
     * Logic for loading plugins at boot.
     *
     * @since S60 v3.2
     */
    void LoadBootPluginsL();
   
    /**
     * Logic for loading single plugin at boot
     * 
     * @since S60 v3.2
     * @param aServiceId Service id for loadable plugin
     */
    void LoadBootPluginL( TInt aServiceId );
    
    /**
     * Append to emergency call array if not already there.
     *
     * @since S60 v3.2
     */
    void AddToAlternativeEmergencyArray(TUid aUid);

private:

    /** Constructor */
    CCCEPluginManager( 
        CCCECallContainer& aCallContainer,
        CCCETransferController& aTransferController );

    /** Leaving stuff in constructor */
    void ConstructL();

private:
    
    /**
     * Reference to call container
     */
    CCCECallContainer& iCallContainer;
    
    /**
     * Reference to transfer controller
     */
    CCCETransferController& iTransferController;

    /**
     * This calls DoAfterBoot method
     * Own
     */
    CIdle* iIdle;

    /**
     * Array of plugins
     * Own
     */
    RPointerArray<CCCEPlugin> iPluginArray;
    
    /**
     * Array of plugins
     * Own
     */
    RPointerArray<CCCEPlugin> iPluginsToClose;

    /**
     * Observer
     * Own
     */
    const MCCEObserver* iObserver;
   
    /**
     * TODO
     * Own?
     */
    MCCPCallObserver* iEmergencyCallObserver;
    
    /**
     * TODO
     * Own?
     */
    CCCEPlugin* iPrimaryEmergencyCallPlugin;
    
    /**
     * Used in incoming call monitor
     */
    RProperty iProperty;
    
    /**
     * Array of plugins with emergency call attribute
     */
    RArray<TUid> iAlternativeEmergencyPlugins;
    
    /**
     * SPSettings listener
     * Own
     */
    CCCESPSettingsHandler* iSPSettings;
    };

#endif // CCCEPLUGINMANAGER_H
