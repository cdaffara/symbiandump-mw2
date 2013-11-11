/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Positioning Indicator Helper Client Requestor
*
*/



#ifndef POS_IND_HELPER_CLIENTREQUESTER_H
#define POS_IND_HELPER_CLIENTREQUESTER_H

#include "posindicatorserverconst.h"

// Forward declaration
class MServerRequestCompletetionObserver;
/**
 * Client requestor class.
 *
 * This is a helper class used by RPosIndicatorHelper class to serve the asynchronous operations
 * requested to the server.
 */
class CPosIndHelperClientRequester : public CActive
    {
public: 

    /**
     * Construct a CPosIndHelperClientRequester object. 
     */
   	static CPosIndHelperClientRequester* NewL( MServerRequestCompletetionObserver& aServerRequestCompletetionObserver );
   
    /**
     * Destructor. 
     */
    ~CPosIndHelperClientRequester();

    /**
     * Starts the asynchronous operation by setting the iStatus to active
     * 
     * @param[in] aServiceId - Service id of the operation requested
     */
    void Start(TServiceIds aServiceId);
 
private://Derived from CActive
    void RunL();
    TInt RunError( TInt aError );
    void DoCancel();

private:
    /**
     * C++ default constructor
     */
    CPosIndHelperClientRequester( MServerRequestCompletetionObserver& aServerRequestCompletetionObserver );
    
    /**
     *Second phase constructor
     */   
    void ConstructL();

private:// data
    
    /*
     * Handler to the observer which notifies operation completion to the client 
     */    
    MServerRequestCompletetionObserver& iServerRequestCompletetionObserver;  
    
    /*
     * Service id of the operation requested. 
     */
    TServiceIds iServiceId;
    };

#endif // POS_IND_HELPER_CLIENTREQUESTER_H

