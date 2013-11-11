/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Registers mediator command to get the initial call infos.
*
*/


#ifndef CCCECALLINFOMEDIATOR_H
#define CCCECALLINFOMEDIATOR_H

#include <e32base.h>

#include "mccecallinfomediator.h"

class CMediatorCommandResponder;
class CCCECallInfoMediatorUpdater;
class MCCECallArray;
class CMediatorEventProvider;

/**
 *  Registers mediator command to get the initial call infos.
 * 
 *  @lib CCE.lib
 *  @since S60 5.1
 */
NONSHARABLE_CLASS( CCCECallInfoMediator ) : 
    public CBase,
    public MCCECallInfoMediator
    {
public:

    static CCCECallInfoMediator* NewL( MCCECallArray& aCallArray );
    static CCCECallInfoMediator* NewLC( MCCECallArray& aCallArray );
    
    /**
    * Destructor.
    */
    virtual ~CCCECallInfoMediator();
    
    /**
     * Updates call information to mediator clients.
     * 
     * @since S60 5.0
     */
    void UpdateCallInfos();
    
    /**
     * From base class MCCECallInfoMediator.
     * @see MCCECallInfoMediator.
     */
    void SendResponse( TInt aCommandId, const TDesC8& aData );
    
    /**
     * From base class MCCECallInfoMediator
     * @see MCCECallInfoMediator.
     */
    void RaiseEvent( TInt aEventId, const TDesC8& aData );

private:

    CCCECallInfoMediator();

    void ConstructL( MCCECallArray& aCallArray );

private: // data
    
    /**
     * To register and respond commands.
     * Own.
     */
    CMediatorCommandResponder* iResponder;
    
    /**
     * To register and raise events.
     * Own.
     */
    CMediatorEventProvider* iEventProvider;
    
    /**
     * To update the call info to mediator clients.
     * Own.
     */
    CCCECallInfoMediatorUpdater* iUpdater;

    };

#endif // CCCECALLINFOMEDIATOR_H
