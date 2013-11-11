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
* Description:  This file contains the header file of the CCbsScheduler class.
*
*/


#ifndef CCBSSCHEDULER_H
#define CCBSSCHEDULER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CCbsServer;

//  CLASS DECLARATION 

/**
*   CbsServer includes a scheduler of it's own to simplify
*   client request handling code. All active object leaves are
*   handled in function Error(). The behaviour of Error()
*   depends on whether there is an outstanding client request
*   or not: it will tell the client the reason for inability
*   to handle the client request. It will also panic the
*   client if the descriptor passed to the server was malformed.
*   Finally, a panic is raised if the error does not originate
*   from the server itself.
*
*   See Tasker: Professional Symbian Programming, Chapter 20
*   for details.
*/
class CCbsScheduler : public CActiveScheduler
    {
public:     // new

    /**
    *   Default constructor  
    */
    CCbsScheduler();
            
    /**
    *   Gives a pointer of the current CCbsServer instance
    *   to the scheduler.
    *
    *   @param      aServer     The current server pointer
    */
    void SetServer( CCbsServer* aServer );

public:     // from CActiveScheduler

    /**
    *   Overridden to handle leaves in request functions and
    *   unexpected situations.
    *
    *   @param      aError      Error code, 
    *                           see Symbian OS system documentation.
    */
    void Error( TInt aError ) const;

private:    // prohibited operators and functions

    // Copy constructor
    CCbsScheduler( const CCbsScheduler& );

    // Assignment operator
    CCbsScheduler& operator=( const CCbsScheduler& );

private:    // data

    // Ref: server instance
    CCbsServer* iServer;
    };

#endif // CCBSSCHEDULER_H   
            
// End of File


