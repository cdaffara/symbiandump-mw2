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
 * Description: Media Player Object Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xamediaplayer.h"
#include "xaplayitf.h"
#include "xaseekitf.h"
#include "xaprefetchstatusitf.h"
#include "xadynintmgmtitf.h"
#include "xavolumeitf.h"
#include "xametadataextractionitf.h"
#include "xaplaybackrateitf.h"
#include "xathreadsafety.h"
#include "xacapabilitiesmgr.h"
#include "xadynamicsourceitf.h"
#include "xastreaminformationitf.h"
#include "xanlinearvolumeitf.h"
#include "xavideopostprocessingitf.h"
#include "xanvolumeextitf.h"

extern void* vfHandle;

/* Static mapping of enumeration XAMediaPlayerInterfaces to interface iids */
static const XAInterfaceID* xaMediaPlayerItfIIDs[MP_ITFCOUNT] =
    {
    &XA_IID_OBJECT,
    &XA_IID_DYNAMICINTERFACEMANAGEMENT,
    &XA_IID_PLAY,
    &XA_IID_SEEK,
    &XA_IID_VOLUME,
    &XA_IID_PREFETCHSTATUS,
    &XA_IID_DYNAMICSOURCE,
    &XA_IID_METADATAEXTRACTION,
    &XA_IID_PLAYBACKRATE,
    &XA_IID_NOKIAVOLUMEEXT,
    &XA_IID_NOKIALINEARVOLUME,
    &XA_IID_STREAMINFORMATION,
    &XA_IID_VIDEOPOSTPROCESSING
    };

/* Global methods */

/* XAResult XAMediaPlayerImpl_CreateMediaPlayer
 * Create object
 */
