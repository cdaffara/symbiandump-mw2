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

#ifndef TMS_CALL_IMPL_H
#define TMS_CALL_IMPL_H

#include <glib.h>
#include <tmscall.h>

namespace TMS {

// TMSCallImpl class
class TMSCallImpl : public TMSCall
    {
public:
    virtual ~TMSCallImpl();
    IMPORT_C static gint Create(TMSCallType ctype, TMSCall*& tmscall,
            guint ctxid);
    IMPORT_C static gint Delete(TMSCall*& tmscall);

private:
    TMSCallImpl();
    gint PostConstruct(TMSCallType ctype, guint ctxid);
    };

} //namespace TMS

#endif // TMS_CALL_IMPL_H

// End of file
