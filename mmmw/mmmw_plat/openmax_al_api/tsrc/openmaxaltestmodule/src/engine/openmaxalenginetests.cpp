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

TInt COpenMAXALTestModule::al_createEngine( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt numEngineOption(0);
    TInt numInterfaces(0);

    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_OPTIONS; i++)
        {
        EngineOption[i].feature = 0;
        EngineOption[i].data = 0;
        }
    
    status = aItem.GetNextInt(numEngineOption);
    
    if(!status)
        {
        for(TInt i = 0; i < numEngineOption; i++)
            {
            TInt feature(0);
            TInt data(0);
            status = aItem.GetNextInt(feature);
            if(!status)
                {
                status = aItem.GetNextInt(data);
                if(!status)
                    {
                    EngineOption[i].data = data;
                    }
                else
                    {
                    numEngineOption = 0;
                    break;
                    }
                EngineOption[i].feature = feature;
                }
            else
                {
                break;
                }
            
            }
        }
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    if(!status)
        {
        status = aItem.GetNextInt(numInterfaces);
        
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    XAresult xa_res = xaCreateEngine(&m_EOEngine, numEngineOption, EngineOption, numInterfaces, iidArray, required);
    status = MapErr(xa_res);    
    return status;
    }
    
TInt COpenMAXALTestModule::al_queryNumSupportedEngineInterfaces( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 numInterfaces(0);
    XAresult xa_res = xaQueryNumSupportedEngineInterfaces(&numInterfaces);
    status = MapErr(xa_res);    
    iLog->Log(_L("COpenMAXALTestModule::al_queryNumSupportedEngineInterfaces NumInterfaces[%d] status[%d]"),numInterfaces,status);
    return status;
    }

TInt COpenMAXALTestModule::al_querySupportedEngineInterfaces( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt index(0);
    status = aItem.GetNextInt(index);
    if(!status)
        {
        XAresult xa_res = xaQuerySupportedEngineInterfaces(index, iidArray);
        status = MapErr(xa_res);    
        }
    return status;
    }

