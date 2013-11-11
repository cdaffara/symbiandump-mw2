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
* Description:  MPX view utility implementation
*
*/



#ifndef CMPXVIEWUTILITYIMP_H
#define CMPXVIEWUTILITYIMP_H


// INCLUDES
#include <mpxcustomcommandobserver.h>
#include <mpxpluginhandlerobserver.h>
#include "mpxviewutility.h"

// FORWARD DECLARATIONS
class CMPXViewPluginHandler;
class CRepository;
class CMPXViewPlugin;
class CMPXViewHistory;

// CLASS DECLARATION

/**
 *  MPX view utility implementation.
 *
 *  @lib mpxviewutility.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CMPXViewUtilityImp ) : public CBase,
                                          public MMPXViewUtility,
                                          public MMPXCustomCommandObserver,
                                          public MMPXPluginHandlerObserver
    {
public:

    /**
     * Retrieves view utility from TLS. Creates if it's not there.
     *
     * @since 3.1
     * @return Pointer to a view utility object.
     */
    static MMPXViewUtility* UtilityL();

private:

    /**
     * C++ default constructor.
     */
    CMPXViewUtilityImp();

    /**
     * Destructor.
     */
    virtual ~CMPXViewUtilityImp();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Two-phased constructor.
     * Intentionally make this API private as this class is a singleton.
     * To get a view utility instance, UtilityL() should be called.
     *
     * @return Pointer to newly created object.
     */
    static CMPXViewUtilityImp* NewL();

    /**
     * Store view utility to TLS.
     *
     * @param aUtility View utility object. Ownership not transferred.
     */
    static void StoreViewUtility( MMPXViewUtility* aUtility );

// from base class MMPXViewUtility

    /**
     * From MMPXViewUtility
     * This method must be called when the client no longer needs the utility
     * instance. This object will be destroyed when no one is using it.
     *
     * @since 3.1
     */
    void Close();

    /**
     * From MMPXViewUtility
     * Activate view by resolving passed uid.
     *
     * @since 3.1
     * @param aUid Uid to be resolved. This uid can be plugin implementation
     *             uid, plugin type uid or specific view plugin uid.
     * @param aParam Parameter passed to the view on activation
     * @leave KErrNotFound if a matching plugin cannot be found
     */
    void ActivateViewL(
        const TUid& aUid,
        const TDesC* aParam = NULL );

    /**
     * From MMPXViewUtility
     * Activate view by resolving passed uids.
     *
     * @since 3.1
     * @param aUids List of Uids to be resolved, this uid can be plugin uid,
     *              plugin type uid or specific view plugin uid.
     * @param aParam Parameter passed to the view on activation
     * @leave KErrNotFound if a matching plugin cannot be found
     */
    void ActivateViewL(
        const RArray<TUid>& aUids,
        const TDesC* aParam = NULL );

    /**
     * From MMPXViewUtility
     * Set view as default view, not bring to foreground. This is only valid
     * if there are no views previously activated
     *
     * @since 3.1
     * @param aUid Uid of item to be activated. This uid can be plugin implementation
     *             uid, plugin type uid or specific view plugin uid.
     * @leave Error code KErrAlreadyExist when there are views already
     *        activated
     * @leave Error code KErrNotSupported for views that doesn't support
     *        default view (such as dialogs)
     */
    void SetAsDefaultViewL( const TUid& aUid );

    /**
     * From MMPXViewUtility
     * Set view as default view, not bring to foreground. This is only valid
     * if there are no views previously activated
     *
     * @since 3.1
     * @param aUid Uids of item to be activated. This uid can be plugin implementation
     *             uid, plugin type uid or specific view plugin uid.
     * @leave Error code KErrAlreadyExist when there are views already
     *        activated
     * @leave Error code KErrNotSupported for views that doesn't support
     *        default view (such as dialogs)
     */
    void SetAsDefaultViewL( const RArray<TUid>& aUids );

    /**
     * From MMPXViewUtility
     * Activate previous view.
     *
     * @since 3.1
     *
     * @leave Error code KErrNotFound if there's no history.
     */
    void ActivatePreviousViewL();

    /**
     * From MMPXViewUtility
     * Pre-load view plug-ins that have KMPXVpFeaturePreLoaded flag set.
     *
     * @since 3.1
     */
    void PreLoadViewsL();

    /**
     * From MMPXViewUtility
     * Pre-load view plug-ins that have KMPXVpFeaturePreLoaded flag set and also by resolving aUid.
     *
     * @param aUid Uid of specific applicaiton to be resolved
     */
    void PreLoadViewsL(const TUid& aUid);
	
    /**
     * From MMPXViewUtility
     * Pre-load view plug-in by resolving passed uid.
     *
     * @since 3.1
     * @param aUid Uid to be resolved. This uid can be plugin uid, plugin type
     *             uid or specific view plugin uid.
     * @leave if no match is found.
     */
    void PreLoadViewL( const TUid& aUid );

    /**
     * From MMPXViewUtility
     * Get a reference to view plugin manager.
     *
     * @since 3.1
     * @return Reference to view plugin manager.
     */
    MMPXViewPluginManager& ViewPluginManager();

    /**
     * From MMPXViewUtility
     * Constructs default view history
     *
     * @since 3.1
     * @return if defined, the default view UID.  KNullUid if not defined
     */
    TUid ConstructDefaultViewHistoryL();

    /**
     * From MMPXViewUtility
     *
     * Push default history to view history.  The current browse history
     * is erased and the default browsing path is loaded as the current history
     *
     * @since 3.1
     */
    void PushDefaultHistoryL();

    /**
     * From MMPXViewUtility
     * Gets default view's UID
     *
     * @return if defined, the default view UID.  KNullUid if not defined
     *
     * @since 3.1
     */
    TUid DefaultViewUid();

    /**
     * From MMPXViewUtility
     * Returns the current history depth
     * if depth is greater than 1, the client should call ActivatePreviousViewL
     * on a back command
     *
     * if depth is 1, the client should exit the app on a back command instead
     * of calling ActivatePreviousViewL
     *
     * @since 3.1
     *
     * @return The current browsing history depth.
     */
    TInt ViewHistoryDepth();

    /**
     * From MMPXViewUtility
     * Returns the last activated view type
     *
     * @return previous view type if defined.  KNullUid if not.
     *
     * @since 3.1
     */
    TUid PreviousViewType();

    /**
     * From MMPXViewUtility
     * Returns the view type of the active view.
     *
     * @return current view type if defined.  KNullUid if not.
     *
     * @since 3.1
     */
    TUid ActiveViewType();

    /**
     * From MMPXViewUtility
     * Returns the implementation uid of the active view.
     *
     * @return current view implementation id if defined.  KNullUid if not.
     *
     * @since 3.1
     */
    TUid ActiveViewImplementationUid();

    /**
     * From MMPXViewUtility
     * Add view activation observer.
     *
     * @since 3.1
     * @param aObserver Pointer to an observer object
     */
    void AddObserverL(
        MMPXViewActivationObserver* aObserver );

    /**
     * From MMPXViewUtility
     * Remove view activation observer.
     *
     * @since 3.1
     * @param aObserver Pointer to an observer object
     */
    void RemoveObserver(
        MMPXViewActivationObserver* aObserver );

    /**
     * From MMPXCustomCommandObserver
     * Handle custom commands.
     *
     * @since 3.1
     * @param aUid Command category Uid. Different command set will have
     *             different Uid to avoid clash.
     * @param aCommand Command to be handled.
     * @param aData data for the specific command
     */
    void HandleCustomCommand(
        const TUid& aUid,
        const TInt aCommand,
        const TDesC8& aData = KNullDesC8 );

    /*
    * From MMPXPluginHandlerObserver
    * @see MMPXPluginHandlerObserver
    */
    void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent, const TUid& aPluginUid,
        TInt aLoaded, TInt aData);

