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
void RecordItfCallback (
    XARecordItf caller,
    void * pContext,
    XAuint32 event
);
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

TInt COpenMAXALTestModule::al_recorditf_SetRecordState( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt state(0);
    XAresult res;

    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(state);
    if(!status)
        {
        if(m_RecordItf)
            {
            TAG_TIME_PROFILING_BEGIN;
            res = (*m_RecordItf)->SetRecordState(
                    (XARecordItf)itfPtr, state);              
            TAG_TIME_PROFILING_END;
            PRINT_TO_CONSOLE_TIME_DIFF;
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_SetRecordState Status[%d] RecordState[%d]"), status, state);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_GetRecordState( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAuint32 state;
    void* stateParam = (void*) &state;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, stateParam);
    RET_ERR_IF_ERR(status);
    
    if(m_RecordItf)
        {
        res = (*m_RecordItf)->GetRecordState(
                (XARecordItf)itfPtr, (XAuint32*)stateParam);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }   

    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_GetRecordState Status[%d] RecordState[%d]"), status, state);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_SetDurationLimit( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TUint duration(0);
    XAresult res;
    
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(duration);
    if(!status)
        {
        if(m_RecordItf)
            {
            res = (*m_RecordItf)->SetDurationLimit(
                    XARecordItf(itfPtr), duration);              
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }

    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_SetDurationLimit Status[%d] DurationLimit[%d]"), status, duration);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_GetPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAmillisecond pos;
    void* param = (void*)&pos;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param);
    RET_ERR_IF_ERR(status);
    
    if(m_RecordItf)
        {
        res = (*m_RecordItf)->GetPosition(
                XARecordItf(itfPtr), (XAmillisecond*)param);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }    

    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_GetPosition Status[%d] Position[%d]"), status, pos);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_RegisterCallback( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    

    if(m_RecordItf)
        {
        res = (*m_RecordItf)->RegisterCallback(
                XARecordItf(itfPtr), &RecordItfCallback, (void*)this);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }    

    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_RegisterCallback Status[%d] "), status);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_SetCallbackEventsMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TUint flags(0);
    XAresult res;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(flags);
    if(!status)
        {
        if(m_RecordItf)
            {
            res = (*m_RecordItf)->SetCallbackEventsMask(
                    XARecordItf(itfPtr), flags);              
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_SetCallbackEventsMask Status[%d] Mask[%d]"), status, flags);

    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_GetCallbackEventsMask( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAuint32 flags;
    void* param = (void*)&flags;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param);
    RET_ERR_IF_ERR(status);
    
    if(m_RecordItf)
        {
        res = (*m_RecordItf)->GetCallbackEventsMask(
                XARecordItf(itfPtr), (XAuint32*)param);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }   
    
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_GetCallbackEventsMask Status[%d] Mask[%d]"), status, flags);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_SetMarkerPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TUint pos(0);
    XAresult res;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(pos);
    if(!status)
        {
        if(m_RecordItf)
            {
            res = (*m_RecordItf)->SetMarkerPosition(
                    XARecordItf(itfPtr), pos);              
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_SetMarkerPosition Status[%d] Pos[%d]"), status, pos);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_ClearMarkerPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    if(m_RecordItf)
        {
        res = (*m_RecordItf)->ClearMarkerPosition(XARecordItf(itfPtr));              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
        
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_ClearMarkerPosition Status[%d]"), status);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_GetMarkerPosition( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAmillisecond pos;
    void* param = (void*)&pos;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param);
    RET_ERR_IF_ERR(status);
    
    if(m_RecordItf)
        {
        res = (*m_RecordItf)->GetMarkerPosition(
                XARecordItf(itfPtr), (XAmillisecond*)param);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_GetMarkerPosition Status[%d] Pos[%d]"), status, pos);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_SetPositionUpdatePeriod( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt updatePeriod(0);
    XAresult res;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(updatePeriod);
    if(!status)
        {
        if(m_RecordItf)
            {
            res = (*m_RecordItf)->SetPositionUpdatePeriod(
                    XARecordItf(itfPtr), updatePeriod);              
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_SetPositionUpdatePeriod Status[%d] Pos[%d]"), status, updatePeriod);
    return status;
    }

TInt COpenMAXALTestModule::al_recorditf_GetPositionUpdatePeriod( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAmillisecond pos;
    void* param = (void*)&pos;
    void* itfPtr = (void*) m_RecordItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param);
    RET_ERR_IF_ERR(status);
    
    if(m_RecordItf)
        {
        res = (*m_RecordItf)->GetPositionUpdatePeriod(
                XARecordItf(itfPtr), (XAmillisecond*)param);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }    
    
    iLog->Log(_L("COpenMAXALTestModule::al_recorditf_GetPositionUpdatePeriod Status[%d] Pos[%d]"), status, pos);
    return status;
    }

