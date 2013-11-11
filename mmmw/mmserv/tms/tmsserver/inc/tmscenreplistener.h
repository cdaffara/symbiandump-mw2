/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  This file contains the header of CenRep Listener class.
 *
 */

#ifndef TMSCENREPLISTENER_H
#define TMSCENREPLISTENER_H

// INCLUDE FILES
#include <glib.h>
#include <e32base.h>
#include <centralrepository.h>

namespace TMS {

// Forward declarations
class TMSCenRepObserver;

/**
 *   TMSCenRepListener manages getting notification on CenRep key changes.
 */
class TMSCenRepListener : public CActive
    {
public:

    /**
     * Two-phased construction.
     *
     * @param aUid cenrep Uid
     * @param aKey key to be listened
     * @param aObserver observer for key change.
     * @return instance of the class
     */
    static TMSCenRepListener* NewL(TUid aUid, TUint32 aKey,
            TMSCenRepObserver* aObserver);

    /**
     * Destructor.
     */
    virtual ~TMSCenRepListener();

public:

    /**
     * Gets the current value of the monitored setting
     * @param aValue the current value of the monitored setting
     * @return gint Symbian OS error code from central repository
     */
    gint Get(gint& aValue);

    gint Set(gint aValue);

protected:
    // From CActive
    void DoCancel();
    gint RunError(TInt aError);
    void RunL();

private:

    /**
     * Constructor
     * @param aUid cenrep Uid
     * @param aKey key to be listened
     * @param aObserver observer for key change.
     */
    TMSCenRepListener(TUid aUid, TUint32 aKey, TMSCenRepObserver* aObserver);

    /**
     * Private constructing.
     */
    void ConstructL();

    /**
     * Submits a notification request
     */
    void SubmitNotifyRequestL();

private:
    // Owned by this object

    /**
     * UID of CR key.
     */
    TUid iUid;

    /**
     * Repository access.
     * Own.
     */
    CRepository* iRepository;

    /**
     * Identification number of the monitored setting.
     */
    TUint32 iMonitorSetting;

    TMSCenRepObserver* iObserver;
    };

} //namespace TMS

#endif // TMSCENREPLISTENER_H
