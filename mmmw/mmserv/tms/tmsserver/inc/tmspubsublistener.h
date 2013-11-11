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
 * Description:  Declarations for class TMSPubSubListener
 *
 */

#ifndef TMSPUBSUBLISTENER_H
#define TMSPUBSUBLISTENER_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <e32property.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSPubSubObserver;

// CLASS DECLARATION

/**
 *  RProperty poller.
 */
class TMSPubSubListener : public CActive
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     * @param aUid the Uid to use
     * @param aKey item's key
     * @param aObserver callback interface for notification
     * @return instance of CSPPubSubListener
     */
    static TMSPubSubListener* NewL(const TUid aUid, const TInt aKey,
            TMSPubSubObserver* aObserver);

    /**
     * Destructor.
     */
    virtual ~TMSPubSubListener();

public:
    // New functions.

    /**
     * Gets integer value from P & S.
     * @param aVal a value in return
     * @return error code
     */
    TInt Get(TInt& aVal);

    /**
     * Gets 8 bit string value from P&S.
     * @param aVal a value in return
     * @return error code
     */
    TInt Get(TDes8& aVal);

    /**
     * Gets 16 bit descriptor value from P&S.
     * @param aVal a value in return
     * @return error code
     */
    TInt Get(TDes16& aVal);

    /**
     * Sets integer value for P & S.
     * @param aVal a value in return
     * @return error code
     */
    TInt Set(TInt aVal);

private:
    // New functions.

    /**
     * Starts the listening (RunL).
     */
    void StartListening();

private:
    // From CActive.

    /** @see CActive::RunL */
    virtual void RunL();

    /** @see CActive::Cancel */
    virtual void DoCancel();

    /** @see CActive::RunError */
    virtual TInt RunError(TInt aError);

private:

    /**
     * Constructor of the listener.
     * @param aUid the Uid to use
     * @param aKey item's key
     * @param aObserver callback interface for notification
     */
    TMSPubSubListener(const TUid aUid, TInt aKey,
	        TMSPubSubObserver* aObserver);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    // Data

    /** UID of the monitored item. */
    TUid iUid;

    /** ID of the monitored item. */
    TInt iId;

    /** Property to subscribe to. */
    RProperty iProperty;

    /** Observer for PS notifications. */
    TMSPubSubObserver* iObserver;
    };

} //namespace TMS

#endif // TMSPUBSUBLISTENER_H

// End of File
