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


#ifndef CPOSTP135_H
#define CPOSTP135_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
/**
*  Test procedure 135
*/
class CPosTp135 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp135(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp135() {};

    public: // Functions from base classes

       
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
    private:
    
    void VerifyDbListL(CDesCArray* aDbArray);
    
    void VerifyDbInfoListL(RPointerArray<HPosLmDatabaseInfo> aDbInfoList, RPointerArray<HPosLmDatabaseInfo> aDbInfoList2);

    private:
          CDesCArray* iDbArray;
          TInt        iErrorsFound;

    };
    
#endif      // CPOSTP135_H
            
// End of File
