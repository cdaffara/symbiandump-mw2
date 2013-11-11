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
*    Active object for de-initialization request cycle.
*
*/



#ifndef __EPOS_CSUPLDEINITREQUESTOR_H
#define __EPOS_CSUPLDEINITREQUESTOR_H

// INCLUDE FILES
#include <e32base.h>


// CONSTANT DECLARATIONS

// CLASS DECLARATION

/**
*    Active object implementing a de-initialization request cycle. 
*
*/
class CSuplServerDeInitRequest : public CActive
    {
    public:  // Constructors and destructor
   
        /**
        * Two-phased constructor.
        */
        static CSuplServerDeInitRequest* NewL(CSuplSessionManager& aSessionManager);

        /**
        * Destructor.
        */
        virtual ~CSuplServerDeInitRequest();

    public:  // New functions

        /**
        * Starts a DeInitialize request cycle. 
        */

        void DeInitialize();
		
    protected:  // From CActive

        void RunL();
        TInt RunError(TInt aError);
        void DoCancel();

    private:

        CSuplServerDeInitRequest(CSuplSessionManager& aSessionManager);

        void ConstructL();

        // By default, prohibit copy constructor
        CSuplServerDeInitRequest( const CSuplServerDeInitRequest& );
        // Prohibit assigment operator
        CSuplServerDeInitRequest& operator= ( const CSuplServerDeInitRequest& );
 		
	
    private:  // Data
        CSuplSessionManager&        iSessionManager;

    };  

#endif  // __EPOS_CSUPLDEINITREQUESTOR_H

// End of File
