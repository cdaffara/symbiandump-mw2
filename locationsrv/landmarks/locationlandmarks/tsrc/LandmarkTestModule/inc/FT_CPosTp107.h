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

#ifndef CPosTp666_H
#define CPosTp666_H

//  INCLUDES
#include <s32file.h>
#include "FT_CLandmarkTestProcedureBase.h"

// FORWARD DECLARATION

// CLASS DECLARATION

/**
*  Test procedure 666
*
*/
class CPosTp666 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp666() : CLandmarkTestProcedureBase(666) {};
        
        /**
        * Destructor.
        */
        ~CPosTp666();

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        *
        */
        void GetName(TDes& aName) const;

        /**
        * From CPosTestProcedureBase
        * Not used here. See StartL(TInt aIndex)
        */
        void StartL();

    private:
    
    void SetupTestPartL(TInt aIndex);
            
    void SetupTestEnvironmentL();
        
    };

#endif      // CPosTp666_H
            
// End of File
