/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Defines VoIP downlink stream APIs.
 *
 */

#ifndef VOIPDOWNLINKSTREAM_H
#define VOIPDOWNLINKSTREAM_H

#include <voipaudiocommon.h>
#include <voipdtmftoneplayer.h>

// FORWARD DECLARATIONS
class CVoIPAudioDownlinkStream;
class CVoIPAudioDownlinkStreamImpl;
class CVoIPDataBuffer;
class CVoIPFormatIntfc;

/**
 *  MVoIPDownlinkObserver class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class MVoIPDownlinkObserver
    {
public:
    static const TUint KOpenComplete = 1;
    static const TUint KDownlinkError = 2;
    static const TUint KDownlinkClosed = 3;
    static const TUint KAudioDeviceChanged = 4;

public:
    /**
     * FillBuffer
     * @param const CVoIPAudioDownlinkStream&
     * @param CVoIPDataBuffer*
     * @return void
     */
    virtual void FillBuffer(const CVoIPAudioDownlinkStream& aSrc,
            CVoIPDataBuffer* aBuffer) = 0;
    /**
     * Event
     * @param const CVoIPAudioDownlinkStream&
     * @param TInt - Event type
     * @param TInt - Status
     * @return void
     */
    virtual void Event(const CVoIPAudioDownlinkStream& aSrc, TInt aEventType,
            TInt aError) = 0;
    };

/**
 *  CVoIPAudioDownlinkStream class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPAudioDownlinkStream
    {
public:
    /**
     *  TVoIPOutputDevice
     */
    enum TVoIPOutputDevice
        {
        EHandset,
        ELoudSpeaker,
        ENoPreference
        };

public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPAudioDownlinkStream();

    /**
     * Open
     * @param MVoIPDownlinkObserver&
     * @return TInt
     */
    IMPORT_C TInt Open(MVoIPDownlinkObserver& aObserver);

    /**
     * GetVersion
     * @param TVersion&
     * @return TInt
     */
    IMPORT_C TInt GetVersion(TVersion& aVersion);

    /**
     * GetMaxVolume
     * @param TInt&
     * @return TInt
     */
    IMPORT_C TInt GetMaxVolume(TInt& aVolume);

    /**
     * SetVolume
     * @param TInt
     * @return TInt
     */
    IMPORT_C TInt SetVolume(TInt aVolume);

    /**
     * GetVolume
     * @param TInt&
     * @return TInt
     */
    IMPORT_C TInt GetVolume(TInt& aVolume);

    /**
     * SetFormat
     * @param TVoIPCodecFormat
     * @param CVoIPFormatIntfc*&
     * @return TInt
     */
    IMPORT_C TInt SetFormat(TVoIPCodecFormat aFormat,
            CVoIPFormatIntfc*& aIntfc);

    /**
     * SetAudioDevice
     * @param TVoIPOutputDevice
     * @return TInt
     */
    IMPORT_C TInt SetAudioDevice(TVoIPOutputDevice aDevice);

    /**
     * GetAudioDevice
     * @param TVoIPOutputDevice&
     * @return TInt
     */
    IMPORT_C TInt GetAudioDevice(TVoIPOutputDevice& aDevice);

    /**
     * Start
     * @param none
     * @return TInt
     */
    IMPORT_C TInt Start();

    /**
     * Stop
     * @param none
     * @return TInt
     */
    IMPORT_C TInt Stop();

    /**
     * Close
     * @param none
     * @return void
     */
    IMPORT_C void Close();

    /**
     * BufferFilled
     * @param CVoIPDataBuffer*
     * @return TInt
     */
    IMPORT_C TInt BufferFilled(CVoIPDataBuffer* aBuffer);

protected:

    /**
     * Constructor
     * @param none
     * @return TInt
     */
    CVoIPAudioDownlinkStream();

    /**
     * Symbian constructor
     * @param none
     * @return TInt
     */
    void ConstructL(CVoIPAudioDownlinkStreamImpl*
            aVoIPAudioDownlinkStreamImpl);

private:
    /**
     * For internal use only
     */
    friend class CDTMFTonePlayer;

private:

    CVoIPAudioDownlinkStreamImpl* iVoIPAudioDownlinkStreamImpl;
    };

#endif //VOIPDOWNLINKSTREAM_H

// End of file
