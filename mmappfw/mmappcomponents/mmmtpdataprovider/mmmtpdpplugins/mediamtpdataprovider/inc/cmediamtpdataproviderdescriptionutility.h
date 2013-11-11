/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Media dp specific property description construction utility.
*              Used by all request processors which related to property description
*
*/

#ifndef CMEDIAMTPDATAPROVIDERDESCRIPTIONUTILITY_H
#define CMEDIAMTPDATAPROVIDERDESCRIPTIONUTILITY_H

#include "cdescriptionutility.h"

class CMTPTypeInterdependentPropDesc;
class CMTPTypeObjectPropDesc;

class CMediaMtpDataProviderDescriptionUtility : public CDescriptionUtility
    {
public:
    static CMediaMtpDataProviderDescriptionUtility* NewL();

private:
    CMediaMtpDataProviderDescriptionUtility();

public:
    /**
     * Construct media dp specific interdependent property description.
     * according to assigned format code.
     * @param aFormatCode
     * @return Interdependent property description dataset.
     */
    CMTPTypeInterdependentPropDesc* NewInterdepentPropDescL( MMmMtpDpConfig& aDpConfig, TUint aFormatCode );

    /**
     * Construct media dp specific property description
     * according to assigned format code and property code.
     * @param aFormatCode
     * @param aPropCode
     * @return Property description dataset.
     */
    CMTPTypeObjectPropDesc* NewSpecificPropDescL( TUint aFormatCode, TUint16 aPropCode );

private:

    /**
     * Append WM Audio specific to interdpendent property description.
     * @return None
     */
    void AppendWMAudioInterdepentPropDescL(CMTPTypeInterdependentPropDesc* aInterdependentPropDesc);

    /**
     * Construct audio wave codec property description.
     * @param aFormatCode
     * @return Property description dataset of audio wave codec.
     */
    CMTPTypeObjectPropDesc* NewAudioWaveCodecPropDescL( TUint aFormatCode );

    /**
     * Construct audio bitrate property description.
     * @param aFormatCode, The format code
     * @return Property description dataset of audio bitrate.
     */
    CMTPTypeObjectPropDesc* NewAudioBitratePropDescL( TUint aFormatCode );

    /**
     * Construct property description of video fourCC codec.
     * @param aFormatCode, The format code
     * @return Property description dataset of video fourCC codec.
     */
    CMTPTypeObjectPropDesc* NewVideoFourCCCodecPropDescL( TUint aFormatCode );

    /**
     * Construct property description of VideoBitrate.
     * @param aFormatCode, The format code
     * @return Property description dataset of video bitrate.
     */
    CMTPTypeObjectPropDesc* NewVideoBitratePropDescL( TUint aFormatCode );

    /**
     * Construct property description of number of channels.
     * @return Property description dataset of number of channels.
     */
    CMTPTypeObjectPropDesc* NewNumberOfChannelsPropDescL();

    /**
     * Construct property description of code sample rate.
     * @return Property description dataset of sample rate.
     */
    CMTPTypeObjectPropDesc* NewCodeSampleRatePropDescL();

    /**
     * Construct property description of description.
     * @return Property description dataset of description.
     */
    CMTPTypeObjectPropDesc* NewDescriptionPropDescL();

    /**
     * Construct property description of scan type description.
     * @return Property description dataset of scan type description.
     */
    CMTPTypeObjectPropDesc* NewScanTypeDescriptionPropDescL();

    /**
     * Construct property description of encoding profile description.
     * @return Property description dataset of encoding profile description.
     */
    CMTPTypeObjectPropDesc* NewEncodingProfileDescriptionPropDescL();

    /**
     * Construct property description of DRM status.
     * @return Property description dataset of drm status.
     */
    CMTPTypeObjectPropDesc* NewDRMStatusPropDescL();

    /**
     * Construct property description of OMADRM status.
     * @return Property description dataset of oma drm status.
     */
    CMTPTypeObjectPropDesc* NewOMADRMStatusPropDescL();

    };

#endif // CMEDIAMTPDATAPROVIDERDESCRIPTIONUTILITY_H
