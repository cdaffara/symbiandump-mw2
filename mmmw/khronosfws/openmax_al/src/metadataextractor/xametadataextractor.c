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
 * Description: Metadata Extractor Object Impl
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xametadataextractor.h"
#include "xadynamicsourceitf.h"
#include "xadynintmgmtitf.h"
#include "xametadataextractionitf.h"

#include "xacapabilitiesmgr.h"
#include "xathreadsafety.h"

#include "xaadaptationmmf.h"
#include "xametadataadaptctxmmf.h"

/* Static mapping of enumeration XAMetadataInterfaces to interface iids */
static const XAInterfaceID* xaMetadataExtractorItfIIDs[MDE_ITFCOUNT] =
    {
    &XA_IID_OBJECT,
    &XA_IID_DYNAMICINTERFACEMANAGEMENT,
    &XA_IID_METADATAEXTRACTION,
    &XA_IID_DYNAMICSOURCE
    };

/*****************************************************************************
 * Global methods
 *****************************************************************************/

/* XAResult XAMetadataExtractorImpl_Create
 * Description: Create object
 * Add this method to XAGlobals.h
 */
XAresult XAMetadataExtractorImpl_Create(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf *pMetadataExtractor,
        XADataSource *pDataSource, XAuint32 numInterfaces,
        const XAInterfaceID * pInterfaceIds,
        const XAboolean * pInterfaceRequired)
    {
    XAMetadataExtractorImpl* pImpl = NULL;
    XAObjectItfImpl* pBaseObj = NULL;
    XAuint8 itfIndex = 0;
    FWMgrFwType fwType;
    const char *uri = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMetadataExtractorImpl_Create");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaPlayer);

    if (!pMetadataExtractor)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XAMetadataExtractorImpl_Create");
        return XA_RESULT_PARAMETER_INVALID;
        }

    res = XACommon_CheckDataSource(pDataSource, NULL);
    if (res != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_API("<-XAMetadataExtractorImpl_Create");
        return res;
        }

    /* instantiate object implementation */
    pImpl = (XAMetadataExtractorImpl*) calloc(1,
            sizeof(XAMetadataExtractorImpl));
    if (!pImpl)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        /* memory allocation failed */
        DEBUG_API("<-XAMetadataExtractorImpl_Create");
        return XA_RESULT_MEMORY_FAILURE;
        }
    pBaseObj = &pImpl->baseObj;

    /* Initialize base object default implementation */
    XAObjectItfImpl_Init(pBaseObj, MDE_ITFCOUNT, xaMetadataExtractorItfIIDs,
            XAMetadataExtractorImpl_DoRealize,
            XAMetadataExtractorImpl_DoResume,
            XAMetadataExtractorImpl_FreeResources);

    /* Mark interfaces that need to be exposed */
    /* Implicit and mandated interfaces */
    pBaseObj->interfaceMap[MDE_METADATAEXTRACTIONITF].required
            = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[MDE_DYNAMICSOURCEITF].required = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[MDE_DIMITF].required = XA_BOOLEAN_TRUE;

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
                    XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
                    DEBUG_ERR("Required interface not found - abort creation!");
                    DEBUG_API("<-XAMetadataExtractorImpl_Create");
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

    // Mark interfaces that can be handled dynamically 
    //Set ObjectItf to point to newly created object 
    *pMetadataExtractor = (XAObjectItf) &(pBaseObj->self);

    //store member variables
    pImpl->dataSrc = pDataSource;

    /* Determine framework type that can handle recording */
    fwType = (FWMgrFwType) FWMgrMOUnknown;
    /**/
    if (pDataSource->pLocator)
        {
        XADataLocator_URI* dataLoc =
                (XADataLocator_URI*) pDataSource->pLocator;
        if (dataLoc->locatorType == XA_DATALOCATOR_URI)
            {
            uri = (char*) dataLoc->URI;
            }
        }

    fwType = XAFrameworkMgr_GetFramework(mapper, uri, FWMgrMOPlayer);

    if (fwType == FWMgrMOUnknown)
        {
        XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_API("<-XAMetadataExtractorImpl_Create");
        return XA_RESULT_CONTENT_UNSUPPORTED;
        }

    if (fwType == FWMgrFWMMF)
        {
        pImpl->adaptationCtxMMF = XAMetadataAdaptCtxMMF_Create(pDataSource);
        pImpl->curAdaptCtx = pImpl->adaptationCtxMMF;
        }

    XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
    DEBUG_API("<-XAMetadataExtractorImpl_Create");
    return XA_RESULT_SUCCESS;
    }

