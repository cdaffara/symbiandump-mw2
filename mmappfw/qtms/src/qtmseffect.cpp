/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: QT Bindings for TMS
 *
 */

#include "qtmseffectimpl.h"
#include "qtmseffect.h"
#include "qtmswrapperexport.h"


void QTMSEffect::Construct(QTMSEffectImpl* impl)
{
    QTMSeffectimpl = impl;
}

QTMSEffect::QTMSEffect(void)
{
	
}

 QTMSEffect::~QTMSEffect(void)
{
	
}
