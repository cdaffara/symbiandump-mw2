/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of CPropertyWatcher class.*
*/


#ifndef PROPERTYWATCHER_H
#define PROPERTYWATCHER_H

// SYSTEM INCLUDE
#include <e32base.h>

// FORWARD DECLARATIONS
class CListener;
class MKeyObserver;
class MKeyAndStatusObserver;

// CLASS DECLARATION
/**
 *  An instance of the CPropertyWatcher object - singleton.
 *  Create listeners for RProperty key changes. This class
 *  uses TLS implementation. 
 *
 *  @code
 *	
 * // Get instance
 * iPropertyWatcher = CPropertyWatcher::GetInstanceL();
 *	
 * // Set wanted key, give gategory's uid, observed key and this pointer.
 * iPropertyWatcher->ListenKeyChangesL( KUidSystemCategory,
 *  	conn::KUidBackupRestoreKey, this );
 *	iPropertyWatcher->ListenKeyChangesL( KPSUidScreenSaver,
 *  	KScreenSaverOn, this );
 *
 * ...
 *  // Implement NotifyKeyL for key canges or NotifyKeyAndStatusL for
 *  // key and status changes. 
 *  void CRestoreWatcher::NotifyKeyL( 
 *		const TInt aKeyValue,
 *		const TUid aPropertyCategory,
 *		const TUint aKey)
 *	{
 *	
 *	// In interface check key's category. 
 *	if( aPropertyCategory == KUidSystemCategory &&
 *		aKey == conn::KUidBackupRestoreKey )
 *		{
 *		TInt backupStateValue = aKeyValue;
 *		backupStateValue &= conn::KBURPartTypeMask;
 *		
 *		if ( backupStateValue >= conn::EBURRestoreFull )
 *			{
 *			// Handle backup here
 *			}
 *		}
 *	
 *	if( aPropertyCategory == KPSUidScreenSaver && aKey == KScreenSaverOn )
 *		{
 *		//Handle screensaver activity here 
 *		}	
 *	}
 * 
 *  // Destruction
 * 
 * 	if( iPropertyWatcher )
 *		{
 *		iPropertyWatcher->StopListeningKeyChanges( 
			KUidSystemCategory,
			conn::KUidBackupRestoreKey, this );
 *		
 *		iPropertyWatcher->Delete(); // Release connection to TLS object.
 *		} 
 * 
 * @endcode
 */
NONSHARABLE_CLASS( CPropertyWatcher ) : public CBase
    {
 public: // Exported.
		
	/*!
	@function GetInstanceL
	@Create a CPropertyWatcher object, 
	@result a pointer to the created instance of CPropertyWatcher.
	*/
	IMPORT_C static CPropertyWatcher* GetInstanceL();
	
	/*!
	@function Delete
	@Release an instance of this singleton.
	*/
	IMPORT_C void Delete();
		
	/*!
	@function ListenKeyChanges
	@Registers to watcher and get events from watcher via MKeyObserver interface.
	@param aPropertyCategory observed key's category.       
	@param aKey observed key.
	@aValueObserver client's pointer. Notive that this is not taking ownership.
	*/
	IMPORT_C void ListenKeyChangesL( const TUid aPropertyCategory,
			 const TUint aKey,
			 const MKeyObserver* aValueObserver
			);
   /*
	@function ListenKeyAndStatusChangesL
	@Registers to watcher and get events from watcher via MKeyAndStatusObserver interface.
	@param aPropertyCategory observed key's category.       
	@param aKey observed key.
	@aStatusObserver client's pointer. Notive that this is not taking ownership.
	*/
   IMPORT_C void ListenKeyAndStatusChangesL( const TUid aPropertyCategory,
			const TUint aKey,
			const MKeyAndStatusObserver* aStatusObserver );

   /*
	@function StopListeningKeyChanges
	@Stops listening key changes. 
	@param aPropertyCategory observed key's category.       
	@param aKey observed key.
	@aValueObserver client's pointer.
	*/
   IMPORT_C void StopListeningKeyChanges( 
			const TUid aPropertyCategory,
			const TUint aKey,
			const MKeyObserver* aKeyObserver );
   
   /*
	@function StopListeningKeyAndStatusChanges
	@Stops listening key and status changes. MDS need this to stop Active sheduler during restore.
	@param aPropertyCategory observed key's category.       
	@param aKey observed key.
	@aValueObserver client's pointer.
	*/
    IMPORT_C void StopListeningKeyAndStatusChanges( 
			const TUid aPropertyCategory,
			const TUint aKey,
			MKeyAndStatusObserver* aStatusObserver );
 
private:  
   /*!
	@function ~CPropertyWatcher
	@Destroy the object and release all memory objects.
	*/
	~CPropertyWatcher();  

private:
	/*!
	@function FindListener
	@Find if there is allredy listener which matches
	@aPropertyCategory and aKey.
	@return listener index, KErrNotFound if not exists.
	*/    	
	TInt FindListener( const TUid aPropertyCategory, const TUint aKey ) const;
  
private:
  
	/*!@iListenersArray - all listeners for different key values.
	 * Owned by this class.
	 */
	RPointerArray<CListener> iListenersArray;    
    };

#endif // PROPERTYWATCHER

