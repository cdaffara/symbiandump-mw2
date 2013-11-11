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
* Description:  Copy object operation
*
*/


#ifndef CCOPYOBJECT_H
#define CCOPYOBJECT_H

#include "crequestprocessor.h"

class CMTPObjectMetaData;
class CMTPTypeObjectPropList;
class CMTPTypeObjectPropListElement;
class MMmMtpDpConfig;

/**
* Defines CopyObject request processor
*/
class CCopyObject : public CRequestProcessor
    {
public:
    /**
    * Destructor
    */
    IMPORT_C virtual ~CCopyObject();

protected:
    /**
    * Standard c++ constructor
    */
    IMPORT_C CCopyObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Second phase constructor
    */
    IMPORT_C void ConstructL();

protected:
    // from CMTPRequestProcessor
    /**
    * CopyObject request handler
    */
    IMPORT_C void ServiceL();

    virtual void ServiceGetSpecificObjectPropertyL( TUint16 aPropCode,
        TUint32 aHandle,
        const CMTPObjectMetaData& aObjectMetaData ) = 0;

private:
    /**
    * Retrieve the parameters of the request
    */
    void GetParametersL();

    /**
     * Set a default destination if it is not specified
     */
    void SetDefaultParentObjectL();

    /**
    * Copy file and metadata and send response to pc
    * If everything was right, the handle of newly created file
    * should be sent back
    */
    void CopyObjectL();

    /**
    * Check if the file can be copied
    * @param aOldName used to get file size
    * @param aNewName the full path name of newly copied file
    */
    TMTPResponseCode CanCopyObjectL( const TDesC& aOldName,
        const TDesC& aNewName ) const;

    /**
    * Save the object properties before doing the copy
    */
    void GetPreviousPropertiesL();

    /**
    * Set the object properties after doing the copy
    */
    void SetPreviousPropertiesL();

    /**
    * A helper function of CopyObjectL.
    * @param aNewFileName the new full filename after copy.
    * @return handle of the newly copied object.
    */
    TUint32 CopyFileL( const TDesC& aNewFileName );

    /**
    * Add metadata of the newly created file into database and
    * update related references
    * @param aOldObjectName old reference owner
    * @param aNewObjectName new reference owner
    * @return handle of the newly copied object
    */
    TUint32 AddObjectToStoreL( const TDesC& aOldObjectName,
        const TDesC& aNewObjectName );

protected:
    CMTPTypeObjectPropListElement* iPropertyElement;
    MMmMtpDpConfig& iDpConfig;
    CMTPTypeObjectPropList* iPropertyList;

private:
    TUint32 iStorageId;
    CMTPObjectMetaData* iSourceObject;
    CMTPObjectMetaData* iTargetObject;
    HBufC* iDest;
    TUint32 iNewParentHandle;
    TTime iPreviousModifiedTime;

    };

#endif // CCOPYOBJECT_H
