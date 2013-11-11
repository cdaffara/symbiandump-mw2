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
* Description:  
*     DLL internal functions.
*
*/




// INCLUDE FILES
#include <e32std.h>
#include "muiudomainpan.h"


// LOCAL CONSTANTS AND MACROS
_LIT(KPanicText, "MUIU_DOMAIN");


// ==================== LOCAL FUNCTIONS ====================

// ================= MEMBER FUNCTIONS =======================

GLDEF_C void Panic( TMuiuDomainPanic aPanic )
    {
    User::Panic( KPanicText, aPanic );
    }


// End of file
