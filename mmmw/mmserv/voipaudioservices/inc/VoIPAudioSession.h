/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Declares RVoIPAudioSession - client side interface to
 *                VoIP server.
 *
 */

#ifndef VOIPAUDIOSESSION_H
#define VOIPAUDIOSESSION_H

// INCLUDES
#include <mmfbase.h>
#include <mmfutilities.h> //for TFourCC
#include <voipaudiocommon.h>
#include <voipdownlinkstream.h>
#include <voipformatintfc.h>
#include <voipjitterbufferintfc.h>
#include "VoIPAudioClientServer.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CVoIPFormatIntfc;

// CLASS DECLARATION

/**
 * RVoIPAudioSession API
 */
class RVoIPAudioSession : public RSessionBase
    {
public:
    /**
     * Opens a connection to the server.
     * @return KErrNone if successful connection created
     */
    IMPORT_C TInt Connect();
    /**
     * @return The server version this session can connct to
     */
    IMPORT_C TVersion Version() const;
    /**
     * Close the session to the server
     */
    IMPORT_C void Close();

    // Downlink stream methods

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt OpenDownlink(TMMFPrioritySettings aPriority);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt GetMaxVolume();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt SetVolume(const TInt aVolume);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt GetVolume();

    /**
     * ?description
     *
     * @param const TMMFPrioritySettings
     * @param RArray<TUint32>&
     * @param TInt&
     * @return TInt
     */
    IMPORT_C TInt GetSupportedDecoders(const TMMFPrioritySettings aPriority,
            RArray<TUint32>& aDecoders, TInt& aFrameSize);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt SetDecoder(const TUint32 aDecoder);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt SetAudioDevice(
            const CVoIPAudioDownlinkStream::TVoIPOutputDevice aDevice);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt GetAudioDevice(
            CVoIPAudioDownlinkStream::TVoIPOutputDevice& aDevice);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt StartDownlink();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt StopDownlink();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C void CloseDownlink();

    /**
     * ?description
     *
     * @param TPtr8
     * @param const TInt64
     * @return TInt
     */
    IMPORT_C TInt BufferFilled(TPtr8 aBuffer, const TUint aBufferSequence = 0);

    // Uplink stream methods

    /**
     * ?description
     *
     * @param const TMMFPrioritySettings
     * @return TInt
     */
    IMPORT_C TInt OpenUplink(const TMMFPrioritySettings aPriority);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt GetMaxGain();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt SetGain(const TInt aGain);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt GetGain();

    /**
     * ?description
     *
     * @param const TMMFPrioritySettings
     * @param RArray<TUint32>&
     * @param TInt&
     * @return TInt
     */
    IMPORT_C TInt GetSupportedEncoders(const TMMFPrioritySettings aPriority,
            RArray<TUint32>& aEncoders, TInt& aFrameSize);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt SetEncoder(const TUint32 aEncoder);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt StartUplink();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt StopUplink();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C void CloseUplink();

    /**
     * ?description
     *
     * @param TPtr8
     * @return TInt
     */
    IMPORT_C TInt BufferEmptied(TPtr8 aBuffer);

    // Codec CI methods

    /**
     * ?description
     *
     * @param TArray<TUint>&
     * @return void
     */
    IMPORT_C void GetSupportedBitRatesL(RArray<TUint>& aArray);

    /**
     * ?description
     *
     * @param const TUint
     * @return TInt
     */
    IMPORT_C TInt SetBitRate(const TUint aBitrate);

    /**
     * ?description
     *
     * @param TUint&
     * @return TInt
     */
    IMPORT_C TInt GetBitRate(TUint& aBitrate);

    /**
     * ?description
     *
     *
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetVAD(const TBool aVad);

    /**
     * ?description
     *
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetVAD(TBool& aVad);

    /**
     * ?description
     *
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt FrameModeRequiredForEC(TBool& aMode);

    /**
     * ?description
     *
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetFrameMode(const TBool aMode);

    /**
     * ?description
     *
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetFrameMode(TBool& aMode);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt ConcealErrorForNextBuffer();

    /**
     * ?description
     *
     * @param TG711CodecMode
     * @return TInt
     */
    IMPORT_C TInt SetMode(const CVoIPFormatIntfc::TG711CodecMode aMode);

    /**
     * ?description
     *
     * @param TG711CodecMode&
     * @return TInt
     */
    IMPORT_C TInt GetMode(CVoIPFormatIntfc::TG711CodecMode& aMode);

    /**
     * ?description
     *
     * @param TILBCCodecMode
     * @return TInt
     */
    IMPORT_C TInt SetMode(const CVoIPFormatIntfc::TILBCCodecMode aMode);

    /**
     * ?description
     *
     * @param TILBCCodecMode&
     * @return TInt
     */
    IMPORT_C TInt GetMode(CVoIPFormatIntfc::TILBCCodecMode& aMode);

    /**
     * ?description
     *
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetCNG(const TBool aCng);

    /**
     * ?description
     *
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetCNG(TBool& aCng);

    /**
     * ?description
     *
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetPLC(const TBool aPlc);

    /**
     * ?description
     *
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetPLC(TBool& aPlc);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt BadLsfNextBuffer();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt OpenDTMFTonePlayer();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt PlayDTMFTone(const TDesC& aTones);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt StopDTMFTonePlay();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C void CloseDTMFTonePlayer();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt OpenRingTonePlayer();

    /**
     * ?description
     *
     * @param TDesC&
     * @return TInt
     */
    IMPORT_C TInt OpenRingTonePlayer(const TDesC& aFileName);

    /**
     * ?description
     *
     * @param RFile&
     * @return TInt
     */
    IMPORT_C TInt OpenRingTonePlayer(const RFile& aFileHandle);

    /**
     * ?description
     *
     * @param TDes&
     * @param TDes&
     * @return TInt
     */
    IMPORT_C TInt OpenRingTonePlayer(const TDesC8& aDesData);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt PlayRingTone();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt PauseRingTone();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt ResumeRingTone();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt StopRingTone();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    IMPORT_C void CloseRingTonePlayer();

    /**
     * ConfigureJitterBuffer
     *
     * @param const TVoIPJBConfig&
     * @return TInt
     */
    IMPORT_C TInt ConfigureJitterBuffer(const TVoIPJBConfig& aJbConfig);

    /**
     * ResetJitterBuffer
     *
     * @param const TBool
     * @return TInt
     */
    IMPORT_C TInt ResetJitterBuffer(const TBool aPlayTone);

    /**
     * DelayDown
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt DelayDown();

    /**
     * DelayUp
     *
     * @param none
     * @return TInt
     */
    IMPORT_C TInt DelayUp();

private:
    /**
     * PopulateArrayL
     *
     * @param TVoIPAudioClientServerRequest
     * @param RArray<TUint32>&
     * @param TInt
     * @return void
     */
    void PopulateArrayL(TVoIPAudioClientServerRequest aRequest,
            RArray<TUint32>& aDecoders, TInt aCount);

    TInt SendClientRequest(TVoIPAudioClientServerRequest aRequest,
            const TIpcArgs& aArgs) const;
    };

#endif //VOIPAUDIOSESSION_H

// End of file
