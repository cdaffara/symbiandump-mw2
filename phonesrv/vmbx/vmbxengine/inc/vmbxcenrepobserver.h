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
* Description: Access class to Notify CenRep changed.
*  Interface   : Private, CVmbxCenRepObserver
 *
*/

#ifndef C_VMBXCENREPOBSERVER_H
#define C_VMBXCENREPOBSERVER_H

// INCLUDES
#include <e32base.h>

// for remote number change
#include <cenrepnotifyhandler.h>
#include "mvmbxcenrepobserver.h"


// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
 *  CVmbxCenRepObserver declaration.
 *
 */
NONSHARABLE_CLASS( CVmbxCenRepObserver ) : public CBase,
                                           public MCenRepNotifyHandlerCallback
    {

public: // Constructors and destructor

    /**
    * Two-phased constructor.
     * @param in aObserver observer notify the CenRep changed.
     * @return New instance of the object. Ownership transferred.
    */
    static CVmbxCenRepObserver* NewL( MVmbxCenRepObserver& aObserver );

    /**
     * Two-phased constructor.
     * @param in aObserver observer notify the CenRep changed.
     * @return New instance of the object. Ownership transferred.
     */
    static CVmbxCenRepObserver* NewLC( MVmbxCenRepObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CVmbxCenRepObserver();


private: // from base class MCenRepNotifyHandlerCallback

    /**
     * @see MCenRepNotifyHandlerCallback::HandleNotifyGeneric
     */
    void HandleNotifyGeneric( TUint32 aId );

private:

    /**
     * C++ default constructor.
     * @param in aObserver observer notify the CenRep changed.
     */
    CVmbxCenRepObserver( MVmbxCenRepObserver& aObserver );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data
    /**
     * Central repository session
     * Own.
     */
    CRepository* iCRSession;

    /**
     * Central repository observer
     * Own.
     */
    CCenRepNotifyHandler* iNotifyHandler;

    /**
     * Reference to MVmbxCenRepObserver
     * 
     */
    MVmbxCenRepObserver& iObserver;

    };

#endif // C_VMBXCENREPOBSERVER_H

