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
 * Description: Common Openmax AL utility functions
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaengine.h"
/*
 *  compare two XAInterfaceIDs, return true if same
 */
XAboolean XACommon_EqualIIds(XAInterfaceID idA, XAInterfaceID idB)
    {
    XAboolean nodeOk;
    XAuint16 nodeIdx;
    if (idA == idB)
        return XA_BOOLEAN_TRUE;
    if (!idA || !idB)
        return XA_BOOLEAN_FALSE;
    if (idA->time_low == idB->time_low && idA->time_mid == idB->time_mid
            && idA->time_hi_and_version == idB->time_hi_and_version
            && idA->clock_seq == idB->clock_seq)
        {
        nodeOk = XA_BOOLEAN_TRUE;
        for (nodeIdx = 0; nodeIdx < sizeof(idA->node); nodeIdx++)
            {
            if (idA->node[nodeIdx] != idB->node[nodeIdx])
                {
                nodeOk = XA_BOOLEAN_FALSE;
                break;
                }
            }
        if (nodeOk)
            {
            return XA_BOOLEAN_TRUE;
            }
        }
    return XA_BOOLEAN_FALSE;
    }

/* Check source availability and content type. Return error only if we are sure
 * content is not valid, otherwise let adaptation try to identify and use content.
 * @param type: on return, contains content type, if known.
 * @return Success code
 */
XAresult XACommon_CheckDataSource(XADataSource* source, XAMediaType* type)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAchar* fname = NULL;
    XAchar* mime = NULL;
    XAboolean isLocal = XA_BOOLEAN_FALSE;

    if (!source || !source->pLocator)
        {
        DEBUG_ERR("Invalid XADataSource structure!!");
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* check URI availability */
    if (*(XAuint32*) (source->pLocator) == XA_DATALOCATOR_URI)
        {
        fname = (XAchar*) ((XADataLocator_URI*) (source->pLocator))->URI;
        /* check protocol prefix */
        if (strstr((char*) fname, "://"))
            {
            if (strncmp((char*) fname, "file:///", 8) == 0)
                {
                isLocal = XA_BOOLEAN_TRUE;
                fname += 8;
                }
            else
                { /* add support for other protocols if needed */
                isLocal = XA_BOOLEAN_FALSE;
                }
            }
        else
            {
            isLocal = XA_BOOLEAN_TRUE;
            }
        if (isLocal)
            {
            FILE* file = fopen((char*) fname, "r");
            if (!file)
                {
                DEBUG_INFO_A1_STR("%s not found from filesystem", (char *)fname);
                ret = XA_RESULT_CONTENT_NOT_FOUND;
                }
            else
                {
                fclose(file);
                }
            }
        }

    /* check content type */
    if (type)
        {
        if (source->pFormat)
            {
            switch (*(XAuint32*) source->pFormat)
                {
                case XA_DATAFORMAT_MIME:
                    {
                    /* try to parse mimetype first */
                    mime = ((XADataFormat_MIME*) (source->pFormat))->mimeType;
                    if (mime && (strncmp((char*) mime, "audio", 5) == 0))
                        {
                        *type = XA_MEDIATYPE_AUDIO;
                        }
                    else if (mime && (strncmp((char*) mime, "video", 5) == 0))
                        {
                        *type = XA_MEDIATYPE_VIDEO;
                        }
                    else if (mime && (strncmp((char*) mime, "image", 5) == 0))
                        {
                        *type = XA_MEDIATYPE_IMAGE;
                        }
                    else
                        {
                        /* try to parse containertype next */
                        switch (((XADataFormat_MIME*) (source->pFormat))->containerType)
                            {
                            case XA_CONTAINERTYPE_M4A:
                            case XA_CONTAINERTYPE_MP3:
                            case XA_CONTAINERTYPE_WAV:
                            case XA_CONTAINERTYPE_XMF_0:
                            case XA_CONTAINERTYPE_XMF_1:
                            case XA_CONTAINERTYPE_XMF_2:
                            case XA_CONTAINERTYPE_XMF_3:
                            case XA_CONTAINERTYPE_XMF_GENERIC:
                            case XA_CONTAINERTYPE_AMR:
                            case XA_CONTAINERTYPE_AAC:
                            case XA_CONTAINERTYPE_3GA:
                            case XA_CONTAINERTYPE_DMF:
                            case XA_CONTAINERTYPE_SMF:
                            case XA_CONTAINERTYPE_MOBILE_DLS:
                                *type = XA_MEDIATYPE_AUDIO;
                                break;

                            case XA_CONTAINERTYPE_ASF:
                            case XA_CONTAINERTYPE_AVI:
                            case XA_CONTAINERTYPE_MP4:
                            case XA_CONTAINERTYPE_MPEG_ES:
                            case XA_CONTAINERTYPE_MPEG_PS:
                            case XA_CONTAINERTYPE_MPEG_TS:
                            case XA_CONTAINERTYPE_QT:
                            case XA_CONTAINERTYPE_3GPP:
                            case XA_CONTAINERTYPE_RM:
                                *type = XA_MEDIATYPE_VIDEO;
                                break;

                            case XA_CONTAINERTYPE_BMP:
                            case XA_CONTAINERTYPE_JPG:
                            case XA_CONTAINERTYPE_JPG2000:
                                *type = XA_MEDIATYPE_IMAGE;
                                break;

                            case XA_CONTAINERTYPE_RAW:
                            case XA_CONTAINERTYPE_UNSPECIFIED:
                            default:
                                *type = XA_MEDIATYPE_UNKNOWN;
                                break;
                            }
                        }
                    }
                    break;

                case XA_DATAFORMAT_PCM:
                    *type = XA_MEDIATYPE_AUDIO;
                    break;

                case XA_DATAFORMAT_RAWIMAGE:
                    *type = XA_MEDIATYPE_IMAGE;
                    break;

                default:
                    *type = XA_MEDIATYPE_UNKNOWN;
                    DEBUG_INFO("Warning: unknown dataformat")
                    ;
                    break;
                }
            }
        else
            {
            DEBUG_ERR("Warning: no data format set");
            *type = XA_MEDIATYPE_UNKNOWN;
            }

        /* Expand this list if add new supported media type */
        /* If media type unknow check it from file extension */
        if (*type == XA_MEDIATYPE_UNKNOWN && fname)
            {
            if ((strstr((char*) fname, ".wav")) || (strstr((char*) fname,
                    ".mid")) || (strstr((char*) fname, ".amr")))
                {
                *type = XA_MEDIATYPE_AUDIO;
                }
            if ((strstr((char*) fname, ".avi")) || (strstr((char*) fname,
                    ".ogg")))
                {

                }
            if (strstr((char*) fname, ".jpg"))
                {
                *type = XA_MEDIATYPE_IMAGE;
                }
            }
        }
    return ret;
    }

