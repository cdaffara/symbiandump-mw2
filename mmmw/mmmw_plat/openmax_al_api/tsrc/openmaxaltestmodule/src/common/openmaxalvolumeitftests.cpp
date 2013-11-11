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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "openmaxaltestmodule.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;



// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

TInt COpenMAXALTestModule::al_volumeitf_SetVolumeLevel( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt volume(0);
    
    status = aItem.GetNextInt(volume);
    RET_ERR_IF_ERR(status);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->SetVolumeLevel(m_VolumeItf, volume);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_GetVolumeLevel( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAmillibel volume(0);
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->GetVolumeLevel(m_VolumeItf, &volume);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_GetMaxVolumeLevel( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAmillibel volume(0);
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->GetMaxVolumeLevel(m_VolumeItf, &volume);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_SetMute( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt mute(0);
    
    status = aItem.GetNextInt(mute);
    RET_ERR_IF_ERR(status);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->SetMute(m_VolumeItf, mute);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_GetMute( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAboolean mute(0);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->GetMute(m_VolumeItf, &mute);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_EnableStereoPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt value(0);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->EnableStereoPosition(m_VolumeItf, value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_IsEnabledStereoPosition( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAboolean value(0);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->IsEnabledStereoPosition(m_VolumeItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_SetStereoPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt value(0);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->SetStereoPosition(m_VolumeItf, value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_volumeitf_GetStereoPosition( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XApermille value(0);
    
    if(m_VolumeItf)
        {
        status = (*m_VolumeItf)->GetStereoPosition(m_VolumeItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    



