/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the handler for the SIM Application Toolkit
*                to dedect the SIM card subscriber id.
*
*/



#ifndef MSATSSIMSUBSCRIBERID_H
#define MSATSSIMSUBSCRIBERID_H

//  INCLUDES
#include <e32std.h>
#include <etelmm.h>

// CLASS DECLARATION

/**
*  SIM Subscriber id handler interface.
*
*  @lib SATEngine.lib
*  @since Series 60 3.0
*/
class MSatSSimSubscriberId
    {
    public:  // Constructors and destructor

        /*
        * Destructor.
        */
        virtual ~MSatSSimSubscriberId() {};

    public: // New functions

        /**
        * Starts listening for the SIM card subscriber id.
        */
        virtual void Start() = 0;

    protected:

        /**
        * Constructor.
        */
        MSatSSimSubscriberId() {};

        // Prohibit copy constructor if not deriving from CBase.
        MSatSSimSubscriberId( const MSatSSimSubscriberId& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSSimSubscriberId& operator=( const MSatSSimSubscriberId& );

    };

#endif      // MSATSSIMSUBSCRIBERID_H

// End of File
