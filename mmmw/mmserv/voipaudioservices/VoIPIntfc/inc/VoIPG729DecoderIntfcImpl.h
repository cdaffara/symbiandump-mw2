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

#ifndef VOIPG729DECFORMATINTFCIMPL_H
#define VOIPG729DECFORMATINTFCIMPL_H

#include <voipg729decoderintfc.h>
#include "VoIPFormatIntfcImpl.h"

// FORWARD DECLARATION
class CVoIPAudioDownlinkStreamImpl;

// -----------------------------------------------------------------------------
// CVoIPG729DecoderIntfcImpl class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPG729DecoderIntfcImpl) : public CBase,
        public CVoIPG729DecoderIntfc,
        public CVoIPFormatIntfcImpl
    {
public:
    static CVoIPG729DecoderIntfcImpl*
    NewL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);

    virtual ~CVoIPG729DecoderIntfcImpl();

    TInt BadLsfNextBuffer();

private:
    CVoIPG729DecoderIntfcImpl();
    void ConstructL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);
    };

#endif //VOIPG729DECFORMATINTFCIMPL_H

// End of file
