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
* Description:
*
*/


#ifndef CREQUESTPROCESSOR_H
#define CREQUESTPROCESSOR_H


#include <mtp/tmtptypenull.h>
#include <mtp/mtpdataproviderapitypes.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/tmtptyperesponse.h>

// for derived processors
#include <mtp/mmtpdataproviderframework.h>

#include "crequestchecker.h"

class MMTPConnection;
class TMTPTypeRequest;
class MMTPDataProviderFramework;
class TMTPTypeEvent;
class CMTPObjectInfo;

const TInt KMmMtpRArrayGranularity = 2;

/**
* Defines a request processor interface
*/
class MMmRequestProcessor
    {
public:
    /**
    * Process a request from the initiator
    * @param aRequest The request to be processed
    * @param aPhase The transaction phase of the request
    * @return ETrue to signal that the processor object can be deleted, EFalse to keep the processor object
    */
    virtual TBool HandleRequestL( const TMTPTypeRequest& aRequest,
        TMTPTransactionPhase aPhase ) = 0;

    /**
    * Process an event from the initiator
    * @param aEvent The event to be processed
    */
    virtual void HandleEventL( const TMTPTypeEvent& aEvent ) = 0;

    /**
    * delete the request processor object
    */
    virtual void Release() = 0;

    /**
    * Check if the processor matches the request on the connection
    * @param aRequest The request to be checked
    * @param aConnection The connection from which the request comes
    * @return ETrue to indicate the processor can handle the request, otherwise, EFalse
    */
    virtual TBool Match( const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection ) const = 0;

    /**
    * Check if the processor matches the event on the connection
    * @param aEvent The event to be checked
    * @param aConnection The connection from which the event comes
    * @return ETrue to indicate the processor can handle the event, otherwise, EFalse
    */
    virtual TBool Match( const TMTPTypeEvent& aEvent,
        MMTPConnection& aConnection ) const = 0;

    /**
    * Get the request object which the processor is currently handling
    * @return the request object which the processor is currently handling
    */
    virtual const TMTPTypeRequest& Request() const = 0;

    /**
    * Get the connection object associated with the current request object
    * @return the connection object associated with the current request object
    */
    virtual MMTPConnection& Connection() const = 0;

    /*
    * Rollback when WMP closed and disconnect the USB cable,
    * during the transferring file
    */
    virtual void UsbDisconnect() = 0;

    /*
    * Get the session id associated with current request object
    * This interface was added to avoid the case that the data member iRequest
    * was sometimes invalid in session close phase, and getting session id from
    * request would cause panic
    * @return session id
    */
    virtual TUint32 SessionId() = 0;
    };

/**
* Defines a processor factory function pointer
*/
typedef MMmRequestProcessor* (*TMTPRequestProcessorCreateFunc)( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection );

/**
* Defines an entry which maps from operation code to the request processor
*/
typedef struct
    {
    TUint16 iOperationCode;
    TMTPRequestProcessorCreateFunc iCreateFunc;
    } TMTPRequestProcessorEntry;

