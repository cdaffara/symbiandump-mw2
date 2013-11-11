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


#ifndef CPOSTP118_H
#define CPOSTP118_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 118
*/
class CPosTp118 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp118(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp118() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
        void DeleteTp118FilesL();

    private:

    	TBool VerifyList(CDesCArray* aList, const TDesC& aDbName);
    	
    	void CreateLandmarksInDbL(const TDesC& aDbName, TInt aIndex=0);
    	
    	void DropTableL(const TDesC& aDbName, const TDesC& aTable);
    	
    	void DropIndexL(const TDesC& aDbName, const TDesC& aTable, const TDesC& aIndex);
    	
    	void AlterTableL(const TDesC& aDbName);
    	
    	void CheckLandmarkDbL(CPosLandmarkDatabase* aDb);
    	
    	private:
    	    CDesCArray*             iDatabases;

    };

#endif      // CPOSTP118_H

            
// End of File
