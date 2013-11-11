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

#ifndef VOIPBASECODECINTFCIMPL_H
#define VOIPBASECODECINTFCIMPL_H

#include <voipformatintfc.h>
#include "VoIPFormatIntfcImpl.h"

// FORWARD DECLARATION
class CVoIPAudioDownlinkStreamImpl;
class CVoIPAudioUplinkStreamImpl;

// -----------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPBaseCodecIntfcImpl) : public CBase,
        public CVoIPFormatIntfc,
        public CVoIPFormatIntfcImpl
    {
public:
    static CVoIPBaseCodecIntfcImpl*
    NewL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);

    static CVoIPBaseCodecIntfcImpl*
    NewL(CVoIPAudioUplinkStreamImpl* auPLinkStreamImpl);

    virtual ~CVoIPBaseCodecIntfcImpl();

private:
    CVoIPBaseCodecIntfcImpl();

    void ConstructL(CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl);
    void ConstructL(CVoIPAudioUplinkStreamImpl* aDnLinkStreamImpl);
    };

#endif //VOIPBASECODECINTFCIMPL_H

// End of file
