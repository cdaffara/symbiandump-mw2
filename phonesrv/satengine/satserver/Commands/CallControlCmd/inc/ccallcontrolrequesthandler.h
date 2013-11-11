/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles CallControl request that need SAT perform
*
*/


#ifndef CCALLCONTROLREQUESTHANDLER_H
#define CCALLCONTROLREQUESTHANDLER_H

#include <e32base.h>
#include "msatmultimodeapi.h"

class CCallControlHandler;

/**
*  This is the handler for the ETel MM api Request.
*  This active objects is registered with ETelMM Api to send request and 
*  receive notifications about some Request Complete.
*
*  @lib CallControlCmd
*  @since S60 v5.0.1
*/

class CCallControlRequestHandler : public CActive
    {

public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the MSatMultiModeApi.
     * @param aDispatcher Pointer to Ss handler
     * @return a pointer to the newly created object.
     */
    static CCallControlRequestHandler* NewL( MSatMultiModeApi& aPhone,
        CCallControlHandler* aDispatcher );

    /**
     * Destructor.
     */
    virtual ~CCallControlRequestHandler();

    /**
     * Access RMobileCall::DialNoFdnCheck by MSatMultiModeApi
     * for the paramter information please see the etelmm.h
     */
    void DialNumber( const TDesC8& aCallParams, const TDesC& aTelNumber );

    /**
     * Starts send the SS Request (No FDN check).
     * for the paramter information please see the etelmm.h
     */
    void SendSs( const TDesC& aServiceString );

    /**
     * Starts send the USSD Request (No FDN check).
     * for the paramter information please see the etelmm.h
     */
    void SendUssd( const TDesC8& aMsgData, const TDesC8& aMsgAttributes );

    /**
     * Cancel the asynchronous operations that required to the ETel MM api  
     * for the paramter information please see the etelmm.h
     */
    void CancelOperation( TInt aOperationType );
 
protected:

    /**
     * From CActive, handles the request completion.
     */
    void RunL();
    
    
    /**
     * From CActive, handle the request cancel
     */
    void DoCancel();

private:

    /**
     * C++ default constructor.
     * @param aPriority An active object priority value.
     * @param aPhone A reference to MSatMultiModeApi.
     */
    CCallControlRequestHandler( MSatMultiModeApi& aPhone,
        CCallControlHandler* aDispatcher );
        
    /**
     * Two phase contruction.
     */    
    void ConstructL();

private: // Data

    /**
     * Reference to the MSatMultiModeApi
     */
    MSatMultiModeApi& iPhone;
        
    /**
     * Result of the operations
     */
    TInt iCcStatus;

    /**
     * Pointer to SendSs command handler
     */
    CCallControlHandler* iDispatcher;
    };

#endif      // CCALLCONTROLREQUESTHANDLER_H
