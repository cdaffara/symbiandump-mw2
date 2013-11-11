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


#ifndef CPOSTP6_H
#define CPOSTP6_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 6
*/
class CPosTp6 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
       CPosTp6(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp6() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        void CheckGetMethodsL(CPosLandmarkCategory* aCategory, const TDesC& aName);

        void CheckRemoveMethodsL(CPosLandmarkCategory* aCategory);

        void CheckRemovedL(CPosLandmarkCategory* aCategory);

        void CheckFieldsMaxLengthL();
        
        void CheckEmptyCategoryNameL();
        
        void CheckSetIconL();

    };

#endif      // CPOSTP6_H
            
// End of File
