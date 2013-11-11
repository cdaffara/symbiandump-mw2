/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of test and logging macroes.
*
*/


#ifndef SATMACROES_H
#define SATMACROES_H

// Use this macro to enable dummy TSY.
// Must be disabled on released version.
// MACRO SAT_USE_DUMMY_TSY
// #define SAT_USE_DUMMY_TSY

// This used only when testing Launch Browser without real BrowserApp.
// Must be disabled on released version.
// MACRO MODULE_TESTING_LB
// #define MODULE_TESTING_LB

#ifdef _DEBUG
// Use this macro to enable logging.
// Must be disabled on released version.
MACRO ENABLE_SAT_LOGGING
#define ENABLE_SAT_LOGGING
#endif

#endif      // SATMACROES_H
