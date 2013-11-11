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
 * Description: Audio Encoder capabilities Itf Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xaglobals.h"
#include "xaadptbasectx.h"
#include "xaaudioencodercapabilitiesitf.h"
#include "xacapabilitiesmgr.h"

/* XAAudioEncoderCapabilitiesItfImpl* GetImpl
 * Description: Validate interface pointer and cast it to implementation pointer.
 */
static XAAudioEncoderCapabilitiesItfImpl* GetImpl(
        XAAudioEncoderCapabilitiesItf self)
    {
    if (self)
        {
        XAAudioEncoderCapabilitiesItfImpl* impl =
                (XAAudioEncoderCapabilitiesItfImpl*) (*self);
        if (impl && (impl == impl->self))
            {
            return impl;
            }
        }
    return NULL;
    }

/*****************************************************************************
 * Base interface XAAudioEncoderCapabilitiesItf implementation
 *****************************************************************************/

/* XAresult XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoders
 * Description: Retrieves the available audio encoders.
 */
XAresult XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoders(
        XAAudioEncoderCapabilitiesItf self, XAuint32* pNumEncoders,
        XAuint32* pEncoderIds)
    {
    XAAudioEncoderCapabilitiesItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoders");

    if (!impl || !pNumEncoders)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
        }
    else
        {
        if (pEncoderIds)
            { /* query array of encoders */
            if (*pNumEncoders < impl->numCodecs)
                {
                DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
                res = XA_RESULT_PARAMETER_INVALID;
                }
            else
                {

                XAuint32 i = 0;
                XACapabilities temp;
                for (i = 0; i < impl->numCodecs; i++)
                    {
                    /* query encoder id from adaptation using index value */
                    XACapabilitiesMgr_GetCapsByIdx(impl->capslist,
                            (XACapsType) (XACAP_ENCODER | XACAP_AUDIO), i,
                            &temp);
                    pEncoderIds[i] = temp.xaid;
                    }
                }
            }

        /* return number of encoders */
        *pNumEncoders = impl->numCodecs;
        }
    DEBUG_API("<-XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoders");
    return res;
    }

/* XAresult XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoderCapabilities
 * Description: Queries for the audio encoders capabilities.
 */
XAresult XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoderCapabilities(
        XAAudioEncoderCapabilitiesItf self, XAuint32 encoderId,
        XAuint32 *pIndex, XAAudioCodecDescriptor *pDescriptor)
    {
    XAAudioEncoderCapabilitiesItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;

    XACapabilities temp;

    XAint16 index;
    DEBUG_API("->XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoderCapabilities");

    /*if( !impl || !pIndex || !pDescriptor )*/
    /*Removing the pDescriptor because the client can pass
     * it as NULL to query the pIndex for number of Codec/Mode
     * pair capabilities for each encoder */
    if (!impl || !pIndex)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
        return res;
        }
    else if(!pDescriptor)
        {
        res = XACapabilitiesMgr_GetCapsById(impl->capslist,
                (XACapsType) (XACAP_ENCODER | XACAP_AUDIO), encoderId, &temp);
        
        *pIndex = temp.noOfEntries;

        return res;

        }
    
    index = *pIndex;
    
    /* query capabilities from adaptation using codec id */
    memset(pDescriptor, 0, sizeof(XAAudioCodecDescriptor));
 
    res = XACapabilitiesMgr_GetCapsById(impl->capslist,
            (XACapsType) (XACAP_ENCODER | XACAP_AUDIO), encoderId, &temp);
    if (res == XA_RESULT_SUCCESS)
        {
        XAAudioCodecDescriptor* desc =
                ((XAAudioCodecDescriptor*) (temp.pEntry));
        if(index >= temp.noOfEntries)
            {
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            res = XA_RESULT_PARAMETER_INVALID;
            return res;
        
            }
        else
            {
            //indexing
            desc = desc+index;
            }

        /* map applicable values to XAAudioCodecCapabilities */
        pDescriptor->maxChannels = desc->maxChannels;
        pDescriptor->minBitsPerSample = desc->minBitsPerSample;
        pDescriptor->maxBitsPerSample = desc->maxBitsPerSample;
        pDescriptor->minSampleRate = desc->minSampleRate ; /* milliHz */
        if (desc->maxSampleRate < (0xFFFFFFFF))
            {
            pDescriptor->maxSampleRate = desc->maxSampleRate;
            }
        else
            {
            pDescriptor->maxSampleRate = 0xFFFFFFFF;
            }

      pDescriptor->isFreqRangeContinuous=desc->isFreqRangeContinuous;
      pDescriptor->pSampleRatesSupported = desc->pSampleRatesSupported;
      pDescriptor->numSampleRatesSupported = desc->numSampleRatesSupported;
      pDescriptor->minBitRate = desc->minBitRate;
      pDescriptor->maxBitRate = desc->maxBitRate;
      pDescriptor->isBitrateRangeContinuous=desc->isBitrateRangeContinuous;
      pDescriptor->pBitratesSupported = desc->pBitratesSupported;
      pDescriptor->numBitratesSupported = desc->numBitratesSupported;
      pDescriptor->profileSetting=desc->profileSetting;
      pDescriptor->modeSetting=desc->modeSetting; /* no chanmode for pcm defined */
    }
      else
      {
          /* do nothing */
      }
      
    
   	
    DEBUG_API("<-XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoderCapabilities");
    return res;
    }

/*****************************************************************************
 * XAAudioEncoderCapabilitiesItfImpl -specific methods
 *****************************************************************************/

/* XAAudioEncoderCapabilitiesItfImpl_Create
 * Description: Allocate and initialize XAAudioEncoderCapabilitiesItfImpl
 */
XAAudioEncoderCapabilitiesItfImpl* XAAudioEncoderCapabilitiesItfImpl_Create(
        XACapabilities* caps)
    {
    XAAudioEncoderCapabilitiesItfImpl* self =
            (XAAudioEncoderCapabilitiesItfImpl*) calloc(1,
                    sizeof(XAAudioEncoderCapabilitiesItfImpl));

    DEBUG_API("->XAAudioEncoderCapabilitiesItfImpl_Create");

    if (self)
        {
        /* init itf default implementation */
        self->itf.GetAudioEncoders
                = XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoders;
        self->itf.GetAudioEncoderCapabilities
                = XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoderCapabilities;
        self->capslist = caps;

        /* init variables */
        XACapabilitiesMgr_GetCapsCount( caps, (XACapsType)((XACapsType)(XACAP_ENCODER|XACAP_AUDIO)),
                        &(self->numCodecs) );

        /*self->mmfEngine = (void*)mmf_capability_engine_init();*/
        
        //self->numCodecs = 3;
        self->self = self;

        }
    DEBUG_API("<-XAAudioEncoderCapabilitiesItfImpl_Create");
    return self;
    }

/* void XAAudioEncoderCapabilitiesItfImpl_Free
 * Description: Free all resources reserved at XAAudioEncoderCapabilitiesItfImpl_Create
 */
void XAAudioEncoderCapabilitiesItfImpl_Free(
        XAAudioEncoderCapabilitiesItfImpl* self)
    {
    DEBUG_API("->XAAudioEncoderCapabilitiesItfImpl_Free");
    free(self);
    DEBUG_API("<-XAAudioEncoderCapabilitiesItfImpl_Free");
    }
