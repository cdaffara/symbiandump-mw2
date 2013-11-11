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
* Description:  location triggering server client interface
*
*/



#ifndef LBTCLIENTREQUESTER_H
#define LBTCLIENTREQUESTER_H

#include <lbt.h>
#include "lbtclientrequester.h"


// Forward declaration
class RLbt;
/**
 * Client requestor class.
 *
 * This is a helper class used by RLbt class to serve the asyncronous operations
 * requested by the client.
 */
class CLbtClientRequester : public CActive
    {
public: 
    /** 
     * This is used to keep track of operation served by this object.
     */
    enum TAsyncReqType
        {
        EListTriggerIdsCount = 1,
        EListTriggersCount = 2,
        EOpOther = 3
        };
    /**
     * Construct a CLbtClientRequester object. 
     */
   	static CLbtClientRequester* NewL( RLbt& aLbt );
   
    /**
     * Destructor. 
     */
    ~CLbtClientRequester();

    /**
     * Starts the asyncronous operation by setting the iStatus to active
     */
    void Start();
    
    /**
     * Adds the buffer into the object.
     */
    void SetBuffer(CBufFlat* aBuf );
    
    /**
     * Starts the asyncronous operation by setting the iStatus to active.
     */
    void Start(TAsyncReqType aType);
    
    /**
     * Stores the client's status.
     */
    void SetStatus(TRequestStatus& aStatus);
    
    /**
     * Retrieves client's status
     */
    TRequestStatus& Status();
    
    /**
     * Stores the list trigger id data.
     */
    void SetListTriggerIdData(RArray < TLbtTriggerId >* aTriggerIdArray);
    
    /**
     * Stores the list trigger data.
     */
    void SetListTriggerData(RPointerArray < CLbtTriggerInfo >* aTriggerInfoList);
    
    
    /**
     * 
     */
    RPointerArray < CLbtTriggerInfo >* ListTriggerData();
    
    /**
     * 
     */
    RArray < TLbtTriggerId >* ListTriggerIdData();
    
    /**
     * Returns reference of list trigger size.
     */
    TInt& ListTriggersSize();
    

private://Derived from CActive
    void RunL();
    TInt RunError( TInt aError );
    void DoCancel();

private:
    /**
     * C++ default constructor
     */
    CLbtClientRequester( RLbt& aLbt);
    
    /**
     *Second phase constructor
     */   
    void ConstructL();

private:// data
    // Subsession handle
    RLbt& iLbt;
    
    // Pointer to client's status
    TRequestStatus* iClientStatus;
    
    // CBufFlat pointer array 
    RPointerArray < CBufFlat > iClientBuf;
    
    // Type of asyncronous request served.
    TAsyncReqType iType;
    
    // TLbtTriggerId array.   
    RArray < TLbtTriggerId >* iTriggerIdArray;
    
    // CLbtTriggerInfo pointer array.
    RPointerArray < CLbtTriggerInfo >* iTriggerInfoList;
    
    TInt iListTriggersSize;
    };

#endif // LBTTRIGGERCHANGEEVENTNOTIFIER_H

