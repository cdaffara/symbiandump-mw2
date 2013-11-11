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
 * Description: Record Itf adapt header
 *
 */

#ifndef XARECORDITFADAPTATIONMMF_H
#define XARECORDITFADAPTATIONMMF_H

#include "xaadaptationmmf.h"
#include "xadebug.h"
#include <glib.h>

#ifdef _DEBUG
/*parse state names for debug prints*/
static const char* recordstatenamesmmf[3] =
    {
            "XA_RECORDSTATE_STOPPED",
            "XA_RECORDSTATE_PAUSED",
            "XA_RECORDSTATE_RECORDING",
    };
#define RECORDSTATENAMEMMF(i) ((i>0&&i<4)?recordstatenamesmmf[i-1]:"INVALID")
#endif /*_DEBUG*/

/*forward declaration of position updater callback*/
gboolean XARecordItfAdaptMMF_PositionUpdate(gpointer ctx, XAuint64 position);

/* FUNCTIONS */

XAresult XARecordItfAdaptMMF_SetRecordState(XAAdaptationMMFCtx *ctx,
        XAuint32 state);
XAresult XARecordItfAdaptMMF_GetPosition(XAAdaptationMMFCtx *ctx,
        XAmillisecond *pMsec);
XAresult XARecordItfAdaptMMF_GetRecordState(XAAdaptationMMFCtx *bCtx,
        XAuint32 *state);
XAresult XARecordItfAdaptMMF_EnablePositionTracking(XAAdaptationMMFCtx *ctx,
        XAboolean enable);
XAresult XARecordItfAdaptMMF_SetPositionUpdatePeriod(XAAdaptationMMFCtx *ctx,
        XAmillisecond pMsec);

#endif /* XARECORDITFADAPTATIONMMF_H */
