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


#ifndef CPOSTP49_H
#define CPOSTP49_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <apgcli.h>


// CLASS DECLARATION

class CDatabaseClient;

/**
*  Test procedure 49
*/
class CPosTp49 : public CLandmarkTestProcedureBase, public MProgressObserver
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp49() : CLandmarkTestProcedureBase(49) {};
        
        /**
        * Destructor.
        */
        ~CPosTp49() {};

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
        * Initializes a test before StartL is called.
        */
        void InitTestL();

        /**
        * From CLandmarkTestProcedureBase
        * Cleans up after a test.
        * Always called, even if the test leaves. 
        */
        void CloseTest();

        /**
        * From MProgressObserver
        */
        void NotifyProgress(TReal aProgress);

    private:

        void ImportAllLandmarksL();
        TInt ImportSomeLandmarksL();
        void VerifyImportedLandmarksL(TInt aNrOfExpectedItems);
        void ParseLandmarksContentL();
        void RemoveThreeLandmarksL();
        void SetupPreConditionsL();
        void ExecuteLmOpL(TInt aExpectedResult, TBool aObserveProgress = EFalse);
        void FetchIteratorL();
        void RecreateParserL(const TDesC& aFileName);
        void StartImportLandmarksL();
        void CreateNameArrayL(TInt aNrOfItems);

    private:

        // Contains the IDs of the improted landmarks
        RArray<TPosLmItemId>    iImportedLandmarkIds;

        // Contains the IDs of imported landmarks that have been removed
        RArray<TPosLmItemId>    iRemovedLandmarkIds;

        // The parser that parses the xml file to import
        CPosLandmarkParser*     iLandmarkParser;

        // The iterator that is to be tested
        CPosLmItemIterator*     iIterator;

        // An array containing the names of the imported landmarks
        CDesCArray*             iLandmarkNames;

        // Indicates when it is time to fetch iterator of imported landmarks.
        TReal                   iProgressInterruptLevel;

    };

#endif      // CPOSTP49_H

            
// End of File
