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
*     Main class
*
*/


#include "AlwaysOnlineStarterApp.h"
#include "AlwaysOnlineManagerLogging.h"



GLDEF_C TInt E32Main()
    {
    KAOMANAGER_LOGGER_FN1("AOMStarter E32Main");

    __UHEAP_MARK;

    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt retVal = KErrNoMemory;
    if ( cleanup )
        {
        TRAP( retVal,RunServerL() );

#ifdef _DEBUG
        if(retVal != KErrNone )
            {
            KAOMANAGER_LOGGER_WRITE_FORMAT("Error when calling RunServerL(), retVal:%d", retVal );
            KAOMANAGER_LOGGER_WRITE_NUMBER(retVal);
            }
#endif
        delete cleanup;
        }

    __UHEAP_MARKEND;

    KAOMANAGER_LOGGER_FN2("AOMStarter E32Main");
    return retVal;
    }


//EOF

