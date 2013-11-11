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
* Description: Entry point for phonesettings test.
*
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <e32svr.h>
#include <StifTestInterface.h>

GLDEF_C TInt E32Main()
    {
    _LIT( KProcessMsgStart, "New process starting" );
    RDebug::Print( KProcessMsgStart );

#ifndef EKA2 // Works only in EKA2
    _LIT( KErrorMsg, "ERROR !!! THIS NEEDS AN EKA2 ENVIRONMENT, ABORTING" );
    RDebug::Print( KErrorMsg );
    return KErrAbort;
#endif

    // This starts a new session that get capabilites that is used in 
    // PhoneSettingsTest_exe.mmp file.
    TInt r = StartSession();

    _LIT( KProcessMsgEnd, "New process ends" );
    RDebug::Print( KProcessMsgEnd );

    return r;

    }

// End of File