XAresult XAMediaPlayerImpl_CreateMediaPlayer(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf *pPlayer,
        XADataSource *pDataSrc, XADataSource *pBankSrc,
        XADataSink *pAudioSnk, XADataSink *pImageVideoSnk,
        XADataSink *pVibra, XADataSink *pLEDArray, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired)
    {
    XAuint8 itfIdx = 0;
    XAMediaType mediaType = XA_MEDIATYPE_UNKNOWN;
    XAMediaPlayerImpl* pPlayerImpl = NULL;
    FWMgrFwType fwType;
    XAObjectItfImpl* pBaseObj = NULL;
    const char *uri = NULL;
    XAresult ret = XA_RESULT_SUCCESS;
    XADataLocator_IODevice* tmpIODevice;
    XADataLocator_IODevice locatorIODevice;

    DEBUG_API("->XAMediaPlayerImpl_CreateMediaPlayer");

    XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaPlayer);

    if (!pPlayer || !pDataSrc)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
        return XA_RESULT_PARAMETER_INVALID;
        }
    /* check sink&source parameters */
    ret = XACommon_ValidateDataLocator(6, pAudioSnk, pImageVideoSnk,
            pDataSrc, pBankSrc, pVibra, pLEDArray);
    if (ret != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
        return ret;
        }
    /* also, check source availability */
    ret = XACommon_CheckDataSource(pDataSrc, &mediaType);
    if (ret != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
        return ret;
        }

    /* instantiate object */
    pPlayerImpl = (XAMediaPlayerImpl*) calloc(1, sizeof(XAMediaPlayerImpl));
    if (!pPlayerImpl)
        {
        /* memory allocation failed */
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
        return XA_RESULT_MEMORY_FAILURE;
        }
    pBaseObj = &pPlayerImpl->baseObj;

    /* Initialize base object default implementation */
    XAObjectItfImpl_Init(pBaseObj, MP_ITFCOUNT, xaMediaPlayerItfIIDs,
            XAMediaPlayerImpl_DoRealize, XAMediaPlayerImpl_DoResume,
            XAMediaPlayerImpl_FreeResources);

    /* Mark interfaces that need to be exposed */
    /* Implicit and mandated interfaces */
    pBaseObj->interfaceMap[MP_PLAYITF].required = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[MP_DIMITF].required = XA_BOOLEAN_TRUE;

    /* Explicit interfaces */
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
                { /* weed out unsupported content-aware itf's */
                if ((mediaType == XA_MEDIATYPE_IMAGE && (entry->mapIdx == MP_SEEKITF 
                        || entry->mapIdx == MP_VOLUMEITF)))
                    {
                    entry->required = XA_BOOLEAN_FALSE;
                    if (pInterfaceRequired[itfIdx])
                        {
                        DEBUG_ERR("Required interface not supported for given media - abort creation!");
                        ret = XA_RESULT_FEATURE_UNSUPPORTED;
                        break;
                        }
                    }
                else
                    {
                    entry->required = XA_BOOLEAN_TRUE;
                    }
                }
            }
        }

    if (ret != XA_RESULT_SUCCESS)
        { /* creation fails */
        XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
        XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
        DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
        return ret;
        }

    /* Mark interfaces that can be handled dynamically */
    /* Mandated dynamic itfs */
    pBaseObj->interfaceMap[MP_METADATAEXTRACTIONITF].isDynamic
            = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[MP_PLAYBACKRATEITF].isDynamic = XA_BOOLEAN_TRUE;

    /*initialize XAPlayerImpl variables */

    pPlayerImpl->dataSrc = pDataSrc;
    pPlayerImpl->bankSrc = pBankSrc;
    pPlayerImpl->audioSnk = pAudioSnk;
    pPlayerImpl->imageVideoSnk = pImageVideoSnk;
    pPlayerImpl->vibra = pVibra;
    pPlayerImpl->LEDArray = pLEDArray;

    // Handle possible radio:      
    tmpIODevice = (XADataLocator_IODevice*) (pPlayerImpl->dataSrc->pLocator);
    locatorIODevice = *tmpIODevice;
    if (locatorIODevice.deviceType == XA_IODEVICE_RADIO)
        {
        fwType = (FWMgrFwType) FWMgrFWMMF;
        }
    else
        {
        /* Determine framework type that can handle recording */
        fwType = (FWMgrFwType) FWMgrMOUnknown;
        /**/
        if (pDataSrc->pLocator)
            {
            XADataLocator_URI* dataLoc =
                    (XADataLocator_URI*) pDataSrc->pLocator;
            if (dataLoc->locatorType == XA_DATALOCATOR_URI)
                {
                uri = (char*) dataLoc->URI;
                }
            }
        fwType = XAFrameworkMgr_GetFramework(mapper, uri, FWMgrMOPlayer);

        if (fwType == FWMgrMOUnknown)
            {
            ret = XA_RESULT_CONTENT_UNSUPPORTED;
            XAObjectItfImpl_Destroy((XAObjectItf) &(pBaseObj));
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
            DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
            return ret;
            }
        } // end else

    if (fwType == FWMgrFWMMF)
        {
        pPlayerImpl->adaptationCtxMMF = XAMediaPlayerAdaptMMF_Create(
                pPlayerImpl->dataSrc, pPlayerImpl->bankSrc,
                pPlayerImpl->audioSnk, pPlayerImpl->imageVideoSnk,
                pPlayerImpl->vibra, pPlayerImpl->LEDArray);

        pPlayerImpl->curAdaptCtx = pPlayerImpl->adaptationCtxMMF;
        }

    pPlayerImpl->curAdaptCtx->capslist = capabilities;
    pPlayerImpl->curAdaptCtx->fwtype = fwType;
    
    /*Set ObjectItf to point to newly created object*/
    *pPlayer = (XAObjectItf) &(pBaseObj->self);
    
    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XAMediaPlayerImpl_CreateMediaPlayer");
    return XA_RESULT_SUCCESS;
    }

/* XAResult XAMediaPlayerImpl_QueryNumSupportedInterfaces
 * Statically query supported interfaces
 */
