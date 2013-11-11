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
* Description:  Header of BalanceControl interface.
*
*/


#ifndef BALANCECONTROL_H
#define BALANCECONTROL_H

#include <EffectControl.h>

namespace multimedia
    {    
    // Uid identifying Balance Control
    const TUid KBalanceEffectControl = {0x10207C11};
    
    class MBalanceControl : public MEffectControl
        {
        public:
        	/**
        	Returns the current stream balance.
        	@return The current stream balance value.
        	*/        
            virtual TInt GetBalance(TInt& aBalance) = 0;
        	/**
        	Sets the current stream audio balance.        	
        	The balance can be any value between KMMFBalanceMaxLeft and KMMFBalanceMaxRight,
        	the default value being KMMFBalanceCenter.
        	@param  aBalance
        	        A specified balance value.
        	*/            
            virtual TInt SetBalance(TInt& aBalance) = 0;                        
        };
    } // namespace multimedia

#endif // BALANCECONTROL_H

// End of file
