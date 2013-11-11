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
* Description:   Error and panic codes
*
*/



#ifndef __EPOS_SUPLNETWORKERRORS_H__
#define __EPOS_SUPLNETWORKERRORS_H__

#include <e32def.h>

_LIT(KSuplNWClientFault,"Supl Network Client Fault");

/**
 * SUPL Client API panic codes
 */
enum TSuplNWClientPanic
	{
	/** An attempt has been made to communicate through a non connected handle. */
	ESuplServerNWBadHandle					= 0,
	/** An attempt has been made to connect using an already open handle. */
	ESuplNWServerHandleNotClosed				= 3
	};

#endif //__EPOS_SUPLNETWORKERRORS_H__
