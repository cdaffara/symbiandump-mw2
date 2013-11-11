/*
 * Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Tracing utility.
 *
 */

#ifndef DEBUGTRACEMACROS_H
#define DEBUGTRACEMACROS_H

#define RET_ERR_IF_ERR(err) if ( err != KErrNone) return err;

#ifdef _DEBUG
#include "e32debug.h"

#define TRACE_PRN_FN_ENT        RDebug::Printf("%s>ENTER", __PRETTY_FUNCTION__)
#define TRACE_PRN_FN_EXT        RDebug::Printf("%s>EXIT", __PRETTY_FUNCTION__)
#define TRACE_PRN_IF_ERR(err) if (err != KErrNone) RDebug::Printf( "%s>BREAK[%d]", __PRETTY_FUNCTION__, err )

#define TRACE_PRN_0(str)              RDebug::Print(str, this)
#define TRACE_PRN_1(str, val1)        RDebug::Print(str, this, val1)
#define TRACE_PRN_2(str, val1, val2)  RDebug::Print(str, this, val1, val2)

#define TRACE_PRN_N(str)              RDebug::Print(str)
#define TRACE_PRN_N1(str, val1)       RDebug::Print(str, val1)
#define TRACE_PRN_N2(str, val1, val2) RDebug::Print(str, val1, val2)

#else

#define TRACE_PRN_FN_ENT
#define TRACE_PRN_FN_EXT
#define TRACE_PRN_IF_ERR(err)
#define TRACE_PRN_0(str)
#define TRACE_PRN_1(str, val1)
#define TRACE_PRN_2(str, val1, val2)
#define TRACE_PRN_N(str)
#define TRACE_PRN_N1(str, val1)
#define TRACE_PRN_N2(str, val1, val2)
#endif //_DEBUG
#endif // DEBUGTRACEMACROS_H

// End of file
