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
 * Description: StreamInfo Itf Adaptation MMF
 *
 */

#include "xadebug.h"
#include "xathreadsafety.h"
#include "xamediaplayeradaptctxmmf.h"
#include "xastreaminformationitfadaptationmmf.h"
#include "xaplayitfadaptationmmf.h"
#include "cmmfbackendengine.h"
/*
 * XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation(
 *   XAAdaptationBaseCtx *adaptCtx,
 *   XAMediaContainerInformation *info);
 * Description: Obtains media container information
 */
XAresult XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 *containerType,
        XAmillisecond *mediaDuration, XAuint32 *numStreams)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XADataSource *xaDataSource = NULL;

    DEBUG_API("->XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation");
    /* Both Media Player and Metadata Extractor objec uses the same ctxId */
    if (!adaptCtx || (adaptCtx->ctxId != XAMediaPlayerAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* Initialize to unspecified */
    *containerType = XA_CONTAINERTYPE_UNSPECIFIED;
    xaDataSource = ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->xaSource;
    /*    DEBUG_INFO("Obtained handle to xaDataSource");*/
    if (xaDataSource && xaDataSource->pFormat
            && ((*(XAuint32*) xaDataSource->pFormat) == XA_DATAFORMAT_MIME))
        {
        *containerType
                = ((XADataFormat_MIME*) (xaDataSource->pFormat))->containerType;
        DEBUG_INFO_A1("Container type obtained from xaDataSource->pFormat is [%u]", (unsigned int)(*containerType));
        }
    else
        {
        DEBUG_INFO("Container type unspecified");
        }

    /* Get media duration */
    ret = XAPlayItfAdaptMMF_GetDuration(adaptCtx, mediaDuration);
    if (ret != XA_RESULT_SUCCESS)
        {
        return ret;
        }

    ret = mmf_streaminformationitf_get_numstreams(
            ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
            numStreams);

    DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryMediaContainerInformation");
    return ret;
    }

/*
 * XAresult XAStreamInformationItfAdaptMMF_QueryStreamType(
 *               XAAdaptationBaseCtx *adaptCtx,
 *               XAuint32 streamIndex,
 *               XAuint32 *domain)
 * Description: Obtains stream type information
 */
XAresult XAStreamInformationItfAdaptMMF_QueryStreamType(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamIndex, XAuint32 *domain)
    {
    XAresult ret;
    XAuint32 numStreams;
    XADataSource *xaDataSource = NULL;

    if (!adaptCtx || !domain)
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
        }

    DEBUG_API("->XAStreamInformationItfAdaptMMF_QueryStreamType");

    ret = mmf_streaminformationitf_get_numstreams(
            ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
            &numStreams);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamType");
        return ret;
        }

    /* validate stream index */
    if (streamIndex > numStreams)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamType");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* if format type is raw image */
    xaDataSource = ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->xaSource;
    if (xaDataSource && xaDataSource->pFormat
            && ((*(XAuint32*) xaDataSource->pFormat)
                    == XA_DATAFORMAT_RAWIMAGE))
        {
        *domain = XA_DOMAINTYPE_IMAGE;
        return XA_RESULT_SUCCESS;
        }
    else
        {
        ret = mmf_streaminformationitf_get_streaminfo(
                ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
                streamIndex, domain);
        }

    DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamType");
    return ret;
    }

/*
 * XAresult XAStreamInformationItfAdaptMMF_QueryStreamInformation(
 *               XAAdaptationBaseCtx *adaptCtx,
 *               XAuint32 streamIndex,
 *               void * info)
 * Description: Obtains stream information
 */
