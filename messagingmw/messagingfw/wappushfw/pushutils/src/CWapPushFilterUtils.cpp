// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "CWapPushMessageFilter.h"
#include "CWapPushFilterUtils.h"

/** loads the push message filter plugin
	@return a plugin owner which contains a push message filter plugin
	@leave can return with EWapErrPluginNotFound if there is no filter plugin
*/
EXPORT_C CWapPushMessageFilter* CWapPushFilterUtils::GetFilterPluginL()
	{
	return CWapPushMessageFilter::NewL();
	}
