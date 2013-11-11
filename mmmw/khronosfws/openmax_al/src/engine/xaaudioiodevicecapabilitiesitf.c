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
* Description: Audio IO Device capabilities Itf
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xaglobals.h"
#include "xaaudioiodevicecapabilitiesitf.h"
#include "xacapabilitiesmgr.h"
#include "xaadptbasectx.h"

/* XAAudIODevCapaItfImpl* GetImpl
 * Description: Validate interface pointer and cast it to implementation pointer.
 */
static XAAudIODevCapaItfImpl* GetImpl(XAAudioIODeviceCapabilitiesItf self)
{
    if( self )
    {
        XAAudIODevCapaItfImpl* impl = (XAAudIODevCapaItfImpl*)(*self);
        if( impl && (impl == impl->self) )
        {
            return impl;
        }
    }
    return NULL;
}

/**
 * Base interface XAAudioIODeviceCapabilitiesItf implementation
 **/

/* XAresult XAAudIODevCapaItfImpl_GetAvailableAudioInputs
 * Description: Gets the number and IDs of audio input devices currently available.
 */
XAresult XAAudIODevCapaItfImpl_GetAvailableAudioInputs(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            XAint32* pNumInputs,
                                            XAuint32* pInputDeviceIDs)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_GetAvailableAudioInputs");

    if( !impl || !pNumInputs )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        if( pInputDeviceIDs )
        {   /* query array of input devices */
            if( *pNumInputs < impl->numInputDevices )
            {
                DEBUG_ERR("XA_RESULT_BUFFER_INSUFFICIENT");
                res = XA_RESULT_BUFFER_INSUFFICIENT;
            }
            else
            {
                XAuint32 i;
                XACapabilities temp;
                for( i=0; i<impl->numInputDevices; i++ )
                {
                    /* query device id from adaptation using index value */
                    res = XACapabilitiesMgr_GetCapsByIdx(impl->capslist, (XACapsType)(XACAP_DEVSRC|XACAP_AUDIO), i, &temp);
                    pInputDeviceIDs[i] = temp.xaid;
                }
            }
        }
        else
        {
            *pNumInputs = impl->numInputDevices;
        }
    }
    DEBUG_API("<-XAAudIODevCapaItfImpl_GetAvailableAudioInputs");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_QueryAudioInputCapabilities
 * Description: Gets the capabilities of the specified audio input device.
 */
XAresult XAAudIODevCapaItfImpl_QueryAudioInputCapabilities(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            XAuint32 deviceId,
                                            XAAudioInputDescriptor* pDescriptor)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XACapabilities temp;
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_QueryAudioInputCapabilities");

    if( !impl || !pDescriptor )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        memset(pDescriptor,0,sizeof(XAAudioInputDescriptor));

        /* query capabilities from adaptation using device id */
        

        res = XACapabilitiesMgr_GetCapsById(impl->capslist, (XACapsType)(XACAP_DEVSRC|XACAP_AUDIO), deviceId, &temp);
        if( res == XA_RESULT_SUCCESS )
        {
            XAAudioInputDescriptor* desc = ((XAAudioInputDescriptor*)(temp.pEntry));
            /* map applicable values to XAAudioCodecCapabilities */
            pDescriptor->maxChannels=desc->maxChannels;
            pDescriptor->minSampleRate=desc->minSampleRate; /* milliHz */
            if (desc->maxSampleRate < (0xFFFFFFFF ))
            {
                pDescriptor->maxSampleRate = desc->maxSampleRate;
            }
            else
            {
                pDescriptor->maxSampleRate = 0xFFFFFFFF;
            }
            pDescriptor->isFreqRangeContinuous=desc->isFreqRangeContinuous;
            pDescriptor->deviceConnection = desc->deviceConnection;
            pDescriptor->deviceScope = desc->deviceScope;
            pDescriptor->deviceLocation = desc->deviceLocation;
            pDescriptor->deviceName = desc->deviceName;
            /* other caps undefined */
            pDescriptor->isForTelephony = desc->isForTelephony;
            pDescriptor->samplingRatesSupported = desc->samplingRatesSupported;
            pDescriptor->numOfSamplingRatesSupported = desc->numOfSamplingRatesSupported;
         }
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_QueryAudioInputCapabilities");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_RegisterAvailableAudioInputsChangedCallback
 * Description: Sets or clears xaAvailableAudioInputsChangedCallback().
 */
