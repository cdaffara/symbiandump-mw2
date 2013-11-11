/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of T_CSPSettings class.
*
*/

#ifndef __T_CSPSETTINGS_H__
#define __T_CSPSETTINGS_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

#include "spsettings.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( T_CSPSettings )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static T_CSPSettings* NewL();
        static T_CSPSettings* NewLC();
        ~T_CSPSettings();

    private: // Constructors

        T_CSPSettings();
        void ConstructL();

    private: // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
        
         void T_CSPSettings_AddEntryLL();
        
         void T_CSPSettings_FindEntryLL();
        
         void T_CSPSettings_UpdateEntryLL();
        
         void T_CSPSettings_DeleteEntryLL();
        
         void T_CSPSettings_FindPropertyLL();
        
         void T_CSPSettings_AddOrUpdatePropertiesLL();
        
         void T_CSPSettings_AddOrUpdatePropertyLL();
        
         void T_CSPSettings_SettingsCountLL();
        
         void T_CSPSettings_FindServiceIdsLL();
        
         void T_CSPSettings_FindServiceNamesLL();
        
         void T_CSPSettings_FindSubServicePropertiesLL();
        
         void T_CSPSettings_DeleteServicePropertiesLL();
        
         void T_CSPSettings_FindServiceIdsFromPropertiesLL();
        
         void T_CSPSettings_GetSIPVoIPSWVersionL();
        
         void T_CSPSettings_IsFeatureSupportedL();
         
         void T_CSPSettings_CheckSupportedLsL();
        

    private: // Data

        CSPSettings* iCSPSettings;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __T_CSPSETTINGS_H__
