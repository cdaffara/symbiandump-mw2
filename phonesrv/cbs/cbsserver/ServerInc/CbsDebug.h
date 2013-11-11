/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains various macros to control debug printing with
*                RDebug::Print() and insertion of file failure test code.
*                NOTE-macro is provided to simplify printing warnings compile-time.
*
*/



#ifndef CBS_DEBUG_H
#define CBS_DEBUG_H

/////////////////////////////////////////////////////////////////////////////
// #Defines that control printing of additional information to debug stream.
/////////////////////////////////////////////////////////////////////////////

// Defining this enables debug printing with DEBUG(), DVAL() and DVAL2 macros.
//#define CBS_PRINT_DEBUG_INFO

// Defining this enables printing of headers of received messages
//#define CBS_PRINT_MESSAGE_HEADER

// Defining this makes message generators print the content of generated
// messages in plaintext.
//#define CBS_PRINT_MESSAGE_CONTENT

// Defining this makes message generators print the content of generated 
// messages in hexadecimal
//#define CBS_PRINT_GENERATED_MESSAGES_IN_HEX

// Prints topic file store structure to debug stream on startup.
//#define CBS_PRINT_STORE_STRUCTURE 1

// Prints additional info on generated messages
//#define CBS_EMULATOR_PRINT_ADDITIONAL_INFO

/////////////////////////////////////////////////////////////////////////////
// #Defines that control compilation of file operation failure tests.
/////////////////////////////////////////////////////////////////////////////

// Defining this inserts User::Leave:s before file changes are commited.
// Usage: FAIL_FILE_OP; forces a leave if DO_FILE_OP_FAILURE_TESTS is defined.

// #define DO_FILE_OP_FAILURE_TESTS 1

// Defining this inserts User::Leave:s to critical places trying to break 
// the database integrity.
// Usage: FAIL_FILE_OP_IN_A_NASTY_WAY; forces a leave if 
// DO_NASTY_FILE_OP_FAILURE_TESTS is defined.

// #define DO_NASTY_FILE_OP_FAILURE_TESTS 1

// Defining this inserts User::Leave:s before file operations on startup.
// Usage: FAIL_FILE_OP_STARTUP; forces a leave if 
// DO_FILE_OP_FAILURE_STARTUP_TESTS is defined.

//#define DO_FILE_OP_FAILURE_STARTUP_TESTS

/////////////////////////////////////////////////////////////////////////////
// Macros to simplify writing to the debug stream.
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#ifdef CBS_PRINT_DEBUG_INFO
#include <e32svr.h>
#define DEBUG(s) RDebug::Print(_L(s))
#define DVA(s, v) RDebug::Print(_L(s), v)
#define DVA2(s, v1, v2) RDebug::Print(_L(s), v1, v2)
#define DPRINT(s) RDebug::Print(s)

#else // CBS_PRINT_DEBUG_INFO
#define DEBUG(s) 
#define DVA(s, v)
#define DVA2(s, v1, v2)
#define DPRINT(s)
#endif // CBS_PRINT_DEBUG_INFO
#else
#define DEBUG(s)
#define DVA(s, v)
#define DVA2(s, v1, v2)
#define DPRINT(s)
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// Macros to test file operation failures.
/////////////////////////////////////////////////////////////////////////////

#ifdef DO_FILE_OP_FAILURE_TESTS
#define FAIL_FILE_OP User::Leave( KErrDiskFull )
#else
#define FAIL_FILE_OP
#endif // DO_FILE_OP_FAILURE_TESTS

#ifdef DO_NASTY_FILE_OP_FAILURE_TESTS
#define FAIL_FILE_OP_IN_A_NASTY_WAY User::Leave( KErrDiskFull )
#else
#define FAIL_FILE_OP_IN_A_NASTY_WAY
#endif // DO_FILE_OP_FAILURE_TESTS

#ifdef DO_FILE_OP_FAILURE_STARTUP_TESTS
#define FAIL_FILE_OP_STARTUP User::Leave( KErrDiskFull )
#else
#define FAIL_FILE_OP_STARTUP
#endif // DO_FILE_OP_FAILURE_TESTS

/////////////////////////////////////////////////////////////////////////////
// NOTE is a convience macro to simplify printing warning texts compile-time.
// The macro will print the filename and line number in question.
// 
// Usage: #pragma message(NOTE "Please remove this")
/////////////////////////////////////////////////////////////////////////////
#define STRINGIZE( L )             #L
#define MAKESTRING( M, L )         M(L)
#define LINENUMBER                 MAKESTRING( STRINGIZE, __LINE__ )
#define NOTE                       __FILE__ "(" LINENUMBER ") : "

#endif      // CBS_DEBUG_H

// End of File
