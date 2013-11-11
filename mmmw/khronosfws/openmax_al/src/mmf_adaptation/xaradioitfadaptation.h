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
#ifndef XARADIOITFFADAPTATION_H
#define XARADIOITFFADAPTATION_H

#include "xaadaptationmmf.h"
#include "xadebug.h"

/* FUNCTIONS */

XAresult XARadioItfAdapt_SetFreqRange(XAAdaptationMMFCtx *bCtx, XAuint8 range);

XAresult XARadioItfAdapt_GetFreqRange(XAuint8* pFreqRange);

XAresult XARadioItfAdapt_IsFreqRangeSupported(XAuint8 range,
                                             XAboolean * pSupported);

XAresult XARadioItfAdapt_GetFreqRangeProperties(XAAdaptationMMFCtx *bCtx,
                                               XAuint8  range,
                                               XAuint32* pMinFreq,
                                               XAuint32* pMaxFreq);

XAresult XARadioItfAdapt_SetFrequency(XAAdaptationMMFCtx *bCtx, XAuint32 freq);

XAresult XARadioItfAdapt_GetFrequency(XAuint32 * pFrequency);

XAresult XARadioItfAdapt_CancelSetFrequency(void);

XAresult XARadioItfAdapt_SetSquelch(XAAdaptationMMFCtx *bCtx, XAboolean squelch);

XAresult XARadioItfAdapt_GetSquelch(XAboolean* squelch);

XAresult XARadioItfAdapt_SetStereoMode(XAAdaptationMMFCtx *bCtx, XAuint32 mode);

XAresult XARadioItfAdapt_GetSignalStrength(XAuint32 * pStrength);

XAresult XARadioItfAdapt_Seek(XAAdaptationMMFCtx *bCtx, XAboolean upwards);

void XARadioItfAdapt_StopSeeking(XAAdaptationMMFCtx *bCtx);

void XARadioItfAdapt_Free(void);

void XARadioItfAdapt_SeekComplete(XAAdaptationBaseCtx *ctx, TInt aError, TInt aFrequency);
	
void XARadioItfAdapt_FrequencyChange(XAAdaptationBaseCtx *ctx, TInt aError);

void XARadioItfAdapt_FrequencyRangeChange(XAAdaptationBaseCtx *ctx, TInt aFreqRangeChange);

void XARadioItfAdapt_SetFrequencyComplete(XAAdaptationBaseCtx *ctx, TInt aError, TInt aFrequency);

void XARadioItfAdapt_SetFrequencyRangeComplete(XAAdaptationBaseCtx *ctx, TInt aError);

XAresult XARadioItfAdapt_GetStereoMode(XAuint32 * pMode);

void XARadioItfAdapt_StereoStatusChange(XAAdaptationBaseCtx *ctx, XAboolean aStereoStatus);

void XARadioItfAdapt_SignalStrengthChange(XAAdaptationBaseCtx *ctx, TInt aSignalStrength);

void XARadioItfAdapt_StateChange(XAAdaptationBaseCtx *ctx, XAboolean aState);

void XARadioItfAdapt_VolumeChange(XAAdaptationBaseCtx *ctx, TInt aVolume);

void XARadioItfAdapt_MuteChange(XAAdaptationBaseCtx* ctx, XAboolean aMute);

#endif /* XARADIOITFADAPTATION_H */
