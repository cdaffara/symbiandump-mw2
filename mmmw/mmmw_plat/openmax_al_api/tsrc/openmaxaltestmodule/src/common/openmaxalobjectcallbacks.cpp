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
void ObjectCallback(XAObjectItf caller,
        const void * pContext,
        XAuint32 event,
        XAresult result,
        XAuint32 param,
        void * pInterface);

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

xaObjectCallback COpenMAXALTestModule::GetCallbackFunc(TInt object)
    {
    xaObjectCallback func(NULL);
    switch(object)
        {
        case EEngine:
            func = ObjectCallback;
            break;
        case EMediaRecorder:
            func = ObjectCallback;
            break;
        case EMediaPlayer:
            func = ObjectCallback;
            break;
        default:
            func = NULL;
            break;
        }
    return func;
    
    }

void COpenMAXALTestModule::HandleObjectCallback(XAObjectItf caller,XAuint32 event,
                        XAresult result,XAuint32 param, void * pInterface)
    {
    switch(event)
        {
        case XA_OBJECT_EVENT_RUNTIME_ERROR:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_RUNTIME_ERROR") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_ASYNC_TERMINATION:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_ASYNC_TERMINATION") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_RESOURCES_LOST:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_RESOURCES_LOST") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_RESOURCES_AVAILABLE:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_RESOURCES_AVAILABLE") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_ITF_CONTROL_TAKEN:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_ITF_CONTROL_TAKEN") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_ITF_CONTROL_RETURNED:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_ITF_CONTROL_RETURNED") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_ITF_PARAMETERS_CHANGED:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_ITF_PARAMETERS_CHANGED") );
            TestModuleIf().Event( event );

            }
            break;
        default:
            break;
        }
    }

void ObjectCallback(XAObjectItf caller,
        const void * pContext,
        XAuint32 event,
        XAresult result,
        XAuint32 param,
        void * pInterface)
    {
    if (pContext)
        {
        ((COpenMAXALTestModule*)pContext)->HandleObjectCallback(caller, event, result, param, pInterface);
        }
    }

