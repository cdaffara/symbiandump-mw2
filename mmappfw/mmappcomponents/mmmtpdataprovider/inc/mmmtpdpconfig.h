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


#ifndef MMMTPDPCONFIG_H
#define MMMTPDPCONFIG_H

class CMmMtpDpMetadataAccessWrapper;
class CPropertySettingUtility;
class CDescriptionUtility;

class MMmMtpDpConfig
    {
public:
    /**
    * Get supported format from special data provider
    * @return The array which is used to store supported format
    */
    virtual const RArray<TUint>* GetSupportedFormat() const = 0;

    /**
    * Get the arry of property code which is inline with the format code
    * @return The array of what specified format supported
    */
    virtual const RArray<TUint>* GetSupportedPropertiesL( TUint32 aFormatCode ) const = 0;

    /**
    * Get the array of property code what dp supported all
    * @return The array of what dp supported
    */
    virtual const RArray<TUint>* GetAllSupportedProperties() const = 0;

    /**
    * Get default storage Id from resource file, if not specify a storage Id
    * @return TUint32 Default storage Id which read from resource file
    */
    virtual TUint32 GetDefaultStorageIdL() const = 0;

    /**
    * Get db handler wrapper
    * @return wrapper references
    */
    virtual CMmMtpDpMetadataAccessWrapper& GetWrapperL() = 0;

    /**
    * Get the utility to setting properties
    */
    virtual CPropertySettingUtility* PropSettingUtility() = 0;

    /**
    * Get the utiltiy to setting descriptions
    */
    virtual CDescriptionUtility* DescriptionUtility() = 0;

    };

#endif // MMMTPDPCONFIG_H
