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


#ifndef CPOSTP2_H
#define CPOSTP2_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <ecom.h>
// CLASS DECLARATION


/**
*  Test procedure 2
*/
class CPosTp2 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp2(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp2() {};

    public: // Functions from base classes

      
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
    
    private:

        void WaitForEcomL();
    
    };

#endif      // CPOSTP1_H
            
// End of File
