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
 * Description:  Defines APIs common to all supported codecs.
 *
 */

#ifndef VOIPFORMATINTFC_H
#define VOIPFORMATINTFC_H

#include <voipaudiocommon.h>

// FORWARD DECLARATIONS
class CVoIPFormatIntfc;
class CVoIPFormatIntfcImpl;

/**
 *  MVoIPFormatObserver class
 *
 *  ?more_complete_description
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class MVoIPFormatObserver
    {
public:
    static const TUint KSilenceBegin = 1;
    static const TUint KSilenceUpdate = 2;
    static const TUint KSilenceEnd = 3;
    static const TUint KObserverEventMax = 4;

public:
    /**
     * Event
     * @param const CVoIPFormatIntfc&
     * @param TInt - event type
     * @return void
     */
    virtual void Event(const CVoIPFormatIntfc& aSrc, TInt aEventType) = 0;
    };

/**
 *  CVoIPFormatIntfc class
 *
 *  Base class for codec format configuration
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPFormatIntfc
    {
public:
    /**
     *  TG711CodecMode
     */
    enum TG711CodecMode
        {
        EG711ALaw,
        EG711uLaw
        };

    /**
     *  TILBCCodecMode
     */
    enum TILBCCodecMode
        {
        EiLBC20mSecFrame,
        EiLBC30mSecFrame
        };

public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPFormatIntfc();

    /**
     * SetObserver
     * @param MVoIPFormatObserver&
     * @return TInt
     */
    IMPORT_C TInt SetObserver(MVoIPFormatObserver& aObserver);

    /**
     * GetSupportedBitRates
     * @param RArray<TUint>&
     * @return TInt
     */
    IMPORT_C TInt GetSupportedBitRates(RArray<TUint>& aArray);

    /**
     * SetBitRate
     * @param TUint
     * @return TInt
     */
    IMPORT_C TInt SetBitRate(TUint aBitrate);

    /**
     * GetBitRate
     * @param TUint&
     * @return TInt
     */
    IMPORT_C TInt GetBitRate(TUint& aBitrate);

    /**
     * SetVAD
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetVAD(TBool aVad);

    /**
     * GetVAD
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetVAD(TBool& aVad);

    /**
     * FrameModeRequiredForErrorConcealment
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt FrameModeRequiredForErrorConcealment(TBool& aMode);

    /**
     * SetFrameMode
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetFrameMode(TBool aMode);
    /**
     * GetFrameMode
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetFrameMode(TBool& aMode);

    /**
     * ConcealErrorForNextBuffer
     * @param none
     * @return TInt
     */
    IMPORT_C TInt ConcealErrorForNextBuffer();

protected:

    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPFormatIntfc();

    /**
     * 2-nd phase constructor
     * @param CVoIPFormatIntfcImpl*
     * @return void
     */
    void ConstructL(CVoIPFormatIntfcImpl* aVoIPFormatIntfcImpl);

protected:

    CVoIPFormatIntfcImpl* iVoIPFormatIntfcImpl;
    };

#endif //VOIPFORMATINTFC_H

// End of file
