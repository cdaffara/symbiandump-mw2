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
 * Description: Entry Point Functions to Create AL Objects
 *
 */
/*All global definitions and declarations here */

#ifndef XAOBJECTS_H
#define XAOBJECTS_H

#include "openmaxalwrapper.h"
#include "xaframeworkmgr.h"
#include "xaglobals.h"
#include "xacapabilitiesmgr.h"

/**
 * GLOBAL METHODS
 */

/*
 * Engine
 */
XAresult XAEngineImpl_Create(XAObjectItf *pEngine, XAuint32 numOptions,
        const XAEngineOption *pEngineOptions, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XAEngineImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Media Player
 */
XAresult XAMediaPlayerImpl_CreateMediaPlayer(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf *pPlayer,
        XADataSource *pDataSrc, XADataSource *pBankSrc,
        XADataSink *pAudioSnk, XADataSink *pImageVideoSnk,
        XADataSink *pVibra, XADataSink *pLEDArray, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAMediaPlayerImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XAMediaPlayerImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Metadata Extractor
 */
XAresult XAMetadataExtractorImpl_Create(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf *pMetadataExtractor,
        XADataSource *pDataSource, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAMetadataExtractorImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XAMetadataExtractorImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Output Mix
 */
XAresult XAOMixImpl_CreateOutputMix(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf *pMix,
        XAuint32 numInterfaces, const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAOMixImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XAOMixImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Media Recorder
 */
XAresult XAMediaRecorderImpl_CreateMediaRecorder(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf* pRecorder,
        XADataSource* pAudioSrc, XADataSource* pImageVideoSrc,
        XADataSink* pDataSnk, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAMediaRecorderImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XAMediaRecorderImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Camera Device
 */
XAresult XACameraDeviceImpl_CreateCameraDevice(FrameworkMap* mapper,
        XACapabilities* capabilities, XAObjectItf* pDevice,
        XAuint32 deviceID, XAuint32 numInterfaces,
        const XAInterfaceID * pInterfaceIds,
        const XAboolean * pInterfaceRequired);

XAresult XACameraDeviceImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XACameraDeviceImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Radio Device
 */
XAresult XARadioDeviceImpl_CreateRadioDevice( /*FrameworkMap* mapper,*/
XAObjectItf* pDevice, XAuint32 numInterfaces,
        const XAInterfaceID * pInterfaceIds,
        const XAboolean * pInterfaceRequired);

XAresult XARadioDeviceImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XARadioDeviceImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * Vibra Device
 */
XAresult XAVibraDeviceImpl_CreateVibraDevice(FrameworkMap* mapper,
        XAObjectItf* pDevice, XAuint32 deviceID, XAuint32 numInterfaces,
        const XAInterfaceID * pInterfaceIds,
        const XAboolean * pInterfaceRequired);

XAresult XAVibraDeviceImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XAVibraDeviceImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

/*
 * LED Array
 */
XAresult XALEDArrayDeviceImpl_CreateLEDArrayDevice(FrameworkMap* mapper,
        XAObjectItf* pDevice, XAuint32 deviceID, XAuint32 numInterfaces,
        const XAInterfaceID * pInterfaceIds,
        const XAboolean * pInterfaceRequired);

XAresult XALEDArrayDeviceImpl_QueryNumSupportedInterfaces(
        XAuint32 *pNumSupportedInterfaces);

XAresult XALEDArrayDeviceImpl_QuerySupportedInterfaces(XAuint32 index,
        XAInterfaceID *pInterfaceId);

#endif /*XAOBJECTS_H */
