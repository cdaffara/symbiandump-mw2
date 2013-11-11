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
 * Description: 
 *
 */

#include "xamediarecorderadaptctxmmf.h"
#include "xaaudioencoderitf.h"
#include "xaaudioencoderitfadaptation.h"
#include "xarecorditfadaptation.h"
#include "xathreadsafety.h"
#include "cmmfbackendengine.h"
#include "xacapabilitiesmgr.h"

/* XAAudioEncoderItfImpl* GetImpl(XAAudioEncoderItf self)
 * Description: Validate interface pointer and cast it to implementation pointer.
 **/
static XAAudioEncoderItfImpl* GetImpl(XAAudioEncoderItf self)
    {
    if (self)
        {
        XAAudioEncoderItfImpl* impl = (XAAudioEncoderItfImpl*) (*self);
        if (impl && (impl == impl->self))
            {
            return impl;
            }
        }
    return NULL;
    }

/*****************************************************************************
 * Base interface XAAudioEncoderItf implementation
 *****************************************************************************/

/**
 * XAresult XAAudioEncoderItfImpl_SetEncoderSettings(XAAudioEncoderItf self,
 *                                                   XAAudioEncoderSettings *pSettings)
 * Description: Set audio encoder settings.
 **/
XAresult XAAudioEncoderItfImpl_SetEncoderSettings(XAAudioEncoderItf self,
        XAAudioEncoderSettings *pSettings)
    {
    XAMediaRecorderAdaptationMMFCtx* mCtx;
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 recState = XA_RECORDSTATE_STOPPED;
    XAAudioEncoderSettings currentSettings;
    XAAudioEncoderItfImpl *impl = GetImpl(self);

    DEBUG_API("->XAAudioEncoderItfImpl_SetEncoderSettings");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaRecorder );

    if (!impl || !pSettings)
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAAudioEncoderItfImpl_SetEncoderSettings");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        mCtx
                = (XAMediaRecorderAdaptationMMFCtx*) impl->pObjImpl->adaptationCtx;

        if (mCtx->xaRecordState != recState)
            {
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
            DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
            DEBUG_API("<-XAAudioEncoderItfImpl_SetEncoderSettings");
            return XA_RESULT_PRECONDITIONS_VIOLATED;
            }

        XAAudioEncoderItfImpl_GetEncoderSettings(self, &currentSettings);
        if (pSettings->encoderId == currentSettings.encoderId)
            {
            if (pSettings->channelsIn != currentSettings.channelsIn)
                {
                ret = mmf_set_destination_channels(mCtx->mmfContext,
                        &(pSettings->channelsIn));
                }

            if (pSettings->channelsOut != currentSettings.channelsOut)
                {
                ret = mmf_set_destination_channels(mCtx->mmfContext,
                        &(pSettings->channelsOut));
                }

            if (pSettings->sampleRate != currentSettings.sampleRate)
                {
                ret = mmf_set_destination_samplerate(mCtx->mmfContext,
                        &(pSettings->sampleRate));
                }

            if (pSettings->bitRate != currentSettings.bitRate)
                {
                ret = mmf_set_destination_bitrate(mCtx->mmfContext,
                        &(pSettings->bitRate));
                }

            if (pSettings->rateControl != currentSettings.rateControl)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            if (pSettings->channelMode != currentSettings.channelMode)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            if (pSettings->encodeOptions != currentSettings.encodeOptions)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            if (pSettings->blockAlignment != currentSettings.blockAlignment)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            if (pSettings->bitsPerSample != currentSettings.bitsPerSample)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            if (pSettings->profileSetting != currentSettings.profileSetting)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            if (pSettings->levelSetting != currentSettings.levelSetting)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            }
        else
            {
            ret = XA_RESULT_PARAMETER_INVALID;
            }
        }
    else
        {
        XACapabilities temp;

        ret = XACapabilitiesMgr_GetCapsById(
                impl->pObjImpl->adaptationCtx->capslist,
                (XACapsType) (XACAP_ENCODER | XACAP_AUDIO),
                pSettings->encoderId, &temp);
        if (ret == XA_RESULT_SUCCESS)
            {
            ret = XARecordItfAdapt_GetRecordState(
                    (XAAdaptationGstCtx*) impl->adapCtx, &recState);
            if (ret == XA_RESULT_SUCCESS)
                {
                if (XA_RECORDSTATE_STOPPED == recState)
                    {
                    ret = XAAudioEncoderItfAdapt_SetEncoderSettings(
                            impl->adapCtx, pSettings);
                    }
                else
                    {
                    DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
                    DEBUG_API("<-XAAudioEncoderItfImpl_SetEncoderSettings");
                    return XA_RESULT_PRECONDITIONS_VIOLATED;
                    }
                }
            }
        else
            {
            DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
            DEBUG_API("<-XAAudioEncoderItfImpl_SetEncoderSettings");
            return XA_RESULT_FEATURE_UNSUPPORTED;
            }
        }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaRecorder );
    DEBUG_API("<-XAAudioEncoderItfImpl_SetEncoderSettings");
    return ret;
    }

