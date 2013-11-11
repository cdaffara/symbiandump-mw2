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
* Description:  Dummy object manager for test module
*
*/


#include <e32def.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/cmtpobjectmetadata.h>
#include <e32debug.h>

#include "ctestobjectmanager.h"
#include "logging.h"

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog

CTestObjectManager* CTestObjectManager::NewL()
    {
    CTestObjectManager* self = new ( ELeave ) CTestObjectManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CTestObjectManager::CTestObjectManager()
    {
    // implementation not required
    }

void CTestObjectManager::ConstructL()
    {
    TFileName logFileName;
    logFileName.Copy( KMtpDataProviderTestModuleLogFile );
    
    iLog = CStifLogger::NewL( KMtpDataProviderTestModuleLogPath, 
            logFileName,
            CStifLogger::ETxt,
            TRACE_TARGET,
            ETrue, ETrue, ETrue, EFalse, ETrue );
    PRINTF0( "CTestObjectManager::ConstructL" );
    }

TUint CTestObjectManager::CountL( const TMTPObjectMgrQueryParams& aParams ) const
    {
    PRINTF3( ">CTestObjectManager::CountL storage = 0x%x parent = 0x%x format = 0x%x", aParams.iStorageId, aParams.iParentHandle, aParams.iFormatCode );
    TUint count = 0;
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
         {
         TUint handle = iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle );
         TUint formatCode = iMTPObjects[i]->Uint( CMTPObjectMetaData::EFormatCode );
         TUint storageId = iMTPObjects[i]->Uint( CMTPObjectMetaData::EStorageId );
         TInt parentId = iMTPObjects[i]->Int( CMTPObjectMetaData::EParentId );
         if ( ( ( aParams.iStorageId == storageId   ) || ( aParams.iStorageId == KMTPStorageAll ) )
            &&
              ( ( aParams.iFormatCode == formatCode ) || ( aParams.iFormatCode == KMTPFormatsAll ) )
            &&
              ( ( aParams.iParentHandle == parentId ) || ( aParams.iParentHandle == KMTPHandleNone ) || ( ( aParams.iParentHandle == KMTPHandleNoParent ) && ( parentId == KErrNotFound ) ) ) )
             {
             PRINTV1( "Adding handle %d to count", handle );
             count++;
             }
         }
    PRINTF1( "<CTestObjectManager::CountL, count = %d", count );
    return count;
    }

void CTestObjectManager::GetObjectHandlesL(
        const TMTPObjectMgrQueryParams& aParams,
        RMTPObjectMgrQueryContext& aContext, RArray<TUint>& aHandles ) const
    {
    CleanupClosePushL( aHandles ); 
    PRINTF3( ">CTestObjectManager::GetObjectHandlesL storage = 0x%x parent = 0x%x format = 0x%x", aParams.iStorageId, aParams.iParentHandle, aParams.iFormatCode );
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
         {
         TUint handle = iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle );
         TUint formatCode = iMTPObjects[i]->Uint( CMTPObjectMetaData::EFormatCode );
         TUint storageId = iMTPObjects[i]->Uint( CMTPObjectMetaData::EStorageId );
         TInt parentId = iMTPObjects[i]->Int( CMTPObjectMetaData::EParentId );
         if ( ( ( aParams.iStorageId == storageId   ) || ( aParams.iStorageId == KMTPStorageAll ) )
            &&
              ( ( aParams.iFormatCode == formatCode ) || ( aParams.iFormatCode == KMTPFormatsAll ) )
            &&
            ( ( aParams.iParentHandle == parentId ) || ( aParams.iParentHandle == KMTPHandleNone ) || ( ( aParams.iParentHandle == KMTPHandleNoParent ) && ( parentId == KErrNotFound ) ) ) )
             {
             PRINTV1( "Appending handle %d", handle );
             aHandles.AppendL( handle );
             }
         }
    aContext.Close();
    PRINTF0( "<CTestObjectManager::GetObjectHandlesL" );
    CleanupStack::Pop(); 
    }
      
