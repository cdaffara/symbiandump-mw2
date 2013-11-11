/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Phone Server scheduler.
*
*/


#ifndef CPHSRVSCHEDULER_H
#define CPHSRVSCHEDULER_H

// INCLUDES
#include <e32base.h>


// CLASS DECLARATION

/**
*  It is the scheduler for active objects.
*  
*  @since 1.0
*/
class CPhSrvScheduler : 
    public CActiveScheduler
    {
    public: // Constructors and destructors

        /**
        * C++ default constructor.
        */
        CPhSrvScheduler();


    public: // Functions from base classes

        /**
        * From CActiveScheduler, it is called in case of unhandled error.
        *
        * @param aError It is the error code.
        */
        void Error( TInt anError ) const;

    };

#endif      // CPHSRVSCHEDULER_H


// End of File
