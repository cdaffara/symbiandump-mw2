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
 * Description: Metadata Adaptation MMF
 *
 */

#ifndef XAMETADATAADAPTCTXMMF_H
#define XAMETADATAADAPTCTXMMF_H

#include "xaadaptationmmf.h"

/* TYPEDEFS */

typedef struct XAMetadataAdaptationMMFCtx_ XAMetadataAdaptationMMFCtx;

/* FUNCTIONS */
XAAdaptationBaseCtx* XAMetadataAdaptCtxMMF_Create(XADataSource *pDataSrc);
XAresult XAMetadataAdaptCtxMMF_PostInit(XAAdaptationMMFCtx* bCtx);
void XAMetadataAdaptCtxMMF_Destroy(XAAdaptationMMFCtx* bCtx);
#endif /*XAMETADATAADAPTCTXMMF_H*/

