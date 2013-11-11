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
* Description:  CCP emergency call class
*
*/


#ifndef MCCPEMERGENCYCALL_H
#define MCCPEMERGENCYCALL_H

//  INCLUDES
#include <e32base.h>
#include <e32cmn.h> // TDesC

/**
* Emergency call class
* @since S60 3.2
*/
class MCCPEmergencyCall
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from plug-in client (CCE).
    * @since S60 3.2
    */
    virtual inline ~MCCPEmergencyCall() {};
    
public:
    /**
    * Dials an emergency call.
    * @param aRecipient Emergency call address suggested to be used.
    * Can be given also in CConvergedCallProvider::NewEmergencyCallL.
    * Plug-in will use given address and/or its own known one. 
    * Preferred address usage order:
    * 1) Plug-in own emergency address
    * 2) Address given in Dial
    * 3) Address given in CConvergedCallProvider::NewEmergencyCallL.
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady If call is not in idle state.
    * @return KErrNotSupported Emergency call is not supported.
    * @since S60 3.2
    */
    virtual TInt Dial( const TDesC& aRecipient=KNullDesC) = 0;

    /**
    * Called by CCE instead of HangUp when Dial has been called and state change to 
    * MCCPCallObserver::ECCPStateConnecting has not been received. 
    * Cancels the last ongoing request. Used for Dial only. Not supported for other actions.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None.
    * @return KErrNone if request was started succesfully. 
    * @since S60 3.2
    * @pre MCCPEmergencyCall::Dial has been called and call state is MCCPCallObserver::ECCPIdle 
    */
    virtual TInt Cancel() = 0;

    /**
    * Ends an ongoing call.
    * @param none
    * @return KErrNone If request was started succesfully.
    * @return KErrAlreadyExists If call is already in idle state.
    * @since S60 3.2
    */
    virtual TInt HangUp() = 0;

    /**
    * Answers to an incoming call.
    * TODO: remove as unneeded? no answer supported ?
    * @param none
    * @return KErrNone If request was started succesfully.
    * @return KErrNotReady If call is not in ringing or queued state.
    * @since S60 3.2
    */
    virtual TInt Answer() = 0;
    };

#endif // MCCPEMERGENCYCALL_H

