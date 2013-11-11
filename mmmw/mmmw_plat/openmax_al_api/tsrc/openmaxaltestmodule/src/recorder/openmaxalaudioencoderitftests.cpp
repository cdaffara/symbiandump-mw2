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
#include <strings.h>
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

TInt COpenMAXALTestModule::al_audioencoderitf_SetEncoderSettings( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAAudioEncoderSettings setting;
    TUint value(0);
    TInt i(0);
    TInt numItems = -1;
    void* param = (void*)&setting;
    void* itfPtr = (void*) m_AudEncItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNull(aItem, param);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(numItems);
    
    while((numItems != 0) && (status == KErrNone))
        {
        status = aItem.GetNextInt(value);
        if(!status)
            {
            switch(i)
                {
                case 0:
                    setting.encoderId = value;
                    break;
                case 1:
                    setting.channelsIn = value;
                    break;
                case 2:
                    setting.channelsOut = value;
                    break;
                case 3:
                    setting.sampleRate = value;
                    break;
                case 4:
                    setting.bitRate = value;
                    break;
                case 5:
                    setting.bitsPerSample = value;
                    break;
                case 6:
                    setting.rateControl = value;
                    break;
                case 7:
                    setting.profileSetting = value;
                    break;
                case 8:
                    setting.levelSetting = value;
                    break;
                case 9:
                    setting.channelMode = value;
                    break;
                case 10:
                    setting.streamFormat = value;
                    break;
                case 11:
                    setting.encodeOptions = value;
                    break;
                case 12:
                    setting.blockAlignment = value;
                    break;
                default:
                    break;
                }
            }
        i++;
        numItems--;
        }

    if(m_AudEncItf)
        {
        res = (*m_AudEncItf)->SetEncoderSettings(
                XAAudioEncoderItf(itfPtr), (XAAudioEncoderSettings*)param);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioencoderitf_GetEncoderSettings( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt status_check(KErrNone);
    XAresult res;
    XAAudioEncoderSettings setting;
    TUint value(0);
    TInt i(0);
    TInt numItems;
    TPtrC check;
    
    void* param = (void*)&setting;
    void* itfPtr = (void*) m_AudEncItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNull(aItem, param);
    RET_ERR_IF_ERR(status);
    
    if(m_AudEncItf)
        {
        res = (*m_AudEncItf)->GetEncoderSettings(
                XAAudioEncoderItf(itfPtr), (XAAudioEncoderSettings*)param);
        iLog->Log(_L("GetEncoderSettings"));
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.encoderId);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.channelsIn);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.channelsOut);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.sampleRate);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.bitRate);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.bitsPerSample);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.rateControl);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.profileSetting);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.levelSetting);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.channelMode);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.streamFormat);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.encodeOptions);
        iLog->Log(_L("GetEncoderSettings value[%d]"),setting.blockAlignment);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    status_check = aItem.GetNextString(check);
    
    if(status_check == KErrNone)
        {    
        HBufC8* buf = HBufC8::NewL(check.Length()+1);
        CleanupStack::PushL(buf);
        TPtr8 des = buf->Des();
        des.Copy(check);
        if(strcasecmp((const char*)des.PtrZ(),"check") == NULL)
        { 
        status = aItem.GetNextInt(numItems);
        while((numItems != 0) && (status == KErrNone))
                {
                status = aItem.GetNextInt(value);
                if(!status)
                    {
                    switch(i)
                        {
                        case 0:
                            if(!(setting.encoderId == value))
                                status_check = KErrGeneral;
                            break;
                            
                        case 1:
                            if(!(setting.channelsIn == value))
                                status_check = KErrGeneral;
                            break;
                        case 2:
                            if(!(setting.channelsOut == value))
                                status_check = KErrGeneral;
                            break;
                        case 3:
                            if(!(setting.sampleRate == value))
                                status_check = KErrGeneral;
                            break;
                        case 4:
                            if(!(setting.bitRate == value))
                            status_check = KErrGeneral;
                            break;
                        case 5:
                            if(!(setting.bitsPerSample == value))
                            status_check = KErrGeneral;
                            break;
                        case 6:
                            if(!(setting.rateControl == value))
                            status_check = KErrGeneral;
                            break;
                        case 7:
                            if(!(setting.profileSetting == value))
                            status_check = KErrGeneral;
                            break;
                        case 8:
                            if(!(setting.levelSetting == value))
                            status_check = KErrGeneral;
                            break;
                        case 9:
                            if(!(setting.channelMode == value))
                            status_check = KErrGeneral;
                            break;
                        case 10:
                            if(!(setting.streamFormat == value))
                            status_check = KErrGeneral;
                            break;
                        case 11:
                            if(!(setting.encodeOptions == value))
                            status_check = KErrGeneral;
                            break;
                        case 12:
                            if(!(setting.blockAlignment == value))
                            status_check = KErrGeneral;
                            break;
                        default:
                            break;
                        }
                    }
                i++;
                numItems--;
                }
        
            }
        CleanupStack::PopAndDestroy(buf);
        }
    if(status_check == KErrNotFound)
        status_check = KErrNone;

    return status | status_check;
    }
