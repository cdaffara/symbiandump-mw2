/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPosLmNameIndexIterator class
*
*
*/


#ifndef EPOS_CPOSLMNAMEINDEXITERATOR_H_
#define EPOS_CPOSLMNAMEINDEXITERATOR_H_

#include <e32base.h>
#include <epos_rposlmlocalnameindex.h>

class CPosLmNameIndexIterator : public CBase
    {
    public:
    
        enum TIndexMode
            {
            EModePartial,   //< discards already enumerated items on next load
            EModeFull       //< keeps all index items
            };
    
    public:
        static CPosLmNameIndexIterator* NewL( 
            RPosLmLocalNameIndex& aIndex,
            TBool aAscending );
        ~CPosLmNameIndexIterator();
        
        TInt Count() const;
        TInt TotalCount() const;
        TInt LoadedCount() const;

        TBool HasMore() const;
        void LoadMoreL( TInt aCount );
        
        void First();
        TBool Next();
        TBool AtEnd() const;
        TInt CurrentPosition() const;

        const RPosLmLocalNameIndex::CIndexItem& Current() const;
        
        void SetMode( TIndexMode aMode );
        TIndexMode Mode() const;
        
        const RPosLmLocalNameIndex::CIndexItem& Item( TInt aIndex ) const;
        TInt Find( TPosLmItemId aLmid ) const;
        
    protected:
        CPosLmNameIndexIterator( 
            RPosLmLocalNameIndex& aIndex,
            TBool aAscending );
        
        void DiscardProcessedItems();
        
    private:
        RPosLmLocalNameIndex& iIndex;
        TInt iLeftInIndex;
        TInt iNextToLoad;

        RPointerArray<RPosLmLocalNameIndex::CIndexItem> iSortedLandmarks;
        TInt iCurrent; // index in iSortedLandmarks
        TIndexMode iMode;
        
        TBool iAscending;
    };

#endif /*EPOS_CPOSLMNAMEINDEXITERATOR_H_*/
