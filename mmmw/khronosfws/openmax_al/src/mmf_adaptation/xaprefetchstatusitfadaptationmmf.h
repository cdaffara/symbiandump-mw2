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

#ifndef XAPREFETCHSTATUSITFADAPTATIONMMF_H
#define XAPREFETCHSTATUSITFADAPTATIONMMF_H

#include "xaadaptationmmf.h"

/* FUNCTIONS */

XAresult XAPrefetchStatusItfAdaptMMF_GetPrefetchStatus(XAAdaptationBaseCtx *bCtx, XAuint32 *pStatus);

XAresult XAPrefetchStatusItfAdaptMMF_GetFillLevel(XAAdaptationBaseCtx *bCtx, XApermille *pLevel);

XAresult XAPrefetchStatusItfAdaptMMF_RegisterCallback(XAAdaptationBaseCtx *bCtx,
                                                    xaPrefetchCallback callback);

XAresult XAPrefetchStatusItfAdaptMMF_SetCallbackEventsMask(XAAdaptationBaseCtx *bCtx,
                                                         XAuint32 eventFlags);

XAresult XAPrefetchStatusItfAdaptMMF_SetFillUpdatePeriod(XAAdaptationBaseCtx *bCtx,
                                                       XApermille period);

#endif /* XAPLAYITFADAPTATIONMMF_H */
