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
 * Description: MediaPlayer Adapt MMF HEader
 *
 */

#ifndef XAMEDIAPLAYERADAPTCTXMMF_H
#define XAMEDIAPLAYERADAPTCTXMMF_H

#include "xaadaptationmmf.h"

/* TYPEDEFS */

typedef struct XAMediaPlayerAdaptationMMFCtx_ XAMediaPlayerAdaptationMMFCtx;

/* FUNCTIONS */
XAAdaptationBaseCtx
        * XAMediaPlayerAdaptMMF_Create(XADataSource *pDataSrc,
                XADataSource *pBankSrc, XADataSink *pAudioSnk,
                XADataSink *pImageVideoSnk, XADataSink *pVibra,
                XADataSink *pLEDArray);
XAAdaptationBaseCtx
* XAMediaPlayerAdaptMMF_CreateRadio(XAMediaPlayerAdaptationMMFCtx *pSelf,
        XADataSource *pDataSrc, XADataSource *pBankSrc,
        XADataSink *pAudioSnk, XADataSink *pImageVideoSnk,
        XADataSink *pVibra, XADataSink *pLEDArray);
XAresult XAMediaPlayerAdaptMMF_PostInit(XAAdaptationMMFCtx* bCtx);
void XAMediaPlayerAdaptMMF_Destroy(XAAdaptationMMFCtx* bCtx);

//XAresult XAMediaPlayerAdapt_InitContentPipeSrc(XAMediaPlayerAdaptationCtx* ctx);
#endif /*XAMEDIAPLAYERADAPTCTXMMF_H*/

