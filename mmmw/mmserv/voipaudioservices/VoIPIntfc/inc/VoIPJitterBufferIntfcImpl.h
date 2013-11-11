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
 * Description:  VOIP Audio Services
 *
 */

#ifndef VOIPJITTERBUFFERINTFCIMPL_H
#define VOIPJITTERBUFFERINTFCIMPL_H

#include <voipjitterbufferintfc.h>

// FORWARD DECLARATIONS
class RVoIPAudioSession;

// -----------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl base class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPJitterBufferIntfcImpl) : public CBase,
                                                public CVoIPJitterBufferIntfc
    {
public:
    static CVoIPJitterBufferIntfcImpl* NewL(
            RVoIPAudioSession* aVoIPAudioSession);

    virtual ~CVoIPJitterBufferIntfcImpl();

    TInt SetObserver(MVoIPJitterBufferObserver& aObserver);
    TInt ConfigureJitterBuffer(const TVoIPJBConfig& aJbConfig);
    TInt ResetJitterBuffer(TBool aPlayTone);
    TInt DelayDown();
    TInt DelayUp();

#ifdef __FEATURE_NOT_SUPPORTED__
    void SendEventToObserver(TInt aEvent);
#endif //__FEATURE_NOT_SUPPORTED__

protected:
    CVoIPJitterBufferIntfcImpl();
    void ConstructL(RVoIPAudioSession* aVoIPAudioSession);

protected:
    RVoIPAudioSession* iVoIPAudioSession;
    MVoIPJitterBufferObserver* iObserver;
    };

#endif //VOIPJITTERBUFFERINTFCIMPL_H

// End of file
