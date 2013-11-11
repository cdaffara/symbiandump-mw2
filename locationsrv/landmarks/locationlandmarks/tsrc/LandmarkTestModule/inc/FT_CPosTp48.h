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


#ifndef CPOSTP48_H
#define CPOSTP48_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkParser;

/**
*  Test procedure 48
*/
class CPosTp48 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp48(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp48() {};

    public: // Functions from base classes

        
        
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        /**
        * Do the import of landmarks
        */
        void ImportLandmarksL(TBool aSync, TBool aParseDone, TUint aTransferOption, const RArray<TUint>&  aLandmarkSubSet, TBool aDoParse=EFalse, TBool aCancel=EFalse);

        /**
        * Prepare landmarkdb for test
        */
        void ResetLandmarksDbL();

        /**
        * Create data used for this test
        */
        void CreateXMLDataL();


        /**
        * List all landmarks in db
        */
        void ListLandmarksDbL();

        /**
        * Check that imported landmarks has been added to database
        */ 
        void CheckLandmarksDatabaseL(TUint aTransferOption);

        void PrintLandmarkIdFieldsL(const CPosLandmark& aLandmark);

        /**
        * Helper method, removing all associated categories from a landmark that should be "saved"
        * in the iLandmarks array
        */
        void RemoveCategoriesFromLandmarkL(CPosLandmark& aLandmark);

        /**
        * Try to set a long file name as input
        */
        void ImportFromFileWithLongFileNameL();

        /**
        * Helper method to check that a global category exists
        and another does not
        */
        void CheckGlobalCategoryExistL(const TDesC& aShouldExistName, const TDesC& aShouldNotExistName);

        /**
        * Does the acutal test
        */
        void DoTestL(TBool aFile, TBool aFullParse=EFalse, TBool aDoParse=EFalse, TBool aCancel=EFalse);

        /**
        * Change input method after parse
        */
        void DoTestChangeInputMethodL();
       
    private:
        CPosLandmarkParser* iLandmarkParser;
        CPosLandmarkEncoder* iLandmarkEncoder;
        RPointerArray<CPosLandmark> iLandmarks;
        TInt iErrorsFound;
        CBufBase* iEncoderBuffer;
        
        

    };

#endif      // CPOSTp48_H
            
// End of File
