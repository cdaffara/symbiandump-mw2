/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  header for tests
*
*/


#ifndef __T_XSPSETTINGSAPIMODULETESTS_H__
#define __T_XSPSETTINGSAPIMODULETESTS_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <spdefinitions.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CSPSettings;
class CSPEntry;
class CSPProperty;
class CStubObserver;
class CSPSettingsVoIPUtils;


#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( T_xSPSettingsAPIModuleTests )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_xSPSettingsAPIModuleTests* NewL();
        static T_xSPSettingsAPIModuleTests* NewLC();
        /**
         * Destructor
         */
        ~T_xSPSettingsAPIModuleTests();

    private:    // Constructors and destructors

        T_xSPSettingsAPIModuleTests();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();
        void Setup2L();
        void Setup3L();
        
        void Teardown();
        void Teardown2();
        void Teardown3();

        // Test cases

        // CSPEntry class
        void EntryTestL();
        void EntryMisuseTestL();

        // CSPProperty class
        void PropertyTestL();
        void PropertyMisuseTestL();

        // CSPSettings class
        void NewAPITestL();
        void AddEntryTestL();
        void FindEntryByIdTestL();
        void FindEntryByIdTest2L();
        void UpdateEntryTestL();
        void ResetTestL();
        void DeleteEntryTestL();        
        void FindPropertyByIdTestL();
				void GetAllPropertiesTestL();
        void DeletePropertyByNameTestL();
        void AddOrUpdatePropertiesTestL();
        void SettingsCountTestL();
        void FindServiceIdsTestL();
        void FindServiceNamesTestL();
        void FindSubServicePropertiesTestL();
        void DeleteServicePropertiesTestL();
        void GetSIPVoIPSWVersionTestL();
        void NotifyChangeTestL();
        void FindServiceIdsFromPropertiesTestL();
        void FindPropertyTestL();
        void ConcurrentTestL();
        void IsFeatureSupportedTestL();
        
        void BackupTestL();
        void RestoreTestL();
        
        void VoIPProfilesExistTestL();
        void IsVoIPSupportedTestL();
        void IsPreferredTelephonyVoIPTestL();

        // Helper methods
        CSPEntry* CreateTestEntryLC( const TDesC& aName );

        template <class T>
        CSPProperty* CreateTestPropertyL( 
                TServicePropertyName aProperty, const T& aValue );


        TInt CreateAndAddTestEntryL( const TDesC& aName );
        
        static TInt AddNewEntryCallBack1( TAny* aPointer );

        void AddNewEntry1();
        
        static TInt AddNewEntryCallBack2( TAny* aPointer );

        void AddNewEntry2();
		static void CleanupPointerArray( TAny* aPointer );
		
		void AddDeletePropertyTest1L();
		void AddDeletePropertyTest2L();



    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        // Settings API        
        CSPSettings* iSettings;
        
        CStubObserver* iStubObserver;
        
        CActiveSchedulerWait* iWait;
        
        CPeriodic* iPeriodic1;
        CPeriodic* iPeriodic2;
        
        TInt iTimes1;
        TInt iTimes2;
        
        CSPSettingsVoIPUtils* iVoIPUtils;

    };

#endif      //  __T_XSPSETTINGSAPIMODULETESTS_H__

// End of file
