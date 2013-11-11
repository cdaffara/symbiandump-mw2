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
* Description:  MPX view utility interface definition
*
*/



#ifndef MMPXVIEWUTILITY_H
#define MMPXVIEWUTILITY_H


// INCLUDES
#include <e32base.h>
#include <mpxviewactivationobserver.h>


// FORWARD DECLARATIONS
class MMPXViewPluginManager;
class CMPXViewPlugin;


// CLASS DECLARATION

/**
 *  MPX view utility interface.
 *
 *  @lib mpxviewutility.lib
 */
NONSHARABLE_CLASS( MMPXViewUtility )
    {
public:

    /**
     * Retrieve a view utility of this thread.  All clients in this thread
     * shares the same viewutility (singleton).
     * Call Close() when this object is not needed anymore.
     *
     * @since S60 3.2.3
     * @return Pointer to view utility. Ownership not transferred.
     */
    IMPORT_C static MMPXViewUtility* UtilityL();

    /**
     * This method must be called when this object can be freed. This object
     * will be destroyed when no one is using it.
     *
     * @since S60 3.2.3
     */
    virtual void Close() = 0;

    /**
     * Activate view by resolving passed uid.
     *
     * @since S60 3.2.3
     * @param aUid Uid to be resolved. This uid can be plugin implementation
     *             uid, plugin type uid or specific view plugin uid.
     * @param aParam Parameter passed to the view on activation
     * @leave KErrNotFound if a matching plugin cannot be found
     */
    virtual void ActivateViewL(
        const TUid& aUid,
        const TDesC* aParam = NULL ) = 0;

    /**
     * Activate view by resolving passed uids.
     *
     * @since S60 3.2.3
     * @param aUids List of Uids to be resolved, this uid can be plugin uid,
     *              plugin type uid or specific view plugin uid.
     * @param aParam Parameter passed to the view on activation
     * @leave KErrNotFound if a matching plugin cannot be found
     */
    virtual void ActivateViewL(
        const RArray<TUid>& aUids,
        const TDesC* aParam = NULL ) = 0;

    /**
     * Set view as default view, not bring to foreground. This is only valid
     * if there are no views previously activated.
     *
     * @since S60 3.2.3
     * @param aUid Uid of item to be activated. This uid can be plugin implementation
     *             uid, plugin type uid or specific view plugin uid.
     * @leave Error code KErrAlreadyExist when there are views already
     *        activated
     * @leave Error code KErrNotSupported for views that doesn't support
     *        default view (such as dialogs)
     */
    virtual void SetAsDefaultViewL( const TUid& aUid ) = 0;

     /**
     * Set view as default view, not bring to foreground. This is only valid
     * if there are no views previously activated.
     *
     * @since S60 3.2.3
     * @param aUid Uids of item to be activated. This uid can be plugin implementation
     *             uid, plugin type uid or specific view plugin uid.
     * @leave Error code KErrAlreadyExist when there are views already
     *        activated
     * @leave Error code KErrNotSupported for views that doesn't support
     *        default view (such as dialogs)
     */
    virtual void SetAsDefaultViewL( const RArray<TUid>& aUids ) = 0;

    /**
     * Activate previous view.
     *
     * @since S60 3.2.3
     *
     * @leave Error code KErrNotFound if there's no history.
     */
    virtual void ActivatePreviousViewL() = 0;

    /**
     * Pre-load view plug-ins that have EMPXPluginFlagPreLoad flag set.
     *
     * @since S60 3.2.3
     */
    virtual void PreLoadViewsL() = 0;
	
    /**
     * Pre-load view plug-ins that have EMPXPluginFlagPreLoad flag and also by resolving passed uid.
     *
     * @param aUid Uid of specific application to be resolved.
     */
    virtual void PreLoadViewsL( const TUid& aUid ) = 0;

    /**
     * Pre-load view plug-in by resolving passed uid.
     *
     * @since S60 3.2.3
     * @param aUid Uid to be resolved. This uid can be plugin uid, plugin type
     *             uid or specific view plugin uid.
     * @leave if no match is found.
     */
    virtual void PreLoadViewL( const TUid& aUid ) = 0;

    /**
     * Get a reference to view plugin manager.
     *
     * @since S60 3.2.3
     * @return Reference to view plugin manager.
     */
    virtual MMPXViewPluginManager& ViewPluginManager() = 0;

    /**
     * Constructs default view history.
     *
     * @since S60 3.2.3
     * @return if defined, the default view UID.  KNullUid if not defined
     */
    virtual TUid ConstructDefaultViewHistoryL() = 0;

    /**
     * Push default history to view history.  The current browse history
     * is erased and the default browsing path is loaded as the current history.
     * @since S60 3.2.3
     */
    virtual void PushDefaultHistoryL() = 0;

    /**
     * Gets default view's UID.
     *
     * @since S60 3.2.3
     * @return if defined, the default view UID.  KNullUid if not defined
     */
    virtual TUid DefaultViewUid() = 0;

    /**
     * Returns the current history depth.
     * if depth is greater than 1, the client should call ActivatePreviousViewL
     * on a back command.
     *
     * if depth is 1, the client should exit the app on a back command instead
     * of calling ActivatePreviousViewL.
     *
     * @since S60 3.2.3
     *
     * @return The current browsing history depth.
     */
    virtual TInt ViewHistoryDepth() = 0;

    /**
     * Returns the last activated view type.
     *
     * @since S60 3.2.3
     * @return previous view type if defined.  KNullUid if not.
     */
    virtual TUid PreviousViewType() = 0;

    /**
     * Returns the view type of the active view.
     *
     * @since S60 3.2.3
     * @return current view type if defined.  KNullUid if not.
     */
    virtual TUid ActiveViewType() = 0;

    /**
     * Returns the implementation uid of the active view.
     *
     * @since S60 3.2.3
     * @return current view implementation id if defined.  KNullUid if not.
     */
    virtual TUid ActiveViewImplementationUid() = 0;

    /**
     * Add view activation observer.
     *
     * @since S60 3.2.3
     * @param aObserver Pointer to an observer object
     */
    virtual void AddObserverL(
        MMPXViewActivationObserver* aObserver ) = 0;

    /**
     * Remove view activation observer.
     *
     * @since S60 3.2.3
     * @param aObserver Pointer to an observer object
     */
    virtual void RemoveObserver(
        MMPXViewActivationObserver* aObserver ) = 0;
    };

#endif  // MMPXVIEWUTILITY_H

// End of File
