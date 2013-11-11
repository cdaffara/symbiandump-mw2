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

TInt COpenMAXALTestModule::al_seekitf_SetPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt seekMode(0);
    TInt position(0);

    status = aItem.GetNextInt(position);
    RET_ERR_IF_ERR(status);
        
    status = aItem.GetNextInt(seekMode);
    RET_ERR_IF_ERR(status);

    if(m_SeekItf)
        {
        TAG_TIME_PROFILING_BEGIN;
        status = (*m_SeekItf)->SetPosition(
                m_SeekItf, position, seekMode);              
        TAG_TIME_PROFILING_END;
        PRINT_TO_CONSOLE_TIME_DIFF;
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_seekitf_SetLoop( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt loopEnable(0);
    TInt startPos(0);
    TInt endPos(0);
    
    status = aItem.GetNextInt(loopEnable);
    RET_ERR_IF_ERR(status);
        
    status = aItem.GetNextInt(startPos);
    RET_ERR_IF_ERR(status);    
    
    status = aItem.GetNextInt(endPos);
    RET_ERR_IF_ERR(status);

    if (m_SeekItf)
        {
        status = (*m_SeekItf)->SetLoop(
                m_SeekItf, loopEnable, startPos, endPos );              
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_seekitf_GetLoop( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAboolean loopEnable;
    XAuint32 startPos(0);
    XAuint32 endPos(0);
    
    if (m_SeekItf)
        {
        status = (*m_SeekItf)->GetLoop(
                m_SeekItf, &loopEnable, &startPos, &endPos);
        iLog->Log(_L("al_seekitf_GetLoop loopEnable[%d]"),loopEnable);
        iLog->Log(_L("al_seekitf_GetLoop startPos[%d]"),startPos);
        iLog->Log(_L("al_seekitf_GetLoop endPos[%d]"),endPos);
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

