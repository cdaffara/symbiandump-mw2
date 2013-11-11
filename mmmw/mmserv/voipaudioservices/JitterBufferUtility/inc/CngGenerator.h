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
 * Description:  Declares class for comfort noise generating utility (CNG)
 *
 */

#ifndef CNGGENERATOR_H
#define CNGGENERATOR_H

#include <mmfutilities.h>

// FORWARD DECLARATIONS
class MJitterBufferObserver;

/**
 * Comfort noise generating utility
 *
 * @lib MMccJitterBuffer.lib
 */
NONSHARABLE_CLASS (CVoIPCNGenerator) : public CBase
    {
public:
    // Constructor and destructor

    /**
     * Two phased constructor
     */
    static CVoIPCNGenerator* NewL(MJitterBufferObserver* iObserver,
            const TFourCC& aCodec);

    /**
     * Destructor
     */
    virtual ~CVoIPCNGenerator();

public:
    // New functions

    /**
     * Generates an SID packet to given buffer
     * @param aPayload Buffer to be filled with SID data
     * @param aRequestSize Amount of data to be generated to buffer.
     * @return void
     */
    void GenerateSidPacket(TDes8& aPayload, TInt aRequestSize);

    /**
     * Requests error concealment for the next frame.
     * @param None
     * @return void
     */
    void ConcealErrorForNextFrame() const;

#ifdef __FEATURE_NOT_SUPPORTED__
    /**
     * Returns current DTX status
     * @param None
     * @return TBool ETrue if DTX period ongoing
     */
    TBool DtxPeriodStatus() const;

    /**
     * Decide whether we are in DTX period or in normal speech period
     * @param const TDes8& - Buffer containing data
     * @return TInt
     */
    void DoDtxDecision(const TDes8& aPayload);

    /**
     * Checks whether buffer contains SID frame.
     *
     * @param const TDes8& - Buffer containing data
     * @return TBool - ETrue if buffer contains SID frame
     */
    TBool IsSidBuffer(const TDes8& aData);
#endif //__FEATURE_NOT_SUPPORTED__

private:
    // New functions

    /**
     * Generates an AMR CN packet into aPayload
     *
     * @param aPayload
     * @return void
     */
    void GenerateAmrNoDataPacket(TDes8& aPayload) const;

    /**
     * Generates an VoIP NO_DATA frame into aPayload
     *
     * @param aPayload Buffer to be filled with VoIP NO_DATA information
     * @param aRequestSize Amount of data to be generated to buffer.
     * @return void
     */
    void GenerateVoIPNoDataPacket(TDes8& aPayload, TInt aRequestSize) const;

private:
    //Constructors

    /**
     * C++ default constructor.
     */
    CVoIPCNGenerator(MJitterBufferObserver* iObserver, const TFourCC& aCodec);

    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();

private:
    // Data

    // Codec type
    TFourCC iCodec;

    // For Error Concealment IF creation
    MJitterBufferObserver* iObserver;

    // Generated CN frames
    TInt iGeneratedCNFrames;

    // DTX period started
    TBool iDtxPeriodStarted;
    };

#endif  // CNGGENERATOR_H

// End of file
