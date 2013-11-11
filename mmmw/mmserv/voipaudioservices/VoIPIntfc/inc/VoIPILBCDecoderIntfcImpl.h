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
 * Description:  VOIP Audio Services
 *
 */

#ifndef VOIPILBCDECFORMATINTFCIMPL_H
#define VOIPILBCDECFORMATINTFCIMPL_H

#include <voipilbcdecoderintfc.h>
#include "VoIPFormatIntfcImpl.h"

// FORWARD DECLARATION
class CVoIPAudioDownlinkStreamImpl;

// -----------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPILBCDecoderIntfcImpl) : public CBase,
        public CVoIPILBCDecoderIntfc,
        public CVoIPFormatIntfcImpl
    {
public:
    static CVoIPILBCDecoderIntfcImpl*
    NewL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);

    virtual ~CVoIPILBCDecoderIntfcImpl();

    TInt SetMode(CVoIPFormatIntfc::TILBCCodecMode aMode);
    TInt GetMode(CVoIPFormatIntfc::TILBCCodecMode& aMode); //n/a through CIs
    TInt SetCNG(TBool aCng);
    TInt GetCNG(TBool& aCng);

private:
    CVoIPILBCDecoderIntfcImpl();
    void ConstructL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);
    };

#endif //VOIPILBCDECFORMATINTFCIMPL_H

// End of file
