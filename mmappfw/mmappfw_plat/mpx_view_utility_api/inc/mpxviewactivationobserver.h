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
* Description:  MPX view activcation observer interface definition
*
*/



#ifndef MMPXVIEWACTIVATIONOBSERVER_H
#define MMPXVIEWACTIVATIONOBSERVER_H


// INCLUDES
#include <e32std.h>


// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
 *  MPX view activation observer.
 *
 *  @lib mpxviewutility.lib
 */
NONSHARABLE_CLASS( MMPXViewActivationObserver )
    {
public:
    enum TViewUpdateEvent
        {
        EViewPluginAdd,
        EViewPluginUpdate,
        EViewPluginRemove
        };

public:

    /**
     * Handle view activation.
     *
     * @since S60 3.2.3
     * @param aCurrentViewType Current view type Uid.
     * @param aPreviousViewType Previous view type Uid.
     */
    virtual void HandleViewActivation(
        const TUid& aCurrentViewType,
        const TUid& aPreviousViewType ) = 0;

    /**
     * Called in case a view plugin is added/updated/removed through a SIS install.
     *
     * @since S60 3.2.3
     * @param aViewUid view UID
     * @param aUpdateEvent Defines the update type (addition/update/removal).
     * @param aLoaded ETrue if the view plugin is loaded, EFalse otherwise. It is recommended
     *          that client applications exit when loaded plugins are updated or removed.
     *          This would normally be handled in the AppUi.
     * @param aData Its meaning depends on the update event:
     *            EViewPluginAdd - new view plugin version
     *            EViewPluginUpdate - new view plugin version
     *            EViewPluginRemove - old view plugin version
     */
    virtual void HandleViewUpdate(
        TUid /* aViewUid */,
        TViewUpdateEvent /* aUpdateEvent */,
        TBool /* aLoaded */,
        TInt /* aData */) {};
    };

#endif  // MMPXVIEWACTIVATIONOBSERVER_H

// End of File
