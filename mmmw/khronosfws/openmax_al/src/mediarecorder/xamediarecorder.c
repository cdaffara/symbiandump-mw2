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
 * Description: MediaRecoder Object Impl
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xamediarecorder.h"
#include "xaaudioencoderitf.h"
#include "xadynintmgmtitf.h"
#include "xarecorditf.h"
#include "xathreadsafety.h"
#include "xacapabilitiesmgr.h"
#include "xamediarecorderadaptctxmmf.h"
#include "xamediarecorderadaptctx.h"
/* Static mapping of enumeration XAMediaRecorderInterfaces to interface iids */
static const XAInterfaceID* xaMediaRecorderItfIIDs[MR_ITFCOUNT] =
    {
    &XA_IID_OBJECT,
    &XA_IID_AUDIOENCODER,
    &XA_IID_DYNAMICINTERFACEMANAGEMENT,
    &XA_IID_RECORD
    };

/* Global methods */

/* XAResult XAMediaRecorderImpl_Create
 * Create object
 */
XAresult XAMediaRecorderImpl_CreateMediaRecorder(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf* pRecorder,
        XADataSource* pAudioSrc, XADataSource* pImageVideoSrc,
        XADataSink* pDataSnk, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired)
    {
    XAMediaRecorderImpl* pImpl = NULL;
    XAObjectItfImpl* pBaseObj = NULL;
    FWMgrFwType fwType;
    XAuint8 itfIdx;
    XAMediaType mediaType = XA_MEDIATYPE_UNKNOWN;
    XAresult ret = XA_RESULT_SUCCESS;
    const char *uri = NULL;

    DEBUG_API("->XAMediaRecorderImpl_CreateMediaRecorder");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaRecorder );

    /* check sink&source parameters */
    ret = XACommon_ValidateDataLocator(3, pAudioSrc, pImageVideoSrc,
                    pDataSnk);
    if (ret != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
        return ret;
        }
    
    /* check only sink media type, do not care about return value (availability) */
    XACommon_CheckDataSource((XADataSource*) pDataSnk, &mediaType);
    
    if (!pRecorder || (!pAudioSrc && !pImageVideoSrc) || !pDataSnk)
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* instantiate object implementation */
    pImpl = (XAMediaRecorderImpl*) calloc(1, sizeof(XAMediaRecorderImpl));
    if (!pImpl)
        {
        /* memory allocation failed */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
        return XA_RESULT_MEMORY_FAILURE;
        }
    
    pBaseObj = &pImpl->baseObj;

    /* Initialize base object default implementation */
    XAObjectItfImpl_Init(pBaseObj, MR_ITFCOUNT, xaMediaRecorderItfIIDs,
            XAMediaRecorderImpl_DoRealize, XAMediaRecorderImpl_DoResume,
            XAMediaRecorderImpl_FreeResources);
        
    /* Mark interfaces that need to be exposed */
    /* Mandated Implicit interfaces */
    pBaseObj->interfaceMap[MR_DIMITF].required = XA_BOOLEAN_TRUE;

    /* Mark interfaces that can be handled dynamically */
    /* Mandated dynamic itfs */

    /*Mandated  Explicit interfaces */
    if ((numInterfaces != 0) && pInterfaceIds && pInterfaceRequired)
        {
        /* Check required interfaces */
        for (itfIdx = 0; itfIdx < numInterfaces; itfIdx++)
            {
            /* If mapEntry is null then required interface is not supported.*/
            XAObjItfMapEntry *entry = XAObjectItfImpl_GetItfEntry(
                    (XAObjectItf) &(pBaseObj), pInterfaceIds[itfIdx]);
            if (!entry)
                {
                if (pInterfaceRequired[itfIdx])
                    {
                    /* required interface cannot be accommodated - fail creation */
                    DEBUG_ERR("Required interface not found - abort creation!");
                    ret = XA_RESULT_FEATURE_UNSUPPORTED;
                    break;
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

    if (ret != XA_RESULT_SUCCESS)
        { /* creation fails */
        XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
        return ret;
        }
    
    /* Initialize XAMediaRecorderImpl variables */
    pImpl->audioSrc = pAudioSrc;
    pImpl->dataSnk = pDataSnk;
    pImpl->imageVideoSrc = pImageVideoSrc;
    pImpl->recModes |= XA_RECMODE_STREAM;

    /* Determine framework type that can handle recording */
    fwType = (FWMgrFwType) FWMgrMOUnknown;
    /**/
    if (pDataSnk->pLocator)
        {
        XADataLocator_URI* dataLoc = (XADataLocator_URI*) pDataSnk->pLocator;
        if (dataLoc->locatorType == XA_DATALOCATOR_URI)
            {
            uri = (char*) dataLoc->URI;
            }
        }
    
    fwType = XAFrameworkMgr_GetFramework(mapper, uri, FWMgrMORecorder);

    switch(fwType)
        {
        case FWMgrFWMMF:
            {

            pImpl->adaptationCtx = XAMediaRecorderAdaptMMF_Create(
                    pImpl->audioSrc, pImpl->imageVideoSrc, pImpl->dataSnk,
                    pImpl->recModes);
            }
            break;
        case FWMgrFWGST:
            {
            pImpl->adaptationCtx = XAMediaRecorderAdapt_Create(pImpl->audioSrc,
                    pImpl->imageVideoSrc, pImpl->dataSnk, pImpl->recModes);
            break;
            }
        case FWMgrFWUknown:
        default:
            {
            ret = XA_RESULT_CONTENT_UNSUPPORTED;
            break;
            }
        }
    
    if (ret != XA_RESULT_SUCCESS)
        { /* creation fails */
        XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
        return ret;
        }
    
    if (pImpl->adaptationCtx)
        {
        pImpl->adaptationCtx->capslist = capabilities;
        pImpl->adaptationCtx->fwtype = fwType;
        }
    else
        {
        ret = XA_RESULT_MEMORY_FAILURE;
        XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
        return ret;
        }

    /* Set ObjectItf to point to newly created object */
    *pRecorder = (XAObjectItf) &(pBaseObj->self);
    
    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
    DEBUG_API("<-XAMediaRecorderImpl_CreateMediaRecorder");
    return XA_RESULT_SUCCESS;
    }

/* XAResult XAMediaRecorderImpl_QueryNumSupportedInterfaces
 * Statically query supported interfaces
 */
XAresult XAMediaRecorderImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces)
    {
    DEBUG_API("->XAMediaRecorderImpl_QueryNumSupportedInterfaces");
    *pNumSupportedInterfaces = MR_ITFCOUNT;
    DEBUG_API("<-XAMediaRecorderImpl_QueryNumSupportedInterfaces");
    return XA_RESULT_SUCCESS;
    }
/* XAResult XAMediaRecorderImpl_QuerySupportedInterfaces
 * Statically query supported interfaces
 */
XAresult XAMediaRecorderImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId)
    {
    DEBUG_API("->XAMediaRecorderImpl_QuerySupportedInterfaces");
    if (index >= MR_ITFCOUNT)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        return XA_RESULT_PARAMETER_INVALID;
        }
    else
        {
        *pInterfaceId = *(xaMediaRecorderItfIIDs[index]);
        DEBUG_API("<-XAMediaRecorderImpl_QuerySupportedInterfaces");
        return XA_RESULT_SUCCESS;
        }
    }

