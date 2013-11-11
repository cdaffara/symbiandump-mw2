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


#ifndef CPOSTP5_H
#define CPOSTP5_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifLogger.h>

// CLASS DECLARATION

/**
*  Test procedure 5
*/
class CPosTp5 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
           
            CPosTp5(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp5() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        void CheckGetMethodsL(CPosLandmark* aLandmark);

        void CheckRemoveMethodsL(CPosLandmark* aLandmark);

        void CheckRemovedL(CPosLandmark* aLandmark);

        void CheckPositionFieldL(const CPosLandmark& aLandmark, const TUint16 aId);

        void CheckFieldsMaxLengthL();

        void CheckNaNErrorL();

    };

#endif      // CPOSTP51_H
            
// End of File
