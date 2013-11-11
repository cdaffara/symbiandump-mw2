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
 * Description:  Defines VoIP uplink stream APIs.
 *
 */

#ifndef VOIPUPLINKSTREAM_H
#define VOIPUPLINKSTREAM_H

#include <voipaudiocommon.h>
#include <voipdtmftoneplayer.h>

// FORWARD DECLARATIONS
class CVoIPDataBuffer;
class CVoIPFormatIntfc;
class CVoIPAudioUplinkStream;
class CVoIPAudioUplinkStreamImpl;

/**
 *  MVoIPUplinkObserver class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class MVoIPUplinkObserver
    {
public:
    static const TUint KOpenComplete = 1;
    static const TUint KUplinkError = 2;
    static const TUint KUplinkClosed = 3;

public:
    /**
     * EmptyBuffer
     * @param CVoIPAudioUplinkStream&
     * @param TPtr8
     * @return void
     */
    virtual void EmptyBuffer(const CVoIPAudioUplinkStream& aSrc,
            CVoIPDataBuffer* aBuffer) = 0;

    /**
     * Event
     * @param CVoIPAudioUplinkStream&
     * @param TInt - Event type
     * @param TInt - Status
     * @return void
     */
    virtual void Event(const CVoIPAudioUplinkStream& aSrc, TInt aEventType,
            TInt aError) = 0;
    };

/**
 *  CVoIPAudioUplinkStream class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPAudioUplinkStream
    {
public:

    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPAudioUplinkStream();

    /**
     * Open
     * @param MVoIPUplinkObserver&
     * @return TInt
     */
    IMPORT_C TInt Open(MVoIPUplinkObserver& aObserver);

    /**
     * GetVersion
     * @param TVersion&
     * @return TInt
     */
    IMPORT_C TInt GetVersion(TVersion& aVersion);

    /**
     * GetMaxGain
     * @param TInt&
     * @return TInt
     */
    IMPORT_C TInt GetMaxGain(TInt& aGain);

    /**
     * SetGain
     * @param TInt
     * @return TInt
     */
    IMPORT_C TInt SetGain(TInt aGain);

    /**
     * GetGain
     * @param TInt&
     * @return TInt
     */
    IMPORT_C TInt GetGain(TInt& aGain);

    /**
     * SetFormat
     * @param TVoIPCodecFormat
     * @param CVoIPFormatIntfc&
     * @return TInt
     */
    IMPORT_C TInt SetFormat(TVoIPCodecFormat aFormat,
            CVoIPFormatIntfc*& aIntfc);

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
     * BufferEmptied
     * @param CVoIPDataBuffer*
     * @return TInt
     */
    IMPORT_C TInt BufferEmptied(CVoIPDataBuffer* aBuffer);

protected:

    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPAudioUplinkStream();

    /**
     * Symbian constructor
     * @param none
     * @return void
     */
    void ConstructL(CVoIPAudioUplinkStreamImpl* aVoIPAudioUplinkStreamImpl);

private:
    /**
     * For internal use only
     */
    friend class CDTMFTonePlayer;

private:

    CVoIPAudioUplinkStreamImpl* iVoIPAudioUplinkStreamImpl;
    };

#endif //VOIPUPLINKSTREAM_H

// End of file
