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


#ifndef CPOSTP42_H
#define CPOSTP42_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <apgcli.h>


//CONSTANTA
const TInt NOLANDMARKNUMBER = 123;
// CLASS DECLARATION

class CRecognizerData;

/**
*  Test procedure 42
*/
class CPosTp42 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp42(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp42() {};

    public: // Functions from base classes

        
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
    
        void TestWithBufferL();
        
        void TestWithFileL();
        
        void TestWithEmptyFileL();
        
        void TestWithLargeFilesL(const TDesC& msg, TInt aExpCode);
    
        void CreateFileL( const TDesC& msg, TFileName& aFileName );
        
        void CheckResultL(TDataRecognitionResult aExpectedResult, TDataRecognitionResult aResult);

    private:
        RPointerArray<CRecognizerData>   iDataToRecognize;
        TInt iErrorsFound;

    };

#endif      // CPOSTP42_H

/**
* Data class only used with TP42 to hold information
* read from the file i,e the data to be recongnized and
* the expected result.
*/
class CRecognizerData: public CBase
    {
    public:  // Constructors and destructor
        
         /**
        * Two-phased constructor.
        */
        static CRecognizerData* NewL(const TDesC& aDes); 

        /**
        * Destructor.
        */
        ~CRecognizerData();      

    public:

         TPtrC RecognizerData() const;
        
         const TDataRecognitionResult& ExpectedResult() const;

    private: 
        
        CRecognizerData();

        void ConstructL(const TDesC& aDes);

    private:
         // By default, prohibit copy constructor
        CRecognizerData(const CRecognizerData&);
        // Prohibit assigment operator
        CRecognizerData& operator=(const CRecognizerData&);

        void ParseResult(const TDesC& aDes);
        void ParseType(const TDesC& aDes);
        
        
    private: // Data
        
        enum TValueIndex
			{
            ETextBuffer          = 0,	
            EResult              = 1,
            EType                = 2
			};
        HBufC*                     iRecognizerData;
        TDataRecognitionResult      iExpectedResult;

    };

            
// End of File
