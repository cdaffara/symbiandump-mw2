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

#ifndef XAAUDIOIODEVICECAPABILITIESITF_H
#define XAAUDIOIODEVICECAPABILITIESITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xacapabilitiesmgr.h"
/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAAudioIODeviceCapabilitiesItf implementation */
typedef struct XAAudIODevCapaItfImpl_
    {
    /* parent interface */
    struct XAAudioIODeviceCapabilitiesItf_ itf;
    /* pointer to self */
    struct XAAudIODevCapaItfImpl_* self;

    /* variables */
    xaAvailableAudioInputsChangedCallback inputCb;
    void* inputCbCtx;
    XAAudioIODeviceCapabilitiesItf inputCbPtrToSelf;
    xaAvailableAudioOutputsChangedCallback outputCb;
    void* outputCbCtx;
    XAAudioIODeviceCapabilitiesItf outputCbPtrToSelf;
    xaDefaultDeviceIDMapChangedCallback deviceMapCb;
    void* deviceMapCtx;
    XAAudioIODeviceCapabilitiesItf deviceMapCbPtrToSelf;

    XAuint32 numInputDevices;
    XAuint32 numOutputDevices;
    XAmilliHertz sampleRateArray[12];
    XACapabilities* capslist;

    } XAAudIODevCapaItfImpl;

/* Base interface XAAudioIODeviceCapabilitiesItf implementation */
XAresult XAAudIODevCapaItfImpl_GetAvailableAudioInputs(
        XAAudioIODeviceCapabilitiesItf self, XAint32* pNumInputs,
        XAuint32* pInputDeviceIDs);

XAresult XAAudIODevCapaItfImpl_QueryAudioInputCapabilities(
        XAAudioIODeviceCapabilitiesItf self, XAuint32 deviceId,
        XAAudioInputDescriptor* pDescriptor);

XAresult XAAudIODevCapaItfImpl_RegisterAvailableAudioInputsChangedCallback(
        XAAudioIODeviceCapabilitiesItf self,
        xaAvailableAudioInputsChangedCallback callback, void* pContext);

XAresult XAAudIODevCapaItfImpl_GetAvailableAudioOutputs(
        XAAudioIODeviceCapabilitiesItf self, XAint32* pNumOutputs,
        XAuint32* pOutputDeviceIDs);

XAresult XAAudIODevCapaItfImpl_QueryAudioOutputCapabilities(
        XAAudioIODeviceCapabilitiesItf self, XAuint32 deviceId,
        XAAudioOutputDescriptor* pDescriptor);

XAresult XAAudIODevCapaItfImpl_RegisterAvailableAudioOutputsChangedCallback(
        XAAudioIODeviceCapabilitiesItf self,
        xaAvailableAudioOutputsChangedCallback callback, void* pContext);

XAresult XAAudIODevCapaItfImpl_RegisterDefaultDeviceIDMapChangedCallback(
        XAAudioIODeviceCapabilitiesItf self,
        xaDefaultDeviceIDMapChangedCallback callback, void* pContext);

XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioInputs(
        XAAudioIODeviceCapabilitiesItf self, XAuint32 deviceId,
        XAint32* pNumAudioInputs, XAuint32* pAudioInputDeviceIDs);

XAresult XAAudIODevCapaItfImpl_GetAssociatedAudioOutputs(
        XAAudioIODeviceCapabilitiesItf self, XAuint32 deviceId,
        XAint32* pNumAudioOutputs, XAuint32* pAudioOutputDeviceIDs);

XAresult XAAudIODevCapaItfImpl_GetDefaultAudioDevices(
        XAAudioIODeviceCapabilitiesItf self, XAuint32 defaultDeviceID,
        XAint32 *pNumAudioDevices, XAuint32 *pAudioDeviceIDs);

XAresult XAAudIODevCapaItfImpl_QuerySampleFormatsSupported(
        XAAudioIODeviceCapabilitiesItf self, XAuint32 deviceId,
        XAmilliHertz samplingRate, XAint32* pSampleFormats,
        XAint32* pNumOfSampleFormats);

/* XAAudIODevCapaItfImpl -specific methods */
XAAudIODevCapaItfImpl* XAAudIODevCapaItfImpl_Create(XACapabilities* caps);
void XAAudIODevCapaItfImpl_Free(XAAudIODevCapaItfImpl* self);

#endif /* XAAUDIOIODEVICECAPABILITIESITF_H */
