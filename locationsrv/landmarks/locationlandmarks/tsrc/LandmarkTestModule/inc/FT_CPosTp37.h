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
* Description: 
*   ?description_line
*
*/


#ifndef CPOSTp37_H
#define CPOSTp37_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 1
*/
class CPosTp37 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp37(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound = KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp37() {};

    public: // Functions from base classes

        

        /**
        * From CUtfwTestBase. Always called even if test leaves, 
        * after test completion
        */
        void CloseTest();


        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        /**
        * Test to mix CPosLmCatNameCriteria and CPosLmIdListCriteria
        * in a StartCategorySearchL search, should leave
        * Test to mix CPosLmCatNameCriteria and CPosLmIdListCriteria
        * in a StartLandmarkSearchL search, should leave
        */
        void TestCompositeAndCategoryL(TInt aTestNr);

        /**
        * Test that it is not possible to mix landmark search and a landmark category search
        * in a composite object.
        *
        * Test that it is not possible perform a search if the composite object 
        * does not have any arguments
        */
        void DoErrorTestL(TInt aTestNr);

        /**
        * Test that it is not possible to use only one ID List
        * without any other search arguments
        */
        void DoErrorTest3L(TInt aTestNr);

        /**
        * Test that it is not possible to use several ID Lists
        * in a composite search
        */
        void DoErrorTest4L(TInt aTestNr);

    private:
            TInt iErrorsFound;

    };

#endif      // CPOSTp37_H
            
// End of File
