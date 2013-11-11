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
* Description:  Handles dtmf 
*
*/


#ifndef CCCEDTMF_H
#define CCCEDTMF_H

#include <e32base.h>

#include "mccpdtmfobserver.h"   // MCCPDTMFObserver
#include "mccedtmfinterface.h"  // MCCEDtmfInterface

class CCCEPluginManager;
class CCCECallContainer;
class MCCPDTMFProvider;
/**
 *  CCE Dtmf
 *
 *  Contains dtmf logic
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEDtmf): public CBase,
                             public MCCPDTMFObserver,
                             public MCCEDtmfInterface
    {
public:

    /**
     * Two-phased constructor.
     * @param aCallContainer Reference to call container
     * @param aPluginManager Reference to plugin manager
     */
    static CCCEDtmf* NewL( 
        CCCECallContainer& aCallContainer,
        CCCEPluginManager& aPluginManager );

    /**
     * Destructor.
     */
    virtual ~CCCEDtmf();

public:

// from base class MCCEDtmfInterface

    /** @see MCCEDtmfInterface::SetObserver */
    void SetObserver( const MCCEDtmfObserver& aObserver );
    
    /** @see MCCEDtmfInterface::AddObserverL */
    void AddObserverL( const MCCEDtmfObserver& aObserver );    

private:

// from base class MCCEDtmfInterface

    /** @see MCCEDtmfInterface::StopDtmfTone */
    TInt StopDtmfTone();

    /** @see MCCEDtmfInterface::StartDtmfTone */
    TInt StartDtmfTone( const TChar aTone );
      
    /** @see MCCEDtmfInterface::SendDtmfToneString */
    TInt SendDtmfToneString( const TDesC& aTones );
      
    /** @see MCCEDtmfInterface::ContinueStringSending */
    TInt ContinueStringSending( const TBool aContinue );
     
    /** @see MCCEDtmfInterface::CancelDtmfStringSending */
    void CancelDtmfStringSending();
    
    /** @see MCCEDtmfInterface::RemoveObserver */
    TInt RemoveObserver( const MCCEDtmfObserver& aObserver );

// from base class MCCPDTMFObserver

    /** @see MCCPDTMFObserver::HandleDTMFEvent */
    void HandleDTMFEvent( const TCCPDtmfEvent aEvent,
                          const TInt aError,
                          const TChar aTone ) const;

private:

    /**
     * Some general logic that can be const`ed
     * Adds correct providers to array
     *
     * @since S60 v3.2
     * @param aProviders 
     * @return KErrNone if successful.
     */
    TInt GetDtmfProviders( RPointerArray<MCCPDTMFProvider>& aProviders ) const;

private:

    /** Constructor */
    CCCEDtmf(
        CCCECallContainer& aCallContainer,
        CCCEPluginManager& aPluginManager );

    /** Leaving stuff in constructor */
    void ConstructL();
                
private: // data

    /**
     * Reference to call container
     */
    const CCCECallContainer& iCallContainer;

    /**
     * Reference to plugin manager
     */
    const CCCEPluginManager& iPluginManager;

    /**
     * DTMF Observer array. 
     * Own
     */
    RArray<const MCCEDtmfObserver*> iObservers;
    };

#endif // CCCEDTMF_H