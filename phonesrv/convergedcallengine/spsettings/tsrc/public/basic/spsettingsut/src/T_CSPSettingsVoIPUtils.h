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
* Description: Declaration of T_CSPSettingsVoIPUtils class.
*
*/

#ifndef __T_CSPSETTINGSVOIPUTILS_H__
#define __T_CSPSETTINGSVOIPUTILS_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

#include "spsettingsvoiputils.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( T_CSPSettingsVoIPUtils )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static T_CSPSettingsVoIPUtils* NewL();
        static T_CSPSettingsVoIPUtils* NewLC();
        ~T_CSPSettingsVoIPUtils();

    private: // Constructors

        T_CSPSettingsVoIPUtils();
        void ConstructL();

    private: // New methods

         void SetupL();
         
         void Setup2L();
         
         void Setup3L();
        
         void Teardown();
        
         void T_CSPSettingsVoIPUtils_VoIPProfilesExistLL();
        
         void T_CSPSettingsVoIPUtils_IsVoIPSupportedL();
        
         void T_CSPSettingsVoIPUtils_IsPreferredTelephonyVoIPL();
        
         void T_CSPSettingsVoIPUtils_GetPreferredServiceL();
         
         void T_CSPSettingsVoIPUtils_DoGetPreferredServiceLL();
        

    private: // Data

        CSPSettingsVoIPUtils* iCSPSettingsVoIPUtils;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __T_CSPSETTINGSVOIPUTILS_H__
