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

#ifndef VOIPG711DECFORMATINTFCIMPL_H
#define VOIPG711DECFORMATINTFCIMPL_H

#include <voipg711decoderintfc.h>
#include "VoIPFormatIntfcImpl.h"

// FORWARD DECLARATION
class CVoIPAudioDownlinkStreamImpl;

// -----------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPG711DecoderIntfcImpl) : public CBase,
        public CVoIPG711DecoderIntfc,
        public CVoIPFormatIntfcImpl
    {
public:
    static CVoIPG711DecoderIntfcImpl*
    NewL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);

    virtual ~CVoIPG711DecoderIntfcImpl();

    TInt SetMode(CVoIPFormatIntfc::TG711CodecMode aMode);
    TInt GetMode(CVoIPFormatIntfc::TG711CodecMode& aMode); //n/a through CIs
    TInt SetCNG(TBool aCng);
    TInt GetCNG(TBool& aCng);
    TInt SetPLC(TBool aPlc);
    TInt GetPLC(TBool& aPlc); //n/a through CIs

private:
    CVoIPG711DecoderIntfcImpl();
    void ConstructL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);
    };

#endif //VOIPG711DECFORMATINTFCIMPL_H

// End of file
