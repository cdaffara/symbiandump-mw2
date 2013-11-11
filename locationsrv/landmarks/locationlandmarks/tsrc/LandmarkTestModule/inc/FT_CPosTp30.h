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


#ifndef CPOSTP30_H
#define CPOSTP30_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

#include <e32base.h>

// CLASS DECLARATION

/**
*  Test procedure 30
*/
class CPosTp30 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp30() : CLandmarkTestProcedureBase(30) {};
        
        /**
        * Destructor.
        */
        ~CPosTp30() {};

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
        
    private: // data   
        
        RArray<RThread>	    iThreads; 
        TInt                iThreadIndex;
        
        TBool               iRemoveTest;
        TBool               iAsyncSearch;
    };



#endif      // CPOSTP30_H
            
// End of File
