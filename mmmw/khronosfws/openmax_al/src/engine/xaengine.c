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
 * Description: Engine Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xaengine.h"
#include "xaobjectitf.h"
#include "xaengineitf.h"
#include "xathreadsyncitf.h"
#include "xadynintmgmtitf.h"
#include "xaaudioiodevicecapabilitiesitf.h"
#include "xaaudioencodercapabilitiesitf.h"
#include "xathreadsafety.h"
#include "xaframeworkmgr.h"  

/* Static mapping of enumeration XAEngineInterfaces to interface iids */
static const XAInterfaceID* xaEngineItfIIDs[ENGINE_ITFCOUNT] =
    {
    &XA_IID_OBJECT,
    &XA_IID_ENGINE,
    &XA_IID_DYNAMICINTERFACEMANAGEMENT,
    &XA_IID_THREADSYNC,
    &XA_IID_AUDIOIODEVICECAPABILITIES,
    &XA_IID_AUDIOENCODERCAPABILITIES
    };

/*****************************************************************************
 * Global methods
 *****************************************************************************/

/* XAResult XAEngineImpl_Create
 * Description: Create object
 */
XAresult XAEngineImpl_Create(XAObjectItf *pEngine, XAuint32 numOptions,
        const XAEngineOption *pEngineOptions, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired)
    {
    XAEngineImpl* pImpl = NULL;
    XAObjectItfImpl* pBaseObj = NULL;
    XAuint32 itfIndex = 0;
    //XAboolean optEnabled = XA_BOOLEAN_FALSE;
    XAboolean threadSafeEnabled = XA_BOOLEAN_TRUE;
    DEBUG_API("->XAEngineImpl_Create");

    if (!pEngine)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAEngineImpl_Create");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* check engine options */
    if (pEngineOptions && (numOptions != 0))
        {
        int i;
        for (i = 0; i < numOptions; i++)
            {
            if (pEngineOptions[i].feature == XA_ENGINEOPTION_LOSSOFCONTROL)
                {
                //XAboolean optEnabled = pEngineOptions[i].data;
                // TODO: do something with the above value or else remove the code
                }
            if (pEngineOptions[i].feature == XA_ENGINEOPTION_THREADSAFE)
                {
                threadSafeEnabled = pEngineOptions[i].data;
                }
            }
        }

    /* instantiate object implementation */
    pImpl = (XAEngineImpl*) calloc(1, sizeof(XAEngineImpl));
    if (!pImpl)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAEngineImpl_Create");
        /* memory allocation failed */
        return XA_RESULT_MEMORY_FAILURE;
        }
    pBaseObj = &pImpl->baseObj;

    /* Initialize base object default implementation */
    XAObjectItfImpl_Init(pBaseObj, ENGINE_ITFCOUNT, xaEngineItfIIDs,
            XAEngineImpl_DoRealize, XAEngineImpl_DoResume,
            XAEngineImpl_FreeResources);

    /* Mark interfaces that need to be exposed */
    /* Implicit and mandated interfaces */
    pBaseObj->interfaceMap[ENGINE_ENGINEITF].required = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[ENGINE_DIMITF].required = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[ENGINE_THREADSYNCITF].required = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[ENGINE_AUDIOIODEVICECAPAITF].required
            = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[ENGINE_AUDIOENCODERCAPAITF].required
            = XA_BOOLEAN_TRUE;
    /* Explicit interfaces */
    if ((numInterfaces != 0) && pInterfaceIds && pInterfaceRequired)
        {
        /* Check required interfaces */
        for (itfIndex = 0; itfIndex < numInterfaces; itfIndex++)
            {
            /* If mapEntry is null then required interface is not supported.*/
            XAObjItfMapEntry *entry = XAObjectItfImpl_GetItfEntry(
                    (XAObjectItf) &(pBaseObj), pInterfaceIds[itfIndex]);
            if (!entry)
                {
                if (pInterfaceRequired[itfIndex])
                    {
                    /* required interface cannot be accommodated - fail creation */
                    XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
                    DEBUG_ERR("Required interface not found - abort creation!");
                    DEBUG_API("<-XAEngineImpl_Create");
                    return XA_RESULT_FEATURE_UNSUPPORTED;
                    }
                else
                    {
                    DEBUG_INFO("Requested (not required) interface not found - continue creation");
                    }
                }
            else
                {
                entry->required = XA_BOOLEAN_TRUE;
                }
            }
        }

    /* Initialize XAEngineImpl variables */
    if (threadSafeEnabled)
        {
        XAresult ret = XA_RESULT_SUCCESS;
        pImpl->isThreadSafe = threadSafeEnabled;
        ret = XAThreadSafety_Init(threadSafeEnabled);
        if (ret != XA_RESULT_SUCCESS)
            {
            DEBUG_INFO_A1("Unable to initialize thread safety - ret:%x", ret );
            }
        }

    /* Set ObjectItf to point to newly created object */
    *pEngine = ((XAObjectItf) &(pBaseObj->self));

    DEBUG_API("<-XAEngineImpl_Create");
    return XA_RESULT_SUCCESS;
    }

