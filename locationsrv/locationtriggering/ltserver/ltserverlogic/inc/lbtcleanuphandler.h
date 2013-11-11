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
* Description:  This header file describes the class reposnible cleanuping
*				 of triggers based on some filter after a particular duration
*
*/


#ifndef C_LBTCLEANUPHANDLER_H
#define C_LBTCLEANUPHANDLER_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CLbtContainer;
class CLbtCleanupItem;
class TTime;

class CLbtCleanupHandler : public CTimer
	{
	public:
		/**
	     * Constructs a new instance of cleanup handler. As soon as this object is 
	     * created the a low priority timer is started and the cleanup is done after
	     * the specified time. 
	     *
	     *
	     * @return The new instance of cleanup handler object.
	     */
		static CLbtCleanupHandler* NewL( CLbtContainer &aContainer );
				
		/**
		 * Adds triggers into the cleanup mechanism. The specified triggers will be
		 * cleaned up after the specified cleanup time retreived from central repository.
		 * The triggers specified will be saved onto a file for retreiving the trigger ids
		 * in case of system reboot.
		 *
		 * @param aTriggers the trigger ids that need to be cleanedup
		 */
		void AddTriggersForCleanupL( const RArray<TLbtTriggerId>& aTriggers );
		
		/**
		 * Removes the specified triggers from cleanup.
		 *
		 * @param aTriggers the trigger ids that need to be removed from clean up
		 */
		void RemoveTriggersFromCleanupL(const RArray<TLbtTriggerId>& aTriggers );
		
		/**
		 * Cancels the ongoing operation. 
		 */
		void Cancel();
		
		/**
	     * Destructor.
	     */
		~CLbtCleanupHandler();
	
	private: // new functions
	
		// Default constructor
		CLbtCleanupHandler( CLbtContainer &aContainer );
		
		/**
	     * Symbian 2nd phase of construction
	     */
		void ConstructL( );
		
		/**
		 * Write the cleanup triggers into a file. This is needed in cases
		 * when the system is rebooted.
		 */
		void WriteCleanupDataToFileL();
		
		/**
		 * Reads the cleanup triggers from the saved file. This is done during
		 * system startup
		 */
		void ReadCleanupDataFromFileL();
		
		/**
		 * Cleans up the next item on the cleanup list. The cleanup list is
		 * maintained and saved in a chronological order.
		 *
		 */
		void CleanupNextItemL();
			
	private: // From CActive		
		void RunL();		
		TInt RunError(TInt aError);
		
	private: // data members

		// Pointer to instance of Lbt Container
		CLbtContainer &iContainer;
	
		// Array of pointers to hold the clean items
		RPointerArray<CLbtCleanupItem> iCleanupItems;
		
		// Days for cleanup read up from repository
		TInt iDaysForCleanup;
		
		// Local integer to ascertain the asynch operation
		TInt iOperation;
		
		// operation code for container requests
		TInt iContainerOp;
	};

/**
 * 
 */
class CLbtCleanupItem : public CBase
	{
	public:
	    /**
	     *
	     */
		void AddCleanupItem( const RArray<TLbtTriggerId>& aTriggers, TTime& aUTCTime );
		
		/**
	     *
	     */
		const TTime& GetTime() const;
		
		/**
	     *
	     */
		RArray<TLbtTriggerId>& GetTriggers();

		/**
		 * Destructor
		 */
		 ~CLbtCleanupItem();

	private:
		// Array of trigger ids that need to be cleaned up
		RArray<TLbtTriggerId> iTriggers;
		
		// The time at which the items have to be cleanedup
		TTime iUTCTime;
	};


#endif //C_LBTCLEANUPHANDLER_H

// end of file

