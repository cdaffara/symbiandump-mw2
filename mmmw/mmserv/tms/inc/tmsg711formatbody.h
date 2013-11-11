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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMS_G711_FORMAT_BODY_H
#define TMS_G711_FORMAT_BODY_H

#include <tms.h>

namespace TMS {

// TMSG711FormatBody class
class TMSG711FormatBody
    {
public:
    virtual ~TMSG711FormatBody() {}

    virtual gint SetMode(const TMSG711CodecMode aMode) = 0;
    virtual gint GetMode(TMSG711CodecMode& aMode) = 0;
    virtual gint SetCNG(const gboolean aCng) = 0;
    virtual gint GetCNG(gboolean& aCng) = 0;
    virtual gint SetPlc(const gboolean aPlc) = 0;
    virtual gint GetPlc(gboolean& aPlc) = 0;
    virtual gint SetVADMode(const gboolean aVad) = 0;
    virtual gint GetVADMode(gboolean& aVad) = 0;
    virtual gint GetType(TMSFormatType& Formattype) = 0;
    };

} //namespace TMS

#endif // TMS_G711_FORMAT_BODY_H

// End of file
