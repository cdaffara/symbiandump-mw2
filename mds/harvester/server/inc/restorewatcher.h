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
* Description:  Definition of CRestoreWatcher class. Observers restore changes.
*
*/


#ifndef RESTOREWATCHER_H
#define RESTOREWATCHER_H

// SYSTEM INCLUDE
#include <e32base.h>
#include <e32cmn.h>
#include <e32property.h>

// USER INCLUDE
#include "mpropertyobserver.h"

// FORWARD DECLERAATIONS
class CPropertyWatcher;

// CLASS DECLARATION
/**
 *  An instance of the CRestoreWatcher object.
 *  Controls harvesterAO and message monitor behaviour when
 *  user restoring data.
 *
 *  @code
 *   
 * 
 * @endcode
 *
 *  @lib -
 *  @since -
 */
NONSHARABLE_CLASS( CRestoreWatcher ) : public CBase, public MKeyObserver
	{	

public: 
    
	/**
     * Factory method NewL
     */
	static CRestoreWatcher* NewL();

	/**
	 * Destructor
	 */
	virtual ~CRestoreWatcher();

public:	
	/**
     * Method Register.
     */
	TBool Register();

	/**
     * Method UnregisterL.
     */
	void UnregisterL();
	
public:	 // From MKeyObserver.

	/**
	 * Method NotifyKeyL from MKeyObserver get notifications when
	 * property key has changed.
	 */
	void NotifyKeyL( 
			const TInt aKeyValue,
			const TUid aPropertyCategory,
			const TUint aKey ); 

private: // Construction 

	/**
	 * Constructor
	 */
	CRestoreWatcher();

	/**
	 * Symbian 2nd phase constructor.
	 */
	void ConstructL();
		
private:

	/**
	 * Method HandleBackupRestoreKeyActionL.
	 * If user make full restore sets restore flag on.
	 */
	void HandleBackupRestoreKeyActionL( const TUint aKeyValue );
	
	/**
	 * Method HandleClientsKeyActionL.
	 * Unregister and register message clients. 	
	 */
	void HandleClientsKeyActionL( const TUint aKeyValue );
	
	/**
	 * Method SetRestoreFlagL.
	 */
	void SetRestoreFlagL( TBool aRestoreDone );
	
	/**
	 * Method CheckRestoreL.
	 */
	void CheckRestoreL();
	
	/**
	 * Method CreateFileNameL.
	 * Creates file name in variable iFileName.
	 */
	void CreateFileNameL(); 

private:
	
	/*iRegisteredClients - registered clients for this observer.*/
	static TInt iRegisteredClients;
	
	/*iPartialRestoreDone - indicates if restore is executed.*/
	static TBool iRestoreDone;
	
	/*iPropertyWatcher - constains observed key's, owned by this class*/
	CPropertyWatcher* iPropertyWatcher;
    
	/*iClients - messageclients count.*/
	TInt iClients;
	
	/*iFileName - File used for restore flag.*/
	TFileName iFileName;
	
	/*iFs - File server session, own.*/
	RFs iFs;
	};

#endif  // RESTOREWATCHER_H
