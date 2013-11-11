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
 * Description: Play Itf Adaptation MMF Header
 *
 */

#ifndef XAPLAYITFADAPTATIONMMF_H
#define XAPLAYITFADAPTATIONMMF_H

#include "xaadaptationmmf.h"

#ifdef _DEBUG
/*parse state names for debug prints*/
static const char* playstatenamemmf[3] =
    {
    "XA_PLAYSTATE_STOPPED", "XA_PLAYSTATE_PAUSED", "XA_PLAYSTATE_PLAYING"
    };
#define PLAYSTATENAME(i) (const char*)((i>0&&i<4)?playstatenamemmf[i-1]:"INVALID")
#endif /*_DEBUG*/

/* FUNCTIONS */

XAresult XAPlayItfAdaptMMF_SetPlayState(XAAdaptationBaseCtx *bCtx,
        XAuint32 state);
XAresult XAPlayItfAdaptMMF_GetPlayState(XAAdaptationBaseCtx *bCtx,
        XAuint32 *pState);
XAresult XAPlayItfAdaptMMF_GetDuration(XAAdaptationBaseCtx *bCtx,
        XAmillisecond *pMsec);
XAresult XAPlayItfAdaptMMF_GetPosition(XAAdaptationBaseCtx *bCtx,
        XAmillisecond *pMsec);
XAresult XAPlayItfAdaptMMF_RegisterCallback(XAAdaptationBaseCtx *bCtx,
        xaPlayCallback callback);
XAresult XAPlayItfAdaptMMF_SetCallbackEventsMask(XAAdaptationBaseCtx *bCtx,
        XAuint32 eventFlags);
XAresult XAPlayItfAdaptMMF_SetMarkerPosition(XAAdaptationBaseCtx *bCtx,
        XAmillisecond mSec);
XAresult XAPlayItfAdaptMMF_ClearMarkerPosition(XAAdaptationBaseCtx *bCtx);
XAresult XAPlayItfAdaptMMF_SetPositionUpdatePeriod(XAAdaptationBaseCtx *bCtx,
        XAmillisecond mSec);
#endif /* XAPLAYITFADAPTATIONMMF_H */
