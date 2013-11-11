/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Location Triggering panic category and panic code
*
*/


#ifndef LBTERRORS_H
#define LBTERRORS_H

#include <e32def.h>
#include <e32cmn.h>

/**
 * Location Triggering Client API panic categories
 */
_LIT( KLbtClientPanicCategory, "LocTriggering" );

/**
 * Location Triggering Client API panic codes
 */
enum TLbtClientPanic
    {
    /** 
     * An attempt has been made to connect using an already opened handle. 
     */
    ELbtServerHandleNotClosed = 1,
    /** 
     * An attempt to communicate using an unconnected handle.
     */
    ELbtServerBadHandle = 2,
    /** 
     * A subsession was closed with requests still outstanding. 
     */
    ELbtRequestsNotCancelled = 3,
    /** 
     * An attempt to get trigger objects incrementally when iterator was not
     * created.
     */
    ELbtIteratorNotCreated = 4,
    /** 
     * A client has called an asynchronous request while it is still outstanding.
     */
    ELbtDuplicateRequest = 5,
    /** 
     * Wrong value is given as parameter.
     */
    ELbtErrArgument = 6
    };


#endif //LBTERRORS_H