/*****************************************************************************
 * base object XAObjectItfImpl methods
 *****************************************************************************/

/* XAresult XAMediaRecorderImpl_DoRealize( XAObjectItf self )
 * Description: Realize all implicit and explicitly wanted interfaces.
 * Create and initialize implementation-specific variables.
 * Called from base object XAObjectItfImpl
 */
XAresult XAMediaRecorderImpl_DoRealize(XAObjectItf self)
    {
    XAuint8 itfIdx;
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAMediaRecorderImpl* pObjImpl = (XAMediaRecorderImpl*) (pObj);
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMediaRecorderImpl_DoRealize");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaRecorder );

    /* check casting from correct pointer type */
    if (!pObjImpl || pObj != pObjImpl->baseObj.self)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        return XA_RESULT_PARAMETER_INVALID;
        }
 
    /* Realize all implicit and explicitly wanted interfaces */
    for (itfIdx = 0; itfIdx < MR_ITFCOUNT; itfIdx++)
        {
        if (!(pObj->interfaceMap[itfIdx].pItf)
                && pObj->interfaceMap[itfIdx].required)
            {
            void *pItf = NULL;
            switch (itfIdx)
                {
                case MR_DIMITF:
                    pItf = XADIMItfImpl_Create();
                    if (pItf)
                        {
                        XADIMItfImpl_Init(pItf, self,
                                XAMediaRecorderImpl_DoAddItf,
                                XAMediaRecorderImpl_DoResumeItf,
                                XAMediaRecorderImpl_DoRemoveItf);
                        }
                    break;
                case MR_RECORDITF:
                    pItf = XARecordItfImpl_Create(pObjImpl);
                    break;
                case MR_AUDIOENCODERITF:
                    pItf = XAAudioEncoderItfImpl_Create(pObjImpl);
                    break;
                default:
                    break;
                }
            if (!pItf)
                {
                DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
                /* memory allocation failed */
                XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
                return XA_RESULT_MEMORY_FAILURE;
                }
            else
                {
                pObj->interfaceMap[itfIdx].pItf = pItf;
                }
            }
        }

    /* init adaptation */
    if (pObjImpl->adaptationCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAMediaRecorderAdaptMMF_PostInit(
                (XAAdaptationMMFCtx*) pObjImpl->adaptationCtx);
        }
    else
        {
        ret = XAMediaRecorderAdapt_PostInit(
                (XAAdaptationGstCtx*) pObjImpl->adaptationCtx);
        }

    if (ret != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        return ret;
        }

    XAAdaptationBase_AddEventHandler(pObjImpl->adaptationCtx,
            &XAMediaRecorderImpl_AdaptCb, XA_OBJECTITFEVENTS, pObjImpl);
    
    pObj->state = XA_OBJECT_STATE_REALIZED;
    DEBUG_API("<-XAMediaRecorderImpl_DoRealize");
    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
    return XA_RESULT_SUCCESS;
    }