XAresult XAAudIODevCapaItfImpl_RegisterAvailableAudioInputsChangedCallback(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            xaAvailableAudioInputsChangedCallback callback,
                                            void* pContext)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAAudIODevCapaItfImpl_RegisterAvailableAudioInputsChangedCallback");

    if( !impl )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        impl->inputCb = callback;
        impl->inputCbCtx = pContext;
        impl->inputCbPtrToSelf = self;
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_RegisterAvailableAudioInputsChangedCallback");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_GetAvailableAudioOutputs
 * Description: Gets the number and IDs of audio output devices currently available.
 */
XAresult XAAudIODevCapaItfImpl_GetAvailableAudioOutputs(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            XAint32* pNumOutputs,
                                            XAuint32* pOutputDeviceIDs)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAAudIODevCapaItfImpl_GetAvailableAudioOutputs");

    /* NOTE: only default speaker supported by this impl */
    if( !impl || !pNumOutputs )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        if( pOutputDeviceIDs )
        {   /* query array of output devices */
            if( *pNumOutputs < impl->numOutputDevices )
            {
                DEBUG_ERR("XA_RESULT_BUFFER_INSUFFICIENT");
                res = XA_RESULT_BUFFER_INSUFFICIENT;
            }
            else
            {


                XAuint32 i = 0;
                XACapabilities temp;
                for( i=0; i<impl->numOutputDevices; i++ )
                {
                    /* query device id from adaptation using index value */
                    res = XACapabilitiesMgr_GetCapsByIdx(impl->capslist, (XACapsType)(XACAP_DEVSNK|XACAP_AUDIO), i, &temp);
                    pOutputDeviceIDs[i] = temp.xaid;
                }
            }
        }
        else
        {
            *pNumOutputs = impl->numOutputDevices;
        }
    }
    DEBUG_API("<-XAAudIODevCapaItfImpl_GetAvailableAudioOutputs");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_QueryAudioOutputCapabilities
 * Description: Gets the capabilities of the specified audio output device.
 */
