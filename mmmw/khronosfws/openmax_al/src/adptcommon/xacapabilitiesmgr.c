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
 * Description: Capability Manager to query capabilities from different
 *              adaptations and store it in a linked list.
 *
 */

#include "xacapabilitiesmgr.h"
#include "xammfcapabilitiesmgr.h"
#include "xagstcapabilitiesmgr.h"
#include "xaadptbasectx.h"
#include <stdio.h>
#include <string.h>

static XAresult XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities(
        XACapabilities **ppNode);
static XAresult XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities(
        XACapabilities **ppNode);
static XAresult XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities(
        XACapabilities **ppNode);
static XAresult XACapabilitiesMgr_GetAudioOutputDeviceCapabilities(
        XACapabilities **ppNode);

/* XAresult XAGSTCapabilitiesMgr_UpdateAudioIOCapabilitieList
 * Description: Update the capabilities list supported by the engine.
 */
XAresult XACapabilitiesMgr_UpdateAudioIOCapabilitieList(
        FrameworkMap *frameworkMap, XACapabilities **ppListHead)

    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *lastNode = NULL;
    XACapabilities *firstNode = NULL;
    XACapabilities *newNode = NULL;

    DEBUG_API("->XACapabilitiesMgr_UpdateAudioIOCapabilitieList");

    if (!frameworkMap || !ppListHead)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_UpdateAudioIOCapabilitieList");
        return XA_RESULT_PARAMETER_INVALID;
        }

    res = XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities(&newNode);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
        DEBUG_API("<-XACapabilitiesMgr_UpdateAudioIOCapabilitieList");
        return res;
        }

    if (!firstNode)
        {
        firstNode = newNode;
        lastNode = newNode;
        }
    
    res = XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities(&newNode);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
        DEBUG_API("<-XACapabilitiesMgr_UpdateAudioIOCapabilitieList");
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
    
    res = XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities(&newNode);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
        DEBUG_API("<-XACapabilitiesMgr_UpdateAudioIOCapabilitieList");
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

    res = XACapabilitiesMgr_GetAudioOutputDeviceCapabilities(&newNode);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&firstNode);
        DEBUG_API("<-XACapabilitiesMgr_UpdateAudioIOCapabilitieList");
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

    /* if we have some iodevice capabilities in the list */
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
    DEBUG_API("<-XACapabilitiesMgr_UpdateAudioIOCapabilitieList");
    return res;
    }

XAresult XACapabilitiesMgr_CreateCapabilitieList(FrameworkMap* frameworkMap,
        XACapabilities** ppListHead)
    {
    XACapabilities* list = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XACapabilitiesMgr_CreateCapabilitieList");
    if (!frameworkMap || !ppListHead)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_CreateCapabilitieList");
        return XA_RESULT_PARAMETER_INVALID;
        }

    *ppListHead = NULL;

    res = XACapabilitiesMgr_UpdateAudioIOCapabilitieList(frameworkMap, &list);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&list);
        DEBUG_API("<-XACapabilitiesMgr_CreateCapabilitieList");
        return res;
        }

    res = XAGSTCapabilitiesMgr_UpdateCapabilitieList(frameworkMap, &list);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&list);
        DEBUG_API("<-XACapabilitiesMgr_CreateCapabilitieList");
        return res;
        }

    res = XAMMFCapabilitiesMgr_UpdateCapabilitieList(frameworkMap, &list);
    if (res != XA_RESULT_SUCCESS)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&list);
        DEBUG_API("<-XACapabilitiesMgr_CreateCapabilitieList");
        return res;
        }

    *ppListHead = list;
    DEBUG_API("<-XACapabilitiesMgr_CreateCapabilitieList");
    return res;
    }

XAresult XACapabilitiesMgr_DeleteCapabilitieList(XACapabilities** ppListHead)
    {
    XACapabilities* currNode = NULL;
    XACapabilities* nextNode = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XACapabilitiesMgr_DeleteCapabilitieList");
    if (!ppListHead)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_DeleteCapabilitieList");
        return XA_RESULT_PARAMETER_INVALID;
        }

    currNode = *ppListHead;
    while (currNode)
        {
        if (currNode->capsType == AUD_I)
            {
            XAAudioInputDescriptor* entries =
                    (XAAudioInputDescriptor*) currNode->pEntry;
            XAint32 i;
            for (i = 0; i < currNode->noOfEntries; i++)
                {
                free(entries[i].samplingRatesSupported);
                free(entries[i].deviceName);
                }
            free(entries);
            }
        else if (currNode->capsType == AUD_O)
            {
            XAAudioOutputDescriptor* entries =
                    (XAAudioOutputDescriptor*) currNode->pEntry;
            XAint32 i;
            for (i = 0; i < currNode->noOfEntries; i++)
                {
                free(entries[i].samplingRatesSupported);
                free(entries[i].pDeviceName);
                }
            free(entries);
            }
        else if (currNode->capsType == AUD_E)
            {
            XAAudioCodecDescriptor* entries =
                    (XAAudioCodecDescriptor*) currNode->pEntry;
            XAint32 i;
            for (i = 0; i < currNode->noOfEntries; i++)
                {
                free(entries[i].pSampleRatesSupported);
                free(entries[i].pBitratesSupported);
                }
            free(entries);
            }

        if (currNode->adaptId)
            {
            free(currNode->adaptId);
            }

        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
        }

    *ppListHead = NULL;

    DEBUG_API("<-XACapabilitiesMgr_DeleteCapabilitieList");
    return res;
    }

