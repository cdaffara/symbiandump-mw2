/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Remote party information
*
*/

#ifndef MCALLREMOTEPARTYINFO_H
#define MCALLREMOTEPARTYINFO_H

#include <e32std.h>

/**
 * 
 */
NONSHARABLE_CLASS( MCallRemotePartyInfo )
    {
    
public:
    
    /**  Remote Identity Status */
    enum TRemoteIdentityStatus
        {        
        /**
         * The remote party's identity can not be determined.
         * Calling party's number is not available.         
         * Cause of No CLI is "unavailable".
         */
        ERemoteIdentityUnknown,

        /**
         * The remote party's identity, or number, is available.
         * However, cause of No CLI has not been set.
         */
        ERemoteIdentityAvailable,

        /**
         * The remote party identity transmission has been suppressed 
         * i.e. their number is not available.
         * Cause of No CLI is "reject by user".
         */
        ERemoteIdentityRejectedByUser,
          
        /**
         * The remote party's identity, or number, is unavailable.         
         * To be used where the network has specified that CLI information
         * of the remote party is not available due to interaction with
         * another service.
         * The cause of No CLI is "Interaction with other service".
         */
        ERemoteIdentityNoCliInteractionWithOtherService,            

        /**
         * The remote party's identity, or number, is unavailable.
         * Cause of No CLI is "Coin Line/ Payphone".         
         */
        ERemoteIdentityNoCliCoinOrPayphone       
        };

public:

    /**
    * Remote party address getter.  
    *
    * @since S60 v5.1    
    * @return the remote party number (CS call) or address (VoIP call).
    */
    virtual const TDesC& Address() const = 0;        
       
    /**
    * Remote party name getter. 
    *
    * @since S60 v5.1
    * @return the remote party name that is locally matched in device.
    */    
    virtual const TDesC& MatchedName() const = 0;   
    
    /**
     * Remote party name from network getter. 
     *
     * @since S60 v5.1
     * @return the remote party name that is received from the network.
     *         This can be e.g. name received via CNAP service.
     */    
    virtual const TDesC& NameFromNetwork() const = 0;
        
    /**
     * Remote party company name getter. 
     *
     * @since S60 v5.1
     * @return the remote party company name that is locally matched in device.
     */    
    virtual const TDesC& CompanyName() const = 0;        
    
    /**
     * Index of the call.
     *
     * @since S60 v5.2
     * @return Call index.
     */
    virtual TInt CallIndex() const = 0;
    
    /**
     * Remote Identity Status.
     *
     * @since S60 v5.2
     * @return Remote identity.
     */
    virtual TRemoteIdentityStatus RemoteIdentity() const = 0;
    
    /**
     * Returns information whether call is a SAT call.
     * SAT call's remote number should not be shown in UI.
     * @since S60 v5.2     
     * @return ETrue if call is SAT call else EFalse.
     */
    virtual TBool SATCall() const = 0;
    
    /**
     * comparison operator.
     *
     * @since S60 v5.2
     * @return True if infos equal, otherwise false.
     */
    virtual TBool operator==(const MCallRemotePartyInfo& aRemoteInfo) const = 0;
    };
    
#endif // MCALLREMOTEPARTYINFO_H
