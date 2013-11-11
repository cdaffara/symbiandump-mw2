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


#ifndef CPOSTp47_H
#define CPOSTp47_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkParser;
class CPosLandmarkEncoder;
/**
*  Test procedure 47
*/
class CPosTp47 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp47(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp47() {};

    public: // Functions from base classes

        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        /**
        * Create XML data files used in this test
        * The files then need to be manually changed
        * aUseFile if data should be written to XML file
        * if EFalse data is written to buffer (and then to a buffer file)
        */
        void CreateXMLData(TBool aUseFile);

        /**
        * Check XML data created in CreateXMLData
        */
        void CheckXMLData(TBool aUseFile);

        /**
        * 
        * @aFile the file name
        */
        void PrintParsedDataFromEncoderL(
            const TDesC& aFile, 
            const TDesC8&  aBuffer, 
            TInt aExpectedStatusCode,
            const TDesC8& aMimeType);

        void PrintLandmarkFieldsL(const CPosLandmark& aLandmark, TBool aTraceFlag=EFalse);
        void PrintLandmarkFieldsWithDescriptionL(const CPosLandmark& aLandmark, TBool aTraceFlag=EFalse);

        /**
        * Check that the parser does not support 
        * building index for parsed landmarks
        */
        void CheckIndexNotSupportedL();

        /**
        * Check that cancel of parse is correct
        */
        void DoCancelTestL();


        /**
        * Trace to trace file
        */
        void TraceL(const TDesC& msg);
        
         // Added for BC testing 
        void MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                   CPosLmCategoryManager& aCatMgr ); 

    private:
        CPosLandmarkParser* iLandmarkParser;

        CPosLandmarkEncoder* iLandmarkEncoder;

        RPointerArray<CPosLandmark> iLandmarks;

        TInt iErrorsFound;

        CBufBase* iEncoderBuffer;
        
        CPosLmCategoryManager*                 iCategoryManager;

    };

#endif      // CPOSTp47_H
            
// End of File
