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
void PrefetchStatusItfCallback (
    XAPrefetchStatusItf caller,
    void * pContext,
    XAuint32 event);

void PrefetchStatusItfTestEventCallback (
    XAPrefetchStatusItf caller,
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

TInt COpenMAXALTestModule::al_prefetchstatusitf_GetFillLevel( CStifItemParser& aItem )
{
    TInt status(KErrGeneral);
    XApermille fL(0);
    
    TInt fillLevel = 0;
    status = aItem.GetNextInt(fillLevel);

    if((status==KErrNone) && m_PrefetchStatusItf)
    {
        XAresult res = (*m_PrefetchStatusItf)->GetFillLevel(m_PrefetchStatusItf, &fL);              
        status = MapErr(res);
    }
    
    if((m_PrefetchStatusItf == NULL) || 
       (status != KErrNone) || 
       (fL != fillLevel))  
    {
        status = KErrNotFound;
    }    
    
    return status;
}

TInt COpenMAXALTestModule::al_prefetchstatusitf_GetPrefetchStatus( CStifItemParser& aItem )
{
    TInt status(KErrGeneral);
    XAuint32 pfs(0);
    
    TInt prefetchStatus = 0;
    status = aItem.GetNextInt(prefetchStatus);
    
    if((status==KErrNone) && m_PrefetchStatusItf)
    {
        XAresult res = (*m_PrefetchStatusItf)->GetPrefetchStatus(m_PrefetchStatusItf, &pfs);              
        status = MapErr(res);
    }
    
    if( (m_PrefetchStatusItf == NULL) || 
        (status != KErrNone) || 
        (pfs != prefetchStatus))  
    {
        status = KErrNotFound;
    }    
    
    return status;
}

TInt COpenMAXALTestModule::al_prefetchstatusitf_RegisterCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;

    if(m_PrefetchStatusItf)
        {
        res = (*m_PrefetchStatusItf)->RegisterCallback(
                m_PrefetchStatusItf, &PrefetchStatusItfCallback, (void*)this);              
        status = MapErr(res);
        }
    else
        {
        status = KErrNotFound;
        }    

    return status;
    }

TInt COpenMAXALTestModule::al_prefetchstatusitf_RegisterTestEventCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;

    if(m_PrefetchStatusItf)
        {
        res = (*m_PrefetchStatusItf)->RegisterCallback(
                m_PrefetchStatusItf, &PrefetchStatusItfTestEventCallback, (void*)this);              
        status = MapErr(res);
        }
    else
        {
        status = KErrNotFound;
        }    

    return status;
    }

