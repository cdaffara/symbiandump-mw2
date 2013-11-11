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
void PlayItfCallback (
    XAPlayItf caller,
    void * pContext,
    XAuint32 event);
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

TInt COpenMAXALTestModule::al_playitf_SetPlayState( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt state(0);
    status = aItem.GetNextInt(state);
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        TAG_TIME_PROFILING_BEGIN;
        status = (*m_PlayItf)->SetPlayState(
                m_PlayItf, state);              
        TAG_TIME_PROFILING_END;
        PRINT_TO_CONSOLE_TIME_DIFF;
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetPlayState( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint32 state;
    TInt expectedState;
    status = aItem.GetNextInt(expectedState);
    RET_ERR_IF_ERR(status);

    if (m_PlayItf)
        {
        status = (*m_PlayItf)->GetPlayState(
                m_PlayItf, &state);              
        if (!status)
            {
            if (state != expectedState)
                {
                status = KErrCompletion;
                }
            }
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetDurationNullParam( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    if (m_PlayItf)
        {
        status = (*m_PlayItf)->GetDuration(
                m_PlayItf, NULL);              
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetDuration( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAmillisecond duration;
    TInt expectedDurationLowLimit;
    TInt expectedDurationHighLimit;
    TBool expectedRangeDefined(EFalse);
    status = aItem.GetNextInt(expectedDurationLowLimit);
    if (status == KErrNone)
        {
        status = aItem.GetNextInt(expectedDurationHighLimit);
        if (status == KErrNone)
            {
            expectedRangeDefined = ETrue;
            }
        }
    if (m_PlayItf)
        {
        status = (*m_PlayItf)->GetDuration(
                m_PlayItf, &duration);              
        if (status == XA_RESULT_SUCCESS)
            {
            iLog->Log(_L("COpenMAXALTestModule::al_playitf_GetDuration:Duration[%u]"), duration);
            /* If duration does not fall between the range we expected fail the case */
            if (expectedRangeDefined &&
                    !((expectedDurationLowLimit <= duration) && (duration <= expectedDurationHighLimit)))
                {
                status = KErrCompletion;
                }
            }
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetPositionNullParam( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    if (m_PlayItf)
        {
        status = (*m_PlayItf)->GetPosition(
                m_PlayItf, NULL);              
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt expectedPos;
    TBool expectedPosDefined(EFalse);
    XAmillisecond position;
    status = aItem.GetNextInt(expectedPos);
    if (status == KErrNone)
        {
        expectedPosDefined = ETrue;
        }

    if (m_PlayItf)
        {
        status = (*m_PlayItf)->GetPosition(
                m_PlayItf, &position);
        if (status == XA_RESULT_SUCCESS)
            {
            if (expectedPosDefined &&
                (expectedPos != position))
                {
                status = KErrCompletion;
                }
            iLog->Log(_L("COpenMAXALTestModule::al_playitf_GetPosition:Position[%u]"), position);
            }
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_RegisterCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;

    if(m_PlayItf)
        {
        res = (*m_PlayItf)->RegisterCallback(
                m_PlayItf, &PlayItfCallback, (void*)this);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }    

    return status;
    }

TInt COpenMAXALTestModule::al_playitf_SetCallbackEventMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt eventMask;
    status = aItem.GetNextInt(eventMask);
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->SetCallbackEventsMask(
                m_PlayItf, eventMask);              
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetCallbackEventMaskNullParam( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    if(m_PlayItf)
        {
        status = (*m_PlayItf)->GetCallbackEventsMask(
                m_PlayItf, NULL);              
        }
    else
        {
        status = KErrNotFound;
        }    

    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetCallbackEventMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint32 eventMask;
    TInt expectedEventMask;
    status = aItem.GetNextInt(expectedEventMask);
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->GetCallbackEventsMask(
                m_PlayItf, &eventMask);              
        if (!status)
            {
            /* Fail if position is not expected*/
            if (eventMask != expectedEventMask)
                {
                status = KErrCompletion;
                }
            }
        }
    else
        {
        status = KErrNotFound;
        }    

    return status;
    }
TInt COpenMAXALTestModule::al_playitf_SetMarkerPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt pos(0);
    status = aItem.GetNextInt(pos);
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->SetMarkerPosition(
                m_PlayItf, (XAmillisecond)pos);              
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_ClearMarkerPosition( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->ClearMarkerPosition(
                m_PlayItf);              
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetMarkerPositionNullParam( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->GetMarkerPosition(
                m_PlayItf, NULL);
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetMarkerPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt expectedMarkerPos(0);
    status = aItem.GetNextInt(expectedMarkerPos);
    XAmillisecond markerPos;
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->GetMarkerPosition(
                m_PlayItf, &markerPos);
        if (!status && (markerPos != expectedMarkerPos))
            {
            status = KErrCompletion;
            }
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_SetPositionUpdatePeriod( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt pos(0);
    status = aItem.GetNextInt(pos);
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->SetPositionUpdatePeriod(
                m_PlayItf, (XAmillisecond)pos);
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetPositionUpdatePeriodNullParam( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->GetPositionUpdatePeriod(
                m_PlayItf, NULL);
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_playitf_GetPositionUpdatePeriod( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt expectedMarkerPos(0);
    status = aItem.GetNextInt(expectedMarkerPos);
    XAmillisecond markerPos;
    RET_ERR_IF_ERR(status);

    if(m_PlayItf)
        {
        status = (*m_PlayItf)->GetPositionUpdatePeriod(
                m_PlayItf, &markerPos);
        if (!status && (markerPos != expectedMarkerPos))
            {
            status = KErrCompletion;
            }
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

void COpenMAXALTestModule::HandlePlayItfCallback( XAPlayItf /*caller*/, XAuint32 event )
    {
    switch (event)
        {
        case XA_PLAYEVENT_HEADATEND:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PLAYEVENT_HEADATEND") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_PLAYEVENT_HEADATMARKER:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PLAYEVENT_HEADATMARKER") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_OBJECT_EVENT_RESOURCES_LOST:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_OBJECT_EVENT_RESOURCES_LOST") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_PLAYEVENT_HEADATNEWPOS:
            {
            iLog->Log(_L("COpenMAXALTestModule::HandlePlayItfCallback:XA_PLAYEVENT_HEADATNEWPOS"));
            }
            break;
        case XA_PLAYEVENT_HEADMOVING:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PLAYEVENT_HEADMOVING") );
            TestModuleIf().Event( event );
            }
            break;
        case XA_PLAYEVENT_HEADSTALLED:
            {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PLAYEVENT_HEADSTALLED") );
            TestModuleIf().Event( event );
            }
            break;
        default:
            break;
        }
    }

void PlayItfCallback (
    XAPlayItf caller,
    void * pContext,
    XAuint32 event)
    {
    if (pContext)
        {
        ((COpenMAXALTestModule*)pContext)->HandlePlayItfCallback(caller, event);
        }
    }
