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
void StreamEventChangeCallback (
        XAStreamInformationItf caller,
        XAuint32 eventId,
        XAuint32 streamIndex,
        void * pEventData,
        void * pContext
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
 
TInt COpenMAXALTestModule::al_strminfoitf_QueryMediaContainerInformation( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    XAMediaContainerInformation info;
    
    if(m_StrInfoItf)
        {
        res = (*m_StrInfoItf)->QueryMediaContainerInformation(m_StrInfoItf,&info);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;    
    }

TInt COpenMAXALTestModule::al_strminfoitf_QueryStreamType( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt index(0);
    XAuint32 streamType;

    status = aItem.GetNextInt(index);
    if(!status)
        {
        if(m_StrInfoItf)
            {
            res = (*m_StrInfoItf)->QueryStreamType(m_StrInfoItf,index, &streamType);
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

TInt COpenMAXALTestModule::al_strminfoitf_QueryStreamInformation( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt streamindex(0);
    void* info = NULL;
    XAuint32 domain;
    XAAudioStreamInformation audioinfo;
    XAVideoStreamInformation videoinfo;
    
    status = aItem.GetNextInt(streamindex);
    RET_ERR_IF_ERR(status);
    
    if(m_StrInfoItf)
        {
        res = (*m_StrInfoItf)->QueryStreamType(m_StrInfoItf,streamindex, &domain);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    
    if(domain == XA_DOMAINTYPE_AUDIO)
        {
        info = &audioinfo;
        }
    else if ( domain == XA_DOMAINTYPE_VIDEO)
        {
        info = &videoinfo;
        }
    
    if(!status)
        {
        if(m_StrInfoItf)
            {
            res = (*m_StrInfoItf)->QueryStreamInformation(m_StrInfoItf,streamindex, info);
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

TInt COpenMAXALTestModule::al_strminfoitf_QueryStreamName( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt streamindex(0);
    XAuint16 size;

    status = aItem.GetNextInt(streamindex);
    if(!status)
        {
        if(m_StrInfoItf)
            {
            res = (*m_StrInfoItf)->QueryStreamName(m_StrInfoItf,streamindex, &size, NULL);
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

TInt COpenMAXALTestModule::al_strminfoitf_RegisterStreamChangeCallback( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
		
		TInt expectedIndex = -1;
    status = aItem.GetNextInt(expectedIndex);
    if(status==KErrNone)
    {
    	iStreamInfoItfIndex = expectedIndex; //reset expected event
    }
    else
    {
    	iStreamInfoItfIndex = -1;
    	status = KErrNone;
    }

    if(m_StrInfoItf)
        {
        res = (*m_StrInfoItf)->RegisterStreamChangeCallback(m_StrInfoItf,StreamEventChangeCallback,(void*)this);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;    
    }

TInt COpenMAXALTestModule::al_strminfoitf_QueryActiveStreams( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    XAuint32 size(0);
    TInt inpsize;
    XAboolean arr[2];
    
    status = aItem.GetNextInt(inpsize);
    if(!status)
        {
        if(m_StrInfoItf)
            {
            size = inpsize;
            res = (*m_StrInfoItf)->QueryActiveStreams(m_StrInfoItf, &size, arr);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    else
        {
        if(m_StrInfoItf)
            {
            res = (*m_StrInfoItf)->QueryActiveStreams(m_StrInfoItf,&size, NULL);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    return status;     
    }

TInt COpenMAXALTestModule::al_strminfoitf_SetActiveStream( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt streamindex(0);
    XAboolean active;
    XAboolean commitnow;
    TInt value;
    
    status = aItem.GetNextInt(streamindex);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    active = value;

    status = aItem.GetNextInt(value);
    RET_ERR_IF_ERR(status);
    commitnow = value;
    
    if(m_StrInfoItf)
        {
        res = (*m_StrInfoItf)->SetActiveStream(m_StrInfoItf,streamindex, active, commitnow);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;          
    }

void COpenMAXALTestModule::HandleStreamInfoItfCallback( XAStreamInformationItf /*caller*/, XAuint32 event, XAuint32 streamIndex )
{
    switch (event)
    {
        case XA_STREAMCBEVENT_PROPERTYCHANGE:
        {
        		if((iStreamInfoItfIndex == -1) || (iStreamInfoItfIndex == streamIndex))
        		{
            	TEventIf event( TEventIf::ESetEvent, _L("Event_XA_STREAMCBEVENT_PROPERTYCHANGE") );
            	TestModuleIf().Event( event );
        		}
        }
        break;
       
        default:
            break;
    }
}

TInt COpenMAXALTestModule::al_strminfoitf_SetStreamInfoIndex(CStifItemParser& aItem)
{
		TInt status(KErrNone);
    TInt streamindex(0);
 
    status = aItem.GetNextInt(streamindex);
    RET_ERR_IF_ERR(status);
		iStreamInfoItfIndex = streamindex;
		return status;
}

void StreamEventChangeCallback (
        XAStreamInformationItf caller,
        XAuint32 eventId,
        XAuint32 streamIndex,
        void * pEventData,
        void * pContext
)
{
    if (pContext)
    {
    	((COpenMAXALTestModule*)pContext)->HandleStreamInfoItfCallback(caller, eventId, streamIndex);
    }
}
