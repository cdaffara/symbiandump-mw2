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


#ifndef CPOSTP43_H
#define CPOSTP43_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkEncoder;
class CPosLandmarkParser;

/**
*  Test procedure 43
*/
class CPosTp43 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp43(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp43() {};

    public: // Functions from base classes

        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        void TestErrorScenariosL();

        /**
        * Test that CPosLandmarkEncoder::SetOutputFileL leaves with
        * KErrAlreadyExists if file already exists
        */
        void ErrorScenario1L();

        /**
        * Test that CPosLandmarkEncoder::SetOutputFileL is opened in exclusive mode
        */
        void ErrorScenario2L();

        /**
        * Test that CPosLandmarkEncoder::AddCollectionDataL leaves with KErrAlreadyExists if
        * collection ID is used twice (or more)
        */
        void ErrorScenario3L();

        /**
        * Test that it is not possible to set collection data "out-of-range" from TPosLmCollectionDataId
        * (will be silently ignored)
        * Test that it is possible to set collection meta data,
        * the data will however not be encoded to the XML file (silently ignored)
        */
        void ErrorScenario4L();

        /**
        * Test that it is possible to use XML tags as name/description/ etc 
        * verify that they are correct enocded
        */
        void ErrorScenario5L();
        
        /**
        * Create all landmark items that should be encoded with the encoder
        */
        void CreateEncoderPartsL();


        /**
        * Check the data (XML) created by the eoncoder
        */
        void ParseAndCheckDataFromEncoderL(const TDesC& aFile, const TDesC& aCollectionName, 
            const TDesC& aCollectiondDescription ,const TDesC8&  aBuffer);

        /**
        * Used for debugging
        */
        void PrintParsedDataFromEncoderL(const TDesC& aFile, /*const TDesC& aCollectionName, 
            const TDesC& aCollectiondDescription, */const TDesC8&  aBuffer);

        void PrintLandmarkIdFieldsL(const CPosLandmark& aLandmark);

        /**
        * Test with long collection data field
        */ 
        void TestLongCollectionFieldsL(TBool aUseFile);

        /**
        * Compare two files
        */
        void CompareTwoFilesL(const TDesC16&  aCorrectFile, const TDesC16&  aFile);

        /**
        * Test with long filename as file output
        */ 
        void TestWithLongFileNameL();
        
        
        void SetupGlobalCategoriesL();


    private:
        CPosLandmarkEncoder* iLandmarkEncoder;
        CPosLandmarkParser* iLandmarkParser;
        RPointerArray<CPosLandmark> iLandmarks;
        TInt iErrorsFound;
        RArray<TUint>    iGlobalCategoryIds;
        RArray<TPtrC>    iGlobalCategoryNames;
        RArray<TPtrC>    iGlobalCategoryTestNames;
        

    };

#endif      // CPOSTP43_H
            
// End of File
