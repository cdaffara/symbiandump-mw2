/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/


#ifndef CONTEXTENGINEAO_H
#define CONTEXTENGINEAO_H

// system include
#include <e32base.h>

// user include
#include "contextplugin.h"
#include "mpropertyobserver.h"

// forward declarations
class CContextSnapshotItem;
class MContextInitializationObserver;
class MContextSnapshotObserver;
class CMdESession;
class CPropertyWatcher;

/**
* This class implements an active object for collecting context metadata
* asyncronously from the context plugins.
*/
NONSHARABLE_CLASS( CContextEngineAO ) : public CActive, 
									    public MContextPluginObserver,
										public MKeyObserver
  	{
public:

	/**
	 * State for context engine (=this active object).
	 * Possible states: Ready for snapshot, initializing plugins or taking a snapshot.
	 */
	enum TContextEngineState { EReady = 0, EInitialization, ESnapshot };

	/**
	 * Creates and constructs a new instance of CContextEngineAO.
	 *
	 * @return A pointer to the new instance.
	 */
	static CContextEngineAO* NewL( MContextInitializationObserver* aInitObserver,
		MContextSnapshotObserver* aSnapshotObserver );

	/**
	 * Destructor.
	 */
	virtual ~CContextEngineAO();

public:
	/**
	 * Set a pointer to MdESession. The pointer is given forward to context plugins.
	 * Session must be set in order to successfully harvest context data.
	 *
	 * @param aSession Session pointer to set.
	 */
	void SetMdeSession( CMdESession* aSession );

	/**
	 * Start taking a context snapshot (for multiple objects).
	 *
	 * @param aItem  Observer to inform about snapshot completion
	 *               and MdE objects the snapshot is taken to.
	 */
	void StartSnapshot( CContextSnapshotItem* aItem );

	/**
	 * Method used to clarify how many plug-ins are loaded.
	 *
	 * @return Amount of plug-ins loaded.
	 */
	TInt PluginCount();

public:   // from base class CActive

	/**
	 * From CActive.
	 * Every call takes a context snapshot from one plug-in.
	 */
	void RunL();

	/**
	 * From CActive.
	 * Handles a leave occurring in the request completion event handler RunL().
	 *
	 * @param aError  An error code.
	 * @return An error code.
	 */
	TInt RunError( TInt aError );

	/**
	 * From CActive.
	 * Cancels ????.
	 */
	void DoCancel();

public:   // from MContextPluginObserver

	/**
	 * From MContextPluginObserver.
	 * This is derived from MContextPluginObserver and it is called
	 * by each plug-in when they have initialized.
	 * Error code for the first encountered error is returned as a parameter.
	 *
	 * @param aErrorCode  Error code for the first error occurred during initialization.
	 */
	void PluginInitializationStatus( TInt aErrorCode );
	
	/**
	 * From MContextPluginObserver.
	 * This is derived from MContextPluginObserver and it is called
	 * by each plug-in when they have taken the context snapshot.
	 * Error code for the first encountered error is returned as a parameter.
	 *
	 * @param aErrorCode  Error code for the first error occurred while taking the snapshot.
	 */
	void PluginSnapshotStatus( CHarvesterData* aHD );
        
public: // From MKeyObserver.   	

	/**
	 * Method NotifyKeyL from MKeyObserver get notifications when
	 * property key has changed.
	 */
	void NotifyKeyL( 
			const TInt aKeyValue,
			const TUid aPropertyCategory,
			const TUint aKey );     

private:

	/**
	 * Default constructor, implicitly called by NewL().
	 */
	CContextEngineAO( MContextInitializationObserver* aInitObserver,
		MContextSnapshotObserver* aSnapshotObserver );

	/**
	 * 2nd phase construction, called by NewL().
	 */
	void ConstructL();

	/**
	 * Load context plugins.
	 */
	void LoadPluginsL();

	/**
	 * Initialize this object. Thus load and initialize context plugins.
	 */
	void InitializeL();

	/**
	  * Called when
	  * Backup&Restore is backing up or restoring.
	  */
	 void BackupRestoreStart();

	 /**
	  * Called when
	  * Backup&Restore has finished backup or restore.
	  */
	 void BackupRestoreReady();

	/**
	 * Method HandleBackupRestoreKeyActionL.
	 * If user make full restore sets restore flag on.
	 */
	void HandleBackupRestoreKeyAction( const TUint aKeyValue );
    
private: // data

	/**
	 * This observer is used to inform the client application about
	 * finishing plug-in initializations.
	 */
	MContextInitializationObserver* iInitializationObserver;
	
	/**
	 * This observer is used to inform Context Engine about
	 * finishing a context snapshot.
	 */
	MContextSnapshotObserver* iContextSnapshotObserver;

	/**
	 * Plug-in array contains the plug-ins.
	 */
	RPointerArray<CContextPlugin> iPlugins;

	/**
	 * Number of plug-ins currently not being initialized yet.
	 */
	TInt iUninitializedPluginsCount;
	
	/**
	 * Number of HDs currently not completed the snapshot yet.
	 */
	TInt iPluginSnapshotsLacking;
	
	/**
	 * Item to which we are currently doing a snapshot for.
	 */
	CContextSnapshotItem* iItem;
	
	/**
	 * State of this active object.
	 */
	TContextEngineState iState;
	
	/** First error code encountered in initialization is cached here and returned later. */
	TInt iInitializationError;
	
	/** First error code encountered in snapshot is cached here and returned later. */
	TInt iSnapshotError;
	
	/**
	  * iMdESession - MDE session.
	  */
	CMdESession* iMdESession;

	/**
	 * iPropertyWatcher - constains observed key's, owned by this class.
	 */
	CPropertyWatcher* iPropertyWatcher;
	
    /**
     * iPropertyWatcher - used to check if only one plugin has not finished work.
     */
    TBool iLastPlugin;
    };

#endif // CONTEXTENGINEAO_H 