/* XAResult XAMetadataExtractorImpl_QueryNumSupportedInterfaces
 * Description: Statically query number of supported interfaces
 */
XAresult XAMetadataExtractorImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces)
    {
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMetadataExtractorImpl_QueryNumSupportedInterfaces");
    if (pNumSupportedInterfaces)
        {
        *pNumSupportedInterfaces = MDE_ITFCOUNT;
        res = XA_RESULT_SUCCESS;
        }
    else
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
        }
    DEBUG_API_A1("<-XAMetadataExtractorImpl_QueryNumSupportedInterfaces (%d)", (int)res);
    return res;
    }
/* XAResult XAMetadataExtractorImpl_QuerySupportedInterfaces
 * Description: Statically query supported interfaces
 */
XAresult XAMetadataExtractorImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID * pInterfaceId)
    {
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMetadataExtractorImpl_QuerySupportedInterfaces");

    if (index >= MDE_ITFCOUNT || !pInterfaceId)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
        }
    else
        {
        *pInterfaceId = *(xaMetadataExtractorItfIIDs[index]);
        res = XA_RESULT_SUCCESS;
        }
    DEBUG_API_A1("<-XAMetadataExtractorImpl_QuerySupportedInterfaces (%d)", (int)res);
    return res;
    }

/*****************************************************************************
 * base object XAObjectItfImpl methods
 *****************************************************************************/

/* XAresult XAMetadataExtractorImpl_DoRealize( XAObjectItf self )
 * Description: Realize all implicit and explicitly wanted interfaces.
 * Create and initialize implementation-specific variables.
 * Called from base object XAObjectItfImpl
 */
XAresult XAMetadataExtractorImpl_DoRealize(XAObjectItf self)
    {
    XAuint8 itfIdx = 0;
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAMetadataExtractorImpl* pObjImpl = (XAMetadataExtractorImpl*) (pObj);
    void *pItf = NULL;
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMetadataExtractorImpl_DoRealize");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaPlayer);

    /* check casting from correct pointer type */
    if (!pObjImpl || pObj != pObjImpl->baseObj.self)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XAMetadataExtractorImpl_DoRealize");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* Realize all implicit and explicitly wanted interfaces */
    for (itfIdx = 0; itfIdx < MDE_ITFCOUNT; itfIdx++)
        {
        if (!(pObj->interfaceMap[itfIdx].pItf)
                && pObj->interfaceMap[itfIdx].required)
            {
            switch (itfIdx)
                {
                case MDE_DIMITF:
                    pItf = XADIMItfImpl_Create();
                    if (pItf)
                        {
                        XADIMItfImpl_Init(pItf, self,
                                XAMetadataExtractorImpl_DoAddItf,
                                XAMetadataExtractorImpl_DoResumeItf,
                                XAMetadataExtractorImpl_DoRemoveItf);
                        }
                    break;

                case MDE_METADATAEXTRACTIONITF:
                    pItf = XAMetadataExtractionItfImpl_Create(
                            pObjImpl->curAdaptCtx);
                    break;
                case MDE_DYNAMICSOURCEITF:
                    pItf = XADynamicSourceItfImpl_Create(
                            pObjImpl->curAdaptCtx);
                    break;

                default:
                    break;
                }
            if (!pItf)
                {
                XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
                DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
                /* memory allocation failed */
                DEBUG_API("<-XAMetadataExtractorImpl_DoRealize");
                return XA_RESULT_MEMORY_FAILURE;
                }
            else
                {
                pObj->interfaceMap[itfIdx].pItf = pItf;
                }
            }
        }
    /*Initialize adaptation context*/
    /* init adaptation */

    if (pObjImpl->curAdaptCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAMetadataAdaptCtxMMF_PostInit(
                (XAAdaptationMMFCtx*) pObjImpl->adaptationCtxMMF);
        }

    if (ret != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_ERR("Adaptation init failed!");
        DEBUG_API("<-XAMetadataExtractorImpl_DoRealize");
        return ret;
        }
    pObj->state = XA_OBJECT_STATE_REALIZED;
    XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
    DEBUG_API("<-XAMetadataExtractorImpl_DoRealize");
    return XA_RESULT_SUCCESS;
    }

