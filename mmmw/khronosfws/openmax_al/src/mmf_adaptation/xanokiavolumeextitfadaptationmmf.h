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
 * Description: 
 *
 */

#ifndef XANOKIAVOLUMEEXTITFADAPTATIONMMF_H
#define XANOKIAVOLUMEEXTITFADAPTATIONMMF_H

#include "xaadaptationmmf.h"
#include "xadebug.h"
#include "xathreadsafety.h"

/* MACROS */

/* DEFINES */

/* FUNCTIONS */

XAresult XANokiaVolumeExtItfAdapt_SetMute(XAAdaptationMMFCtx *ctx,
        XAboolean mute);

XAresult XANokiaVolumeExtItfAdapt_EnableStereoPosition(
        XAAdaptationMMFCtx *ctx, XAboolean enable);

XAresult XANokiaVolumeExtItfAdapt_SetStereoPosition(XAAdaptationMMFCtx *ctx,
        XApermille stereoPosition);

#endif /* XANOKIAVOLUMEEXTITFADAPTATIONMMF_H */
