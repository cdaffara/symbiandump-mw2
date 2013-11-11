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
 * Description: GST Capabilities Mgr. Here you can query the capabilities
 *               of various GST components and store it in a list.
 *
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "xagstcapabilitiesmgr.h"
#include "xaframeworkmgr.h"

static XAresult XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities(
        XACapabilities **ppNode);
static XAresult XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities(
        XACapabilities **ppNode);
static XAresult XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities(
        XACapabilities **ppNode);

/* XAresult XAGSTCapabilitiesMgr_UpdateCapabilitieList(
 *       FrameworkMap *frameworkMap, XACapabilities **ppListHead)
 * Description: Append capabilities supported by Gstreamer framework.
 */
XAresult XAGSTCapabilitiesMgr_UpdateCapabilitieList(
        FrameworkMap *frameworkMap, XACapabilities **ppListHead)

    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    FWMgrFwType fwtype = FWMgrFWUknown;
    char *uri = NULL;
    XACapabilities *lastNode = NULL;
    XACapabilities *firstNode = NULL;

    DEBUG_API("->XAGSTCapabilitiesMgr_UpdateCapabilitieList");

    if (!frameworkMap || !ppListHead)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAGSTCapabilitiesMgr_UpdateCapabilitieList");
        return XA_RESULT_PARAMETER_INVALID;
        }

    uri = "file:///c:/test.mp4";
    fwtype = XAFrameworkMgr_GetFramework(frameworkMap, uri, FWMgrMORecorder);
    if (fwtype == FWMgrFWGST)
        {
        res = XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities(&newNode);
        if (res != XA_RESULT_SUCCESS)
            {
            XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
            DEBUG_API("<-XAGSTCapabilitiesMgr_UpdateCapabilitieList");
            return res;
            }
        if (!firstNode)
            {
            firstNode = newNode;
            lastNode = newNode;
            }
        }

    uri = "file:///c:/test.amr";
    fwtype = XAFrameworkMgr_GetFramework(frameworkMap, uri, FWMgrMORecorder);
    if (fwtype == FWMgrFWGST)
        {
        res = XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities(&newNode);
        if (res != XA_RESULT_SUCCESS)
            {
            XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
            DEBUG_API("<-XAGSTCapabilitiesMgr_UpdateCapabilitieList");
            return res;
            }
        if (!firstNode)
            {
            firstNode = newNode;
            lastNode = newNode;
            }
        else
            {
            lastNode->next = newNode;
            lastNode = newNode;
            }
        }

    uri = "file:///c:/test.wav";
    fwtype = XAFrameworkMgr_GetFramework(frameworkMap, uri, FWMgrMORecorder);
    if (fwtype == FWMgrFWGST)
        {
        res = XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities(&newNode);
        if (res != XA_RESULT_SUCCESS)
            {
            XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
            DEBUG_API("<-XAGSTCapabilitiesMgr_UpdateCapabilitieList");
            return res;
            }
        if (!firstNode)
            {
            firstNode = newNode;
            lastNode = newNode;
            }
        else
            {
            lastNode->next = newNode;
            lastNode = newNode;
            }
        }

    /* if we have some capabilities supported by Gstreamer framework */
    if (firstNode)
        {
        /* if empty list, then append first node as the head */
        if (!(*ppListHead))
            {
            *ppListHead = firstNode;
            }
        else /* traverse to the last item in the list and link firstNode to it */
            {
            lastNode = *ppListHead;
            while(lastNode->next)
                {
                lastNode = lastNode->next;
                }
            lastNode->next = firstNode;
            }
        }
    DEBUG_API("<-XAGSTCapabilitiesMgr_UpdateCapabilitieList");
    return res;
    }

/* XAresult XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities(
        XACapabilities **ppNode)
 * Description: If return value is XA_RESULT_SUCCESS, Creats a new XACapabilities
 * node and update ppNode to contain pointer to it.
 * Otherwise return error code for failure.
 */