/* XAResult XAEngineImpl_QueryNumSupportedInterfaces
 * Description: Statically query number of supported interfaces
 */
XAresult XAEngineImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces)
    {
    DEBUG_API("->XAEngineImpl_QueryNumSupportedInterfaces");
    if (pNumSupportedInterfaces)
        {
        *pNumSupportedInterfaces = ENGINE_ITFCOUNT;
        DEBUG_API("<-XAEngineImpl_QueryNumSupportedInterfaces");
        return XA_RESULT_SUCCESS;
        }
    else
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAEngineImpl_QueryNumSupportedInterfaces");
        return XA_RESULT_PARAMETER_INVALID;
        }
    }

/* XAResult XAEngineImpl_QuerySupportedInterfaces
 * Description: Statically query supported interfaces
 */
XAresult XAEngineImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId)
    {
    DEBUG_API("->XAEngineImpl_QuerySupportedInterfaces");
    if (index >= ENGINE_ITFCOUNT || !pInterfaceId)
        {
        if (pInterfaceId)
            {
            *pInterfaceId = XA_IID_NULL;
            }
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAEngineImpl_QuerySupportedInterfaces");
        return XA_RESULT_PARAMETER_INVALID;
        }
    else
        {
        *pInterfaceId = *(xaEngineItfIIDs[index]);
        DEBUG_API("<-XAEngineImpl_QuerySupportedInterfaces");
        return XA_RESULT_SUCCESS;
        }
    }

/*****************************************************************************
 * base object XAObjectItfImpl methods
 *****************************************************************************/

/* XAresult XAEngineImpl_DoRealize
 * Description: Realize all implicit and explicitly wanted interfaces.
 * Create and initialize implementation-specific variables.
 * Called from base object XAObjectItfImpl
 */
