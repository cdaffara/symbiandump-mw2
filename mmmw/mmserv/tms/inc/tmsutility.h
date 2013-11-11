/*
 * Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Tracing and convertion utility.
 *
 */

#ifndef TMSUTILITY_H
#define TMSUTILITY_H

#include <e32std.h>
#include <tms.h>
#include <TelephonyAudioRouting.h>
#ifdef _DEBUG
#include <e32debug.h>
#endif

namespace TMS {

// DEBUG TRACE DEFINITIONS
#ifdef _DEBUG
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

// MACRO DEFINITIONS
#define RET_ERR_IF_ERR(err) if (err != 0) return err;
#define RET_REASON_IF_ERR(s) if(s!=0) return s

#define TMSRESULT(result) TMSUtility::TMSResult(result)
#define SYMBIANRESULT(result) TMSUtility::SymbianResult(result)
#define TOTMSOUTPUT(output) TMSUtility::ToTMSOutput(output)
#define TOTAROUTPUT(output) TMSUtility::ToTAROutput(output)
#define TOFOURCC(format) TMSUtility::ToFourCC(format)
#define TOTMSFORMAT(fourcc) TMSUtility::FourCCToTMSFormat(fourcc)

_LIT(KTMSPanic, "TMS Panic!" );
#define PANIC(reason) User::Panic(KTMSPanic, reason)

// TMSUtility class
class TMSUtility
    {
public:
    IMPORT_C static gint TMSResult(const gint error);
    IMPORT_C static TInt SymbianResult(const gint error);
    IMPORT_C static gint EtelToTMSResult(const gint error);
    IMPORT_C static TMSAudioOutput ToTMSOutput(
            CTelephonyAudioRouting::TAudioOutput taroutput);
    IMPORT_C static CTelephonyAudioRouting::TAudioOutput ToTAROutput(
            TMSAudioOutput output);
    IMPORT_C static TUint32 ToFourCC(TMSFormatType frmtType);
    IMPORT_C static TMSFormatType FourCCToTMSFormat(TUint32 fourcc);
    };

} //namespace TMS

#endif //TMSUTILITY_H