void CTestObjectManager::GetObjectSuidsL( 
        const TMTPObjectMgrQueryParams& aParams,
        RMTPObjectMgrQueryContext& aContext, CDesCArray& aSuids ) const
    {
    PRINTF3( ">CTestObjectManager::GetObjectSuidsL storage = 0x%x parent = 0x%x format = 0x%x", aParams.iStorageId, aParams.iParentHandle, aParams.iFormatCode );
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
         {
         TPtrC suid = iMTPObjects[i]->DesC( CMTPObjectMetaData::ESuid );
         TUint formatCode = iMTPObjects[i]->Uint( CMTPObjectMetaData::EFormatCode );
         TUint storageId = iMTPObjects[i]->Uint( CMTPObjectMetaData::EStorageId );
         TInt parentId = iMTPObjects[i]->Int( CMTPObjectMetaData::EParentId );
         if ( ( ( aParams.iStorageId == storageId   ) || ( aParams.iStorageId == KMTPStorageAll ) )
            &&
              ( ( aParams.iFormatCode == formatCode ) || ( aParams.iFormatCode == KMTPFormatsAll ) )
            &&
            ( ( aParams.iParentHandle == parentId ) || ( aParams.iParentHandle == KMTPHandleNone ) || ( ( aParams.iParentHandle == KMTPHandleNoParent ) && ( parentId == KErrNotFound ) ) ) )
             {
             PRINTV1( "Appending suid %S", &suid );
             aSuids.AppendL( suid );
             }
         }
    aContext.Close();
    PRINTF0( "<CTestObjectManager::GetObjectSuidsL" );
    }
      
TUint32 CTestObjectManager::HandleL( TInt64 aObjectUid ) const
    {
    PRINTF1( "<>CTestObjectManager::HandleL uid = %Ld", aObjectUid );
    PRINTE0( "NOT IMPLEMENTED" );
    return NULL;
    }
      
 
TUint32 CTestObjectManager::HandleL( const TMTPTypeUint128& aPuid ) const
    {
    TUint64 lower = aPuid.LowerValue();
    TUint64 higher = aPuid.UpperValue();
    PRINTF2( "<>CTestObjectManager::HandleL puid = %Lu %Lu", lower, higher );
    PRINTE0( "NOT IMPLEMENTED" );
    return NULL;    
    }
      
 
TUint32 CTestObjectManager::HandleL( const TDesC& aSuid ) const
    {
    PRINTF1( ">CTestObjectManager::HandleL suid = %S", &aSuid );
    TUint handle = 0;
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
         {
         if ( ( iMTPObjects[i]->DesC( CMTPObjectMetaData::ESuid ) ).Match( aSuid ) == 0 )
              {
              handle = iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle );
              break;
              }
         }
    PRINTF1( "<CTestObjectManager::HandleL with handle %d", handle );
    return handle;
    }

void CTestObjectManager::InsertObjectL( CMTPObjectMetaData& aObject )
    {
    TUint handle = aObject.Uint( CMTPObjectMetaData::EHandle );
    PRINTF1( ">CTestObjectManager::InsertObjectL handle = %d", handle );
    
    CMTPObjectMetaData* newObject = CMTPObjectMetaData::NewL(
            aObject.DesC( CMTPObjectMetaData::ESuid ), 0, 0, 0 );
    
    CleanupStack::PushL( newObject );
    iHandle++;
    aObject.SetUint( CMTPObjectMetaData::EHandle, iHandle );
    newObject->SetUint( CMTPObjectMetaData::EHandle, iHandle );
    CopyObjectMetadataL( aObject, *newObject );
    CleanupStack::Pop( newObject );
    
    iMTPObjects.Append( newObject );
    PRINTF1( "New object inserted with handle %d", iHandle );
    }

void CTestObjectManager::InsertObjectsL( RPointerArray<CMTPObjectMetaData>& /*aObjects*/ )
    {
    PRINTF0( "<>CTestObjectManager::InsertObjectsL" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObjectManager::ModifyObjectL( const CMTPObjectMetaData& aObject )
    {
    TUint32 handle = aObject.Uint( CMTPObjectMetaData::EHandle );
    PRINTF1( ">CTestObjectManager::ModifyObjectL handle = %d", handle );
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
        {
        if ( iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle ) == handle )
            {
            PRINTF1( "<CTestObjectManager::ModifyObjectL, index = %d", i );
            CopyObjectMetadataL( aObject, *iMTPObjects[i] );
            return;
            }
        }
    PRINTF0( "<CTestObjectManager::ModifyObjectL, object not found" );
    User::Leave( KErrNotFound );
    }

TBool CTestObjectManager::ObjectL( const TMTPTypeUint32& aHandle, CMTPObjectMetaData& aObject ) const
    {
    TUint32 handle = aHandle.Value();
    PRINTF1( ">CTestObjectManager::ObjectL handle = %d", handle );
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
        {
        if ( iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle ) == handle )
            {
            PRINTF1( "<CTestObjectManager::ObjectL, index = %d", i );
            CopyObjectMetadataL( *iMTPObjects[i], aObject );
            return ETrue;
            }
        }
    PRINTF0( "<CTestObjectManager::ObjectL, object not found" );
    return EFalse;
    }

