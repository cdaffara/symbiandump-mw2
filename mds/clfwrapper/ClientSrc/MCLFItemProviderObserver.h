/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/



#ifndef MCLFITEMPROVIDEROBSERVER_H
#define MCLFITEMPROVIDEROBSERVER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Item provider observer.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
class MCLFItemProviderObserver
    {
    public:

        /**
        *  Item provider operation is completed.
        *  @param aError system-wide error code
        */
        virtual void OperationCompleteL( TInt aError ) = 0;

    protected:

        // Destructor
        virtual ~MCLFItemProviderObserver() {}

    };

#endif      // MCLFITEMPROVIDEROBSERVER_H

// End of File
