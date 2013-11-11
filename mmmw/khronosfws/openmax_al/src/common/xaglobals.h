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
 * Description: Header File for common AL enums, functions.
 *
 */
/*All global definitions and declarations here */

#ifndef XAGLOBALS_H
#define XAGLOBALS_H

#include "openmaxalwrapper.h"
#include "xadebug.h"

/**
 GLOBAL ENUMERATIONS
 */
/**
 * enumeration for different adaptation event types.
 * add more types when needed, use single bit values.
 **/
typedef enum
    {
    XA_GENERICEVENTS = 0x1,
    XA_PLAYITFEVENTS = 0x2,
    XA_RECORDITFEVENTS = 0x4,
    XA_SNAPSHOTITFEVENTS = 0x8,
    XA_OUTPUTMIXITFEVENTS = 0x10,
    XA_CAMERAITFEVENTS = 0x20,
    XA_PREFETCHITFEVENTS = 0x40,
    XA_RADIOITFEVENTS = 0x80,
    XA_RDSITFEVENTS = 0x100,
    XA_METADATAEVENTS = 0x200,
    XA_VOLUMEITFEVENTS = 0x400,
    XA_NOKIALINEARVOLITFEVENTS = 0x800,
    XA_NOKIAEXTVOLITFEVENTS = 0x1000,
    XA_STREAMINFOEVENTS = 0x2000,
    XA_OBJECTITFEVENTS = 0x4000
    } XAAdaptEventTypes;

typedef enum
    {
    XACAP_DECODER = 0x1,
    XACAP_ENCODER = 0x2,
    XACAP_DEVSNK = 0x4,
    XACAP_DEVSRC = 0x8,

    XACAP_AUDIO = 0x10,
    XACAP_VIDEO = 0x20,
    XACAP_IMAGE = 0x40
    } XACapsType;

typedef enum
    {
    XA_UNDEFINED = 0, XA_READBUFFER, XA_WRITEBUFFER, XA_READ, XA_WRITE
    } XAConfigExtensionCpKey;

typedef enum
    {
    XA_MEDIATYPE_AUDIO = 0,
    XA_MEDIATYPE_VIDEO,
    XA_MEDIATYPE_IMAGE,
    XA_MEDIATYPE_UNKNOWN
    } XAMediaType;

#define CPCONFIGKEY "TestAppPipeConfig"

#define XA_RECMODE_STREAM 1
#define XA_RECMODE_STILL 2

#define XA_IMPL_SUPPORTED_AUDIO_OUT_NUM 3
#define XA_IMPL_OMIX_MAX_CONNECTED_MEDIAPLAYERS 10

/* Max volume level is implementation-dependent but must be at least 0mB
 * now used max volume 10 mB
 */
#define MAX_PERCENTAGE_VOLUME 100
#define MIN_VOLUME_LEVEL 0
#define MAX_SUPPORT_VOLUME_LEVEL 9000

/* Stereo position range is -1000 to 1000 permille. -1000 permille is fully left
 * and 1000 permille is fully right. 0 permille is center.
 */
#define STEREO_POSITION_RIGHT 1000
#define STEREO_POSITION_LEFT -1000
#define STEREO_POSITION_CENTER 0

/**
 * GLOBAL METHODS
 */

/*
 * Check if interface IDs are equal
 */
XAboolean XACommon_EqualIIds(XAInterfaceID idA, XAInterfaceID idB);

/*
 * Check Data Source availability and type
 */
XAresult XACommon_CheckDataSource(XADataSource* source, XAMediaType* type);

XAresult XACommon_ValidateDataLocator(XAuint32 pCounter, void* pSnk, ...);
#endif /*XAGLOBALS_H */
