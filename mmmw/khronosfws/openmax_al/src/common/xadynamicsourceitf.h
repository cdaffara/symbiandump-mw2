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
 * Description: Dynamic Source Interface Implementation Header
 *
 */

#ifndef XADYNAMICSOURCEITF_H
#define XADYNAMICSOURCEITF_H

#include "xaadptbasectx.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XADynamicSourceItf implementation */
typedef struct XADynamicSourceItfImpl_
    {
    /* parent interface */
    struct XADynamicSourceItf_ itf;
    /* pointer to self */
    struct XADynamicSourceItfImpl_* self;
    /* variables */
    XAAdaptationBaseCtx *adaptCtx;

    } XADynamicSourceItfImpl;

/** METHODS **/

/* Base interface XADynamicSourceItf implementation
 * See API Specification for method documentation
 */
XAresult XADynamicSourceItfImpl_SetSource(XADynamicSourceItf self,
        XADataSource *pDataSource);

/* XADynamicSourceItfImpl -specific methods */
XADynamicSourceItfImpl* XADynamicSourceItfImpl_Create(
        XAAdaptationBaseCtx *adaptCtx);
void XADynamicSourceItfImpl_Free(XADynamicSourceItfImpl* self);

#endif /* XADYNAMICSOURCEITF_H */
