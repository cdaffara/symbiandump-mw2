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

#ifndef TMS_FORMATIMPL_H
#define TMS_FORMATIMPL_H

#include <tms.h>
#include <tmsformat.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSFormatImpl class
class TMSFormatImpl : public TMSFormat
    {
public:
    IMPORT_C static gint Create(TMSFormatType formattype,
            TMSFormat*& tmsformat);
    IMPORT_C static gint Delete(TMSFormat*& tmsformat);
    };

} //namespace TMS

#endif //TMS_FORMATIMPL_H