/* Validate XADataSink and XADataSrc object creation parameters.
 * @return Success code
 */
XAresult XACommon_ValidateDataLocator(XAuint32 pCounter, void* pSnk, ...)
    {
    va_list argptr;
    XAuint32 counter = 0;
    XADataLocator_URI* uri = NULL;
    XADataLocator_NativeDisplay* display = NULL;
    XADataLocator_OutputMix* omix = NULL;
    XADataLocator_Address* address = NULL;
    XADataLocator_IODevice* iodevice = NULL;
    XAresult ret = XA_RESULT_SUCCESS;
    XADataSink* xaSnk = NULL;
    DEBUG_API("->XACommon_ValidateDataLocator");

    xaSnk = (XADataSink*) pSnk;

    /* Initialize variable arguments. */
    va_start (argptr, pSnk);
    while (counter < pCounter)
        {
        if (counter > 0)
            {
            /*XADataSink and XADataSrc are equivalent, use XADataSink for both*/
            xaSnk = va_arg(argptr, XADataSink*);
            }
        if( !xaSnk )
            { /*NULL sinks/sources are possible, not checked here*/
            counter++;
            continue;
            }
        else if( !xaSnk->pLocator )
            {
            DEBUG_ERR_A1("NULL XADataLocator in parameter %d",(int)counter);
            ret = XA_RESULT_PARAMETER_INVALID;
            va_end(argptr);
            DEBUG_API("<-XACommon_ValidateDataLocator");
            return ret;
            }
        switch ( *(XAuint32*)(xaSnk->pLocator) )
            {
            case XA_DEFAULTDEVICEID_AUDIOOUTPUT:
                DEBUG_INFO("XA_DEFAULTDEVICEID_AUDIOOUTPUT");
                uri = (XADataLocator_URI*)xaSnk->pLocator;
                break;
            case XA_DATALOCATOR_URI:
                DEBUG_INFO("XA_DATALOCATOR_URI");
                uri = (XADataLocator_URI*)xaSnk->pLocator;
                if( !uri->URI && !xaSnk->pFormat )
                    {
                    DEBUG_ERR("Invalid parameter set for uri ");
                    ret = XA_RESULT_PARAMETER_INVALID;
                    }
                break;
            case XA_DATALOCATOR_NATIVEDISPLAY:
                DEBUG_INFO("Sink locator type - XA_DATALOCATOR_NATIVEDISPLAY");
                display = (XADataLocator_NativeDisplay*)(xaSnk->pLocator);
                if( !display->hDisplay || !display->hWindow )
                    {
                    /*No action at this time*/
                    DEBUG_INFO("Warning - no display or window handle set");
                    }
                break;
            case XA_DATALOCATOR_OUTPUTMIX:
                DEBUG_INFO("Sink locator type - XA_DATALOCATOR_OUTPUTMIX");
                    {
                    omix = (XADataLocator_OutputMix*)(xaSnk->pLocator);
                    if ( !omix->outputMix )
                        {
                        DEBUG_ERR("Invalid parameter set for outputmix");
                        ret = XA_RESULT_PARAMETER_INVALID;
                        }
                    }
                break;
            case XA_DATALOCATOR_ADDRESS:
                DEBUG_INFO("XA_DATALOCATOR_ADDRESS");
                address = (XADataLocator_Address*)(xaSnk->pLocator);
                if( !address->pAddress && !xaSnk->pFormat )
                    {
                    DEBUG_ERR("Invalid parameter set for address");
                    ret = XA_RESULT_PARAMETER_INVALID;
                    }
                break;
            case XA_DATALOCATOR_IODEVICE:
                DEBUG_INFO("XA_DATALOCATOR_IODEVICE");
                iodevice = (XADataLocator_IODevice*)(xaSnk->pLocator);
                if( iodevice->device )
                    {
                    DEBUG_INFO("Media object as IODevice");
                    }
                else
                    {
                    DEBUG_INFO("Iodevice id used");
                    }
                break;
            default:
                DEBUG_ERR("Incorrect data locator for sink.");
                ret = XA_RESULT_PARAMETER_INVALID;
                break;
            }
        counter ++;
        }
    va_end(argptr);
    DEBUG_API("<-XACommon_ValidateDataLocator");
    return ret;
    }
