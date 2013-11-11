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
* Description:    Simple Engine utils
*
*/




// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>

#include "simpleengineutils.h"
#include "simplecommon.h"

// ================= MEMBER FUNCTIONS =======================

//**********************************
// TSimpleEngineUtils
//**********************************

// ---------------------------------------------------------
// TSimpleEngineUtils::OpIdRange
// ---------------------------------------------------------
//
TInt TSimpleEngineUtils::OpIdRange()
    {
    // Bit mask to create server OOM error response
    const TUint KMaxId = 0x40000000;
    const TInt KMagic2 = 50;
    TInt limitLowerValue = REINTERPRET_CAST(TInt, Dll::Tls() );
    TInt limitUpperValue = limitLowerValue + KRangeSize;
    // Check that counter does not go around
    if ( limitUpperValue & KMaxId )
        {
        // start from 50, if this ever happens
        limitLowerValue = KMagic2;
        limitUpperValue = limitLowerValue + KRangeSize;
        }
    Dll::SetTls( REINTERPRET_CAST(TAny*, limitUpperValue ) );
    return limitLowerValue;
    }

//  End of File

