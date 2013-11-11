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

#include <tms.h>
#include "tmscallipadpt.h"
#include "tmscallcsadpt.h"
#include "tmscalladpt.h"
#include "tmsutility.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSCallAdpt::TMSCallAdpt
//
// -----------------------------------------------------------------------------
//
TMSCallAdpt::TMSCallAdpt()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallAdpt::~TMSCallAdpt
//
// -----------------------------------------------------------------------------
//
TMSCallAdpt::~TMSCallAdpt()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallAdpt::CreateCallL
//
// -----------------------------------------------------------------------------
//
gint TMSCallAdpt::CreateCallL(gint callType, TMSCallAdpt*& callAdpt)
    {
    TRACE_PRN_FN_ENT;
    gint retVal(TMS_RESULT_SUCCESS);
    TMSCallAdpt* self(NULL);

    switch (callType)
        {
        case TMS_CALL_IP:
            retVal = TMS_RESULT_INSUFFICIENT_MEMORY;
            self = TMSCallIPAdpt::NewL();
            if (self)
                {
                retVal = self->PostConstruct();
                self->iCallType = callType;
                if (retVal != TMS_RESULT_SUCCESS)
                    {
                    delete self;
                    self = NULL;
                    }
                }
            break;

        case TMS_CALL_CS:
            retVal = TMS_RESULT_INSUFFICIENT_MEMORY;
            self = TMSCallCSAdpt::NewL();
            if (self)
                {
                retVal = self->PostConstruct();
                self->iCallType = callType;
                if (retVal != TMS_RESULT_SUCCESS)
                    {
                    delete self;
                    self = NULL;
                    }
                }
            break;
        default:
            retVal = TMS_RESULT_CALL_TYPE_NOT_SUPPORTED;
            break;
        }
    callAdpt = self;
    TRACE_PRN_FN_EXT;
    return retVal;
    }

// -----------------------------------------------------------------------------
// TMSCallAdpt::GetCallType
//
// -----------------------------------------------------------------------------
//
void TMSCallAdpt::GetCallType(TMSCallType& callType)
    {
    callType = iCallType;
    }

// -----------------------------------------------------------------------------
// TMSCallAdpt::IsUplActivated
//
// -----------------------------------------------------------------------------
//
gboolean TMSCallAdpt::IsUplActivated()
    {
    return (iUplState == EActivated)? TRUE : FALSE;
    }

