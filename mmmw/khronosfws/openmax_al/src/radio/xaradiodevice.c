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
* Description: 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "xaradiodevice.h"
#include "xaradioitf.h"
#include "xadynintmgmtitf.h"
#include "xathreadsafety.h"
#include "xaradioadaptctx.h"
/* Static mapping of enumeration XARadioDeviceInterfaces to interface iids */
static const XAInterfaceID* XARadioDeviceItfIIDs[RADIO_ITFCOUNT]=
{
    &XA_IID_OBJECT,
    &XA_IID_RADIO,
    &XA_IID_DYNAMICINTERFACEMANAGEMENT
};

/*****************************************************************************
 * Global methods
 *****************************************************************************/

/* XAResult XARadioDeviceImpl_Create
 * Description: Create object
 */
XAresult XARadioDeviceImpl_CreateRadioDevice(XAObjectItf* pDevice,
                                             XAuint32 numInterfaces,
                                             const XAInterfaceID * pInterfaceIds,
                                             const XAboolean * pInterfaceRequired)
{
    XARadioDeviceImpl* pImpl = NULL;
    XAObjectItfImpl* pBaseObj = NULL;
    XAuint8 itfIndex = 0;

    DEBUG_API("->XARadioDeviceImpl_Create");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);
   
    if( !pDevice )
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioDeviceImpl_CreateRadioDevice");
        return XA_RESULT_PARAMETER_INVALID;
    }

    /* instantiate object implementation */
    pImpl = (XARadioDeviceImpl*)calloc(1,sizeof(XARadioDeviceImpl));
    if( !pImpl )
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* memory allocation failed */
        DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
        DEBUG_API("<-XARadioDeviceImpl_CreateRadioDevice");
        return XA_RESULT_MEMORY_FAILURE;
    }
    pBaseObj = &pImpl->baseObj;

    /* Initialize base object default implementation */
    XAObjectItfImpl_Init(pBaseObj,
                         RADIO_ITFCOUNT,
                         XARadioDeviceItfIIDs,
                         XARadioDeviceImpl_DoRealize,
                         XARadioDeviceImpl_DoResume,
                         XARadioDeviceImpl_FreeResources);

    /* Mark interfaces that need to be exposed */
    /* Implicit and mandated interfaces */
    pBaseObj->interfaceMap[RADIO_RADIOITF].required = XA_BOOLEAN_TRUE;
    pBaseObj->interfaceMap[RADIO_DIMITF].required = XA_BOOLEAN_TRUE;

    /* Explicit interfaces */
    if( (numInterfaces != 0) && pInterfaceIds && pInterfaceRequired )
    {
        /* Check required interfaces */
        for( itfIndex = 0; itfIndex < numInterfaces; itfIndex++ )
        {
            /* If mapEntry is null then required interface is not supported.*/
            XAObjItfMapEntry *entry =
                XAObjectItfImpl_GetItfEntry((XAObjectItf)&(pBaseObj), pInterfaceIds[itfIndex]);
            if( !entry )
            {
                if( pInterfaceRequired[itfIndex] )
                {
                    /* required interface cannot be accommodated - fail creation */
                    XAObjectItfImpl_Destroy((XAObjectItf)&(pBaseObj));
                    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
                    DEBUG_ERR("Required interface not found - abort creation!");
                    DEBUG_API("<-XARadioDeviceImpl_CreateRadioDevice");
                    return XA_RESULT_FEATURE_UNSUPPORTED;
                }
                else
                {
                    DEBUG_INFO("Requested (not required) interface not found - continue creation");
                }
            }
            else
            {
                entry->required = XA_BOOLEAN_TRUE;
            }
        }
    }

    pImpl->adaptationCtx = XARadioAdapt_Create();
    if (pImpl->adaptationCtx == NULL)
    {
        XAObjectItfImpl_Destroy((XAObjectItf)&(pBaseObj));
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        DEBUG_API("<-XARadioDeviceImpl_CreateRadioDevice With ERROR");
        return XA_RESULT_RESOURCE_ERROR;
    }

    *pDevice = (XAObjectItf)&(pBaseObj->self);
    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioDeviceImpl_Create");
    return XA_RESULT_SUCCESS;
}

/* XAResult XARadioDeviceImpl_QueryNumSupportedInterfaces
 * Description: Statically query number of supported interfaces
 */
XAresult XARadioDeviceImpl_QueryNumSupportedInterfaces( XAuint32 *pNumSupportedInterfaces )
{
    DEBUG_API("->XARadioDeviceImpl_QueryNumSupportedInterfaces");
    if( pNumSupportedInterfaces )
    {
        *pNumSupportedInterfaces = RADIO_ITFCOUNT;

        DEBUG_API_A1("<-XARadioDeviceImpl_QueryNumSupportedInterfaces - %lu", *pNumSupportedInterfaces );
        return XA_RESULT_SUCCESS;
    }
    else
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioDeviceImpl_QueryNumSupportedInterfaces");
        return XA_RESULT_PARAMETER_INVALID;
    }
}
/* XAResult XARadioDeviceImpl_QuerySupportedInterfaces
 * Description: Statically query supported interfaces
 */
