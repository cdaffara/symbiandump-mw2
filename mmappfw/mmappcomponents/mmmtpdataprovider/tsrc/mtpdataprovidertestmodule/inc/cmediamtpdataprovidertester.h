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
* Description:  Tester class for mediamtpdp plugin
*
*/


#ifndef CMEDIAMTPDATAPROVIDERTESTER_H
#define CMEDIAMTPDATAPROVIDERTESTER_H

#include "cmmmtpdataprovidertester.h"

/**
*  CMediaMtpDataProviderTester class.
*  
*/
class CMediaMtpDataProviderTester : public CMmMtpDataProviderTester
    {
public:
    static CMediaMtpDataProviderTester* NewL();
    virtual ~CMediaMtpDataProviderTester();
private:
    TInt CheckSupportedL( TMTPSupportCategory aCategory,
            RArray<TUint> aResponseArray );
    
    TInt PrepareEnvironmentL();
    TInt CleanEnvironmentL();
    
    TBool ObjectEnumerationL_prepare( CMTPStorageMetaData* aPhysicalStorage,
            CMTPStorageMetaData* aLogicalStorage );
    TInt ObjectEnumerationL_checkresults( MMTPObjectMgr& aObjectManager );
    
    TInt GetObjectInfoL_checkresults( const CMTPTypeObjectInfo* aObjectInfo );
   
    TInt GetObjectL_checkresults( CMTPTypeFile*  aObjectInfo );
    
    TInt GetObjectPropValueL_checkresults( const MMTPType* aObjectPropValue,
            const TUint aPropCode );
    
    TInt GetObjectPropListL_checkresults( const CMTPTypeObjectPropList*
            aObjectPropList );
    
    TInt GetObjectPropDescL_checkresults( const CMTPTypeObjectPropDesc*
            aObjectPropDesc, const TUint aPropCode );
    
    TBool SendObjectInfoL_prepare( TMTPTypeRequest& aRequest );
    void SendObjectInfoReceiveData( CMTPTypeObjectInfo* aData );
    
    TBool SendObjectL_prepare( TMTPTypeRequest& aRequest );
    void SendObjectReceiveDataL( CMTPTypeFile* aData );
    TInt SendObjectL_checkresults();
    
    void SetObjectPropValueReceiveDataL( const TMTPTypeRequest& aRequest,
    		MMTPType* aData );
    
    void SetObjectPropListReceiveDataL( CMTPTypeObjectPropList* aData );
    
    TInt DeleteObjectL_checkresults( MMTPObjectMgr& aObjectManager );
    
    TBool CopyObjectL_prepare( TMTPTypeRequest& aRequest );
    TInt CopyObjectL_checkresults( const CMTPObjectMetaData* aMetaData );
    
    TBool MoveObjectL_prepare( TMTPTypeRequest& aRequest );
    TInt MoveObjectL_checkresults( const CMTPObjectMetaData* aMetaData );
    
    TBool RenameObjectL_prepare( TMTPNotificationParamsHandle& aParam );
    TInt RenameObjectL_checkresults( const CMTPObjectMetaData* aMetaData );
    
    TInt GetPartialObjectL_checkresults ( CMTPTypeFile* aObjectInfo );
    
    TBool GetObjectPropsSupportedL_prepare( TMTPTypeRequest& aRequest );
    TInt GetObjectPropsSupportedL_checkresults( const CMTPTypeArray*
            aObjectProps );
    
    TBool ProcessRequestIllegalIdL_prepare( TInt aOpCode );
    TInt ProcessRequestIllegalIdL_checkresults( TInt aOpCode,
            TInt aResponseCode );
private:
    CMediaMtpDataProviderTester();
    void ConstructL();
private:
    CActiveSchedulerWait* iActiveSchedulerWait;
    RFs iFsSession;
    CFileMan* iFileMan;
    };

#endif // CMEDIAMTPDATAPROVIDERTESTER_H
