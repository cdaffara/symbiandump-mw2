/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Factory for creating divert specific components.
*
*/


#ifndef CSETCALLDIVERTINGCREATOR_H
#define CSETCALLDIVERTINGCREATOR_H

//  INCLUDES
#include    <e32base.h>
#include    <badesca.h>
#include    <etelmm.h>
#include    <nwdefs.h>
#include    "mpsetdivertobs.h" 
#include    "mcalldiverting.h" 

/**
*  Factory class for creating divert related classes.
*
*  @lib PhoneSettings
*  @since 5.1
*/
NONSHARABLE_CLASS( CPSetCallDivertingCreator ): public CBase
    {
    public:
        static MCallDiverting* CreateCallDivertingL( 
                                            MPsetDivertObserver& aObserver, 
                                            RMobilePhone& aPhone,
                                            CPsetCallDiverting* aDivert );
    private:
        
        /**
        * Check if KTelephonyLVFlagDualActivation is active.
        */
        static TBool IsDivertDualAffectFeatureEnabledL();
    };

#endif      // CSETCALLDIVERTINGCREATOR_H
            
// End of File
