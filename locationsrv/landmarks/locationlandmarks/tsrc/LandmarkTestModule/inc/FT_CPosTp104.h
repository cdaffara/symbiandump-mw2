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


#ifndef CPOSTp104_H
#define CPOSTp104_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLmDatabaseManager.h>
#include <centralrepository.h>



/**
*  Test procedure 104
*/
class CPosTp104 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp104(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp104() {delete iDbMan; delete iRepository; delete iLandmarksDb;};

    public: // Functions from base classes

        
		void CloseTest();
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

	private:
	
	    void TestSetUriL(
	            const TDesC& aUriString,
	            TInt aExpectedErrorCode
	            );
	    void ArgumentTestsL();
	    
    CPosLmDatabaseManager* iDbMan;
    CRepository*           iRepository;
    CPosLandmarkDatabase*  iLandmarksDb;

    };

#endif      // CPOSTp104_H
            
// End of File
