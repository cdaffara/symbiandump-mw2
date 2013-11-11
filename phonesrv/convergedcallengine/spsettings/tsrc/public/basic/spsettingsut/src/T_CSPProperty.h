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
* Description: Declaration of T_CSPProperty class.
*
*/

#ifndef __T_CSPPROPERTY_H__
#define __T_CSPPROPERTY_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

#include "spproperty.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( T_CSPProperty )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static T_CSPProperty* NewL();
        static T_CSPProperty* NewLC();
        ~T_CSPProperty();

    private: // Constructors

        T_CSPProperty();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();
         
         void PropertyDatatypeTIntSetGetTestL();
         
         void PropertyDatatypeOnOffSetGetTestL();
         
         void PropertyDatatypeTDesCSetGetTestL();
         
         void CopyLL();
         
         void SetValueNoDataTypeCheckL();
         
         void PropertyTypeL();
         
         void EqualToOperatorOverloadL();
        

    private: // Data

        CSPProperty* iCSPProperty;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __T_CSPPROPERTY_H__
