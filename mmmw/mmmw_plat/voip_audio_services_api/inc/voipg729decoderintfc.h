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
 * Description:  Defines APIs specific to G.729 decoder.
 *
 */

#ifndef VOIPG729DECFORMATINTFC_H
#define VOIPG729DECFORMATINTFC_H

#include <voipaudiocommon.h>
#include <voipformatintfc.h>

// FORWARD DECLARATIONS
class CVoIPG729DecoderIntfcImpl;

/**
 *  CVoIPG729DecoderIntfc class
 *
 *  G.729 decoder configuration class
 *
 *  @lib VoIPAudioIntfc.dll
 *
 */
class CVoIPG729DecoderIntfc : public CVoIPFormatIntfc
    {
public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPG729DecoderIntfc();

    /**
     * BadLsfNextBuffer
     * @param none
     * @return TInt
     */
    IMPORT_C TInt BadLsfNextBuffer();

protected:
    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPG729DecoderIntfc();

    /**
     * 2-nd phase constructor
     * @param CVoIPG729DecoderIntfcImpl*
     * @return void
     */
    void ConstructL(CVoIPG729DecoderIntfcImpl* aFormatIntfcImpl);

protected:
    CVoIPG729DecoderIntfcImpl* iFormatIntfcImpl;
    };

#endif //VOIPG729DECFORMATINTFC_H

// End of file
