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

#include <tmspcmformat.h>
#include "tmspcmformatbody.h"
#include "tmsutility.h"

using namespace TMS;

EXPORT_C TMSPCMFormat::TMSPCMFormat() :
    iBody(NULL)
    {
    }

EXPORT_C TMSPCMFormat::~TMSPCMFormat()
    {
    // iBody is deleted with TMSFormat::iFormatBody
    }

// End of file