/* XAresult XACapabilitiesMgr_GetCapsCount
 * Description: Count capabilities of certain type. Filter is specified by
 *              bitmasking XACapsType values.
 */
XAresult XACapabilitiesMgr_GetCapsCount(XACapabilities* pListHead,
        XACapsType filter, XAuint32* count)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities* currNode = pListHead;

    DEBUG_API("->XACapabilitiesMgr_GetCapsCount");
    if (!currNode || !count)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_GetCapsCount");
        return XA_RESULT_PARAMETER_INVALID;
        }

    (*count) = 0;
    while (currNode)
        {
        if ((currNode->capsType & filter) == filter)
            {
            (*count)++;
            }
        currNode = currNode->next;
        }

    DEBUG_API("<-XACapabilitiesMgr_GetCapsCount");
    return res;
    }

/* XAresult XACapabilitiesMgr_GetCapsById
 * Description: Get capabilities of type XACapsType and matching id
 */
XAresult XACapabilitiesMgr_GetCapsById(XACapabilities* pListHead,
        XACapsType filter, XAuint32 maxId, XACapabilities* data)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities* currNode = pListHead;
    XAboolean found = XA_BOOLEAN_FALSE;

    DEBUG_API("->XACapabilitiesMgr_GetCapsById");

    if (!currNode)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_GetCapsById");
        return XA_RESULT_PARAMETER_INVALID;
        }

    while (currNode)
        {
        if (((currNode->capsType & filter) == filter) && (maxId
                == currNode->xaid))
            {
            memcpy(data, currNode, sizeof(XACapabilities));
            found = XA_BOOLEAN_TRUE;
            break;
            }
        currNode = currNode->next;
        }

    if (!found)
        {
        DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
        DEBUG_API("<-XACapabilitiesMgr_GetCapsById");
        return XA_RESULT_FEATURE_UNSUPPORTED;
        }

    DEBUG_API("<-XACapabilitiesMgr_GetCapsById");
    return res;
    }

/* XAresult XACapabilitiesMgr_GetCapsByIdx
 * Description: Get n'th capabilities of type XACapsType
 */
XAresult XACapabilitiesMgr_GetCapsByIdx(XACapabilities* pListHead,
        XACapsType filter, XAuint32 idx, XACapabilities* data)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities* currNode = pListHead;
    XAboolean found = XA_BOOLEAN_FALSE;
    XAuint32 j = 0;

    DEBUG_API("->XACapabilitiesMgr_GetCapsByIdx");

    if (!currNode)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_GetCapsByIdx");
        return XA_RESULT_PARAMETER_INVALID;
        }

    while (currNode)
        {
        if ((currNode->capsType & filter) == filter)
            {
            if (idx == j++)
                {
                memcpy(data, currNode, sizeof(XACapabilities));
                found = XA_BOOLEAN_TRUE;
                break;
                }
            }
        currNode = currNode->next;
        }

    if (!found)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_GetCapsByIdx");
        return XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API("<-XACapabilitiesMgr_GetCapsByIdx");
    return res;
    }

/* XAresult XACapabilitiesMgr_QueryColorFormats
 * Description: Get color formats associated with the XA_IMAGECODEC_RAW codec.
 */
XAresult XACapabilitiesMgr_QueryColorFormats(XACapabilities* pListHead,
        XAuint32* pIndex, XAuint32* pColorFormats)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities temp;

    DEBUG_API("->XACapabilitiesMgr_QueryColorFormats");

    if (!pIndex)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XACapabilitiesMgr_QueryColorFormats");
        return XA_RESULT_PARAMETER_INVALID;
        }

    res = XACapabilitiesMgr_GetCapsById(pListHead,
            (XACapsType) ((XACapsType) (XACAP_ENCODER | XACAP_IMAGE)),
            XA_IMAGECODEC_RAW, &temp);
    if (res == XA_RESULT_SUCCESS)
        {
        if (!pColorFormats)
            { /* query number of color formats */
            *pIndex = 1; /* one used by camera context */
            }
        else
            { /* query color format */
            if (*pIndex >= 1) /* one used by camera context */
                {
                DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
                res = XA_RESULT_PARAMETER_INVALID;
                }
            else
                { /* internal format used by pipeline, look camera context for details  */
                *pColorFormats = XA_COLORFORMAT_YUV420PLANAR;
                }
            }
        }
    else
        {
        *pIndex = 0;
        }

    DEBUG_API("<-XACapabilitiesMgr_QueryColorFormats");
    return res;
    }

