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
* Description:  Container class for refresh related implementation.
*
*/


#ifndef CSSSETTINGSREFRESHCONTAINER_H
#define CSSSETTINGSREFRESHCONTAINER_H


//  INCLUDES
#include <msssettingsrefreshhandler.h> 


// FORWARD DECLARATIONS
class CSSSettingsRefreshHandler;


// CLASS DECLARATION

/**
* Container class for refresh related implementation.
* 
* @since    2.8
* @lib      SsSettings.lib
*/
class CSSSettingsRefreshContainer :
    public CBase
    {
    public: // Constructor and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSSSettingsRefreshContainer* NewL();

        /**
        * Destructor.
        */
        IMPORT_C ~CSSSettingsRefreshContainer();


    public: // New functions

        /**
        * Return the refresh handler instance.
        * 
        * @return The Refresh handler instance.
        */
        IMPORT_C MSSSettingsRefreshHandler& RefreshHandler();


    private:

        /**
        * C++ default constructor.
        */
        CSSSettingsRefreshContainer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private:    // Data

        CSSSettingsRefreshHandler* iRefreshHandler;
    };

#endif      // CSSSETTINGSREFRESHCONTAINER_H  
            
// End of File
