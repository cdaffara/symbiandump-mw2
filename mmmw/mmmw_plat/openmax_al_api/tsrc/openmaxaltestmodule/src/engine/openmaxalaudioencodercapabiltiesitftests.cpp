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
#include <strings.h>
#include <StifTestInterface.h>
#include "openmaxaltestmodule.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
#define RETURN_ERR_IF_NOT_EQUAL(item, valFromItem, value) \
    status = item.GetNextInt(valFromItem); \
    if (status || (valFromItem != value)) \
        { \
        iLog->Log(_L("Value[%d] is not equal to [%d]"), valFromItem, value); \
        return KErrCompletion; \
        } \
    

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

TInt COpenMAXALTestModule::al_audioencodercapitf_GetAudioEncoders( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt numEncoders(0);
    XAuint32 deviceIdArr[4];
    XAresult res;
    XAuint32 numEnc(0);
    
    status = aItem.GetNextInt(numEncoders);
    if(!status)
        {
        numEnc = numEncoders;
        if(m_AEncCapItf)
            {
            res = (*m_AEncCapItf)->GetAudioEncoders(
                    m_AEncCapItf, &numEnc, deviceIdArr);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }            
        }
    else
        {
        if(m_AEncCapItf)
            {
            res = (*m_AEncCapItf)->GetAudioEncoders(
                    m_AEncCapItf, &numEnc, NULL);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }            
        }
    return status;
    }

TInt COpenMAXALTestModule::al_audioencodercapitf_GetAudioEncoderCapabilities( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt encId(0);
    TInt idx(0);
    XAuint32 encoderid(0);
    XAuint32 index(0);
    XAAudioCodecDescriptor desc;
    XAresult res;
    
    status = aItem.GetNextInt(encId);
    if(!status)
        {
        status = aItem.GetNextInt(idx);
        if(!status)
            {
            index = idx;
            encoderid = encId;
            if(m_AEncCapItf)
                {
                res = (*m_AEncCapItf)->GetAudioEncoderCapabilities(
                        m_AEncCapItf, encoderid, &index, &desc);
                status = res;
                if (res == XA_RESULT_SUCCESS)
                    {
                    status = validateAudioCodecDescriptorAtIndex(encoderid, desc, aItem);
                    }
                }
            else
                {
                status = KErrNotFound;
                }                
            }
        }
    return status;
}

TInt COpenMAXALTestModule::validateAudioCodecDescriptorAtIndex(
        XAuint32 aAudioCodecId,
        XAAudioCodecDescriptor& aDesc,
        CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt intVal(0);
    TInt loopIndex(0);
    
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.maxChannels);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.minBitsPerSample);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.maxBitsPerSample);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.minSampleRate);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.maxSampleRate);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.isFreqRangeContinuous);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.numSampleRatesSupported);
    for(loopIndex = 0; loopIndex < aDesc.numSampleRatesSupported; loopIndex++)
        {
        RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.pSampleRatesSupported[loopIndex]);
        }
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.minBitRate);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.maxBitRate);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.isBitrateRangeContinuous);
    RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.numBitratesSupported);
    for(loopIndex = 0; loopIndex < aDesc.numBitratesSupported; loopIndex++)
        {
        RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.pBitratesSupported[loopIndex]);
        }
    switch (aAudioCodecId)
        {
        case XA_AUDIOCODEC_PCM:
            RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.profileSetting);
            if (!status)
                {
                RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.modeSetting);
                }
            break;
        case XA_AUDIOCODEC_AMR:
            RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.profileSetting);
            if (!status)
                {
                RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.modeSetting);
                }
            break;
        case XA_AUDIOCODEC_AAC:
            RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.profileSetting);
            if (!status)
                {
                RETURN_ERR_IF_NOT_EQUAL(aItem, intVal, aDesc.modeSetting);
                }
            break;
        }
    return status;
    }
