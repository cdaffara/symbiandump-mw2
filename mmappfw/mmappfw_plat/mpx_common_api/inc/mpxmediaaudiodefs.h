/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for audio media type
*
*/



#ifndef CMPXMEDIAAUDIODEFS_H
#define CMPXMEDIAAUDIODEFS_H


#include <e32base.h>
#include <mpxattribute.h>

// CONSTANTS
/**
* Content ID identifying audio category of content provided
* in the media object and associated attributes. 
*/

const TInt KMPXMediaIdAudio=0x101FFC41;

/**
*  Attributes supported for KMPXMediaIdAudio
*/    
const TMPXAttributeData KMPXMediaAudioBitrate = {KMPXMediaIdAudio, 0x01}; // TInt
const TMPXAttributeData KMPXMediaAudioSamplerate = {KMPXMediaIdAudio, 0x02}; // TInt
const TMPXAttributeData KMPXMediaAudioNumberOfChannels = {KMPXMediaIdAudio, 0x04}; // TInt
const TMPXAttributeData KMPXMediaAudioAudioCodec = {KMPXMediaIdAudio, 0x08}; // TInt
const TMPXAttributeData KMPXMediaAudioAudioAll = {KMPXMediaIdAudio, 0xffffffff};

/** 
*  Attributes supported for KMPXMediaIdAUdio.
*
*  @deprecated Use the above consts.
*/        
enum 
    {
    EMPXMediaAudioBitrate=0x01, // TInt
    EMPXMediaAudioSamplerate=0x02, // TInt
    EMPXMediaAudioNumberOfChannels=0x04,  // TInt
    EMPXMediaAudioCodec=0x08, // TInt
    EMPXMediaAudioAll=0xffffffff
    };         
     
#endif // CMPXMEDIAAUDIODEFS_H
