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
* Description:  Other useful methods
*
*/


#ifndef CCCEUTILITY_H
#define CCCEUTILITY_H


#include <e32base.h>

enum TCCEPanic
    {
    ECCEPanicTooManyCalls,
    ECCEPanicInvalidCallId,
    ECCEPanicInvalidCallType,
    ECCEPanicInvalidState,
    ECCEPanicInvalidRequestToCancel,
    ECCEPanicNoRequestActive,
    ECCEPanicRequestAlreadyActive,
    ECCEPanicTwoRequests,
    ECCEPanicNullPointer,
    ECCEPanicBadHandle,
    ECCEPanicSwapNoCallOnHold,
    ECCEPanicInvalidIncomingSession,
    ECCEPanicInvalidCallDirection,
    ECCEPanicInvalidError
    };

/**
 *  Useful static methods
 *
 *  
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEUtility)
    {
public:

    /**
     * Generates cce specific errors
     *
     * @since S60 3.2
     * @param aPanic Panic code
     */
    static void CCCEUtility::Panic( const TCCEPanic aPanic );

    };

#endif // CCCEUTILITY_H
