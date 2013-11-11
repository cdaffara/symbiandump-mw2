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
* Description: Abstract media dp specific property description construction utility.
*              Used by all request processors which related to property description
*
*/

#ifndef CABSTRACTMEDIAMTPDATAPROVIDERDESCRIPTIONUTILITY_H
#define CABSTRACTMEDIAMTPDATAPROVIDERDESCRIPTIONUTILITY_H

#include "cdescriptionutility.h"

class CMTPTypeInterdependentPropDesc;
class CMTPTypeObjectPropDesc;

class CAbstractMediaMtpDataProviderDescriptionUtility : public CDescriptionUtility
    {
public:
    static CAbstractMediaMtpDataProviderDescriptionUtility* NewL();

    ~CAbstractMediaMtpDataProviderDescriptionUtility();

private:
    CAbstractMediaMtpDataProviderDescriptionUtility();

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
     * Construct property description of SampleFormat.
     * @return Property description dataset of sample format.
     */
    CMTPTypeObjectPropDesc* NewRepresentativeSampleFormatL();                   

    /**
     * Construct property description of SampleSize.
     * @return Property description dataset of sample size.
     */
    CMTPTypeObjectPropDesc* NewRepresentativeSampleSizeL();
    
    /**
     * Construct property description of SampleHeight.
     * @return Property description dataset of sample height.
     */
    CMTPTypeObjectPropDesc* NewRepresentativeSampleHeightL();
    
    /**
     * Construct property description of SampleWidth.
     * @return Property description dataset of sample width.
     */
    CMTPTypeObjectPropDesc* NewRepresentativeSampleWidthL();
    
    /**
     * Construct property description of SampleData.
     * @return Property description dataset of sample data.
     */
    CMTPTypeObjectPropDesc* NewRepresentativeSampleDataL();
    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERDESCRIPTIONUTILITY_H
