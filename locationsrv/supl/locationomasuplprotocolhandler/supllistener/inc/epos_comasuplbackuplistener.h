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
* Description:   Class to handle backup and restore events and resore previous settings manually
*
*/



#ifndef _EPOS_COMASUPLBACKUPLISTENER_H
#define _EPOS_COMASUPLBACKUPLISTENER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <sbdefs.h>
#include <epos_msuplsettingsobserver.h>

// CLASS DECLARATION
class CSuplSettings;

using namespace conn;

/**
 *  Class to handle backup and restore events
 */
class COMASuplBackupListener : public CActive,public MSuplSettingsObserver
    {
    
    enum TOMASuplBackupStarterState
    	{
    		EBootCompletion,
    		ERestore,
    		ESettingInitilization
    	};
    	
    private:    // Constructor
        /**
        * C++ Constructor
        */
        COMASuplBackupListener();

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~COMASuplBackupListener();

        /**
        * Two-phase constructor
        */
        static COMASuplBackupListener* NewL();

    private:     // New functions


		/**
        * Reads current value of Backup & Restore P&S property
        * @param aState Backup state as defined in <sbdefs.h>
        * @param aBackupType Backup type as defined in <sbdefs.h>
        */
        TInt GetCurrentValue();
		/**
		* Writes log into log file...
		*/
		void Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line );
		
		/**
		* Reads the current setting using settings API
		*/
		void ReadCurrentSettingsL(TBool aUsage);
		
		/**
		* Check if boot is Completed
		*/
		void CheckForBootCompletionL();

		/**
		* Initilize the settings API
		*/
		void InitializeSettings();
		
		/**
		* Listens for restore event
		*/
		void ListenRestoreEventAgainL();

	protected:
		/**
		* Callback for setting changes...
		*/
		void HandleSuplSettingsChangeL (TSuplSettingsEventType /*aEvent*/);

    private:
        /**
        * EPOC default constructor
        */
        void ConstructL();

        // By default, prohibit copy constructor
        COMASuplBackupListener( const COMASuplBackupListener& );
        // Prohibit assigment operator
        COMASuplBackupListener& operator= ( const COMASuplBackupListener& );

    protected:  // From CActive

        void RunL();
        TInt RunError(TInt aError);
        void DoCancel();

    private:    // Data
    
        RProperty                   	iBackupProperty;
        
		/**
		* Handle to CSuplSettings class used to update SUPL settings
		*
		*/	
	    CSuplSettings*               iSettings;

		TBool iRestoreInProgrees;
		
		TOMASuplBackupStarterState 	 iState;
		
		/**
		* Handle to the startup property object
		*
		*/	
		RProperty                    iStartupObserver;
		
		TBURPartType iTBURState;
		
		TBool iIsLastChangedByConfigurator;
		
		RBuf iPreviousHSLPAddress;
		
		RBuf iIAPName;
		
		
    };

#endif // _EPOS_COMASUPLBACKUPLISTENER_H

// End of File