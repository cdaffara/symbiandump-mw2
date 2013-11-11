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
* Description:  Tester class for abstractmediamtpdp plugin
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
#include "cabstractmediamtpdataprovidertester.h"
#include "logging.h"

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog

// definitions and constants for the test data

const TUint32 KTestStorageId = 0x10000;
const TInt KTestObjectsCount = 1;
const TUint KTestDirHandle = 1;
const TInt KTestDirDataproviderId = 0;
const TInt KPlaylistsDirParentId = 0;
_LIT( KTempDirPath, "C:\\mtp_testdata\\temp\\" );
_LIT( KTestDirPath, "C:\\mtp_testdata\\temp\\TESTDIR\\" );
_LIT( KPlaylistFileOrigPath, "C:\\mtp_testdata\\PLAYLIST.M3U" );
_LIT( KPlaylistFileTempPath, "C:\\mtp_testdata\\temp\\PLAYLIST.m3u" );
_LIT( KPlaylistFilePath, "C:\\mtp_testdata\\temp\\PLAYLIST.m3u" );
_LIT( KPlaylistsDirPath, "C:\\mtp_testdata\\temp\\Playlists\\" );
_LIT( KTestDirOrigPath, "C:\\mtp_testdata\\TESTDIR\\*" );
_LIT( KTestDirTempPath, "C:\\mtp_testdata\\temp\\TESTDIR\\" );
_LIT( KTestDirPhysicalStorage, "C:\\mtp_testdata\\temp\\" );
_LIT( KTestDirLogicalStorage, "C:\\mtp_testdata\\temp\\" );

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderTester* CAbstractMediaMtpDataProviderTester::NewL()
    {
    CAbstractMediaMtpDataProviderTester* self = 
            new ( ELeave ) CAbstractMediaMtpDataProviderTester();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderTester::CAbstractMediaMtpDataProviderTester()
        : CMmMtpDataProviderTester()
    {
    // no implementation required
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderTester::ConstructL()
    {
    TFileName logFileName;
    logFileName.Copy( KMtpDataProviderTestModuleLogFile );
    
    iLog = CStifLogger::NewL( KMtpDataProviderTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          TRACE_TARGET,
                          ETrue, ETrue, ETrue, EFalse, ETrue );
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::ConstructL" );
    iActiveSchedulerWait = new ( ELeave ) CActiveSchedulerWait();
    iFsSession.Connect();
    iFileMan = CFileMan::NewL( iFsSession );
    PRINTF0( "<CAbstractMediaMtpDataProviderTester::ConstructL" );
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//  
CAbstractMediaMtpDataProviderTester::~CAbstractMediaMtpDataProviderTester()
    {
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::~CAbstractMediaMtpDataProviderTester" );
    delete iActiveSchedulerWait;
    delete iFileMan;
    iFsSession.Close();
    PRINTF0( "<CAbstractMediaMtpDataProviderTester::~CAbstractMediaMtpDataProviderTester" );
    delete iLog;
    }

TInt CAbstractMediaMtpDataProviderTester::PrepareEnvironmentL()
    {
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::PrepareEnvironmentL" );
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
    result = iFileMan->Copy( KPlaylistFileOrigPath, KPlaylistFileTempPath );
    if ( result != KErrNone ) 
        {
        PRINTE1( "Failed to copy playlist file, error %d", result );
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
    
    MMPXHarvesterUtility* harvester = CMPXHarvesterFactory::NewL();
    CleanupClosePushL( *harvester );
    PRINTN0( "Starting to import playlist to mpx" );
    iTestObserver->SetPendingOperation( CTestObserver::EHarvesterAddObject );
    harvester->AddFileL(KPlaylistFilePath, iTestObserver);
    
    // waiting for asynchronous operation to complete
    result = iTestObserver->WaitForPendingOperation(); 
    if ( result != KErrNone )
        {
        PRINTE1( "Importing playlist to mpx failed with result %d", result );
        }
    harvester->CloseTransactionL();
    CleanupStack::PopAndDestroy( harvester );
    PRINTF1( "<CAbstractMediaMtpDataProviderTester::PrepareEnvironmentL result = %d", result );
    return result;
    }

TInt CAbstractMediaMtpDataProviderTester::CleanEnvironmentL()
    {
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::CleanEnvironmentL" );
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
        TUint handle = handles[i];
        if ( handle != KTestDirHandle )
            {
            iTestObserver->ObjectMgr().ObjectL( handles[i], *metadata );
            TPtrC fileName = metadata->DesC( CMTPObjectMetaData::ESuid );
            PRINTV1( "Handle: %d", handles[i] );
            PRINTV1( "File name: %S", &fileName );
            
            PRINTN0( "Starting to delete the file from mpx" );
            iTestObserver->SetPendingOperation( CTestObserver::EHarvesterDeleteObject );
            harvester->DeleteFileL( fileName, iTestObserver );
            User::After( 1000000 );
            // waiting for asynchronous operation to complete
            result = iTestObserver->WaitForPendingOperation();
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
    
    PRINTF1( "<CAbstractMediaMtpDataProviderTester::CleanEnvironmentL result = %d", result );
    return result;
    }

TBool CAbstractMediaMtpDataProviderTester::ObjectEnumerationL_prepare(
        CMTPStorageMetaData* aPhysicalStorage,
        CMTPStorageMetaData* aLogicalStorage )
    {
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::ObjectEnumerationL_prepare" );
    
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
    
    PRINTF1( "<CAbstractMediaMtpDataProviderTester::ObjectEnumerationL_prepare result = %d", result );
    return result;
    }

TInt CAbstractMediaMtpDataProviderTester::ObjectEnumerationL_checkresults(
        MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::ObjectEnumerationL_checkresults" );
    
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
        
        if ( dataproviderId != KTestDirDataproviderId )
            {
            PRINTE2( "Wrong dataprovider id: %d != %d", dataproviderId, KTestDirDataproviderId );
            result = KErrGeneral;
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
                if ( suid.Match( KPlaylistsDirPath ) != 0 )
                    {
                    PRINTE2( "Wrong test object 1 path: %S != %S", &suid, &KPlaylistsDirPath );
                    result = KErrGeneral;
                    }
                if ( formatCode != EMTPFormatCodeAssociation )
                    {
                    PRINTE2( "Wrong format code: 0x%x != 0x%x", formatCode, EMTPFormatCodeAssociation );
                    result = KErrGeneral;
                    }
                if ( parentId != KPlaylistsDirParentId )
                    {
                    PRINTE2( "Wrong parent id: %d != %d", parentId, KPlaylistsDirParentId );
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
        /*
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
        */
        }
    CleanupStack::PopAndDestroy( 3 ); // handles, harvester, metadata
    PRINTF1( "<CAbstractMediaMtpDataProviderTester::ObjectEnumerationL_checkresults result = %d", result );
    return result;
    }

TInt CAbstractMediaMtpDataProviderTester::DeleteObjectL_checkresults(
        MMTPObjectMgr& aObjectManager )
    {
    PRINTF0( ">CAbstractMediaMtpDataProviderTester::DeleteObjectL_checkresults" );
    TInt result = KErrNone;
    // check that everything has been deleted from the file system
    if ( BaflUtils::FileExists( iFsSession, KPlaylistFilePath ) ) 
        {
        PRINTE0( "Playlist file was not deleted successfully" );
        // playlist file is not enumerated, so it cannot be deleted
        //result = KErrGeneral;
        }
    PRINTF1( ">CAbstractMediaMtpDataProviderTester::DeleteObjectL_checkresults result = %d", result );
    return result;
    }
