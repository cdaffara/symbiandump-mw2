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

#ifndef XARADIODEVICE_H
#define XARADIODEVICE_H

#include "openmaxalwrapper.h"
#include "xaobjectitf.h"
#include "xaglobals.h"

#include "xaadptbasectx.h"
/** MACROS **/


/** TYPES **/


/** ENUMERATIONS **/
/* Enumeration for interfaces that RadioDevice supports.  */
typedef enum
{
    RADIO_OBJECTITF,
    RADIO_RADIOITF,
    RADIO_DIMITF,
    RADIO_ITFCOUNT
} XARadioDeviceInterfaces;

/** STRUCTURES **/
/* Specification for RadioDeviceImpl. */
typedef struct XARadioDeviceImpl_
{
    /* Parent for XARadioDeviceImpl */
    XAObjectItfImpl baseObj;

    /* variables */

    XAAdaptationBaseCtx* adaptationCtx;
} XARadioDeviceImpl;


/** METHODS **/

/* base object XAObjectItfImpl methods */
XAresult    XARadioDeviceImpl_DoRealize(XAObjectItf self);
XAresult    XARadioDeviceImpl_DoResume(XAObjectItf self);
void        XARadioDeviceImpl_FreeResources(XAObjectItf self);

/* RadioDeviceImpl -specific methods */

#endif /* XARADIODEVICE_H */
