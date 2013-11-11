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

void NokiaLinearVolumeCallback(
    XANokiaLinearVolumeItf caller,
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

TInt COpenMAXALTestModule::al_nokialinearvolumeitf_SetVolumeLevel( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt volume(0);
    XAuint32 alvol;
    status = aItem.GetNextInt(volume);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaLinearVolumeItf)
        {
        alvol = volume;
        status = (*m_NokiaLinearVolumeItf)->SetVolumeLevel(m_NokiaLinearVolumeItf, &alvol);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokialinearvolumeitf_SetGetVolumeLevel( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt volume(0);
    XAuint32 alvol;
    XAuint32 getVol(0);
    status = aItem.GetNextInt(volume);
    RET_ERR_IF_ERR(status);
    XAuint32 expectedVol = volume;
    
    if(m_NokiaLinearVolumeItf)
        {
        alvol = volume;
        status = (*m_NokiaLinearVolumeItf)->SetVolumeLevel(m_NokiaLinearVolumeItf, &alvol);
        if (status != KErrNone)
        {
        	return status;
        }
        status = (*m_NokiaLinearVolumeItf)->GetVolumeLevel(m_NokiaLinearVolumeItf, &getVol);
        if (status != KErrNone)
        {
        	return status;
        } 
        if (getVol != expectedVol) 
        	status = KErrCompletion;     
        }
    else
        {
        return KErrNotFound;
        }
    
    return status;
    }    
    
TInt COpenMAXALTestModule::al_nokialinearvolumeitf_GetVolumeLevel( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 volume(0);
    if(m_NokiaLinearVolumeItf)
        {
        status = (*m_NokiaLinearVolumeItf)->GetVolumeLevel(m_NokiaLinearVolumeItf, &volume);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokialinearvolumeitf_GetStepCount( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 value(0);
    if(m_NokiaLinearVolumeItf)
        {
        status = (*m_NokiaLinearVolumeItf)->GetStepCount(m_NokiaLinearVolumeItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokialinearvolumeitf_RegisterVolumeCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    if(m_NokiaLinearVolumeItf)
        {
        status = (*m_NokiaLinearVolumeItf)->RegisterVolumeCallback(m_NokiaLinearVolumeItf, NokiaLinearVolumeCallback, this);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokialinearvolumeitf_SetCallbackEventsMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt value(0);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    
    if(m_NokiaLinearVolumeItf)
        {
        status = (*m_NokiaLinearVolumeItf)->SetCallbackEventsMask(m_NokiaLinearVolumeItf, value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_nokialinearvolumeitf_GetCallbackEventsMask( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 value(0);
    
    if(m_NokiaLinearVolumeItf)
        {
        status = (*m_NokiaLinearVolumeItf)->GetCallbackEventsMask(m_NokiaLinearVolumeItf, &value);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
void NokiaLinearVolumeCallback(
    XANokiaLinearVolumeItf caller,
    void * pContext,
    XAuint32 event,
    XAboolean eventBooleanData
)
    {
    
    }
