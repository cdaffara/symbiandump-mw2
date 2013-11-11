/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   DLL entry point
*
*/




#include <e32std.h>

#include "epos_omasuplprovhandler.pan"


GLDEF_C TInt E32Dll(/*TDllReason aReason*/)
    {
    return KErrNone;
    }


/**
 * Panic handler
 * @param aPanic reason for panic.
 */
GLDEF_C void Panic( TInt aPanic )
    {
    User::Panic( _L("eposomasuplprovisioninghandler.dll"), aPanic );
    }
