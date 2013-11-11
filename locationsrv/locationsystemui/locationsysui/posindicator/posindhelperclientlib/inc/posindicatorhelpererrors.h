/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Position Indicator Helper Client panic category and panic code
*
*/


#ifndef POS_IND_HELPER_ERRORS_H
#define POS_IND_HELPER_ERRORS_H

#include <e32def.h>
#include <e32cmn.h>

/**
 *Position Indicator Helper Client  API panic categories
 */
_LIT( KPosIndHelperClientPanicCategory, "PosIndicator" );

/**
 * Position Indicator Helper Client API panic codes
 */
enum TPosIndicatorHelperClientPanic
    {
    /** 
     * An attempt has been made to connect using an already opened handle. 
     */
    EPosIndicatorHelperServerHandleNotClosed = 1,
    /** 
     * An attempt to communicate using an unconnected handle.
     */
    EPosIndicatorHelperServerBadHandle = 2,
    /** 
     * A subsession was closed with requests still outstanding. 
     */
    EPosIndicatorHelperRequestsNotCancelled = 3,
    /** 
     * A client has called an asynchronous request while it is still outstanding.
     */
    EPosIndicatorHelperDuplicateRequest = 5,
    /** 
     * Wrong value is given as parameter.
     */
    EPosIndicatorHelperErrArgument = 6
    };
#endif //POS_IND_HELPER_ERRORS_H
