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
* Description: Declaration of T_CSPSettingsEngine class.
*
*/

#ifndef __T_CSPSETTINGSENGINE_H__
#define __T_CSPSETTINGSENGINE_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

#include "spsettingsengine.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( T_CSPSettingsEngine )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static T_CSPSettingsEngine* NewL();
        static T_CSPSettingsEngine* NewLC();
        ~T_CSPSettingsEngine();

    private: // Constructors

        T_CSPSettingsEngine();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();
        
         void T_CSPSettingsEngine_BeginTransactionLCL();
        
         void T_CSPSettingsEngine_CommitTransactionL();
        
         void T_CSPSettingsEngine_RollbackTransactionL();
        
         void T_CSPSettingsEngine_AddEntryLL();
        
         void T_CSPSettingsEngine_FindEntryLL();
        
         void T_CSPSettingsEngine_UpdateEntryLL();
        
         void T_CSPSettingsEngine_DeleteEntryLL();
        
         void T_CSPSettingsEngine_FindPropertyLL();
        
         void T_CSPSettingsEngine_AddOrUpdatePropertiesLL();
        
         void T_CSPSettingsEngine_SettingsCountLL();
        
         void T_CSPSettingsEngine_FindServiceIdsLL();
        
         void T_CSPSettingsEngine_FindServiceNamesLL();
         
         void T_CSPSettingsEngine_PropertyNameArrayFromItemTypeLL();
        
         void T_CSPSettingsEngine_FindSubServicePropertiesLL();
        
         void T_CSPSettingsEngine_DeleteServicePropertiesLL();
        
         void T_CSPSettingsEngine_FindServiceIdsFromPropertiesLL();
        
         void T_CSPSettingsEngine_IsFeatureSupportedLL();
        
         void T_CSPSettingsEngine_UpdateSupportFeaturePSKeyLL();
        
         void T_CSPSettingsEngine_IsVoIPServiceLL();
        
         void T_CSPSettingsEngine_HasCallPropertyNameL();
        
         void T_CSPSettingsEngine_HasVoIPAndVmbxPropertyNameL();
         
         void T_CSPSettingsEngine_NameArrayFromPropertyArrayLL();
        

    private: // Data

        CSPSettingsEngine* iCSPSettingsEngine;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __T_CSPSETTINGSENGINE_H__
