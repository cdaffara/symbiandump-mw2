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
void DynamicInterfaceManagementCallback (
    XADynamicInterfaceManagementItf caller,
    void * pContext,
    XAuint32 event,
    XAresult result,
    const XAInterfaceID iid
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

TInt COpenMAXALTestModule::al_dimitf_AddInterface( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt async(0);
    TInt interface(0);
    XAInterfaceID id(NULL);


    status = aItem.GetNextInt(interface);
    if(!status)
        {
        id = MapInterface(interface);
        }
    else
        {
        status = KErrGeneral;
        }

    status = aItem.GetNextInt(async);
    if(!status)
        {
        if(m_DIMItf)
            {
            res = (*m_DIMItf)->AddInterface(m_DIMItf,id,async);
            status = res;
            }
        }
    else
        {
        status = KErrGeneral;
        }
    return status;    
    }

TInt COpenMAXALTestModule::al_dimitf_RemoveInterface( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt interface(0);
    XAInterfaceID id(NULL);

    status = aItem.GetNextInt(interface);
    if(!status)
        {
        id = MapInterface(interface);
        status = StoreInterface(interface, NULL);
        }
    else
        {
        status = KErrGeneral;
        }

    if(m_DIMItf)
        {
        res = (*m_DIMItf)->RemoveInterface(m_DIMItf,id);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;            
    }

TInt COpenMAXALTestModule::al_dimitf_ResumeInterface( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt async(0);
    TInt interface(0);
    XAInterfaceID id(NULL);


    status = aItem.GetNextInt(interface);
    if(!status)
        {
        id = MapInterface(interface);
        }
    else
        {
        status = KErrGeneral;
        }

    status = aItem.GetNextInt(async);
    if(!status)
        {
        if(m_DIMItf)
            {
            res = (*m_DIMItf)->ResumeInterface(m_DIMItf,id,async);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    else
        {
        status = KErrGeneral;
        }
    return status;        
    }

TInt COpenMAXALTestModule::al_dimitf_RegisterCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    if(m_DIMItf)
        {
        res = (*m_DIMItf)->RegisterCallback(m_DIMItf,DynamicInterfaceManagementCallback,(void*)this);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;    
    }



