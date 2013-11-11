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
 * This file is the direct implementation of the System Tone Service
 * API.  This acts as a facade to the actual implementation logic
 * which is in the CStsImplementation class.
 */

#ifndef STSDEBUG_H
#define STSDEBUG_H

#ifdef _DEBUG
#include <e32debug.h>
#endif /*_DEBUG*/

#ifdef _DEBUG
#   define TRACE_FUNCTION_ENTRY         RDebug::Printf( "%s >", __PRETTY_FUNCTION__)
#   define TRACE_FUNCTION_EXIT          RDebug::Printf( "%s <", __PRETTY_FUNCTION__)
#   define TRACE_FUNCTION_ENTRY_EXIT    RDebug::Printf( "%s ><", __PRETTY_FUNCTION__)
#   define TRACE_LOG(s)                 RDebug::Print s
#else
#   define TRACE_FUNCTION_ENTRY
#   define TRACE_FUNCTION_EXIT
#   define TRACE_FUNCTION_ENTRY_EXIT
#   define TRACE_LOG
#endif /* _DEBUG */

#endif /*STSDEBUG_H*/
