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

#ifndef VOIPUTILITYFACTORYIMPL_H
#define VOIPUTILITYFACTORYIMPL_H

#include <mmfbase.h>
#include <voipaudiocommon.h>
#include <voiputilityfactory.h>

/**
 *  CVoIPUtilityFactoryImpl class
 *
 *  ?more_complete_description
 *
 *  @lib VoIPAudioIntfc.dll
 */
NONSHARABLE_CLASS(CVoIPUtilityFactoryImpl) : public CBase
    {
    enum TStreamType
        {
        EDownlink,
        EUplink
        };

public:

    /**
     * ?description
     *
     *
     * @param none
     * @return CVoIPUtilityFactoryImpl*
     */
    static CVoIPUtilityFactoryImpl* NewL();

    /**
     * Destructor
     *
     *
     * @param none
     * @return none
     */
    virtual ~CVoIPUtilityFactoryImpl();

    /**
     * ?description
     *
     *
     * @param TVersion
     * @param TCallType
     * @param CVoIPAudioDownlinkStream*&
     * @return void
     */
    void CreateDownlinkStreamL(TVersion aVersion,
            CVoIPUtilityFactory::TVoIPCallType aCallType,
            CVoIPAudioDownlinkStream*& aDnLink);

    /**
     * ?description
     *
     *
     * @param TVersion
     * @param CVoIPJitterBufferIntfc*&
     * @param CVoIPAudioDownlinkStream*&
     * @return void
     */
    void CreateDownlinkStreamL(TVersion aVersion,
            CVoIPJitterBufferIntfc*& aJBIntfc,
            CVoIPAudioDownlinkStream*& aDnLink);

    /**
     * ?description
     *
     *
     * @param TVersion
     * @param TCallType
     * @param CVoIPAudioUplinkStream*&
     * @return void
     */
    void CreateUplinkStreamL(TVersion aVersion,
            CVoIPUtilityFactory::TVoIPCallType aCallType,
            CVoIPAudioUplinkStream*& aUpLink);

    /**
     * ?description
     *
     *
     * @param CDTMFTonePlayer*&
     * @return void
     */
    void CreateDTMFTonePlayerL(CDTMFTonePlayer*& aDtmfPlayer);

    /**
     * ?description
     *
     *
     * @param CVoIPDataBuffer*&
     * @param const CVoIPDataBuffer::TVoIPBufferType
     * @param const TVoIPCodecFormat
     * @return void
     */
    void CreateBufferL(CVoIPDataBuffer*& aBuffer,
            const CVoIPDataBuffer::TVoIPBufferType aType,
            const TVoIPCodecFormat aCodecFormat);

    /**
     * ?description
     *
     *
     * @param RArray<TVoIPCodecFormat>&
     * @return TInt -
     */
    TInt GetSupportedDownlinkFormats(RArray<TVoIPCodecFormat>& aFormats);

    /**
     * ?description
     *
     *
     * @param RArray<TVoIPCodecFormat>&
     * @return TInt -
     */
    TInt GetSupportedUplinkFormats(RArray<TVoIPCodecFormat>& aFormats);

protected:

private:

    /**
     * Constructor
     *
     *
     * @param none
     * @return none
     */
    CVoIPUtilityFactoryImpl();

    /**
     * Symbian constructor
     *
     *
     * @param none
     * @return void
     */
    void ConstructL();

private:

    CVoIPAudioDownlinkStreamImpl* iDownLink; // not owned!
    CVoIPAudioUplinkStreamImpl* iUpLink; // not owned!
    TMMFPrioritySettings iPriorityUp;
    TMMFPrioritySettings iPriorityDn;
    };

#endif //VOIPUTILITYFACTORYIMPL_H

// End of file
