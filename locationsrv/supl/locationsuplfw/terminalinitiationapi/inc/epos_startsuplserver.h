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
* Description:   SUPL Server startup code
*
*/



#ifndef __STARTSUPLSERVER_H__
#define __STARTSUPLSERVER_H__

#include <e32std.h>

const TInt8 KSuplMajorVersionNumber = 1;
const TInt8 KSuplMinorVersionNumber = 0;
const TInt16 KSuplBuildVersionNumber = 0;

const TInt KDefaultMessageSlots = 16;

// The name of the server held in the kernel
_LIT(KSuplServerName, "SuplGateway");
_LIT(KSuplServerImg, "EPosSuplGateway");

// The third UID of the server DLL
const TUid KSuplServerUid3 = { 0x102073CA };


#endif // __STARTSUPLSERVER_H__
