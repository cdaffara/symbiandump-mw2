/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer to Sim Store changed
*  Interface   : Private, CVmbxSimStoreObserver
*
*/


#ifndef C_VMBXSIMSTOREOBSERVERR_H
#define C_VMBXSIMSTOREOBSERVERR_H

// INCLUDES
#include <e32base.h>
#include "mvmbxsimstoreobserver.h"

// FORWARD DECLARATIONS
class CVmbxSimHandler;

// CLASS DECLARATION

/**
*  DECLARATION CVmbxSimStoreObserver
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxSimStoreObserver ) : public CActive
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param in aSimHandler Reference to Sim handler
     * @param in aObserver observer of MVmbxSimStoreObserver
     */
    static CVmbxSimStoreObserver* NewL( CVmbxSimHandler& aSimHandler,
                                    MVmbxSimStoreObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CVmbxSimStoreObserver();

public: // New functions

    /**
    * Activates this notification. Call ETel NotifyStoreEvent.
    */
    void ActivateL();

private:

    /**
     * C++ default constructor.
     * @param in aSimHandler Reference to Sim handler
     * @param in aObserver observer of MVmbxSimStoreObserver
     */
    CVmbxSimStoreObserver( CVmbxSimHandler& aSimHandler,
                        MVmbxSimStoreObserver& aObserver );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Informs SimHandler about changed number
     */
    void NotifySimHandler();

// from base class CActive
private: 
    /**
    * @see CActive::RunL
    */
    void RunL();

    /**
    * @see CActive::DoCancel
    */
    void DoCancel();


private: // data

    /**
     * The index of the entry that has been changed
     */
    TInt iIndex;

    /**
     * The events that ETel sends
     */
    TUint32 iEvents;

    /**
     * CVmbxSimHandler reference
     * 
     */
    CVmbxSimHandler& iSimHandler;

    /**
     * MVmbxSimStoreObserver reference
     * 
     */
    MVmbxSimStoreObserver& iObserver;

    };

#endif  // C_VMBXSIMSTOREOBSERVERR_H
