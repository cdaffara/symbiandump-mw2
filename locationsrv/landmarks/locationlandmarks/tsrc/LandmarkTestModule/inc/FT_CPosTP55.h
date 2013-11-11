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


#ifndef CPOSTP55_H
#define CPOSTP55_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 55
*/
class CPosTp55 : public CLandmarkTestProcedureBase,
public MProgressObserver
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp55() : CLandmarkTestProcedureBase(55) {};
        
        /**
        * Destructor.
        */
        ~CPosTp55() {};

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
        * From CUtfwTestBase
        */
        void CloseTest();

        /**
        * From CUtfwTestBase
        */
        void InitTestL();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        /**
        * From MProgressObserver
        * Notifies the progress of the execution of a CPosLmOperation object.
        *
        * @param sProgress the progress of the operation
        */
        void NotifyProgress(TReal aProgress);

    private:    // New functions

        void TestCriticalLevelL();

        void RecreateAndOpenFilesL();

        void ExportLandmarksTestL();

        void ImportLandmarksTestL(TBool aCriticalLevelPassed);

        void ConsumeDiskSpaceL(RFile& aFile, TInt aSizeLimit);        

        /**
        * Imports landmarks from an XML file or a buffer to the database.
        *
        * @param aInputFile file to import from. KNullDesC if buffer is to be 
        * used as target.
        * @param aInputBuffer buffer to import landmarks from. NULL if not used.
        * @param aArrayOfIds to export when calling ExportLandmarksL(). NULL 
        * if encoder is to be used stand-alone. 
        * @param aExecuteIncrementally ETrue if CPosLmOperations should be 
        * executed incrementally, EFalse if CPosLmOperations should be 
        * synchronously.
        * @param aTestBeyondCriticalLevel ETrue if critical level is passed, EFalse otherwise
        */
        void ImportLandmarksL(
            const TDesC& aInputFile, 
            RArray<TUint>* aArrayOfIds, 
            TBool aExecuteIncrementally,
            TBool aTestBeyondCriticalLevel);

        void InitiateCriticalLevelL();

        void LogVolumeInfoL();

        void DeleteFileL(const TDesC& aFile);

        void FillFile3L();

        HBufC8* CreateInputBufferL();

        void ReInitializeEncoderEtcL();

        TInt ExecuteLmOperationL(TBool aExecuteIncrementally);

        void SetOutputBufferL();

        void AddCollectionDataToEncoderL();

        void AddLandmarksToEncoderL(TBool aAddAll);

        void ExportWithExportLandmarksMethodL(
            RArray<TPosLmItemId>& aArrayOfIds,
            TBool aExecuteIncrementally);

        void FinalizeEncoderL(TBool aExecuteIncrementally);

        void PopulateArrayL(
            RArray<TPosLmItemId>& aArray,
            TBool aDoAddAllLms);

        void CopyResourceFileL(const TDesC& aResourceFile = KNullDesC);
    
        void TryRestoreResourceFile();

    private:    // Data

        RFs     iFileServer;
        RFile   iFile1;
        RFile   iFile2;
        RFile   iFile3;
        RFile   iXmlOutputFile;
        TInt    iCriticalLevel;
        TInt    iCriticalThreshold;
        TInt    iSizeLimit1;
        TInt    iSizeLimit2;
        TBuf<150> iMsg;
        TInt    iProgress;

        CBufBase*               iOutputBuffer;
        HBufC8*                 iInputBuffer;
        CPosLandmarkEncoder*    iLandmarkEncoder;
        CPosLandmarkParser*     iLandmarkParser;
        CPosLmOperation*        iOperation;
        CActiveLmOperation*     iOperationWrapper;
    };

#endif      // CPOSTP55_H
            
// End of File