XAresult XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities(
        XACapabilities **ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAchar aacencelement[] = "nokiaaacenc";
    int strLen = 0;
    XAAudioCodecDescriptor *codecDesc = NULL;

    DEBUG_API("->XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_E;
    newNode->xaid = XA_AUDIOCODEC_AAC;

    strLen = strlen((char*) aacencelement);
    newNode->adaptId = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!newNode->adaptId)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    strncpy((char*) newNode->adaptId, (char*) aacencelement, strLen);
    newNode->adaptId[strLen] = '\0'; /*Null terminate it*/

    /* create XAAudioCodecDescriptor for AAC and initialize values */
    codecDesc = (XAAudioCodecDescriptor*) calloc(2, sizeof(XAAudioCodecDescriptor));
    if (!codecDesc)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    newNode->noOfEntries = 2;
    newNode->pEntry = (void *)codecDesc;

    codecDesc->maxChannels = 2;
    (codecDesc+1)->maxChannels = 2;
    codecDesc->minBitsPerSample = 16;
    (codecDesc+1)->minBitsPerSample = 16;
    codecDesc->maxBitsPerSample = 16;
    (codecDesc+1)->maxBitsPerSample = 16;
    codecDesc->minSampleRate = 8000000;
    (codecDesc+1)->minSampleRate = 8000000;
    codecDesc->maxSampleRate = 48000000;
    (codecDesc+1)->maxSampleRate = 48000000;
    codecDesc->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    (codecDesc+1)->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    codecDesc->pSampleRatesSupported = (XAmilliHertz*)calloc(5, sizeof(XAmilliHertz));
    if (!(codecDesc->pSampleRatesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    (codecDesc+1)->pSampleRatesSupported = (XAmilliHertz*)calloc(5, sizeof(XAmilliHertz));
    if (!((codecDesc+1)->pSampleRatesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    codecDesc->pSampleRatesSupported[0] = 8000000;
    (codecDesc+1)->pSampleRatesSupported[0] = 8000000;
    codecDesc->pSampleRatesSupported[1] = 16000000;
    (codecDesc+1)->pSampleRatesSupported[1] = 16000000;
    codecDesc->pSampleRatesSupported[2] = 24000000;
    (codecDesc+1)->pSampleRatesSupported[2] = 24000000;
    codecDesc->pSampleRatesSupported[3] = 32000000;
    (codecDesc+1)->pSampleRatesSupported[3] = 32000000;
    codecDesc->pSampleRatesSupported[4] = 48000000;
    (codecDesc+1)->pSampleRatesSupported[4] = 48000000;
    codecDesc->numSampleRatesSupported = 5;
    (codecDesc+1)->numSampleRatesSupported = 5;
    codecDesc->minBitRate = 32000;
    (codecDesc+1)->minBitRate = 32000;
    codecDesc->maxBitRate = 256000;
    (codecDesc+1)->maxBitRate = 256000;
    codecDesc->isBitrateRangeContinuous = XA_BOOLEAN_FALSE;
    (codecDesc+1)->isBitrateRangeContinuous = XA_BOOLEAN_FALSE;
    codecDesc->pBitratesSupported = (XAuint32 *)calloc(8, sizeof(XAuint32));;
    if (!(codecDesc->pBitratesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    (codecDesc+1)->pBitratesSupported = (XAuint32 *)calloc(8, sizeof(XAuint32));;
    if (!((codecDesc+1)->pBitratesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    codecDesc->pBitratesSupported[0] = 32000;
    (codecDesc+1)->pBitratesSupported[0] = 32000;
    codecDesc->pBitratesSupported[1] = 64000;
    (codecDesc+1)->pBitratesSupported[1] = 64000;
    codecDesc->pBitratesSupported[2] = 96000;
    (codecDesc+1)->pBitratesSupported[2] = 96000;
    codecDesc->pBitratesSupported[3] = 128000;
    (codecDesc+1)->pBitratesSupported[3] = 128000;
    codecDesc->pBitratesSupported[4] = 160000;
    (codecDesc+1)->pBitratesSupported[4] = 160000;
    codecDesc->pBitratesSupported[5] = 192000;
    (codecDesc+1)->pBitratesSupported[5] = 192000;
    codecDesc->pBitratesSupported[6] = 224000;
    (codecDesc+1)->pBitratesSupported[6] = 224000;
    codecDesc->pBitratesSupported[7] = 256000;
    (codecDesc+1)->pBitratesSupported[7] = 256000;
    codecDesc->numBitratesSupported = 8;    
    (codecDesc+1)->numBitratesSupported = 8;    
    codecDesc->profileSetting = XA_AUDIOPROFILE_AAC_AAC;
    (codecDesc+1)->profileSetting = XA_AUDIOPROFILE_AAC_AAC;
    codecDesc->modeSetting = XA_AUDIOMODE_AAC_HE;
    (codecDesc+1)->modeSetting = XA_AUDIOMODE_AAC_LC;

    *ppNode = newNode;

    DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAACEncoderCapabilities");
    return res;
    }

XAresult XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities(
        XACapabilities **ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAchar amrencelement[] = "devsoundsrc";
    int strLen = 0;
    XAAudioCodecDescriptor *codecDesc = NULL;

    DEBUG_API("->XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_E;
    newNode->xaid = XA_AUDIOCODEC_AMR;
    
    strLen = strlen((char*) amrencelement);
    newNode->adaptId = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!newNode->adaptId)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    strncpy((char*) newNode->adaptId, (char*) amrencelement, strLen);
    newNode->adaptId[strLen] = '\0'; /*Null terminate it*/
    
    /* create XAAudioCodecDescriptor for AMR and initialize values */
    codecDesc = (XAAudioCodecDescriptor*) calloc(1, sizeof(XAAudioCodecDescriptor));
    if (!codecDesc)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    newNode->noOfEntries = 1;
    newNode->pEntry = (void *)codecDesc;

    codecDesc->maxChannels = 1;
    codecDesc->minBitsPerSample = 8;
    codecDesc->maxBitsPerSample = 8;
    codecDesc->minSampleRate = 8000000;
    codecDesc->maxSampleRate = 8000000;
    codecDesc->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    codecDesc->pSampleRatesSupported = (XAmilliHertz*)calloc(1, sizeof(XAmilliHertz));
    if (!(codecDesc->pSampleRatesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    codecDesc->pSampleRatesSupported[0] = 8000000;
    codecDesc->numSampleRatesSupported = 1;
    codecDesc->minBitRate = 4750;
    codecDesc->maxBitRate = 12200;
    codecDesc->isBitrateRangeContinuous = XA_BOOLEAN_FALSE;
    codecDesc->pBitratesSupported = (XAuint32 *)calloc(8, sizeof(XAuint32));;
    if (!(codecDesc->pBitratesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    codecDesc->pBitratesSupported[0] = 4750;
    codecDesc->pBitratesSupported[1] = 5150;
    codecDesc->pBitratesSupported[2] = 5900;
    codecDesc->pBitratesSupported[3] = 6700;
    codecDesc->pBitratesSupported[4] = 7400;
    codecDesc->pBitratesSupported[5] = 7950;
    codecDesc->pBitratesSupported[6] = 10200;
    codecDesc->pBitratesSupported[7] = 12200;    
    codecDesc->numBitratesSupported = 8;
    codecDesc->profileSetting = XA_AUDIOPROFILE_AMR;
    codecDesc->modeSetting = 0;
    
    *ppNode = newNode;

    DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioAMREncoderCapabilities");
    return res;
    }

XAresult XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities(
        XACapabilities **ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAchar wavencelement[] = "wavenc";
    int strLen = 0;
    XAAudioCodecDescriptor *codecDesc = NULL;

    DEBUG_API("->XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_E;
    newNode->xaid = XA_AUDIOCODEC_PCM;
    
    strLen = strlen((char*) wavencelement);
    newNode->adaptId = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!newNode->adaptId)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    strncpy((char*) newNode->adaptId, (char*) wavencelement, strLen);
    newNode->adaptId[strLen] = '\0'; /*Null terminate it*/

    /* create XAAudioCodecDescriptor for PCM and initialize values */
    codecDesc = (XAAudioCodecDescriptor*) calloc(1, sizeof(XAAudioCodecDescriptor));
    if (!codecDesc)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    newNode->noOfEntries = 1;
    newNode->pEntry = (void *)codecDesc;

    codecDesc->maxChannels = 2;
    codecDesc->minBitsPerSample = 16;
    codecDesc->maxBitsPerSample = 16;
    codecDesc->minSampleRate = 8000000;
    codecDesc->maxSampleRate = 48000000;
    codecDesc->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    codecDesc->pSampleRatesSupported = (XAmilliHertz*)calloc(12, sizeof(XAmilliHertz));
    if (!(codecDesc->pSampleRatesSupported))
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    codecDesc->pSampleRatesSupported[0] = 8000000;
    codecDesc->pSampleRatesSupported[1] = 16000000;
    codecDesc->pSampleRatesSupported[2] = 24000000;
    codecDesc->pSampleRatesSupported[3] = 32000000;
    codecDesc->pSampleRatesSupported[4] = 48000000;
    codecDesc->numSampleRatesSupported = 5;
    codecDesc->minBitRate = 0;
    codecDesc->maxBitRate = 0;
    codecDesc->isBitrateRangeContinuous = XA_BOOLEAN_FALSE;
    codecDesc->pBitratesSupported = NULL;
    codecDesc->numBitratesSupported = 0;
    codecDesc->profileSetting = XA_AUDIOPROFILE_PCM;
    codecDesc->modeSetting = 0;
    
    *ppNode = newNode;

    DEBUG_API("<-XAGSTCapabilitiesMgr_GetAudioPCMEncoderCapabilities");
    return res;
    }
