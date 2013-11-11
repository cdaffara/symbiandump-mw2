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
* Description: This class is the extension class for CPosLmDatabaseManagerPluginBase
*  which is needed for preserving binary compatibility.
*
*
*/


#ifndef CPOSLMDBMANPLUGINBASEEXTENSION_H
#define CPOSLMDBMANPLUGINBASEEXTENSION_H

//  INCLUDES
//#include <e32base.h>
//#include <badesca.h>

// FORWARD DECLARATIONS
class RPosLandmarkServer;

// CLASS DECLARATION

/**
*  This class is the extension class for @ref CPosLmDatabaseManagerPluginBase
*  which is needed for preserving binary compatibility.
*
*  @lib eposlmdbmanlib.lib
*  @since S60 3.0
*  @version $Revision: 1.5 $, $Date: 2005/07/07 13:40:39 $
*/
NONSHARABLE_CLASS(CPosLmDbManPluginBaseExtension) : public CBase
    {
    public:

        /**
        * Destructor.
        */
        virtual ~CPosLmDbManPluginBaseExtension();

        /**
        * C++ default constructor.
        * @param aSession Pointer to a landmark server session handle.
        */
        CPosLmDbManPluginBaseExtension(
        /* IN */ RPosLandmarkServer* aSession);

    private:

        // By default, prohibit copy constructor
        CPosLmDbManPluginBaseExtension(
            const CPosLmDbManPluginBaseExtension& );
        // Prohibit assigment operator
        CPosLmDbManPluginBaseExtension& operator=(
            const CPosLmDbManPluginBaseExtension& );

    public:    // Data

        RPosLandmarkServer* iSession;
    };

#endif      // CPOSLMDBMANPLUGINBASEEXTENSION_H

// End of File
