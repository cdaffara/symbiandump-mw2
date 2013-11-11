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
* Description:  Test module method implementations
*
*/


// INCLUDE FILES

#include <e32svr.h>
#include <stiflogger.h>
#include <stifparser.h>
#include <stiftestinterface.h>
#include <mtp/cmtpdataproviderplugin.h>
#include <mtp/mmtpdataprovider.h>
#include <ecom/ecom.h>
#include <mtp/tmtptypeevent.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/tmtptyperequest.h>
#include <mtp/cmtptypeobjectinfo.h>
#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypefile.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/cmtptypeobjectpropdesc.h>
#include <bautils.h>
#include <e32debug.h>

#include "cmmmtpdataprovidertestmodule.h"
#include "cmmmtpdataprovidertester.h"
#include "cmediamtpdataprovidertester.h"
#include "cabstractmediamtpdataprovidertester.h"
#include "logging.h"

// MACROS

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog
#ifdef PRINTTESTMODULEIF
#undef PRINTTESTMODULEIF
#endif
#define PRINTTESTMODULEIF( p ) TestModuleIf().Printf( 0, KTestModuleName, p );
#ifdef PRINTTESTMODULE
#undef PRINTTESTMODULE
#endif
#define PRINTTESTMODULE( p ) this->SetResultDescription( p );

// MEMBER FUNCTIONS

void CMmMtpDataProviderTestModule::Delete() 
    {
    PRINTF0( ">CMmMtpDataProviderTestModule::Delete()" );
    delete iDataProviderPlugin;
    delete iPluginTester;
    
    if ( iRequest != NULL )
        {
        PRINTN0( "Delete request" );
        delete iRequest;
        }
    REComSession::FinalClose();
    PRINTF0( "<CMmMtpDataProviderTestModule::Delete()" );
    }

