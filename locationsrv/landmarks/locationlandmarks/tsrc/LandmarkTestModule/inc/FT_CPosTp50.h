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


#ifndef CPOSTP50_H
#define CPOSTP50_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

#include <e32base.h>

// CLASS DECLARATION

/**
*  Test procedure 50
*/
class CPosTp50 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp50() : CLandmarkTestProcedureBase(50) {};
        
        /**
        * Destructor.
        */
        ~CPosTp50() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

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
      
        void StartMultipleClientsL(TUint aNoClients);

        void CreateThreadsL(const TUint aNoThreads);
        
        void ExportLandmarksL();
        
        void TestESLI_64LLU3L();

    private: // data   
         
        enum TTestStep
			{
            ESync               =0,	
            EInc                =1,	
            EReadLockTaken      =2,
            ESearchCancelled    =3
			};
        
        TInt                iTestStep;
        
        RArray<RThread>	    iThreads; 
        TInt                iThreadIndex;
        
    };



#endif      // CPOSTP50_H
            
// End of File