TBool CTestObjectManager::ObjectL( const TDesC& aSuid, CMTPObjectMetaData& aObject ) const
    {
    PRINTF1( ">CTestObjectManager::ObjectL suid = %S", &aSuid );
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
        {
        if ( ( iMTPObjects[i]->DesC(CMTPObjectMetaData::ESuid ) ).Match( aSuid ) == 0)
            {
            PRINTF1( "<CTestObjectManager::ObjectL, index = %d", i );
            CopyObjectMetadataL( *( iMTPObjects[i] ), aObject );
            return ETrue;
            }
        }
    PRINTF0( "<CTestObjectManager::ObjectL, object not found" );
    return EFalse;
    }
      
TUint CTestObjectManager::ObjectOwnerId( const TMTPTypeUint32& aHandle ) const
    {
    TUint32 handle = aHandle.Value();
    PRINTF1( ">CTestObjectManager::ObjectOwnerId handle = %d", handle );
    TUint ownerId = KErrNotFound;
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
        {
        if ( handle == iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle ) )
            {
            ownerId = iMTPObjects[i]->Uint( CMTPObjectMetaData::EDataProviderId );
            break;
            }
        }
    if ( ownerId == KErrNotFound )
        {
        PRINTE1( "<CTestObjectManager::ObjectOwnerId, object not found: %d", handle );
        User::Leave( KErrNotFound );
        }
    PRINTF1( "<CTestObjectManager::ObjectOwnerId, owner id = %d", ownerId );
    return ownerId;
    }

TInt64 CTestObjectManager::ObjectUidL( TUint32 aHandle ) const
    {
    PRINTF1( "<>CTestObjectManager::ObjectUidL handle = %d", aHandle );
    return aHandle;  
    }
  
TInt64 CTestObjectManager::ObjectUidL( const TDesC& aSuid ) const
    {
    PRINTF1( ">CTestObjectManager::ObjectUidL suid = %S", &aSuid );
    TUint handle = 0;
    for ( TInt i = 0; i < iMTPObjects.Count(); i++ )
        {
        if ( ( iMTPObjects[i]->DesC( CMTPObjectMetaData::ESuid ) ).Match( aSuid ) == 0 )
            {
            handle = iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle );
            break;
            }
        }
    PRINTF1( "<CTestObjectManager::ObjectUidL, handle = %d", handle );
    return handle;
    }
 
TInt64 CTestObjectManager::ObjectUidL( TMTPTypeUint128 aPuid ) const
    {
    TUint64 lower = aPuid.LowerValue();
    TUint64 higher = aPuid.UpperValue();
    PRINTF2( "<>CTestObjectManager::ObjectUidL puid = %Lu %Lu", lower, higher );
    PRINTE0( "NOT IMPLEMENTED" );
    return 0;  
    }
  
TMTPTypeUint128 CTestObjectManager::PuidL( TUint32 aHandle ) const
    {
    PRINTF1( ">CTestObjectManager::PuidL handle = %d", aHandle );
    TMTPTypeUint128 returnValue;
    for ( TInt i = 0; i < iMTPObjects.Count(); i++)
         {
         TUint handle = iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle );
         if ( handle == aHandle )
              {
              returnValue.Set( handle, handle );
              break;   
              }
         }
    return returnValue;
    }
      
TMTPTypeUint128 CTestObjectManager::PuidL( TInt64 aObjectUid ) const
    {
    PRINTF1( "<>CTestObjectManager::PuidL uid = %Ld", aObjectUid );
    TMTPTypeUint128 returnValue;
    PRINTE0( "NOT IMPLEMENTED" );
    return returnValue;
    }

TMTPTypeUint128 CTestObjectManager::PuidL( const TDesC& aSuid ) const
    {
    PRINTF1( "<>CTestObjectManager::PuidL suid = %S", &aSuid );
    TMTPTypeUint128 returnValue;
    PRINTE0( "NOT IMPLEMENTED" );
    return returnValue;
    }