TInt COpenMAXALTestModule::al_prefetchstatusitf_SetCallbackEventMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt eventMask;
    status = aItem.GetNextInt(eventMask);
    RET_ERR_IF_ERR(status);

    if(m_PrefetchStatusItf)
        {
        status = (*m_PrefetchStatusItf)->SetCallbackEventsMask(
                m_PrefetchStatusItf, eventMask);              
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_prefetchstatusitf_GetCallbackEventMaskNullParam( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    if(m_PrefetchStatusItf)
        {
        status = (*m_PrefetchStatusItf)->GetCallbackEventsMask(
                m_PrefetchStatusItf, NULL);              
        }
    else
        {
        status = KErrNotFound;
        }    

    return status;
    }

TInt COpenMAXALTestModule::al_prefetchstatusitf_GetCallbackEventMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint32 eventMask;
    TInt expectedEventMask;
    status = aItem.GetNextInt(expectedEventMask);
    RET_ERR_IF_ERR(status);

    if(m_PrefetchStatusItf)
        {
        status = (*m_PrefetchStatusItf)->GetCallbackEventsMask(
                m_PrefetchStatusItf, &eventMask);              
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

TInt COpenMAXALTestModule::al_prefetchstatusitf_SetFillUpdatePeriod( CStifItemParser& aItem )
{
    TInt status(KErrNone);
    TInt updatePeriod;
    status = aItem.GetNextInt(updatePeriod);
    RET_ERR_IF_ERR(status);

    if(m_PrefetchStatusItf)
    {
        status = (*m_PrefetchStatusItf)->SetFillUpdatePeriod(
                m_PrefetchStatusItf, (XAmillisecond)updatePeriod);
        
        //used for testing fill events
        iFillUpdateIncrement = updatePeriod;
    }
    else
    {
        status = KErrNotFound;
    }    
    return status;
}

TInt COpenMAXALTestModule::al_prefetchstatusitf_GetFillUpdatePeriodNullParam( CStifItemParser& /*aItem*/ )
{
    TInt status(KErrNone);

    if(m_PrefetchStatusItf)
    {
        status = (*m_PrefetchStatusItf)->GetFillUpdatePeriod(
                m_PrefetchStatusItf, NULL);
    }
    else
    {
        status = KErrNotFound;
    }    
    return status;
}

TInt COpenMAXALTestModule::al_prefetchstatusitf_GetFillUpdatePeriod( CStifItemParser& aItem )
{
    TInt status(KErrNone);
    TInt expectedUpdatePeriod(0);
    status = aItem.GetNextInt(expectedUpdatePeriod);
    XApermille updatePeriod;
    RET_ERR_IF_ERR(status);

    if(m_PrefetchStatusItf)
    {
        status = (*m_PrefetchStatusItf)->GetFillUpdatePeriod(
                m_PrefetchStatusItf, &updatePeriod);
        if (!status && (updatePeriod != expectedUpdatePeriod))
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

TInt COpenMAXALTestModule::al_prefetchstatusitf_TestEvent( CStifItemParser& /*aItem*/ )
{
    iStatusEventCount = 0;
    iFillEventCount = 0;
    iLastFillIncrement = 0;
    iPrefetchEventTesting = ETrue;
    iPrefetchEventError = EFalse;
    
    return KErrNone;
}

void COpenMAXALTestModule::HandlePrefetchStatusItfCallback( XAPrefetchStatusItf /*caller*/, XAuint32 event )
{
    switch (event)
    {
        case XA_PREFETCHEVENT_STATUSCHANGE:
        {
            TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PREFETCHEVENT_STATUSCHANGE") );
            TestModuleIf().Event( event );
        }
        break;
        case XA_PREFETCHEVENT_FILLLEVELCHANGE:
        {
                TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PREFETCHEVENT_FILLLEVELCHANGE") );
                TestModuleIf().Event( event );
        }
        break;
       default:
            break;
    }
}

void COpenMAXALTestModule::HandlePrefetchStatusItfTestEventCallback( XAPrefetchStatusItf /*caller*/, XAuint32 event )
{
    switch (event)
    {
        case XA_PREFETCHEVENT_STATUSCHANGE:
        {
            ++iStatusEventCount;                
            if(iStatusEventCount==2)
            {
                XAuint32 pfs(0);
                XAresult res = (*m_PrefetchStatusItf)->GetPrefetchStatus(m_PrefetchStatusItf, &pfs);
                if((res==XA_RESULT_SUCCESS) && (pfs == XA_PREFETCHSTATUS_SUFFICIENTDATA))
                {
                    TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PREFETCHEVENT_STATUSCHANGE") );
                    TestModuleIf().Event( event );
                }
            }
        }
        break;
        case XA_PREFETCHEVENT_FILLLEVELCHANGE:
        {
            if(iPrefetchEventError)
            {
                return;
            }

            XApermille fL = 0;
            XAresult res = (*m_PrefetchStatusItf)->GetFillLevel(m_PrefetchStatusItf, &fL);
            
            if((res != XA_RESULT_SUCCESS) ||
               ((iFillEventCount>0) && (fL<(iLastFillIncrement + iFillUpdateIncrement))))
            {
                iPrefetchEventError = ETrue;
            }
            else
            {
                iLastFillIncrement = fL - (fL % iFillUpdateIncrement);
                
                if(fL == 1000)
                {
                    TEventIf event( TEventIf::ESetEvent, _L("Event_XA_PREFETCHEVENT_FILLLEVELCHANGE") );
                    TestModuleIf().Event( event );
                }
                
                ++iFillEventCount;
            }
        }
        break;
       default:
            break;
    }
}


void PrefetchStatusItfCallback (
    XAPrefetchStatusItf caller,
    void * pContext,
    XAuint32 event)
    {
    if (pContext)
        {
        ((COpenMAXALTestModule*)pContext)->HandlePrefetchStatusItfCallback(caller, event);
        }
    }

void PrefetchStatusItfTestEventCallback (
    XAPrefetchStatusItf caller,
    void * pContext,
    XAuint32 event)
    {
    if (pContext)
        {
        ((COpenMAXALTestModule*)pContext)->HandlePrefetchStatusItfTestEventCallback(caller, event);
        }
    }
