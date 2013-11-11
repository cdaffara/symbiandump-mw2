/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  common utilities*
*/

#include <e32std.h>

#include "mdccommon.h"
#include "mdccommon.pan"

// ---------------------------------------------------------------------------
// Panic
// ---------------------------------------------------------------------------
//
EXPORT_C void MMdCCommon::Panic( TInt aPanicCode )
    {
    User::Panic( KMetadataEngine, aPanicCode );
    }

// ---------------------------------------------------------------------------
// E32Dll
// ---------------------------------------------------------------------------
//
TBool E32Dll()
    {
    return ETrue;
    }
	
