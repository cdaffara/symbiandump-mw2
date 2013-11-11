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

#ifndef TMS_AMR_FORMAT_IMPL_H
#define TMS_AMR_FORMAT_IMPL_H

#include <tms.h>
#include <tmsamrformat.h>
#include "tmsglobalcontext.h"

namespace TMS {

// TMSAMRFormatImpl class
class TMSAMRFormatImpl : public TMSAMRFormat
    {
public:
    virtual ~TMSAMRFormatImpl();
    static gint Create(TMSFormat*& tmsfmt);
    gint SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSAMRFormatImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_AMR_FORMAT_IMPL_H

// End of file

