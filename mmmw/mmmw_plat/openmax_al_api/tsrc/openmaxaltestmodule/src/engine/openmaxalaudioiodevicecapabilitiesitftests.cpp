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
void AvailAudioInputsChangedCallback(
        XAAudioIODeviceCapabilitiesItf caller,
        void * pContext,
        XAuint32 deviceID,
        XAint32 numInputs,
        XAboolean isNew
    );

void AvailAudioOutputsChangedCallback(
        XAAudioIODeviceCapabilitiesItf caller,
        void * pContext,
        XAuint32 deviceID,
        XAint32 numInputs,
        XAboolean isNew
    );

void DefaultDeviceIDMapChangedCallback (
    XAAudioIODeviceCapabilitiesItf caller,
    void * pContext,
    XAboolean isOutput,
    XAint32 numDevices);
    
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

TInt COpenMAXALTestModule::al_audioiodevcapitf_GetAvailableAudioInputs( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    
    TInt deviceId(0);
    XAint32 devId;
    XAuint32* deviceIdArr(NULL);
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        if(m_AIODevCapItf)
            {
            res = (*m_AIODevCapItf)->GetAvailableAudioInputs(
                    m_AIODevCapItf, &devId, deviceIdArr);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }        
        }
    else
        {
        if(m_AIODevCapItf)
            {
            res = (*m_AIODevCapItf)->GetAvailableAudioInputs(
                    m_AIODevCapItf, &devId, NULL);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }        
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_QueryAudioInputCapabilities( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAAudioInputDescriptor inputDesc;
    XAresult res;
    TUint deviceId(0);
    XAuint32 devId(0);
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        if(m_AIODevCapItf)
            {
            res = (*m_AIODevCapItf)->QueryAudioInputCapabilities(
                    m_AIODevCapItf, devId, &inputDesc);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            } 
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_RegisterAvailableAudioInputsChangedCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    
    if(m_AIODevCapItf)
        {
        res = (*m_AIODevCapItf)->RegisterAvailableAudioInputsChangedCallback(
                m_AIODevCapItf, AvailAudioInputsChangedCallback, (void*)this);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }            
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_GetAvailableAudioOutputs( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    
    TInt deviceId(0);
    XAint32 devId;
    XAuint32* deviceIdArr(NULL);
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        if(m_AIODevCapItf)
            {
            res = (*m_AIODevCapItf)->GetAvailableAudioOutputs(
                    m_AIODevCapItf, &devId, deviceIdArr);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }        
        }
    else
        {
        if(m_AIODevCapItf)
            {
            res = (*m_AIODevCapItf)->GetAvailableAudioOutputs(
                    m_AIODevCapItf, &devId, NULL);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }        
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_QueryAudioOutputCapabilities( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAAudioOutputDescriptor outputDesc;
    XAresult res;
    TUint deviceId(0);
    XAuint32 devId(0);
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        if(m_AIODevCapItf)
            {
            res = (*m_AIODevCapItf)->QueryAudioOutputCapabilities(
                    m_AIODevCapItf, devId, &outputDesc);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            } 
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_RegisterAvailableAudioOutputsChangedCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    
    if(m_AIODevCapItf)
        {
        res = (*m_AIODevCapItf)->RegisterAvailableAudioOutputsChangedCallback(
                m_AIODevCapItf, AvailAudioOutputsChangedCallback, (void*)this);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }            
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_RegisterDefaultDeviceIDMapChangedCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    
    if(m_AIODevCapItf)
        {
        res = (*m_AIODevCapItf)->RegisterDefaultDeviceIDMapChangedCallback(
                m_AIODevCapItf, DefaultDeviceIDMapChangedCallback, (void*)this);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }            
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_GetAssociatedAudioInputs( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    
    TUint deviceId(0);
    TInt sizeArr(0);
    XAuint32 devId(0);
    XAint32 numInputs(0);
    XAuint32 deviceIdArr[2];
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        status = aItem.GetNextInt(sizeArr);
        if(!status)
            {
            numInputs = sizeArr;
            if(m_AIODevCapItf)
                {
                res = (*m_AIODevCapItf)->GetAssociatedAudioInputs(
                        m_AIODevCapItf, devId, &numInputs, deviceIdArr);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }        
            }
        else
            {
            if(m_AIODevCapItf)
                {
                res = (*m_AIODevCapItf)->GetAssociatedAudioInputs(
                        m_AIODevCapItf, devId,&numInputs, NULL);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }                
            }
        }
    else
        {
        status = KErrGeneral;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_GetAssociatedAudioOutputs( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    
    TUint deviceId(0);
    TInt sizeArr(0);
    XAuint32 devId(0);
    XAint32 numOutputs(0);
    XAuint32 deviceIdArr[2];
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        status = aItem.GetNextInt(sizeArr);
        if(!status)
            {
            numOutputs = sizeArr;
            if(m_AIODevCapItf)
                {
                res = (*m_AIODevCapItf)->GetAssociatedAudioOutputs(
                        m_AIODevCapItf, devId, &numOutputs, deviceIdArr);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }        
            }
        else
            {
            if(m_AIODevCapItf)
                {
                res = (*m_AIODevCapItf)->GetAssociatedAudioOutputs(
                        m_AIODevCapItf, devId,&numOutputs, NULL);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }                
            }
        }
    else
        {
        status = KErrGeneral;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_GetDefaultAudioDevices( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    
    TUint deviceId(0);
    TInt sizeArr(0);
    XAuint32 devId(0);
    XAint32 numOutputs(0);
    XAuint32 deviceIdArr[2];
    
    status = aItem.GetNextInt(deviceId, EHex);
    if(!status)
        {
        devId = deviceId;
        status = aItem.GetNextInt(sizeArr);
        if(!status)
            {
            numOutputs = sizeArr;
            if(m_AIODevCapItf)
                {
                res = (*m_AIODevCapItf)->GetDefaultAudioDevices(
                        m_AIODevCapItf, devId, &numOutputs, deviceIdArr);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }        
            }
        else
            {
            if(m_AIODevCapItf)
                {
                res = (*m_AIODevCapItf)->GetDefaultAudioDevices(
                        m_AIODevCapItf, devId,&numOutputs, NULL);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }                
            }
        }
    else
        {
        status = KErrGeneral;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioiodevcapitf_QuerySampleFormatsSupported( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt sizeArr(0);
    TUint deviceId(0);
    TInt samplerate(0);
    XAuint32 devId(0);
    XAmilliHertz samplingrate;
    XAint32 deviceIdArr[20];
    XAint32 numSampleRates(0);
    
    status = aItem.GetNextInt(deviceId);
    if(!status)
        {
        devId = deviceId;
        status = aItem.GetNextInt(samplerate);
        if(!status)
            {
            samplingrate = samplerate;
            status = aItem.GetNextInt(sizeArr);
            if(!status)
                {
                numSampleRates = sizeArr;
                if(m_AIODevCapItf)
                    {
                    res = (*m_AIODevCapItf)->QuerySampleFormatsSupported(
                            m_AIODevCapItf, devId, samplingrate, deviceIdArr, &numSampleRates);
                    status = res;
                    }
                else
                    {
                    status = KErrNotFound;
                    }        
                }
            else
                {
                if(m_AIODevCapItf)
                    {
                    res = (*m_AIODevCapItf)->QuerySampleFormatsSupported(
                            m_AIODevCapItf, devId, samplingrate, NULL, &numSampleRates);
                    status = res;
                    }
                else
                    {
                    status = KErrNotFound;
                    }        
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
    return status;
    }

