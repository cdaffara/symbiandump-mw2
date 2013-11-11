/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Help tools
*
*/

#include <stiflogger.h>
#include <aknlists.h>
#include "testutility.h"

// -----------------------------------------------------------------------------
// Convert string to decemial
// -----------------------------------------------------------------------------
EXPORT_C TInt TestUtility::ConvertStrToDecL(const TDesC& aStr)
    {
    TInt result = 0;
    for(TInt i=0; i<aStr.Length(); i++)
        {
        TUint16 ch = aStr[i];
        if(ch < 48 || ch > 57)
            User::Leave(KErrArgument);
        ch -= 48;
        result *= 10;
        result += ch;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// Log message
// -----------------------------------------------------------------------------
EXPORT_C void TestUtility::TestLog(CStifLogger* aLogger,
                                   TRefByValue< const TDesC > aFmt, 
                                   ...)
    {
    VA_LIST list;
    VA_START(list, aFmt);
    RDebug::Print( aFmt, list );
    if(aLogger)
        {
        TBuf<KMaxLogData> logData;
        logData.FormatList( aFmt, list );
        aLogger->Log(logData);
        }
    VA_END(list);
    }

