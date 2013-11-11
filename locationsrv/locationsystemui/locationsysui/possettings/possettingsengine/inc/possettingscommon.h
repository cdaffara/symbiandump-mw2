/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This header exposes common data structues and enums.
*
*/

#ifndef POSSETTINGSCOMMON_H
#define POSSETTINGSCOMMON_H

#include <e32cmn.h>

/**
 * This enum defines type of position technology.
 */
enum PositionTechnology
    {
    /**
     * Bit mask used to specify GPS technology. 
     */
    PositionTechnologyGps = 0x01,
    
    /**
     * Bit mask used to specify network technology.
     */
    PositionTechnologyNetwork = 0x02,
    
    };

/**
 * This enum defines different state.
 */
enum State
    {
    StateEnable,
    
    StateDisable
    };

/**
 * PSY module information
 */
struct PsyModuleInfo
    {
    /** 
     * Module id of Psy
     */
    TUid mPsyModuleId;
    
    /**
     * Position technology.
     */
    PositionTechnology mPosTech;
    
    /**
     * State of Psy
     */
    State mState;
    };


#endif /* PosSettingsCOMMON_H */
