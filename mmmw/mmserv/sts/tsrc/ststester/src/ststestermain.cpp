/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * This is the main driver of System Tone Service Tester.
 */

//  Include Files  
#include "ststestermain.h"
#include "ststester.h"

//  Global Functions
GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    CActiveScheduler* scheduler = new CActiveScheduler;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if( scheduler != NULL && cleanup != NULL )
    {
        CActiveScheduler::Install( scheduler );
        TRAP_IGNORE( CStsTester::ExecuteL() );
    }
    delete cleanup;
    delete scheduler;
    __UHEAP_MARKEND;
    return KErrNone;
    }
