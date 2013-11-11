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
 * Description: Engine Interface Header
 *
 */

#ifndef XAENGINEITF_H
#define XAENGINEITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaframeworkmgr.h"
#include "xacapabilitiesmgr.h"
/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAEngineItf implementation */
typedef struct XAEngineItfImpl_
    {
    /* parent interface */
    struct XAEngineItf_ itf;
    /* pointer to self */
    struct XAEngineItfImpl_* self;
    /* variables */
    /*Not Owned*/
    struct FrameworkMap_* mapper;
    /*Not Owned*/
    struct XACapabilities_* capabilities;
    } XAEngineItfImpl;

/** METHODS **/

/* Base interface XAEngineItf implementation */
XAresult XAEngineItfImpl_CreateCameraDevice(XAEngineItf self,
        XAObjectItf *pDevice, XAuint32 deviceID, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateRadioDevice(XAEngineItf self,
        XAObjectItf *pDevice, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateLEDDevice(XAEngineItf self,
        XAObjectItf *pDevice, XAuint32 deviceID, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateVibraDevice(XAEngineItf self,
        XAObjectItf *pDevice, XAuint32 deviceID, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateMediaPlayer(XAEngineItf self,
        XAObjectItf *pPlayer, XADataSource *pDataSrc, XADataSource *pBankSrc,
        XADataSink *pAudioSnk, XADataSink *pImageVideoSnk,
        XADataSink *pVibra, XADataSink *pLEDArray, XAuint32 numInterfaces,
        const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateMediaRecorder(XAEngineItf self,
        XAObjectItf *pRecorder, XADataSource *pAudioSrc,
        XADataSource *pImageVideoSrc, XADataSink *pDataSnk,
        XAuint32 numInterfaces, const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateOutputMix(XAEngineItf self, XAObjectItf *pMix,
        XAuint32 numInterfaces, const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateMetadataExtractor(XAEngineItf self,
        XAObjectItf *pMetadataExtractor, XADataSource *pDataSource,
        XAuint32 numInterfaces, const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_CreateExtensionObject(XAEngineItf self,
        XAObjectItf *pObject, void *pParameters, XAuint32 objectID,
        XAuint32 numInterfaces, const XAInterfaceID *pInterfaceIds,
        const XAboolean *pInterfaceRequired);

XAresult XAEngineItfImpl_GetImplementationInfo(XAEngineItf self,
        XAuint32 *pMajor, XAuint32 *pMinor, XAuint32 *pStep,
        const XAchar *pImplementationText);

XAresult XAEngineItfImpl_QuerySupportedProfiles(XAEngineItf self,
        XAint16 *pProfilesSupported);

XAresult XAEngineItfImpl_QueryNumSupportedInterfaces(XAEngineItf self,
        XAuint32 objectID, XAuint32 *pNumSupportedInterfaces);

XAresult XAEngineItfImpl_QuerySupportedInterfaces(XAEngineItf self,
        XAuint32 objectID, XAuint32 index, XAInterfaceID *pInterfaceId);

XAresult XAEngineItfImpl_QueryLEDCapabilities(XAEngineItf self,
        XAuint32 *pIndex, XAuint32 *pLEDDeviceID,
        XALEDDescriptor *pDescriptor);

XAresult XAEngineItfImpl_QueryVibraCapabilities(XAEngineItf self,
        XAuint32 *pIndex, XAuint32 *pVibraDeviceID,
        XAVibraDescriptor *pDescriptor);

XAresult XAEngineItfImpl_QueryNumSupportedExtensions(XAEngineItf self,
        XAuint32 *pNumExtensions);

XAresult XAEngineItfImpl_QuerySupportedExtension(XAEngineItf self,
        XAuint32 index, XAchar *pExtensionName, XAint16 *pNameLength);

XAresult XAEngineItfImpl_IsExtensionSupported(XAEngineItf self,
        const XAchar *pExtensionName, XAboolean *pSupported);

/* XAEngineItfImpl -specific methods */
XAEngineItfImpl* XAEngineItfImpl_Create(FrameworkMap* fwkmapper,
        XACapabilities* capabilities);
void XAEngineItfImpl_Free(XAEngineItfImpl *self);

#endif /* XAENGINEITF_H */
