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

#ifndef VOIPFORMATINTFCIMPL_H
#define VOIPFORMATINTFCIMPL_H

// FORWARD DECLARATIONS
class RVoIPAudioSession;

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl base class
// -----------------------------------------------------------------------------
NONSHARABLE_CLASS(CVoIPFormatIntfcImpl)
    {
public:
    virtual ~CVoIPFormatIntfcImpl();

    TInt SetObserver(MVoIPFormatObserver& aObserver);
    TInt GetSupportedBitRates(RArray<TUint>& aArray);
    TInt SetBitRate(TUint aBitrate);
    TInt GetBitRate(TUint& aBitrate);
    TInt SetVAD(TBool aVad);
    TInt GetVAD(TBool& aVad);
    TInt FrameModeRequiredForErrorConcealment(TBool& aMode);
    TInt SetFrameMode(TBool aMode);
    TInt GetFrameMode(TBool& aMode); //not avail. through CIs
    TInt ConcealErrorForNextBuffer();

protected:
    CVoIPFormatIntfcImpl();
    void ConstructL(RVoIPAudioSession* aVoIPAudioSession);

protected:
    RVoIPAudioSession* iVoIPAudioSession;
    MVoIPFormatObserver* iObserver;
    };

#endif //VOIPFORMATINTFCIMPL_H

// End of file
