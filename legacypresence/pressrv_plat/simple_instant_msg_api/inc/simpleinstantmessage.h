/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: IM API for Simple Engine
*
*/


#ifndef M_SIMPLEINSTANTMESSAGE_H
#define M_SIMPLEINSTANTMESSAGE_H


#include <e32std.h>


/**
 *  Class for sending instant messages.
 *
 *  @code
 *   MSimpleInstantMessage* im = 
 *      TSimpleFactory::NewInstantMessageL( conn, connObs );
 *   TInt result = im->SendInstantMessage( text8, recipientsPtr8 );
 *  @endcode
 *
 *  @lib simpleengine.lib
 *  @since S60 v5.1
 */
class MSimpleInstantMessage
    { 

public:

    /**
     * Send instant message.
     *
     * @since S60 v5.1
     * @param aMsgContent message content
     * @param aRecipient  recipient ID
     * @return Operation ID
     */
    virtual TInt SendInstantMessage( TPtrC8 aMsgContent, 
        TPtrC8 aRecipient ) = 0;

    /**
     * Destroy this entity.
     *
     * @since S60 v5.1
     */
    virtual void Close() = 0;
    };


#endif // M_SIMPLEINSTANTMESSAGE_H
