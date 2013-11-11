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
* Description:    Simple Engine
*
*/




// INCLUDE FILES

#include <e32std.h>

// own simple
#include "simplecommon.h"
#include "simpleutils.h"


// ================= MEMBER FUNCTIONS =======================
//


EXPORT_C TPtrC8 TSimpleUtils::NsUriL(
        const TDesC8& aPrefix )
    {
    if ( !aPrefix.CompareF( KSimpleDefault ) )
        {
        return TPtrC8(KSimpleNsDefault);
        }
    else if ( !aPrefix.CompareF( KSimplePDM ) )
        {
        return TPtrC8(KSimpleNsPDM);
        }
    else if ( !aPrefix.CompareF( KSimpleRPID ) )
        {
        return TPtrC8(KSimpleNsRPID);
        }
    else if ( !aPrefix.CompareF( KSimpleOP ) )
        {
        return TPtrC8(KSimpleNsOP);
        }
    else if ( !aPrefix.CompareF( KSimpleCL ) )
        {
        return TPtrC8(KSimpleNsCL);
        }
    else if ( !aPrefix.CompareF( KSimpleGML ) )
        {
        return TPtrC8(KSimpleNsGML);
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    return TPtrC8();
    }

EXPORT_C TPtrC8 TSimpleUtils::NsPrefixL(
        const TDesC8& aURI )
    {
    if ( !aURI.CompareF( KSimpleNsDefault ) )
        {
        return TPtrC8(KSimpleDefault);
        }
    else if ( !aURI.CompareF( KSimpleNsPDM ) )
        {
        return TPtrC8(KSimplePDM);
        }
    else if ( !aURI.CompareF( KSimpleNsRPID ) )
        {
        return TPtrC8(KSimpleRPID);
        }
    else if ( !aURI.CompareF( KSimpleNsOP ) )
        {
        return TPtrC8(KSimpleOP);
        }
    else if ( !aURI.CompareF( KSimpleNsCL ) )
        {
        return TPtrC8(KSimpleCL);
        }
    else if ( !aURI.CompareF( KSimpleNsGML ) )
        {
        return TPtrC8(KSimpleGML);
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    return TPtrC8();
    }


