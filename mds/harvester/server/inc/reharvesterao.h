/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active Object which actually performs the re-harvesting of the files*
*/


#ifndef __CREHARVESTERAO_H__
#define __CREHARVESTERAO_H__

#include <e32base.h>
#include "harvesterqueue.h"

class CReHarvesterAO : public CActive
	{
    public:
    
        static CReHarvesterAO* NewL();
        static CReHarvesterAO* NewLC();
        virtual ~CReHarvesterAO();
        void ConstructL();

        void RunL();

        void DoCancel();

        TInt RunError( TInt aError );
        
        void SetHarvesterQueue( CHarvesterQueue* aQueue );
        void AddItem( CHarvesterData* aItem );
        void CheckItem( CHarvesterData& aItem );
        TInt ItemsInQueue();

	private:
	
	    CReHarvesterAO();
		
 	private:
 	    CHarvesterQueue* iQueue;
 	    RPointerArray<CHarvesterData> iItems;
 	    RTimer iTimer;
 	    TInt iDelay;
	};
	
#endif //__CREHARVESTERAO_H__

