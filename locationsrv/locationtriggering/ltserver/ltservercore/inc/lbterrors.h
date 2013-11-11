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

#include <e32std.h>

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
    * An attempt to close Location Triggering Server session when not
    * all subsessions are closed.
    */
    ELbtServerSubsessionNotClosed = 2,
    /** 
    * An attempt has been made to communicate though a not connected handle. 
    */
    ELbtServerBadHandle = 3,
    /** 
    * A subsession was closed with requests still outstanding. 
    */
    ELbtRequestsNotCancelled = 4,
    /** 
    * An attempt to get trigger objects incrementally when iterator was not
    * created.
    */
    ELbtIteratorNotCreated = 5,
    /** 
    * A client has called an asynchronous request while it is still outstanding.
    */
    ELbtDuplicateRequest = 6,
    /** 
    * Wrong value is given as parameter.
    */
    ELbtErrArgument = 7,
    /** 
     * A bad descriptor has been given to the server by the client. 
     */
	ELbtBadDescriptor = 8	
    };

#endif //LBTERRORS_H