private:

    /**
     * Activate view by resolving passed uids.
     *
     * @since 3.1
     * @param aUids List of Uids to be resolved, this uid can be plugin uid,
     *              plugin type uid or specific view plugin uid.
     * @param aIsActivatingPreviousView ETrue if activating previous view
     * @param aUpdatedPreviousView ETrue iPreviousView has been updated or no need
     *                             EFalse will update iPreviousView
     * @param aParam Parameter passed to the view on activation
     */
    void ActivateViewL(
        const RArray<TUid>& aUids,
        TBool aIsActivatingPreviousView,
        TBool aUpdatedPreviousView,
        const TDesC* aParam = NULL );

    /**
     * Marks the view closed.  This does not actually close the view.
     * When a dialog is closed, it should call this method to indicate it has been closed.
     *
     * @since 3.1
     * @param aPlugin a pointer to the current dialog
     */
    void MarkViewAsClosedL( CMPXViewPlugin* aPlugin );

    /**
     * Notify observers on view activation.
     *
     * @param aCurrentViewType Current view type Uid
     * @param aPreviousViewType Previous view type Uid
     */
    void NotifyObservers(
        const TUid& aCurrentViewType,
        const TUid& aPreviousViewType );

    /**
     * Notify observers on view update events (addition/update/removal).
     *
     * @param aViewUid Identifies the view.
     * @param aUpdateEvent Identifies the event (addition/update/removal).
     * @param aLoaded ETrue if the view is currently loaded
     * @param aVersion View version (new for addition and update and old for removal)
     */
    void NotifyViewUpdate(
        const TUid& aViewUid,
        MMPXViewActivationObserver::TViewUpdateEvent aUpdateEvent,
        TBool aLoaded,
        TInt aVersion = 0);

    /**
     * Insert/Append to default view history array
     *
     * @param aData data to append/insert
     * @param aLoc location to insert (default append)
     *
     * @return if successful, returns the plulgin type uid
     */
    TInt AddToDefaultViewHistoryL( const TDesC& aData, TInt aLoc = -1 );

    /**
     * Updates previous view info
     *
     * @param aIndex index in current view history to store
     */
    void UpdatePreviousViewInfoL( TInt aIndex );

    /**
     * Find if the view exist in current history
     *
     * @param aType type to search for
     * @return index of the plugin in history if found, KErrNotFound otherwise
     */
    TInt FindCurrentHistoryEntryByTypeL( const TUid aType );

private:    // data

    /**
     * View plugin handler.
     * Own.
     */
    CMPXViewPluginHandler* iPluginManager;

    /**
     * Reference count.
     */
    TInt iRefCount;

    /**
     * History paths
     */
    RPointerArray<CMPXViewHistory> iDefaultHistoryPath;
    RPointerArray<CMPXViewHistory> iCurrentHistoryPath;

    /**
     * List of observers
     * Owned.
     */
    RPointerArray<MMPXViewActivationObserver> iObservers;

    CMPXViewHistory* iPreviousView;
    CMPXViewHistory* iCurrentView;
    CMPXViewHistory* iDefaultView;
    };

#endif  // CMPXVIEWUTILITYIMP_H

// End of File
