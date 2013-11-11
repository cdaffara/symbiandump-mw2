/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Error and panic codes
*
*/

/*! \file epos_suplterminalerrors.h
    \brief Constant values for SUPL Terminal Initiation API
*/
#ifndef __SUPLERRORS_H__
#define __SUPLERRORS_H__

#include <e32def.h>

_LIT(KSuplClientFault,"Supl Terminal Client Fault");

/**
 * SUPL Client API panic codes
 */
enum TSuplClientPanic
	{
	/** An attempt has been made to communicate though a non connected handle. */
	ESuplServerBadHandle					= 0,
	/** A client has tried to use a RSuplTerminalIPCSubSession/RSuplTerminalServer handle that hasn't
	    been properly initialised. */
	ESuplNullPointerHolder					= 1,
	/** A client has called an asynchronous request while it is still outstanding
	on that RSuplTerminalServer / RSuplTerminalIPCSubSession. */
	ESuplDuplicateRequest                   = 2,
	/** A session or sub-session was closed with requests still outstanding. */
	ESuplRequestsNotCancelled               = 3,
	/** A unknown IPC request has been made on the server. */
	ESuplBadRequest							= 4,
	/** A bad descriptor has been given to the server by the client. */
	ESuplBadDescriptor						= 5,
	/** An attempt has been made to connect using an already open handle. */
	ESuplServerHandleNotClosed				= 6
                
	};

/**
 * SUPL specific error codes.
 */
const TInt KSuplErrBase = -30371;

const TInt KErrSuplBufferOverflow = KSuplErrBase;		//-30371
const TInt KErrSuplIncalculable = KSuplErrBase - 1;		//-30372


#endif //__SUPLERRORS_H__
