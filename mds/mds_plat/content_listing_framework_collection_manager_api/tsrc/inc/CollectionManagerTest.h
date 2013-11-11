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
* Description: 
*
*/


#ifndef CONTEXTENGINEPLUGINTEST_H
#define CONTEXTENGINEPLUGINTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <mediacollectionmanager.h>

// Logging path
_LIT( KCollectionManagerTestLogPath, "\\logs\\testframework\\CollectionManagerTest\\" ); 
// Log file
_LIT( KCollectionManagerTestLogFile, "CollectionManagerTest.txt" ); 
_LIT( KCollectionManagerTestLogFileWithTitle, "CollectionManagerTest_[%S].txt" );

// FORWARD DECLARATIONS
class CCollectionManagerTest;

// CLASS DECLARATION
NONSHARABLE_CLASS(CCollectionManagerTest) : public CScriptBase,
                                                                            public MMediaCollectionObserver
				                              
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCollectionManagerTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCollectionManagerTest();

    public: // Functions from base classes

        virtual TInt RunMethodL( CStifItemParser& aItem );
        
        void CollectionChangedL( const TArray<TInt>& aCollectionIdArray );
        void CollectionManagerErrorL( TInt aError );

    private:

        /**
        * C++ default constructor.
        */
        CCollectionManagerTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();
        
        /**
        * Test methods are listed below. 
        */

        // engine
        virtual TInt CreateCollectionManagerL( CStifItemParser& aItem );
        virtual TInt GetCollectionInfosL( CStifItemParser& aItem );
        virtual TInt CollectionInfoByIdL( CStifItemParser& aItem );
        virtual TInt CreateCollectionL( CStifItemParser& aItem );
        virtual TInt DeleteCollectionL( CStifItemParser& aItem );
        virtual TInt RenameCollectionL( CStifItemParser& aItem );
        virtual TInt AddToCollectionL( CStifItemParser& aItem );
        virtual TInt RemoveFromCollectionL( CStifItemParser& aItem );
        virtual TInt RemoveFromAllCollectionsL( CStifItemParser& aItem );
        virtual TInt GetCollectionItemArrayL( CStifItemParser& aItem );
        virtual TInt GetCollectionIdByItemL( CStifItemParser& aItem );
        virtual TInt AddCollectionObserverL( CStifItemParser& aItem );
        virtual TInt RemoveCollectionObserverL( CStifItemParser& aItem );
        virtual TInt GetDriveByIdL( CStifItemParser& aItem );
        virtual TInt EndTestSessionL( CStifItemParser& aItem );

    private:    // Data

        MMediaCollectionManager* iCollectionManager;
        
    };

#endif      // CONTEXTENGINEPLUGINTEST_H

// End of File
