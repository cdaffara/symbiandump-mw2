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
 * Description:  Defines APIs specific to G.711 decoder.
 *
 */

#ifndef VOIPG711DECFORMATINTFC_H
#define VOIPG711DECFORMATINTFC_H

#include <voipaudiocommon.h>
#include <voipformatintfc.h>

// FORWARD DECLARATIONS
class CVoIPG711DecoderIntfcImpl;

/**
 *  CVoIPG711DecoderIntfc class
 *
 *  G.711 decoder configuration class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPG711DecoderIntfc : public CVoIPFormatIntfc
    {
public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPG711DecoderIntfc();

    /**
     * SetMode
     * @param TG711CodecMode
     * @return TInt
     */
    IMPORT_C TInt SetMode(TG711CodecMode aMode);

    /**
     * GetMode
     * @param TG711CodecMode&
     * @return TInt
     */
    IMPORT_C TInt GetMode(TG711CodecMode& aMode);

    /**
     * SetCNG
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetCNG(TBool aCng);

    /**
     * GetCNG
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetCNG(TBool& aCng);

    /**
     * SetPLC
     * @param TBool
     * @return TInt
     */
    IMPORT_C TInt SetPLC(TBool aPlc);

    /**
     * GetPLC
     * @param TBool&
     * @return TInt
     */
    IMPORT_C TInt GetPLC(TBool& aPlc);

protected:
    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPG711DecoderIntfc();

    /**
     * 2-nd phase constructor
     * @param CVoIPG711DecoderIntfcImpl*
     * @return void
     */
    void ConstructL(CVoIPG711DecoderIntfcImpl* aFormatIntfcImpl);

protected:
    CVoIPG711DecoderIntfcImpl* iFormatIntfcImpl;
    };

#endif //VOIPG711DECFORMATINTFC_H

// End of file
