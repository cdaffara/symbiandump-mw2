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


#ifndef CPOSTP18_H
#define CPOSTP18_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLandmark.h>

// CLASS DECLARATION


/**
*  Test procedure 18
*/
class CPosTp18 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp18(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp18() {};

    public: // Functions from base classes

        
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();
    
    private:

        void TestAllPartialAttributesL();

        void TestPartialPositionFieldsL();

        void PreparePartialLandmarksL();

        void PreparePartialLandmarksAsyncL(TExecutionMode aExecutionMode);
        
        void PreparePartialLandmarksAsyncAndCancelL(const TBool& aInCallback=EFalse);

        void AppendFields(RArray<TUint>& aArray);

        CPosLandmark* GetPartialLandmarkLC(const TPosLmItemId aId,
                                           const CPosLmPartialReadParameters& partialParam);
        
        void CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                   const CPosLandmark::TAttributes& aAttr);
        
        void CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                   const RArray<TUint>& aReqFields,
                                   const RArray<TBool>& aAvailFields);

        void CheckPartialErrorCodeL(const TBool aPartial, const TInt aErr);

    private: // data
        CArrayPtr<CPosLandmark>*    iArrayPtr;

    };

#endif      // CPOSTP18_H
            
// End of File