XAresult XAStreamInformationItfAdaptMMF_QueryStreamInformation(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamIndex, void * info)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 domain;
    XAuint32 codecId;
    XAuint32 channels;
    XAmilliHertz sampleRate;
    XAuint32 bitRate;
    XAmillisecond duration;
    XAuint32 width;
    XAuint32 height;
    XAuint32 frameRate;
    void* mmfctx;

    if (!adaptCtx || !info)
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
        }

    mmfctx = ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext;

    DEBUG_API("->XAStreamInformationItfAdaptMMF_QueryStreamInformation");

    /* stream index is validated in XAStreamInformationItfAdaptMMF_QueryStreamType */
    ret = XAStreamInformationItfAdaptMMF_QueryStreamType(adaptCtx,
            streamIndex, &domain);

    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamInformation");
        return ret;
        }

    switch (domain)
        {
        case XA_DOMAINTYPE_AUDIO:
            DEBUG_INFO("Getting Audio Stream Info from Backend Engine");
            ret = mmf_get_channels(mmfctx, &channels);
            ret = mmf_get_samplerate(mmfctx, &sampleRate);
            ret = mmf_get_bitrate(mmfctx, &bitRate);
            ret = mmf_get_codec_id(mmfctx, &codecId);
            break;
        case XA_DOMAINTYPE_VIDEO:
            DEBUG_INFO("Getting Video Stream Info from Backend Engine");
            ret = mmf_get_bitrate(mmfctx, &bitRate);
            ret = mmf_get_codec_id(mmfctx, &codecId);
            ret = mmf_streaminformationitf_get_videoframesize(mmfctx,
                    &height, &width, &frameRate);
            break;
        case XA_DOMAINTYPE_IMAGE:
        case XA_DOMAINTYPE_MIDI:
        default:
            DEBUG_ERR("XA_RESULT_CONTENT_UNSUPPORTED");
            ret = XA_RESULT_CONTENT_UNSUPPORTED;
            break;
        };

    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamInformation");
        return ret;
        }

    DEBUG_INFO("Getting media duration");
    ret = XAPlayItfAdaptMMF_GetDuration(adaptCtx, &duration);

    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamInformation");
        return ret;
        }

    /* Now assign properties to data structure */
    switch (domain)
        {
        case XA_DOMAINTYPE_AUDIO:
            {
            XAAudioStreamInformation* streamInfo =
                    (XAAudioStreamInformation*) info;
            DEBUG_INFO("Assigning values for XAAudioStreamInformation");
            streamInfo->codecId = codecId;
            streamInfo->channels = channels;
            streamInfo->sampleRate = sampleRate;
            streamInfo->bitRate = bitRate;
            streamInfo->langCountry[0] = '\0';
            streamInfo->duration = duration;
            }
            break;
        case XA_DOMAINTYPE_VIDEO:
            {
            XAVideoStreamInformation* streamInfo =
                    (XAVideoStreamInformation*) info;
            DEBUG_INFO("Assigning values for XAVideoStreamInformation");
            streamInfo->codecId = codecId;
            streamInfo->width = width;
            streamInfo->height = height;
            streamInfo->bitRate = bitRate;
            streamInfo->duration = duration;
            streamInfo->frameRate = frameRate;
            }
            break;
        };
    DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamInformation");
    return ret;
    }

/*
 * XAresult XAStreamInformationItfAdaptMMF_QueryStreamName(
 *               XAAdaptationBaseCtx *adaptCtx,
 *               XAuint32 streamIndex,
 *               XAuint16 * pNameSize,
 *               XAchar * pName)
 * Description: Obtains stream name
 */
XAresult XAStreamInformationItfAdaptMMF_QueryStreamName(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamIndex,
        XAuint16 * pNameSize, XAchar * pName)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 numStreams;

    DEBUG_API("->XAStreamInformationItfAdaptMMF_QueryStreamName");
    /* Both Media Player and Metadata Extractor objec uses the same ctxId */
    if (!adaptCtx || !pNameSize || (adaptCtx->ctxId
            != XAMediaPlayerAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamName");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* validate stream index */
    ret = mmf_streaminformationitf_get_numstreams(
            ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
            &numStreams);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamName");
        return ret;
        }

    /* validate stream index */
    if (streamIndex > numStreams)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamName");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* TODO there is no straight way to retreive stream name from mmf*/
    /*XAuint16 inputNameSize = *pNameSize;*/
    *pNameSize = 0; /* Replace this with actual size*/
    if (pName)
        {
        /* TODO copy stream name into pName taking into account inputNameSize */
        }

    DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryStreamName");
    return ret;
    }

