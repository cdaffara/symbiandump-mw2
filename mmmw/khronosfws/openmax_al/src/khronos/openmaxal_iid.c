/*
 * Copyright (c) 2007-2009 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and/or associated documentation files (the
 * "Materials "), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to 
 * the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 * OpenMAXAL_IID.c - OpenMAX AL version 1.0
 *
 */

/****************************************************************************/
/* NOTE: This file is a standard OpenMAX AL file and should not be          */
/* modified in any way.                                                     */
/****************************************************************************/
#include "OpenMAXAL.h"

/*****************************************************************************/
/* Interface IDs                                                             */
/*****************************************************************************/

static const struct XAInterfaceID_ XA_IID_NULL_ = { 0xec7178ec, 0xe5e1, 0x4432, 0xa3f4, { 0x46, 0x57, 0xe6, 0x79, 0x52, 0x10 } };
XA_API const XAInterfaceID XA_IID_NULL = &XA_IID_NULL_;

static const struct XAInterfaceID_ XA_IID_AUDIODECODERCAPABILITIES_ = { 0xdeac0cc0, 0x3995, 0x11dc, 0x8872, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_AUDIODECODERCAPABILITIES = &XA_IID_AUDIODECODERCAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_AUDIOENCODER_ = { 0xebbab900, 0x3997, 0x11dc, 0x891f, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_AUDIOENCODER = &XA_IID_AUDIOENCODER_;

static const struct XAInterfaceID_ XA_IID_AUDIOENCODERCAPABILITIES_ = { 0x83fbc600, 0x3998, 0x11d, 0x8f6d, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_AUDIOENCODERCAPABILITIES = &XA_IID_AUDIOENCODERCAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_AUDIOIODEVICECAPABILITIES_ = { 0x2b276d00, 0xf775, 0x11db, 0xa963, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_AUDIOIODEVICECAPABILITIES = &XA_IID_AUDIOIODEVICECAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_CAMERA_ = { 0xc7b84d20, 0xdf00, 0x11db, 0xba87, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_CAMERA = &XA_IID_CAMERA_;

static const struct XAInterfaceID_ XA_IID_CAMERACAPABILITIES_ = { 0x01cab1c0, 0xe86a, 0x11db, 0xa5b9, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_CAMERACAPABILITIES = &XA_IID_CAMERACAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_CONFIGEXTENSION_ = { 0x6dc22ea0, 0xdf03, 0x11db, 0xbed7, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_CONFIGEXTENSION = &XA_IID_CONFIGEXTENSION_;

static const struct XAInterfaceID_ XA_IID_DEVICEVOLUME_ = { 0x4bb44020, 0xf775, 0x11db, 0xad03, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_DEVICEVOLUME = &XA_IID_DEVICEVOLUME_;

static const struct XAInterfaceID_ XA_IID_DYNAMICINTERFACEMANAGEMENT_ = { 0x6e2340c0, 0xf775, 0x11db, 0x85da, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_DYNAMICINTERFACEMANAGEMENT = &XA_IID_DYNAMICINTERFACEMANAGEMENT_;

static const struct XAInterfaceID_ XA_IID_DYNAMICSOURCE_ = { 0xc88d5480, 0x3a12, 0x11dc, 0x80a2, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_DYNAMICSOURCE = &XA_IID_DYNAMICSOURCE_;

static const struct XAInterfaceID_ XA_IID_ENGINE_ = { 0x45c58f40, 0xdf04, 0x11db, 0x9e76, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_ENGINE = &XA_IID_ENGINE_;

static const struct XAInterfaceID_ XA_IID_EQUALIZER_ = { 0x7ad86d40, 0xf775, 0x11db, 0xbc77, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_EQUALIZER = &XA_IID_EQUALIZER_;

static const struct XAInterfaceID_ XA_IID_IMAGECONTROLS_ = { 0xf46de3e0, 0xdf03, 0x11db, 0x92f1, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_IMAGECONTROLS = &XA_IID_IMAGECONTROLS_;

static const struct XAInterfaceID_ XA_IID_IMAGEDECODERCAPABILITIES_ = { 0xc333e7a0, 0xe616, 0x11dc, 0xa93e, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_IMAGEDECODERCAPABILITIES = &XA_IID_IMAGEDECODERCAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_IMAGEEFFECTS_ = { 0xb865bca0, 0xdf04, 0x11db, 0xbab9, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_IMAGEEFFECTS = &XA_IID_IMAGEEFFECTS_;

static const struct XAInterfaceID_ XA_IID_IMAGEENCODER_ = { 0xcd49f140, 0xdf04, 0x11db, 0x8888, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_IMAGEENCODER = &XA_IID_IMAGEENCODER_;

static const struct XAInterfaceID_ XA_IID_IMAGEENCODERCAPABILITIES_ = { 0xc19f0640, 0xe86f, 0x11db, 0xb2d2, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_IMAGEENCODERCAPABILITIES = &XA_IID_IMAGEENCODERCAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_LED_ = { 0xa534d920, 0xf775, 0x11db, 0x8b70, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_LED = &XA_IID_LED_;

static const struct XAInterfaceID_ XA_IID_METADATAEXTRACTION_ = { 0x5df4fda0, 0xf776, 0x11db, 0xabc5, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_METADATAEXTRACTION = &XA_IID_METADATAEXTRACTION_;

static const struct XAInterfaceID_ XA_IID_METADATAINSERTION_ = { 0x49a14d60, 0xdf05, 0x11db, 0x9191, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_METADATAINSERTION = &XA_IID_METADATAINSERTION_;

static const struct XAInterfaceID_ XA_IID_METADATATRAVERSAL_ = { 0x73ffb0e0, 0xf776, 0x11db, 0xa00e, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_METADATATRAVERSAL = &XA_IID_METADATATRAVERSAL_;

static const struct XAInterfaceID_ XA_IID_OBJECT_ = { 0x82f5a5a0, 0xf776, 0x11db, 0x9700, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_OBJECT = &XA_IID_OBJECT_;

static const struct XAInterfaceID_ XA_IID_OUTPUTMIX_ = { 0xb25b6fa0, 0xf776, 0x11db, 0xb86b, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_OUTPUTMIX = &XA_IID_OUTPUTMIX_;

static const struct XAInterfaceID_ XA_IID_PLAY_ = { 0xb9c293e0, 0xf776, 0x11db, 0x80df, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_PLAY = &XA_IID_PLAY_;

static const struct XAInterfaceID_ XA_IID_PLAYBACKRATE_ = { 0xc36f1440, 0xf776, 0x11db, 0xac48, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_PLAYBACKRATE = &XA_IID_PLAYBACKRATE_;

static const struct XAInterfaceID_ XA_IID_PREFETCHSTATUS_ = { 0xcceac0a0, 0xf776, 0x11db, 0xbb9c, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_PREFETCHSTATUS = &XA_IID_PREFETCHSTATUS_;

static const struct XAInterfaceID_ XA_IID_RADIO_ = { 0xb316ad80, 0xdf05, 0x11db, 0xb5b6, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_RADIO = &XA_IID_RADIO_;

static const struct XAInterfaceID_ XA_IID_RDS_ = { 0xb80f42c0, 0xdf05, 0x11db, 0x92a5, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_RDS = &XA_IID_RDS_;

static const struct XAInterfaceID_ XA_IID_RECORD_ = { 0xd7948cc0, 0xf776, 0x11db, 0x8a3b, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_RECORD = &XA_IID_RECORD_;

static const struct XAInterfaceID_ XA_IID_SEEK_ = { 0xee6a3120, 0xf776, 0x11db, 0xb518, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_SEEK = &XA_IID_SEEK_;

static const struct XAInterfaceID_ XA_IID_SNAPSHOT_ = { 0xdb1b6dc0, 0xdf05, 0x11db, 0x8c01, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_SNAPSHOT = &XA_IID_SNAPSHOT_;

static const struct XAInterfaceID_ XA_IID_THREADSYNC_ = { 0xf3599ea0, 0xf776, 0x11db, 0xb3ea, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_THREADSYNC = &XA_IID_THREADSYNC_;

static const struct XAInterfaceID_ XA_IID_VIBRA_ = { 0xfe374c00, 0xf776, 0x11db, 0xa8f0, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_VIBRA = &XA_IID_VIBRA_;

static const struct XAInterfaceID_ XA_IID_VIDEODECODERCAPABILITIES_ = { 0xd18cb200, 0xe616, 0x11dc, 0xab01, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_VIDEODECODERCAPABILITIES = &XA_IID_VIDEODECODERCAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_VIDEOENCODER_ = { 0x9444db60, 0xdf06, 0x11db, 0xb311, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_VIDEOENCODER = &XA_IID_VIDEOENCODER_;

static const struct XAInterfaceID_ XA_IID_VIDEOENCODERCAPABILITIES_ = { 0x5aef2760, 0xe872, 0x11db, 0x849f, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_VIDEOENCODERCAPABILITIES = &XA_IID_VIDEOENCODERCAPABILITIES_;

static const struct XAInterfaceID_ XA_IID_VIDEOPOSTPROCESSING_ = { 0x898b6820, 0x7e6e, 0x11dd, 0x8caf, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_VIDEOPOSTPROCESSING = &XA_IID_VIDEOPOSTPROCESSING_;

static const struct XAInterfaceID_ XA_IID_VOLUME_ = { 0x088ba520, 0xf777, 0x11db, 0xa5e3, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_VOLUME = &XA_IID_VOLUME_;

static const struct XAInterfaceID_ XA_IID_STREAMINFORMATION_ = { 0x3a628fe0, 0x1238, 0x11de, 0xad9f, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } };
XA_API const XAInterfaceID XA_IID_STREAMINFORMATION = &XA_IID_STREAMINFORMATION_;


