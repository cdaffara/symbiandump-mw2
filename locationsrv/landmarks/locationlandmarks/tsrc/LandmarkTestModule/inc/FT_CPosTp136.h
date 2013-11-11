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


#ifndef CPOSTp136_H
#define CPOSTp136_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 136
*/
class CPosTp136 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp136(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp136() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
        void CloseTest();

    private:
    
        void TestImportL(const TDesC& aFile, const TDesC8& aMime);
        
        void TestParseL(const TDesC& aFile, const TDesC8& aMime);
        
        TInt iErrorsFound;
    };
    

#endif      // CPOSTp136_H
            
// End of File
