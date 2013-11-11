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


#ifndef CPOSTP81_H
#define CPOSTP81_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 81
*/
class CPosTp81 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp81() : CLandmarkTestProcedureBase(81) {};
        
        /**
        * Destructor.
        */
        ~CPosTp81() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

    void TestDifferentNamesL();
    
    };

#endif      // CPOSTp81_H
            
// End of File
