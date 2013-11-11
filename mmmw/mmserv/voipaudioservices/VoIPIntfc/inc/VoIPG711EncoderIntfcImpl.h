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

#ifndef VOIPG711ENCFORMATINTFCIMPL_H
#define VOIPG711ENCFORMATINTFCIMPL_H

#include <voipg711encoderintfc.h>
#include "VoIPFormatIntfcImpl.h"

// FORWARD DECLARATION
class CVoIPAudioUplinkStreamImpl;

// -----------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPG711EncoderIntfcImpl) : public CBase,
        public CVoIPG711EncoderIntfc,
        public CVoIPFormatIntfcImpl
    {
public:
    static CVoIPG711EncoderIntfcImpl*
    NewL(CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl);

    virtual ~CVoIPG711EncoderIntfcImpl();

    TInt SetMode(CVoIPFormatIntfc::TG711CodecMode aMode);
    TInt GetMode(CVoIPFormatIntfc::TG711CodecMode& aMode); //n/a through CIs

private:
    CVoIPG711EncoderIntfcImpl();
    void ConstructL(CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl);
    };

#endif //VOIPG711ENCFORMATINTFCIMPL_H

// End of file
