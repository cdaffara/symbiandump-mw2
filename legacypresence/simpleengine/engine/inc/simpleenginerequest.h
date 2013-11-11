/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    SIMPLE Engine request
*
*/



#ifndef CSimpleEngineRequest_H
#define CSimpleEngineRequest_H


// INCLUDES
#include <e32base.h>
#include "simplecommon.h"
#include "msimpleenginerequest.h"


// FORWARD DECLARATIONS
class CSimpleSipConnection;
class MSimpleOwnCallback;


// CLASS DECLARATION

/**
 *  CSimpleEngineRequest
 *
 *  SIMPLE Engine request.
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class CSimpleEngineRequest : public CActive, public MSimpleEngineRequest
    {

public:

    /**
     * Two-phased constructor.
     * @param aEngine callback method interface
     * @param aType request type
     * @param aOpId client operation id
     * @since S60 3.2
     */
    static CSimpleEngineRequest* NewL(
        MSimpleOwnCallback& aEngine,
        MSimpleEngineRequest::TSimpleRequest aType,
        TInt aOpId );

    /**
     * Destructor.
     */
    virtual ~CSimpleEngineRequest();

    /**
     * Delete this entity
     */
    void Destroy();

    /**
     * Request data setter
     * @param aData  data payload. Data is not copied but ownership is transferred.
     */
    void SetRequestData( TPtrC8 aData );
    
    inline static TInt LinkOffset();
    
    void SetRequestContentTypeL( const TDesC8& aData );         

// from base class MSimpleEngineRequest

    /**
     * From MSimpleEngineRequest.
     * Request type accessor
     * @since S60 3.2
     * @return request type
     */
    MSimpleEngineRequest::TSimpleRequest RequestType();

    /**
     * Request type accessor
     * @since S60 3.2
     * @param aType request type
     */
    void ModifyType( MSimpleEngineRequest::TSimpleRequest aType );

    /**
     * From MSimpleEngineRequest.
     * Complete the open request
     * @since S60 3.2
     * @param aStatus result status
     */
    void Complete( TInt aStatus );

    /**
     * From MSimpleEngineRequest.
     * Request data type accessor
     * @since S60 3.2
     * @return request data content type
     */
    TPtrC8 RequestContentType();       

    /**
     * From MSimpleEngineRequest.
     * Request data accessor
     * @since S60 3.2
     * @return request data payload
     */
    TPtrC8 RequestData();

    /**
     * Response data type setter.
     * @since S60 3.2
     * @param aData content that is to be copied?
     */
    void SetResponseContentType( HBufC8* aData );

    /**
     * Response data MIME multipart boundary setter.
     * @since S60 3.2
     * @param aData boundary
     */
    void SetResponseBoundaryL( const TDesC8& aData );

    /**
     * Response data MIME multipart start content-id setter
     * @since S60 3.2
     * @param aData content-id
     */
    void SetResponseStartL( const TDesC8& aData );

    /**
     * From MSimpleEngineRequest.
     * Response data setter.
     * @since S60 3.2
     * @param aData content
     */
    void SetResponseDataL( const TDesC8& aData );

    /**
     * From MSimpleEngineRequest.
     * Response method setter.
     * @since S60 3.2
     * @param aMethod a method
     */
    void SetResponseMethod( TSimpleSIPResponse aMethod );

    /**
     * From MSimpleEngineRequest.
     * Aux data setter.
     * @since S60 3.2
     * @param aData data
     */
    void SetAux( TInt aData );

    /**
     * Server response/request method accessor
     * @return SIP method
     */
    TSimpleSIPResponse ResponseMethod();

    /**
     * From MSimpleEngineRequest.
     * Response data type accessor
     * @since S60 3.2
     * @return content type
     */
    TPtrC8 ResponseContentType();

    /**
     * Response data MIME multipart boundary getter.
     * @since S60 3.2
     * @return data boundary
     */
    TPtrC8 ResponseBoundary( );

    /**
     * Response data MIME multipart start content-id getter
     * @since S60 3.2
     * @return start content-id
     */
    TPtrC8 ResponseStart( );

    /**
     * From MSimpleEngineRequest.
     * Response data accessor
     * @since S60 3.2
     * @return response data payload
     */
    TPtrC8 ResponseData();

    /**
     * From MSimpleEngineRequest.
     * Refresh time accessor
     * @since S60 3.2
     * @return refresh time in seconds, 0 means query once
     */
    TUint RefreshTime();

    /**
     * Set refresh time
     * @param aTime time in seconds
     */
    void SetRefreshTime( TUint aTime );

    /**
     * From MSimpleEngineRequest.
     * Remote URI accessor
     * @since S60 3.2
     * @return remote URI
     */
    TPtrC8 RemoteURI();

    /**
     * Remote URI setter
     * @since S60 3.2     
     * @param aURI remote URI
     */
    void SetRemoteURIL( const TDesC8& aURI );

    /**
     * Get refresh parameter
     * @since S60 3.2
     * @return refresh parameter     
     */
    TBool IsRefresh();

    /**
     * Refresh parameter setter
     * @since S60 3.2       
     * @param aRefresh refresh value
     */
    void SetRefresh( TBool aRefresh );

    /**
     * operation id getter
     * @since S60 3.2       
     * @return operation id
     */
     TInt OpId();

    /**
     * auxiliary parameter needed sometimes
     * @since S60 3.2       
     * @return aux number
     */
     TInt Aux();

    /**
     * SIP Response reason setter
     * @since S60 3.2       
     * @param aReason reason
     */
    void SetResponseReason( TSimpleResponseReason aReason );

    /**
     * SIP Response reason getter
     * @since S60 3.2       
     * @return aReason reason
     */
    TSimpleResponseReason ResponseReason();
    
    /**
     * Set SIP-ETag value
     *
     * @since S60 3.2     
     * @param aTag tag
     */
    void SetETagL( const TDesC8& aTag );

    /**
     * get SIP-ETag value
     *
     * @since S60 3.2     
     * @return SIP-ETag value.
     */
    TPtrC8 ETag();      
        
    /**
     * IM message recipient setter
     *
     * @since S60 5.0
     * @param aRecipientId recipient ID
     */
    void SetRecipientL( const TDesC8& aRecipientId );

    /**
     * IM message recipient getter
     *
     * @since S60 5.0
     * @return recipient ID
     */
    TPtrC8 Recipient();
        
