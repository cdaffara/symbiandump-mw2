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
* Description:  Test observer to act as dummy mtp framework
*
*/


#include "ctestobserver.h"
#include "cmmmtpdataprovidertestmodule.h"
#include "cmmmtpdataprovidertester.h"
#include <mtp/tmtptypeevent.h>
#include <mtp/tmtptyperesponse.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/mmtpdataprovider.h>
#include <mtp/cmtptypeobjectinfo.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypefile.h>
#include <barsc.h>
#include <barsread.h>
#include <stiflogger.h>
#include <settingserverclient.h>
#include <mpxmedia.h>
#include "logging.h"

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog

_LIT( KResFileName, "Z:\\resource\\mmmtpdataprovidertester.rsc" );

const TInt KDataProviderId = 1;

CTestObserver::CTestObserver( CMmMtpDataProviderTestModule* aTestModule )
        : iObjectEnumerationComplete( EFalse ),
          iStorageEnumerationComplete( EFalse ),
          iTestModule( aTestModule ) 
    {
    // no implementation required
    }

CTestObserver::~CTestObserver()
    {
    PRINTF0( ">CTestObserver::~CTestObserver" );
    iFsSession.Close();
    delete iStorageManager;
    delete iTestObjManager;
    delete iActiveSchedulerWait;
    PRINTF0( "<CTestObserver::~CTestObserver" );
    delete iLog;
    }