/* XAresult XAMediaRecorderImpl_DoResume
 * Description: Resume object from suspended state
 */
XAresult XAMediaRecorderImpl_DoResume(XAObjectItf self)
    {
    DEBUG_API("->XAMediaRecorderImpl_DoResume");
    DEBUG_API("<-XAMediaRecorderImpl_DoResume");
    /* This implementation does not support suspended state */
    return XA_RESULT_PRECONDITIONS_VIOLATED;
    }

/* void XAMediaRecorderImpl_FreeResources
 * Description: Free all resources reserved at XAMediaRecorderImpl_DoRealize()
 */
void XAMediaRecorderImpl_FreeResources(XAObjectItf self)
    {
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAMediaRecorderImpl* pObjImpl = (XAMediaRecorderImpl*) (pObj);
    XAuint8 itfIdx;
    XAMediaRecorderImpl* pImpl = (XAMediaRecorderImpl*) (*self);
    DEBUG_API("->XAMediaRecorderImpl_FreeResources");
    XA_IMPL_THREAD_SAFETY_ENTRY_FOR_VOID_FUNCTIONS( XATSMediaRecorder );

    /* free all allocated interfaces */
    for (itfIdx = 0; itfIdx < MR_ITFCOUNT; itfIdx++)
        {
        void *pItf = pObj->interfaceMap[itfIdx].pItf;
        if (pItf)
            {
            switch (itfIdx)
                {
                case MR_AUDIOENCODERITF:
                    XAAudioEncoderItfImpl_Free(pItf);
                    break;
                case MR_DIMITF:
                    XADIMItfImpl_Free(pItf);
                    break;
                case MR_RECORDITF:
                    XARecordItfImpl_Free(pItf);
                    break;
                default:
                    break;
                }
            pObj->interfaceMap[itfIdx].pItf = NULL;
            }
        }

    if (pImpl->adaptationCtx != NULL)
        {
        if (pImpl->adaptationCtx->fwtype == FWMgrFWMMF)
            {
            XAMediaRecorderAdaptMMF_Destroy(
                    (XAAdaptationMMFCtx*) pObjImpl->adaptationCtx);
            }
        else
            {
            XAMediaRecorderAdapt_Destroy(
                    (XAAdaptationGstCtx*) pImpl->adaptationCtx);
            }
        pImpl->adaptationCtx = NULL;
        }

    /* free all other allocated resources*/

    DEBUG_API("<-XAMediaRecorderImpl_FreeResources");
    XA_IMPL_THREAD_SAFETY_EXIT_FOR_VOID_FUNCTIONS( XATSMediaRecorder );
    return;
    }

