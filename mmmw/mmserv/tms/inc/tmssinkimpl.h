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

#ifndef TMS_SINK_IMPL_H
#define TMS_SINK_IMPL_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSSink;

// TMSSinkImpl class
class TMSSinkImpl
    {
public:
    IMPORT_C static gint Create(TMSSinkType sinktype, TMSSink*& tmssink);
    IMPORT_C static gint Delete(TMSSink*& tmssink);
    };

} //namespace TMS

#endif // TMS_SINK_IMPL_H

// End of file
