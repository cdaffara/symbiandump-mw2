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
* Description:   Class to handle product configuration
*
*/



#ifndef _EPOS_COMASUPLPRODUCTCONFIG_H
#define _EPOS_COMASUPLPRODUCTCONFIG_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <sbdefs.h>

#include <centralrepository.h>
#include "epos_mproductconfigobserver.h"


// CLASS DECLARATION

class CSuplSettingsInternal;

using namespace conn;
/**
 *  Class to handle product configuration
 */
class COMASuplProductConfig : public CActive
    {
    
    enum TOMASuplProductConfigState
    	{
    		EBootCompletion
    	};
    	
    private:    // Constructor
        /**
        * C++ Constructor
        */
        COMASuplProductConfig(MProductConfigObserver& aObserver);

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~COMASuplProductConfig();

        /**
        * Two-phase constructor
        */
        static COMASuplProductConfig* NewL(MProductConfigObserver& aObserver);
        
        /**
                * Check if boot is Completed
                */
         void CheckForBootCompletionL();

    private:     // New functions


		
		/**
		* Writes log into log file...
		*/
		void Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line );
		
		
		
		/**
		* Starts the process of Product configuration
		*/
		void StartProductConfigurationL();
	
    private:
        /**
        * EPOC default constructor
        */
        void ConstructL();

     protected:  // From CActive

        void RunL();
        TInt RunError(TInt aError);
        void DoCancel();

    private:    // Data
        
        CSuplSettingsInternal* iSettings;
        CRepository* iProductRepository;
        
        MProductConfigObserver& iProductObserver;
        
        TOMASuplProductConfigState   iState;
		
        RProperty                    iStartupObserver;	
        
        RBuf iServerAddress;
        RBuf iIapName;
		
    };

#endif // _EPOS_COMASUPLPRODUCTCONFIG_H

// End of File