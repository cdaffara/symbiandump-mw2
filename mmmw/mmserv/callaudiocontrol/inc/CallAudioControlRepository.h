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
* Description:  Defines the CallAudioControlRepository class used by CallAudioControl
*
*/


#ifndef CallAudioControlRepository_H
#define CallAudioControlRepository_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h> 
#include <centralrepository.h>
#include "CallAudioControlUtility.h"


// FORWARD DECLARATIONS
class CCallAudioControlImpl;

// CLASS DECLARATION
class CCallAudioControlRepository : public CActive
    {

    public:  

        static CCallAudioControlRepository* NewL(CCallAudioControlImpl* aImplParent, const TUid aUid, const TUint32 aKey, TAction aAction);

 		~CCallAudioControlRepository();

        void NotifyRequest();
        void NotifyCancel();
 		TInt Get(TInt& aValue);  
 		TInt Set(TInt aValue);    		     
    private:
    
    
  		// For CActive:
  		void DoCancel();
        void RunL();
   	TInt RunError();
 		  	
    	void ConstructL(CCallAudioControlImpl* aImplParent, const TUid aUid, const TUint32 aKey, TAction aAction);
    	CCallAudioControlRepository();
															   	    
    	// Data:
    	CCallAudioControlImpl* iImplParent;
        CRepository*	iRepository;  
	//TRequestStatus iStatus; -MB-
		TAction iAction;
		TBool iSubscribed;
		TUint32 iKey;			

    };
    
#endif      // CallAudioControlRepository_H

// End of File
