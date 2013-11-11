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


#ifndef CPOSTp149_H
#define CPOSTp149_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkParser;
class CPosLandmarkEncoder;
/**
*  Test procedure 149
*/
class CPosTp149 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp149(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp149() {};
        
        
  public: // Functions from base classes

        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        /**
        * 
        * @aFile the file name
        */
        void PrintParsedDataFromEncoderL(
            const TDesC& aFile, 
            const TDesC8&  aBuffer, 
            TInt aExpectedStatusCode,
            const TDesC8& aMimeType);
            
            
       void PrintLandmarkFieldsWithDescriptionL(const CPosLandmark& aLandmark, TBool aTraceFlag=EFalse);
       
       /**
        * Check that cancel of parse is correct
        */
        void DoCancelTestL();


        /**
        * Trace to trace file
        */
        void TraceL(const TDesC& msg);
        
            private:
        CPosLandmarkParser* iLandmarkParser;

        CPosLandmarkEncoder* iLandmarkEncoder;

        RPointerArray<CPosLandmark> iLandmarks;

        TInt iErrorsFound;

        CBufBase* iEncoderBuffer;
        
        CPosLmCategoryManager* iCategoryManager;

    };

#endif      // CPOSTp149_H
            
// End of File