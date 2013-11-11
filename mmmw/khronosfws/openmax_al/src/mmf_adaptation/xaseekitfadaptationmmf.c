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
 * Description: Seek Itf Adaptation MMF
 *
 */

#include <glib.h>
#include "unistd.h"
#include "xamediaplayeradaptctxmmf.h"
#include "xaseekitfadaptationmmf.h"
#include "cmmfbackendengine.h"

/*
 * XAresult XAPlayItfAdaptMMF_SetPlayState(XAAdaptationBaseCtx *bCtx, XAuint32 state)
 * Sets play state to GStreamer.
 * @param XAAdaptationBaseCtx *bCtx - Adaptation context, this will be casted to correct type regarding to contextID
 * XAuint32 state - Play state to be set
 * @return XAresult ret - Success value
 */
XAresult XASeekItfAdaptMMF_SetPosition(XAAdaptationBaseCtx *bCtx,
        XAmillisecond pos, XAuint32 seekMode)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx =
            (XAMediaPlayerAdaptationMMFCtx*) bCtx;

    DEBUG_API("->XASeekItfAdaptMMF_SetPosition ");
    ret = mmf_seekitf_set_position(mCtx->mmfContext, pos);
    /* bCtx and parameter pointer validation happens in the calling function.
     * We don't need to repeat it here*/

    DEBUG_API("<-XASeekItfAdaptMMF_SetPosition");
    return ret;
    }

XAresult XASeekItfAdaptMMF_SetLoop(XAAdaptationBaseCtx *bCtx,
        XAboolean loopEnable, XAmillisecond startPos, XAmillisecond endPos)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaPlayerAdaptationMMFCtx* mCtx =
            (XAMediaPlayerAdaptationMMFCtx*) bCtx;

    DEBUG_API("->XASeekItfAdaptMMF_SetLoop ");

    /* bCtx and parameter pointer validation happens in the calling function.
     * We don't need to repeat it here*/
    ret = mmf_seekitf_set_repeats(mCtx->mmfContext, loopEnable);

    ret = mmf_seekitf_set_playwindow(mCtx->mmfContext, startPos, endPos);

    DEBUG_API("<-XASeekItfAdaptMMF_SetLoop");
    return ret;
    }
