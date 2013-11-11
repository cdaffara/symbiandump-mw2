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
 * Description: StreamInfo Itf Adaptation MMF header
 *
 */

#ifndef XASTREAMINFORMATIONITFADAPTATIONMMF_H
#define XASTREAMINFORMATIONITFADAPTATIONMMF_H

#include "openmaxalwrapper.h"
#include "xaadptbasectx.h"
/* MACROS */

/* FUNCTIONS */
XAresult XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 *containerType,
        XAmillisecond *mediaDuration, XAuint32 *numStreams);

XAresult XAStreamInformationItfAdaptMMF_QueryStreamType(
                XAAdaptationBaseCtx *adaptCtx, XAuint32 streamIndex,
                XAuint32 *domain);

XAresult XAStreamInformationItfAdaptMMF_QueryStreamInformation(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamIndex, void * info);

XAresult XAStreamInformationItfAdaptMMF_QueryStreamName(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamIndex,
        XAuint16 * pNameSize, XAchar * pName);

XAresult XAStreamInformationItfAdaptMMF_QueryActiveStreams(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 *numStreams,
        XAboolean *activeStreams);

XAresult XAStreamInformationItfAdaptMMF_SetActiveStream(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamNum, XAboolean active,
        XAboolean commitNow);

XAresult XAStreamInformationItfAdaptMMF_RegisterCallback(
				XAAdaptationBaseCtx *adaptCtx,
				xaStreamEventChangeCallback callback);

#endif /* XASTREAMINFORMATIONITFADAPTATION_H */
