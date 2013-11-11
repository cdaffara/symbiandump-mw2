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

#include "tmsvolumeeffectimpl.h"
#include "tmsgaineffectimpl.h"
#include "tmsglobalvoleffectimpl.h"
#include "tmsglobalgaineffectimpl.h"
#include "tmsutility.h"
#include "tmseffectimpl.h"

using namespace TMS;

EXPORT_C gint TMSEffectImpl::Create(TMSEffectType effecttype,
        TMSEffect*& tmseffect)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    switch (effecttype)
        {
        case TMS_EFFECT_VOLUME:
            ret = TMSVolumeEffectImpl::Create(tmseffect);
            break;
        case TMS_EFFECT_GAIN:
            ret = TMSGainEffectImpl::Create(tmseffect);
            break;
        case TMS_EFFECT_GLOBAL_VOL:
            ret = TMSGlobalVolEffectImpl::Create(tmseffect);
            break;
        case TMS_EFFECT_GLOBAL_GAIN:
            ret = TMSGlobalGainEffectImpl::Create(tmseffect);
            break;
        default:
            ret = TMS_RESULT_EFFECT_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSEffectImpl::Delete(TMSEffect*& tmseffect)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    TRACE_PRN_FN_ENT;
    TMSEffectType effecttype;
    ret = tmseffect->GetType(effecttype);
    switch (effecttype)
        {
        case TMS_EFFECT_VOLUME:
            delete (static_cast<TMSVolumeEffectImpl*> (tmseffect));
            tmseffect = NULL;
            break;
        case TMS_EFFECT_GAIN:
            delete (static_cast<TMSGainEffectImpl*> (tmseffect));
            tmseffect = NULL;
            break;
        case TMS_EFFECT_GLOBAL_VOL:
            delete (static_cast<TMSGlobalVolEffectImpl*> (tmseffect));
            tmseffect = NULL;
            break;
        case TMS_EFFECT_GLOBAL_GAIN:
            delete (static_cast<TMSGlobalGainEffectImpl*> (tmseffect));
            tmseffect = NULL;
            break;
        default:
            ret = TMS_RESULT_EFFECT_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

// End of file
