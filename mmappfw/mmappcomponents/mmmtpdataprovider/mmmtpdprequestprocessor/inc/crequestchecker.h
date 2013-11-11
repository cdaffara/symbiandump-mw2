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


#ifndef CREQUESTCHECKER_H
#define CREQUESTCHECKER_H

#include <mtp/tmtptyperequest.h>
#include <mtp/cmtpobjectmetadata.h>

class MMTPDataProviderFramework;
class MMTPConnection;

/**
* element type in the request
*/
enum TMTPElementType
    {
    /**
    * Session ID type
    */
    EMTPElementTypeSessionID,

    /**
    * Object Handle type
    */
    EMTPElementTypeObjectHandle,

    /**
    * Format code type
    */
    EMTPElementTypeFormatCode,

    /**
    * Storeage Id type
    */
    EMTPElementTypeStorageId,
    };

/**
* element attribute to check in the reqeust
*/
enum TMTPElementAttr
    {
    /**
    * no attribute to check
    */
    EMTPElementAttrNone = 0x0000,

    /**
    * the object/storage should be readable
    */
    EMTPElementAttrRead = 0x0001,

    /**
    * the object/storage should be writable
    */
    EMTPElementAttrWrite = 0x0002,

    /**
    * the object should be a file
    */
    EMTPElementAttrFile = 0x0004,

    /**
    * the object should be a directory
    */
    EMTPElementAttrDir = 0x0008,

    /**
    * supported format array
    */
    EMTPElementAttrFormatEnums = 0x0010,

    /**
    * The object can be either a file or directory.
    */
    EMTPElementAttrFileOrDir = 0x0020,
    };

/**
* defines the verification data structure
*/
struct TMTPRequestElementInfo
    {
    TMTPTypeRequest::TElements iElementIndex; //which element in the request to check
    TMTPElementType iElementType; //the type of the element
    TInt iElementAttr; //the attribute of the element
    TInt iCount; //number of special values (0, 1, 2)
    TUint32 iValue1; //special value 1
    TUint32 iValue2; //special value 2
    };

/**
* defines a generic request verification class.  It iterates through the verification data elements,
* and performs the check based on the element type/attribute.
*/
class CRequestChecker : public CBase
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework   The data provider framework
    * @param aConnection  The connection object
    * @return a pointer to the created request checker object
    */
    static CRequestChecker* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

    /**
    * Destructor
    */
    ~CRequestChecker();

    /**
    * Verfiy the request.  It check the request header first (session id and transaction code), and check for special values, last
    * it iterates through the verification elements for checking individul parameters in the request
    * @param aRequest The request object to verify
    * @param aCount   The number of verification elements
    * @param aElementInfo The array of verification elements
    * @return reponse code to return to the initiator
    */
    TMTPResponseCode VerifyRequestL( const TMTPTypeRequest& aRequest,
        TInt aCount,
        const TMTPRequestElementInfo* aElementInfo );

    /**
    * Return the object info for the handle.
    * This is to remove extra expensive DMBS retrieval operations.
    * @param aHandle  the handle of the object requested
    * @return an object info for the handle
    */
    CMTPObjectMetaData* GetObjectInfo( TUint32 aHandle ) const;

private:
    /**
    * Check the request header portion (session Id and transaction code)
    * @param aRequest the request object to check
    * @return repsonse code to return to initiator
    */
    TMTPResponseCode CheckRequestHeader( const TMTPTypeRequest& aRequest ) const;

    /**
    * Check the session id in the request parameter (NOTE the session id is different from the one in the request header),
    * this usually only applies to the OpenSession request
    * @param aSessionId   Session id of the request.
    * @param aElementInfo ElementInfo data array to check against.
    * @return repsonse code to return to initiator
    */
    TMTPResponseCode VerifySessionId( TUint32 aSessionId,
        const TMTPRequestElementInfo& aElementInfo ) const;

    /**
    * Check the object handle in the request parameter, whether the handle is in the object store, read/write, file/dir
    * @param aHandle  Object handle to be checked.
    * @param aElementInfo Element info array to be checked against.
    * @return repsonse code to return to initiator
    */
    TMTPResponseCode VerifyObjectHandleL( TUint32 aHandle,
        const TMTPRequestElementInfo& aElementInfo );

    /**
    * Check the storage id parameter in the request, read/write attributes
    * @param aStorageId   Storage id to be checked.
    * @param aElementInfo Element info array to be checked against.
    * @return repsonse code to return to initiator
    */
    TMTPResponseCode VerifyStorageIdL( TUint32 aStorageId,
        const TMTPRequestElementInfo& aElementInfo ) const;

    /**
    * Check the format code parameter in the request,
    * @param aStorageId   aFormatCode to be checked.
    * @param aElementInfo Element info array to be checked against.
    * @return repsonse code to return to initiator
    */
    TMTPResponseCode VerifyFormatCode( TUint32 aFormatCode,
        const TMTPRequestElementInfo& aElementInfo ) const;

    /**
    * Check if the parameter is one of the special values
    * @param aParameter   The parameter value in the request
    * @param aElementInfo The ElementInfo for the parameter
    * @return ETrue if the parameter is one of the special values, otherwise, EFalse
    */
    TBool IsSpecialValue( TUint32 aParameter,
        const TMTPRequestElementInfo& aElementInfo ) const;

private:
    /**
    * Standard c++ constructor
    */
    CRequestChecker( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );
    
    /**
    * Two-phase construction
    */
    void ConstructL();

private:
    MMTPDataProviderFramework& iFramework;
    MMTPConnection& iConnection;
    RArray<TUint32> iHandles; //these two arrays contain the mapping from the handle to objectinfo
    RPointerArray<CMTPObjectMetaData> iObjectArray;
    
    };

#endif // CREQUESTCHECKER_H