/* XAresult XAMetadataExtractorImpl_DoResume
 * Description: Resume object from suspended state
 */
XAresult XAMetadataExtractorImpl_DoResume(XAObjectItf self)
    {
    DEBUG_API("->XAMetadataExtractorImpl_DoResume");
    DEBUG_API("<-XAMetadataExtractorImpl_DoResume");
    /* "suspended" state not supported by this implementation */
    return XA_RESULT_PRECONDITIONS_VIOLATED;
    }

/* void XAMetadataExtractorImpl_FreeResources
 * Description: Free all resources reserved at XAMetadataExtractorImpl_DoRealize()
 */
void XAMetadataExtractorImpl_FreeResources(XAObjectItf self)
    {
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);

    XAuint8 itfIdx = 0;
    void *pItf = NULL;
    XAMetadataExtractorImpl* pImpl = (XAMetadataExtractorImpl*) (*self);
    DEBUG_API("->XAMetadataExtractorImpl_FreeResources");
    XA_IMPL_THREAD_SAFETY_ENTRY_FOR_VOID_FUNCTIONS(XATSMediaPlayer);

    for (itfIdx = 0; itfIdx < MDE_ITFCOUNT; itfIdx++)
        {
        pItf = pObj->interfaceMap[itfIdx].pItf;
        if (pItf)
            {
            switch (itfIdx)
                {
                case MDE_METADATAEXTRACTIONITF:
                    XAMetadataExtractionItfImpl_Free(pItf);
                    break;
                case MDE_DYNAMICSOURCEITF:
                    XADynamicSourceItfImpl_Free(pItf);
                    break;
                case MDE_DIMITF:
                    XADIMItfImpl_Free(pItf);
                    break;
                }
            pObj->interfaceMap[itfIdx].pItf = NULL;
            }
        }

    if (pImpl->curAdaptCtx)
        {
        if (pImpl->curAdaptCtx->fwtype == FWMgrFWMMF)
            {
            XAMetadataAdaptCtxMMF_Destroy(
                    (XAAdaptationMMFCtx*) pImpl->adaptationCtxMMF);
            pImpl->adaptationCtxMMF = NULL;
            }
        }

    pImpl->curAdaptCtx = NULL;

    XA_IMPL_THREAD_SAFETY_EXIT_FOR_VOID_FUNCTIONS(XATSMediaPlayer);
    DEBUG_API("<-XAMetadataExtractorImpl_FreeResources");
    return;
    }

/*****************************************************************************
 * MetadataExtractorImpl -specific methods
 *****************************************************************************/

/* XAMetadataExtractorImpl_DoAddItf
 * Dynamically add an interface, object specific parts
 */
XAresult XAMetadataExtractorImpl_DoAddItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMetadataExtractorImpl_DoAddItf");

    if (mapEntry)
        {
        switch (mapEntry->mapIdx)
            {
            default:
                DEBUG_ERR("XAMetadataExtractorImpl_DoAddItf unknown id");
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                break;
            }
        if (!mapEntry->pItf && ret == XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("XAMetadataExtractorImpl_DoAddItf itf creation failed");
            ret = XA_RESULT_MEMORY_FAILURE;
            }
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API("<-XAMetadataExtractorImpl_DoAddItf");
    return ret;
    }

/* XAMetadataExtractorImpl_DoResumeItf
 * Try to resume lost interface, object specific parts
 */
XAresult XAMetadataExtractorImpl_DoResumeItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMetadataExtractorImpl_DoResumeItf");
    /* For now, no difference between suspended and unrealised itfs */
    ret = XAMetadataExtractorImpl_DoAddItf(self, mapEntry);
    DEBUG_API("<-XAMetadataExtractorImpl_DoResumeItf");
    return ret;
    }

/* XAMetadataExtractorImpl_DoRemoveItf
 * Dynamically remove an interface, object specific parts
 */
XAresult XAMetadataExtractorImpl_DoRemoveItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMetadataExtractorImpl_DoRemoveItf");
    ret = XA_RESULT_SUCCESS;
    if (mapEntry)
        {
        switch (mapEntry->mapIdx)
            {
            default:
                DEBUG_ERR("XAMetadataExtractorImpl_DoRemoveItf unknown id");
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                break;
            }
        mapEntry->pItf = NULL;
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }
    DEBUG_API("<-XAMetadataExtractorImpl_DoRemoveItf");
    return ret;
    }
/* END OF FILE */
