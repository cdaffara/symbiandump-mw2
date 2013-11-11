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

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
void RadioItfCallback(XARadioItf caller, 
void* pContext, 
XAuint32 event,
XAuint32 eventIntData,
XAboolean eventBooleanData);

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

TInt COpenMAXALTestModule::al_radioitf_SetFreqRange( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt freqRange(0);
    status = aItem.GetNextInt(freqRange);
    RET_ERR_IF_ERR(status);

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->SetFreqRange(
                m_RadioItf, freqRange);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetFreqRange( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint8 freqRange;
    TInt expectedRange;
    status = aItem.GetNextInt(expectedRange);
    RET_ERR_IF_ERR(status);

    if (m_RadioItf)
        {
        status = (*m_RadioItf)->GetFreqRange(
                m_RadioItf, &freqRange);
        if (!status)
            {
            if (freqRange != (TInt)expectedRange)
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

TInt COpenMAXALTestModule::al_radioitf_IsFreqRangeSupported( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt freqRange;  
    XAboolean isSupported;
    status = aItem.GetNextInt(freqRange);    
    if (m_RadioItf)
        {
        status = (*m_RadioItf)->IsFreqRangeSupported(
                m_RadioItf, (XAuint8)freqRange, &isSupported);
        if (!isSupported)
        	status = KErrCompletion;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetFreqRangeProperties( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt range;
    status = aItem.GetNextInt(range);
    RET_ERR_IF_ERR(status);
    TInt expectedMinFreq;
    status = aItem.GetNextInt(expectedMinFreq);
    RET_ERR_IF_ERR(status);
    TInt expectedMaxFreq;
    status = aItem.GetNextInt(expectedMaxFreq);
    RET_ERR_IF_ERR(status);
    TInt expectedFreqInterval;
    status = aItem.GetNextInt(expectedFreqInterval);
    RET_ERR_IF_ERR(status);

    XAuint32 minFreq;
    XAuint32 maxFreq;
    XAuint32 freqInterval;

    if (m_RadioItf)
        {
        status = (*m_RadioItf)->GetFreqRangeProperties(
                m_RadioItf, range, &minFreq, &maxFreq, &freqInterval);
        if (!status)
            {
            /* If duration does not fall between the range we expected fail the case */
            if ( ( minFreq != (XAuint32)expectedMinFreq) || (maxFreq != (XAuint32)expectedMaxFreq) || (freqInterval != (XAuint32)expectedFreqInterval) )
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

TInt COpenMAXALTestModule::al_radioitf_SetFrequency( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt freq;
    status = aItem.GetNextInt(freq);
    RET_ERR_IF_ERR(status);
    if (m_RadioItf)
        {
        status = (*m_RadioItf)->SetFrequency(
                m_RadioItf, freq);
        if (status != KErrNone)
            status = KErrCompletion;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetFrequency( CStifItemParser& aItem)
    {
    TInt status(KErrNone);
    TInt expectedFreq;
    status = aItem.GetNextInt(expectedFreq); 
    RET_ERR_IF_ERR(status); 
    XAuint32 freq;

    if (m_RadioItf)
        {
        status = (*m_RadioItf)->GetFrequency(
                m_RadioItf, &freq);
         if (!status)
            {
            if ( freq != expectedFreq )
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

TInt COpenMAXALTestModule::al_radioitf_RegisterRadioCallback( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;

    if(m_RadioItf)
        {
        res = (*m_RadioItf)->RegisterRadioCallback(
                m_RadioItf, &RadioItfCallback, (void*)this);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }


TInt COpenMAXALTestModule::al_radioitf_SetSquelch( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TBool squelch;
    status = aItem.GetNextInt(squelch);
    RET_ERR_IF_ERR(status);

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->SetSquelch(
                m_RadioItf, squelch);
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetSquelch( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TBool expectedSquelch;
    status = aItem.GetNextInt(expectedSquelch);
    RET_ERR_IF_ERR(status);
    XAboolean squelch;

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->GetSquelch(
                m_RadioItf, &squelch);
         if (!status)
            {
            if (squelch != (XAboolean)expectedSquelch)
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

TInt COpenMAXALTestModule::al_radioitf_CancelSetFrequency( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->CancelSetFrequency(
                m_RadioItf);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_SetStereoMode( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt stereoMode;
    status = aItem.GetNextInt(stereoMode);
    RET_ERR_IF_ERR(status);

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->SetStereoMode(
                m_RadioItf, (XAuint32)stereoMode);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetStereoMode( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt expectedStereoMode;
    status = aItem.GetNextInt(expectedStereoMode);
    RET_ERR_IF_ERR(status);
    XAuint32 stereoMode;

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->GetStereoMode(
                m_RadioItf, &stereoMode);
        if (!status)
           {
           if (stereoMode != (XAboolean)expectedStereoMode)
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

TInt COpenMAXALTestModule::al_radioitf_GetSignalStrength( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAuint32 signalStrength;

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->GetSignalStrength(
                m_RadioItf, &signalStrength);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_Seek( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TBool upwards;
    status = aItem.GetNextInt(upwards);
    RET_ERR_IF_ERR(status);

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->Seek(
                m_RadioItf, upwards);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_StopSeeking( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);

    if(m_RadioItf)
        {
        status = (*m_RadioItf)->StopSeeking(
                m_RadioItf);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetNumberOfPresets( CStifItemParser& /*aItem*/)
    {
    TInt status(KErrNone);
    RET_ERR_IF_ERR(status);
    XAuint32 num;

    if (m_RadioItf)
        {
        status = (*m_RadioItf)->GetNumberOfPresets(
                m_RadioItf, &num);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_SetPreset( CStifItemParser& aItem)
    {
    TInt status(KErrNone);
    TInt preset;
    status = aItem.GetNextInt(preset);
    RET_ERR_IF_ERR(status);
    XAuint32 presetVal = preset;
    XAuint32 freq = 0;
    XAuint8 range = 0;
    XAuint32 mode = 0;

    if (m_RadioItf)
        {
        status = (*m_RadioItf)->SetPreset(m_RadioItf, presetVal, freq, range, mode, 0);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_radioitf_GetPreset( CStifItemParser& aItem)
    {
    TInt status(KErrNone);
    TInt presetToGet;
    status = aItem.GetNextInt(presetToGet);
    RET_ERR_IF_ERR(status); 
    XAuint32 preset = presetToGet;
    XAuint32 freq = 0;
    XAuint8 range = 0;
    XAuint32 mode = 0;
    XAuint16 nameLength = 0;

    if (m_RadioItf)
        {
        status = (*m_RadioItf)->GetPreset(m_RadioItf, preset, &freq, &range, &mode, 0, &nameLength);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

void COpenMAXALTestModule::HandleRadioItfCallback(
        XARadioItf  caller,
        XAuint32    event
)
{

    switch (event)
        {
        case XA_RADIO_EVENT_ANTENNA_STATUS_CHANGED:
            {
            TEventIf antennaStatusEvent( TEventIf::ESetEvent, _L("Event_XA_RADIO_EVENT_ANTENNA_STATUS_CHANGED:") );
            TestModuleIf().Event( antennaStatusEvent );
            }
            break;

        case XA_RADIO_EVENT_FREQUENCY_CHANGED:
            {
            TEventIf freqChangedEvent( TEventIf::ESetEvent, _L("Event_XA_RADIO_EVENT_FREQUENCY_CHANGED") );
            TestModuleIf().Event( freqChangedEvent );
            }
            break;

        case XA_RADIO_EVENT_FREQUENCY_RANGE_CHANGED:
            {
            TEventIf freqRangeChangedEvent( TEventIf::ESetEvent,_L("Event_XA_RADIO_EVENT_FREQUENCY_RANGE_CHANGED"));
            TestModuleIf().Event( freqRangeChangedEvent );     
            }
            break;

        case XA_RADIO_EVENT_SEEK_COMPLETED:
           	{
           	TEventIf seekEvent( TEventIf::ESetEvent, _L("Event_XA_RADIO_EVENT_SEEK_COMPLETED") );
            TestModuleIf().Event( seekEvent );          
            }
            break; 
               
        default:
            break;	
          }

}

void RadioItfCallback (
    XARadioItf caller,
    void * pContext,
    XAuint32 event,
    XAuint32 eventIntData,
    XAboolean eventBooleanData
    )
    {
    if (pContext)
        {
        ((COpenMAXALTestModule*)pContext)->HandleRadioItfCallback(caller, event);
        }
    }

