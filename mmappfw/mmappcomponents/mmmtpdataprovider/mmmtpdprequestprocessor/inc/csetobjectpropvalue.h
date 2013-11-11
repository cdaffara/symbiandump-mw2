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
* Description:  Set object property value operation.
*
*/


#ifndef CSETOBJECTPROPVALUE_H
#define CSETOBJECTPROPVALUE_H

#include "crequestprocessor.h"

// forward declaration
class CMTPTypeString;
class CMTPTypeArray;
class TMTPTypeUint8;
class TMTPTypeUint16;
class TMTPTypeUint32;
class CMTPObjectMetaData;
class MMTPObjectMgr;
class MMmMtpDpConfig;
class RFs;

/**
* Defines SetObjectPropValue request processor for common
*/
class CSetObjectPropValue : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    * @return a pointer to the created request processor object
    */
    //IMPORT_C static MMmRequestProcessor* NewL(MMTPDataProviderFramework& aFramework, MMTPConnection& aConnection, CMmMtpDpMetadataAccessWrapper& aWrapper);

    /**
    * Destructor
    */
    IMPORT_C virtual ~CSetObjectPropValue();

protected:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    */
    IMPORT_C CSetObjectPropValue( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

protected:
    // from CRequestProcessor
    /**
    * Verify the reqeust
    * @return EMTPRespCodeOK if request is verified, otherwise one of
    *     the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

    /**
    * SetObjectPropValue request handler
    */
    IMPORT_C void ServiceL();

    /**
    * Edit the file to update the prop value
    * @return EFalse
    */
    IMPORT_C TBool DoHandleResponsePhaseL();

    /**
    * Derived from CRequestProcessor, can not be neglected
    * @return ETrue
    */
    IMPORT_C TBool HasDataphase() const;

protected:
    // new virtuals
    virtual TBool IsSpecificPropCodeReadOnly( TUint16 aPropCode ) const = 0;

    virtual void ReceiveDataSpecificObjectPropertyL( TUint16 aPropCode ) = 0;

    virtual TMTPResponseCode ServiceSpecificObjectPropertyL( TUint16 aPropCode ) = 0;

protected:
    /**
    * Set MetaData to CMetadataAccessWrapper, for internal use
    * @param aPropCode, specify property code of aMediaProp
    * @param aNewData, object property value which will be get from
    *    aObjectMetaData
    * @param aObjectMetaData, owner of the property which should be
    *    inserted or updated into database
    * @return response code
    */
    IMPORT_C TMTPResponseCode ServiceMetaDataToWrapperL( const TUint16 aPropCode,
        MMTPType& aNewData,
        const CMTPObjectMetaData& aObjectMetaData );

private:
    /*
    * Check if property could be set
    */
    TBool IsPropCodeReadonly( TUint16 aPropCode );

protected:
    //TUint32 iPropCode;
    TUint16 iPropCode;
    CMTPTypeString* iMTPTypeString;
    CMTPTypeArray* iMTPTypeArray;
    TMTPTypeUint8 iMTPTypeUint8;
    TMTPTypeUint16 iMTPTypeUint16;
    TMTPTypeUint32 iMTPTypeUint32;
    CMTPObjectMetaData* iObjectInfo;
    MMTPObjectMgr& iObjectMgr;
    MMmMtpDpConfig& iDpConfig;
    RFs& iFs;

    };

#endif // CSETOBJECTPROPVALUE_H
