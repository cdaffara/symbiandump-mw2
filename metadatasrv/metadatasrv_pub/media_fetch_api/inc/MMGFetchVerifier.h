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


#ifndef MMGFETCHVERIFIER_H
#define MMGFETCHVERIFIER_H


// INCLUDES
#include <bamdesca.h> // MDesCArray

// CLASS DECLARATION

/**
*  Interface for verifying fetch selection before the fetch popup is closed
*
*  @since 2.0
*/
class MMGFetchVerifier
    {
    public:   // Constructors and destructor

        virtual ~MMGFetchVerifier() {}

    public:  // New functions
        /**
        * Interface for verifying fetch file selection (see MGFetch.h)
        * If MMGFetchVerifier is passed to MGFetch then VerifySelectionL
        * is called when user tries to select file(s) from the fetcher.
        * If VerifySelectionL returns ETrue then the fetcher popup is closed and
        * the selection is accepted. If VerifySelectionL returns EFalse then the
        * fetcher is not closed and user needs to select different file(s) or
        * cancel the operation.
        *
        * @since 2.0
        * @param aSelectedFiles Array holding the file(s) to be verified
        * @return ETrue if selection is accepted, EFalse if not
        */
        virtual TBool VerifySelectionL( const MDesCArray* aSelectedFiles ) = 0;
    };
#endif // MMGFETCHVERIFIER_H

// End of File
