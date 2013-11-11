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
* Description:  Observer to Sat refresh
*  Interface   : Private, CVmbxSatRefreshObserver
*
*/


#ifndef C_VMBXSATREFRESHOBSERVERR_H
#define C_VMBXSATREFRESHOBSERVERR_H

// INCLUDES
#include <e32base.h>
#include <rsatsession.h>
#include <rsatrefresh.h>
#include <msatrefreshobserver.h>
#include "mvmbxsatrefreshobserver.h"


// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  Implements functionality specifig to voice mailbox entry
*

*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxSatRefreshObserver ) : public CBase,
                              public MSatRefreshObserver
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param in aObserver observer which want to notify sat refresh
     */
    static CVmbxSatRefreshObserver* NewL( MVmbxSatRefreshObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CVmbxSatRefreshObserver();

public: // from base class MSatRefreshObserver

    /**
     * @See MSatRefreshObserver::AllowRefresh
     */
    TBool AllowRefresh( TSatRefreshType aType,
                        const TSatRefreshFiles& aFiles ) ;

    /**
     * @See MSatRefreshObserver::Refresh
     */
    void Refresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles );

private:

    /**
     * C++ default constructor.
     * @param aObserver Reference to MVmbxSatRefreshObserver
     */
    CVmbxSatRefreshObserver( MVmbxSatRefreshObserver& aObserver);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data

    /**
     * Sat server session
     * Own
     */
    RSatSession iRSatSession;

    /**
     * Sat refresh connection
     * Own
     */
    RSatRefresh iSatRefresh;

    /**
     * A package for the TRefreshFileList.
     * list of files, waiting for notification.
     */
    RSat::TRefreshFileList iObservedFileList;

    /**
     * Reference to MVmbxSatRefreshObserver
     */
    MVmbxSatRefreshObserver& iObserver;
    };

#endif  // C_VMBXSATREFRESHOBSERVERR_H