/*
 * XAresult XAStreamInformationItfAdaptMMF_QueryActiveStreams(
 *               XAAdaptationBaseCtx *adaptCtx,
 *               XAuint32 *numStreams,
 *               XAboolean *activeStreams);
 * Description: Obtains active stream information
 */
XAresult XAStreamInformationItfAdaptMMF_QueryActiveStreams(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 *numStreams,
        XAboolean *activeStreams)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 inputNumStreams;

    DEBUG_API("->XAStreamInformationItfAdaptMMF_QueryActiveStreams");

    /* Both Media Player and Metadata Extractor objec uses the same ctxId */
    if (!adaptCtx  || !numStreams || (adaptCtx->ctxId
            != XAMediaPlayerAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryActiveStreams");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (adaptCtx->ctxId == XAMDAdaptation)
        {
        DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryActiveStreams");
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }

    /* Determine number of streams */
    ret = mmf_streaminformationitf_get_numstreams(
            ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
            &inputNumStreams);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryActiveStreams");
        return ret;
        }

    if (activeStreams)
        {
        XAuint32 loopIndex;
        if (*numStreams < inputNumStreams)
            {
            DEBUG_INFO("numStreams param value is smaller than number of streams");
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryActiveStreams");
            return XA_RESULT_PARAMETER_INVALID;
            }
        for (loopIndex = 0; loopIndex < inputNumStreams; loopIndex++)
            {
            /* TODO currently gstreamer doesn't allow multiple dynamic streams
             * all streams are active streams */
            activeStreams[loopIndex] = XA_BOOLEAN_TRUE;
            }
        }
    else
        {
        *numStreams = inputNumStreams;
        }

    DEBUG_API("->XAStreamInformationItfAdaptMMF_QueryActiveStreams");
    return XA_RESULT_SUCCESS;
    }

XAresult XAStreamInformationItfAdaptMMF_SetActiveStream(
        XAAdaptationBaseCtx *adaptCtx, XAuint32 streamNum, XAboolean active,
        XAboolean commitNow)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 numStreams;

    DEBUG_API("->XAStreamInformationItfAdaptMMF_SetActiveStream");

    /* Both Media Player and Metadata Extractor objec uses the same ctxId */
    if (!adaptCtx || (adaptCtx->ctxId != XAMediaPlayerAdaptation))
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_SetActiveStream");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (adaptCtx->ctxId == XAMDAdaptation)
        {
        DEBUG_ERR("XA_RESULT_PRECONDITIONS_VIOLATED");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_SetActiveStream");
        return XA_RESULT_PRECONDITIONS_VIOLATED;
        }

    /* Determine number of streams */
    ret = mmf_streaminformationitf_get_numstreams(
            ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
            &numStreams);
    if (ret != XA_RESULT_SUCCESS)
        {
        DEBUG_ERR_A1("XA_RESULT_[0x%x]", (unsigned int)ret);
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_QueryActiveStreams");
        return ret;
        }

    /* validate stream index */
    if (streamNum > numStreams)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAStreamInformationItfAdaptMMF_SetActiveStream");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (commitNow)
        {
        ret = mmf_streaminformationitf_set_activestream(
                ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,
                streamNum, active);
        }

    DEBUG_API("->XAStreamInformationItfAdaptMMF_SetActiveStream");
    return ret;
    }

XAresult XAStreamInformationItfAdaptMMF_RegisterCallback(
					XAAdaptationBaseCtx *adaptCtx,
					xaStreamEventChangeCallback callback)
{
    DEBUG_API("<->XAStreamInformationItfAdaptMMF_RegisterCallback");
	return mmf_streaminfoitf_register_callback(((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfContext,callback);
}

