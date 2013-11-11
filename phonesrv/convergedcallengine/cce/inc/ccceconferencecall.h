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
* Description:  Implementation for call object
*
*/


#ifndef CCCECONFERENCECALL_H
#define CCCECONFERENCECALL_H

#include <e32base.h>
#include <cccpcallparameters.h>

#include "mcceconferencecall.h"         // MCCEConferenceCall
#include "mccpconferencecallobserver.h" // MCCPConferenceCallObserver

class CCCECall;
class MCCPConferenceCall;
class CCCECallContainer;
class CCCEPluginManager;
class CCCEDurationTimer;

/**
 *  Internal conference call object
 *
 *  This object contains logic for conference call
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEConferenceCall) : public CBase,
                                        public MCCEConferenceCall,
                                        public MCCPConferenceCallObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aCallContainer Reference to call container
     * @param aPluginManager Reference to plugin manager
     */
    static CCCEConferenceCall* NewL( 
        CCCECallContainer& aCallContainer,
        CCCEPluginManager& aPluginManager );

    /**
     * Destructor.
     */
    virtual ~CCCEConferenceCall();

public:
    /**
     * Returns state of conferencecall
     *
     * @since S60 v3.2
     * @return State
     */
    MCCEConferenceCallObserver::TCCEConferenceCallState State() const;

    /**
     * Return implementation uid of conferencecall
     *
     * @since S60 v3.2
     * @return Implementation uid
     */
    TUid ImplementationUid() const;

    /**
     * Checks if referenced call is part of conference.
     *
     * @since S60 v3.2
     * @param Reference to call to be checked
     * @return ETrue if aCall is member of conference
     */
    TBool IsPartOfConference( CCCECall& aCall );

    /**
     * Sets plugin originated conference call 
     * 
     * @since S60 v3.2
     * @param Reference to ongoig conference call
     * @param ImplementationUid of plug-in which created
     *     this conference call
     * @return Error code
     */
    TInt SetOngoingConferenceCall( 
        MCCPConferenceCall& aConferenceCall,
        TUid aImplementationUid );
    
// from base class MCCEConferenceCall

    /** @see MCCEConferenceCall::EnumerateCalls */
    TInt EnumerateCalls();

    /** @see MCCEConferenceCall::Swap */
    TInt Swap();

    /** @see MCCEConferenceCall::SetObserver */
    void SetObserver( const MCCEConferenceCallObserver& aObserver );

    /** @see MCCEConferenceCall::HangUp */
    TInt HangUp();

    /** @see MCCECall::CallDuration */
    TTimeIntervalSeconds CallDuration() const;

   /** @see MCCECall::Parameters */
    const CCCPCallParameters& Parameters() const;
    

private:

// from base class MCCEConferenceCall

    /** @see MCCEConferenceCall::Release */
    TInt Release();

    /** @see MCCEConferenceCall::ServiceId */
    TUint32 ServiceId() const;

    /** @see MCCEConferenceCall::AddCallL */
    void AddCallL( MCCECall& aCall );

    /** @see MCCEConferenceCall::RemoveCallL */
    void RemoveCallL( MCCECall& aCall );

    /** @see MCCEConferenceCall::Caps */
    MCCEConferenceCallObserver::TCCEConferenceCallCaps Caps() const;

    /** @see MCCEConferenceCall::GoOneToOneL */
    void GoOneToOneL( MCCECall& aCall );
     
    /** @see MCCEConferenceCall::GetCallArray */
    TInt GetCallArray( RPointerArray<MCCECall>& aCallArray );
private:

// from MCCPConferenceCallObserver

    /** @see MCCPConferenceCallObserver::ErrorOccurred */
    void ErrorOccurred( const TCCPConferenceCallError aError );

    /** @see MCCPConferenceCallObserver::ConferenceCallCapsChanged */
    void ConferenceCallCapsChanged( 
        const TCCPConferenceCallCaps aCaps );

    /** @see MCCPConferenceCallObserver::ConferenceCallStateChanged */
    void ConferenceCallStateChanged( 
        const TCCPConferenceCallState aState );
        
    /** @see MCCPConferenceCallObserver::ConferenceCallEventOccurred */
    void ConferenceCallEventOccurred( 
         const TCCPConferenceCallEvent aEvent,
         MCCPCall* aReferredCall=NULL );
         
private:

    /**
     * DeActivate conferencecall
     *
     * @since S60 v3.2
     */
    void DeActivate();
    
    /**
     * Change caps and if they are changed inform to observer
     *
     * @since S60 v3.2
     * @param aCaps New caps
     */
    void CapsChanged( TInt aCaps );

    /**
     * Resets conference call to original state
     *
     * @since S60 v3.2
     */
    void Reset();

private:

    /** Constructor */
    CCCEConferenceCall(
        CCCECallContainer& aCallContainer,
        CCCEPluginManager& aPluginManager );

    /** Leaving stuff in constructor */
    void ConstructL();
    
private: // data

    /**
     * Reference to pluginmanager
     */
    CCCEPluginManager& iPluginManager;

    /**
     * Reference to callcontainer
     */
    CCCECallContainer& iCallContainer;

    /**
     * Converged conference call currently in use 
     * Own
     */
    MCCPConferenceCall* iConferenceCall;

    /**
     * Observer
     */
    MCCEConferenceCallObserver* iObserver;

    /**
     * Contains call that are currently in conference
     * Not Own
     */    
    RPointerArray<CCCECall> iCallArray;
    
    /**
     * Conferencecall caps
     */
    TInt iConferenceCaps;
    
    /**
     * Conferencecall state
     */
    MCCEConferenceCallObserver::TCCEConferenceCallState iState;
    
    /**
     * Implementation uid of plugin used
     */
    TUid iImplementationUid;

    /**
     * Counts duration for this call
     * Own
     */
    CCCEDurationTimer* iCCEDurationTimer;
    
    /**
     * Callparameters
     */
    CCCPCallParameters* iCallParameters;
    };

#endif // CCCECALL_H
