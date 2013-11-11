/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes Backup & Restore's state key and status -*
*/


#ifndef CTBACKUPSUBSCRIBER_H
#define CTBACKUPSUBSCRIBER_H

// SYSTEM INCLUDE
#include <e32base.h>
#include <e32cmn.h>

// USER INCLUDE
#include "mpropertyobserver.h"

// FORWARD CLASS DECLERATION
class CPropertyWatcher;

/**
 * Observer interface for Backup & Restore changes.
 */
 class MBackupRestoreObserver
     {
     public:
         /**
          * Called by CBlacklistBackupSubscriberAO when
          * Backup&Restore is backing up or restoring.
          */
         virtual void BackupRestoreStart() = 0;

         /**
          * Called by CBlacklistBackupSubscriberAO when
          * Backup&Restore has finished backup or restore.
          */
         virtual void BackupRestoreReady() = 0;
     };

/**
* This class implements object for watching
* Backup & Restore is state. 
*/
 NONSHARABLE_CLASS( CBackupSubscriber ) : public CBase, public MKeyAndStatusObserver
  	{
public:

    /**
     * Creates and constructs a new instance of CBackupSubscriber.
     *
     * @return A pointer to the new instance.
     */
    static CBackupSubscriber* NewL( MBackupRestoreObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CBackupSubscriber();
  
public:
   /*
    @function NotifyKeyAndStatusL - get called when propertys value has changed.
    @param aKeyValue - key's value.
    @param aStatus - active object status, get from CListener object.
    @param aPropertyCategory observed key's category.       
    @param aKey observed key.
    */
	void CBackupSubscriber::NotifyKeyAndStatusL( 
		const TInt aKeyValue,
		TRequestStatus& aStatus,
		const TUid aPropertyCategory,
		const TUint aKey );


private:

    /**
     * Default constructor, implicitly called by NewL().
     */
    CBackupSubscriber( MBackupRestoreObserver& aObserver );

    /**
     * 2nd phase construction, called by NewL().
     */
    void ConstructL();
    
    /**
     * Check Backup & Restore's state key status and act accordingly.
     */
    void CheckBackupState( const TInt aKeyValue, TRequestStatus& aStatus );

private: // data

	/*iPropertyWatcher - constains observed key's, owned by this class*/
	CPropertyWatcher* iPropertyWatcher;
    
	/*iObserver - callback for HarvesterAO to notify when backup/restore starts.*/
	MBackupRestoreObserver& iObserver;
  	};

#endif // CTBACKUPSUBSCRIBER_H
