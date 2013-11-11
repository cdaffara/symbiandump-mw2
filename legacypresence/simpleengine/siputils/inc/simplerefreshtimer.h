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
* Description:    Refreshtimer.
*
*/




#ifndef CSimpleRefreshTimer_H
#define CSimpleRefreshTimer_H

// INLUCDES
#include <e32base.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class RTimer;
class MSimpleSipConnCallback;
class CSimpleRequest;

/**
 *  Refresh timer
 *
 *  Triggers SIP PUBLISH requests before they expire.
 *
 *  @lib siputils
 *  @since S60 3.2
 */
class CSimpleRefreshTimer : public CActive
    {

//  friend classes go here:

public:


    CSimpleRefreshTimer(
        MSimpleSipConnCallback& aEngine, CSimpleRequest& aReq );

    virtual ~CSimpleRefreshTimer();

    /**
     * Start timer.
     *
     * @since S60 3.2
     * @param aSec seconds
     */
    void Start( TInt aSec );

protected:

// from base class CActive

    /**
     * From CActive.
     * Implements cancellation of an outstanding request.
     *
     * @since S60 3.2
     */
    void DoCancel( );

    /**
     * From CActive.
     * Handles an active object's request completion event.
     *
     * @since S60 3.2
     */
    void RunL( );

    /**
     * From CActive.
     * Error routine for RunL exceptions
     *
     * @since S60 3.2
     * @param aError error code
     */
    TInt RunError(TInt aError);


private: // data

    /**
     * The actual timer
     */
    RTimer                  iTimer;
    
    /**
     * Seconds to wait
     */     
    TInt                    iSeconds;
    
    /**
     * Engine core
     */
    MSimpleSipConnCallback& iEngine;
    
    /**
     * The corresponding client request
     */
    CSimpleRequest&         iRequest;

#ifdef _DEBUG
    friend class T_CSimpleRefreshTimer;
#endif
    };
    
/**
 *  Expiry timer
 *
 *  Triggers requests after they expire.
 *
 *  @lib ?siputils
 *  @since S60 v3.2
 */
class CSimpleExpiryTimer : public CActive
    {

//  friend classes go here:

public:

    CSimpleExpiryTimer(
        MSimpleSipConnCallback& aEngine, CSimpleRequest& aReq );

    virtual ~CSimpleExpiryTimer();

    /**
     * Start timer.
     *
     * @since S60 3.2
     * @param aSec seconds
     */
    void Start( TInt aSec );

protected:

// from base class CActive

    /**
     * From CActive.
     * Implements cancellation of an outstanding request.
     *
     * @since S60 3.2
     */
    void DoCancel( );

    /**
     * From CActive.
     * Handles an active object's request completion event.
     *
     * @since S60 3.2
     */
    void RunL( );

    /**
     * From CActive.
     * Error routine for RunL exceptions
     *
     * @since S60 3.2
     * @param aError error code
     */
    TInt RunError(TInt aError);

private: // data

    /**
     * The actual timer
     */
    RTimer                  iTimer;
    
    /**
     * Seconds to wait
     */     
    TInt                    iSeconds;
    
    /**
     * Engine core
     */
    MSimpleSipConnCallback& iEngine;
    
    /**
     * The corresponding client request
     */
    CSimpleRequest&         iRequest;

#ifdef _DEBUG
    friend class T_CSimpleRefreshTimer;
#endif
    };    

#endif 
