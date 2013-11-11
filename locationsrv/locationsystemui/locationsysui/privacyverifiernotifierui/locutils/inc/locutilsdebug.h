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
* Description:  LocUtilsDebug contains debug macros.
*
*/


#ifndef LOCUTILSDEBUG_H
#define LOCUTILSDEBUG_H

// INCLUDES
#include "locutilsdebugconfig.hrh"

#include <e32std.h>


// MACROS
#define DEBUG(TEXT) 				 { \
									  _LIT( KText, #TEXT ); \
									   Debug(KText) \
									 }
									 
#define LOC(TEXT, ARG1) 			 { \
									   _LIT( KText, #TEXT ); \
									   Debug( KText, (ARG1) ); \
									 }
									 
#define LOC2(TEXT, ARG1, ARG2) 		 { \
									   _LIT( KText, #TEXT ); \
									   Debug( KText,(ARG1), (ARG2) ); \
									 }
									 
#define LOC3(TEXT, ARG1, ARG2, ARG3) { \
									   _LIT( KText, #TEXT ); \
									   Debug( KText, (ARG1), (ARG2), (ARG3) ); \
									 }

IMPORT_C void Debug( TRefByValue<const TDesC> aText, ...);

/**
* By using component specific debug macros unnecessary 
* debug prints can be avoided. 
* Also component specific debugging is possible by compiling 
* only certain components when LOC_ENABLE_DEBUG_PRINT is defined.
*/

#ifdef LOC_ENABLE_DEBUG_PRINT

#define LOCUTILSDEBUG(TEXT)                     DEBUG(TEXT);
#define LOCUTILSDEBUG1(TEXT, ARG1)              LOC(TEXT, ARG1);
#define LOCUTILSDEBUG2(TEXT, ARG1, ARG2)        LOC2(TEXT, ARG1, ARG2);
#define LOCUTILSDEBUG3(TEXT, ARG1, ARG2, ARG3)  LOC3(TEXT, ARG1, ARG2, ARG3);

#else

#define LOCUTILSDEBUG(TEXT)                     ;
#define LOCUTILSDEBUG1(TEXT, ARG1)              ;
#define LOCUTILSDEBUG2(TEXT, ARG1, ARG2)        ;
#define LOCUTILSDEBUG3(TEXT, ARG1, ARG2, ARG3)  ;

#endif

#endif // LOCUTILSDEBUG_H
