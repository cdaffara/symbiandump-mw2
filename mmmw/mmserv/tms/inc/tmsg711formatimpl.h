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

#ifndef TMS_G711_FORMAT_IMPL_H
#define TMS_G711_FORMAT_IMPL_H

#include <tms.h>
#include <tmsg711format.h>

namespace TMS {

// TMSG711FormatImpl class
class TMSG711FormatImpl : public TMSG711Format
    {
public:
    virtual ~TMSG711FormatImpl();
    static gint Create(TMSFormat*& tmsfmt);
    gint SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSG711FormatImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_G711_FORMAT_IMPL_H

// End of file