TInt CMmMtpDataProviderTestModule::RunMethodL( CStifItemParser& aItem ) 
    {
    PRINTF0( ">CMmMtpDataProviderTestModule::RunMethodL" );
    
    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "LoadPlugin", CMmMtpDataProviderTestModule::LoadPluginL ),
        ENTRY( "PrepareEnvironment", CMmMtpDataProviderTestModule::PrepareEnvironmentL ),
        ENTRY( "CleanEnvironment", CMmMtpDataProviderTestModule::CleanEnvironmentL ),
        ENTRY( "OpenSession", CMmMtpDataProviderTestModule::OpenSessionL ),
        ENTRY( "CloseSession", CMmMtpDataProviderTestModule::CloseSessionL ),
        ENTRY( "ObjectEnumeration", CMmMtpDataProviderTestModule::ObjectEnumerationL ),
        ENTRY( "StorageEnumeration", CMmMtpDataProviderTestModule::StorageEnumerationL ),
        ENTRY( "StartTransaction", CMmMtpDataProviderTestModule::StartTransactionL ),
        ENTRY( "CancelTransaction", CMmMtpDataProviderTestModule::CancelTransactionL ),
        ENTRY( "CompleteTransaction", CMmMtpDataProviderTestModule::CompleteTransactionL ),
        ENTRY( "GetObjectInfo", CMmMtpDataProviderTestModule::GetObjectInfoL ),
        ENTRY( "GetObject", CMmMtpDataProviderTestModule::GetObjectL ),
        ENTRY( "GetObjectPropValue", CMmMtpDataProviderTestModule::GetObjectPropValueL ),
        ENTRY( "GetObjectPropList", CMmMtpDataProviderTestModule::GetObjectPropListL ),
        ENTRY( "GetObjectPropDesc", CMmMtpDataProviderTestModule::GetObjectPropDescL ),
        ENTRY( "SendObjectInfo", CMmMtpDataProviderTestModule::SendObjectInfoL ),
        ENTRY( "SendObject", CMmMtpDataProviderTestModule::SendObjectL ),
        ENTRY( "SetObjectPropValue", CMmMtpDataProviderTestModule::SetObjectPropValueL ),
        ENTRY( "SetObjectPropList", CMmMtpDataProviderTestModule::SetObjectPropListL ),
        ENTRY( "DeleteObject", CMmMtpDataProviderTestModule::DeleteObjectL ),
        ENTRY( "CopyObject", CMmMtpDataProviderTestModule::CopyObjectL ),
        ENTRY( "MoveObject", CMmMtpDataProviderTestModule::MoveObjectL ),
        ENTRY( "RenameObject", CMmMtpDataProviderTestModule::RenameObjectL ),
        ENTRY( "GetPartialObject", CMmMtpDataProviderTestModule::GetPartialObjectL ),
        ENTRY( "SetObjectReferences", CMmMtpDataProviderTestModule::SetObjectReferencesL ),
        ENTRY( "GetObjectReferences", CMmMtpDataProviderTestModule::GetObjectReferencesL ),
        ENTRY( "GetObjectPropsSupported", CMmMtpDataProviderTestModule::GetObjectPropsSupportedL)
        };
    
    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
    
    TInt result = RunInternalL( KFunctions, count, aItem );
    PRINTF1( "<CMmMtpDataProviderTestModule::RunMethodL, result = %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::LoadPluginL( CStifItemParser& aItem )
    {
    PRINTM0( "LoadPlugin" );
    
    TPtrC string;
    TInt result;
    
    result = aItem.GetNextString ( string );
    if ( result == KErrNone )
        {
        _LIT( KMediaDpString, "mediamtpdp" );
        _LIT( KAbstractMediaDpString, "abstractmediamtpdp" );
        const TInt KMediaDpUid = 0x10207C4B;
        const TInt KAbstractMediaDpUid = 0x10207C53;
        TInt uidValue = 0;
        
        if ( string.CompareF( KMediaDpString() ) == 0 )
            {
            PRINTM0( "Loading mediamtpdp plugin" );
            uidValue = KMediaDpUid;
            iPluginTester = CMediaMtpDataProviderTester::NewL();
            }
        else
            {
            if ( string.CompareF( KAbstractMediaDpString() ) == 0 )
                {
                PRINTM0( "Loading abstractmediamtpdp plugin" );
                uidValue = KAbstractMediaDpUid;
                iPluginTester = CAbstractMediaMtpDataProviderTester::NewL();
                }
            else
                {
                PRINTE0( "Unknown plugin" );
                User::Leave( KErrArgument );
                }
            }
        
        TUid pluginUid;
        pluginUid = TUid::Uid( uidValue );
        iDataProviderPlugin = CMTPDataProviderPlugin::NewL( pluginUid, iTestObserver );
        iPluginTester->RegisterObserver( iTestObserver, iTestConnChannel, iDataProviderPlugin );
        }
    else
        {
        PRINTE0( "Missing parameter: plugin name" );
        result = KErrArgument;
        }
    
    PRINTN1( "Exiting LoadPluginL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::PrepareEnvironmentL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "PrepareEnvironment" );
    
    TInt result = iPluginTester->PrepareEnvironmentL();
    if ( result != KErrNone )
        {
        PRINTE0( "Preparing environment failed" );
        }
    PRINTN1( "Exiting PrepareEnvironmentL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::CleanEnvironmentL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "CleanEnvironment" );
    
    TInt result = iPluginTester->CleanEnvironmentL();
    if ( result != KErrNone )
        {
        PRINTE0( "Cleaning environment failed" );
        }
    PRINTN1( "Exiting CleanEnvironmentL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::OpenSessionL ( CStifItemParser& aItem )
    {
    PRINTM0( "OpenSession" );
    
    TInt result = KErrNone;
    if ( iSessionOpen )
        {
        PRINTE0( "OpenSession failed, session already open" );
        result = KErrNotReady;
        }
    else
        {
        iSessionId++;
        TInt sessionId;
        result = aItem.GetNextInt ( sessionId );
        if ( result == KErrNone )
            {
            PRINTN1( "Got session id parameter: %d", sessionId );
            iSessionId = sessionId;
            }
        else
            {
            PRINTN1( "GetNextInt returned %d", result );
            }
        
        TMTPNotificationParamsSessionChange param = { iSessionId, *iTestConnChannel } ;
        if ( iPluginTester->OpenSessionL_prepare( param ) )
            {
            iDataProviderPlugin->ProcessNotificationL( EMTPSessionOpened, &param );
            result = iPluginTester->OpenSessionL_checkresults();
            if ( result == KErrNone )
                {
                iSessionOpen = ETrue;
                }
            else
                {
                PRINTE0( "OpenSession result check failed" );
                }
            }
        else
            {
            PRINTE0( "OpenSession prepare phase failed" );
            result = KErrGeneral;
            }
        }
    
    PRINTN1( "Exiting OpenSession with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::CloseSessionL ( CStifItemParser& aItem )
    {
    PRINTM0( "CloseSession" );
    
    TInt result = KErrNone;
    if ( !iSessionOpen )
        {
        PRINTE0( "CloseSession failed, session not open" );
        result = KErrNotReady;
        }
    else
        {
        TMTPNotificationParamsSessionChange param = { iSessionId, *iTestConnChannel };
        if ( iPluginTester->CloseSessionL_prepare( param ) )
            {
            iDataProviderPlugin->ProcessNotificationL( EMTPSessionClosed, &param );
            result = iPluginTester->CloseSessionL_checkresults();
            if ( result == KErrNone )
                {
                iSessionId = 0;
                iSessionOpen = EFalse;
                iTransactionId = 0; // start transaction numbering from beginning
                iTransactionOpen = EFalse;
                }
            else
                {
                PRINTE0( "CloseSession result check failed" );
                }
            }
        else 
            {
            PRINTE0( "CloseSession prepare phase failed" );
            result = KErrGeneral;
            }
        }
    PRINTN1( "Exiting CloseSession with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::ObjectEnumerationL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "ObjectEnumeration" );
    
    TInt result = KErrNotReady;
    
    TBool complete = iTestObserver->ObjectEnumerationComplete();
    if ( complete == EFalse )
        {
        CMTPStorageMetaData* physicalStorage( CMTPStorageMetaData::NewLC() );
        CMTPStorageMetaData* logicalStorage( CMTPStorageMetaData::NewLC() );
        if ( iPluginTester->ObjectEnumerationL_prepare( physicalStorage, logicalStorage ) ) 
            {
            TUint32 physicalStorageId;
            physicalStorageId = iTestObserver->StorageMgr().AllocatePhysicalStorageIdL( 0, *physicalStorage );
            physicalStorage->SetUint( CMTPStorageMetaData::EStorageId, physicalStorageId );
            TUint32 logicalStorageId;
            TRAP_IGNORE( logicalStorageId = iTestObserver->StorageMgr().AllocateLogicalStorageIdL( 0, physicalStorageId, *logicalStorage ) );
            logicalStorage->SetUint( CMTPStorageMetaData::EStorageId, logicalStorageId );
            iTestObserver->SetPendingOperation( CTestObserver::EObjectEnumeration );
            iDataProviderPlugin->StartObjectEnumerationL( KMTPStorageAll );
            iTestObserver->WaitForPendingOperation();
            result = iPluginTester->ObjectEnumerationL_checkresults( iTestObserver->ObjectMgr() );
            if ( result != KErrNone )
                {
                PRINTE0( "ObjectEnumeration result check failed" );
                }
            }
        else
            {
            PRINTE0 ( "ObjectEnumeration prepare phase failed" );
            result = KErrGeneral;
            }
        CleanupStack::PopAndDestroy( 2 ); // logicalStorage, physicalStorage
        }
     else
        {
        PRINTE0( "Object enumeration already run in this session" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting ObjectEnumerationL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::StorageEnumerationL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "StorageEnumeration" );
    
    TInt result = KErrNotReady;
    TBool complete = iTestObserver->StorageEnumerationComplete();
    if ( complete == EFalse )
         {
         if ( iPluginTester->StorageEnumerationL_prepare() )
             {
             iDataProviderPlugin->StartStorageEnumerationL();
             complete = iTestObserver->StorageEnumerationComplete();
             if (complete) 
                 {
                 result = KErrNone;
                 }
             }
         else 
             {
             PRINTE0( "StartStorageEnumeration prepare phase failed" );
             result = KErrGeneral;
             }
         }
     else 
         {
         PRINTE0( "Storage enumeration already run in this session" );
         result = KErrGeneral;
         }
     return result;
     } 

TInt CMmMtpDataProviderTestModule::StartTransactionL( CStifItemParser& aItem )
    {
    PRINTM0( "StartTransaction" );
    
    TInt result = KErrNone;
    if ( iSessionOpen )
        {
        if ( iTransactionOpen )
            {
            PRINTE0( "StartTransaction failed, transaction already open" );
            result = KErrGeneral;
            }
        else
            {
            if ( iRequest != NULL )
                {
                PRINTN0( "Delete previous request" );
                delete iRequest;
                iRequest = NULL;
                }
            iRequest = new TMTPTypeRequest();
            iTransactionId++;
            iTransactionOpen = ETrue;
            iTestObserver->SetPhaseL( EUndefined );
            }
        }
    else
        {
        PRINTE0( "StartTransaction failed, session not open" );
        result = KErrNotReady;
        }
    
    PRINTN1( "Exiting StartTransactionL with result %d", result );
    return result;
    }

// Notify that the current transaction has been cancelled.
// Affects:
// Response code of GetObject response phase
// Also default response code of other operations
// (EMTPRespCodeIncompleteTransfer)
// Outcome of SendObject/SendObjectInfo (should rollback)
TInt CMmMtpDataProviderTestModule::CancelTransactionL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "CancelTransaction" );
    
    TInt result = KErrNone;
    iTransactionOpen = EFalse;
    TMTPTypeEvent eventDataSet;
    eventDataSet.SetUint16( TMTPTypeEvent::EEventCode, EMTPEventCodeCancelTransaction );
    iDataProviderPlugin->ProcessEventL( eventDataSet, *iTestConnChannel );
    
    PRINTN1( "Exiting CancelTransactionL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::CompleteTransactionL( CStifItemParser& aItem )
    {
    PRINTM0( "CompleteTransaction" );
    
    TInt result = KErrNone;
    if ( iSessionOpen )
        {
        if ( !iTransactionOpen )
            {
            PRINTE0( "CompleteTransaction failed, transaction not open" );
            result = KErrNotReady;
            }
        else
            {
            iTransactionOpen = EFalse;
            TMTPTransactionPhase phase = iTestObserver->SetPhaseL( ECompletingPhase );
            iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
            }
        }
    else
        {
        PRINTE0( "CompleteTransaction failed, session not open" );
        result = KErrNotReady;
        }
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectInfoL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "GetObjectInfo" );
    
    if ( !iTransactionOpen )
        {
        PRINTE0( "Running GetObjectInfo without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = 2;
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetObjectInfo );
    
    if ( iPluginTester->GetObjectInfoL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "GetObjectInfo request failed" );
            PRINTE1( "Response code: 0x%x" , iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            result = iPluginTester->GetObjectInfoL_checkresults( static_cast<const CMTPTypeObjectInfo*> ( iTestObserver->iMMTPTypeData ) );
            if ( result != KErrNone )
                {
                PRINTE0( "GetObjectInfo result check failed" );
                }
            }
        }
    else
        {
        PRINTE0( "GetObjectInfo prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting GetObjectInfoL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "GetObject" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running GetObject without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;

    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = 2;
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, KMTPFormatsAll );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetObject );
    
    if ( iPluginTester->GetObjectL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "GetObjectInfo request failed" );
            PRINTE1( "Response code: 0x%x" , iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            result = iPluginTester->GetObjectL_checkresults( const_cast<CMTPTypeFile*>( static_cast<const CMTPTypeFile*>( iTestObserver->iMMTPTypeData ) ) );
            if ( result != KErrNone )
                {
                PRINTE0( "GetObject result check failed" );
                }
            }
        }
    else
        {
        result = KErrGeneral;
        PRINTE0( "GetObject prepare phase failed" );
        }
    
    PRINTN1( "Exiting GetObjectL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectPropValueL( CStifItemParser& aItem )
    {
    PRINTM0( "GetObjectPropValue" );
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    // get supported properties and test with them all
    RArray<TUint> properties;
    iDataProviderPlugin->Supported( EObjectProperties, properties );
    
    for ( TInt i = 0; i < properties.Count(); i++ )
        {
        switch ( properties[i] )
            {
            //case EMTPObjectPropCodeObjectFileName:  // 0xDC07
            case EMTPObjectPropCodeName: // 0xDC44
            case EMTPObjectPropCodeArtist: // 0xDC46
            //case EMTPObjectPropCodeGenre: // 0xDC8C
            //case EMTPObjectPropCodeParentalRating: // 0xDC94
            //case EMTPObjectPropCodeComposer: // 0xDC96
            //case EMTPObjectPropCodeOriginalReleaseDate: // 0xDC99
            case EMTPObjectPropCodeAlbumName: // 0xDC9A
            //case EMTPObjectPropCodeEncodingProfile: // 0xDEA1
            //case EMTPObjectPropCodeDescription:
            //case EMTPObjectPropCodeWidth: // 0xDC87
            //case EMTPObjectPropCodeHeight: // 0xDC88
            //case EMTPObjectPropCodeDuration: // 0xDC89
            //case EMTPObjectPropCodeUseCount: // 0xDC91
            //case EMTPObjectPropCodeSampleRate: // 0xDE93
            //case EMTPObjectPropCodeAudioWAVECodec: // 0xDE99
            //case EMTPObjectPropCodeAudioBitRate: // 0xDE9A
            //case EMTPObjectPropCodeVideoFourCCCodec: // 0xDE9B
            //case EMTPObjectPropCodeVideoBitRate: // 0xDE9C
            //case EMTPObjectPropCodeFramesPerThousandSeconds: // 0xDE9D
            //case EMTPObjectPropCodeKeyFrameDistance: // 0xDE9E
            //case EMTPObjectPropCodeTrack: // 0xDC8B
            //case EMTPObjectPropCodeDRMStatus: // 0xDC9D
            //case EMTPObjectPropCodeNumberOfChannels: // 0xDE94
            //case EMTPObjectPropCodeScanType: // 0xDE97
            //case EMTPExtObjectPropCodeOmaDrmStatus:
            result = StartTransactionL( aItem );
            if ( result != KErrNone )
                {
                break; // transaction cycle is broken, better to abort
                }
            iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
            iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
            TUint32 objectHandle = 2;
            iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
            iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, properties[i] );
            iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetObjectPropValue );
            
            if ( iPluginTester->GetObjectPropValueL_prepare( *iRequest ) )
                {
                phase = iTestObserver->SetPhaseL( ERequestPhase );
                PRINTV1( "Testing with property 0x%x" , properties[i] );
                iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
                phase = iTestObserver->SetPhaseL( EResponsePhase );
                iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
                if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
                    {
                    if ( iTestObserver->iResponseCode == EMTPRespCodeAccessDenied )
                        {
                        PRINTN1( "Could not access value for property 0x%x", properties[i] );
                        }
                    else
                        {
                        PRINTE0( "GetObjectPropValue request failed" );
                        PRINTE2( "Property code: 0x%x, response code: 0x%x", properties[i], iTestObserver->iResponseCode );
                        result = KErrGeneral;
                        }
                    }
                else
                    {
                    result = iPluginTester->GetObjectPropValueL_checkresults( iTestObserver->iMMTPTypeData, properties[i] );
                    if ( result != KErrNone )
                        {
                        PRINTE0( "GetObjectPropValue result check failed" );
                        }
                    }
                }
            else
                {
                PRINTE0( "GetObjectPropValue prepare phase failed" );
                result = KErrGeneral;
                }
            
            result = CompleteTransactionL( aItem );
            break;
            }
        // don't allow errors in any request
        if ( result != KErrNone )
            {
            break;
            }
        }
    properties.Close();
    PRINTN1( "Exiting GetObjectPropValueL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectPropListL( CStifItemParser& aItem )
    {
    PRINTM0( "GetObjectPropList" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running GetObjectPropList without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );  
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, KMTPHandleAll );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, KMTPFormatsAll );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter3, EMTPObjectPropCodeUndefined );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter4, 0x00FF0000 ); // mediadb group
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter5, 0 ); // depth
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetObjectPropList );
    
    if ( iPluginTester->GetObjectPropListL_prepare( *iRequest ) )
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iTestObserver->SetPendingOperation( CTestObserver::EGetObjectPropList );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        result = iTestObserver->WaitForPendingOperation();
        if ( result != KErrNone )
            {
            PRINTE0( "GetObjectPropList asynchronous operation failed" );
            }
        else
            {
            phase = iTestObserver->SetPhaseL( EResponsePhase );
            iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
            if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
                {
                PRINTE0( "GetObjectPropList request failed" );
                PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
                result = KErrGeneral;
                }
            else
                {
                result = iPluginTester->GetObjectPropListL_checkresults( static_cast<const CMTPTypeObjectPropList*>( iTestObserver->iMMTPTypeData ) );
                if ( result != KErrNone )
                    {
                    PRINTE0( "GetObjectPropList result check failed" );
                    }
                }
            }
        }
    else
        {
        PRINTE0( "GetObjectPropList prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting GetObjectPropList with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectPropDescL( CStifItemParser& aItem )
    {
    PRINTM0( "GetObjectPropDesc" );
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    // get supported properties and test with them all
    RArray<TUint> properties;
    iDataProviderPlugin->Supported( EObjectProperties, properties );
    
    for ( TInt i = 0; i < properties.Count(); i++ )
        {
        if ( properties[i] < 0xde99 ) // some problem with these
            {
            result = StartTransactionL( aItem );
            if ( result != KErrNone )
                {
                break; // transaction cycle is broken, better to abort
                }
            iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
            iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
            iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, KMTPFormatsAll );
            iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetObjectPropDesc );
            iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, properties[i] );
            TInt result = KErrNone;
            if ( iPluginTester->GetObjectPropDescL_prepare( *iRequest ) ) 
                {
                phase = iTestObserver->SetPhaseL( ERequestPhase );
                PRINTV1( "Testing with property 0x%x", properties[i] );
                iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
                phase = iTestObserver->SetPhaseL( EResponsePhase );
                iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
                if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
                    {
                    PRINTE0( "GetObjectPropDesc request failed")
                    PRINTE2( "Property code: 0x%x, response code: 0x%x", properties[i], iTestObserver->iResponseCode );
                    result = KErrGeneral;
                    }
                else
                    {
                    result = iPluginTester->GetObjectPropDescL_checkresults( static_cast<const CMTPTypeObjectPropDesc*>( iTestObserver->iMMTPTypeData ), properties[i] );
                    if ( result != KErrNone )
                        {
                        PRINTE0( "GetObjectPropDesc result check failed" );
                        }
                    }
                }
            else
                {
                PRINTE0( "GetObjectPropValue prepare phase failed" );
                }
            
            result = CompleteTransactionL( aItem );
            // don't allow errors in any request
            if ( result != KErrNone )
                {
                break;
                }
            }
        }
    properties.Close();
    
    PRINTN1( "Exiting GetObjectPropValueL with result %d", result );
    return result; 
    }

TInt CMmMtpDataProviderTestModule::SendObjectInfoL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "SendObjectInfo" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running SendObjectInfo without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;

    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = 0; // send to storage root
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, objectHandle );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeSendObjectInfo );
    
    if ( iPluginTester->SendObjectInfoL_prepare( *iRequest ) )
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "SendObjectInfo request failed" )
            PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            result = iPluginTester->SendObjectInfoL_checkresults();
            if ( result != KErrNone )
                {
                PRINTE0( "SendObjectInfo result check failed" );
                }
            }
        }
    else
        {
        PRINTE0( "SendObjectInfo prepare phase failed" );
        result = KErrGeneral;
        }

    PRINTN1( "Exiting SendObjectInfoL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::SendObjectL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "SendObject" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running SendObject without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    // the transaction id should be one greater than the id for send object info
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    // parameter 1 (storage id) is set in prepare function
    TUint32 objectHandle = 0; // send to storage root
    iRequest->SetUint32(TMTPTypeRequest::ERequestParameter2, objectHandle );
    iRequest->SetUint16(TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeSendObject );
    
    if ( iPluginTester->SendObjectL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
             {
             PRINTE0( "SendObject request failed" );
             PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
             result = KErrArgument;
             }
        else
            {
            result = iPluginTester->SendObjectL_checkresults();
            if ( result != KErrNone )
                {
                PRINTE0( "SendObject result check failed" );
                }
            }
        }
    else 
        {
        PRINTE0( "SendObject prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting SendObject with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::SetObjectPropValueL( CStifItemParser& aItem )
    {
    PRINTM0( "SetObjectPropValue" );
    
    TMTPTransactionPhase phase = ERequestPhase;
    TInt result = KErrNone;

    // get supported properties and test with them all
    RArray<TUint> properties;
    iDataProviderPlugin->Supported( EObjectProperties, properties );
    
    for ( TInt i = 0; i < properties.Count(); i++ )
        {
        switch ( properties[i] )
            {
            //case EMTPObjectPropCodeObjectFileName:  // 0xDC07
            case EMTPObjectPropCodeName: // 0xDC44
            case EMTPObjectPropCodeArtist: // 0xDC46
            //case EMTPObjectPropCodeGenre: // 0xDC8C
            //case EMTPObjectPropCodeParentalRating: // 0xDC94
            //case EMTPObjectPropCodeComposer: // 0xDC96
            //case EMTPObjectPropCodeOriginalReleaseDate: // 0xDC99
            case EMTPObjectPropCodeAlbumName: // 0xDC9A
            //case EMTPObjectPropCodeEncodingProfile: // 0xDEA1
            //case EMTPObjectPropCodeDescription:
            //case EMTPObjectPropCodeWidth: // 0xDC87
            //case EMTPObjectPropCodeHeight: // 0xDC88
            //case EMTPObjectPropCodeDuration: // 0xDC89
            //case EMTPObjectPropCodeUseCount: // 0xDC91
            //case EMTPObjectPropCodeSampleRate: // 0xDE93
            //case EMTPObjectPropCodeAudioWAVECodec: // 0xDE99
            //case EMTPObjectPropCodeAudioBitRate: // 0xDE9A
            //case EMTPObjectPropCodeVideoFourCCCodec: // 0xDE9B
            //case EMTPObjectPropCodeFramesPerThousandSeconds: // 0xDE9D
            //case EMTPObjectPropCodeKeyFrameDistance: // 0xDE9E
            //case EMTPObjectPropCodeTrack: // 0xDC8B
            //case EMTPObjectPropCodeDRMStatus: // 0xDC9D
            //case EMTPObjectPropCodeNumberOfChannels: // 0xDE94
            //case EMTPObjectPropCodeScanType: // 0xDE97
            //case EMTPExtObjectPropCodeOmaDrmStatus:
            result = StartTransactionL( aItem );
            if ( result != KErrNone )
                {
                break; // transaction cycle is broken, better to abort
                }
            iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
            iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
            TUint32 objectHandle = 2;
            iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
            iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeSetObjectPropValue );
            
            iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, properties[i] );
            TInt result = KErrNone;
            if ( iPluginTester->SetObjectPropValueL_prepare( *iRequest ) )
                {
                phase = iTestObserver->SetPhaseL( ERequestPhase );
                PRINTV1( "Testing with property 0x%x", properties[i] );
                iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
                phase = iTestObserver->SetPhaseL( EResponsePhase );
                iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );    
                if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
                    {
                    PRINTE0( "SetObjectPropValue request failed" );
                    PRINTE2( "Property code: 0x%x, response code: 0x%x", properties[i], iTestObserver->iResponseCode );
                    result = KErrGeneral;
                    }
                else
                    {
                    result = iPluginTester->SetObjectPropValueL_checkresults( static_cast<const CMTPTypeObjectInfo*> ( iTestObserver->iMMTPTypeData ) );
                    if ( result != KErrNone )
                        {
                        PRINTE0( "SetObjectPropValue result check failed" );
                        }
                    }
                }
            else
                {
                PRINTE0( "SetObjectPropValue prepare phase failed" );
                }
            
            result = CompleteTransactionL( aItem );
            break;
            }
        // don't allow errors in any request
        if ( result != KErrNone )
            {
            break;
            }
        }
    properties.Close();
    
    PRINTN1( "Exiting SetObjectPropValueL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::SetObjectPropListL( CStifItemParser& aItem )
    {
    PRINTM0( "SetObjectPropList" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running SetObjectPropList without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNotReady;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeSetObjectPropList );
    
    if ( iPluginTester->SetObjectPropListL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iTestObserver->SetPendingOperation( CTestObserver::ESetObjectPropList );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        result = iTestObserver->WaitForPendingOperation();
        if ( result != KErrNone )
            {
            PRINTE0( "SetObjectPropList asynchronous operation failed" );
            }
        else
            {
            if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
                {
                PRINTE0( "SetObjectPropList request failed" );
                PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
                result = KErrGeneral;
                }
            else
                {
                result = iPluginTester->SetObjectPropListL_checkresults( static_cast<const CMTPTypeObjectPropList*>( iTestObserver->iMMTPTypeData ) );
                if ( result != KErrNone )
                    {
                    PRINTE0( "SetObjectPropList result check failed" );
                    result = KErrGeneral;
                    }
                }
            }
        }
    else
        {
        PRINTE0( "SetObjectPropList prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting SetObjectPropList with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::DeleteObjectL( CStifItemParser& aItem )
    {
    PRINTM0( "DeleteObject" );
    
    if ( !iTransactionOpen )
        {
        PRINTE0( "Running DeleteObject without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNotReady;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = KMTPHandleAll;
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeDeleteObject );
    
    if ( iPluginTester->DeleteObjectL_prepare( *iRequest ) ) 
        {
        iTestObserver->SetPendingOperation( CTestObserver::EDeleteObject );
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        result = iTestObserver->WaitForPendingOperation();
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "DeleteObject request failed" );
            PRINTE1( "Response code 0x%x", iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            result = iPluginTester->DeleteObjectL_checkresults( iTestObserver->ObjectMgr() );
            if ( result != KErrNone )
                {
                PRINTE0( "DeleteObject result check failed" );
                }
            }
        }
    else
        {
        PRINTE0( "DeleteObject prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting TestDeleteObjectL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::CopyObjectL( CStifItemParser& aItem )
    {
    PRINTM0( "CopyObject" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running CopyObject without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = 3;
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
    // parameter 2 (storage id) set in prepare function
    TUint32 parentObjectHandle = 0; // copy to root directory
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter3, parentObjectHandle );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeCopyObject );
    
    if ( iPluginTester->CopyObjectL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            __TRACE( KError, ( _L( "CopyObject request phase failed, response code: 0x%x" ), iTestObserver->iResponseCode ) );
            result = KErrGeneral;
            }
        else
            {
            TUint32 handle = iTestObserver->iResponse.Uint32( TMTPTypeResponse::EResponseParameter1 );
            PRINTV1( "Copied object handle: %d", handle );
            CMTPObjectMetaData* metaData = CMTPObjectMetaData::NewLC();
            iTestObserver->ObjectMgr().ObjectL( handle, *metaData );
            result = iPluginTester->CopyObjectL_checkresults( metaData );
            if ( result != KErrNone )
                {
                PRINTE0( "CopyObject result check failed" );
                }
            CleanupStack::PopAndDestroy( metaData );
            }
        }
    else
        {
        PRINTE0( "CopyObject prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting CopyObject with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::MoveObjectL( CStifItemParser& aItem )
    {
    PRINTM0( "MoveObject" );

    if ( !iTransactionOpen )
        {
        PRINTE0( "Running MoveObject without open transaction" );
        return KErrGeneral;
        }
    
    TInt result = KErrNotReady; 
    TMTPTransactionPhase phase;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    // the transaction id should be one greater than the id for send object info
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = 3;
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
    // parameter 2 (storage id) is set in prepare function
    TUint32 parentHandle = 0; // move to root directory
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter3, parentHandle );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeMoveObject );
    
    if ( iPluginTester->MoveObjectL_prepare( *iRequest ) )
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "MoveObject request failed" );
            PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            CMTPObjectMetaData* metaData = CMTPObjectMetaData::NewLC();
            iTestObserver->ObjectMgr().ObjectL( objectHandle, *metaData );
            result = iPluginTester->MoveObjectL_checkresults( metaData );
            if ( result != KErrNone )
                {
                PRINTE0( "MoveObject result check failed" );
                }
            CleanupStack::PopAndDestroy( metaData );
            }
        }
    else
        {
        PRINTE0( "MoveObject prepare failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting MoveObject with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::RenameObjectL( CStifItemParser& aItem )
    {
    PRINTM0( "RenameObject" );
    
    TInt result = KErrNotReady;    
    TMTPNotificationParamsHandle param;
    
    if ( iPluginTester->RenameObjectL_prepare( param ) ) 
        {
        TUint32 handle = param.iHandleId;
        iDataProviderPlugin->ProcessNotificationL( EMTPRenameObject, &param );
        User::After( 10000 );
        CMTPObjectMetaData* metaData = CMTPObjectMetaData::NewLC();
        iTestObserver->ObjectMgr().ObjectL( handle, *metaData );
        result = iPluginTester->RenameObjectL_checkresults( metaData );
        if ( result != KErrNone )
            {
            __TRACE( KError, ( _L( "Result check failed with error %d" ), result ) );
            }
        CleanupStack::PopAndDestroy( metaData );
        }
    else
        {
        PRINTE0( "RenameObject prepare phase failed" );
        result = KErrGeneral;
        }
    
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetPartialObjectL( CStifItemParser& aItem )
    {
    PRINTM0( "GetPartialObject" );
    
    if ( !iTransactionOpen )
        {
        PRINTE0( "Running GetPartialObject without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    TUint32 objectHandle = 2; // first enumerated object
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, objectHandle );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter2, 0 );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter3, KMTPHandleAll );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetPartialObject );
        
    if ( iPluginTester->GetPartialObjectL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "GetPartialObject request failed" );
            PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            result = iPluginTester->GetPartialObjectL_checkresults(); 
            if ( result != KErrNone ) 
                {
                PRINTE0( "GetPartialObject result check failed" );
                }
            }
        }
    else
        {
        PRINTE0( "GetPartialObject prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting GetPartialObjectL with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::SetObjectReferencesL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "SetObjectReferences" );
    TInt result = KErrNone;
    PRINTN1( "Exiting SetObjectReferences with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectReferencesL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "GetObjectReferences" );
    TInt result = KErrNone;
    PRINTN1( "Exiting GetObjectReferences with result %d", result );
    return result;
    }

TInt CMmMtpDataProviderTestModule::GetObjectPropsSupportedL( CStifItemParser& /*aItem*/ )
    {
    PRINTM0( "GetObjectPropsSupported" );
    
    if ( !iTransactionOpen )
        {
        PRINTE0( "Running GetObjectPropsSupported without open transaction" );
        return KErrGeneral;
        }
    
    TMTPTransactionPhase phase;
    TInt result = KErrNone;
    
    iRequest->SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestTransactionID, iTransactionId );
    iRequest->SetUint32( TMTPTypeRequest::ERequestParameter1, EMTPFormatCodeMP3 );
    iRequest->SetUint16( TMTPTypeRequest::ERequestOperationCode, EMTPOpCodeGetObjectPropsSupported );
    
    if ( iPluginTester->GetObjectPropsSupportedL_prepare( *iRequest ) ) 
        {
        phase = iTestObserver->SetPhaseL( ERequestPhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        phase = iTestObserver->SetPhaseL( EResponsePhase );
        iDataProviderPlugin->ProcessRequestPhaseL( phase, *iRequest, *iTestConnChannel );
        if ( iTestObserver->iResponseCode != EMTPRespCodeOK )
            {
            PRINTE0( "GetObjectPropsSupported request failed" );
            PRINTE1( "Response code: 0x%x", iTestObserver->iResponseCode );
            result = KErrGeneral;
            }
        else
            {
            result = iPluginTester->GetObjectPropsSupportedL_checkresults( static_cast<const CMTPTypeArray*>( iTestObserver->iMMTPTypeData ) );
            if ( result != KErrNone )
                {
                PRINTE0( "GetObjectPropsSupported result check failed" );
                }
            }
        }
    else
        {
        PRINTE0( "GetObjectPropsSupported prepare phase failed" );
        result = KErrGeneral;
        }
    
    PRINTN1( "Exiting GetObjectPropsSupportedL with result %d", result );
    return result;
    }
