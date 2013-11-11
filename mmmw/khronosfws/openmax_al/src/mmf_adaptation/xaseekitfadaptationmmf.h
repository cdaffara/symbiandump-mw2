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
 * Description: Seek Itf Adaptation Header
 *
 */

#ifndef XASEEKITFADAPTATIONMMF_H
#define XASEEKITFADAPTATIONMMF_H

#include "xaadaptationmmf.h"

/* FUNCTIONS */

XAresult XASeekItfAdaptMMF_SetPosition(XAAdaptationBaseCtx *bCtx,
        XAmillisecond pos, XAuint32 seekMode);
XAresult XASeekItfAdaptMMF_SetLoop(XAAdaptationBaseCtx *bCtx,
        XAboolean loopEnable, XAmillisecond startPos, XAmillisecond endPos);

#endif /* XASEEKITFADAPTATIONMMF_H */