/**
* Defines a generic request processor interface from which all processors derive
*/
class CRequestProcessor : public CActive, public MMmRequestProcessor
    {
protected:
    /**
    * Standard c++ constructor
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aElementCount Number of element in the request.
    * @param aElements    The element info data.
    * @return a pointer to the created request processor object
    */
    IMPORT_C CRequestProcessor( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        TInt aElementCount,
        const TMTPRequestElementInfo* aElements );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CRequestProcessor();

protected:
    // utility methods
    /**
    * Send a response to the initiator
    * @param aResponseCode The response code to send
    * @param aParamCount  The number of parameters
    * @param aParmas  The pointer to array of parameters
    */
    IMPORT_C void SendResponseL( TMTPResponseCode aResponseCode,
        TInt aParameterCount = 0,
        TUint32* aParams = NULL );

    /**
    * Signal to the framework that the current request transaction has completed
    */
    void CompleteRequestL();

    /**
    * Send data to the initiator
    * @param aData    The data to send
    */
    IMPORT_C void SendDataL( const MMTPType& aData );

    /**
    * Receive data from the initiator
    * @param aData    The data to receive
    */
    IMPORT_C void ReceiveDataL( MMTPType& aData );

protected:
    // from MMTPRequestProcessor
    /**
    * Handle the request
    * @param aRequest The request to be processed
    * @param aPhase   The current transaction phase of the request
    * @return ETrue if the transaction has completed, otherwise, EFalse
    */
    IMPORT_C TBool HandleRequestL( const TMTPTypeRequest& aRequest,
        TMTPTransactionPhase aPhase );

    /**
    * Handle the event
    * @param aEvent The event to be processed
    */
    IMPORT_C void HandleEventL( const TMTPTypeEvent& aEvent );

    /**
    * Relese (delete) this request processor
    */
    IMPORT_C void Release();

    /**
    * Check whether the processor can process the request
    * @param aRequest The request to be processed
    * @param aConnection The connection from which the request comes
    * @return ETrue if the processor can process the request, otherwise EFalse
    */
    IMPORT_C TBool Match( const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection ) const;

    /**
    * Check whether the processor can process the event
    * @param aEvent The event to be processed
    * @param aConnection The connection from which the request comes
    * @return ETrue if the processor can process the request, otherwise EFalse
    */
    IMPORT_C TBool Match( const TMTPTypeEvent& aEvent,
        MMTPConnection& aConnection ) const;

    /**
    * The current active request
    * @return A reference to the current request
    */
    IMPORT_C const TMTPTypeRequest& Request() const;

    /**
    * The connection from which the current request comes
    * @return A reference to the current connection
    */
    IMPORT_C MMTPConnection& Connection() const;

    /*
    * Rollback when WMP closed and disconnect the USB cable,
    * during the transferring file
    */
    IMPORT_C void UsbDisconnect();

    /*
    * Get the session id associated with current request object
    * This interface was added to avoid the case that the data member iRequest
    * was sometimes invalid in session close phase, and getting session id from
    * request would cause panic
    * @return session id
    */
    IMPORT_C TUint32 SessionId();

protected:
    // from CActive
    /**
    * part of active object framework, provide default implementation
    */
    IMPORT_C void RunL();

    /**
    * part of active object framework, provide default implementation
    */
    IMPORT_C void DoCancel();

    /**
    * part of active object framework, provide default implementation
    */
    IMPORT_C TInt RunError( TInt aError );

protected:
    // new virtuals
    /**
    * Handle the request phase of the current request
    * @return EFalse
    */
    IMPORT_C virtual TBool DoHandleRequestPhaseL();

    /**
    * Handle the receiving data phase of the current request
    * @return EFalse
    */
    IMPORT_C virtual TBool DoHandleDataIToRPhaseL();

    /**
    * Handle the sending data phase of the current request
    * @return EFalse
    */
    IMPORT_C virtual TBool DoHandleRToIPhaseL();

    /**
    * Handle the response phase of the current request
    * @return EFalse
    */
    IMPORT_C virtual TBool DoHandleResponsePhaseL();

    /**
    * Handle the completing phase of the current request
    * @return ETrue
    */
    IMPORT_C virtual TBool DoHandleCompletingPhaseL();

    /**
    * Check the current request
    * @return EMTPRespCodeOK if the reqeust is good, otherwise, one of the error response codes
    */
    IMPORT_C virtual TMTPResponseCode CheckRequestL();

    IMPORT_C virtual TBool HasDataphase() const;

    /**
    * service a request at request phase
    */
    virtual void ServiceL() = 0;

protected:
    /**
    * Set P&S Status to avoid MPX access conflict
    */

private:
    /**
    * retrieve the session id and transaction code from the current request
    */
    void ExtractSessionTransactionId();

protected:
    MMTPDataProviderFramework& iFramework;
    const TMTPTypeRequest* iRequest; //the pending requst object
    MMTPConnection& iConnection; //the connection from which the request comes
    TMTPTypeResponse iResponse; //the response object to send to the initiator
    TBool iCancelled; //indicates whether the data phase (send/receive) has been cancelled
    CRequestChecker* iRequestChecker; //a utility class providing generic request verification service
    TInt iElementCount; //number of verification elements used for request checker
    const TMTPRequestElementInfo* iElements; //pointer to an array of verification elements
    TUint32 iSessionId; //session id for the pending request
    TUint32 iTransactionCode; //transaction code for the pending request

private:
    TMTPResponseCode iResponseCode; // contains response from CheckRequestL call
    RBuf8 iNullBuffer; // buffer to receive data from discarded data phase
    TMTPTypeNull iNull;

    };

#endif // CREQUESTPROCESSOR_H
