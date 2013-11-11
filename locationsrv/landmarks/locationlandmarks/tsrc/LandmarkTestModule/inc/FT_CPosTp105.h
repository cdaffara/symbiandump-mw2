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


#ifndef CPOSTp105_H
#define CPOSTp105_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLmDatabaseManager.h>
#include <centralrepository.h>

// CLASS DECLARATION

/**
*  Test procedure 105
*/
class CPosTp105 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp105() : CLandmarkTestProcedureBase(105) {};
        
        /**
        * Destructor.
        */
        ~CPosTp105() {delete iDbMan;};

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

	private:
	    void CheckDisplayNameL(const TDesC& aLanguage, const TDesC& aDisplayName);
	    
	    void CopyResourceFileL(const TDesC& aResourceNr);
	    
	    void RemoveResourceFileL(const TDesC& aResourceNr);
	    
	private:
	    
    CPosLmDatabaseManager* iDbMan;

    };

#endif      // CPOSTp105_H
            
// End of File
