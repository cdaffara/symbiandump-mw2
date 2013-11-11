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
* Description: Declaration of server session class.
*
*/

#ifndef POSINDICATORSERVERSESSION_H
#define POSINDICATORSERVERSESSION_H

#include <e32base.h>

// Forward declaration
class CPosIndicatorServerCore;
class CPosIndicatorSubSessionRegistry;

class CPosIndicatorServerSession : public CSession2 
    {
public:
    /**
     * Instantiates a new object of CPosIndicatorServerSession
     * 
     */
    static CPosIndicatorServerSession* NewL( const 
                             CPosIndicatorServerCore& aServerCore );
    
public: // From CSession2
    /**
     * Handles the servicing of a client request that 
     * has been passed to the server via this session.
     *
     * @param[in] aMessage The message containing the details of 
     * the client request
     *
     */      
    virtual void ServiceL(const RMessage2 &aMessage);

    /**
     * Handles error cases when the ServiceL method leaves.
     *
     * @param[in] aMessage The message containing the details of 
     * the client request
     * @param[in] aError The error code that the serviceL method 
     * leaves with
     */ 
    virtual void ServiceError(const RMessage2& aMessage,TInt aError);

    /**
     * Handles session closure when the client panics.
     *
     * @param[in] aMessage The message containing the details of 
     * the client request
     * @param[in] aError The error code that the serviceL method 
     * leaves with
     */ 
    virtual void Disconnect(const RMessage2& aMessage);

private:
    /**
     * Default C++ Constructor
     *
     * @param[in] aSecureId the secure Id of the client application
     * which has created this session object. 
     *
     */
    CPosIndicatorServerSession( const CPosIndicatorServerCore& aServerCore );
    
    /**
     * Destructor
     *
     */
    ~CPosIndicatorServerSession();
    
    /**
     * 2nd phase constructor for instantiating 
     * member variables
     *
     * @param[in] aSecureId the secure Id of the client application
     * which has created this session object.
     */
    void ConstructL();
    
    /**
     * Create a sub-session object
     *
     */
    void CreateSubSessionL(const RMessage2 &aMessage);
    
    /**
     * Forwards client request to sub-session
     *
     * @aMessage[in] The message containing the details of 
     * the client request
     *
     */
    void ForwardToSubSessionL(const RMessage2& aMessage);
    
            
private: //Data
    
    /**
     * Reference to the server object
     */
    const CPosIndicatorServerCore& iServerCore;
    
    /**
     * Pointer to the sub-session registry
     * Own
     */ 
    CPosIndicatorSubSessionRegistry* iSubSessionRegistry;
    };

#endif /* POSINDICATORSERVERSESSION_H */
