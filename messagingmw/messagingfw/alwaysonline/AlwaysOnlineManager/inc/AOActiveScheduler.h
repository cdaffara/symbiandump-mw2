/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Always Online class
*
*/


#ifndef __CAOACTIVESCHEDULER_H__
#define __CAOACTIVESCHEDULER_H__

// INCLUDES
#include    <e32std.h>

#include "AlwaysOnlineManagerClient.h"

class CAlwaysOnlineManagerServer;

// CLASS DECLARATION
/**
 * Active Scheduler
 *
 * Own derived CActiveScheduler which is overwritting
 * Error() function so it does not panic there as default implementation
 *
 * @since S60 v3.1
 */
NONSHARABLE_CLASS( CAOActiveScheduler ):
    public CActiveScheduler
    {
    public:
    
    /**
    * C++ constructor
    * @since S60 v3.1
    * @param aAoServer reference to ao server object
    */
    CAOActiveScheduler();

    /**
    * C++ destructor
    * @since S60 v3.1
    */
    virtual ~CAOActiveScheduler();

    /**
    * From CActiveScheduler
    *
    * If some active object returns error code from RunError()
    * this function is called, implementation broadcasts 
    * EServerAPIBaseCommandAoSchedulerError to every ao plugin.
    *
    * NOTE: This error must be handled in HandleServerCommandL() 
    * if there is a risk that some plugin's active object returns 
    * something else than KErrNone from RunError() method.
    * 
    * @since S60 v3.1
    * @param anError Error code from RunError function
    */
    virtual void Error( TInt anError ) const;
    
    /**
    *
    * Setter function for AO server object pointer, this can't set
    * in constructor because Active scheduler have to be
    * installed before creation of AO server object
    * (chicken - egg problem)
    * 
    * @since S60 v3.1
    * @param aAoServer pointer to AoServer object, NOT OWNED
    */
    void SetServerRef( CAlwaysOnlineManagerServer* aAoServer );
    
    private:
    
    // pointer to ao server class (NOT OWNED)
    CAlwaysOnlineManagerServer* iAoServer;
    };


#endif      // __CAOACTIVESCHEDULER_H__
