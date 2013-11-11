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
* Description:  Rename object
*
*/


#ifndef CRENAMEOBJECT_H
#define CRENAMEOBJECT_H

// Forward declarations
class MMTPDataProviderFramework;
class CMmMtpDpMetadataAccessWrapper;
class CMTPObjectMetaData;

class CRenameObject: public CActive
    {
public:
    /**
    * Two phase constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aWrapper Reference to CMmMtpDpMetadataAccessWrapper
    * @return A pointer to a new instance of the object
    */
    IMPORT_C static CRenameObject* NewL( MMTPDataProviderFramework& aFramework,
        CMmMtpDpMetadataAccessWrapper& aWrapper );

    /**
    * destructor
    */
    IMPORT_C virtual ~CRenameObject();

    IMPORT_C void StartL( const TUint32 aParentHandle, 
        const TDesC& aOldFolderName );
    
protected:
    /**
    * Standard C++ Constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aWrapper Reference to CMmMtpDpMetadataAccessWrapper
    */
    IMPORT_C CRenameObject( MMTPDataProviderFramework& aFramework,
        CMmMtpDpMetadataAccessWrapper& aWrapper );

    IMPORT_C void ConstructL();
    
    // added to perform additional action base on need
    virtual void PerformAdditionalActionL();

protected:
    // from CActive
    IMPORT_C void DoCancel();

    IMPORT_C void RunL();

    IMPORT_C TInt RunError( TInt aError );

private:
    
    void GenerateObjectHandleListL( TUint32 aParentHandle );
    
    void GetParentSuidL( TUint32 aHandle, const TDesC& aFolderName );

protected:  
    MMTPDataProviderFramework& iFramework;
    RArray<TUint> iObjectHandles;
    CMTPObjectMetaData* iObjectInfo;
    TInt iCount;
    TInt iIndex;
    RBuf iNewFolderName;
    RBuf iOldFolderFullName;
    RBuf iFileName;
    RBuf iRightPartName;
    RBuf iOldFileName;
    
    // Owned
    /** MMTPDataProviderFramework used to access MTP components*/
    CMmMtpDpMetadataAccessWrapper& iWrapper;
    
private:
    CActiveSchedulerWait* iRenameWaiter;
    TUint32 iParentHandle;

    };

#endif // CRENAMEOBJECT_H
