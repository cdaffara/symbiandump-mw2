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

#ifndef TMS_PCM_FORMAT_IMPL_H
#define TMS_PCM_FORMAT_IMPL_H

#include <tms.h>
#include <tmspcmformat.h>

namespace TMS {

// TMSPCMFormatImpl class
class TMSPCMFormatImpl : public TMSPCMFormat
    {
public:
    virtual ~TMSPCMFormatImpl();
    static gint Create(TMSFormat*& tmsfmt);
    gint SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSPCMFormatImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_PCM_FORMAT_IMPL_H

// End of file
