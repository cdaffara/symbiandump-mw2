/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Create the call and emergency call to ETelMM
*
*/


#ifndef CSETUPCALLREQUESTHANDLER_H
#define CSETUPCALLREQUESTHANDLER_H

#include <e32base.h>
#include "msatmultimodeapi.h"

class CSetUpCallHandler;
class MSatAsyncToSync;
class CSetupCallDtmfSender;

/**
*  This is the handler for the ETel MM api Request.
*  This active objects is registered with ETelMM Api to send request and 
*  receive notifications about some Request Complete.
*
*/

NONSHARABLE_CLASS ( CSetupCallRequestHandler ) : public CActive
    {
public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the MSatMultiModeApi.
     * @param aDispatcher Pointer to Ss handler
     * @return a pointer to the newly created object.
     */
    static CSetupCallRequestHandler* NewL( MSatMultiModeApi& aPhone,
        CSetUpCallHandler* aDispatcher );

    /**
     * Destructor.
     */
    virtual ~CSetupCallRequestHandler();

    /**
     * Access RMobileCall::DialNoFdnCheck by MSatMultiModeApi
     * for the paramter information please see the etelmm.h
     */
    void DialNumber( const TDesC8& aCallParams, TDes& aTelNumber,
            TBool aTerminateOtherCall, MSatAsyncToSync* aAsyncToSync );

    /**
     * Access RMobileCall::DialEmergencyCall by MSatMultiModeApi
     * for the paramter information please see the etelmm.h
     */
    void DialEmergencyCall( const TDesC& aTelNumber );
    
    /**
     * Cancel the asynchronous operations that required to the ETel MM api  
     */
    void CancelOperation();
    
 
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
    CSetupCallRequestHandler( MSatMultiModeApi& aPhone,
        CSetUpCallHandler* aDispatcher );
    
    /**
    * By default Symbian 2nd phase constructor is private.
    */ 
    void ConstructL();
        
private: // Data

    /**
     * Reference to the MSatMultiModeApi
     */
    MSatMultiModeApi& iPhone;
        
    /**
     * Pointer to SetupCall command handler
     */
    CSetUpCallHandler* iDispatcher;
    
    /**
     * Own. Dtmf sender
     */    
    CSetupCallDtmfSender* iDtmfSender;
    };

#endif      // CSETUPCALLREQUESTHANDLER_H