XAresult XARadioDeviceImpl_QuerySupportedInterfaces( XAuint32 index,
                                                      XAInterfaceID * pInterfaceId )
{
    DEBUG_API("->XARadioDeviceImpl_QuerySupportedInterfaces");

    if( index >= RADIO_ITFCOUNT || !pInterfaceId )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioDeviceImpl_QuerySupportedInterfaces");
        return XA_RESULT_PARAMETER_INVALID;
    }
    else
    {
        *pInterfaceId = *(XARadioDeviceItfIIDs[index]);

        DEBUG_API("<-XARadioDeviceImpl_QuerySupportedInterfaces");
        return XA_RESULT_SUCCESS;
    }
}


/*****************************************************************************
 * base object XAObjectItfImpl methods
 *****************************************************************************/

/* XAresult XARadioDeviceImpl_DoRealize( XAObjectItf self )
 * Description: Realize all implicit and explicitly wanted interfaces.
 * Create and initialize implementation-specific variables.
 * Called from base object XAObjectItfImpl
 */
XAresult XARadioDeviceImpl_DoRealize( XAObjectItf self )
{
    XAuint8 itfIdx = 0;
    XAObjectItfImpl* pObj = (XAObjectItfImpl*)(*self);
    XARadioDeviceImpl* pObjImpl = (XARadioDeviceImpl*)(pObj);
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("->XARadioDeviceImpl_DoRealize");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    /* check casting from correct pointer type */
    if( !pObjImpl || pObj != pObjImpl->baseObj.self )
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioDeviceImpl_DoRealize");

        return XA_RESULT_PARAMETER_INVALID;
    }


    ret = XARadioAdapt_PostInit( pObjImpl->adaptationCtx );

    if( ret != XA_RESULT_SUCCESS )
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        DEBUG_API("<-XARadioDeviceImpl_DoRealize");
        return ret;
    }

    /* Realize all implicit and explicitly wanted interfaces */
    for( itfIdx = 0; itfIdx < RADIO_ITFCOUNT; itfIdx++)
    {
        if( !(pObj->interfaceMap[itfIdx].pItf) &&
            pObj->interfaceMap[itfIdx].required )
        {
            void *pItf = NULL;
            switch( itfIdx )
            {
                case RADIO_RADIOITF:

                    pItf = XARadioItfImpl_Create( pObjImpl->adaptationCtx );
                    break;
                case RADIO_DIMITF:
                    pItf = XADIMItfImpl_Create();
                    break;
                default:
                    break;
            }
            if( !pItf )
            {
                DEBUG_ERR("XA_RESULT_MEMORY_FAILURE");
                /* memory allocation failed */
                DEBUG_API("<-XARadioDeviceImpl_DoRealize");
                XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
                return XA_RESULT_MEMORY_FAILURE;
            }
            else
            {
                pObj->interfaceMap[itfIdx].pItf = pItf;
            }
        }
    }

    pObj->state = XA_OBJECT_STATE_REALIZED;
    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioDeviceImpl_DoRealize");
    return XA_RESULT_SUCCESS;
}

/* XAresult XARadioDeviceImpl_DoResume
 * Description: Resume object from suspended state
 */
XAresult XARadioDeviceImpl_DoResume(XAObjectItf self)
{
    DEBUG_API("->XARadioDeviceImpl_DoResume");
    DEBUG_API("<-XARadioDeviceImpl_DoResume");

    return XA_RESULT_PRECONDITIONS_VIOLATED;
}

/* void XARadioDeviceImpl_FreeResources
 * Description: Free all resources reserved at XARadioDeviceImpl_DoRealize()
 */
void XARadioDeviceImpl_FreeResources(XAObjectItf self)
{
    XAObjectItfImpl* pObj = (XAObjectItfImpl*)(*self);
    XARadioDeviceImpl* pImpl = (XARadioDeviceImpl*)(*self);
    XAuint8 itfIdx = 0;
    DEBUG_API("->XARadioDeviceImpl_FreeResources");
    XA_IMPL_THREAD_SAFETY_ENTRY_FOR_VOID_FUNCTIONS(XATSRadio);

    /* free all allocated interfaces */
    for(itfIdx = 0; itfIdx < RADIO_ITFCOUNT; itfIdx++)
    {
        void *pItf = pObj->interfaceMap[itfIdx].pItf;
        if(pItf)
        {
            switch(itfIdx)
            {
                case RADIO_RADIOITF:
                    XARadioItfImpl_Free( pItf );
                    break;
                case RADIO_DIMITF:
                    XADIMItfImpl_Free( pItf );
                    break;
                default:
                    break;
            }
            pObj->interfaceMap[itfIdx].pItf = NULL;
        }
    }


    if ( pImpl->adaptationCtx != NULL )
    {
        XARadioAdapt_Destroy( pImpl->adaptationCtx );
        pImpl->adaptationCtx = NULL;
    }

    XA_IMPL_THREAD_SAFETY_EXIT_FOR_VOID_FUNCTIONS(XATSRadio);
    DEBUG_API("<-XARadioDeviceImpl_FreeResources");
    return;
}

/* END OF FILE */
