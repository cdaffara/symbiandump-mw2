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

#ifndef POSINDICATORSERVERSUBSESSION_H
#define POSINDICATORSERVERSUBSESSION_H

#include <e32base.h>

class CPosIndicatorLocationRequestor;

class CPosIndicatorServerSubsession : public CObject
    {
public:    
    /**
     * Instantiates a new object of 
     * CPosIndicatorServerSubsession
     *
     * @return a pointer to instance of CLbtSubSession
     */
    static CPosIndicatorServerSubsession* NewL( CPosIndicatorLocationRequestor&
                                                aLocationRequestor );
    
    /**
     * C++ destructor
     *
     */
    ~CPosIndicatorServerSubsession();
    
    /**
     * Handles the servicing of a client request that 
     * has been passed from the session
     *
     * @aMessage The message containing the details of 
     * the client request
     *
     */      
    void ServiceL(const RMessage2 &aMessage);
    
private:
    /**
     * C++ Default constructor
     *
     */
    CPosIndicatorServerSubsession( CPosIndicatorLocationRequestor&
                                   aLocationRequestor );
    
    /**
     * 2nd phase constructor for instantiating 
     * member variables
     *
     */
    void ConstructL();

private: // Data member
    /**
     * Pointer to location requestor object.
     * Not own
     */
    CPosIndicatorLocationRequestor& iLocationRequestor;
    };
#endif /* POSINDICATORSERVERSUBSESSION_H */
