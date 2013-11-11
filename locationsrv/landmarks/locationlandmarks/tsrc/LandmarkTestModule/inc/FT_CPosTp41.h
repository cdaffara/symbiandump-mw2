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


#ifndef CPOSTp41_H
#define CPOSTp41_H

//  INCLUDES
#include <EPos_CPosLandmark.h> 
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION


/**
*  Test procedure 41
*/
class CPosTp41 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp41() : CLandmarkTestProcedureBase(41) {};
        
        /**
        * Destructor.
        */
        ~CPosTp41() {};

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

        /**
        * From CLandmarkTestProcedureBase
        * Cleans up after a test.
        * Always called, even if the test leaves. 
        */
        void CloseTest();

    private:
        
        /**
        * This part verifies error report "ESLI-5ZDH4N Landmarks - Empty descriptors"
        */
        void VerifyEmptyLandmarksL();

        /**
        * Check that landmark read from db has correct name and desc
        */
        void CheckLandmarkL(TPosLmItemId aId, TDesC& aName, TDesC& aDesc);

    private: // data
        
    };

#endif      // CPOSTp41_H
            
// End of File
