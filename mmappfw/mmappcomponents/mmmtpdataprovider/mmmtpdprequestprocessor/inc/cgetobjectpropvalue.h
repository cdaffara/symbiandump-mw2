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
* Description:  Get object prop value
*
*/


#ifndef CGETOBJECTPROPVALUE_H
#define CGETOBJECTPROPVALUE_H

#include "crequestprocessor.h"

// forward declaration
class TMTPTypeUint8;
class TMTPTypeUint16;
class TMTPTypeUint32;
class TMTPTypeUint64;
class TMTPTypeUint128;
class CMTPTypeString;
class CMTPTypeArray;
class MMmMtpDpConfig;


/**
* Defines GetObjectPropValue request processor
*/
class CGetObjectPropValue : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper    medadata access interface
    * @return a pointer to the created request processor object
    */
    //IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework, MMTPConnection& aConnection, CMmMtpDpMetadataAccessWrapper& aWrapper );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CGetObjectPropValue();

protected:
    /**
    * Standard c++ constructor
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper    medadata access interface
    */
    IMPORT_C CGetObjectPropValue( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

protected:
    //from CRequestProcessor
    /**
    * Verify the reqeust
    * @return EMTPRespCodeOK if request is verified, otherwise one of
    *      the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

    /**
    * GetObjectPropValue request handler
    */
    IMPORT_C void ServiceL();

protected:
    // new virtuals
    virtual void ServiceSpecificObjectPropertyL( TUint16 aPropCode ) = 0;

protected:
    /**
    * Get MetaData from CMetadataAccessWrapper and send data
    * or response to initiator. for internal use
    * @param aPropCode, specify property code of aMediaProp
    * @param aNewData, object property value which will be get from
    *    aObjectMetaData
    * @param aObjectMetaData, owner of the property which should be
    *    inserted or updated into database
    * @return void
    */
    IMPORT_C void ServiceMetaDataFromWrapperL( const TUint16 aPropCode,
        MMTPType& aNewData,
        const CMTPObjectMetaData& aObjectMetaData );

protected:
    // object which is prepare for PC
    CMTPObjectMetaData* iObjectInfo;

    MMmMtpDpConfig& iDpConfig;
    TMTPTypeUint8 iMTPTypeUint8;
    TMTPTypeUint16 iMTPTypeUint16;
    TMTPTypeUint32 iMTPTypeUint32;
    TMTPTypeUint64 iMTPTypeUint64;
    TMTPTypeUint128 iMTPTypeUint128;
    CMTPTypeString* iMTPTypeString;
    CMTPTypeArray* iMTPTypeArray;

    };

#endif // CGETOBJECTPROPVALUE_H
