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


#ifndef CPosTp45_H
#define CPosTp45_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

//CONSTANTS

// CLASS DECLARATION
class CPosLandmarkEncoder;

/**
*  Test procedure 45
*/
class CPosTp45 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp45(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp45() {};

    public: // Functions from base classes

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        /**
        * From CLandmarkTestProcedureBase
        * Cleans up after a test.
        * Always called, even if the test leaves. 
        */
        void CloseTest();
        
    private:
    
        void ExportLandmarksL(  const TDesC& aFileName, 
                                RArray<TPosLmItemId>& aArrayOfIds, 
                                CPosLandmarkDatabase::TTransferOptions aTransferOptions);
        
        void ExportLandmarksIncL(   const TDesC& aFileName, 
                                    RArray<TPosLmItemId>& aArrayOfIds, 
                                    CPosLandmarkDatabase::TTransferOptions aTransferOptions,
                                    TBool aUseWaitForRequest);
        
        void ExportWithCollectionDataL(RArray<TPosLmItemId>& aArrayOfIds, 
                                       CPosLandmarkDatabase::TTransferOptions aTransferOptions);
        
        void ExportAndCancelL(  const TDesC& aFileName,
                                const TBool& aInCallback, 
                                CPosLandmarkDatabase::TTransferOptions aTransferOptions);
                        
        void VerifyCancelL(const TDesC& aFileName, const TInt aMaxExported);                
                              
        void AddLandmarksL();
        
        void AddCategoriesL();
        
        void RenameGlobalCategoryL();
        
        void TestErrorCodesL();
        
        void DeleteFile(const TDesC& aFile);

        /**
        * Test to export to a file with long filename (more than 255 characters)
        */
        void ExportToFileWithLongFileNameL();
        
        // Added for BC testing 
        void MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                   CPosLmCategoryManager& aCatMgr ); 
                                   
		void SetupGlobalCategoriesL();                                   
        
    private:
        
        CPosLmCategoryManager*                 iCategoryManager;
        RPointerArray<CPosLandmarkCategory>    iCategories;
        RArray<TUint>    iGlobalCategoryIds;
        RArray<TPtrC>    iGlobalCategoryNames;
        RArray<TPtrC>    iGlobalCategoryTestNames;
        
        CPosLandmarkEncoder*    iLandmarkEncoder;
        
        CBufBase*               iBuffer;
        
        TBool                   iLogToFile;

    };

#endif      // CPosTp45_H

            
// End of File
