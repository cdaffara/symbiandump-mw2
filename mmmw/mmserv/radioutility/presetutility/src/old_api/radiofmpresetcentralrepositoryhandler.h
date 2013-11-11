/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Defines APIs to communicate with Central Repository
*
*/


#ifndef RADIOFMPRESETCENTRALREPOSITORYHANDLER_H
#define RADIOFMPRESETCENTRALREPOSITORYHANDLER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  it is a wrapper of Central Repository, responsible for the operations that need to access 
*  the central respositorhy.
*
*  @lib FmPresetUtility.lib
*  @since Series 60 3.0_version
*/
class CCentralRepositoryHandler : public CBase
    {
    public:
      	/**
      	* Two-phased class constructor.
      	*/
        static CCentralRepositoryHandler* NewL();
      	/**
      	* Destructor of CCentralRepositoryHandler class.
      	*/
        virtual ~CCentralRepositoryHandler();
              
    private:
      	/**
      	* Default class constructor.
      	*/
        CCentralRepositoryHandler();
    };

#endif  // RADIOFMPRESETCENTRALREPOSITORYHANDLER_H

// end of file
