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
 * Description: Record Itf GST adapt header
 *
 */

#ifndef XARECORDITFADAPTATION_H
#define XARECORDITFADAPTATION_H

#include "xaadaptationgst.h"

#ifdef _DEBUG
/*parse state names for debug prints*/
static const char* recordstatenames[3] =
    {
            "XA_RECORDSTATE_STOPPED",
            "XA_RECORDSTATE_PAUSED",
            "XA_RECORDSTATE_RECORDING",
    };
#define RECORDSTATENAME(i) ((i>0&&i<4)?recordstatenames[i-1]:"INVALID")
#endif /*_DEBUG*/

/* FUNCTIONS */

XAresult XARecordItfAdapt_SetRecordState(XAAdaptationGstCtx *ctx,
        XAuint32 state);
XAresult XARecordItfAdapt_SetDurationLimit(XAAdaptationGstCtx *ctx,
        XAmillisecond msec);
XAresult XARecordItfAdapt_GetPosition(XAAdaptationGstCtx *ctx,
        XAmillisecond *pMsec);
XAresult XARecordItfAdapt_EnablePositionTracking(XAAdaptationGstCtx *ctx,
        XAboolean enable);
XAresult XARecordItfAdapt_GetRecordState(XAAdaptationGstCtx *bCtx,
        XAuint32 *state);

#endif /* XARECORDITFADAPTATION_H */
