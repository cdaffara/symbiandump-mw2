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


#ifndef CPOSTp29_H
#define CPOSTp29_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 29
*/
class CPosTp29 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp29() : CLandmarkTestProcedureBase(29) {iGlobalErr = KErrGeneral;};
        
        /**
        * Destructor.
        */
        ~CPosTp29() {};

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
        * From CUtfwTestBase
        */
        void CloseTest();

        /**
        * From CUtfwTestBase
        */
        void InitTestL();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:    // New functions

        void ReInitTestL();

        void TestCriticalLevelL();

        void ConsumeDiscSpace1L(TInt aSizeLimit);

        void ConsumeDiscSpace2L(TInt aSizeLimit);

        void DeleteFile2L();

        void RemoveAllLandmarksL();
        
        void DoOutOfDiskTestL(TBool aSync);

        void InitiateCriticalLevelL();

        void CopyResourceFileL(const TDesC& aResourceFile = KNullDesC);

        void TryRestoreResourceFile();

    private:    // Data

        RFs     iFileServer;
        RFile   iFile1;
        RFile   iFile2;
        TInt    iGlobalErr;
        TInt    iCriticalLevel;
        TInt    iCriticalThreshold;

        CPosLandmarkDatabase*   iLandmarksDb;
        CPosLmCategoryManager*  iCategoryManager;

    };

#endif      // CPOSTp29_H
            
// End of File
