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


#ifndef CPOSTP7_H
#define CPOSTP7_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

#include <e32base.h>

// CLASS DECLARATION

/**
*  Test procedure 7
*/
class CPosTp7 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp7(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp7() {};

    public: // Functions from base classes

        
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        /**
        * From CLandmarkTestProcedureBase
        * Always called, even if the test leaves
        */
        void CloseTest();

        static void RunTestL(TAny* aData);

    private: // Functions

        void CreateThreadsL();
        
    private: // data   
        
        RArray<RThread>	          iThreads;
       
        TInt                      iThreadIndex;
        TBool                     iOneSucces;
        
    };



#endif      // CPOSTP7_H
            
// End of File
