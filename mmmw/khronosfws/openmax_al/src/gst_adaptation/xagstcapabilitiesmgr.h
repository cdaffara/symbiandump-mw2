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
 * Description: GST Capability Mgr header
 *
 */

#ifndef XAGSTCAPABILITIESMGR_H
#define XAGSTCAPABILITIESMGR_H

#include "openmaxalwrapper.h"
#include "xaframeworkmgr.h"
#include "xacapabilitiesmgr.h"

XAresult XAGSTCapabilitiesMgr_UpdateCapabilitieList(
        FrameworkMap *frameworkMap, XACapabilities** ppListHead);

#endif /* XAGSTCAPABILITIESMGR_H */
