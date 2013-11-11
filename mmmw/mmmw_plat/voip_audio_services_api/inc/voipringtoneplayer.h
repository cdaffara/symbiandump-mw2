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
 * Description:  Defines APIs for VoIP ring tone player.
 *
 */

#ifndef RINGTONEPLAYER_H
#define RINGTONEPLAYER_H

// FORWARD DECLARATIONS
class RFile;
class CRingTonePlayer;
class CRingTonePlayerImpl;

/**
 *  MRingToneObserver class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class MRingToneObserver
    {
public:
    static const TUint KOpenComplete = 1;
    static const TUint KPlaybackComplete = 2;

public:
    /**
     * Event
     * @param CRingTonePlayer& - Ring tone player reference
     * @param TInt - Event type
     * @param TInt - Status
     * @return void
     */
    virtual void Event(const CRingTonePlayer& aSrc, TInt aEventType,
            TInt aError) = 0;
    };

/**
 *  CRingTonePlayer class
 *
 *  Ring Tone Player class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CRingTonePlayer
    {
public:

    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CRingTonePlayer();

    /**
     * Open from default profile
     * @param MRingToneObserver&
     * @return TInt
     */
    IMPORT_C TInt Open(MRingToneObserver& aObserver);

    /**
     * Open file
     * @param MRingToneObserver&
     * @param TPtr
     * @return TInt
     */
    IMPORT_C TInt Open(MRingToneObserver& aObserver, TPtr aFileName);

    /**
     * Open from file handle
     * @param MRingToneObserver&
     * @param RFile&
     * @return TInt
     */
    IMPORT_C TInt Open(MRingToneObserver& aObserver, RFile& aFileHandle);

    /**
     * Open from descriptor
     * @param MRingToneObserver&
     * @param TPtr8
     * @param TPtr8
     * @return TInt
     */
    IMPORT_C TInt Open(MRingToneObserver& aObserver, TPtr8 aDesTone,
            TPtr8 aMimeType);

    /**
     * Play
     * @param none
     * @return TInt
     */
    IMPORT_C TInt Play();

    /**
     * Pause
     * @param none
     * @return TInt
     */
    IMPORT_C TInt Pause();

    /**
     * Resume
     * @param none
     * @return TInt
     */
    IMPORT_C TInt Resume();

    /**
     * Stop
     * @param none
     * @return TInt
     */
    IMPORT_C TInt Stop();

    /**
     * Close
     * @param none
     * @return TInt
     */
    IMPORT_C void Close();

protected:

    /**
     * Constructor
     * @param none
     * @return none
     */
    CRingTonePlayer();

    /**
     * Symbian constructor
     * @param none
     * @return void
     */
    void ConstructL(CRingTonePlayerImpl* aRingTonePlayerImpl);

private:

    CRingTonePlayerImpl* iRingTonePlayerImpl;
    };

#endif //RINGTONEPLAYER_H

// End of file
