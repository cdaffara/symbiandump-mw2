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
* Description:  LocVerifierDlgDebug contains debug macros.
*
*/


#ifndef LOCVERIFIERDLGDEBUG_H 
#define LOCVERIFIERDLGDEBUG_H

// INCLUDES
#include "locutilsdebug.h"
#include <e32std.h>


// MACROS

/**
* By using component specific debug macros unnecessary 
* debug prints can be avoided. 
* Also component specific debugging is possible by compiling 
* only certain components when LOC_ENABLE_DEBUG_PRINT is defined.
*/

#ifdef LOC_ENABLE_DEBUG_PRINT
#define LOCVERIFIERDLGDEBUG(TEXT) DEBUG(TEXT);
#define LOCVERIFIERDLGDEBUG1(TEXT, ARG1) LOC(TEXT, ARG1);
#define LOCVERIFIERDLGDEBUG2(TEXT, ARG1, ARG2) LOC2(TEXT, ARG1, ARG2);
#define LOCVERIFIERDLGDEBUG3(TEXT, ARG1, ARG2, ARG3) LOC3(TEXT, ARG1, ARG2, ARG3);
#else
#define LOCVERIFIERDLGDEBUG(TEXT);
#define LOCVERIFIERDLGDEBUG1(TEXT, ARG1);
#define LOCVERIFIERDLGDEBUG2(TEXT, ARG1, ARG2);
#define LOCVERIFIERDLGDEBUG3(TEXT, ARG1, ARG2, ARG3);
#endif

#endif /* LOCVERIFIERDLGDEBUG_H */
