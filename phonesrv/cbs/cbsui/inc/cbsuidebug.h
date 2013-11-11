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
*    Provides macros for testing.
*
*
*/


#ifndef CBSUIDEBUG_H
#define CBSUIDEBUG_H


////////////////////////////
// MEMORY TEST MACRO     //


#ifdef _DEBUG
#define _CBSUI_DEBUG
#endif // _DEBUG

#ifdef _CBSUI_DEBUG
    #define _CBSUI_ASSERT(condition, function) \
            _ASSERT_DEBUG(condition, function)
    #define _CBSUI_ASSERT_E(condition, error) \
            _ASSERT_DEBUG(condition, CbsUiPanic(error) )
#else
    #define _CBSUI_ASSERT(condition, function)
    #define _CBSUI_ASSERT_E(condition, error)
#endif //_CBSUI_DEBUG


#endif // CBSUIDEBUG_H

// End of File