void CTestObjectManager::RemoveObjectL( const TMTPTypeUint32& aHandle )
    {
    TUint handle = aHandle.Value();
    PRINTF1( "<>CTestObjectManager::RemoveObjectL handle = %d", handle );
    PRINTE0( "NOT IMPLEMENTED" );
    }
      
void CTestObjectManager::RemoveObjectL( const TDesC& aSuid )
    {
    PRINTF1( "<>CTestObjectManager::RemoveObjectL suid = %S", &aSuid );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObjectManager::RemoveObjectsL( const CDesCArray& /*aSuids*/ )
    {
    PRINTF0( "<>CTestObjectManager::RemoveObjectsL suid arr" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObjectManager::RemoveObjectsL( TUint aDataProviderId )
    {
    PRINTF1( "<>CTestObjectManager::RemoveObjectsL data provider id = %d", aDataProviderId );
    PRINTE0( "NOT IMPLEMENTED" );
    }
   
void CTestObjectManager::ReserveObjectHandleL( CMTPObjectMetaData& aObject, TUint64 aSpaceRequired )
    {
    TUint handle = aObject.Uint( CMTPObjectMetaData::EHandle );
    PRINTF2( ">CTestObjectManager::ReserveObjectHandleL handle = %d, space = %Ld", handle, aSpaceRequired );
    
    CMTPObjectMetaData* newObject = CMTPObjectMetaData::NewL(
            aObject.DesC( CMTPObjectMetaData::ESuid ), 0, 0, 0 );
    
    CleanupStack::PushL( newObject );
    iHandle++;
    aObject.SetUint( CMTPObjectMetaData::EHandle, iHandle );
    newObject->SetUint( CMTPObjectMetaData::EHandle, iHandle );
    CopyObjectMetadataL( aObject, *newObject );
    CleanupStack::Pop( newObject );
    
    iMTPObjects.Append( newObject );
    PRINTF1( "<CTestObjectManager::ReserveObjectHandleL handle = %d", iHandle );
    }

void CTestObjectManager::CommitReservedObjectHandleL( CMTPObjectMetaData& aObject )
    {
    TUint handle = aObject.Uint( CMTPObjectMetaData::EHandle );
    PRINTF1( "<>CTestObjectManager::CommitReservedObjectHandleL handle = %d", handle );
    }

void CTestObjectManager::UnreserveObjectHandleL( const CMTPObjectMetaData& aObject )
    {
    TUint reservedHandle = aObject.Uint( CMTPObjectMetaData::EHandle );
    PRINTF1( ">CTestObjectManager::UnreserveObjectHandleL handle = %d", reservedHandle );
    
    for ( TInt i = 0; i < iMTPObjects.Count(); i++)
         {
         TUint handle = iMTPObjects[i]->Uint( CMTPObjectMetaData::EHandle );
         if ( reservedHandle == handle )
              {
              CMTPObjectMetaData* object = iMTPObjects[i];
              iMTPObjects.Remove( i );
              delete object;
              break;
              }
         }
    PRINTF0( "<CTestObjectManager::UnreserveObjectHandleL" );
    }

CTestObjectManager::~CTestObjectManager()
    {
    PRINTF0( "<>CTestObjectManager::~CTestObjectManager()" );
    delete iLog;
    iMTPObjects.ResetAndDestroy();
    }

void CTestObjectManager::CopyObjectMetadataL( 
        const CMTPObjectMetaData& aSource, CMTPObjectMetaData& aTarget ) const
    {
    PRINTF0( ">CTestObjectManager::CopyObjectMetadataL" );
    for ( TInt i = CMTPObjectMetaData::EDataProviderId;
            i <= CMTPObjectMetaData::EStorageVolumeId; i++ )
        {
        aTarget.SetUint( i, aSource.Uint( i ) );
        }
    
    aTarget.SetDesCL( CMTPObjectMetaData::ESuid, 
            aSource.DesC( CMTPObjectMetaData::ESuid ) );
        
    for ( TInt i = CMTPObjectMetaData::EUserRating;
            i <= CMTPObjectMetaData::EIdentifier; i++ )
        {
        aTarget.SetUint( i, aSource.Uint( i ) );
        }
   
    aTarget.SetInt( CMTPObjectMetaData::EParentId,
            aSource.Int( CMTPObjectMetaData::EParentId ) );
    PRINTF0( "<CTestObjectManager::CopyObjectMetadataL" );
    }