XAresult XAMediaPlayerImpl_QueryNumSupportedInterfaces(
        XAuint32 * pNumSupportedInterfaces)
    {
    DEBUG_API("->XAMediaPlayerImpl_QueryNumSupportedInterfaces");

    if (pNumSupportedInterfaces)
        {
        *pNumSupportedInterfaces = MP_ITFCOUNT;
        DEBUG_API("<-XAMediaPlayerImpl_QueryNumSupportedInterfaces");
        return XA_RESULT_SUCCESS;
        }
    else
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMediaPlayerImpl_QueryNumSupportedInterfaces");
        return XA_RESULT_PARAMETER_INVALID;
        }
    }

/* XAResult XAMediaPlayerImpl_QuerySupportedInterfaces
 * Statically query supported interfaces
 */
XAresult XAMediaPlayerImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID * pInterfaceId)
    {
    DEBUG_API("->XAMediaPlayerImpl_QuerySupportedInterfaces");

    if (index >= MP_ITFCOUNT || !pInterfaceId)
        {
        if (pInterfaceId)
            {
            *pInterfaceId = XA_IID_NULL;
            }DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            DEBUG_API("<-XAMediaPlayerImpl_QuerySupportedInterfaces");
        return XA_RESULT_PARAMETER_INVALID;
        }
    else
        {
        *pInterfaceId = *(xaMediaPlayerItfIIDs[index]);
        DEBUG_API("<-XAMediaPlayerImpl_QuerySupportedInterfaces");
        return XA_RESULT_SUCCESS;
        }
    }

/*
 * Realize the object
 */
XAresult XAMediaPlayerImpl_DoRealize(XAObjectItf self)
    {
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAMediaPlayerImpl* pImpl = (XAMediaPlayerImpl*) (pObj);
    XAuint8 itfIdx = 0;
    void *pItf = NULL;
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMediaPlayerImpl_DoRealize");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );

    /* check casting from correct pointer type */
    if (!pImpl || pObj != pImpl->baseObj.self)
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMediaPlayerImpl_DoRealize");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* Realize all implicit and explicitly wanted interfaces */
    for (itfIdx = 0; itfIdx < MP_ITFCOUNT; itfIdx++)
        {
        if (!(pObj->interfaceMap[itfIdx].pItf)
                && pObj->interfaceMap[itfIdx].required)
            {
            switch (itfIdx)
                {
                case MP_DIMITF:
                    pItf = XADIMItfImpl_Create();
                    if (pItf)
                        {
                        XADIMItfImpl_Init(pItf, self,
                                XAMediaPlayerImpl_DoAddItf,
                                XAMediaPlayerImpl_DoResumeItf,
                                XAMediaPlayerImpl_DoRemoveItf);
                        }
                    break;
                case MP_PLAYITF:
                    pItf = XAPlayItfImpl_Create(pImpl);
                    if (pImpl->dataSrc)
                        {
                        XAuint32 locType =
                                *((XAuint32*) (pImpl->dataSrc->pLocator));
                        if (locType == XA_DATALOCATOR_IODEVICE)
                            {
                            XADataLocator_IODevice
                                    *ioDevice =
                                            (XADataLocator_IODevice*) (pImpl->dataSrc->pLocator);
                            if (ioDevice->deviceType == XA_IODEVICE_CAMERA)
                                {
                                vfHandle = (void*) pItf;
                                DEBUG_INFO_A1("Stored view finder pointer to global address %x", (int)vfHandle);
                                }
                            }
                        else
                            {

                            }
                        }
                    break;
                case MP_VOLUMEITF:
                    pItf = XAVolumeItfImpl_Create(pImpl->curAdaptCtx);
                    break;
                case MP_SEEKITF:
                    pItf = XASeekItfImpl_Create(pImpl);
                    break;
                case MP_PREFETCHSTATUSITF:
                    pItf = XAPrefetchStatusItfImpl_Create(pImpl);
                    break;
                case MP_METADATAEXTRACTIONITF:
                    pItf = XAMetadataExtractionItfImpl_Create(
                            pImpl->curAdaptCtx);
                    break;
                case MP_PLAYBACKRATEITF:
                    pItf = XAPlaybackRateItfImpl_Create(pImpl);
                    break;
                case MP_DYNAMICSOURCEITF:
                    pItf = XADynamicSourceItfImpl_Create(pImpl->curAdaptCtx);
                    break;
                case MP_NOKIAVOLUMEEXT:
                    pItf = XANokiaVolumeExtItfImpl_Create(pImpl->curAdaptCtx);
                    break;
                case MP_NOKIALINEARVOLUME:
                    pItf = XANokiaLinearVolumeItfImpl_Create(
                            pImpl->curAdaptCtx);
                    break;
                case MP_STREAMINFORMATIONITF:
                    pItf = XAStreamInformationItfImpl_Create(
                            pImpl);
                    break;
                case MP_VIDEOPOSTPROCESSINGITF:
                    pItf = XAVideoPostProcessingItfImpl_Create(
                            pImpl->curAdaptCtx);
                    break;
                default:
                    break;
                }
            if (!pItf)
                {
                /* Memory allocation failed */
                XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
                DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
                DEBUG_API("<-XAMediaPlayerImpl_DoRealize");
                return XA_RESULT_MEMORY_FAILURE;
                }
            else
                {
                pObj->interfaceMap[itfIdx].pItf = pItf;
                }
            }
        }
    /* init adaptation */
    if (pImpl->curAdaptCtx->fwtype == FWMgrFWMMF)
        {
        ret = XAMediaPlayerAdaptMMF_PostInit(
                (XAAdaptationMMFCtx*) pImpl->adaptationCtxMMF);
        }

    if (ret != XA_RESULT_SUCCESS)
        {
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("Adaptation init failed!");
        DEBUG_API("<-XAMediaPlayerImpl_DoRealize");
        return ret;
        }

    pObj->state = XA_OBJECT_STATE_REALIZED;

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XAMediaPlayerImpl_DoRealize");
    return XA_RESULT_SUCCESS;
    }