/* Add AudioInputDeviceCaps */
XAresult XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities(
        XACapabilities** ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAAudioInputDescriptor *entries = NULL;
    XAchar inpDeviceName[] = "Default Mic";
    int strLen = 0;

    DEBUG_API("->XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_I;
    newNode->xaid = XA_ADAPTID_DEVSOUNDSRC_MIC;
    newNode->noOfEntries = 1;

    /* Allocate array */
    entries = (XAAudioInputDescriptor*) calloc(1,
            sizeof(XAAudioInputDescriptor));
    if (!entries)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->pEntry = (void*) entries;

    strLen = strlen((char*) inpDeviceName);
    entries->deviceName = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!entries->deviceName)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    strncpy((char*) entries->deviceName, (char*) inpDeviceName, strLen);
    entries->deviceName[strLen] = '\0'; /*Null terminate it*/
    entries->deviceConnection = XA_DEVCONNECTION_INTEGRATED;
    entries->deviceScope = XA_DEVSCOPE_ENVIRONMENT;
    entries->deviceLocation = XA_DEVLOCATION_HANDSET;
    entries->isForTelephony = XA_BOOLEAN_FALSE;
    entries->minSampleRate = 8000000; /* milliHz */
    entries->maxSampleRate = 48000000; /* milliHz */
    entries->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    entries->numOfSamplingRatesSupported = 5;
    entries->samplingRatesSupported = (XAmilliHertz*) calloc(
            entries->numOfSamplingRatesSupported, sizeof(XAmilliHertz));
    if (!entries->samplingRatesSupported)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    /* entries in milliHz */
    entries->samplingRatesSupported[0] = 8000000;
    entries->samplingRatesSupported[1] = 16000000;
    entries->samplingRatesSupported[2] = 24000000;
    entries->samplingRatesSupported[3] = 32000000;
    entries->samplingRatesSupported[4] = 48000000;
    entries->maxChannels = 2;

    newNode->pEntry = (void*) entries;

    *ppNode = newNode;
    DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceMicCapabilities");
    return res;
    }

/* Add AudioInputDeviceCaps */
XAresult XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities(
        XACapabilities** ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAAudioInputDescriptor *entries = NULL;
    XAchar inpDeviceName[] = "FMRadio Recvr";
    int strLen = 0;

    DEBUG_API("->XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_I;
    newNode->xaid = XA_ADAPTID_DEVSOUNDSRC_FMRX;
    newNode->noOfEntries = 1;

    /* Allocate array */
    entries = (XAAudioInputDescriptor*) calloc(1,
            sizeof(XAAudioInputDescriptor));
    if (!entries)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->pEntry = (void*) entries;

    strLen = strlen((char*) inpDeviceName);
    entries->deviceName = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!entries->deviceName)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    strncpy((char*) entries->deviceName, (char*) inpDeviceName, strLen);
    entries->deviceName[strLen] = '\0'; /*Null terminate it*/
    entries->deviceConnection = XA_DEVCONNECTION_INTEGRATED;
    entries->deviceScope = XA_DEVSCOPE_ENVIRONMENT;
    entries->deviceLocation = XA_DEVLOCATION_HANDSET;
    entries->isForTelephony = XA_BOOLEAN_FALSE;
    entries->minSampleRate = 8000000; /* milliHz */
    entries->maxSampleRate = 48000000; /* milliHz */
    entries->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    entries->numOfSamplingRatesSupported = 5;
    entries->samplingRatesSupported = (XAmilliHertz*) calloc(
            entries->numOfSamplingRatesSupported, sizeof(XAmilliHertz));
    if (!entries->samplingRatesSupported)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    /* entries in milliHz */
    entries->samplingRatesSupported[0] = 8000000;
    entries->samplingRatesSupported[1] = 16000000;
    entries->samplingRatesSupported[2] = 24000000;
    entries->samplingRatesSupported[3] = 32000000;
    entries->samplingRatesSupported[4] = 48000000;
    entries->maxChannels = 2;

    newNode->pEntry = (void*) entries;

    *ppNode = newNode;
    DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceFMRxCapabilities");
    return res;
    }

