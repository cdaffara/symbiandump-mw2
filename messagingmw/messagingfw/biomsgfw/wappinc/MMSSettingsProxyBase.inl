// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MMSSettingProxyBase.inl
// Base MMS Settings Proxy
// 
//

#include <mmssettingsproxybase.h>

CMMSSettingsProxyBase* CMMSSettingsProxyBase::NewL(TUid aInterfaceImplUid)
	{
	TAny* ptr = NULL;
	ptr = REComSession::CreateImplementationL(aInterfaceImplUid, _FOFF(CMMSSettingsProxyBase, iDtor_ID_Key));
	return REINTERPRET_CAST(CMMSSettingsProxyBase*, ptr);
	}

CMMSSettingsProxyBase::~CMMSSettingsProxyBase()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}