CTestObserver* CTestObserver::NewLC( CMmMtpDataProviderTestModule* aTestModule ) 
    { 
    CTestObserver* self = new ( ELeave ) CTestObserver( aTestModule );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CTestObserver* CTestObserver::NewL( CMmMtpDataProviderTestModule* aTestModule ) 
    { 
    CTestObserver* self = CTestObserver::NewLC( aTestModule );
    CleanupStack::Pop();
    return self;
    }

void CTestObserver::ConstructL() 
    {
    TFileName logFileName;  
    logFileName.Copy( KMtpDataProviderTestModuleLogFile );
    
    iLog = CStifLogger::NewL( KMtpDataProviderTestModuleLogPath, 
            logFileName,
            CStifLogger::ETxt,
            TRACE_TARGET,
            ETrue, ETrue, ETrue, EFalse, ETrue );
    
    PRINTF0( ">CTestObserver::ConstructL" );
    iStorageManager = CTestStorageMgr::NewL();
    iTestObjManager = CTestObjectManager::NewL();
    iFsSession.Connect();
    iActiveSchedulerWait = new ( ELeave ) CActiveSchedulerWait();
    
    RResourceFile resFile;
    CleanupClosePushL( resFile );
    
    resFile.OpenL( Fs(), KResFileName );
    const TInt KDefaultResourceId( 1 );
    HBufC8* resBuffer( resFile.AllocReadLC( KDefaultResourceId ) );
    TResourceReader resReader;
    resReader.SetBuffer( resBuffer );
    
    // WORD type
    TInt temp;
    HBufC* tempBuf;
    temp = resReader.ReadUint16(); // read and discard type
    temp = resReader.ReadInt8(); // read and discard version
    temp = resReader.ReadInt8(); // read and discard enumeration persistent flag
    temp = resReader.ReadUint16(); // read and discard supported modes 

    tempBuf = resReader.ReadHBufCL(); // read server name
    delete tempBuf;
    tempBuf = resReader.ReadHBufCL(); // read server image name
    delete tempBuf;

    // LLINK opaque_resource
    iResourceId = resReader.ReadUint32();
    __TRACE( KPrint, ( _L( "Resource id = 0x%x" ), iResourceId ) );
    temp = resReader.ReadUint8(); // read and discard enumeration phase
    temp = temp; // prevent warning
    CleanupStack::PopAndDestroy( resBuffer );
    CleanupStack::PopAndDestroy( &resFile );
    
    PRINTF0( "<CTestObserver::ConstructL" );
    }

TUint CTestObserver::DataProviderId() const
    {
    TUint id = KDataProviderId;
    PRINTF1( "<>CTestObserver::DataProviderId id = %d", id );
    return id;
    }

TMTPOperationalMode CTestObserver::Mode() const
    {
    TMTPOperationalMode mode = EModeMTP;
    PRINTF1( "<>CTestObserver::Mode mode = %d", mode );
    return EModeMTP;
    }

void CTestObserver::ReceiveDataL( MMTPType& aData,
        const TMTPTypeRequest& aRequest, MMTPConnection& /*aConnection*/ )
    {
    TInt requestCode = aRequest.Uint16( TMTPTypeRequest::ERequestOperationCode );
    TInt sessionId = aRequest.Uint32( TMTPTypeRequest::ERequestSessionID );
    
    PRINTF2( ">CTestObserver::ReceiveDataL request = 0x%x session = %d", requestCode, sessionId );
    
    SetPhaseL( EDataIToRPhase );
    
    TUint type = aData.Type();
    PRINTV1( "Type of data: 0x%x", type );
    if ( type == EMTPTypeUndefined )
        {
        PRINTN0( "Null data received, executing dummy data phase" );
        return;
        }
    
    switch ( requestCode ) 
        {
        case EMTPOpCodeSendObjectInfo:
            {
            PRINTN0( "Receiving data for SendObjectInfo" );
            if ( type != EMTPTypeObjectInfoDataset )
                {
                PRINTE2( "Wrong data type received: 0x%x != 0x%x", type, EMTPTypeObjectInfoDataset );
                User::Leave( KErrArgument );
                }
            iTestModule->iPluginTester->SendObjectInfoReceiveData(
                    static_cast <CMTPTypeObjectInfo*>( &aData ) );
            }
            break;
        case EMTPOpCodeSendObject:
            {
            PRINTN0( "Receiving data for SendObject" );
            if ( type != EMTPTypeFile )
                {
                PRINTE2( "Wrong data type received: 0x%x != 0x%x", type,
                        EMTPTypeFile );
                User::Leave( KErrArgument );
                }
            iTestModule->iPluginTester->SendObjectReceiveDataL(
                    static_cast<CMTPTypeFile*>( &aData ) );
            }
            break;
        case EMTPOpCodeSetObjectPropList:
            {
            PRINTN0( "Receiving data for SetObjectPropList" );
            if ( type != EMTPTypeObjectPropListDataset )
                {
                PRINTE2( "Wrong data type received: 0x%x != 0x%x", type, EMTPTypeObjectPropListDataset );
                User::Leave( KErrArgument );
                }
            iTestModule->iPluginTester->SetObjectPropListReceiveDataL(
                    static_cast <CMTPTypeObjectPropList* >( &aData ) );
            }
            break;
        case EMTPOpCodeSetObjectPropValue:
            {
            PRINTN0( "Receiving data for SetObjectPropValue" );
            iTestModule->iPluginTester->SetObjectPropValueReceiveDataL(
                    aRequest, &aData );
            }
            break;
        }
    PRINTF0( "<CTestObserver::ReceiveDataL" );
    }

void CTestObserver::SendDataL(const MMTPType& aData, 
        const TMTPTypeRequest& aRequest, MMTPConnection& /*aConnection*/)
    {
    TUint16 operationCode = aRequest.Uint16( TMTPTypeRequest::ERequestOperationCode );
    PRINTF1( ">CTestObserver::SendDataL operation = 0x%x", operationCode );
    SetPhaseL( EDataRToIPhase );
    iMMTPTypeData = &aData;
    switch ( operationCode ) 
        {
        case EMTPOpCodeGetObjectPropList:
            if ( iPendingOperation == EGetObjectPropList )
                {
                PRINTN0( "GetObjectPropList async operation finished successfully" );
                iAsyncResult = KErrNone;
                }
            else
                {
                PRINTE0( "GetObjectPropList async operation finished in incorrect state" );
                iAsyncResult = KErrNotReady;
                }
            if ( iActiveSchedulerWait->IsStarted() )
                {
                iActiveSchedulerWait->AsyncStop();
                }
            iPendingOperation = EIdleOperation;
            break;
        }
    PRINTF0( "<CTestObserver::SendDataL" );
    }

void CTestObserver::SendEventL(
        const TMTPTypeEvent& aEvent, MMTPConnection& /*aConnection*/ )
    {
    TUint16 eventCode = aEvent.Uint16( TMTPTypeEvent::EEventCode );
    TUint32 sessionId = aEvent.Uint32( TMTPTypeEvent::EEventSessionID );
    PRINTF2( "<>CTestObserver::SendEventL event = 0x%x session = %d", eventCode, sessionId );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObserver::SendEventL( const TMTPTypeEvent& aEvent )
    {
    TUint16 eventCode = aEvent.Uint16( TMTPTypeEvent::EEventCode );
    TUint32 sessionId = aEvent.Uint32( TMTPTypeEvent::EEventSessionID );
    PRINTF2( "<>CTestObserver::SendEventL event = 0x%x session = %d", eventCode, sessionId );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObserver::SendResponseL(
        const TMTPTypeResponse& aResponse,
        const TMTPTypeRequest& aRequest,
        MMTPConnection& /*aConnection*/ )
    {
    TUint16 response = aResponse.Uint16( TMTPTypeResponse::EResponseCode );
    TUint16 operationCode = aRequest.Uint16( TMTPTypeRequest::ERequestOperationCode );
    PRINTF2( ">CTestObserver::SendResponseL response = 0x%x operation = 0x%x", response, operationCode );
    TInt result = KErrNone;
    if ( iPhase == ECompletingPhase )
        {
        PRINTN1( "Response 0x%x received while already in completing phase", response );
        }
    else
        {
        SetPhaseL( ECompletingPhase );
        TMTPTypeResponse::CopyL( aResponse, iResponse );
        iResponseCode = response;
        switch ( operationCode ) 
            {
            case EMTPOpCodeGetObjectPropList:
                if ( iPendingOperation == EGetObjectPropList )
                    {
                    PRINTN0( "GetObjectPropList async operation aborted" );
                    iAsyncResult = KErrAbort;
                    if ( iActiveSchedulerWait->IsStarted() )
                        {
                        iActiveSchedulerWait->AsyncStop();
                        }
                    iPendingOperation = EIdleOperation;
                    }
                break;
            case EMTPOpCodeSetObjectPropList:
                if ( iPendingOperation == ESetObjectPropList )
                    {
                    PRINTN0( "SetObjectPropList async operation finished successfully" );
                    iAsyncResult = KErrNone;
                    }
                else
                    {
                    PRINTE0( "SetObjectPropList async operation finished in incorrect state" );
                    iAsyncResult = KErrNotReady;
                    }
                if ( iActiveSchedulerWait->IsStarted() )
                    {
                    iActiveSchedulerWait->AsyncStop();
                    }
                iPendingOperation = EIdleOperation;
                break;
            case EMTPOpCodeDeleteObject:
                if ( iPendingOperation == EDeleteObject )
                    {
                    PRINTN0( "DeleteObject async operation finished successfully" );
                    iAsyncResult = KErrNone;
                    }
                else
                    {
                    PRINTE0( "DeleteObject async operation finished in incorrect state" );
                    iAsyncResult = KErrNotReady;
                    }
                if ( iActiveSchedulerWait->IsStarted() )
                    {
                    iActiveSchedulerWait->AsyncStop();
                    }
                iPendingOperation = EIdleOperation;
                break;
            }
        }
    PRINTF0( "<CTestObserver::SendResponseL" );
    }

void CTestObserver::TransactionCompleteL(const TMTPTypeRequest& /*aRequest*/,
        MMTPConnection& /*aConnection*/)
    {
    PRINTF0( "<>CTestObserver::TransactionCompleteL" );
    SetPhaseL( EIdlePhase );
    }

void CTestObserver::RouteRequestRegisterL(const TMTPTypeRequest& /*aRequest*/,
        MMTPConnection& /*aConnection*/)
    {
    PRINTF0( "<>CTestObserver::RouteRequestRegisterL" );
    }

void CTestObserver::RouteRequestUnregisterL(const TMTPTypeRequest& /*aRequest*/,
        MMTPConnection& /*aConnection*/)
    {
    PRINTF0( "<>CTestObserver::RouteRequestUnregisterL" );
    }

void CTestObserver::ObjectEnumerationCompleteL( TUint32 aStorageId )
    {
    PRINTF1( ">CTestObserver::ObjectEnumerationCompleteL aStorageId = 0x%x", aStorageId );
    
    if ( iPendingOperation == EObjectEnumeration )
        {
        if ( iActiveSchedulerWait->IsStarted() )
            {
            iActiveSchedulerWait->AsyncStop();
            }
        iAsyncResult = KErrNone;
        iObjectEnumerationComplete = ETrue;
        iPendingOperation = EIdleOperation;
        }
    else
        {
        PRINTE0( "Unexpected pending operation" );
        iObjectEnumerationComplete = EFalse;
        iAsyncResult = KErrNotReady;
        }
    PRINTF0( "<CTestObserver::ObjectEnumerationCompleteL" );
    }

void CTestObserver::StorageEnumerationCompleteL()
    {
    PRINTF0( ">CTestObserver::StorageEnumerationCompleteL" );
    if ( iPendingOperation == EStorageEnumeration )
        {
        if ( iActiveSchedulerWait->IsStarted() )
            {
            iActiveSchedulerWait->AsyncStop();
            }
        iAsyncResult = KErrNone;
        iStorageEnumerationComplete = ETrue;
        iPendingOperation = EIdleOperation;
        }
    else
        {
        PRINTE0( "Unexpected pending operation" );
        iStorageEnumerationComplete = EFalse;
        iAsyncResult = KErrNotReady;
        }
    PRINTF0( "<CTestObserver::StorageEnumerationCompleteL" );
    }

TBool CTestObserver::ObjectEnumerationComplete() const
    {
    PRINTF1( "<>CTestObserver::ObjectEnumerationComplete = %d", iObjectEnumerationComplete );
    return iObjectEnumerationComplete;
    }

TInt CTestObserver::ObjectEnumerationError() const
    {
    PRINTF1( "<>CTestObserver::ObjectEnumerationError = %d", iObjectEnumerationError );
    return iObjectEnumerationError;
    }

TBool CTestObserver::StorageEnumerationComplete() const
    {
    PRINTF1( "<>CTestObserver::ObjectEnumerationError = %d", iStorageEnumerationComplete );
    return iStorageEnumerationComplete;
    }

// sets the phase member variable and leaves if state transition is illegal
TMTPTransactionPhase CTestObserver::SetPhaseL( TMTPTransactionPhase aPhase )
    {
    PRINTF1( ">CTestObserver::SetPhaseL aPhase = 0x%x", aPhase )
    TMTPTransactionPhase result = iPhase;
    switch ( aPhase )
        {
        case EUndefined:
            iPhase = aPhase;
            result = iPhase;
            break;
        case EIdlePhase:
            if ( iPhase == ECompletingPhase )
                {
                iPhase = aPhase;
                result = iPhase;
                }
            else
                {
                PRINTE0( "Illegal attempt to switch to idle phase" );
                PRINTE1( "Current phase: 0x%x", iPhase );
                User::Leave( KErrNotReady );
                }
            break;
        case ERequestPhase:
            if ( iPhase == EUndefined )
                {
                iPhase = aPhase;
                result = iPhase;
                }
            else
                {
                PRINTE0( "Illegal attempt to switch to request phase" );
                PRINTE1( "Current phase: 0x%x", iPhase );
                User::Leave( KErrNotReady );
                }
            break;
        case EResponsePhase:
            switch ( iPhase )
                {
                case ECompletingPhase:
                    PRINTN0( "Attempted to switch back to response phase from completing phase" );
                    // while already in completing phase, must not change internal state
                    result = aPhase;
                    break;
                case ERequestPhase:
                case EDataIToRPhase:
                case EDataRToIPhase:
                    iPhase = aPhase;
                    result = iPhase;
                    break;
                default:
                    PRINTE0( "Illegal attempt to switch to response phase" );
                    PRINTE1( "Current phase: 0x%x", iPhase );
                    User::Leave( KErrNotReady );
                    break;
                }
            break;
        case ECompletingPhase:
            switch ( iPhase )
                {
                case ECompletingPhase:
                    // allow switch but print notice
                    PRINTN0( "Attempted to switch to completing phase while already in completing phase" );
                    result = iPhase;
                    break;
                case EIdlePhase:
                    // some operations do automatically the completing phase
                    // allow switch but don't change internal phase
                    PRINTN0( "Attempted to switch to completing phase while already in idle phase" );
                    result = aPhase;
                    break;
                case ERequestPhase:
                case EDataIToRPhase:
                case EDataRToIPhase:
                case EResponsePhase:
                    iPhase = aPhase;
                    result = iPhase;
                    break;
                default:
                    PRINTE0( "Illegal attempt to switch to completing phase" );
                    PRINTE1( "Current phase: 0x%x", iPhase );
                    User::Leave( KErrNotReady );
                    break;
                }
            break;
        case EDataIToRPhase:
            if ( iPhase == ERequestPhase )
                {
                iPhase = aPhase;
                result = iPhase;
                }
            else
                {
                PRINTE0( "Illegal attempt to switch to data i to r phase" );
                PRINTE1( "Current phase: 0x%x", iPhase );
                User::Leave( KErrNotReady );
                }
            break;
        case EDataRToIPhase:
            if ( iPhase == ERequestPhase )
                {
                iPhase = aPhase;
                result = iPhase;
                }
            else
                {
                PRINTE0( "Illegal attempt to switch to data r to i phase" );
                PRINTE1( "Current phase: 0x%x", iPhase );
                User::Leave( KErrNotReady );
                }
            break;
        default:
            PRINTE0( "Attempting to switch to illegal phase" );
            PRINTE1( "Tried to change to phase: 0x%x", aPhase );
            User::Leave( KErrArgument );
            break;
        }
    PRINTF1( "<CTestObserver::SetPhaseL result = %d", result );
    return result;
    }

const MMTPDataProviderConfig& CTestObserver::DataProviderConfig() const
    {
    PRINTF0( "<>CTestObserver::DataProviderConfig" );
    return *this;
    }

const MMTPFrameworkConfig& CTestObserver::FrameworkConfig() const
    {
    PRINTF0( "<>CTestObserver::FrameworkConfig" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

 
MMTPObjectMgr& CTestObserver::ObjectMgr() const
     {
     PRINTF0( "<>CTestObserver::ObjectMgr" );
     return *iTestObjManager;
     }

MMTPReferenceMgr& CTestObserver::ReferenceMgr() const
    {
    PRINTF0( "<>CTestObserver::ReferenceMgr()" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

MMTPStorageMgr& CTestObserver::StorageMgr() const
    {
    PRINTF0( "<>CTestObserver::StorageMgr" );
    return *iStorageManager;
    }
 

RFs& CTestObserver::Fs() const
    {
    PRINTF0( "<>CTestObserver::Fs()" );
    return const_cast<RFs&>(iFsSession);
    }

void CTestObserver::SetPendingOperation( TAsyncOperation aOperation )
    {
    PRINTF1( "<>CTestObserver::SetPendingOperation aOperation = %d", aOperation );
    iPendingOperation = aOperation;
    }

TInt CTestObserver::WaitForPendingOperation()
    {
    PRINTF0( ">CTestObserver::WaitForPendingOperation" );
    if ( iPendingOperation != EIdleOperation )
        {
        if ( iActiveSchedulerWait->IsStarted() )
            {
            PRINTE0( "Active scheduler wait already started" );
            iAsyncResult = KErrNotReady;
            }
        else
            {
            iActiveSchedulerWait->Start();
            }
        }
    else
        {
        PRINTN0( "Pending operation already finished" );
        }
    PRINTF1( "<CTestObserver::WaitForPendingOperation result = %d", iAsyncResult );
    return iAsyncResult;
    }

TInt CTestObserver::GetPendingResult()
    {
    PRINTF1( "<>CTestObserver::GetPendingResult = %d", iAsyncResult );
    return iAsyncResult;
    }

void CTestObserver::HandleAbstractMediaExportCompletedL( CMPXMedia* /*aMedia*/, TInt /*aErr*/ )
    {
    PRINTF0( "<>CTestObserver::GetPendingResult" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObserver::HandleAbstractMediaImportCompletedL( CMPXMedia* aMedia, TInt aErr )
    {
    PRINTF0( ">CTestObserver::HandleAbstractMediaImportCompletedL" );
    if ( iPendingOperation == EHarvesterImportAbstractMedia )
        {
        // TODO: add check function for the harvest results using this media object
        if ( aMedia != NULL )
            {
            delete aMedia;
            }
        iAsyncResult = aErr;
        }
    else
        {
        PRINTE0( "HarvesterImportPlaylist async operation finished in incorrect state" );
        iAsyncResult = KErrNotReady;
        }
    if ( iActiveSchedulerWait->IsStarted() )
        {
        iActiveSchedulerWait->AsyncStop();
        }
    PRINTF0( "<CTestObserver::HandleAbstractMediaImportCompletedL" );
    }

void CTestObserver::HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr )
    {
    PRINTF1( ">CTestObserver::HandleFileAddCompletedL aErr = %d", aErr );
    if ( iPendingOperation == EHarvesterAddObject )
        {
        // TODO: add check function for the harvest results using this media object
        if ( aMedia != NULL )
            {
            delete aMedia;
            }
        iAsyncResult = aErr;
        }
    else
        {
        PRINTE0( "HarvesterAddObject async operation finished in incorrect state" );
        iAsyncResult = KErrNotReady;
        }
    if ( iActiveSchedulerWait->IsStarted() )
        {
        iActiveSchedulerWait->AsyncStop();
        }
    PRINTF0( "<CTestObserver::HandleFileAddCompletedL" );
    }

void CTestObserver::HandleDeleteCompletedL( TInt aErr )
    {
    PRINTF1( ">CTestObserver::HandleDeleteCompletedL aErr = %d", aErr );
    if ( iPendingOperation == EHarvesterDeleteObject )
        {
        iAsyncResult = aErr;
        }
    else
        {
        PRINTE0( "HarvesterDeleteObject async operation finished in incorrect state" );
        iAsyncResult = KErrNotReady;
        }
    if ( iActiveSchedulerWait->IsStarted() )
        {
        iActiveSchedulerWait->AsyncStop();
        }
    PRINTF0( "<CTestObserver::HandleDeleteCompletedL" );
    }

void CTestObserver::HandleFileImportCompletedL( CMPXMedia* /*aMedia*/, TInt /*aErr*/  )
    {
    PRINTF0( "<>CTestObserver::HandleFileImportCompletedL" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

void CTestObserver::HandleFileGetMediaCompletedL( CMPXMedia* /*aMedia*/, TInt /*aErr*/ )
    {
    PRINTF0( "<>CTestObserver::HandleFileGetMediaCompletedL" );
    PRINTE0( "NOT IMPLEMENTED" );
    }

TUint CTestObserver::UintValue( MMTPDataProviderConfig::TParameter aParam ) const
    {
    PRINTF1( ">CTestObserver::UintValue aParam = %d", aParam );
    TUint result = 0;
    switch ( aParam )
        {
        case EOpaqueResource:
            result = iResourceId;
            break;
        default:
            PRINTE1( "UintValue called with unexpected param: 0x%x", aParam );
        }
    PRINTF1( "<CTestObserver::UintValue result = %d", result );
    return result;
    }

TBool CTestObserver::BoolValue( MMTPDataProviderConfig::TParameter aParam ) const
    {
    PRINTF1( ">CTestObserver::BoolValue aParam = %d", aParam );
    TBool result = EFalse;
    PRINTF1( "<CTestObserver::BoolValue result = %d", result );
    return result;
    }

const TDesC& CTestObserver::DesCValue( MMTPDataProviderConfig::TParameter aParam ) const
    {
    PRINTF1( ">CTestObserver::DesCValue aParam = %d", aParam );
    const TDesC* nullValue( &KNullDesC );
    const TDesC* resValue( &KResFileName );
    switch ( aParam )
        {
        case EResourceFileName:
            PRINTF1( "<CTestObserver::DesCValue result = %S", resValue );
            return *resValue;
        default:
            PRINTE1( "DesCValue called with unexpected param: 0x%x", aParam );
            User::Leave( KErrArgument );
        }
    }

const TDesC8& CTestObserver::DesC8Value( MMTPDataProviderConfig::TParameter aParam ) const
    {
    PRINTF1( "<>CTestObserver::DesC8Value aParam = %d", aParam );
    return KNullDesC8;
    }

void CTestObserver::GetArrayValue( MMTPDataProviderConfig::TParameter aParam, RArray<TUint>& aArray ) const
    {
    CleanupClosePushL( aArray ); 
    PRINTF1( ">CTestObserver::GetArrayValue aParam = %d", aParam );
    switch (aParam)
        {
        case ESupportedModes:
            {
            PRINTV0( "Appending MTP mode" );
            aArray.Append(EModeMTP);
            break;
            }
        default:
            PRINTE1( "GetArrayValue called with unexpected param: 0x%x", aParam );
            User::Leave( KErrArgument );
        }
    PRINTF0( "<CTestObserver::GetArrayValue" );
    CleanupStack::Pop(); 
    }
