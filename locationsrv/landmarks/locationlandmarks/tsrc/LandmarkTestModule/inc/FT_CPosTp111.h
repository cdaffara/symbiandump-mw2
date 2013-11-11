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


#ifndef CPOSTP111_H
#define CPOSTP111_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLmDatabaseManager;
class CRepository;

/**
*  Test procedure 111
*/
class CPosTp111 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp111() : CLandmarkTestProcedureBase(111) {};
        
        /**
        * Destructor.
        */
        ~CPosTp111() {};

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

    private:
    
        TBool FindResourceFilesL();
        
        TBool FindServerResourceFilesL();
        
        TBool FindEcomPluginsL();

        TBool FindLibrariesL();
    
        void TestCentralRepositoryL();
        
        void FindFileL(const TDesC& aFile, TInt& aNrOfMatches, TDes& aPath);
        
        TBool FindFilesAndVerifyPathL(
            const CDesCArray& aFileNames, 
            const TDesC& aExpectedPath);
            
        void VerifyDefaultDbUriL(
            const TDesC& aExpectedUri, 
            CPosLmDatabaseManager* aDbMan);
            
        void VerifyDefaultDbUriL(
            const TDesC& aExpectedUri, 
            CRepository* aRepository);
            
    };

#endif      // CPOSTP111_H
            
// End of File
