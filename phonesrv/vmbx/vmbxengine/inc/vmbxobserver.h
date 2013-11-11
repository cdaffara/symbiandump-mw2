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
* Description:  Observer class to data refresh
*  Interface   : Private, CVmbxObserver
*
*/

#ifndef C_VMBXOBSERVER_H
#define C_VMBXOBSERVER_H

// INCLUDES
#include <e32base.h>

#include "mvmbxchangeobserver.h"
#include "mvmbxsimstoreobserver.h"
#include "mvmbxsatrefreshobserver.h"
#include "mvmbxcenrepobserver.h"
#include "mvmbxresourceprovider.h"
#include "voicemailboxdefsinternal.h"


// FORWARD DECLARATIONS
class CVmbxSatRefreshObserver;
class CVmbxSimStoreObserver;
class CVmbxCenRepObserver;
class CVmbxSimHandler;

// CLASS DECLARATION

/**
 *  Mailbox observer
 *
 *  @lib vmbxengine.lib
 */
NONSHARABLE_CLASS( CVmbxObserver ) : public CBase,
                                     public MVmbxSimStoreObserver,
                                     public MVmbxSatRefreshObserver,
                                     public MVmbxCenRepObserver
    {

public: // Constructors and destructor

    /**
     * Two-phased constructor.
     * @param in aProvider Reference to MVmbxResourceProvider
    */
    static CVmbxObserver* NewL( MVmbxResourceProvider& aProvider );

    /**
     * Two-phased constructor.
     * @param in aProvider Reference to MVmbxResourceProvider
     */
    static CVmbxObserver* NewLC( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxObserver();

public: // New functions

    /**
     * Observe vmbx change.
     * @param in aObserver Reference to MVmbxChangeObserver
     */
    void SetVmbxObserver( MVmbxChangeObserver& aObserver );

public: // From base class

    /**
     * From MVmbxSimStoreObserver::HandleSimStoreChanged
     */
    void HandleSimStoreChanged();

    /**
     * From MVmbxSatRefreshObserver::HandleSatRefresh
     */
    void HandleSatRefresh();

    /**
     * From MVmbxCenRepObserver::HandleCenRepChange
     * @param in aId the changed key Id
     */
    void HandleCenRepChange( TVmbxCenRepKey aId );

private:

    /**
     * C++ default constructor.
     * @param in aProvider Reference to MVmbxResourceProvider
     */
    CVmbxObserver( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data

    /**
     * Pointer to MVmbxChangeObserver
     * Own.
     */
    MVmbxChangeObserver* iObserver;

    /**
     * Reference to MVmbxResourceProvider
     * 
     */
    MVmbxResourceProvider& iProvider;

    /**
     *Pointer to  CVmbxCenRepHandler
     * Own.
     */
    CVmbxCenRepObserver* iCenRepObserver;

    /**
     * Pointer to CVmbxSimHandler
     * Own.
     */
    CVmbxSimHandler* iSimHandler;

    /**
     * CVmbxSatRefreshObserver
     * Own.
     */
    CVmbxSatRefreshObserver* iSatObserver;

    /**
     * CVmbxSimStoreObserver
     * Own.
     */
    CVmbxSimStoreObserver* iSimObserver;
    };

#endif // C_VMBXOBSERVER_H