/**
 * XAresult XAAudioEncoderItfImpl_GetEncoderSettings(XAAudioEncoderItf self,
 *                                                   XAAudioEncoderSettings *pSettings)
 * Description: Get audio encoder settings.
 **/
XAresult XAAudioEncoderItfImpl_GetEncoderSettings(XAAudioEncoderItf self,
        XAAudioEncoderSettings *pSettings)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 encoderId;
    XAuint32 channelsIn;
    XAuint32 channelsOut;
    XAmilliHertz sampleRate;
    XAuint32 bitRate;
    XAMediaRecorderAdaptationMMFCtx* mCtx;

    XAAudioEncoderItfImpl *impl = GetImpl(self);

    DEBUG_API("->XAAudioEncoderItfImpl_GetEncoderSettings");
    if (!impl || !pSettings)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    mCtx = (XAMediaRecorderAdaptationMMFCtx*) (impl->pObjImpl->adaptationCtx);

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        mmf_get_codec_id(mCtx->mmfContext, &encoderId);

        switch (encoderId)
            {
            case 0x36315020:
                pSettings->encoderId = XA_AUDIOCODEC_PCM;
                pSettings->bitsPerSample = 16;
                pSettings->profileSetting = XA_AUDIOPROFILE_PCM;
                pSettings->levelSetting = 0;
                pSettings->streamFormat = 0;
                break;
            case 0x524d4120:
                pSettings->encoderId = XA_AUDIOCODEC_AMR;
                pSettings->bitsPerSample = 8;
                pSettings->profileSetting = XA_AUDIOPROFILE_AMR;
                pSettings->levelSetting = 0;
                pSettings->streamFormat = XA_AUDIOSTREAMFORMAT_FSF;
                break;
            case 0x43414120:
                pSettings->encoderId = XA_AUDIOCODEC_AAC;
                pSettings->bitsPerSample = 16;
                pSettings->profileSetting = XA_AUDIOPROFILE_AAC_AAC;
                pSettings->levelSetting = XA_AUDIOMODE_AAC_LC;
                pSettings->streamFormat = XA_AUDIOSTREAMFORMAT_RAW;
                break;
            }

        mmf_get_channels(mCtx->mmfContext, &channelsIn);
        pSettings->channelsIn = channelsIn;
        mmf_get_channels(mCtx->mmfContext, &channelsOut);
        pSettings->channelsOut = channelsOut;
        mmf_get_samplerate(mCtx->mmfContext, &sampleRate);
        pSettings->sampleRate = sampleRate;
        mmf_get_bitrate(mCtx->mmfContext, &bitRate);
        pSettings->bitRate = bitRate;
        pSettings->rateControl = XA_RATECONTROLMODE_CONSTANTBITRATE;
        pSettings->channelMode = 0;
        pSettings->encodeOptions = 0;
        pSettings->blockAlignment = 0;
        }
    else
        {
        ret = XAAudioEncoderItfAdapt_GetEncoderSettings(impl->adapCtx,
                pSettings);
        }
    DEBUG_API("<-XAAudioEncoderItfImpl_GetEncoderSettings");
    return ret;
    }

/*****************************************************************************
 * XAAudioEncoderItfImpl -specific methods
 *****************************************************************************/

/* XAAudioEncoderItfImpl* XAAudioEncoderItfImpl_Create()
 * Description: Allocate and initialize XAAudioEncoderItfImpl
 */
XAAudioEncoderItfImpl* XAAudioEncoderItfImpl_Create(XAMediaRecorderImpl* impl)
    {
    XAAudioEncoderItfImpl* self = (XAAudioEncoderItfImpl*) calloc(1,
            sizeof(XAAudioEncoderItfImpl));
    //XAMediaRecorderAdaptationCtx* mCtx = (XAMediaRecorderAdaptationCtx*)(impl->adaptationCtx);

    DEBUG_API("->XAAudioEncoderItfImpl_Create");
    if (self)
        {
        //if(mCtx->fwtype == FWMgrFWMMF)
            {
            /* init itf default implementation */
            self->itf.GetEncoderSettings
                    = XAAudioEncoderItfImpl_GetEncoderSettings;
            self->itf.SetEncoderSettings
                    = XAAudioEncoderItfImpl_SetEncoderSettings;
            }

        self->pObjImpl = impl;
        /* init variables */
        self->adapCtx = impl->adaptationCtx;

        self->self = self;
        }
    DEBUG_API("<-XAAudioEncoderItfImpl_Create");
    return self;
    }

/* void XAAudioEncoderItfImpl_Free(XAAudioEncoderItfImpl* self)
 * Description: Free all resources reserved at XAAudioEncoderItfImpl_Create
 */
void XAAudioEncoderItfImpl_Free(XAAudioEncoderItfImpl* self)
    {
    DEBUG_API("->XAAudioEncoderItfImpl_Free");
    if(self)
        {
        free(self);
        }
    DEBUG_API("<-XAAudioEncoderItfImpl_Free");
    }
