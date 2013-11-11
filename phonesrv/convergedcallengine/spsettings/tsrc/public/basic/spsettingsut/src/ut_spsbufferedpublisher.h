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
* Description: 
*
*/

#ifndef __UT_SPSBUFFEREDPUBLISHER_H__
#define __UT_SPSBUFFEREDPUBLISHER_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

// FORWARD DECLARATIONS
class CSpsBufferedPublisher;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( Ut_CSpsBufferedPublisher )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static Ut_CSpsBufferedPublisher* NewL();

        ~Ut_CSpsBufferedPublisher();

    private: // Constructors

        Ut_CSpsBufferedPublisher();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();
         
         void Ut_ConstructL();
         
         void Ut_CSpsBufferedPublisher_DataSizeL();
         
         void Ut_CSpsBufferedPublisher_BufferSizeL();
         
         void Ut_CSpsBufferedPublisher_DataValidyL();
        
    private: // Data

        CSpsBufferedPublisher* iPublisher;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_SPSBUFFEREDPUBLISHER_H__
