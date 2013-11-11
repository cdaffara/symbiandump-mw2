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

TInt COpenMAXALTestModule::al_videoppitf_SetRotation( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt rotation;
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    status = aItem.GetNextInt(rotation);
    RET_ERR_IF_ERR(status);

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->SetRotation((XAVideoPostProcessingItf)selfPtr, rotation);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_videoppitf_IsArbitraryRotationSupported( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAboolean supported;
    void* supportedParam = (void*) &supported;
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    status = CheckForNullParam(aItem, supportedParam);
    RET_ERR_IF_ERR(status);

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->IsArbitraryRotationSupported((XAVideoPostProcessingItf)selfPtr, (XAboolean*)supportedParam);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        iLog->Log(_L("al_videoppitf_IsArbitraryRotationSupported Supported =%d"),supported);
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_videoppitf_SetScaleOptions( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt options(0);
    TInt backGColor(0);
    TInt hints(0);
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    status = aItem.GetNextInt(options);
    RET_ERR_IF_ERR(status);

    status = aItem.GetNextInt(backGColor);
    RET_ERR_IF_ERR(status);

    status = aItem.GetNextInt(hints);
    RET_ERR_IF_ERR(status);

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->SetScaleOptions((XAVideoPostProcessingItf)selfPtr, options, backGColor, hints);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_videoppitf_SetSourceRectangle( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XARectangle rect;
    TInt top;
    TInt left ;
    TInt width;
    TInt height;
    void* rectParam = (void*) &rect;
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    status = CheckForNull(aItem, rectParam);
    RET_ERR_IF_ERR(status);

    if ((XARectangle*)rectParam != NULL)
        {
        status = aItem.GetNextInt(top);
        RET_ERR_IF_ERR(status);
        rect.top = top;

        status = aItem.GetNextInt(left);
        RET_ERR_IF_ERR(status);
        rect.left = left;

        status = aItem.GetNextInt(width);
        RET_ERR_IF_ERR(status);
        rect.width = width;

        status = aItem.GetNextInt(height);
        RET_ERR_IF_ERR(status);
        rect.height = height;
        }

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->SetSourceRectangle((XAVideoPostProcessingItf)selfPtr, (XARectangle*)rectParam);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_videoppitf_SetDestinationRectangle( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XARectangle rect;
    TInt top;
    TInt left ;
    TInt width;
    TInt height;

    void* rectParam = (void*) &rect;
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    status = CheckForNull(aItem, rectParam);
    RET_ERR_IF_ERR(status);

    if ((XARectangle*)rectParam != NULL)
        {
        status = aItem.GetNextInt(top);
        RET_ERR_IF_ERR(status);
        rect.top = top;

        status = aItem.GetNextInt(left);
        RET_ERR_IF_ERR(status);
        rect.left = left;

        status = aItem.GetNextInt(width);
        RET_ERR_IF_ERR(status);
        rect.width = width;

        status = aItem.GetNextInt(height);
        RET_ERR_IF_ERR(status);
        rect.height = height;
        }

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->SetDestinationRectangle((XAVideoPostProcessingItf)selfPtr, (XARectangle*)rectParam);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_videoppitf_SetMirror( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt mirror;
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    status = aItem.GetNextInt(mirror);
    RET_ERR_IF_ERR(status);

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->SetMirror((XAVideoPostProcessingItf)selfPtr, mirror);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_videoppitf_Commit( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    void* selfPtr = (void*) m_VideoPP;

    status = CheckForNull(aItem, selfPtr);
    RET_ERR_IF_ERR(status);

    if(m_VideoPP)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_VideoPP)->Commit((XAVideoPostProcessingItf)selfPtr);
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }



