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
* Description: This class is only used internally in CPosLmMultiDbSearch.
*
*
*/


#ifndef CPOSLMMULTIDBSEARCHITEM_H
#define CPOSLMMULTIDBSEARCHITEM_H

//  INCLUDES

// CONSTANT DECLARATIONS

// FORWARD DECLARATIONS
class CPosLandmarkDatabase;
class CPosLandmarkSearch;
class CPosLmDisplayData;

// CLASS DECLARATION

/**
* This class is only used internally in CPosLmMultiDbSearch.
*/
NONSHARABLE_CLASS(CPosLmMultiDbSearchItem) : public CBase
    {
    public:  // Constructors and destructor

        CPosLmMultiDbSearchItem(HBufC* aDbUriPtr);
        ~CPosLmMultiDbSearchItem();

    public:  // Data types

        HBufC*                  iDbUriPtr;
        CPosLandmarkDatabase*   iDb;
        CPosLandmarkSearch*     iSearcher;
        CPosLmDisplayData*      iDisplayData;
        TBool                   iToBeSearched;
        TBool                   iHasValidSearchResult;
    };

#endif      // CPOSLMMULTIDBSEARCHITEM_H

// End of File