/*
 * Resume object
 */
XAresult XAMediaPlayerImpl_DoResume(XAObjectItf self)
    {
    DEBUG_API("->XAMediaPlayerImpl_DoResume");
    DEBUG_API("<-XAMediaPlayerImpl_DoResume");
    /* suspended state not supported by this implementation */
    return XA_RESULT_PRECONDITIONS_VIOLATED;
    }

/**
 * void XAMediaPlayerImpl_FreeResources(XAObjectItf self)
 * Description: Free all resources reserved at XAMediaPlayerImpl_DoRealize()
 **/
void XAMediaPlayerImpl_FreeResources(XAObjectItf self)
    {
    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAuint8 itfIdx = 0;
    XAMediaPlayerImpl* pImpl = (XAMediaPlayerImpl*) pObj;
    DEBUG_API("->XAMediaPlayerImpl_FreeResources");
    XA_IMPL_THREAD_SAFETY_ENTRY_FOR_VOID_FUNCTIONS( XATSMediaPlayer );

    for (itfIdx = 0; itfIdx < MP_ITFCOUNT; itfIdx++)
        {
        void *pItf = pObj->interfaceMap[itfIdx].pItf;
        if (pItf)
            {
            switch (itfIdx)
                {
                case MP_DIMITF:
                    XADIMItfImpl_Free(pItf);
                    break;
                case MP_PLAYITF:
                    XAPlayItfImpl_Free(pItf);
                    break;
                case MP_VOLUMEITF:
                    XAVolumeItfImpl_Free(pItf);
                    break;
                case MP_SEEKITF:
                    XASeekItfImpl_Free(pItf);
                    break;
                case MP_PREFETCHSTATUSITF:
                    XAPrefetchStatusItfImpl_Free(pItf);
                    break;
                case MP_METADATAEXTRACTIONITF:
                    XAMetadataExtractionItfImpl_Free(pItf);
                    break;
                case MP_PLAYBACKRATEITF:
                    XAPlaybackRateItfImpl_Free(pItf);
                    break;
                case MP_DYNAMICSOURCEITF:
                    XADynamicSourceItfImpl_Free(pItf);
                    break;
                case MP_NOKIAVOLUMEEXT:
                    XANokiaVolumeExtItfImpl_Free(pItf);
                    break;
                case MP_NOKIALINEARVOLUME:
                    XANokiaLinearVolumeItfImpl_Free(pItf);
                    break;
                case MP_STREAMINFORMATIONITF:
                    XAStreamInformationItfImpl_Free(pItf);
                    break;
                case MP_VIDEOPOSTPROCESSINGITF:
                    XAVideoPostProcessingItfImpl_Free(pItf);
                    break;

                }
            pObj->interfaceMap[itfIdx].pItf = NULL;
            }
        }

    if (pImpl->curAdaptCtx)
        {
        if (pImpl->curAdaptCtx->fwtype == FWMgrFWMMF)
            {
            XAMediaPlayerAdaptMMF_Destroy(
                    (XAAdaptationMMFCtx*) pImpl->adaptationCtxMMF);
            }
        }

    XA_IMPL_THREAD_SAFETY_EXIT_FOR_VOID_FUNCTIONS( XATSMediaPlayer );
    DEBUG_API("<-XAMediaPlayerImpl_FreeResources");
    return;
    }

