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
#include <StifTestEventInterface.h>
#include "openmaxaltestmodule.h"
#include "profileutilmacro.h"

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


TInt COpenMAXALTestModule::al_playbackrateitf_SetRate( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAint16 rate;
    TInt playbackrate(0);
    
    status = aItem.GetNextInt(playbackrate);
    if(!status)
        {
        rate = playbackrate;
        if(m_PlaybackRateItf)
            {
            status = (*m_PlaybackRateItf)->SetRate(m_PlaybackRateItf, rate);
            }
        else
            {
            status = KErrNotFound;
            }
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_playbackrateitf_GetRate( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAint16 rate;
    
    if(m_PlaybackRateItf)
        {
        status = (*m_PlaybackRateItf)->GetRate(m_PlaybackRateItf, &rate);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_playbackrateitf_SetPropertyConstraints( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt props(0);
    XAuint32 properties;
    
    status = aItem.GetNextInt(props);
    if(!status)
        {
        properties = props;
        if(m_PlaybackRateItf)
            {
            status = (*m_PlaybackRateItf)->SetPropertyConstraints(m_PlaybackRateItf, properties);
            }
        else
            {
            status = KErrNotFound;
            }
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_playbackrateitf_GetProperties( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 properties;
    
    if(m_PlaybackRateItf)
        {
        status = (*m_PlaybackRateItf)->GetProperties(m_PlaybackRateItf, &properties);
        }
    else
        {
        status = KErrNotFound;
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_playbackrateitf_GetCapabilitiesOfRate( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAint16 rate;
    TInt playbackrate(0);
    XAuint32 properties;
    
    status = aItem.GetNextInt(playbackrate);
    if(!status)
        {
        rate = playbackrate;
        if(m_PlaybackRateItf)
            {
            status = (*m_PlaybackRateItf)->GetCapabilitiesOfRate(m_PlaybackRateItf, rate, &properties);
            }
        else
            {
            status = KErrNotFound;
            }
        }
    
    return status;
    }
    
TInt COpenMAXALTestModule::al_playbackrateitf_GetRateRange( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint8 index;
    TInt indexIn(0);
    XApermille min;
    XApermille max;
    XApermille step;
    XAuint32 properties;
    
    status = aItem.GetNextInt(indexIn);
    if(!status)
        {
        index = indexIn;
        if(m_PlaybackRateItf)
            {
            status = (*m_PlaybackRateItf)->GetRateRange(m_PlaybackRateItf, index, &min, &max, &step, &properties);
            }
        else
            {
            status = KErrNotFound;
            }
        }
    
    return status;
    }
    
