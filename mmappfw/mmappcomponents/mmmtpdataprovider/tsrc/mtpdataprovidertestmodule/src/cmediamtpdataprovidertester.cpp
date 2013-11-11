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


#include <e32def.h>
#include <stiftestinterface.h>
#include <settingserverclient.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/cmtptypeobjectinfo.h>
#include <mtp/cmtptypefile.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypeobjectpropdesc.h>
#include <bautils.h>
#include <f32file.h>
#include <mpxharvesterutility.h>
#include "cmediamtpdataprovidertester.h"
#include "logging.h"

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog

// definitions and constants for the test data

const TUint32 KTestStorageId = 0x10000;
const TInt KTestObjectsCount = 4;
const TUint KTestDirHandle = 1;
const TInt KTestDirDataproviderId = 0;
_LIT( KTempDirPath, "C:\\mtp_testdata\\temp\\" );
_LIT( KTestDirPath, "C:\\mtp_testdata\\temp\\TESTDIR\\" );
_LIT( KTestFileOrigPath, "C:\\mtp_testdata\\TEST.MP3" );
_LIT( KTestFileTempPath, "C:\\mtp_testdata\\temp\\TEST.MP3" );
_LIT( KTestDirOrigPath, "C:\\mtp_testdata\\TESTDIR\\*" );
_LIT( KTestDirTempPath, "C:\\mtp_testdata\\temp\\TESTDIR\\" );
_LIT( KTestDirPhysicalStorage, "C:\\mtp_testdata\\temp\\" );
_LIT( KTestDirLogicalStorage, "C:\\mtp_testdata\\temp\\" );
_LIT( KTestObjectPath1, "C:\\mtp_testdata\\temp\\TEST.MP3" );
_LIT( KTestObjectPath2, "C:\\mtp_testdata\\temp\\TESTDIR\\TEST1.MP3" );
_LIT( KTestObjectPath3, "C:\\mtp_testdata\\temp\\TESTDIR\\TEST2.MP3" );

_LIT( KSendTestObjectFile, "SENDTEST.MP3" );
_LIT( KSendTestObjectFileFullPath, "C:\\mtp_testdata\\SENDTEST.MP3" );
_LIT( KSendTestObjectPath, "C:\\mtp_testdata\\temp\\SENDTEST.MP3" );
_LIT( KSendTestObjectDateString, "20090511T091022+0800" );
_LIT( KCopyTestObjectPath, "C:\\mtp_testdata\\temp\\TEST1.MP3" );
_LIT( KCopyTestFileName, "TEST1.MP3" );
_LIT( KMoveTestObjectOldPath, "C:\\mtp_testdata\\temp\\TESTDIR\\TEST1.MP3" );
_LIT( KMoveTestObjectNewPath, "C:\\mtp_testdata\\temp\\TEST1.MP3" );
_LIT( KMoveTestFileName, "TEST1.MP3" );
_LIT( KRenameObjectName, "TESTDIR" );
_LIT( KRenameObjectPath, "C:\\mtp_testdata\\temp\\RENAMEDIR\\" );

//_LIT( KSendObjectNameString, "Test Object" );

_LIT( KPropObjectDate, "20090610T090000+0800" );
_LIT( KPropObjectName, "Test Object" );
_LIT( KPropObjectArtist, "Test Artist" );
_LIT( KPropObjectAlbumName, "Test Album" );

