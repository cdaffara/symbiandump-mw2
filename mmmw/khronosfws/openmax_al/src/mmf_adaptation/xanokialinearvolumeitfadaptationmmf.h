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
 * Description: Nokia Linear Volume Header
 *
 */

#ifndef XANOKIALINEARVOLUMEITFADAPTATIONMMF_H
#define XANOKIALINEARVOLUMEITFADAPTATIONMMF_H

#include "xaadaptationmmf.h"
#include "xadebug.h"
#include "xathreadsafety.h"

/* MACROS */

/* FUNCTIONS */
XAresult XANokiaLinearVolumeItfAdapt_GetVolumeLevel(XAAdaptationMMFCtx *ctx,
                                         XAuint32 * percentage);

XAresult XANokiaLinearVolumeItfAdapt_SetVolumeLevel(XAAdaptationMMFCtx *ctx,
        XAuint32 percentage);
XAresult XANokiaLinearVolumeItfAdapt_GetMaxVolumeLevel(
        XAAdaptationMMFCtx *ctx, XAuint32 * pPercentage);

XAresult XANokiaLinearVolumeItfAdapt_GetStepCount(XAAdaptationMMFCtx *ctx,
        XAuint32 * pStepCount);

#endif /* XANOKIALINEARVOLUMEITFADAPTATIONMMF_H */
