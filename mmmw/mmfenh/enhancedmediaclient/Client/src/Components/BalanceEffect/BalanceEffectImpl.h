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
* Description:  Project file for EnhancedMediaClient Utility
*
*/

#ifndef BALANCEEFFECTIMPL_H_
#define BALANCEEFFECTIMPL_H_

#include <e32base.h>
#include <BalanceControl.h>
#include "EffectControlBase.h"

namespace multimedia
    {
    class CBalanceEffect : public CBase,
                           public MBalanceControl,
                           public CEffectControlBase
        {
    public:
    	CBalanceEffect();
    	~CBalanceEffect();
    	TInt PostConstructor();

    	// From MControl begins
        TInt AddObserver( MControlObserver& aObserver );
        TInt RemoveObserver( MControlObserver& aObserver );
        TUid Type();
        TControlType ControlType();  
        // From MControl ends

        // From MEffectControl begins
        TInt Apply();
        // From MEffectControl ends
        
        // From MBalanceControl begins
        TInt GetBalance(TInt& aBalance);
        TInt SetBalance(TInt& aBalance);
        // From MBalanceControl ends
        
        // From CEffectControlBase begins
        void Event( TEffectControlEvent aEvent );
        // From CEffectControlBase ends
        
    private:
        TInt DoApply();        
    private:
    	TInt iBalance;    	
        // Set when Client calls Apply
        TBool iEnabled;    	
        };//class CBalanceEffect
        
    } // namespace multimedia

#endif /*BALANCEEFFECTIMPL_H_*/

// End of file
