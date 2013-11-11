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
* Description:  Set Object proplist operation.
*
*/


#ifndef CSETOBJECTPROPLIST_H
#define CSETOBJECTPROPLIST_H

#include "crequestprocessor.h"

// forward declaration
class CMTPTypeObjectPropList;
class CMmMtpDpMetadataAccessWrapper;
class MMTPObjectMgr;
class CMTPObjectMetaData;
class MMmMtpDpConfig;

/**
* Defines SetObjectPropList request processor
*/
class CSetObjectPropList : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework The data provider framework
    * @param aConnection The connection from which the request comes
    * @param aDpConfig Configuration of data provider
    * @return The pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CSetObjectPropList();

private:
    /**
    * Standard c++ constructor
    * @param aFramework The data provider framework
    * @param aConnection The connection from which the request comes
    * @param aDpConfig Configuration of data provider
    * @return The pointer to the created request processor object
    */
    CSetObjectPropList( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * 2nd Phase Constructor
    */
    void ConstructL();

protected:
    // from CRequestProcessor
    /**
    * Verify the reqeust
    * @return EMTPRespCodeOK if request is verified, otherwise one of
    *     the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

    /**
    * SetObjectPropList request handler
    */
    IMPORT_C void ServiceL();

    /**
    * Completing phase for the request handler
    */
    IMPORT_C TBool DoHandleResponsePhaseL();

    /**
    * Derived from CRequestProcessor, can not be neglected
    * @return ETrue
    */
    IMPORT_C TBool HasDataphase() const;

    // From CActive
    /**
    * Cancel the enumeration process
    */
    IMPORT_C void DoCancel();

    /**
    * Handle completed request
    */
    IMPORT_C void RunL();

    /**
    * Ignore the error, continue with the next one
    */
    IMPORT_C TInt RunError( TInt aError );

private:
    /*
    * Handle response phase of SetObjectPropListL operation
    */
    TMTPResponseCode SetObjectPropListL( const CMTPTypeObjectPropListElement& aPropListElement );

protected:
    MMTPObjectMgr& iObjectMgr;
    RFs& iFs;
    CMTPTypeObjectPropList* iPropertyList;
    MMmMtpDpConfig& iDpConfig;
    TUint32  iUnprocessedIndex;
    TInt  iElementCount;

    };

#endif // CSETOBJECTPROPLIST_H
