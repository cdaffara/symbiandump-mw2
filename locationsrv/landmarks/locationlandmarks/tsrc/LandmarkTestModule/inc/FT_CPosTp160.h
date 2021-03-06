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


#ifndef CPOSTp160_H
#define CPOSTp160_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkParser;
class CPosLandmarkEncoder;
/**
*  Test procedure 160
*/
class CPosTp160 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp160(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp160() {};
        
        
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

#endif      // CPOSTp160_H
            
// End of File