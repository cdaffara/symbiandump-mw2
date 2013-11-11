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
* Description: Declaration of connection timer class.
*
*/

#ifndef POSREVGEOCODECONNECTIONTIMER_H
#define POSREVGEOCODECONNECTIONTIMER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

class MPosRevGeoCodeTimeoutObserver
    {
public:
    virtual void HandleTimedoutEvent( TInt aErrorCode ) = 0;
    };


/**
 * This is the timer class responsible for notifying client's on 
 * time out specified by the client.
 */
class CPosRevGeoCodeConnectionTimer : public CTimer
    {
public:
    /**
     *  Factory function to create the instance of CPosRevGeoCodeConnectionTimer object.
     */
    static CPosRevGeoCodeConnectionTimer* NewL( MPosRevGeoCodeTimeoutObserver&
                                                aConnectionTimeoutHandlerInterface);
    
    /**
     * Destructor
     */
    ~CPosRevGeoCodeConnectionTimer();
    
    /**
     * Starts the timer with specified time out.
     * @return KErrInUse If timer is already running.
     */
    TInt StartTimer( const TInt aTimeoutVal );

     
protected: // From CTimer
    void RunL();
      
private:
    /**
     * Symbian 2nd phase constructor
     */  
    void ConstructL();
    
    /**
     * Constructor
     */
    CPosRevGeoCodeConnectionTimer( MPosRevGeoCodeTimeoutObserver& aConnectionTimeoutHandler );

private: // Data member
    /**
     * Reference to connection time out observer
     */
    MPosRevGeoCodeTimeoutObserver& iConnectionTimeoutObserver;
    };


#endif /* POSREVGEOCODECONNECTIONTIMER_H */