const TInt KTestFileSize = 271890;
const TInt KSendTestFileSize = 271890;
const TInt KCopyTestFileSize = 271890;
const TInt KMoveTestFileSize = 271890;

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMediaMtpDataProviderTester* CMediaMtpDataProviderTester::NewL()
    {
    CMediaMtpDataProviderTester* self = 
            new ( ELeave ) CMediaMtpDataProviderTester();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CMediaMtpDataProviderTester::CMediaMtpDataProviderTester()
        : CMmMtpDataProviderTester()
    {
    // no implementation required
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMediaMtpDataProviderTester::ConstructL()
    {
    TFileName logFileName;
    logFileName.Copy( KMtpDataProviderTestModuleLogFile );
    
    iLog = CStifLogger::NewL( KMtpDataProviderTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          TRACE_TARGET,
                          ETrue, ETrue, ETrue, EFalse, ETrue );
    PRINTF0( ">CMediaMtpDataProviderTester::ConstructL" );
    iActiveSchedulerWait = new ( ELeave ) CActiveSchedulerWait();
    iFsSession.Connect();
    iFileMan = CFileMan::NewL( iFsSession );
    PRINTF0( "<CMediaMtpDataProviderTester::ConstructL" );
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//  
CMediaMtpDataProviderTester::~CMediaMtpDataProviderTester()
    {
    PRINTF0( ">CMediaMtpDataProviderTester::~CMediaMtpDataProviderTester" );
    delete iActiveSchedulerWait;
    delete iFileMan;
    iFsSession.Close();
    PRINTF0( "<CMediaMtpDataProviderTester::~CMediaMtpDataProviderTester" );
    delete iLog;
    }

TInt CMediaMtpDataProviderTester::PrepareEnvironmentL()
    {
    PRINTF0( ">CMediaMtpDataProviderTester::PrepareEnvironmentL" );
    TInt result = KErrNone;
    
    result = iFileMan->RmDir( KTempDirPath );
    if ( result == KErrNone )
        {
        PRINTN0( "Warning: old temp directory found, deleted" );
        }
    result = KErrNone;
    
    // create the temp dir if it doesn't exist
    BaflUtils::EnsurePathExistsL( iFsSession, KTempDirPath );
    
    // copy the test data to temp dir
    result = iFileMan->Copy( KTestFileOrigPath, KTestFileTempPath );
    if ( result != KErrNone ) 
        {
        PRINTE1( "Failed to copy test file, error %d", result );
        }
    else
        {
        result = iFileMan->Copy( KTestDirOrigPath, KTestDirTempPath,
                CFileMan::EOverWrite | CFileMan::ERecurse );
        if ( result != KErrNone ) 
            {
            PRINTE1( "Failed to copy test dir, error %d", result );
            }
        }
    
    PRINTF1( "<CMediaMtpDataProviderTester::PrepareEnvironmentL result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::CleanEnvironmentL()
    {
    PRINTF0( ">CMediaMtpDataProviderTester::CleanEnvironmentL" );
    TInt result = KErrNone;
    TMTPObjectMgrQueryParams queryParams ( KMTPStorageAll, KMTPFormatsAll,
            KMTPHandleNone );
    RMTPObjectMgrQueryContext context;
    RArray<TUint> handles;
    CleanupClosePushL( handles );

    PRINTN0( "Starting to delete entries from mpx" );
    MMPXHarvesterUtility* harvester = CMPXHarvesterFactory::NewL();
    CleanupClosePushL( *harvester );
    
    iTestObserver->ObjectMgr().GetObjectHandlesL( queryParams, context,
            handles );
    PRINTV1( "Handle count: %d", handles.Count() );
    context.Close();
    
    CMTPObjectMetaData* metadata = CMTPObjectMetaData::NewLC();
    for ( TInt i = 0; i < handles.Count(); i++ )
        {
        PRINTN0( "enter for loop" );
        TUint handle = handles[i];
            if ( handle != KTestDirHandle )
                {
                iTestObserver->ObjectMgr().ObjectL( handles[i], *metadata );
                TPtrC fileName = metadata->DesC( CMTPObjectMetaData::ESuid );
                PRINTV1( "Handle: %d", handles[i] );
                PRINTV1( "File name: %S", &fileName );
                    
                PRINTN0( "Starting to delete the file from mpx" );
                iTestObserver->SetPendingOperation( CTestObserver::EHarvesterDeleteObject );
                PRINTN0( "before deletefilel" );
                harvester->DeleteFileL( fileName, iTestObserver );
                PRINTN0( "after deletefilel" );
                User::After( 1000000 );
                PRINTN0( "before waitforpendingoperation" );
                // waiting for asynchronous operation to complete
                result = iTestObserver->WaitForPendingOperation();
                PRINTN0( "after waitforpendingoperation" );
                    
            if ( result != KErrNone )
                {
                PRINTE1( "Warning: failed to delete from mpx, error %d", result );
                result = KErrNone; // ignore for now, there are problems after DeleteObject
                }
            }
        }
    CleanupStack::PopAndDestroy( 3 ); // metadata, harvester, handles
    PRINTN0( "Finished deleting entries from mpx" );
    
    if ( result == KErrNone )
        {
        PRINTN0( "Deleting temp directory" );
        result = iFileMan->RmDir( KTempDirPath );
        if ( result != KErrNone )
            {
            PRINTE1( "Warning: failed to delete temp directory, error %d", result );
            result = KErrNone;
            }
        }
    
    PRINTF1( "<CMediaMtpDataProviderTester::CleanEnvironmentL result = %d", result );
    return result;
    }

TBool CMediaMtpDataProviderTester::ObjectEnumerationL_prepare(
        CMTPStorageMetaData* aPhysicalStorage,
        CMTPStorageMetaData* aLogicalStorage )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::ObjectEnumerationL_prepare" );
    
    TBool result = ETrue;
    aPhysicalStorage->SetDesCL( CMTPStorageMetaData::EStorageSuid,
            KTestDirPhysicalStorage() );
    aPhysicalStorage->SetUint( CMTPStorageMetaData::EStorageSystemType,
            CMTPStorageMetaData::ESystemTypeDefaultFileSystem );
    
    aLogicalStorage->SetDesCL( CMTPStorageMetaData::EStorageSuid,
            KTestDirLogicalStorage() );
    aLogicalStorage->SetUint( CMTPStorageMetaData::EStorageSystemType,
            CMTPStorageMetaData::ESystemTypeDefaultFileSystem );
    
    // insert TESTDIR object manually
    CMTPObjectMetaData* object( CMTPObjectMetaData::NewLC( 0,
            EMTPFormatCodeAssociation, // a folder object
            KTestStorageId, KTestDirPath ) );
    // object is in storage root
    object->SetInt( CMTPObjectMetaData::EParentId, KErrNotFound );
    
    iTestObserver->ObjectMgr().InsertObjectL( *object );
    CleanupStack::PopAndDestroy( object );
    
    PRINTF1( "<CMediaMtpDataProviderTester::ObjectEnumerationL_prepare result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::ObjectEnumerationL_checkresults(
        MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::ObjectEnumerationL_checkresults" );
    
    TInt result = KErrNone;
    
    TMTPObjectMgrQueryParams queryParams ( KMTPStorageAll, KMTPFormatsAll,
            KMTPHandleNone );
    RMTPObjectMgrQueryContext context;
    RArray<TUint> handles;
    CleanupClosePushL( handles );
    MMPXHarvesterUtility* harvester = CMPXHarvesterFactory::NewL();
    CleanupClosePushL( *harvester );
    
    aObjectManager.GetObjectHandlesL( queryParams, context, handles );
    context.Close();
    TInt count = handles.Count();
    PRINTV1( "Handle count: %d", count );
    if ( count != KTestObjectsCount ) 
        {
        PRINTE2( "Wrong number of handles: %d != %d", count, KTestObjectsCount );
        result = KErrGeneral;
        }
    
    CMTPObjectMetaData* metadata = CMTPObjectMetaData::NewLC();
    for ( TInt i = 0; i < count; i++ )
        {
        TUint handle = handles[i];
        aObjectManager.ObjectL( handle, *metadata );
        TPtrC suid = metadata->DesC( CMTPObjectMetaData::ESuid );
        TUint formatCode = metadata->Uint( CMTPObjectMetaData::EFormatCode );
        TUint dataproviderId = metadata->Uint( CMTPObjectMetaData::EDataProviderId );
        TUint storageId = metadata->Uint( CMTPObjectMetaData::EStorageId );
        TInt parentId = metadata->Int( CMTPObjectMetaData::EParentId );
        
        PRINTN1( "Handle: %d", handle );
        PRINTN1( "Suid: %S", &suid );
        
        if ( dataproviderId != iTestObserver->DataProviderId() )
            {
            if ( !( ( handle == KTestDirHandle ) &&
                    ( dataproviderId == KTestDirDataproviderId ) ) )
                {
                PRINTE2( "Wrong dataprovider id: %d != %d", dataproviderId, iTestObserver->DataProviderId() );
                result = KErrGeneral;
                }
            }
        if ( storageId != KTestStorageId )
            {
            PRINTE2( "Wrong storage id: %d != %d", storageId, KTestStorageId );
            result = KErrGeneral;
            }
        switch ( handle )
            {
            case 1:
                {
                if ( suid.Match( KTestDirPath ) != 0 )
                    {
                    PRINTE2( "Wrong test dir path: %S != %S", &suid, &KTestDirPath );
                    result = KErrGeneral;
                    }
                if ( formatCode != EMTPFormatCodeAssociation )
                    {
                    PRINTE2( "Wrong format code: 0x%x != 0x%x", formatCode, EMTPFormatCodeAssociation );
                    result = KErrGeneral;
                    }
                if ( parentId != KErrNotFound )
                    {
                    PRINTE2( "Wrong parent id: %d != %d", parentId, KErrNotFound );
                    result = KErrGeneral;
                    }
                break;
                }
            case 2:
                {
                if ( suid.Match( KTestObjectPath1 ) != 0 )
                    {
                    PRINTE2( "Wrong test object 1 path: %S != %S", &suid, &KTestObjectPath1 );
                    result = KErrGeneral;
                    }
                if ( formatCode != EMTPFormatCodeMP3 )
                    {
                    PRINTE2( "Wrong format code: 0x%x != 0x%x", formatCode, EMTPFormatCodeMP3 );
                    result = KErrGeneral;
                    }
                if ( parentId != KErrNotFound )
                    {
                    PRINTE2( "Wrong parent id: %d != %d", parentId, KErrNotFound );
                    result = KErrGeneral;
                    }
                break;
                }
            case 3:
                {
                if ( suid.Match( KTestObjectPath2 ) != 0 )
                    {
                    PRINTE2( "Wrong test object 2 path: %S != %S", &suid, KTestObjectPath2 );
                    result = KErrGeneral;
                    }
                if ( formatCode != EMTPFormatCodeMP3 )
                    {
                    PRINTE2( "Wrong format code: 0x%x != 0x%x", formatCode, EMTPFormatCodeMP3 );
                    result = KErrGeneral;
                    }
                if ( parentId != 1 )
                    {
                    PRINTE2( "Wrong parent id: %d != %d", parentId, 1 );
                    result = KErrGeneral;
                    }
                break;
                }
            case 4:
                {
                if ( suid.Match( KTestObjectPath3 ) != 0 )
                    {
                    PRINTE2( "Wrong test object 3 path: %S != %S", &suid, &KTestObjectPath3 );
                    result = KErrGeneral;
                    }
                if ( formatCode != EMTPFormatCodeMP3 )
                    {
                    PRINTE2( "Wrong format code: 0x%x != 0x%x", formatCode, EMTPFormatCodeMP3 );
                    result = KErrGeneral;
                    }
                if ( parentId != 1 )
                    {
                    PRINTE2( "Wrong parent id: %d != %d", parentId, 1 );
                    result = KErrGeneral;
                    }
                break;
                }
            default:
                {
                PRINTE1( "Unexpected handle: %d", handle );
                result = KErrGeneral;
                }
            }
        
        if ( ( result == KErrNone ) && ( handle != KTestDirHandle ) )
            {
            PRINTN0( "Starting to harvest the file" );
            iTestObserver->SetPendingOperation( CTestObserver::EHarvesterAddObject );
            harvester->AddFileL( metadata->DesC( CMTPObjectMetaData::ESuid ), iTestObserver );
            PRINTN0( "Waiting for harvesting to complete" );
            result = iTestObserver->WaitForPendingOperation();
            if ( result != KErrNone )
                {
                PRINTE1( "Harvesting error: %d", result );
                }
            }
        }
    CleanupStack::PopAndDestroy( 3 ); // handles, harvester, metadata
    PRINTF1( "<CMediaMtpDataProviderTester::ObjectEnumerationL_checkresults result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::GetObjectInfoL_checkresults(
        const CMTPTypeObjectInfo* aObjectInfo )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectInfoL_checkresults" );
    _LIT( KTestFileName, "TEST.MP3" );
    TInt result = KErrNone;
    if ( aObjectInfo )
        {
        PRINTV1( "Object filename: %S", &aObjectInfo->StringCharsL( CMTPTypeObjectInfo::EFilename ) );
        if ( aObjectInfo->StringCharsL( CMTPTypeObjectInfo::EFilename ).
                CompareF( KTestFileName ) != 0 )
            {
            PRINTE0( "ObjectInfo filename not correct" );
            result = KErrGeneral;
            }
        PRINTV1( "Object modified date: %S", &aObjectInfo->StringCharsL( CMTPTypeObjectInfo::EDateModified ) );
        if ( aObjectInfo->StringCharsL( CMTPTypeObjectInfo::EDateModified ).
                Length() == 0 )
            {
            PRINTE0( "ObjectInfo modified date not set" );
            }
        PRINTV1( "Object formatcode: 0x%x", aObjectInfo->Uint16L( CMTPTypeObjectInfo::EObjectFormat ) );
        if ( aObjectInfo->Uint16L( CMTPTypeObjectInfo::EObjectFormat ) !=
                EMTPFormatCodeMP3 )
            {
            PRINTE0( "ObjectInfo format code not correct" );
            result=KErrGeneral;
            }
        }
    else
        {
        PRINTE0( "ObjectInfo not received correctly" );
        result = KErrArgument;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectInfoL_checkresults result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::GetObjectL_checkresults(
        CMTPTypeFile* aObjectInfo )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectL_checkresults" );
    // just check the handle is correct, based on the size
    TInt result = KErrNone;
    if ( aObjectInfo )
        {
        TInt size = aObjectInfo->Size();
        PRINTV1( "Received object size: %d", size );
        if ( size != KTestFileSize )
            {
            PRINTE2( "Wrong received object size: %d != %d", KTestFileSize, size );
            result =  KErrGeneral;
            }
        aObjectInfo->File().Close();
        }
    else
        {
        PRINTE0( "Object info not received" );
        result = KErrArgument;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectL_checkresults result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::GetObjectPropValueL_checkresults(
        const MMTPType* aObjectPropValue, const TUint aPropCode )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectPropValueL_checkresults" );
    TInt type = aObjectPropValue->Type();
    PRINTV1( "Received data type: 0x%x", type );
    TInt result = KErrNone;
    
    switch ( aPropCode )
        {
        case EMTPObjectPropCodeStorageID:
            {
            TUint32 value = static_cast<const TMTPTypeUint32*>( aObjectPropValue )->Value();
            PRINTV1( "Storage ID: 0x%x", value );
            if ( value == KTestStorageId )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeObjectFormat:
            {
            TUint16 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Format: 0x%x", value );
            if ( value == EMTPFormatCodeMP3 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeProtectionStatus:
            {
            TUint16 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Protections status: 0x%x", value );
            if ( value == EMTPProtectionNoProtection )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeObjectSize:
            {
            TUint64 value = static_cast<const TMTPTypeUint64*>( aObjectPropValue )->Value();
            PRINTV1( "Object size: %d", value );
            if ( value == KTestFileSize )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeObjectFileName:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Object file name: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeParentObject:
            {
            TUint32 value = static_cast<const TMTPTypeUint32*>( aObjectPropValue )->Value();
            PRINTV1( "Parent object: %d", value );
            if ( value == KMaxTUint32 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
            {
            /*
            TUint64 lower = static_cast<const TMTPTypeUint128*>( aObjectPropValue )->LowerValue();
            TUint64 upper = static_cast<const TMTPTypeUint128*>( aObjectPropValue )->UpperValue();
            __TRACE( KPrint, ( _L( "Unique identifier: 0x%x 0x%x" ), lower, upper ) );
            if ( ( lower == 1) && ( upper == 1 ) )
                {
                result = KErrNone;
                }
            */
            }
            break;
        case EMTPObjectPropCodeName:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Object name: '%S'", &value );
            if ( value.Match( KPropObjectName ) != 0 )
                {
                PRINTE2( "Received object name does not match: '%S' != '%S'", &value, &KPropObjectName );
                result = KErrGeneral;
                }
            }
            break;
        case EMTPObjectPropCodeNonConsumable:
            {
            TUint8 value = static_cast<const TMTPTypeUint8*>( aObjectPropValue )->Value();
            PRINTV1( "Non-consumable: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeDateAdded:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Date added: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDateCreated:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Date created: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDateModified:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Date modified: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeArtist:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Artist: '%S'", &value );
            if ( value.Match( KPropObjectArtist ) != 0 )
                {
                PRINTE2( "Received object artist does not match: '%S' != '%S'", &value, &KPropObjectArtist );
                result = KErrGeneral;
                }
            }
            break;
        case EMTPObjectPropCodeTrack:
            {
            TUint16 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Track: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeGenre:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Genre: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeAlbumName:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Album name: '%S'", &value );
            if ( value.Match( KPropObjectAlbumName ) != 0 )
                {
                PRINTE2( "Received object album name does not match: '%S' != '%S'", &value, &KPropObjectAlbumName );
                result = KErrGeneral;
                }
            }
            break;
        case EMTPObjectPropCodeSampleRate:
            {
            TUint32 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Sample rate: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeNumberOfChannels:
            {
            TUint16 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Number of channels: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeAudioWAVECodec:
            {
            TUint32 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "WAVE codec: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeAudioBitRate:
            {
            TUint32 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Audio bit rate: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeDuration:
            {
            TUint32 value = static_cast<const TMTPTypeUint16*>( aObjectPropValue )->Value();
            PRINTV1( "Duration: %d", value );
            if ( value == 1 )
                {
                result = KErrNone;
                }
            }
            break;
        case EMTPObjectPropCodeOriginalReleaseDate:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Original release date: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDescription:
            {
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeComposer:
            {
            TPtrC value = static_cast<const CMTPTypeString*>( aObjectPropValue )->StringChars();
            PRINTV1( "Composer: '%S'", &value );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeWidth:
        case EMTPObjectPropCodeHeight:
        case EMTPObjectPropCodeUseCount:
        case EMTPObjectPropCodeScanType:
        case EMTPObjectPropCodeVideoFourCCCodec:
        case EMTPObjectPropCodeVideoBitRate:
        case EMTPObjectPropCodeFramesPerThousandSeconds:
        case EMTPObjectPropCodeKeyFrameDistance:
        case EMTPObjectPropCodeEncodingProfile:
        case EMTPObjectPropCodeParentalRating:
            {
            result = KErrNone;
            }
            break;
        default:
            {
            PRINTE1( "Unsupported prop code: %d", aPropCode );
            result = KErrArgument;
            }
        }
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectPropValueL_checkresults result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::GetObjectPropListL_checkresults(
        const CMTPTypeObjectPropList* aObjectPropList )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectPropListL_checkresults" );
    TInt result = KErrNone;
    TUint32 count = aObjectPropList->NumberOfElements();
    PRINTV1( "PropList elements received: %d", count );
    for ( TUint32 i = 0; i < count; i++ )
        {
        CMTPTypeObjectPropListElement& element = aObjectPropList->Element( i );
        TUint32 handle = element.Uint32L( CMTPTypeObjectPropListElement::EObjectHandle );
        PRINTV1( "Object handle: %d", handle );
        TUint16 dataType = element.Uint16L( CMTPTypeObjectPropListElement::EDatatype );
        PRINTV1( "Data type: 0x%x", dataType );
        TUint16 propCode = element.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode );
        PRINTV1( "Prop code: 0x%x", propCode );
        if ( propCode == EMTPObjectPropCodeName )
            {
            if ( dataType == EMTPTypeString )
                {
                TPtrC name = element.StringCharsL( CMTPTypeObjectPropListElement::EValue );
                PRINTN1( "Object name: %S", &name );
                if ( name.Match( KPropObjectName ) != 0 )
                    {
                    PRINTE2( "Object modified date is incorrect: %S != %S", &name, &KPropObjectName );
                    result = KErrGeneral;
                    }
                }
            else
                {
                PRINTE2( "Wrong datatype received, 0x%x != 0x%x", dataType, EMTPTypeString );
                result = KErrArgument;
                }
            }
        }
    
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectPropListL_checkresults result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::GetObjectPropDescL_checkresults(
        const CMTPTypeObjectPropDesc* aObjectPropDesc, const TUint aPropCode )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectPropDescL_checkresults" );
    TUint16 propertyCode = aObjectPropDesc->Uint16L( CMTPTypeObjectPropDesc::EPropertyCode );
    TUint16 dataType = aObjectPropDesc->Uint16L( CMTPTypeObjectPropDesc::EDatatype );
    TUint8 getSet = aObjectPropDesc->Uint8L( CMTPTypeObjectPropDesc::EGetSet );
    TUint8 formFlag = aObjectPropDesc->Uint8L( CMTPTypeObjectPropDesc::EFormFlag );

    TInt result = KErrNone;
    
    PRINTV4( "Property code: 0x%x, data type: 0x%x, form flag: 0x%x, get/set: 0x%x", propertyCode, dataType, formFlag, getSet );
    
    if ( formFlag != CMTPTypeObjectPropDesc::ENone )
        {
        switch ( formFlag )
            {
            case CMTPTypeObjectPropDesc::ERangeForm:
                PRINTV0( "Range form" );
                break;
            case CMTPTypeObjectPropDesc::EEnumerationForm:
                PRINTV0( "Enumeration form" );
                break;
            case CMTPTypeObjectPropDesc::EDateTimeForm:
                PRINTV0( "Datetime form" );
                break;
            case CMTPTypeObjectPropDesc::EFixedLengthArrayForm:
                PRINTV0( "Fixed length array form" );
                break;
            case CMTPTypeObjectPropDesc::ERegularExpressionForm:
                PRINTV0( "Regular expression form" );
                break;
            case CMTPTypeObjectPropDesc::EByteArrayForm:
                PRINTV0( "Byte array form" );
                break;
            case CMTPTypeObjectPropDesc::ELongStringForm:
                PRINTV0( "Long string form" );
            default:
                {
                PRINTE0( "Invalid form" );
                result = KErrArgument;
                }
            }
        }
    
    switch ( aPropCode )
        {
        case EMTPObjectPropCodeStorageID:
            {
            PRINTV0( "Storage ID" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeObjectFormat:
            {
            PRINTV0( "Object format" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeProtectionStatus:
            {
            PRINTV0( "Protection status" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeObjectSize:
            {
            PRINTV0( "Object size" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeObjectFileName:
            {
            PRINTV0( "Object file name" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeParentObject:
            {
            PRINTV0( "Parent object" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
            {
            PRINTV0( "Unique object identifier" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeName:
            {
            PRINTV0( "Name" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeNonConsumable:
            {
            PRINTV0( "Non consumable" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDateAdded:
            {
            PRINTV0( "Date added" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDateCreated:
            {
            PRINTV0( "Date created" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDateModified:
            {
            PRINTV0( "Date modified" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeArtist:
            {
            PRINTV0( "Artist" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeTrack:
            {
            PRINTV0( "Track" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeGenre:
            {
            PRINTV0( "Genre" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeAlbumName:
            {
            PRINTV0( "Album name" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeSampleRate:
            {
            PRINTV0( "Sample rate" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeNumberOfChannels:
            {
            PRINTV0( "Number of channels" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeAudioWAVECodec:
            {
            PRINTV0( "Audio wave codec" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeAudioBitRate:
            {
            PRINTV0( "Audio bitrate" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDuration:
            {
            PRINTV0( "Duration" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeOriginalReleaseDate:
            {
            PRINTV0( "Original release date" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeDescription:
            {
            PRINTV0( "Description" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeComposer:
            {
            PRINTV0( "Composer" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeWidth:
            {
            PRINTV0( "Width" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeHeight:
            {
            PRINTV0( "Height" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeUseCount:
            {
            PRINTV0( "Use count" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeScanType:
            {
            PRINTV0( "Scan type" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeVideoFourCCCodec:
            {
            PRINTV0( "Video 4cc codec" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeVideoBitRate:
            {
            PRINTV0( "Video bitrate" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeFramesPerThousandSeconds:
            {
            PRINTV0( "Frames per thousand seconds" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeKeyFrameDistance:
            {
            PRINTV0( "Key frame distance" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeEncodingProfile:
            {
            PRINTV0( "Encoding profile" );
            result = KErrNone;
            }
            break;
        case EMTPObjectPropCodeParentalRating:
            {
            PRINTV0( "Parental rating" );
            result = KErrNone;
            }
            break;
        default:
            {
            PRINTE1( "Unsupported prop code: %d", aPropCode );
            result = KErrArgument;
            }
        }
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectPropDescL_checkresults result = %d", result );
    return result;
    }

TBool CMediaMtpDataProviderTester::SendObjectInfoL_prepare(
        TMTPTypeRequest& aRequest )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::SendObjectInfoL_prepare" );
    TBool result = ETrue;
    TUint32 storageId = KTestStorageId; // physical storage
    aRequest.SetUint32( TMTPTypeRequest::ERequestParameter1, storageId );
    PRINTF1( "<CMediaMtpDataProviderTester::SendObjectInfoL_prepare result = %d", result );
    return result;
    }

void CMediaMtpDataProviderTester::SendObjectInfoReceiveData(
        CMTPTypeObjectInfo* aData )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::SendObjectInfoReceiveData" );
    aData->SetUint32L( CMTPTypeObjectInfo::EStorageID, KTestStorageId );
    aData->SetUint16L( CMTPTypeObjectInfo::EObjectFormat, EMTPFormatCodeMP3 );
    aData->SetStringL( CMTPTypeObjectInfo::EFilename, KSendTestObjectFile );
    aData->SetStringL( CMTPTypeObjectInfo::EDateModified, KSendTestObjectDateString );
    aData->SetUint32L( CMTPTypeObjectInfo::EObjectCompressedSize, KSendTestFileSize );
    PRINTF0( "<CMediaMtpDataProviderTester::SendObjectInfoReceiveData" );
    }

TBool CMediaMtpDataProviderTester::SendObjectL_prepare(
        TMTPTypeRequest& aRequest )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::SendObjectL_prepare" );
    TBool result = ETrue;
    TUint32 storageId = KTestStorageId; // physical storage
    aRequest.SetUint32( TMTPTypeRequest::ERequestParameter1, storageId );
    PRINTF1( "<CMediaMtpDataProviderTester::SendObjectL_prepare result = %d", result );
    return result;
    }

void CMediaMtpDataProviderTester::SendObjectReceiveDataL( CMTPTypeFile* aData )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::SendObjectReceiveDataL" );
    CMTPTypeFile* fileObject = CMTPTypeFile::NewL( iFsSession,
            KSendTestObjectFileFullPath, EFileRead  );
    CMTPTypeFile::CopyL( *fileObject, *aData );
    delete fileObject;
    aData->File().Close();
    PRINTF0( "<CMediaMtpDataProviderTester::SendObjectReceiveDataL" );
    }

TInt CMediaMtpDataProviderTester::SendObjectL_checkresults()
    {
    PRINTF0( ">CMediaMtpDataProviderTester::SendObjectL_checkresults" );
    TInt result = KErrNone;
    TEntry entry;
    if ( !BaflUtils::FileExists( iFsSession, KSendTestObjectPath ) )
        {
        PRINTE0( "Sent file does not exist" );
        result = KErrGeneral;
        }
    else
        {
        result = iFsSession.Entry( KSendTestObjectPath, entry );
        if ( result != KErrNone )
            {
            PRINTE1( "Could not read file entry, error: %d", result );
            }
        else
            {
            if ( entry.iSize != KSendTestFileSize )
                {
                PRINTE2( "Sent file size is incorrect: %d != %d", entry.iSize, KSendTestFileSize );
                result = KErrGeneral;
                }
            }
        }
    if ( result == KErrNone )
        {
        CMTPObjectMetaData* metadata = CMTPObjectMetaData::NewLC();
        iTestObserver->ObjectMgr().ObjectL( KSendTestObjectPath, *metadata );
        if ( metadata->Uint( CMTPObjectMetaData::EHandle ) <= 0 )
            {
            PRINTE1( "Object handle not found: %d", metadata->Uint( CMTPObjectMetaData::EHandle ) );
            result = KErrGeneral;
            }
        if ( metadata->Uint( CMTPObjectMetaData::EFormatCode ) != EMTPFormatCodeMP3 )
            {
            PRINTE1( "Wrong format code: %d", metadata->Uint( CMTPObjectMetaData::EFormatCode ) );
            result = KErrGeneral;
            }
        CleanupStack::PopAndDestroy( metadata );
        }
    PRINTF1( "<CMediaMtpDataProviderTester::SendObjectL_checkresults result = %d", result );
    return result;
    }

void CMediaMtpDataProviderTester::SetObjectPropValueReceiveDataL(
        const TMTPTypeRequest& aRequest, MMTPType* aData )
    {
    TUint32 propCode = aRequest.Uint32( TMTPTypeRequest::ERequestParameter2 );
    TUint type = aData->Type();
    switch ( propCode )
        {
        case EMTPObjectPropCodeName:
            if ( type != EMTPTypeString )
                {
                PRINTE2( "Incorrect data type: 0x%x != 0x%x", type, EMTPTypeString );
                User::Leave( KErrArgument );
                }
            static_cast<CMTPTypeString*>( aData )->SetL( KPropObjectName );
            break;
        case EMTPObjectPropCodeArtist:
            if ( type != EMTPTypeString )
                {
                PRINTE2( "Incorrect data type: 0x%x != 0x%x", type, EMTPTypeString );
                User::Leave( KErrArgument );
                }
            static_cast<CMTPTypeString*>( aData )->SetL( KPropObjectArtist );
            break;
        case EMTPObjectPropCodeAlbumName:
            if ( type != EMTPTypeString )
                {
                PRINTE2( "Incorrect data type: 0x%x != 0x%x", type, EMTPTypeString );
                User::Leave( KErrArgument );
                }
            static_cast<CMTPTypeString*>( aData )->SetL( KPropObjectAlbumName );
            break;
        default:
            PRINTE1( "Property code not supported: 0x%x", propCode );
            User::Leave( KErrNotSupported );
            break;
        }
    }

void CMediaMtpDataProviderTester::SetObjectPropListReceiveDataL(
        CMTPTypeObjectPropList* aData )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::SetObjectPropListReceiveDataL" );
    CMTPTypeString* nameString = CMTPTypeString::NewLC( KPropObjectName );
    CMTPTypeObjectPropListElement* newElement;
    newElement = CMTPTypeObjectPropListElement::NewL( 1, static_cast<TUint16>( EMTPObjectPropCodeName ), *nameString );
    aData->AppendL( newElement );
    newElement = CMTPTypeObjectPropListElement::NewL( 2, static_cast<TUint16>( EMTPObjectPropCodeName ), *nameString );
    aData->AppendL( newElement );
    newElement = CMTPTypeObjectPropListElement::NewL( 3, static_cast<TUint16>( EMTPObjectPropCodeName ), *nameString );
    aData->AppendL( newElement );
    CleanupStack::Pop( nameString );
    PRINTF0( "<CMediaMtpDataProviderTester::SetObjectPropListReceiveDataL" );
    }

TInt CMediaMtpDataProviderTester::DeleteObjectL_checkresults(
        MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::DeleteObjectL_checkresults" );
    TInt result = KErrNone;
    // check that everything has been deleted from the file system
    if ( BaflUtils::FileExists( iFsSession, KTestObjectPath1 ) ) 
        {
        PRINTE0( "Test file 1 was not deleted successfully" );
        result = KErrGeneral;
        }
    if ( BaflUtils::FileExists( iFsSession, KTestObjectPath2 ) ) 
        {
        PRINTE0( "Test file 2 was not deleted successfully" );
        result = KErrGeneral;
        }
    if ( BaflUtils::FileExists( iFsSession, KTestObjectPath3 ) ) 
        {
        PRINTE0( "Test file 3 was not deleted successfully" );
        result = KErrGeneral;
        }
    PRINTF1( ">CMediaMtpDataProviderTester::DeleteObjectL_checkresults result = %d", result );
    return result;
    }

TBool CMediaMtpDataProviderTester::CopyObjectL_prepare(
        TMTPTypeRequest& aRequest )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::CopyObjectL_prepare" );
    TBool result = ETrue;
    TUint32 storageId = KTestStorageId; // physical storage
    aRequest.SetUint32( TMTPTypeRequest::ERequestParameter2, storageId );
    PRINTF1( "<CMediaMtpDataProviderTester::CopyObjectL_prepare result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::CopyObjectL_checkresults(
        const CMTPObjectMetaData* aMetaData )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::CopyObjectL_checkresults" );
    TInt result = KErrNone;
    if ( !BaflUtils::FileExists( iFsSession, KCopyTestObjectPath ) )
        {
        PRINTE0( "Copied file does not exist" );
        result = KErrGeneral;
        }
    else
        {
        TEntry entry;
        result = iFsSession.Entry( KCopyTestObjectPath, entry );
        if ( result != KErrNone )
            {
            PRINTE1( "Could not read file entry, error: %d", result );
            }
        else
            {
            if ( entry.iSize != KCopyTestFileSize )
                {
                PRINTE2( "Copied file size is incorrect: %d != %d", entry.iSize, KCopyTestFileSize );
                result = KErrGeneral;
                }
            }
        }
    
    if ( aMetaData )
        {
        PRINTV1( "Suid: %S", &aMetaData->DesC( CMTPObjectMetaData::ESuid ) );
        
        if ( aMetaData->Uint( CMTPObjectMetaData::EParentHandle ) != KMTPHandleNoParent )
            {
            PRINTV1( "Parent handle = %d", aMetaData->Uint( CMTPObjectMetaData::EParentHandle ) );
            //TODO: parent handles don't work yet properly
            //result = KErrGeneral;
            }
        
        if ( aMetaData->Int( CMTPObjectMetaData::EParentId ) != KErrNotFound )
            {
            PRINTV1( "Parent id = %d", aMetaData->Int( CMTPObjectMetaData::EParentId ) );
            //TODO: parent ids don't work yet properly
            //result = KErrGeneral;
            }
        if ( aMetaData->Uint( CMTPObjectMetaData::EDataProviderId ) != 0 )
            {
            PRINTV1( "Dataprovider id = %d", aMetaData->Uint( CMTPObjectMetaData::EDataProviderId ) );
            //TODO: not sure if this is necessary
            //result = KErrGeneral;
            }
        if ( aMetaData->Uint( CMTPObjectMetaData::EFormatCode ) != EMTPFormatCodeMP3 )
            {
            PRINTV1( "Format code = %d", aMetaData->Uint( CMTPObjectMetaData::EFormatCode ) );
            result = KErrGeneral;
            }
        if ( aMetaData->Uint( CMTPObjectMetaData::EStorageId ) != KTestStorageId )
            {
            PRINTV1( "StorageId = %d ", aMetaData->Uint( CMTPObjectMetaData::EStorageId ) );
            result = KErrGeneral;
            }
        if ( ( aMetaData->DesC( CMTPObjectMetaData::ESuid ) ).Match( KCopyTestObjectPath ) != 0 )
            {
            PRINTV0( "Copied object path incorrect" );
            result = KErrGeneral;
            }
        }
    else
        {
        PRINTV0( "Object metadata not received correctly" );
        result = KErrArgument;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::CopyObjectL_checkresults result = %d", result );
    return result;
    }

TBool CMediaMtpDataProviderTester::MoveObjectL_prepare(
        TMTPTypeRequest& aRequest )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::MoveObjectL_prepare" );
    TBool result = ETrue;
    TUint32 storageId = KTestStorageId; // physical storage
    aRequest.SetUint32( TMTPTypeRequest::ERequestParameter2, storageId );
    PRINTF1( "<CMediaMtpDataProviderTester::MoveObjectL_prepare result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::MoveObjectL_checkresults( const CMTPObjectMetaData* aMetaData )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::MoveObjectL_checkresults" );
    TInt result = KErrNone;
    if ( BaflUtils::FileExists( iFsSession, KMoveTestObjectOldPath ) )
        {
        PRINTE0( "Moved file still exists in original location" );
        result = KErrGeneral;
        }
    if ( !BaflUtils::FileExists( iFsSession, KMoveTestObjectNewPath ) )
        {
        PRINTE0( "Moved file does not exist in new location" );
        result = KErrGeneral;
        }
    else
        {
        TEntry entry;
        result = iFsSession.Entry( KMoveTestObjectNewPath, entry );
        if ( result != KErrNone )
            {
            PRINTE1( "Could not read file entry, error: %d", result );
            }
        else
            {
            if ( entry.iSize != KMoveTestFileSize )
                {
                PRINTE2( "Moved file size is incorrect: %d != %d", entry.iSize, KCopyTestFileSize );
                result = KErrGeneral;
                }
            }
        }
    
    if ( aMetaData )
        {
        PRINTV1( "Suid: %S", &aMetaData->DesC( CMTPObjectMetaData::ESuid ) );
        
        if ( aMetaData->Uint( CMTPObjectMetaData::EParentHandle ) != KMTPHandleNoParent )
            {
            PRINTV1( "Parent handle = %d", aMetaData->Uint( CMTPObjectMetaData::EParentHandle ) );
            //TODO: parent handles don't work yet properly
            //result = KErrGeneral;
            }
     
        if ( aMetaData->Int( CMTPObjectMetaData::EParentId ) != KErrNotFound )
            {
            PRINTV1( "Parent id = %d", aMetaData->Int( CMTPObjectMetaData::EParentId ) );
            //TODO: parent ids don't work yet properly
            //result = KErrGeneral;
            }
        if ( aMetaData->Uint( CMTPObjectMetaData::EDataProviderId ) != 0 )
            {
            PRINTV1( "Dataprovider id = %d", aMetaData->Uint( CMTPObjectMetaData::EDataProviderId ) );
            //TODO: not sure if this is necessary
            //result = KErrGeneral;
            }
        if ( aMetaData->Uint( CMTPObjectMetaData::EFormatCode ) != EMTPFormatCodeMP3 )
            {
            PRINTV1( "Format code = %d", aMetaData->Uint( CMTPObjectMetaData::EFormatCode ) );
            result = KErrGeneral;
            }
        if ( aMetaData->Uint( CMTPObjectMetaData::EStorageId ) != KTestStorageId )
            {
            PRINTV1( "StorageId = %d ", aMetaData->Uint( CMTPObjectMetaData::EStorageId ) );
            result = KErrGeneral;
            }
        if ( ( aMetaData->DesC( CMTPObjectMetaData::ESuid ) ).Match( KMoveTestObjectNewPath ) != 0 )
            {
            PRINTV0( "Copied object path incorrect" );
            result = KErrGeneral;
            }
        }
    else
        {
        PRINTV0( "Object metadata not received correctly" );
        result = KErrArgument;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::MoveObjectL_checkresults result = %d", result );
    return KErrNone;
    }

TInt CMediaMtpDataProviderTester::GetPartialObjectL_checkresults( CMTPTypeFile *aObjectInfo )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetPartialObjectL_checkresults" );
    // just check the handle is correct, based on the size...
    TInt result = KErrNone;
    if ( aObjectInfo ) 
        {
        TInt size = aObjectInfo->Size();
        __TRACE( KPrint, ( _L("Received object size: %d"), size ) );
        if ( size != KTestFileSize )
            {
            __TRACE( KError, ( _L("Wrong received object size: %d != %d" ), KTestFileSize, size ) );
            result =  KErrGeneral;
            }
        aObjectInfo->File().Close();
        }
    else
        {
        __TRACE( KError, ( _L("Object info not received" ) ) );
        result = KErrArgument;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::GetPartialObjectL_checkresults result = %d", result );
    return result;    
    }

TBool CMediaMtpDataProviderTester::RenameObjectL_prepare( TMTPNotificationParamsHandle& aParam )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::RenameObjectL_prepare" );
    TBool result = ETrue;
    TFileName fileName( KRenameObjectName );
    aParam.iHandleId = KTestDirHandle;
    aParam.iFileName = fileName;
    
    TInt moveResult = iFileMan->Move( KTestDirPath, KRenameObjectPath,
            CFileMan::ERecurse );
    if ( moveResult != KErrNone )
        {
        PRINTE1( "Moving directory failed with error %d", moveResult );
        }
    CMTPObjectMetaData* metadata = CMTPObjectMetaData::NewLC();
    iTestObserver->ObjectMgr().ObjectL( KTestDirPath, *metadata );
    metadata->SetDesCL( CMTPObjectMetaData::ESuid, KRenameObjectPath );
    iTestObserver->ObjectMgr().ModifyObjectL( *metadata );
    CleanupStack::PopAndDestroy( metadata );
    PRINTF1( "<CMediaMtpDataProviderTester::RenameObjectL_prepare result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::RenameObjectL_checkresults( const CMTPObjectMetaData* aMetaData )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::RenameObjectL_checkresults" );
    TInt result = KErrNone;
    
    TMTPObjectMgrQueryParams queryParams ( KMTPStorageAll, KMTPFormatsAll, KMTPHandleNone );
    RMTPObjectMgrQueryContext context;
    RArray<TUint> handles;
    CleanupClosePushL( handles );
    
    iTestObserver->ObjectMgr().GetObjectHandlesL( queryParams, context, handles );
    context.Close();
    TInt count = handles.Count();
    PRINTV1( "Handle count: %d", count );
    
    CMTPObjectMetaData* metadata = CMTPObjectMetaData::NewLC();
    for ( TInt i = 0; i < count; i++ )
        {
        TUint handle = handles[i];
        iTestObserver->ObjectMgr().ObjectL( handle, *metadata );
        TPtrC fileName = metadata->DesC( CMTPObjectMetaData::ESuid );
        PRINTV1( "Handle: %d", handle );
        PRINTV1( "File name: %S", &fileName );
        }
    CleanupStack::PopAndDestroy( 2 ); // metadata, handles
    PRINTF1( "<CMediaMtpDataProviderTester::RenameObjectL_checkresults result = %d", result );
    return result;
    }

TBool CMediaMtpDataProviderTester::GetObjectPropsSupportedL_prepare( TMTPTypeRequest& aRequest )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectPropsSupportedL_prepare" );
    TBool result = ETrue;
    aRequest.SetUint32( TMTPTypeRequest::ERequestParameter1, EMTPFormatCodeMP3 );
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectPropsSupportedL_prepare result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::GetObjectPropsSupportedL_checkresults( const CMTPTypeArray* aObjectProps )
    {
    PRINTF0( ">CMediaMtpDataProviderTester::GetObjectPropsSupportedL_checkresults" );
    TInt result = KErrNone;
    TInt count = aObjectProps->NumElements();
    PRINTV1( "Object props found: %d", count );
    PRINTF1( "<CMediaMtpDataProviderTester::GetObjectPropsSupportedL_checkresults result = %d", result );
    return result;
    }

TBool CMediaMtpDataProviderTester::ProcessRequestIllegalIdL_prepare(TInt aOpCode) 
    {
    PRINTF0( ">CMediaMtpDataProviderTester::ProcessRequestIllegalIdL_prepare" );
    // EMTPOpCodeGetObjectPropValue cannot be tested with the invalid session id, as the object
    // must exist or the data provider fails in __ASSERT_DEBUG(aObjectInfo, Panic(EMTPPictureDpObjectNull));
    TBool result = ETrue;
    if ( aOpCode == EMTPOpCodeGetObjectPropValue )
        {
        result = EFalse;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::ProcessRequestIllegalIdL_prepare result = %d", result );
    return result;
    }

TInt CMediaMtpDataProviderTester::ProcessRequestIllegalIdL_checkresults(TInt aOpCode, TInt aResponseCode)
    {
    PRINTF0( ">CMediaMtpDataProviderTester::ProcessRequestIllegalIdL_checkresults" );
    TInt result = KErrGeneral;
    switch (aOpCode)
        {
        case EMTPOpCodeGetObjectInfo:
        case EMTPOpCodeGetObject:
        case EMTPOpCodeGetPartialObject:
        case EMTPOpCodeGetThumb:
        case EMTPOpCodeDeleteObject:
        case EMTPOpCodeMoveObject:
        case EMTPOpCodeCopyObject:
        case EMTPOpCodeGetObjectPropsSupported:
        case EMTPOpCodeGetObjectPropList:
        case EMTPOpCodeGetObjectReferences:
            if (aResponseCode == EMTPRespCodeSessionNotOpen) 
                {
                result = KErrNone;
                }
            break;
        case EMTPOpCodeSendObjectInfo:
        case EMTPOpCodeSendObject:
        case EMTPOpCodeSetObjectPropValue:
        case EMTPOpCodeSetObjectPropList:
        case EMTPOpCodeSendObjectPropList:
        case EMTPOpCodeSetObjectReferences:
            if (aResponseCode == 0) 
                 {
                 result = KErrNone;
                 }   
            break;
        case EMTPOpCodeGetObjectPropDesc:
            if (aResponseCode == EMTPRespCodeInvalidObjectFormatCode) 
                 {
                 result = KErrNone;
                 }
            break;
        case EMTPOpCodeGetDeviceInfo:
            if (aResponseCode == EMTPRespCodeInvalidTransactionID) 
                 {
                 result = KErrNone;
                 }
            break;            
        default:
            result = KErrNone;
            break;
        }
    PRINTF1( "<CMediaMtpDataProviderTester::ProcessRequestIllegalIdL_checkresults result = %d", result );
    return result;
    }
