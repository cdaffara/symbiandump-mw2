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


#ifndef CPOSTp134_H
#define CPOSTp134_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"


// CLASS DECLARATION
const TInt KTopTest = 17;
/**
*  Test procedure 134
*/
class CPosTp134 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
      
        CPosTp134(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp134() {iDbArray.ResetAndDestroy();
                      delete iDbMan;};

    public: // Functions from base classes

       
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        void CheckInfoL(
                        HPosLmDatabaseInfo* aDbInfo,
                        TInt aTestNr
                        );
        void SetupExpectedResultsL();
        
        void CheckUriLengthL(
                        const TDesC& aUri,
                        TInt aExpectedErrorCode
                        );

        
        // data members
    private:
        CPosLmDatabaseManager* iDbMan;
        
        TChar      iDbDrive[KTopTest];
        TMediaType iMediaType[KTopTest];
        TBool      iDefaultDb[KTopTest];
        TBuf<50>   iDbUri[KTopTest];
        TBuf<10>   iProtocol[KTopTest];
        TBuf<25>   iDbName[KTopTest];
        TBool      iAttrSet[KTopTest];
        RPointerArray<HPosLmDatabaseInfo> iDbArray;
        TInt       iErrorsFound;
    };

#endif      // CPOSTp134_H
            
// End of File
