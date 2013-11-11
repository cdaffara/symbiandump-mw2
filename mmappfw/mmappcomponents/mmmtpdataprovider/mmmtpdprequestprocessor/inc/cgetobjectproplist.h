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
* Description:  Get object proplist operation
*
*/


#ifndef CGETOBJECTPROPLIST_H
#define CGETOBJECTPROPLIST_H

#include "crequestprocessor.h"
#include "cmmmtpdpperflog.h"

// forward declaration
class CMTPTypeArray;
class CMTPTypeObjectPropList;
class MMmMtpDpConfig;
class CMTPTypeObjectPropListElement;

/**
* Defines GetObjectPropList request processor
*/
class CGetObjectPropList : public CRequestProcessor
    {
public:
    /**
    * Destructor
    */
    IMPORT_C virtual ~CGetObjectPropList();

protected:
    /**
    * Standard C++ Constructor
    * @param aFramework, The data provider framework
    * @param aConnection, The connection from which the request comes
    * @param aDpConfig, The interface of dataprovider configuration
    */
    IMPORT_C CGetObjectPropList( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * 2nd Phase Constructor
    */
    IMPORT_C void ConstructL();

protected:
    // from CRequestProcessor
    /**
    * Service a request at request phase
    */
    IMPORT_C void ServiceL();

    /**
    * Check the current request
    * @return EMTPRespCodeOK if the reqeust is good, otherwise,
    *     one of the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

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

protected:
    // new virtuals
    virtual TInt ServiceSpecificObjectPropertyL( TUint16 aPropCode, TUint32 aHandle ) = 0;

private:
    /**
    * Ensures the object format operation parameter is valid
    * (done for CheckRequestL)
    * @return Response code of the check result
    */
    TMTPResponseCode CheckFormatL() const;

    /**
    * Ensures the object prop code operation parameter is valid
    * (done for CheckRequestL)
    * @return Response code of the check
    */
    TMTPResponseCode CheckPropCodeL() const;

    /**
    * Ensures the depth operation parameter is valid (done for CheckRequestL)
    * @return Response code of the check
    */
    TMTPResponseCode CheckDepth() const;

    /**
    * Gets the handles for the objects that we want the properties for
    */
    void GetObjectHandlesL();

    /**
    * Gets all object handles (for GetObjectHandlesL)
    * @param aStorageId, storage ID
    * @param aParentHandle, parent handle
    * @param aFormat, format
    */
    void GetObjectHandlesL( TUint32 aStorageId, TUint32 aParentHandle, TUint16 aFormat = 0x0000 );

    /**
    * Gets the handles of all objects on the root (for GetObjectHandlesL)
    * @param aDepth, depth of file structure
    * @param aFormat, format
    */
    void GetRootObjectHandlesL( TUint32 aDepth, TUint16 aFormat );

    /**
    * Gets a folder object handle (for GetObjectHandlesL)
    * @param aDepth, depth of file structure
    * @param aParentHandle, parent handle
    */
    void GetFolderObjectHandlesL( TUint32 aDepth, TUint32 aParentHandle );

    /**
    * Gets a specified object handle (for GetObjectHandlesL)
    * @param aHandle, object handler
    */
//    void GetSpecificObjectHandlesL( TUint32 aHandle );

    /**
    * Gets the object property information for the required object
    * @param aHandle, object handler
    * @param aPropCode, property code
    * @return error code
    */
    TInt ServiceOneObjectPropertyL( TUint32 aHandle, TUint16 aPropCode );

    /**
    * Gets a list of all the properties supported
    * @param aHandle, object handler
    */
    TInt ServiceAllPropertiesL( TUint32 aHandle );

    TInt ServiceGroupPropertiesL( TUint32 aHandle );

    void GetPropertiesL( RArray<TUint>& aPropArray, TUint32 aGroupCode, TUint16 aFormatCode ) const;

protected:
    CMTPTypeObjectPropListElement* iPropertyElement;  // used by media DP directly

    // object which is prepare for PC
    CMTPObjectMetaData* iObject;

    // Array of the object handles to get object properties about
    RArray<TUint> iHandles;

    // Final object property array to send back
    CMTPTypeObjectPropList* iPropertyList;

    MMmMtpDpConfig& iDpConfig;

    mutable RArray<TUint> iPropertyArray;

    mutable TUint32 iHandle;
    mutable TUint32 iDepth;
    mutable TUint32 iPropCode;
    mutable TUint32 iFormatCode;

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    CMmMtpDpPerfLog* iPerfLog;
#endif

    };

#endif // CGETOBJECTPROPLIST_H