/* XAMediaPlayerImpl_DoAddItf
 * Dynamically add an interface, object specific parts
 */
XAresult XAMediaPlayerImpl_DoAddItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {

    XAObjectItfImpl* pObj = (XAObjectItfImpl*) (*self);
    XAMediaPlayerImpl* pImpl = (XAMediaPlayerImpl*) (pObj);

    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMediaPlayerImpl_DoAddItf");

    if (mapEntry)
        {
        switch (mapEntry->mapIdx)
            {

            case MP_METADATAEXTRACTIONITF:
                mapEntry->pItf = XAMetadataExtractionItfImpl_Create(
                        pImpl->curAdaptCtx);
                break;
            case MP_PLAYBACKRATEITF:
                mapEntry->pItf = XAPlaybackRateItfImpl_Create(pImpl);
                break;
            default:
                DEBUG_ERR("XAMediaPlayerImpl_DoAddItf unknown id")
                ;
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                break;
            }

        if (!mapEntry->pItf && ret == XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("XAMediaPlayerImpl_DoAddItf itf creation failed");
            ret = XA_RESULT_MEMORY_FAILURE;
            }
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API("<-XAMediaPlayerImpl_DoAddItf");
    return ret;
    }

/* XAMediaPlayerImpl_DoResumeItf
 * Try to resume lost interface, object specific parts
 */
XAresult XAMediaPlayerImpl_DoResumeItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    /* For now, no difference between suspended and unrealised itfs */
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMediaPlayerImpl_DoResumeItf");
    ret = XAMediaPlayerImpl_DoAddItf(self, mapEntry);
    DEBUG_API("<-XAMediaPlayerImpl_DoResumeItf");
    return ret;
    }

/* XAMediaPlayerImpl_DoRemoveItf
 * Dynamically remove an interface, object specific parts
 */
XAresult XAMediaPlayerImpl_DoRemoveItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMediaPlayerImpl_DoRemoveItf");

    if (mapEntry)
        {
        switch (mapEntry->mapIdx)
            {
            case MP_METADATAEXTRACTIONITF:
                XAMetadataExtractionItfImpl_Free(mapEntry->pItf);
                break;
            case MP_PLAYBACKRATEITF:
                XAPlaybackRateItfImpl_Free(mapEntry->pItf);
                break;
            default:
                DEBUG_ERR("XAMediaPlayerImpl_DoRemoveItf unknown id")
                ;
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                break;
            }
        mapEntry->pItf = NULL;
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API("<-XAMediaPlayerImpl_DoRemoveItf");
    return ret;
    }