XAresult XAAudIODevCapaItfImpl_QueryAudioOutputCapabilities(
                                        XAAudioIODeviceCapabilitiesItf self,
                                        XAuint32 deviceId,
                                        XAAudioOutputDescriptor* pDescriptor)
{

    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities temp;
    DEBUG_API("->XAAudIODevCapaItfImpl_QueryAudioOutputCapabilities");

    if( !impl || !pDescriptor )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        memset(pDescriptor,0,sizeof(XAAudioOutputDescriptor));
        /* query capabilities from adaptation using device id */

        

        res = XACapabilitiesMgr_GetCapsById(impl->capslist, (XACapsType)(XACAP_DEVSNK|XACAP_AUDIO), deviceId, &temp);
        if( res == XA_RESULT_SUCCESS )
        {
            XAAudioOutputDescriptor* desc = ((XAAudioOutputDescriptor*)(temp.pEntry));
            /* map applicable values to XAAudioCodecCapabilities */
            pDescriptor->maxChannels=desc->maxChannels;
            pDescriptor->minSampleRate=desc->minSampleRate; /* milliHz */
            if (desc->maxSampleRate < (0xFFFFFFFF))
            {
                pDescriptor->maxSampleRate = desc->maxSampleRate;
            }
            else
            {
                pDescriptor->maxSampleRate = 0xFFFFFFFF;
            }
            pDescriptor->isFreqRangeContinuous=desc->isFreqRangeContinuous;
            pDescriptor->deviceConnection = desc->deviceConnection;
            pDescriptor->deviceScope = desc->deviceScope;
            pDescriptor->deviceLocation = desc->deviceLocation;
            pDescriptor->pDeviceName = desc->pDeviceName;
            /* other caps undefined */
            pDescriptor->isForTelephony = desc->isForTelephony;
            pDescriptor->samplingRatesSupported = desc->samplingRatesSupported;
            pDescriptor->numOfSamplingRatesSupported = desc->numOfSamplingRatesSupported;
         }
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_QueryAudioOutputCapabilities");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_RegisterAvailableAudioOutputsChangedCallback
 * Description: Sets or clears xaAvailableAudioOutputsChangedCallback().
 */
XAresult XAAudIODevCapaItfImpl_RegisterAvailableAudioOutputsChangedCallback(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            xaAvailableAudioOutputsChangedCallback callback,
                                            void* pContext)
{

    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_RegisterAvailableAudioOutputsChangedCallback");

    if( !impl )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        impl->outputCb = callback;
        impl->outputCbCtx = pContext;
        impl->outputCbPtrToSelf = self;
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_RegisterAvailableAudioOutputsChangedCallback");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_RegisterDefaultDeviceIDMapChangedCallback
 * Description: Sets or clears xaDefaultDeviceIDMapChangedCallback().
 */
XAresult XAAudIODevCapaItfImpl_RegisterDefaultDeviceIDMapChangedCallback(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            xaDefaultDeviceIDMapChangedCallback callback,
                                            void* pContext)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAAudIODevCapaItfImpl_RegisterDefaultDeviceIDMapChangedCallback");

    if( !impl )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        impl->deviceMapCb = callback;
        impl->deviceMapCtx = pContext;
        impl->deviceMapCbPtrToSelf = self;
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_RegisterDefaultDeviceIDMapChangedCallback");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioInputs
 * Description: This method returns an array of audio input devices physically
 * associated with this audio I/O device.
 */
XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioInputs(
                                                XAAudioIODeviceCapabilitiesItf self,
                                                XAuint32 deviceId,
                                                XAint32* pNumAudioInputs,
                                                XAuint32* pAudioInputDeviceIDs)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_GetAssociatedAudioInputs");
    if( !impl || !pNumAudioInputs )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {

        XACapabilities temp;
        XAuint32 associatedCount = 0;

        XAuint32 i = 0;
        for( i=0; i<impl->numInputDevices; i++ )
        {
            /* query device id from adaptation using index value */
            res = XACapabilitiesMgr_GetCapsByIdx(impl->capslist, (XACapsType)(XACAP_DEVSRC|XACAP_AUDIO), i, &temp);
            if (temp.xaid != deviceId)
            {
                associatedCount++;
            }
        }

        if (pAudioInputDeviceIDs)
        {
            if( *pNumAudioInputs < associatedCount )
            {
                DEBUG_ERR("XA_RESULT_BUFFER_INSUFFICIENT");
                DEBUG_API("<-XAAudIODevCapaItfImpl_GetAssociatedAudioInputs");
                return XA_RESULT_BUFFER_INSUFFICIENT;
            }

            for( i=0, associatedCount = 0; i<impl->numInputDevices; i++ )
            {
                /* query device id from adaptation using index value */
                res = XACapabilitiesMgr_GetCapsByIdx(impl->capslist, (XACapsType)(XACAP_DEVSRC|XACAP_AUDIO), i, &temp);
                if (temp.xaid != deviceId)
                {
                    pAudioInputDeviceIDs[associatedCount++] = temp.xaid;
                }
            }
        }

        *pNumAudioInputs = associatedCount;
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_GetAssociatedAudioInputs");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs
 * Description: This method returns an array of audio output devices physically
 * associated with this audio I/O device.
 */
XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs(
                                                XAAudioIODeviceCapabilitiesItf self,
                                                XAuint32 deviceId,
                                                XAint32* pNumAudioOutputs,
                                                XAuint32* pAudioOutputDeviceIDs)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs");

    if( !impl || !pNumAudioOutputs )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {

        XACapabilities temp;
        XAuint32 associatedCount = 0;

        XAuint32 i = 0;
        for( i=0; i<impl->numOutputDevices; i++ )
        {
            /* query device id from adaptation using index value */
            res = XACapabilitiesMgr_GetCapsByIdx(impl->capslist, (XACapsType)(XACAP_DEVSNK|XACAP_AUDIO), i, &temp);
            if (temp.xaid != deviceId)
            {
                associatedCount++;
            }
        }

        if (pAudioOutputDeviceIDs)
        {
            if( *pNumAudioOutputs < associatedCount )
            {
                DEBUG_ERR("XA_RESULT_BUFFER_INSUFFICIENT");
                DEBUG_API("<-XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs");
                return XA_RESULT_BUFFER_INSUFFICIENT;
            }

            for( i=0, associatedCount = 0; i<impl->numOutputDevices; i++ )
            {
                /* query device id from adaptation using index value */
                res = XACapabilitiesMgr_GetCapsByIdx(impl->capslist, (XACapsType)(XACAP_DEVSNK|XACAP_AUDIO), i, &temp);
                if (temp.xaid != deviceId)
                {
                    pAudioOutputDeviceIDs[associatedCount++] = temp.xaid;
                }
            }
        }

        *pNumAudioOutputs = associatedCount;
    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_GetDefaultAudioDevices
 * Gets the number of audio devices currently mapped to the given default device ID.
 */
XAresult XAAudIODevCapaItfImpl_GetDefaultAudioDevices(XAAudioIODeviceCapabilitiesItf self,
                                                      XAuint32 defaultDeviceID,
                                                      XAint32 *pNumAudioDevices,
                                                      XAuint32 *pAudioDeviceIDs)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_GetDefaultAudioDevices");

    if( !impl || !pNumAudioDevices )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
    if(!pAudioDeviceIDs)
        {
        switch(defaultDeviceID)
            {
            case XA_DEFAULTDEVICEID_AUDIOOUTPUT:
                *pNumAudioDevices = 1;
                break;
            case XA_DEFAULTDEVICEID_AUDIOINPUT :
                *pNumAudioDevices = 1;
                break;
            default:
                res = XA_RESULT_PARAMETER_INVALID;
                break;
            }
        }
    else
        {
        switch(defaultDeviceID)
            {
            case XA_DEFAULTDEVICEID_AUDIOOUTPUT:
                pAudioDeviceIDs[*pNumAudioDevices - 1] = XA_ADAPT_DEFAULTAUDIOOUTPUT;
                break;
            case XA_DEFAULTDEVICEID_AUDIOINPUT:
                pAudioDeviceIDs[*pNumAudioDevices - 1] = XA_ADAPT_DEFAULTAUDIOINPUT;
                break;
            default:
                res = XA_RESULT_PARAMETER_INVALID;
                break;
            }
        }      

    }
    DEBUG_API("<-XAAudIODevCapaItfImpl_GetDefaultAudioDevices");
    return res;
}

/* XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs
 * Description: Gets an array of sample formats supported by the audio I/O
 * device for the given sampling rate.
 */
XAresult XAAudIODevCapaItfImpl_QuerySampleFormatsSupported(
                                            XAAudioIODeviceCapabilitiesItf self,
                                            XAuint32 deviceId,
                                            XAmilliHertz samplingRate,
                                            XAint32* pSampleFormats,
                                            XAint32* pNumOfSampleFormats)
{
    XAAudIODevCapaItfImpl* impl = GetImpl(self);
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAudIODevCapaItfImpl_QuerySampleFormatsSupported");

    if( !impl || !pNumOfSampleFormats )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
    }
    else
    {

/*        XACapabilities temp;
        res = XACapabilitiesMgr_GetCapsById(NULL, (XACapsType)(XACAP_DEVSNK|XACAP_AUDIO),
                                            deviceId, &temp);
         deviceid can be either input or output
        if( res == XA_RESULT_FEATURE_UNSUPPORTED )
        {
            res = XACapabilitiesMgr_GetCapsById(NULL, (XACapsType)(XACAP_DEVSRC|XACAP_AUDIO),
                                                deviceId, &temp);
        }
        if( res == XA_RESULT_SUCCESS )
        {
            XAAudioOutputDescriptor* desc = ((XAAudioOutputDescriptor*)(temp.pEntry));
            XAuint32 count = 0, i = 0;
            for (i=0; i < sizeof(temp.pcmProfilesSupported)*8; i++)
                count+=temp.pcmProfilesSupported>>i&0x1;

            if(pSampleFormats)
            {
                if(*pNumOfSampleFormats<count)
                {
                    res = XA_RESULT_BUFFER_INSUFFICIENT;
                }
                else
                {
                    XAuint32 insertCount = 0;
                    if (temp.pcmProfilesSupported & XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_8) {
                        pSampleFormats[insertCount++] = XA_PCMSAMPLEFORMAT_FIXED_8; }
                    if (temp.pcmProfilesSupported & XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_16) {
                        pSampleFormats[insertCount++] = XA_PCMSAMPLEFORMAT_FIXED_16; }
                    if (temp.pcmProfilesSupported & XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_20) {
                        pSampleFormats[insertCount++] = XA_PCMSAMPLEFORMAT_FIXED_20; }
                    if (temp.pcmProfilesSupported & XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_24) {
                        pSampleFormats[insertCount++] = XA_PCMSAMPLEFORMAT_FIXED_20; }
                    if (temp.pcmProfilesSupported & XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_28) {
                        pSampleFormats[insertCount++] = XA_PCMSAMPLEFORMAT_FIXED_20; }
                    if (temp.pcmProfilesSupported & XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_32) {
                        pSampleFormats[insertCount++] = XA_PCMSAMPLEFORMAT_FIXED_20; }
                }
            }
            *pNumOfSampleFormats = count;
        }*/


    if(!pSampleFormats)
        {
        *pNumOfSampleFormats = 1;
        }
    else
        {
        pSampleFormats[*pNumOfSampleFormats - 1] = XA_PCMSAMPLEFORMAT_FIXED_16;
        }

    }

    DEBUG_API("<-XAAudIODevCapaItfImpl_QuerySampleFormatsSupported");
    return res;
}

/**
 * XAAudIODevCapaItfImpl -specific methods
 **/

/* XAAudIODevCapaItfImpl_Create
 * Description: Allocate and initialize XAAudIODevCapaItfImpl
 */
XAAudIODevCapaItfImpl* XAAudIODevCapaItfImpl_Create(XACapabilities* caps)
{
    XAAudIODevCapaItfImpl* self = (XAAudIODevCapaItfImpl*)
        calloc(1,sizeof(XAAudIODevCapaItfImpl));
    DEBUG_API("->XAAudIODevCapaItfImpl_Create");

    if( self )
    {
        /* init itf default implementation */
        self->itf.GetAssociatedAudioInputs =
            XAAudIODevCapaItfImpl_GetAssociatedAudioInputs;
        self->itf.GetAssociatedAudioOutputs =
            XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs;
        self->itf.GetAvailableAudioInputs =
            XAAudIODevCapaItfImpl_GetAvailableAudioInputs;
        self->itf.GetAvailableAudioOutputs =
            XAAudIODevCapaItfImpl_GetAvailableAudioOutputs;
        self->itf.QueryAudioInputCapabilities =
            XAAudIODevCapaItfImpl_QueryAudioInputCapabilities;
        self->itf.QueryAudioOutputCapabilities =
            XAAudIODevCapaItfImpl_QueryAudioOutputCapabilities;
        self->itf.GetDefaultAudioDevices =
            XAAudIODevCapaItfImpl_GetDefaultAudioDevices;
        self->itf.QuerySampleFormatsSupported =
            XAAudIODevCapaItfImpl_QuerySampleFormatsSupported;
        self->itf.RegisterAvailableAudioInputsChangedCallback =
            XAAudIODevCapaItfImpl_RegisterAvailableAudioInputsChangedCallback;
        self->itf.RegisterAvailableAudioOutputsChangedCallback =
            XAAudIODevCapaItfImpl_RegisterAvailableAudioOutputsChangedCallback;
        self->itf.RegisterDefaultDeviceIDMapChangedCallback =
            XAAudIODevCapaItfImpl_RegisterDefaultDeviceIDMapChangedCallback;
        self->capslist = caps;
        /* init variables */
        XACapabilitiesMgr_GetCapsCount( caps, (XACapsType)(XACAP_DEVSNK|XACAP_AUDIO),
                                  &(self->numOutputDevices) );
        XACapabilitiesMgr_GetCapsCount( caps, (XACapsType)(XACAP_DEVSRC|XACAP_AUDIO),
                                  &(self->numInputDevices) );
        
        /*TODO: Remove this later*/
        XACapabilitiesMgr_QueryColorFormats(caps, NULL, NULL);

        self->inputCbPtrToSelf = NULL;
        self->outputCbPtrToSelf = NULL;
        self->deviceMapCbPtrToSelf = NULL;
        self->self = self;
    }
    DEBUG_API("<-XAAudIODevCapaItfImpl_Create");
    return self;
}

/* void XAAudIODevCapaItfImpl_Free
 * Description: Free all resources reserved at XAAudIODevCapaItfImpl_Create
 */
void XAAudIODevCapaItfImpl_Free(XAAudIODevCapaItfImpl* self)
{
    DEBUG_API("->XAAudIODevCapaItfImpl_Free");
    free(self);
    DEBUG_API("<-XAAudIODevCapaItfImpl_Free");
}
