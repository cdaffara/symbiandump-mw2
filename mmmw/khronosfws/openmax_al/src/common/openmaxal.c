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
 * Description:  Entry Point to the OpenMAX AL Core from the Client 
 *               Application. Implementation of the OpenMAX AL Global
 *               Functions.
 *              
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "openmaxalwrapper.h"
#include "openmaxal_iid.c"
#include "xanokialinearvolume_iid.c"
#include "xanokiavolumeext_iid.c"

#include "xaglobals.h"
#include "xaobjects.h"

EXPORT_C XAresult XAAPIENTRY xaCreateEngine(XAObjectItf *pEngine,
        XAuint32 numOptions, const XAEngineOption *pEngineOptions,
        XAuint32 numInterfaces, const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired)
    {
    XAresult ret;
    DEBUG_API("->xaCreateEngine");
    ret = XAEngineImpl_Create(pEngine, numOptions, pEngineOptions,
            numInterfaces, pInterfaceIds, pInterfaceRequired);
    DEBUG_API_A1("<-xaCreateEngine: 0x%x",(int)ret);
    return ret;
    }

EXPORT_C XAresult XAAPIENTRY xaQueryNumSupportedEngineInterfaces(
        XAuint32 *pNumSupportedInterfaces)
    {
    XAresult ret;
    DEBUG_API("->xaQueryNumSupportedEngineInterfaces");
    ret = XAEngineImpl_QueryNumSupportedInterfaces(pNumSupportedInterfaces);
    DEBUG_API_A1("<-xaQueryNumSupportedEngineInterfaces: 0x%x",(int)ret);
    return ret;
    }

EXPORT_C XAresult XAAPIENTRY xaQuerySupportedEngineInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId)
    {
    XAresult ret;
    DEBUG_API("->xaQuerySupportedEngineInterfaces");
    ret = XAEngineImpl_QuerySupportedInterfaces(index, pInterfaceId);
    DEBUG_API_A1("<-xaQuerySupportedEngineInterfaces: 0x%x",(int)ret);
    return ret;
    }
