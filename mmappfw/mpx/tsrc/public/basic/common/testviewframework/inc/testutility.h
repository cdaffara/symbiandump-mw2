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

#ifndef TESTUTILITY_H
#define TESTUTILITY_H

#include <e32cmn.h>
#include <e32std.h>

// FORWARD DECLARATION
class CStifLogger;

class TestUtility
    {
    public:
        /**
        * Convert a string to decemial
        * @param aStr a string to convert
        */
        IMPORT_C static TInt ConvertStrToDecL(const TDesC& aStr);
        
        /**
        * Log message to RDebug and Stiftestlogger if any
        * @param aLogger a STIF logger to use (can be NULL)
        * @param aFmt a string contains parameter list 
        * @param ...
        */
        IMPORT_C static void TestLog(CStifLogger* aLogger, 
                                     TRefByValue< const TDesC > aFmt, 
                                     ...);
                                     
    };
    
#endif  //TESTUTILITY_H    
