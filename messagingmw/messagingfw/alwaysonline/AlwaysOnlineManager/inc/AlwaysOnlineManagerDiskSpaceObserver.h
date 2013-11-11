/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
*     CAlwaysOnlineDiskSpaceObserver declaration file
*
*
*/


#ifndef ALWAYSONLINEDISKSPACEOBSERVER_H
#define ALWAYSONLINEDISKSPACEOBSERVER_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>

// FORWARD DECLARATIONS
class CAlwaysOnlineManager;
class CMsvSession;
// CLASS DECLARATION

/**
* CAlwaysOnlineDiskSpaceObserver
*/
class CAlwaysOnlineDiskSpaceObserver : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        *
        * @param aManager A pointer to CAlwaysOnlineManager
        * @param aFs A reference to file session
        */
        static CAlwaysOnlineDiskSpaceObserver* NewL( CAlwaysOnlineManager* aManager, 
        											CMsvSession& aSession );
        
        /**
        * Destructor.
        */
        virtual ~CAlwaysOnlineDiskSpaceObserver();

    public: // New functions

        /**
        *
        * @param aLimit The trigger limit in bytes
        */
        void SetLimitAndActivateL();

    private:

        /**
        * Private C++ constructor.
        *
        * @param aManager A pointer to CAlwaysOnlineManager
        * @param aFs A reference to file session
        */
        CAlwaysOnlineDiskSpaceObserver( CAlwaysOnlineManager* aManager, CMsvSession& aSession );

    private:

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();
        
        /**
		* Function to handle error during running the operation
		* @since Series60 2.6
		* @return KErrNone, when error has been handled
		*/
		virtual TInt RunError( TInt aError );
		
    private: // New Functions
    
        /**
        * Updates current disk limit
        * @since Series60 3.0
        */
        void HandleLimitChange();

    private: // Data

        CAlwaysOnlineManager*   iWatcher;
        CAlwaysOnlineManager&   iManager;
        CMsvSession&            iSession;
        TUint                   iNewLimit;
        TUint                   iCurrentLimit;
        
    };

#endif      // 
            
// End of File