protected:

    /**
     * C++ default constructor.
     */
    CSimpleEngineRequest(
        MSimpleOwnCallback& aEngine, TSimpleRequest aType, TInt aOpId );

// from base class CActive

    /**
     * From CActive.
     * Implements cancellation of an outstanding request.
     *
     * @since S60 3.2
     */
    void DoCancel( );

    /**
     * From CActive.
     * Handles an active object's request completion event.
     *
     * @since S60 3.2
     */
    void RunL( );

private:


private:    // Data

    TDblQueLink iLink;

    /**
     * Request Time
     */
    TSimpleRequest iType;

    /**
     * Refresh Time
     */
    TUint iRefreshTime;

    /**
     * Client request data
     */
    TPtrC8 iRequestData;

    /**
     * IM message recipient ID
     * Own.
     */
    HBufC8* iRecipientId;
    
    /**
     * Buffer for server response data
     * Own.
     */
    HBufC8* iResponseData;

    /**
     * Server response method
     */
    TSimpleSIPResponse iResponseMethod;

    /**
     * Server response reason explanation
     */
    TSimpleResponseReason iReason;

    /**
     * Buffer for remote URI
     * Own.
     */
    HBufC8* iRemoteURI;

    /**
     * Buffer for request content type
     * Own.
     */
    HBufC8* iRequestContentType;

    /**
     * Buffer for response content type
     * Own.
     */
    HBufC8* iResponseContentType;

    /**
     * Buffer for response MIME multipart boundary
     * Own.
     */
    HBufC8* iResponseBoundary;

    /**
     * Buffer for response MIME multipart start content-id
     * Own.
     */
    HBufC8* iResponseStart;

    /**
     * operation id
     */
    TInt iOpId;

    /**
     * client callback observer
     */
    MSimpleOwnCallback& iEngine;

    /**
     * whether refresh is made automatically
     */
    TBool iRefresh;

    /**
     * operation id
     */
    TInt iAux;
    
    /**
     * SIP ETag header
     * Own.
     */
    HBufC8* iETag; 
    
    /**
     * Pointer to stack variable to detect the deletion of the heap
     * instance. This case takes place if a client
     * calls destructor in callback method.
     * Own.
     */
    TBool* iDestroyedPtr;          

  };
  
    TInt CSimpleEngineRequest::LinkOffset()
        {
        return _FOFF(CSimpleEngineRequest, iLink);
        }    

#endif

// End of File
