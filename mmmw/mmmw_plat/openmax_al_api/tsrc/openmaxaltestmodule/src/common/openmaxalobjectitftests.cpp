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

TInt COpenMAXALTestModule::al_objitf_Realize( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt object(0);
    TInt async;
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            status = aItem.GetNextInt(async);
            if(!status)
                {
                if (object == XA_OBJECTID_MEDIAPLAYER)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    res = (*itf)->Realize(itf, async);
                    TAG_TIME_PROFILING_END;
                    PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                else
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    res = (*itf)->Realize(itf, async);
                    TAG_TIME_PROFILING_END;
                    PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                status = res;
                }
            else
                {
                status = KErrGeneral;
                }
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_Resume( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt object(0);
    TInt async;
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            status = aItem.GetNextInt(async);
            if(!status)
                {
                res = (*itf)->Resume(itf, async);
                status = res;
                }
            else
                {
                status = KErrGeneral;
                }
            }
        }
    return status;    }

TInt COpenMAXALTestModule::al_objitf_GetState( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt object(0);
    XAuint32 state;
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            res = (*itf)->GetState(itf, &state);
            status = res;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_GetInterface( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt object(0);
    TInt interface(0);
    XAInterfaceID id;
    void* retid(NULL);
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                id = MapInterface(interface);
                res = (*itf)->GetInterface(itf, id, &retid);
                status = res;
                if(!status)
                    {
                    status = StoreInterface(interface, retid);
                    }
                }
            else
                {
                status = KErrGeneral;
                }
            }
        else
            {
            status = KErrGeneral;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_RegisterCallback( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt object(0);
    XAresult res;
    xaObjectCallback func;
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            func = GetCallbackFunc(object);
            if(func)
                {
                res = (*itf)->RegisterCallback(itf, func, this);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }
            }
        else
            {
            status = KErrNotFound;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_AbortAsyncOperation( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt object(0);
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            (*itf)->AbortAsyncOperation(itf);
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_Destroy( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt object(0);
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            if (object == XA_OBJECTID_MEDIAPLAYER)
                {
                TAG_TIME_PROFILING_BEGIN;
                (*itf)->Destroy(itf);
                TAG_TIME_PROFILING_END;
                PRINT_TO_CONSOLE_TIME_DIFF;
                }
            else
                {
                TAG_TIME_PROFILING_BEGIN;
                (*itf)->Destroy(itf);
                TAG_TIME_PROFILING_END;
                PRINT_TO_CONSOLE_TIME_DIFF;
                }
            }
        else
            {
            status = KErrNotFound;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_SetPriority( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt object(0);
    TInt priority(0);
    TInt preemptable(0);
    
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            status = aItem.GetNextInt(priority);
            if(status)
                {
                priority = 0;
                preemptable = 0;
                }
            else
                {
                status = aItem.GetNextInt(preemptable);
                if(status)
                    {
                    preemptable = 0;
                    }
                }
            (*itf)->SetPriority(itf,priority,preemptable);
            }
        else
            {
            status = KErrNotFound;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_GetPriority( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt object(0);
    XAint32 priority(0);
    XAboolean preemptable(0);
    status = aItem.GetNextInt(object);
    if(!status)
        {
        XAObjectItf itf = GetObject(object);
        if(itf != NULL)
            {
            res = (*itf)->GetPriority(itf, &priority, &preemptable);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_objitf_SetLossOfControlInterfaces( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt object(0);
    TInt enabled(0);
    TInt numInterfaces(0);    
    XAresult res;
    
    status = aItem.GetNextInt(object);
    
    if(!status)
        {
        /* Initialize arrays required[] and iidArray[] */
        for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
            {
            iidArray[i] = XA_IID_NULL;
            }
    
        status = aItem.GetNextInt(numInterfaces);
        
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                iidArray[j] = MapInterface(interface);
                numInterfaces = j++;
                }
            else
                {
                status = KErrGeneral;
                break;
                }
            }
    
        if(!status)
            {
            status = aItem.GetNextInt(enabled);
            if(!status)
                {
                XAObjectItf itf = GetObject(object);
                 if(itf != NULL)
                     {
                     res = (*itf)->SetLossOfControlInterfaces(itf, numInterfaces, iidArray, enabled);
                     status = res;
                     }
                 else
                     {
                     status = KErrNotFound;
                     }
                }
            }
    
        }
    
    return status;

    }




