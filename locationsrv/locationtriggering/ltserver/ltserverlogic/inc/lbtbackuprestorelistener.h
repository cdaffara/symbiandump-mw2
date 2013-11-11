/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of CLbtBackupRestoreListener
*
*/


#ifndef C_LBTSYSTEMSTATELISETER_H
#define C_LBTSYSTEMSTATELISETER_H

//FORWARD DECLARATION
class MLbtBackupRestoreObserver;
class RProperty;

// INCLUDE FILE
#include <e32base.h>


/**
 * Class definition for LBT Backup & Restore Listener
 *
 * @since S60 v4.0
 */
class CLbtBackupRestoreListener : public CActive
	{
	public:	
		/**
	     * Instantiates a new object of 
	     * CLbtBackupRestoreListener
	     *
	     * @param[in] aObserver reference to the observer
	     * which needs to listen to back and restore events
	     */
	    static CLbtBackupRestoreListener* NewL(MLbtBackupRestoreObserver& aObserver);
	    
	    /**
	     * Starts listening to backup and restore events
	     *
	     */
	    void StartToListenL();
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtBackupRestoreListener();
	    
	    /**
	     * Determines if any backup or restore operation
	     * is in progress
	     *
	     * @return ETrue if backup or restore operation is in 
	     * progress, EFalse otherwise
	     */
	    TBool IsBackupRestoreOperationInProgress();
	    
	private:		
	
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
		/**
	     * Default C++ Constructor
	     *
	     * @param[in] aObserver reference to the observer
	     * which needs to listen to back and restore events
	     */
	    CLbtBackupRestoreListener(MLbtBackupRestoreObserver& aObserver);
	    
	private: // From CActive
	
		/**
	     * Request completion event handler
	     *
	     */	
	    void RunL();
	    
	    /**
	     * Cancels any outstanding request
	     *
	     */	
	    void DoCancel();
	    
	    /**
	     * Handles when RunL leaves
	     */
	    TInt RunError( TInt aError ); 
	    
	private: // Data Members
	
		/**
	     * Handle to the startup property object
	     *
	     */	
	    RProperty		iBackupRestoreListener;
	    
	    /**
	     * Reference to the observer
	     *
	     */
	    MLbtBackupRestoreObserver&	iObserver;

	};

#endif // C_LBTSYSTEMSTATELISETER_H

