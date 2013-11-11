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


#include "cmmmtpdataprovidertester.h"
#include "logging.h"

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog

/**
Destructor.
*/
CMmMtpDataProviderTester::~CMmMtpDataProviderTester()
    {
    // no implementation required
    }

/**
Constructor.
@param aParams Parameter block pointer.
@panic USER 0 If the parameter block pointer is 0.
*/
CMmMtpDataProviderTester::CMmMtpDataProviderTester()
    {
    // no implementation required
    }

void CMmMtpDataProviderTester::RegisterObserver( 
        CTestObserver* aTestObserver,
        CTestConnectionChannel* aTestConnChannel,
        CMTPDataProviderPlugin* aDataProviderPlugin )
    {
    PRINTF0( ">CMmMtpDataProviderTester::RegisterObserver" );
    iTestObserver = aTestObserver;
    iTestConnChannel = aTestConnChannel;
    iDataProviderPlugin = aDataProviderPlugin;
    PRINTF0( "<CMmMtpDataProviderTester::RegisterObserver" );
    }

// default implementations

TInt CMmMtpDataProviderTester::CheckSupportedL( TMTPSupportCategory aCategory, RArray<TUint> aResponseArray )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::CheckSupportedL" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TInt CMmMtpDataProviderTester::PrepareEnvironmentL()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::PrepareEnvironmentL" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TInt CMmMtpDataProviderTester::CleanEnvironmentL()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::CleanEnvironmentL" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::OpenSessionL_prepare( TMTPNotificationParamsSessionChange& aParam )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::OpenSessionL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::OpenSessionL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::OpenSessionL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::CloseSessionL_prepare( TMTPNotificationParamsSessionChange& aParam )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::CloseSessionL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::CloseSessionL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::CloseSessionL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::ObjectEnumerationL_prepare( CMTPStorageMetaData* aPhysicalStorage, CMTPStorageMetaData* aLogicalStorage )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::ObjectEnumerationL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::ObjectEnumerationL_checkresults( MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::ObjectEnumerationL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::StorageEnumerationL_prepare()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::StorageEnumerationL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::StorageEnumerationL_checkresults( MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::StorageEnumerationL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::CancelL_prepare()
   {
   PRINTF0( "<>CMmMtpDataProviderTester::CancelL_prepare" );
   PRINTE0( "DUMMY IMPLEMENTATION" );
   return ETrue;
   }

TInt CMmMtpDataProviderTester::CancelL_checkresults()
   {
   PRINTF0( "<>CMmMtpDataProviderTester::CancelL_checkresults" );
   PRINTE0( "DUMMY IMPLEMENTATION" );
   return KErrNone;
   }

TBool CMmMtpDataProviderTester::GetObjectInfoL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectInfoL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectInfoL_checkresults( const CMTPTypeObjectInfo* aObjectInfo )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectInfoL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetObjectL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectL_checkresults( CMTPTypeFile* aObjectInfo )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetObjectPropValueL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropValueL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectPropValueL_checkresults( const MMTPType* aObjectInfo, const TUint aPropCode )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropValueL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetObjectPropListL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropListL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectPropListL_checkresults( const CMTPTypeObjectPropList* aObjectPropList )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropListL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetObjectPropDescL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropDescL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectPropDescL_checkresults( const CMTPTypeObjectPropDesc* aObjectPropDesc, const TUint aPropCode )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropDescL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::SendObjectInfoL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SendObjectInfoL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

void CMmMtpDataProviderTester::SendObjectInfoReceiveData( CMTPTypeObjectInfo* aData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SendObjectInfoReceiveData" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    }

TInt CMmMtpDataProviderTester::SendObjectInfoL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SendObjectInfoL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::SendObjectL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SendObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

void CMmMtpDataProviderTester::SendObjectReceiveDataL( CMTPTypeFile* aData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SendObjectReceiveDataL" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    }

TInt CMmMtpDataProviderTester::SendObjectL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SendObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::SetObjectPropValueL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectPropValueL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

void CMmMtpDataProviderTester::SetObjectPropValueReceiveDataL(
        const TMTPTypeRequest& aRequest, MMTPType* aData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectPropValueReceiveDataL" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    }

TInt CMmMtpDataProviderTester::SetObjectPropValueL_checkresults( const CMTPTypeObjectInfo* aObjectInfo )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectPropValueL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::SetObjectPropListL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectPropListL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

void CMmMtpDataProviderTester::SetObjectPropListReceiveDataL( CMTPTypeObjectPropList* aData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectPropListReceiveDataL" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    }

TInt CMmMtpDataProviderTester::SetObjectPropListL_checkresults( const CMTPTypeObjectPropList* aObjectPropList )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectPropListL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::DeleteObjectL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::DeleteObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::DeleteObjectL_checkresults( MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::DeleteObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::CopyObjectL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::CopyObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::CopyObjectL_checkresults( const CMTPObjectMetaData* aMetaData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::CopyObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;    
    }

TBool CMmMtpDataProviderTester::MoveObjectL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::MoveObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::RenameObjectL_checkresults( const CMTPObjectMetaData* aMetaData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::RenameObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::RenameObjectL_prepare( TMTPNotificationParamsHandle& aParam )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::RenameObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::MoveObjectL_checkresults( const CMTPObjectMetaData* aMetaData )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::MoveObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetPartialObjectL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetPartialObjectL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetPartialObjectL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetPartialObjectL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetObjectReferencesL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectReferencesL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectReferencesL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectReferencesL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::SetObjectReferencesL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectReferencesL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::SetObjectReferencesL_checkresults()
    {
    PRINTF0( "<>CMmMtpDataProviderTester::SetObjectReferencesL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }

TBool CMmMtpDataProviderTester::GetObjectPropsSupportedL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropsSupportedL_prepare" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return ETrue;
    }

TInt CMmMtpDataProviderTester::GetObjectPropsSupportedL_checkresults( const CMTPTypeArray* aObjectProps )
    {
    PRINTF0( "<>CMmMtpDataProviderTester::GetObjectPropsSupportedL_checkresults" );
    PRINTE0( "DUMMY IMPLEMENTATION" );
    return KErrNone;
    }
