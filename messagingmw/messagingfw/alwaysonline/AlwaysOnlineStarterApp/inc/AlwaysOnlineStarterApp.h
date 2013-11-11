/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnline server starter
*
*/


#ifndef __ALWAYSONLINESTARTERAPP_H__
#define __ALWAYSONLINESTARTERAPP_H__


#include <e32base.h>   
#include <e32std.h>

#include <AlwaysOnlineManagerCommon.h>

// Client side thread starting function
// if the server is not already running
// when client tries to contact it.
IMPORT_C TInt StartThread();

// Function to start server.
IMPORT_C void RunServerL();





#endif
//EOF
