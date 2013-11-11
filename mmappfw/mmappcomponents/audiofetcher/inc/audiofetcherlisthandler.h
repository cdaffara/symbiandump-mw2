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
* Description: Media Fetch plug-in list handler
*
*/

// INCLUDES
#include <e32base.h>
#include <bamdesca.h>

/**
* MAudioFetcherListHandlerObserver
* 
* CAudioFetcherListHandler uses MAudioFetcherListHandlerObserver to get listbox item texts.
*/
class MAudioFetcherListHandlerObserver
    {
    public:
        virtual TPtrC ConstructListboxItem( TInt aListIndex ) = 0;
        virtual TInt ListboxItemCount() = 0;
    };

/**
* CMFListHandler
* 
* This class is used as listbox model's item array. Listbox uses it to draw 
* list items. CAudioFetcherListHandler gets list item texts from CAudioFetcherFileHandler
* (metadata query result). This way there is no need to copy all list items
* into listbox model.  
*/
class CAudioFetcherListHandler : public CBase, public MDesCArray
    {
    public:
        static CAudioFetcherListHandler* NewL();
        virtual ~CAudioFetcherListHandler();
        
    private:
        
        void ConstructL();
        CAudioFetcherListHandler();

    public:  // from MDesCArray
        
        TInt MdcaCount() const;
        TPtrC16 MdcaPoint(TInt aIndex) const;

    public:
        
        void SetObserver( MAudioFetcherListHandlerObserver* aObserver );
        static void Panic( TInt aReason );
        
    private:
        
        MAudioFetcherListHandlerObserver* iObserver;  // does not own
    };
