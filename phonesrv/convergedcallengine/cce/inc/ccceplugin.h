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
* Description:  Handles plugins 
*
*/


#ifndef CCCEPLUGIN_H
#define CCCEPLUGIN_H

#include <e32base.h>

#include "mccpcsobserver.h"  // MCCPCSObserver
#include "mccpssobserver.h" 

class CConvergedCallProvider;
class MCCPDTMFProvider;
class MCCPDTMFObserver;
class MCCPPluginDeathObserver;
class CCCPHandle;
class CCCECallContainer;
class MCCPEmergencyCall;
class MCCEObserver;
class MCCPCallObserver;
class CCCETransferController;
class MCCPConferenceCall;

/**
 *  CCE Plugin
 *
 *
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEPlugin): public CBase,
                               public MCCPCSObserver,
                               public MCCPSsObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aPluginUID Plugin uid for this plugin
     * @param aObserver Reference to observer
     * @param aCallContainer Reference to call container
     * @param aDeathObserver Reference to death observer
     * @param iTransferController Reference to transfer controller
     */
    static CCCEPlugin* NewL( 
        const TUid& aPluginUID,
        MCCEObserver& aObserver,
        CCCECallContainer& aCallContainer,
        MCCPPluginDeathObserver& aDeathObserver,
        CCCETransferController& iTransferController );

    /**
     * Destructor.
     */
    virtual ~CCCEPlugin();

public:

    /**
     * Returns interface for this plugin.
     *
     * @since S60 v3.2
     * @return Pointer to this plugin.
     */
    CConvergedCallProvider* GetPluginL() const;

    /**
     * Initializes emergency call for this plugin.
     *
     * @since S60 v3.2
     * @param aEmergencyCallObserver Reference to used observer.
     */
    void InitialiseEmergencyCallL( MCCPCallObserver& aEmergencyCallObserver );

    /**
     * Gets interface for emergency call.
     *
     * @since S60 v3.2
     * @return Pointer to emergencycall interface.
     */
    MCCPEmergencyCall* GetEmergencyCall() const;

    /**
     * Gets interface for dtmf handling
     *
     * @since S60 v3.2
     * @param aObserver Reference to used observer.
     * @return Reference to providers dtmf interface
     */
    MCCPDTMFProvider& GetDtmfProviderL(
        const MCCPDTMFObserver& aObserver );

    /**
     * Returns plugin type
     *
     * @since S60 v3.2
     * @return Uid for this plugin.
     */
    TUid Type() const;
    
   /**
    * Returns ETrue if plugin can be relesed if idle
    *
    * @since S60 v3.2
    * @return TBool EFalse If plugin is permanently in memory
    */
    TBool ReleaseWhenIdle();
   
   /**
    * Returns ETrue if plugin can be relesed if idle
    *
    * @since S60 v3.2
    * @param aReleaseWhenIdle Set EFalse if plugin is permanently in memory
    */
    void SetReleaseWhenIdle(TBool aReleaseWhenIdle);
    
private:
    /**
     * Checks if call is allowed
     *
     * @since S60 v3.2
     * @return Count of non conference calls
     * @return Error if call is not allowed
     */
    TInt IsCallAllowed() const;
    
    /**
     * Checks if data call can be allowed 
     *
     * @since S60 v3.2
     * @return ETrue if data call can be allowed. 
     */
    TBool IsDataCallAllowed() const;

// from base class MCCPCSObserver

    /** @see MCCPCSObserver::PluginInitiatedCSMoCallCreated */
    void MoCallCreated( MCCPCall& aCall );

    /** @see MCCPCSObserver::DataPortName */
    void DataPortName( TName& aPortName );

    /** @see MCCPCSObserver::ConferenceCallCreated */
    void ConferenceCallCreated( MCCPConferenceCall& aConferenceCall ); 
    
// from base class MCCPObserver

    /** @see MCCPObserver::ErrorOccurred */
    void ErrorOccurred( TCCPError aError );

    /** @see MCCPObserver::IncomingCall */
    void IncomingCall( MCCPCall* aCall, MCCPCall& aTempCall );

    /** @see MCCPObserver::IncomingCall */
    void IncomingCall( MCCPCall* aCall );

    /** @see MCCPObserver::CallCreated */
    void CallCreated( MCCPCall* aCall, MCCPCall* aOriginator, TBool aAttented );

// from base class MCCPObserver
    
    /** @see MCCPSsObserver::BarringEventOccurred */
    void BarringEventOccurred( const MCCPSsObserver::TCCPSsBarringEvent aBarringEvent );
                                  
     /** @see MCCPSsObserver::CLIEventOccurred */
    void CLIEventOccurred( const MCCPSsObserver::TCCPSsCLIEvent aCallLineEvent );
                                  
     /** @see MCCPSsObserver::CallForwardEventOccurred */
    void CallForwardEventOccurred( const MCCPSsObserver::TCCPSsCallForwardEvent aCallForwardEvent,
        const TDesC& aRemoteAddress );

    /** @see MCCPSsObserver::CallCugEventOccurred */
    void CallCugEventOccurred( const MCCPSsObserver::TCCPSsCugEvent aCugEvent );
    
    /** @see MCCPSsObserver::NotifyCurrentActiveALSLine */
    void NotifyCurrentActiveALSLine( TInt aLine );
private:

    /** Constructor */
    CCCEPlugin( 
        const TUid& aPluginUID,
        MCCEObserver& aObserver,
        CCCECallContainer& aCallContainer,
        MCCPPluginDeathObserver& aDeathObserver,
        CCCETransferController& iTransferController );

    /** Leaving stuff in constructor */
    void ConstructL();
    
    /**
     * Tells if cs plugin is used in current call
     *
     * @since S60 v3.2
     * @param aCall Call to be checked
     * @return ETrue if cs plugin used
     */
    TBool IsCsPlugin( MCCPCall& aCall );

private: // data

    /**
     * Reference to observer
     */
    MCCEObserver& iObserver;
    
    /**
     * Reference to call container
     */
    CCCECallContainer& iCallContainer;
    
    /**
     * Reference to trasfer controller
     */
    CCCETransferController& iTransferController;

    /**
     * Reference to dead observer
     * This will be informed when plugin crashes
     */
    MCCPPluginDeathObserver& iDeathObserver;

    /**
     * Use handle to ecom plugin.
     * Own
     */
    CConvergedCallProvider* iPlugin;
    
    /**
     * Dtmf interface for this plugin
     * Own
     */
    MCCPDTMFProvider* iDtmfProvider;
    
    /**
     * Emergency call interface for this plugin
     * Own
     */
    MCCPEmergencyCall*  iEmergencyCall;
    
    /**
     * Plugin uid for this plugin
     */
    const TUid iPluginId;
    
    /**
     * This plugin shoild not be Relased even no active calls exists
     */
    TBool iReleaseWhenIdle;
    };

#endif // CCCEPLUGIN_H