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

#ifndef XARADIOADAPTCTX_H
#define XARADIOADAPTCTX_H

#include "xaadaptationmmf.h"


#define RADIO_DEFAULT_FREQ_RANGE XA_FREQRANGE_FMEUROAMERICA
#define RADIO_DEFAULT_FREQ 88000000

/* TYPEDEFS */
typedef struct XARadioAdaptationCtx_ XARadioAdaptationCtx;

/* FUNCTIONS */
XAAdaptationBaseCtx* XARadioAdapt_Create(void);
XAresult XARadioAdapt_PostInit(XAAdaptationBaseCtx* bCtx);
void XARadioAdapt_Destroy(XAAdaptationBaseCtx* bCtx);

#endif /* XARADIOADAPTCTX_H */
