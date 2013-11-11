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
void NokiaVolumeExtCallback(
    XANokiaVolumeExtItf caller,
    void * pContext,
    XAuint32 event,
    XAboolean eventBooleanData
);



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

TInt COpenMAXALTestModule::al_nokiavolumeextitf_SetVolumeLevel( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt volume(0);
    
    status = aItem.GetNextInt(volume);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->SetVolumeLevel(m_NokiaVolumeExtItf, volume);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_GetVolumeLevel( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAmillibel volume(0);
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->GetVolumeLevel(m_NokiaVolumeExtItf, &volume);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_GetMaxVolumeLevel( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAmillibel volume(0);
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->GetMaxVolumeLevel(m_NokiaVolumeExtItf, &volume);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_SetMute( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt mute(0);
    
    status = aItem.GetNextInt(mute);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->SetMute(m_NokiaVolumeExtItf, mute);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_GetMute( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAboolean mute(0);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->GetMute(m_NokiaVolumeExtItf, &mute);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_EnableStereoPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt value(0);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->EnableStereoPosition(m_NokiaVolumeExtItf, value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_IsEnabledStereoPosition( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAboolean value(0);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->IsEnabledStereoPosition(m_NokiaVolumeExtItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_SetStereoPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt value(0);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->SetStereoPosition(m_NokiaVolumeExtItf, value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_GetStereoPosition( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XApermille value(0);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->GetStereoPosition(m_NokiaVolumeExtItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }

TInt COpenMAXALTestModule::al_nokiavolumeextitf_RegisterVolumeCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->RegisterVolumeCallback(m_NokiaVolumeExtItf, NokiaVolumeExtCallback, this);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_SetCallbackEventsMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt value(0);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->SetCallbackEventsMask(m_NokiaVolumeExtItf, value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokiavolumeextitf_GetCallbackEventsMask( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 value(0);
    
    if(m_NokiaVolumeExtItf)
        {
        status = (*m_NokiaVolumeExtItf)->GetCallbackEventsMask(m_NokiaVolumeExtItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    

void NokiaVolumeExtCallback(
    XANokiaVolumeExtItf caller,
    void * pContext,
    XAuint32 event,
    XAboolean eventBooleanData
)
    {

    }

