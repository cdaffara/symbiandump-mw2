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
 * Description:  Defines VoIP DTMF tone player APIs.
 *
 */

#ifndef VOIPDTMFTONEPLAYER_H
#define VOIPDTMFTONEPLAYER_H

// FORWARD DECLARATIONS
class CVoIPAudioUplinkStream;
class CVoIPAudioDownlinkStream;
class CDTMFTonePlayer;
class CDTMFTonePlayerImpl;

/**
 *  MDTMFToneObserver class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class MDTMFToneObserver
    {
public:
    static const TUint KOpenCompleteDNL = 1;
    static const TUint KOpenCompleteUPL = 2;

public:
    /**
     * Event
     * @param CDTMFTonePlayer& - DTMF tone player reference
     * @param TInt - Event type
     * @param TInt - Status
     * @return void
     */
    virtual void Event(const CDTMFTonePlayer& aPlayer, TInt aEventType,
            TInt aError) = 0;
    };

/**
 *  CDTMFTonePlayer class
 *
 *  DTMF Tone Player class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CDTMFTonePlayer
    {
public:

    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CDTMFTonePlayer();

    /**
     * Open
     * @param MDTMFToneObserver&
     * @return TInt
     */
    IMPORT_C TInt Open(MDTMFToneObserver& aObserver);

    /**
     * Play
     * @param TPtr -
     * @return TInt -
     */
    IMPORT_C TInt Play(const TPtr aTones);

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
     * AddVoIPAudioUplinkStream
     * @param CVoIPAudioUplinkStream& -
     * @return TInt
     */
    IMPORT_C TInt AddVoIPAudioUplinkStream(CVoIPAudioUplinkStream& aStream);

    /**
     * RemoveVoIPAudioUplinkStream
     * @param CVoIPAudioUplinkStream&
     * @return TInt
     */
    IMPORT_C TInt RemoveVoIPAudioUplinkStream(CVoIPAudioUplinkStream& aStream);

    /**
     * AddVoIPAudioDownlinkStream
     * @param CVoIPAudioDownlinkStream& -
     * @return TInt
     */
    IMPORT_C TInt AddVoIPAudioDownlinkStream(CVoIPAudioDownlinkStream& aStream);

    /**
     * RemoveVoIPAudioDownlinkStream
     * @param CVoIPAudioDownlinkStream& -
     * @return TInt
     */
    IMPORT_C TInt RemoveVoIPAudioDownlinkStream(
            CVoIPAudioDownlinkStream& aStream);

protected:

    /**
     * Constructor
     * @param none
     * @return none
     */
    CDTMFTonePlayer();

    /**
     * Symbian constructor
     * @param none
     * @return void
     */
    void ConstructL(CDTMFTonePlayerImpl* aDTMFTonePlayerImpl);

private:

    CDTMFTonePlayerImpl* iDTMFTonePlayerImpl;
    };

#endif //VOIPDTMFTONEPLAYER_H

// End of file
