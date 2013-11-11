/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definition of structure
*
*/


#ifndef C_LBTSTRATEGYCOMMON_H
#define C_LBTSTRATEGYCOMMON_H


//#include "lbtcommon.h"


/**
 * Structure that contains ID and distance information of the nearest 
 * trigger
 */
struct TNearestTrigger
    
    {
    
    TLbtTriggerId   id;
    
    TReal           distance;
    
    TInt            remRearmTime;
    
    TInt            minTimeRequired;
    
    };

    
    
#endif
