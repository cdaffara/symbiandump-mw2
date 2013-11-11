/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#include "tmsutility.h"
#include "tmsamrformatimpl.h"
#include "tmsg711formatimpl.h"
#include "tmsg729formatimpl.h"
#include "tmsilbcformatimpl.h"
#include "tmspcmformatimpl.h"
#include "tmsformatbodyimpl.h"
#include "tmsformatimpl.h"

using namespace TMS;

EXPORT_C gint TMSFormatImpl::Create(TMSFormatType fmttype, TMSFormat*& tmsfmt)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;

    switch (fmttype)
        {
        case TMS_FORMAT_PCM:
            ret = TMSPCMFormatImpl::Create(tmsfmt);
            break;
        case TMS_FORMAT_AMR:
            ret = TMSAMRFormatImpl::Create(tmsfmt);
            break;
        case TMS_FORMAT_G711:
            ret = TMSG711FormatImpl::Create(tmsfmt);
            break;
        case TMS_FORMAT_G729:
            ret = TMSG729FormatImpl::Create(tmsfmt);
            break;
        case TMS_FORMAT_ILBC:
            ret = TMSILBCFormatImpl::Create(tmsfmt);
            break;
        default:
            ret = TMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSFormatImpl::Delete(TMSFormat*& tmsfmt)
    {
    gint ret(TMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED);
    TRACE_PRN_FN_ENT;
    TMSFormatType fmttype;
    ret = tmsfmt->GetType(fmttype);
    switch (fmttype)
        {
        case TMS_FORMAT_PCM:
            delete (static_cast<TMSPCMFormatImpl*>(tmsfmt));
            tmsfmt = NULL;
            break;
        case TMS_FORMAT_AMR:
            delete (static_cast<TMSAMRFormatImpl*>(tmsfmt));
            tmsfmt = NULL;
            break;
        case TMS_FORMAT_G711:
            delete (static_cast<TMSG711FormatImpl*>(tmsfmt));
            tmsfmt = NULL;
            break;
        case TMS_FORMAT_G729:
            delete (static_cast<TMSG729FormatImpl*>(tmsfmt));
            tmsfmt = NULL;
            break;
        case TMS_FORMAT_ILBC:
            delete (static_cast<TMSILBCFormatImpl*>(tmsfmt));
            tmsfmt = NULL;
            break;
        default:
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

// End of file