XAresult XAEngineImpl_DoRealize(XAObjectItf self)
    {
    XAuint8 itfIdx = 0;
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAEngineImpl* pObjImpl = (XAEngineImpl*) (pObj);
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("->XAEngineImpl_DoRealize");

    /* check casting from correct pointer type */
    if (!pObjImpl || pObj != pObjImpl->baseObj.self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAEngineImpl_DoRealize");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* Table containing use-case framework map */
    pObjImpl->frameworkMap = XAFrameworkMgr_CreateFrameworkMap();
    if (pObjImpl->frameworkMap == NULL)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAEngineImpl_DoRealize");
        /* memory allocation failed */
        return XA_RESULT_MEMORY_FAILURE;
        }

    /* Create capabilities list */
    ret = XACapabilitiesMgr_CreateCapabilitieList(pObjImpl->frameworkMap,
            &(pObjImpl->capabilities));
    if (ret != XA_RESULT_SUCCESS)
        {
        return ret;
        }

    /* Realize all implicit and explicitly wanted interfaces */
    for (itfIdx = 0; itfIdx < ENGINE_ITFCOUNT; itfIdx++)
        {
        if (!(pObj->interfaceMap[itfIdx].pItf)
                && pObj->interfaceMap[itfIdx].required)
            {
            void *pItf = NULL;
            switch (itfIdx)
                {
                case ENGINE_ENGINEITF:
                    pItf = XAEngineItfImpl_Create(pObjImpl->frameworkMap,
                            pObjImpl->capabilities);
                    break;
                case ENGINE_THREADSYNCITF:
                    pItf = XAThreadSyncItfImpl_Create();
                    break;
                case ENGINE_DIMITF:
                    pItf = XADIMItfImpl_Create();
                    break;
                case ENGINE_AUDIOIODEVICECAPAITF:
                    pItf = XAAudIODevCapaItfImpl_Create(
                            pObjImpl->capabilities);
                    break;
                case ENGINE_AUDIOENCODERCAPAITF:
                    pItf = XAAudioEncoderCapabilitiesItfImpl_Create(
                            pObjImpl->capabilities);
                    break;
                default:
                    break;
                }
            if (!pItf)
                {
                DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
                DEBUG_API("<-XAEngineImpl_DoRealize");
                /* memory allocation failed */
                return XA_RESULT_MEMORY_FAILURE;
                }
            else
                {
                pObj->interfaceMap[itfIdx].pItf = pItf;
                }
            }
        }

    pObj->state = XA_OBJECT_STATE_REALIZED;
    DEBUG_API("<-XAEngineImpl_DoRealize");
    return XA_RESULT_SUCCESS;
    }

/* XAresult XAEngineImpl_DoResume
 * Description: Resume object from suspended state
 */
XAresult XAEngineImpl_DoResume(XAObjectItf self)
    {
    DEBUG_API("->XAEngineImpl_DoResume");
    DEBUG_API("<-XAEngineImpl_DoResume");

    return XA_RESULT_PRECONDITIONS_VIOLATED;
    }

/* void XAEngineImpl_FreeResources
 * Description: Free all resources reserved at XA%ExampleObject%Impl_DoRealize()
 */
void XAEngineImpl_FreeResources(XAObjectItf self)
    {
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAEngineImpl* pImpl = (XAEngineImpl*) (*self);
    XAuint8 itfIdx = 0;
    DEBUG_API("->XAEngineImpl_FreeResources");

    /* free all allocated interfaces */
    for (itfIdx = 0; itfIdx < ENGINE_ITFCOUNT; itfIdx++)
        {
        void *pItf = pObj->interfaceMap[itfIdx].pItf;
        if (pItf)
            {
            switch (itfIdx)
                {
                case ENGINE_ENGINEITF:
                    XAEngineItfImpl_Free(pItf);
                    break;
                case ENGINE_THREADSYNCITF:
                    XAThreadSyncItfImpl_Free(pItf);
                    break;
                case ENGINE_DIMITF:
                    XADIMItfImpl_Free(pItf);
                    break;
                case ENGINE_AUDIOIODEVICECAPAITF:
                    XAAudIODevCapaItfImpl_Free(pItf);
                    break;
                case ENGINE_AUDIOENCODERCAPAITF:
                    XAAudioEncoderCapabilitiesItfImpl_Free(pItf);
                    break;
                default:
                    break;
                }
            pObj->interfaceMap[itfIdx].pItf = NULL;
            }
        }

    /* free all other allocated resources*/

    /* free framework map */
    XAFrameworkMgr_DeleteFrameworkMap(&pImpl->frameworkMap);

    /* free capabilities list */
    XACapabilitiesMgr_DeleteCapabilitieList(&pImpl->capabilities);

    XAThreadSafety_Destroy();

    DEBUG_API("<-XAEngineImpl_FreeResources");
    return;
    }

/* END OF FILE */
