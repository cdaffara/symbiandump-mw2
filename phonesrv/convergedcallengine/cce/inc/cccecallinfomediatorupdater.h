/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Updates call information to mediator clients.
*
*/


#ifndef CCCECALLINFOMEDIATORUPDATER_H
#define CCCECALLINFOMEDIATORUPDATER_H

#include <e32base.h>
#include <MediatorCommandResponder.h> 

class MCCECallInfoMediator;
class MCCECallArray;
class CCCECallInfoConverter;
class MCCECallInfo;
class CCallInfos;

/**
 *  Updates call information for mediator clients.
 *
 *  @lib cce.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CCCECallInfoMediatorUpdater ) : 
    public CBase, public MMediatorCommandObserver
    {
public:
    
    static CCCECallInfoMediatorUpdater* NewL( 
        MCCECallInfoMediator& aMediator,
        MCCECallArray& aCallArray,
        CCCECallInfoConverter* aConverter );
    
    static CCCECallInfoMediatorUpdater* NewLC( 
        MCCECallInfoMediator& aMediator,
        MCCECallArray& aCallArray,
        CCCECallInfoConverter* aConverter );

    /**
     * Destructor.
     */
    virtual ~CCCECallInfoMediatorUpdater();
    
    /**
     * Updates the current cce call information to mediator clietns.
     * 
     * @since S60 5.0
     */
    void UpdateL();

    /**
     * From base class MMediatorCommandObserver
     * @see MMediatorCommandObserver
     */
    void MediatorCommandL( TUid aDomain,
                           TUid aCategory, 
                           TInt aCommandId,
                           TVersion aVersion, 
                           const TDesC8& aData );

    /**
     * From base class MMediatorCommandObserver
     * @see MMediatorCommandObserver
     */
    void CancelMediatorCommand( TUid aDomain,
                                TUid aCategory, 
                                TInt aCommandId );

private:

    CCCECallInfoMediatorUpdater( 
        MCCECallInfoMediator& aMediator,
        MCCECallArray& aCallArray,
        CCCECallInfoConverter* aConverter );

    void ConstructL();
    
    HBufC8* GetSnapShotOfCallInfosL();
    
    void AddInfoL( MCCECallInfo* aInfo, CCallInfos& aInfos );

private: // data
    
    /**
     * To send response and events.
     */
    MCCECallInfoMediator& iMediator;
    
    /**
     * Access to calls.
     */
    MCCECallArray& iCallArray;
    
    /**
     * For converting cce call info to mediator call info.
     * Own.
     */
    CCCECallInfoConverter* iConverter;

    };

#endif // CCCECALLINFOMEDIATORUPDATER_H
