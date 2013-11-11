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
* Description:  Interface to send responses and events about call infos.
*
*/


#ifndef MCCECALLINFOMEDIATOR_H
#define MCCECALLINFOMEDIATOR_H

#include <e32base.h>

/**
 *  Interface to send call info responses and events
 *
 *  @lib CCE.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( MCCECallInfoMediator ) 
    {
public:

    /**
     * Sends response for mediator command. 
     *
     * @since S60 5.0
     * @param aCommandId Id of the command, which response this is.
     * @param aData Data to be sent.
     */
    virtual void SendResponse( TInt aCommandId, const TDesC8& aData ) = 0;
    
    /**
     * Raises mediator event.
     * 
     * @since S60 5.0
     * @param aEventId Id of the event
     * @param aData Data to be sent
     */
    virtual void RaiseEvent( TInt aEventId, const TDesC8& aData ) = 0;

    };


#endif // MCCECALLINFOMEDIATOR_H
