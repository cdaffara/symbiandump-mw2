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
 * Description:  Comfort noise generator of MCC jitterbuffer
 *
 */

// INCLUDE FILES
#include <mmcccodecinformation.h> //codec FourCC declarations
#include <voipjitterbufferintfc.h>
#include "debugtracemacros.h"
#include "InternalDef.h"
#include "JitterBufferObserver.h"
#include "CngGenerator.h"

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CVoIPCNGenerator* CVoIPCNGenerator::NewL(MJitterBufferObserver* aObserver,
        const TFourCC& aCodec)
    {
    CVoIPCNGenerator* self = new (ELeave) CVoIPCNGenerator(aObserver, aCodec);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::CVoIPCNGenerator
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CVoIPCNGenerator::CVoIPCNGenerator(MJitterBufferObserver* aObserver,
        const TFourCC& aCodec)
    {
    iObserver = aObserver;
    iCodec = aCodec;
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVoIPCNGenerator::ConstructL()
    {
    switch (iCodec.FourCC())
        {
        case KMccFourCCIdG711:
        case KMccFourCCIdG729:
        case KMccFourCCIdILBC:
        case KMccFourCCIdAMRNB:
            break;
        default:
            User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::~CVoIPCNGenerator
// Destructor
// -----------------------------------------------------------------------------
//
CVoIPCNGenerator::~CVoIPCNGenerator()
    {
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::CreateSidBufferL
// Creates codec-specific SID Buffer
// -----------------------------------------------------------------------------
//
void CVoIPCNGenerator::GenerateSidPacket(TDes8& aPayload, TInt aRequestSize)
    {
    if (aRequestSize > aPayload.MaxLength())
        {
        TRACE_PRN_N(_L("CVoIPCNGenerator::GenerateSidPacket->Adjust Size"));
        aRequestSize = aPayload.MaxLength();
        }

    iGeneratedCNFrames++;
    TRACE_PRN_N1(_L("JB-CNG-> Generated CN frames [%d]"), iGeneratedCNFrames);

    if (iCodec.FourCC() == KMccFourCCIdAMRNB)
        {
        GenerateAmrNoDataPacket(aPayload);

        if (iGeneratedCNFrames == KAmrNbSidUpdateInterval)
            {
            // We haven't received SID_UPDATE for the 8-th frame;
            // this means packet loss
            iDtxPeriodStarted = EFalse;
            }
        }
    else
        {
        GenerateVoIPNoDataPacket(aPayload, aRequestSize);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::GenerateAmrNoDataPacket
// Generates AMR SID packet
// -----------------------------------------------------------------------------
//
void CVoIPCNGenerator::GenerateAmrNoDataPacket(TDes8& aPayload) const
    {

    if (aPayload.MaxLength() >= KNoDataLength)
        {
        aPayload.Copy(&KAmrNoDataFrame, KNoDataLength);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::GenerateVoIPNoDataPacket
// Generates SID packet for codecs other than AMR
// -----------------------------------------------------------------------------
//
void CVoIPCNGenerator::GenerateVoIPNoDataPacket(TDes8& aPayload,
        TInt aRequestSize) const
    {

    aPayload.FillZ(aRequestSize);

    if (!iDtxPeriodStarted)
        {
        ConcealErrorForNextFrame();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::ConcealErrorForNextFrame
// Requests error concealment for the next frame
// -----------------------------------------------------------------------------
//
void CVoIPCNGenerator::ConcealErrorForNextFrame() const
    {
    iObserver->EventJB(MJitterBufferObserver::EConcealErrorForNextBuffer);
    }

#ifdef __FEATURE_NOT_SUPPORTED__
// -----------------------------------------------------------------------------
// CVoIPCNGenerator::DtxPeriodStatus
// Returns DTX status
// -----------------------------------------------------------------------------
//
TBool CVoIPCNGenerator::DtxPeriodStatus() const
    {
    return iDtxPeriodStarted;
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::DoDtxDecision()
// Determine DTX period based on codec used and data in the buffer
// -----------------------------------------------------------------------------
//
void CVoIPCNGenerator::DoDtxDecision(const TDes8& aData)
    {
    if (aData.Length() > KMinDataLenForDtx)
        {
        if (IsSidBuffer(aData))
            {
            // DTX period started or updated
            iDtxPeriodStarted = ETrue;
            iGeneratedCNFrames = 0;
            TRACE_PRN_N(_L("JB-CNG-> DTX_START"));
            }
        else
            {
            // DTX period has ended
            iDtxPeriodStarted = EFalse;
            TRACE_PRN_N(_L("JB-CNG-> DTX_END"));
            }
        }
    }

// -----------------------------------------------------------------------------
// CVoIPCNGenerator::IsSidBuffer
// Is the given buffer a SID UPDATE buffer
// -----------------------------------------------------------------------------
//
TBool CVoIPCNGenerator::IsSidBuffer(const TDes8& aData)
    {
    TBool status = ETrue;

    switch (iCodec.FourCC())
        {
        case KMccFourCCIdAMRNB:
            {
            // Get AMR mode by bit-masking first byte and shifting
            const TUint8 mode((aData[0] & KAmrModeMask) >> KModeShiftBits);
            if (mode == KAmrSidMode)
                {
                status = ETrue;
                TRACE_PRN_N(_L("JB-CNG-> SID AMRNB FRAME"));
                }
            else
                {
                status = EFalse;
                TRACE_PRN_N(_L("JB-CNG-> NON-SID AMRNB FRAME"));
                }
            break;
            }
        case KMccFourCCIdG711:
        case KMccFourCCIdILBC:
        case KMccFourCCIdG729:
            {
            // Check the frame type
            if (aData[0] == KVoIPCNFrame)
                {
                status = ETrue;
                TRACE_PRN_N(_L("JB-CNG-> SID FRAME"));
                }
            else
                {
                status = EFalse;
                TRACE_PRN_N(_L("JB-CNG-> NON-SID FRAME"));
                }
            break;
            }
        default:
            {
            status = EFalse;
            TRACE_PRN_N(_L("JB-CNG-> SID-UNKNOWN"));
            }
        }
    return status;
    }
#endif //__FEATURE_NOT_SUPPORTED__

//  End of File
