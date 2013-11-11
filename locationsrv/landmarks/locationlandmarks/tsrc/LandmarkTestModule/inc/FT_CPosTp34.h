/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CPOSTP34_H
#define CPOSTP34_H

//  INCLUDES
#include <EPos_CPosLmTextCriteria.h>

#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 34
*/
class CPosTp34 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp34() : CLandmarkTestProcedureBase(34) {};
        
        /**
        * Destructor.
        */
        ~CPosTp34() {};

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

        void CloseTest();

    private:

        void CheckSearchResultL(CSearchResult& aSearchResult,
                                CPosLandmarkSearch& aLandmarkSearch,
                                CPosLmTextCriteria& aTextCriteria);      
    
    private: // Data
   

      

    };

#endif      // CPOSTP34_H
            
// End of File
