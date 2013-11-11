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
* Description:  Move object operation
*
*/


#ifndef CMOVEOBJECT_H
#define CMOVEOBJECT_H

#include "crequestprocessor.h"

class CMTPObjectMetaData;
class CMTPTypeObjectPropList;
class CMTPTypeObjectPropListElement;
class MMmMtpDpConfig;

/**
* Defines MoveObject request processor
*/
class CMoveObject : public CRequestProcessor
    {
public:
    /**
    * Destructor
    */
    IMPORT_C virtual ~CMoveObject();

protected:
    /**
    * Standard c++ constructor
    */
    IMPORT_C CMoveObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Second phase constructor
    */
    IMPORT_C void ConstructL();

protected:
    // from CRequestProcessor
    /**
    * MoveObject request handler
    */
    IMPORT_C void ServiceL();

protected:
    // new virtuals
    /**
    * Set MetaData to CMetadataAccessWrapper, for internal use
    * @param aPropCode, specify property code of an object
    * @param aHandle, object handles of which the properties are needed
    * @param aObject, owner of the properties which should be
    *    inserted or updated into database
    */
    virtual void ServiceGetSpecificObjectPropertyL( TUint16 aPropCode,
        TUint32 aHandle,
        const CMTPObjectMetaData& aObject ) = 0;

private:
    /**
    * Retrieve the parameters of the request
    */
    void GetParametersL();

    /**
    * Get a default parent object, ff the request does not specify a parent object,
    */
    void SetDefaultParentObjectL();

    /**
    * move object operations
    * @return A valid MTP response code.
    */
    void MoveObjectL();

    /**
    * Check if we can move the file to the new location
    */
    TMTPResponseCode CanMoveObjectL( const TDesC& aOldName,
        const TDesC& aNewName ) const;

    /**
    * Save the object properties before moving
    */
    void GetPreviousPropertiesL( const CMTPObjectMetaData& aObject );

    /**
    * Set the object properties after moving
    */
    void SetPreviousPropertiesL();
    /**
    * A helper function of MoveObjectL.
    * @param aNewFileName the new file name after the object is moved.
    */
    void MoveFileL( const TDesC& aNewFileName );

    /**
    * Set the object properties in the object property store.
    */
    void SetPropertiesL( const TDesC& aNewFileName );

    /*
    * Move a single object and update the database
    */
    void MoveAndUpdateL( TUint32 objectHandle );


protected:
    CMTPTypeObjectPropListElement* iPropertyElement;
    MMmMtpDpConfig& iDpConfig;
    CMTPTypeObjectPropList* iPropertyList;
private:
    TBool iSameStorage;
    CMTPObjectMetaData* iObjectInfo; // Not owned
    HBufC* iDest;
    TUint32 iNewParentHandle;
    TUint32 iStorageId;
    TTime iPreviousModifiedTime;

    };

#endif // CMOVEOBJCT_H
