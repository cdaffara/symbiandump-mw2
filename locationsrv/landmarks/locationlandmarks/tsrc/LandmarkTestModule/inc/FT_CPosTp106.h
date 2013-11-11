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


#ifndef CPOSTp106_H
#define CPOSTp106_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

#include <e32base.h>

// CLASS DECLARATION

/**
*  Test procedure 50
*/
class CPosTp106 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp106(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp106() {};

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
        
        void CreateAndListDatabasesL(TInt index);
        
        void CopyDatabasesL(TInt index);
        
        void DeleteDatabasesL();
        
        void SetAndGetUriL(TInt index);
        
        void GetDisplayNameL();
        
        void CheckIfExistL(TInt index);

    private: // Functions
      
        void StartMultipleClientsL(TUint aNoClients);

        void CreateThreadsL(const TUint aNoThreads);

    private: // data
         
         TTime iStart;
         TTime iStop;
         	
        enum TTestStep
			{
            Step1               =0,	
            Step2               =1,	
            Step3      			=2,
            Step4    			=3,
            Step5    			=4,
            Step6    			=5
			};
        
        TInt                iTestStep;
        
        RArray<RThread>	    iThreads; 
        TInt                iThreadIndex;
        TInt                iThreadNum;
    };



#endif      // CPOSTp106_H
            
// End of File
