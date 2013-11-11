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
* Description:  Base class for plugin tester
*
*/


#ifndef CMMMTPDATAPROVIDERTESTER_H
#define CMMMTPDATAPROVIDERTESTER_H

#include <e32base.h>
#include <stiflogger.h>
#include <mtp/cmtpdataproviderplugin.h>
#include <mtp/tmtptypeevent.h>
#include <mtp/mtpdataproviderapitypes.h>
#include <mtp/tmtptyperequest.h>
#include <mtp/mtpprotocolconstants.h>
#include "ctestobserver.h"
#include "ctestconnectionchannel.h"

class CMTPStorageMetaData;
class MMTPObjectMgr;
class CMTPTypeObjectInfo;
class CMTPTypeObjectPropList;
class CMTPTypeObjectPropDesc;
class CMTPTypeFile;
class CMTPTypeString;

/** 
Defines the MTP data provider ECOM plug-in service provider 
interface.
@publishedPartner
@released
*/
class CMmMtpDataProviderTester : public CBase
    {
public:
    virtual ~CMmMtpDataProviderTester();
    
    void RegisterObserver( CTestObserver* aTestObserver,
            CTestConnectionChannel* aTestConnChannel,
            CMTPDataProviderPlugin* aDataProviderPlugin );
public:
    TInt CheckSupportedL( TMTPSupportCategory aCategory,
            RArray<TUint> aResponseArray );
    
    virtual TInt PrepareEnvironmentL();
    virtual TInt CleanEnvironmentL();
    
    virtual TBool OpenSessionL_prepare(
            TMTPNotificationParamsSessionChange& param );
    virtual TInt OpenSessionL_checkresults();
    
    virtual TBool CloseSessionL_prepare(
            TMTPNotificationParamsSessionChange& param );
    virtual TInt CloseSessionL_checkresults();
    
    virtual TBool ObjectEnumerationL_prepare( CMTPStorageMetaData*
            aPhysicalStorage, CMTPStorageMetaData* aLogicalStorage );
    virtual TInt ObjectEnumerationL_checkresults( MMTPObjectMgr&
            aObjectManager );
    
    virtual TBool StorageEnumerationL_prepare();
    virtual TInt StorageEnumerationL_checkresults( MMTPObjectMgr&
            aObjectManager );
    
    virtual TBool CancelL_prepare();
    virtual TInt CancelL_checkresults();
    
    virtual TBool GetObjectInfoL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetObjectInfoL_checkresults( const CMTPTypeObjectInfo*
            aObjectInfo );
    
    virtual TBool GetObjectL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetObjectL_checkresults( CMTPTypeFile* aObjectInfo );
    
    virtual TBool GetObjectPropValueL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetObjectPropValueL_checkresults(
            const MMTPType* aObjectPropValue, const TUint aPropCode );
    
    virtual TBool GetObjectPropListL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetObjectPropListL_checkresults(
            const CMTPTypeObjectPropList* aObjectPropList );
    
    virtual TBool GetObjectPropDescL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetObjectPropDescL_checkresults(
            const CMTPTypeObjectPropDesc* aObjectPropDesc,
            const TUint aPropCode );
    
    virtual TBool SendObjectInfoL_prepare( TMTPTypeRequest& aRequest );
    virtual void SendObjectInfoReceiveData( CMTPTypeObjectInfo* aData );
    virtual TInt SendObjectInfoL_checkresults();
    
    virtual TBool SendObjectL_prepare( TMTPTypeRequest& aRequest );
    virtual void SendObjectReceiveDataL( CMTPTypeFile* aData );
    virtual TInt SendObjectL_checkresults();
    
    virtual TBool SetObjectPropValueL_prepare( TMTPTypeRequest& aRequest );
    virtual void SetObjectPropValueReceiveDataL(
            const TMTPTypeRequest& aRequest, MMTPType* aData );
    virtual TInt SetObjectPropValueL_checkresults( const CMTPTypeObjectInfo*
            aObjectInfo );
    
    virtual TBool SetObjectPropListL_prepare( TMTPTypeRequest& aRequest );
    virtual void SetObjectPropListReceiveDataL( CMTPTypeObjectPropList*
            aData );
    virtual TInt SetObjectPropListL_checkresults(
            const CMTPTypeObjectPropList* aObjectPropList );
    
    virtual TBool DeleteObjectL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt DeleteObjectL_checkresults( MMTPObjectMgr& aObjectManager );
    
    virtual TBool CopyObjectL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt CopyObjectL_checkresults( const CMTPObjectMetaData*
            aMetaData );
    
    virtual TBool MoveObjectL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt MoveObjectL_checkresults( const CMTPObjectMetaData*
            aMetaData );
    
    virtual TBool RenameObjectL_prepare( TMTPNotificationParamsHandle&
            aParam );
    virtual TInt RenameObjectL_checkresults( const CMTPObjectMetaData*
            aMetaData );
    
    virtual TBool GetPartialObjectL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetPartialObjectL_checkresults();
    
    virtual TBool SetObjectReferencesL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt SetObjectReferencesL_checkresults();
    
    virtual TBool GetObjectReferencesL_prepare( TMTPTypeRequest& aRequest );
    virtual TInt GetObjectReferencesL_checkresults();
    
    virtual TBool GetObjectPropsSupportedL_prepare( TMTPTypeRequest&
            aRequest );
    virtual TInt GetObjectPropsSupportedL_checkresults( const CMTPTypeArray*
            aObjectProps );
protected:
    CMmMtpDataProviderTester();
protected:
    CStifLogger* iLog;
    // not owned:
    CTestObserver* iTestObserver;
    CTestConnectionChannel* iTestConnChannel;
    CMTPDataProviderPlugin* iDataProviderPlugin;
    };

#endif // CMMMTPDATAPROVIDERTESTER_H
