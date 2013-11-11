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
 * Description:
 *
 */


#ifndef CMEDIAMTPDATAPROVIDERPROPERTYSETTINGUTILITY_H
#define CMEDIAMTPDATAPROVIDERPROPERTYSETTINGUTILITY_H

#include "cpropertysettingutility.h"

/**
* Defines media data provider SetObjectPropsList request processor
*/
class CMediaMtpDataProviderPropertySettingUtility: public CPropertySettingUtility
    {
public:
    /**
    * @param aFramework, The data provider framework
    * @param aConnection, The connection from which the request comes
    * @param aWrapper Medadata access
    * @return A pointer to the created request processor object
    */
    static CMediaMtpDataProviderPropertySettingUtility* NewL();

    /**
    * Destructor
    */
    ~CMediaMtpDataProviderPropertySettingUtility();

private:
    /**
    * Standard C++ Constructor
    * @param aFramework, The data provider framework
    * @param aConnection, The connection from which the request comes
    * @param aWrapper Medadata access
    */
    CMediaMtpDataProviderPropertySettingUtility();

protected:
    TMTPResponseCode SetSpecificObjectPropertyL( MMmMtpDpConfig& aDpConfig,
        TUint16 aPropCode,
        const CMTPObjectMetaData& aObject,
        const CMTPTypeObjectPropListElement& aElement );

    };

#endif // CMEDIAMTPDATAPROVIDERPROPERTYSETTINGUTILITY_H
