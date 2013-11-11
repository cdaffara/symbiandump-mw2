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
 * Description:  Defines APIs specific to iLBC encoder.
 *
 */

#ifndef VOIPILBCENCFORMATINTFC_H
#define VOIPILBCENCFORMATINTFC_H

#include <voipaudiocommon.h>
#include <voipformatintfc.h>

// FORWARD DECLARATIONS
class CVoIPILBCEncoderIntfcImpl;

/**
 *  CVoIPILBCEncoderIntfc class
 *
 *  iLBC encoder configuration class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPILBCEncoderIntfc : public CVoIPFormatIntfc
    {
public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPILBCEncoderIntfc();

    /**
     * SetMode
     * @param TILBCCodecMode
     * @return TInt
     */
    IMPORT_C TInt SetMode(TILBCCodecMode aMode);

    /**
     * GetMode
     * @param TILBCCodecMode&
     * @return TInt
     */
    IMPORT_C TInt GetMode(TILBCCodecMode& aMode);

protected:
    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPILBCEncoderIntfc();

    /**
     * 2-nd phase constructor
     * @param CVoIPILBCEncoderIntfcImpl*
     * @return void
     */
    void ConstructL(CVoIPILBCEncoderIntfcImpl* aFormatIntfcImpl);

protected:
    CVoIPILBCEncoderIntfcImpl* iFormatIntfcImpl;
    };

#endif //VOIPILBCENCFORMATINTFC_H

// End of file
