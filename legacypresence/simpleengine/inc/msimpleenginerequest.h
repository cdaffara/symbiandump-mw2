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
* Description:    Abstract base class for WV engine service APIs.
*
*/



#ifndef M_simpleenginerequest_H
#define M_simpleenginerequest_H


// INCLUDES
#include <e32base.h>
#include "simplecommon.h"


/**
 *  Abstarct API for request handling in SIMPLE engine core and SIMPLE SIP LIB communication.
 *
 *  @lib simpleengine
 *  @since S60 v3.2
 */
class MSimpleEngineRequest
    {
public:

    /** Request type */
    enum TSimpleRequest
        {
        /** None */
        ENone=0,        
        /** SIP Register */
        ERegister,
        /** SIP Publish */
        EStartPublish,
        /** SIP modify Publish */
        EPublishModify,
        /** SIP Subscribe */
        ESubscribe,
        /** SIP list Subscribe */
        ESubscribeLista,
        /** SIP Subscribe Get (once) */
        ESubscribeGet,
        /** SIP subscribe WINFO */
        ESubscribeWinfo,
        /** SIP Unsubscribe */
        ESubscribeStop,
        /** SIP Dialog (notify, status) */
        EDialog,
        /** SIP Publish stop */
        EStopPublish,
        /** Listen to event states */
        EListenEvents,       
        /** Destroy the object */
        EDestroy,
        /** Destroy the object with a delay */
        EDestroyStart,
        /** SIP Subscrption-state */
        ESubscriptionState,
        /** SIP send IM */
        ESendIM,
        /** SIP receive IM */
        EReceiveIM
        };
        
    /** SIP response/request message */    
    enum TSimpleSIPResponse
        {
        /** None */
        EUnknownResponse=0,        
        /** SIP NOTIFY */
        ENotify,
        /** SIP status */
        EStatus,
        /** ETag update only in status in refresh response */
        EStatusETag
        }; 
        
    /** NOTIFY response reason */     
    enum TSimpleResponseReason
        {
        /** unknown reason */
        ENoReason,
        /** Application SHOULD re-subscribe later. */        
        EDeactivated,
        /** Application SHOULD re-subscribe later. */      
        EProbation,
        /** The subscription has been terminated due to change in
            authorization policy.  
            Application SHOULD NOT attempt to re-subscribe. */           
        ERejected,
        /** The subscription has been terminated because it was not
            refreshed before it expired.  Application MAY re-subscribe
            later. */
        ETimeout,
        /** The subscription has been terminated because the notifier
            could not obtain authorization in a timely fashion.
            Application MAY re-subscribe later. */ 
        EGiveup,
        /** The subscription has been terminated because the resource
            state which was being monitored no longer exists. 
            Application SHOULD NOT attempt to re-subscribe. */
        ENoresource
        };  
        
    /** SIP subscription-state */    
    enum TSimpleSipSubscriptionState
        {
        ESimpleStateNone=0,
        ESimpleStatePending,
        ESimpleStateActive,
        ESimpleStateTerminated
        };                    


    /**
     * Destructor
     * @since S60 3.2
     */
    virtual void Destroy() = 0;

    /**
     * Request type accessor
     * @since S60 3.2
     * @return request type
     */
    virtual TSimpleRequest RequestType() = 0;

    /**
     * Request type accessor
     * @since S60 3.2
     * @param aType request type
     */
    virtual void ModifyType( TSimpleRequest aType ) = 0;

    /**
     * Complete the open request
     * @since S60 3.2
     * @param aStatus result status
     */
    virtual void Complete( TInt aStatus ) = 0;

    /**
     * Request data type accessor
     * @since S60 3.2
     * @return request data content type
     */
    virtual TPtrC8 RequestContentType() = 0;

    /**
     * Request data accessor
     * @since S60 3.2
     * @return request data payload
     */
    virtual TPtrC8 RequestData() = 0;

    /**
     * Response data type setter.
     * @since S60 3.2
     * @param aData content that is NOT copied, ownership is transferred
     */ 
    virtual void SetResponseContentType( HBufC8* aData ) = 0;
    
    /**
     * Response data MIME multipart boundary setter.
     * @since S60 3.2
     * @param aData boundary
     */ 
    virtual void SetResponseBoundaryL( const TDesC8& aData ) = 0;
    
    /**
     * Response data MIME multipart start content-id setter
     * @since S60 3.2
     * @param aData content-id
     */ 
    virtual void SetResponseStartL( const TDesC8& aData ) = 0;        

    /**
     * Response data setter.
     * @since S60 3.2
     * @param aData content that is to be copied.
     */
    virtual void SetResponseDataL( const TDesC8& aData ) = 0;

    /**
     * Response method setter.
     * @since S60 3.2
     * @param aMethod a method
     */
    virtual void SetResponseMethod( TSimpleSIPResponse aMethod ) = 0;
    
    /**    
     * Aux data setter.
     * @since S60 3.2       
     * @param aData data 
     */
    virtual void SetAux( TInt aData ) = 0;      

    /**
     * Server response/request method accessor
     * @return SIP method
     */
    virtual TSimpleSIPResponse ResponseMethod() = 0;

    /**
     * Response data type accessor
     * @since S60 3.2
     * @return content type
     */
    virtual TPtrC8 ResponseContentType() = 0;
    
    /**
     * Response data MIME multipart boundary getter.
     * @since S60 3.2
     * @return data boundary
     */ 
    virtual TPtrC8 ResponseBoundary( ) = 0;
    
    /**
     * Response data MIME multipart start content-id getter
     * @since S60 3.2
     * @return start content-id
     */ 
    virtual TPtrC8 ResponseStart( ) = 0;     

    /**
     * Response data accessor
     * @since S60 3.2
     * @return response data payload
     */
    virtual TPtrC8 ResponseData() = 0;

    /**
     * Refresh time accessor
     * @since S60 3.2
     * @return refresh time in seconds, 0 means query once
     */
    virtual TUint RefreshTime() = 0;

    /**
     * Refresh time setter
     * @since S60 3.2
     * @param aTime time in seconds, 0 means query once
     */
    virtual void SetRefreshTime( TUint aTime) = 0;

    /**
     * Remote URI accessor
     * @since S60 3.2
     * @return remote URI
     */
    virtual TPtrC8 RemoteURI() = 0;
    
    /**
     * Remote URI setter
     * @since S60 3.2     
     * @param aURI remote URI
     */
    virtual void SetRemoteURIL( const TDesC8& aURI ) = 0;

    /**
     * Get refresh parameter
     * @since S60 3.2
     * return ETrue if refresh     
     */
    virtual TBool IsRefresh() = 0;

    /**
     * Refresh parameter setter
     * @since S60 3.2     
     * @param aRefresh refresh boolean value
     */
    virtual void SetRefresh( TBool aRefresh ) = 0;

    /**
     * Operation id getter
     * @since S60 3.2     
     * @return operation id
     */
    virtual TInt OpId( ) = 0;
    
    /**
     * auxiliary parameter needed sometimes
     * @since S60 3.2     
     * @return aux number
     */   
    virtual TInt Aux() = 0;
             
    /**
     * SIP Response reason setter
     * @since S60 3.2     
     * @param aReason reason
     */    
    virtual void SetResponseReason( TSimpleResponseReason aReason ) = 0;
    
    /**
     * SIP Response reason getter
     * @since S60 3.2     
     * @return aReason reason
     */   
    virtual TSimpleResponseReason ResponseReason() = 0;  
    
    /**
     * Set SIP-ETag value
     *
     * @since S60 3.2     
     * @param aTag tag
     */
    virtual void SetETagL( const TDesC8& aTag ) = 0;

    /**
     * get SIP-ETag value
     *
     * @since S60 3.2     
     * @return SIP-ETag value.
     */
    virtual TPtrC8 ETag() = 0;                    

    /**
     * IM message recipient setter
     *
     * @since S60 5.0
     * @param aRecipientId recipient ID
     */
    virtual void SetRecipientL( const TDesC8& aRecipientId ) = 0;

    /**
     * IM message recipient getter
     *
     * @since S60 5.0
     * @return recipient ID
     */
    virtual TPtrC8 Recipient() = 0;

    };

#endif

// End of File
