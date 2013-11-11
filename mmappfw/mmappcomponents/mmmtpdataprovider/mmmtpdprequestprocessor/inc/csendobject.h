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
* Description:  SendObjectInfo/SendObjectPropList/SendObject operation
*
*/


#ifndef CSENDOBJECT_H
#define CSENDOBJECT_H

#include "crequestprocessor.h"

// forward declaration
class RFs;
class CMTPTypeObjectInfo;
class MMTPObjectMgr;
class CMTPTypeFile;
class CMTPTypeObjectPropListElement;
class CMTPTypeObjectPropList;
class TMTPTypeUint32;
class CMTPObjectMetaData;
class CMTPObjectPropertyMgr;
class MMmMtpDpConfig;

/**
* Defines SendObjectInfo/SendObject request processor
*/
class CSendObject : public CRequestProcessor
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
    IMPORT_C virtual ~CSendObject();

protected:
    /**
    * Standard C++ construction method
    * @param aFramework The data provider framework
    * @param aConnection The connection from which the request comes
    * @param aDpConfig Configuration of data provider
    * @return The pointer to the created request processor object
    */
    CSendObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * 2nd Phase Constructor
    */
    void ConstructL();

protected:
    // from CRequestProcessor
    /**
    * Override to match both the SendObjectInfo and SendObject requests
    * @param aRequest    The request to match
    * @param aConnection The connection from which the request comes
    * @return ETrue if the processor can handle the request, otherwise EFalse
    */
    IMPORT_C TBool Match( const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection ) const;

    /**
    * Verify the reqeust
    * @return EMTPRespCodeOK if request is verified, otherwise one of the
    *     error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

    /**
    * Override from CRequestProcessor, can not be neglected
    * @return ETrue
    */
    IMPORT_C TBool HasDataphase() const;

    /**
    * SendObjectInfo/SendObject request handler
    */
    IMPORT_C void ServiceL();

    /**
    * Response Phase Handler
    */
    IMPORT_C TBool DoHandleResponsePhaseL();

    /**
    * Completeing phase Handler
    */
    IMPORT_C TBool DoHandleCompletingPhaseL();

    /**
    * Override from CRequestProcessor, can not be neglected
    */
    IMPORT_C void UsbDisconnect();

private:
    /**
    * Verify if the SendObject request comes after SendObjectInfo request
    * @return EMTPRespCodeOK if SendObject request comes after a valid
    *     SendObjectInfo request, otherwise
    * EMTPRespCodeNoValidObjectInfo
    */
    TMTPResponseCode CheckSendingStateL();

    /**
    * Handle request phase of SendObjectInfo operation
    */
    void ServiceInfoL();

    /**
     * Handle request phase of SendObjectPropList operation
     */
    void ServicePropListL();

    /**
    * Handle request phase of SendObject operation
    */
    void ServiceObjectL();

    /**
    * Handle response phase of SendObjectInfo operation
    */
    TBool DoHandleResponsePhaseInfoL();

    /**
     * Handle response phase of SendObject operation
     */
    TBool DoHandleResponsePhasePropListL();

    /**
    * Handle response phase of SendObject operation
    */
    TBool DoHandleResponsePhaseObjectL();

    /**
     *
     */
    TMTPResponseCode VerifyObjectPropListL(TInt& aInvalidParameterIndex);

    /**
     *
     */
    TMTPResponseCode CheckPropCodeL(const CMTPTypeObjectPropListElement& aElement);

    /**
     *
     */
    TMTPResponseCode ExtractPropertyL(const CMTPTypeObjectPropListElement& aElement);

    /*
     * Handle response phase of SetObjectPropListL operation
     */
    TMTPResponseCode SetObjectPropListL();

    /**
     *
     */
    TMTPResponseCode MatchStoreAndParentL();

    /**
    * Check if the object is too large
    * @return ETrue if yes, otherwise EFalse
    */
    TBool IsTooLarge( TUint64 aObjectSize ) const;

    /**
    * Get the full path name of the object to be saved
    * @param aFileName, on entry, contains the file name of the object,
    * on return, contains the full path name of the object to be saved
    */
    TBool GetFullPathNameL( const TDesC& aFileName );

    /**
    * Check if the file already exists on the storage.
    * @return ETrue if file is exists, otherwise EFalse
    */
    TBool ExistsL( const TDesC& aName ) const;

    /**
    * Reserve object information before SendObject
    */
    TInt ReserveObjectL();

    /**
    * Set file properties of object
    */
    void SetFileProperties();

    /**
    * Save object information of object whose size is zero
    */
    void SaveEmptyFileL();


    /**
    * Add file information into database in response phase of SendObject
    */
    void AddMediaToStoreL();

    /**
    * Rollback functions
    */
    void Rollback();

    void UnreserveObjectL();
    void RemoveObjectFromDbL();
    void RemoveObjectFromFs();

private:
    enum TMTPSendingObjectState
        {
        EObjectNone,
        EObjectInfoInProgress,
        EObjectInfoSucceed,
        EObjectInfoFail,
        ESendObjectInProgress,
        ESendObjectSucceed,
        ESendObjectFail
        };

    typedef void ( CSendObject::*TMmMtpRollbackAction )();

private:
    RFs& iFs;
    MMTPObjectMgr& iObjectMgr;
    MMmMtpDpConfig& iDpConfig;

    // received dataset
    CMTPTypeObjectInfo* iObjectInfo;
    CMTPTypeObjectPropList* iObjectPropList;
    CMTPTypeFile* iFileReceived;

    // object which need to reserved
    CMTPObjectMetaData* iReceivedObjectInfo;

    // progress of request
    TMTPSendingObjectState iProgress;

    // request params
    TUint16 iOperationCode;
    TUint32 iStorageId;
    TUint32 iParentHandle;
    TUint16 iObjectFormat;
    TUint64 iObjectSize;

    // cache info during SendObjectInfo/SendObjectProplist for file properties
    TUint16 iProtectionStatus;
    HBufC* iDateMod;

    // cache info during SendObjectInfo
    TUint32 iWidth;
    TUint32 iHeight;

    TFileName iFullPath;
    HBufC* iParentSuid;

    // params which is needed for reserve handle
    TMTPTypeRequest iExpectedSendObjectRequest;

    RArray<TMmMtpRollbackAction> iRollbackList;

    TUint32 iPreviousTransactionID;
    TUint32 iPreviousOperation;

    };

#endif // CSENDOBJECT_H
