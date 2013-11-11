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


#ifndef CTESTOBSERVER_H
#define CTESTOBSERVER_H

#include <stiflogger.h>
#include <mtp/mmtpdataproviderframework.h> 
#include <mtp/mmtpdataproviderconfig.h>
#include <mtp/tmtptyperesponse.h>
#include <f32file.h>
#include <mpxmedia.h>
#include <mpxharvesterutilityobserver.h>
#include "ctestobjectmanager.h"
#include "cteststoragemgr.h"

#if defined(_UNICODE)
#define CASTING(a) ((TText)L ## a)
#else
#define CASTING(a) ((TText)(a))
#endif

class CMmMtpDataProviderTestModule;

class CTestObserver : public MMTPDataProviderFramework, public MMTPDataProviderConfig,
        public MMPXHarvesterUtilityObserver, public CBase
    {
public:
    enum TAsyncOperation
        {
        EIdleOperation,
        EStorageEnumeration,
        EObjectEnumeration,
        EHarvesterAddObject,
        EHarvesterDeleteObject,
        EHarvesterImportAbstractMedia,
        ECopyObject,
        EMoveObject,
        EDeleteObject,
        EGetObjectPropList,
        ESetObjectPropList
        };
    
    /**
       * C++ default constructor.
       */
    CTestObserver( CMmMtpDataProviderTestModule* aTestModule );
    
    /**
        * Destructor.
        */
        ~CTestObserver();
        
     static CTestObserver* NewL( CMmMtpDataProviderTestModule* aTestModule );
     static CTestObserver* NewLC( CMmMtpDataProviderTestModule* aTestModule );   
        
        
  // from MMTPDataProviderFramework
   
    /**
     Provides the unique identifier of the calling data provider.
     @return The data provider identifier.
     */
     TUint DataProviderId() const;

     /**
     Provides the current MTP operational mode.
     @return The current MTP operational mode.
     */
     TMTPOperationalMode Mode() const;
   
    /**
    Initiates a data object receive sequence in the MTP data provider framework
    layer. This method should only be invoked when processing the ERequestPhase
    of an MTP transaction (@see CMTPDataProviderPlugin::ProcessRequestPhaseL),
    and causes the MTP session transaction state to transition to the
    @see EDataIToRPhase. The data object receive sequence is completed when the 
    MTP data provider framework layer initiates the @see EResponsePhase of the 
    MTP transaction (@see CMTPDataProviderPlugin::ProcessRequestPhaseL).
    @param aData The MTP data object sink buffer.
    @param aRequest The MTP request dataset of the active MTP transaction.
    @param aConnection The handle of the MTP connection on which the transaction
    is being processed.
    @see CMTPDataProviderPlugin::ProcessRequestPhaseL
    @leave KErrNotReady, if invoked when the current MTP transaction phase is
    not ERequestPhase.
    */
    void ReceiveDataL(MMTPType& aData, const TMTPTypeRequest& aRequest, MMTPConnection& aConnection);

    /**
    Initiates a data object send sequence in the MTP data provider framework
    layer. This method should only be invoked when processing the ERequestPhase
    of an MTP transaction (@see CMTPDataProviderPlugin::ProcessRequestPhaseL),
    and causes the MTP session transaction state to transition to the
    @see EDataRToIPhase. The data object send sequence is completed when the 
    MTP data provider framework layer initiates the @see EResponsePhase of the 
    MTP transaction (@see CMTPDataProviderPlugin::ProcessRequestPhaseL).
    @param aData The MTP data object source buffer.
    @param aRequest The MTP request dataset of the active MTP transaction.
    @param aConnection The handle of the MTP connection on which the transaction
    is being processed.
    @see CMTPDataProviderPlugin::ProcessRequestPhaseL
    @leave KErrNotReady, if invoked when the current MTP transaction phase is
    not ERequestPhase.
    */
    void SendDataL(const MMTPType& aData, const TMTPTypeRequest& aRequest, MMTPConnection& aConnection);

    /**
    Signals the MTP data provider framework layer to send an asynchronous event
    dataset on the specified MTP connection.
    @param aEvent The MTP event dataset source buffer.
    @param aConnection The handle of the MTP connection on which the event is
    to be sent.
    @leave KErrArgument, if the event data is not valid.
    */
    void SendEventL(const TMTPTypeEvent& aEvent, MMTPConnection& aConnection);

    /**
    Signals the MTP data provider framework layer to send an asynchronous event
    dataset on all active MTP connections.
    @param aEvent The MTP event dataset source, this should always target all
    open sessions, i.e. SessionID should be set to KMTPAllSessions.
    @leave KErrArgument, if the event data is invalid.
    */
    void SendEventL(const TMTPTypeEvent& aEvent);

    /**
    Initiates an MTP response dataset send sequence in the MTP data provider
    framework layer. This method should only be invoked when processing either
    the @see ERequestPhase or @see EResponsePhase of an MTP transaction, (@see
    CMTPDataProviderPlugin::ProcessRequestPhaseL) and causes the MTP session
    transaction state to transition to the @see ECompletingPhase. The MTP 
    response dataset send sequence is completed when the MTP data provider 
    framework layer initiates the @see ECompletingPhase of the MTP transaction 
    (@see CMTPDataProviderPlugin::ProcessRequestPhaseL).
    @param aResponse The MTP aResponse dataset source buffer.
    @param aConnection The handle of the MTP connection on which the transaction
    is being processed.
    @see CMTPDataProviderPlugin::ProcessRequestPhaseL
    @leave KErrNotReady, if invoked when the current MTP transaction phase is
    not ERequestPhase or EResponsePhase.
    */
    void SendResponseL(const TMTPTypeResponse& aResponse, const TMTPTypeRequest& aRequest, MMTPConnection& aConnection);

    /**
    Signals to the MTP data provider framework layer that all processing
    related to the current transaction has been completed. This method should
    only be invoked when processing the @see ECompletingPhase of the MTP 
    transaction (@see CMTPDataProviderPlugin::ProcessRequestPhaseL), and causes 
    the MTP session transaction state to transition to the @see EIdle state.
    @param aRequest The MTP request dataset that initiated the transaction.
    @param aRequest The MTP request dataset of the active MTP transaction.
    @param aConnection The handle of the MTP connection on which the transaction
    is being processed.
    @see CMTPDataProviderPlugin::ProcessRequestPhaseL
    @leave KErrNotReady If invoked when the current MTP transaction phase is
    invalid.
    */
    void TransactionCompleteL(const TMTPTypeRequest& aRequest, MMTPConnection& aConnection);

    /**
    Registers the calling data provider to receive one or more occurrences of 
    the specified request dataset that are received on the specified 
    connection. This method should be used to register the calling data 
    provider to receive the following request types:
    
        1.  Follow-on requests of an MTP operation that can span multiple 
            transaction cycles. The following request types are recognised
            by the framework as follow-on requests:
            
            i)  SendObject (preceded by SendObjectInfo or SendObjectPropList). 
            ii) TerminateOpenCapture (preceded by InitiateOpenCapture).
            
        2.  MTP vendor extension requests.
        
    Note that:

        1.  The request dataset being registered must minimally specify the
            Operation Code of the expected operation and the SessionID on which 
            the operation request is expected to be received. 
            
            Follow-on request registrations MUST specify a specific SessionID. 
            Registrations of non follow-on requests may optionally specify a 
            SessionID of @see KMTPSessionAll to receive matching requests from
            any active MTP session.
           
        2.  With the exception of the TransactionID element, registered request
            datasets must exactly match all data elements of the expected request 
            dataset in order to be successfully routed.
           
        3.  Duplicate RouteRequestRegisterL registrations are not permitted. A request 
            dataset that matches that of a previous registration by this or 
            any other data provider on the same MTP session will result in the 
            previous registration being superceded.
           
        4.  Pending RouteRequestRegisterL registrations can be withdrawn at any time
            using the @see RouteRequestUnregisterL method. 
           
        5.  RouteRequestRegisterL registrations of MTP request types which ARE 
            recognised by the framework as MTP follow-on requests (SendObject, 
            TerminateOpenCapture) will result in at most one matching request 
            occurence being routed to the data provider. To receive another 
            request dataset of the same type, a new @see RouteRequestRegisterL 
            registration must be made.
            
        6   RouteRequestRegisterL registrations of MTP request types which ARE
            NOT recognised by the framework as MTP follow-on requests will 
            result in all matching requests which are subsequently received 
            being routed to the data provider. This will continue until such 
            time as the RouteRequestRegisterL registration is withdrawn using 
            the @see RouteRequestUnregisterL method.
            
        7.  RouteRequestRegisterL registrations request datasets which specify 
            an SessionID value of @see KMTPSessionAll, will result in matching 
            requests which are subsequently received on any active MTP session
            being routed to the data provider.

    @param aRequest The operation request dataset being registered.
    @param aConnection The handle of the MTP connection on which the operation
    request is expected to be received.
    @leave KErrArgument, if the request dataset does meet the minimal
    registration requirements specified above.
    @leave One of the system wide error codes, if a general processing error
    occurs.
    @see RouteRequestUnregisterL
    */
    void RouteRequestRegisterL(const TMTPTypeRequest& aRequest, MMTPConnection& aConnection);

    /**
    Cancels a pending RouteRequestRegisterL registration.
    @param aRequest The registered operation request dataset.
    @param aConnection The handle of the MTP connection for which the operation
    request was registered.
    @leave One of the system wide error codes, if a general processing error
    occurs.
    @see RouteRequestRegisterL
    */
    void RouteRequestUnregisterL(const TMTPTypeRequest& aRequest, MMTPConnection& aConnection);

    /**
    Signals the completion of the data provider's object store enumeration
    sequence that was previously initiated by a StartObjectEnumerationL signal
    made to the MTP data provider interface.
    @param aStorageId The MTP StorageID of the enumerated storage. This should 
    match the value specified in the preceding StartObjectEnumerationL.
    @see MMTPDataProvider::StartObjectEnumerationL
    */
    void ObjectEnumerationCompleteL(TUint32 aStorageId);

    /**
    Signals the completion of the data provider's storage enumeration sequence
    that was previously initiated by a StartStorageEnumerationL signal
    made to the MTP data provider interface.
    @see MMTPDataProvider::StartStorageEnumerationL
    */
    void StorageEnumerationCompleteL();

    /**
    Provides a handle to the configurability data specified in the data 
    provider's configuration file. 
    @return Handle to the data provider's configurability data.
    */
    const MMTPDataProviderConfig& DataProviderConfig() const;

    /**
    Provides a handle to the data provider framework configurability parameter 
    data. 
    @return Handle to the data provider framework configurability data.
    */
    const MMTPFrameworkConfig& FrameworkConfig() const;

    /**
    Provides a handle to the MTP object manager, which manages the assignment
    of MTP object handles and their mapping to actual data objects on behalf
    of the data provider.
    @return Handle to the MTP data provider framework object manager.
    */
    MMTPObjectMgr& ObjectMgr() const;

    /**
    Provides a handle to the MTP object reference manager, to which data
    providers can delegate the handling of the MTP persistent, abstract
    object referencing mechanism.
    @return Handle to the MTP data provider framework object reference manager.
    */
    MMTPReferenceMgr& ReferenceMgr() const;

    /**
    Provides a handle to the MTP storage manager, which manages the assignment 
    of MTP storage identifiers on behalf of the data provider.
    @return Handle to the MTP data provider framework storage manager.
    */
    MMTPStorageMgr& StorageMgr() const;
    
    /**
    Provides a handle to the MTP data provider framework RFs session.
    @return Handle to the MTP data provider framework RFs session.
    */
    RFs& Fs() const;   
    
    TBool ObjectEnumerationComplete() const;
    TInt ObjectEnumerationError() const;
    
    TBool StorageEnumerationComplete() const;

    TMTPTransactionPhase SetPhaseL( TMTPTransactionPhase aPhase );
    
    void SetPendingOperation( TAsyncOperation aOperation );
    TInt WaitForPendingOperation();
    TInt GetPendingResult();
public: // From MMTPDataProviderConfig

    TUint UintValue( TParameter aParam ) const;
    TBool BoolValue( TParameter aParam ) const;
    const TDesC& DesCValue( TParameter aParam ) const;
    const TDesC8& DesC8Value( TParameter aParam ) const;
    void GetArrayValue( TParameter aParam, RArray<TUint>& aArray ) const;
        
public: // From MMPXHarvesterUtilityObserver
    void HandleAbstractMediaExportCompletedL( CMPXMedia* aMedia, TInt aErr );
    void HandleAbstractMediaImportCompletedL( CMPXMedia* aMedia, TInt aErr );
    void HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr );
    void HandleDeleteCompletedL( TInt aErr );
    void HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr  );
    void HandleFileGetMediaCompletedL( CMPXMedia* aMedia, TInt aErr );

protected:
    void ConstructL();

public:
    TMTPTransactionPhase iPhase;
    TMTPTypeResponse iResponse;
    TUint16 iResponseCode;
    const MMTPType* iMMTPTypeData;
    CMTPTypeArray* iObjectPropsSupported;
    
private:
    CStifLogger* iLog;
    
    CTestObjectManager* iTestObjManager;
    RFs iFsSession;
    TBool iObjectEnumerationComplete;
    TInt iObjectEnumerationError;
    TBool iStorageEnumerationComplete;
    CTestStorageMgr* iStorageManager;
    CMmMtpDataProviderTestModule *iTestModule;
    TAsyncOperation iPendingOperation;
    TInt iAsyncResult;
    CActiveSchedulerWait* iActiveSchedulerWait;
    TUint32 iResourceId;
    };

#endif // CTESTOBSERVER_H
