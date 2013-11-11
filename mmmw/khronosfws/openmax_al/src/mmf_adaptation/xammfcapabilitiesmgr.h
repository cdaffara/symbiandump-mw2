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
 * Description: MMF capability manager
 *
 */

#ifndef XAMMFCAPABILITIESMGR_H
#define XAMMFCAPABILITIESMGR_H

#include "openmaxalwrapper.h"
#include "xaframeworkmgr.h"
#include "xacapabilitiesmgr.h"

XAresult XAMMFCapabilitiesMgr_UpdateCapabilitieList(
        FrameworkMap *frameworkMap, XACapabilities** ppListHead);

#endif /* XAGSTCAPABILITIESMGR_H */
