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

#ifndef XAPLAYBACKRATEITFADAPTATION_H
#define XAPLAYBACKRATEITFADAPTATION_H

#include "xaadaptationmmf.h"


/* FUNCTIONS */

XAresult XAPlaybackRateItfAdaptMMF_SetRate(XAAdaptationMMFCtx *bCtx, XApermille rate);

XAresult XAPlaybackRateItfAdaptMMF_SetPropertyConstraints(XAAdaptationMMFCtx *bCtx,
                                                        XAuint32 constraints);

XAresult XAPlaybackRateItfAdaptMMF_GetProperties(XAAdaptationMMFCtx *bCtx,
                                               XAuint32 *pProperties);

XAresult XAPlaybackRateItfAdaptMMF_GetCapabilitiesOfRate(XAAdaptationMMFCtx *bCtx,
                                                       XApermille rate,
                                                       XAuint32 *pCapabilities);

XAresult XAPlaybackRateItfAdaptMMF_GetRateRange(XAAdaptationMMFCtx *bCtx,
                                              XAuint8 index,
                                              XApermille *pMinRate,
                                              XApermille *pMaxRate,
                                              XApermille *pStepSize,
                                              XAuint32 *pCapabilities);

#endif /* XAPLAYBACKRATEITFADAPTATION_H */
