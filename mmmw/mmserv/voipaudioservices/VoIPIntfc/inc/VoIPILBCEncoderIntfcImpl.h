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

#ifndef VOIPILBCENCFORMATINTFCIMPL_H
#define VOIPILBCENCFORMATINTFCIMPL_H

#include <voipilbcencoderintfc.h>
#include "VoIPFormatIntfcImpl.h"

// FORWARD DECLARATION
class CVoIPAudioUplinkStreamImpl;

// -----------------------------------------------------------------------------
// CVoIPILBCEncoderIntfc class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPILBCEncoderIntfcImpl) : public CBase,
        public CVoIPILBCEncoderIntfc,
        public CVoIPFormatIntfcImpl
    {
public:
    static CVoIPILBCEncoderIntfcImpl*
    NewL(CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl);

    virtual ~CVoIPILBCEncoderIntfcImpl();

    TInt SetMode(CVoIPFormatIntfc::TILBCCodecMode aMode);
    TInt GetMode(CVoIPFormatIntfc::TILBCCodecMode& aMode); //n/a through CIs

private:
    CVoIPILBCEncoderIntfcImpl();
    void ConstructL(CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl);
    };

#endif //VOIPILBCENCFORMATINTFCIMPL_H

// End of file
