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

#ifndef CPROPERTYSETTINGUTILITY_H
#define CPROPERTYSETTINGUTILITY_H

#include <mtp/mtpprotocolconstants.h>   // TMTPResponseCode

class MMmMtpDpConfig;
class MMTPType;
class CMTPObjectMetaData;
class CMTPTypeObjectPropListElement;

class CPropertySettingUtility : public CBase
    {
protected:
    IMPORT_C CPropertySettingUtility();

public:
    IMPORT_C virtual ~CPropertySettingUtility();

public:
    /**
    * Set metadata to database for processors which need set common property list
    * @param aDpConfig, handler to get db access wrapper
    * @param aPropCode, specify property code of aMediaProp
    * @param aNewData, object property value which will be get from
    *    aObjectMetaData
    * @param aObjectMetaData, owner of the property which should be
    *    inserted or updated into database
    * @return response code
    */
    IMPORT_C TMTPResponseCode SetMetaDataToWrapper( MMmMtpDpConfig& aDpConfig,
        const TUint16 aPropCode,
        MMTPType& aNewData,
        const CMTPObjectMetaData& aObjectMetaData );

    /**
     * Interface to set metadata to database for dp specific objectproplist
     * @param aDpConfig, handler to get db access wrapper
     * @param aPropCode, specify property code of aMediaProp
     * @param aObjectMetaData, owner of the property which should be
     *    inserted or updated into database
     * @param aElement, property list element
     */
    virtual TMTPResponseCode SetSpecificObjectPropertyL( MMmMtpDpConfig& aDpConfig,
        TUint16 aPropCode,
        const CMTPObjectMetaData& aObject,
        const CMTPTypeObjectPropListElement& aElement ) = 0;

    };

#endif // CPROPERTYSETTINGUTILITY_H
