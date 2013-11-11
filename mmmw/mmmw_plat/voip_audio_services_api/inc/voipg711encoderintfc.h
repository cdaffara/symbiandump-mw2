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
 * Description:  Defines APIs specific to G.711 encoder.
 *
 */

#ifndef VOIPG711ENCFORMATINTFC_H
#define VOIPG711ENCFORMATINTFC_H

#include <voipaudiocommon.h>
#include <voipformatintfc.h>

// FORWARD DECLARATIONS
class CVoIPG711EncoderIntfcImpl;

/**
 *  CVoIPG711EncoderIntfc class
 *
 *  G.711 encoder configuration class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPG711EncoderIntfc : public CVoIPFormatIntfc
    {
public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPG711EncoderIntfc();

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

protected:
    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPG711EncoderIntfc();

    /**
     * 2-nd phase constructor
     * @param CVoIPG711EncoderIntfcImpl*
     * @return void
     */
    void ConstructL(CVoIPG711EncoderIntfcImpl* aFormatIntfcImpl);

protected:
    CVoIPG711EncoderIntfcImpl* iFormatIntfcImpl;
    };

#endif //VOIPG711ENCFORMATINTFC_H

// End of file