/* Add AudioInputDeviceCaps */
XAresult XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities(
        XACapabilities** ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAAudioInputDescriptor *entries = NULL;
    XAchar inpDeviceName[] = "Call";
    int strLen = 0;

    DEBUG_API("->XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_I;
    newNode->xaid = XA_ADAPTID_DEVSOUNDSRC_CALL;
    newNode->noOfEntries = 1;

    /* Allocate array */
    entries = (XAAudioInputDescriptor*) calloc(1,
            sizeof(XAAudioInputDescriptor));
    if (!entries)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->pEntry = (void*) entries;

    strLen = strlen((char*) inpDeviceName);
    entries->deviceName = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!entries->deviceName)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    strncpy((char*) entries->deviceName, (char*) inpDeviceName, strLen);
    entries->deviceName[strLen] = '\0'; /*Null terminate it*/
    entries->deviceConnection = XA_DEVCONNECTION_INTEGRATED;
    entries->deviceScope = XA_DEVSCOPE_ENVIRONMENT;
    entries->deviceLocation = XA_DEVLOCATION_HANDSET;
    entries->isForTelephony = XA_BOOLEAN_FALSE;
    entries->minSampleRate = 8000000; /* milliHz */
    entries->maxSampleRate = 48000000; /* milliHz */
    entries->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    entries->numOfSamplingRatesSupported = 5;
    entries->samplingRatesSupported = (XAmilliHertz*) calloc(
            entries->numOfSamplingRatesSupported, sizeof(XAmilliHertz));
    if (!entries->samplingRatesSupported)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    /* entries in milliHz */
    entries->samplingRatesSupported[0] = 8000000;
    entries->samplingRatesSupported[1] = 16000000;
    entries->samplingRatesSupported[2] = 24000000;
    entries->samplingRatesSupported[3] = 32000000;
    entries->samplingRatesSupported[4] = 48000000;
    entries->maxChannels = 2;

    newNode->pEntry = (void*) entries;

    *ppNode = newNode;
    DEBUG_API("<-XACapabilitiesMgr_GetAudioInputDeviceCallCapabilities");
    return res;
    }

XAresult XACapabilitiesMgr_GetAudioOutputDeviceCapabilities(
        XACapabilities **ppNode)
    {
    XAresult res = XA_RESULT_SUCCESS;
    XACapabilities *newNode = NULL;
    XAAudioOutputDescriptor *entries = NULL;
    XAchar outputDeviceName[] = "Default Speaker";
    int strLen = 0;

    DEBUG_API("->XACapabilitiesMgr_GetAudioOutputDeviceCapabilities");
    newNode = (XACapabilities *) calloc(1, sizeof(XACapabilities));
    if (!newNode)
        {
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioOutputDeviceCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->capsType = AUD_O;
    newNode->xaid = XA_ADAPTID_DEVSOUNDSINK;
    newNode->noOfEntries = 1;

    /* Allocate array */
    entries = (XAAudioOutputDescriptor*) calloc(1,
            sizeof(XAAudioOutputDescriptor));
    if (!entries)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioOutputDeviceCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }

    newNode->pEntry = (void*) entries;

    strLen = strlen((char*) outputDeviceName);
    entries->pDeviceName = (XAchar *) calloc(strLen + 1, sizeof(XAchar));
    if (!entries->pDeviceName)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioOutputDeviceCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    strncpy((char*) entries->pDeviceName, (char*) outputDeviceName, strLen);
    entries->pDeviceName[strLen] = '\0'; /*Null terminate it*/
    entries->deviceConnection = XA_DEVCONNECTION_INTEGRATED;
    entries->deviceScope = XA_DEVSCOPE_ENVIRONMENT;
    entries->deviceLocation = XA_DEVLOCATION_HANDSET;
    entries->isForTelephony = XA_BOOLEAN_FALSE;
    entries->minSampleRate = 8000000; /* milliHz */
    entries->maxSampleRate = 48000000; /* milliHz */
    entries->isFreqRangeContinuous = XA_BOOLEAN_FALSE;
    entries->numOfSamplingRatesSupported = 5;
    entries->samplingRatesSupported = (XAmilliHertz*) calloc(
            entries->numOfSamplingRatesSupported, sizeof(XAmilliHertz));
    if (!entries->samplingRatesSupported)
        {
        XACapabilitiesMgr_DeleteCapabilitieList(&newNode);
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XACapabilitiesMgr_GetAudioOutputDeviceCapabilities");
        return XA_RESULT_MEMORY_FAILURE;
        }
    /* entries in milliHz */
    entries->samplingRatesSupported[0] = 8000000;
    entries->samplingRatesSupported[1] = 16000000;
    entries->samplingRatesSupported[2] = 24000000;
    entries->samplingRatesSupported[3] = 32000000;
    entries->samplingRatesSupported[4] = 48000000;
    entries->maxChannels = 2;

    newNode->pEntry = (void*) entries;

    *ppNode = newNode;

    DEBUG_API("<-XACapabilitiesMgr_GetAudioOutputDeviceCapabilities");
    return res;
    }
