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
* Description:  ?Description
*
*/


#ifndef MMGFETCHCANCELER_H
#define MMGFETCHCANCELER_H

// CLASS DECLARATION

/**
*  Interface for canceling the fetch popup
*
*  @since 2.6
*/
class MMGFetchCanceler
    {
    public:  // New functions
        /**
        * Calling this method while the fetcher (MGFetch.h) popup is visible
        * (when the MMGFetchCanceler pointer is NOT NULL) causes the fetcher
        * to close.
        *
        * @since 2.6
        */
        virtual void CancelFetcherL() = 0;
    };

#endif // MMGFETCHCANCELER_H

// End of File
