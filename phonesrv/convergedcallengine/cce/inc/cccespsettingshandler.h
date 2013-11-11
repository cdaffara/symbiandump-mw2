/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains service settings handling logic
*
*/


#ifndef CCCESPSETTINGSHANDLER_H_
#define CCCESPSETTINGSHANDLER_H_

#include <e32base.h>
#include <mspnotifychangeobserver.h>

class CCCEPluginManager;
class CSPNotifyChange;
class CSPSettings;
class MCCESPSObserver;

/**
 *  CCE Service provider settings handler
 *
 *
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCESPSettingsHandler) : public CBase, 
    public MSPNotifyChangeObserver
    {
public:
    /**
     * Two-phased constructor.
     */
    static CCCESPSettingsHandler* NewL( MCCESPSObserver& aObserver );

    /**
     * Destructors.
     */
    virtual ~CCCESPSettingsHandler();
    
public:
    /**
     * Gets leaves if not supported
     * 
     * @param aImplementationUid Implementation uid of plugin
     */
    void IsPluginSupportedL( TUid aImplementationUid );

    /**
     * Gets services available
     * 
     * @param aServices Array of available services
     */
    void GetServicesL( RIdArray& aServices );

    /**
     * Gets call provider implementation uid for desired service
     * 
     * @param aServiceId Service
     * @return Call provider implementation uid
     */
    TUid ImplementationUidL( TInt aServiceId );
    
    /**
     * Gets capablities for desired service
     * 
     * @param aServiceId Service
     * @return Service capablities
     */
    TPropertyServiceAttributes CapabilitiesL( TInt aServiceId );
    
    /**
     * Returns ETrue if VoIP atribute is enabled for desired service
     * 
     * @param aServiceId Service
     * @return ETrue if service is enabled
     */
    TBool IsEnabledL( TInt aServiceId );
    
private:
// from MSPNotifyChangeObserver

    /** @see MSPNotifyChangeObserver */
    void HandleNotifyChange( TServiceId aServiceId );

    /** @see MSPNotifyChangeObserver */
    void HandleError( TInt aError );

private:
    /**
     * Handles SPSettings change
     * 
     * @param aServiceId Changed service
     */
    void HandleServiceChangeL( TInt aServiceId );
    
    /**
     * Start listening SPSettings changes
     * 
     */
    void StartListeningChangesL();
    
    /**
     * Is Plugin on standby state. 
     * 
     * @param aImplementationUid Implementation uid of plugin
     * @return ETrue if plugin is on standby state 
     */
    TBool IsPluginOnStandbyL( TUid aImplementationUid );
    
    /**
     * Custom clean up for pointer array
     */
    static void CleanupPointerArray(  TAny* aPointer );
        

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * C++ default constructor.
     */
    CCCESPSettingsHandler( MCCESPSObserver& aObserver );
    
private:
    /**
     * Reference to observer
     */
    MCCESPSObserver& iObserver;

    /**
     * Notifies service provider setting change
     * Own
     */
    CSPNotifyChange* iNotifier;

    /**
     * Contains service provider settings
     * Own
     */
    CSPSettings*  iSettings;

    };

#endif /*CCCESPSETTINGSHANDLER_H_*/

// end of file