/* void XARecordItfImpl_AdaptCb
 * Description: Listen changes in adaptation
 */
void XAMediaRecorderImpl_AdaptCb(void *pHandlerCtx, XAAdaptEvent *event)
    {
    XAObjectItfImpl* impl = (XAObjectItfImpl*) pHandlerCtx;
    DEBUG_API("->XAMediaRecorderImpl_AdaptCb");

    if (!impl)
        {
        DEBUG_ERR("XAMediaRecorderImpl_AdaptCb, invalid context pointer!");
        DEBUG_API("<-XAMediaRecorderImpl_AdaptCb");
        return;
        }

    if(impl->callBack)
        {
        if(event)
            {
            XAObjItfCBData* data = (XAObjItfCBData*)(event->data);
            if(data->event == XA_OBJECT_EVENT_RESOURCES_LOST )
                {
                impl->state = data->param;
                }
            impl->callBack(impl->cbPtrToSelf, impl->context, data->event, data->result, data->param, data->interface);
            }
        }
    
    DEBUG_API("<-XAMediaRecorderImpl_AdaptCb");
    }


/*****************************************************************************
 * MediaRecorderImpl -specific methods
 *****************************************************************************/

/* XAMediaRecorderImpl_DoAddItf
 * Dynamically add an interface, object specific parts
 */
XAresult XAMediaRecorderImpl_DoAddItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMediaRecorderImpl_DoAddItf");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaRecorder );

    if (mapEntry)
        {
        switch (mapEntry->mapIdx)
            {
            default:
                DEBUG_ERR("XAMediaRecorderImpl_DoAddItf unknown id");
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                break;
            }

        if (!mapEntry->pItf && ret == XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("XAMediaRecorderImpl_DoAddItf itf creation failed");
            ret = XA_RESULT_MEMORY_FAILURE;
            }
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
    DEBUG_API("<-XAMediaRecorderImpl_DoAddItf");
    return ret;
    }

/* XAMediaRecorderImpl_DoResumeItf
 * Try to resume lost interface, object specific parts
 */
XAresult XAMediaRecorderImpl_DoResumeItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret;
    /* For now, no difference between suspended and unrealised itfs */
    DEBUG_API("->XAMediaRecorderImpl_DoResumeItf");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaRecorder );
    ret = XAMediaRecorderImpl_DoAddItf(self, mapEntry);
    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
    DEBUG_API("<-XAMediaRecorderImpl_DoResumeItf");
    return ret;
    }

/* XAMediaRecorderImpl_DoRemoveItf
 * Dynamically remove an interface, object specific parts
 */
XAresult XAMediaRecorderImpl_DoRemoveItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMediaRecorderImpl_DoRemoveItf");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaRecorder );
    if (mapEntry)
        {
        switch (mapEntry->mapIdx)
            {
            default:
                DEBUG_ERR("XAMediaRecorderImpl_DoRemoveItf unknown id");
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                break;
            }
        mapEntry->pItf = NULL;
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
    DEBUG_API("<-XAMediaRecorderImpl_DoRemoveItf");
    return ret;
    }

/* END OF FILE */
