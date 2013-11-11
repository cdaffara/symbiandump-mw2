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
 * Description:Base class for all effect object classes.
 *
 */

#ifndef TMS_EFFECT_H
#define TMS_EFFECT_H

#include <tms.h>

namespace TMS {

/**
 * TMSEffect class
 *
 * Base class for all effect object classes.
 * This is an abstract class.
 *
 * @lib tmsapi.lib
 *
 */
class TMSEffect
    {
public:
    /**
     * Return effect type.
     *
     * @param  effecttype
     *      Type of the effect object.
     *
     * @return
     *      The status of the operation.
     *
     */
    virtual gint GetType(TMSEffectType& effecttype) = 0;
    };

} //namespace TMS

#endif //TMS_EFFECT_H

// End of file
