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

#ifndef TMS_EFFECT_IMPL_H
#define TMS_EFFECT_IMPL_H

#include <glib.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSEffect;

// TMSEffectImpl class
class TMSEffectImpl
    {
public:
    IMPORT_C static gint Create(TMSEffectType effecttype,
            TMSEffect*& tmseffect);
    IMPORT_C static gint Delete(TMSEffect*& tmseffect);
    };

} //namespace TMS

#endif // TMS_EFFECT_IMPL_H

// End of file
