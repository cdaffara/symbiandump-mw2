/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsMcnListener class.
*
*/


#ifndef CCBSMCNLISTENER_H
#define CCBSMCNLISTENER_H

//  INCLUDES
#include <e32base.h>    // CActive
#include <cbsmcncommon.h>

//  FORWARD DECLARATIONS
class CMcn;
class RCbsMcnSession;

//  CLASS DECLARATION

/**
*   Listens for incoming MCN messages. Functionality is based on basic
*   active object implementation.
*/
class CCbsMcnListener 
    : public CActive
    {
public:             // New functions
    /**
    * Creates a new Mcn listener.
    * @return A new CCbsMcnListener instance.
    */
    static CCbsMcnListener* NewL( CMcn& aMcn, RCbsMcnSession& aSession );
    
    /**
    * Destructor.
    */
    ~CCbsMcnListener();

private:             // From CActive

    /**
    * This method is called when an Mcn message is received.
    * The message is stored in the buffer.
    */
    void RunL();

    /**
    * Cancels the outstanding Mcn request.
    */
    void DoCancel();

private:
    /**
        Constructor.
    */
    CCbsMcnListener( CMcn& aMcn, RCbsMcnSession& aSession );

    /**
    * 2nd-phase constructor.
    */
    void ConstructL();

    /**
    * Receive an Mcn message from network.
    */
    void Receive();

private:            // Data

    // The Mcn client to use for reception.
    CMcn& iMcn;

    // Session to the Mcn client.
    RCbsMcnSession& iSession;

    // MCN message for IPC
    TCbsMcnMessage iMcnMessage;

    // Package for MCN message
    TPckg<TCbsMcnMessage> iMcnPckg;
    };

#endif      // CCBSMCNLISTENER_H
            
// End of File


