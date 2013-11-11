/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface class for Networkhandling
*
*/


#ifndef CNWSESSION_H
#define CNWSESSION_H

//  INCLUDES
#include "nwhandlingengine.h" 


// CLASS DECLARATION
/**
*  Interface class for Networkhandling
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWSession : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNWSession();


    protected:

        /**
        * C++ default constructor.
        */
        CNWSession();

    };

#endif    // CNWSession_H
            
// End of File
