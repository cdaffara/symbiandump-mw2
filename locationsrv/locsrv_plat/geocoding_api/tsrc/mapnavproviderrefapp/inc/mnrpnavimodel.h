/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpNaviModel class definition
*
*/


#ifndef MNRP_NAVIMODEL_H
#define MNRP_NAVIMODEL_H

#include <coecntrl.h>
#include "mnrppositionrequest.h"

class CMnrpEngine;
class CPosLandmark;

/** Model observer class */
class MNaviModelObserver
    {
    public:
        virtual void HandleModelUpdateL() =0;
    };

/** Navigation view model. 
 *	Prepares data to be shown on navigation view
 */
class CMnrpNaviModel : public CBase, public MMnrpPositionObserver
    {
    public:
        static CMnrpNaviModel* NewL( MNaviModelObserver& aObserver, CMnrpEngine& aEngine );

		/** Called by service class to inform that client has changed 
		 *	navigation target */
        void SetDestinationL( const CPosLandmark& aNewDestination );

		/** List item information */
        class CListItem : public CBase
            {
            public:
                static CListItem* NewL( 
                	const TDesC& aItemHeader, 
                	const TDesC& aItemValue );
            
                ~CListItem();
                
                const TDesC& Header() { return *iHeader; };
                const TDesC& Value() { return *iValue; };
                
            private:
            	/** List item header */
                HBufC* iHeader;
            	/** List item text */
                HBufC* iValue;                
            };

		/** Returns list items to be shown on Navigation view */
        TArray<CListItem*> ListItemsL();

        ~CMnrpNaviModel();
        
    public:  // from MMnrpPositionObserver

		/** Informs navigation view that data has been changed */
        void HandlePositionRequestCompletedL( TInt aError );
        
    private: 

        void ConstructL();
        CMnrpNaviModel( MNaviModelObserver& aObserver, CMnrpEngine& aEngine );
    
    private:
        CMnrpEngine&                    iEngine;
    
        CMnrpPositionRequest*   		iPositionRequest;
        TPositionInfo                   iPosInfo;

        CPosLandmark*                   iDestination;
        
        MNaviModelObserver&             iObserver;
        
        RPointerArray<CListItem>        iListItems;
    };

#endif // MNRP_NAVIMODEL_H

