// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <ecom/ecom.h>

CSendAsEditUtils* CSendAsEditUtils::NewL(TUid aUid)
	{
	// Get the instantiation specified by the UID.
	TAny* ptr = REComSession::CreateImplementationL(aUid, _FOFF(CSendAsEditUtils, iDtor_ID_Key));
	CSendAsEditUtils* utils = reinterpret_cast<CSendAsEditUtils*>(ptr);
	return utils;
	}
	
CSendAsEditUtils::CSendAsEditUtils()
: CBase()
	{
	}
	
CSendAsEditUtils::~CSendAsEditUtils()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}
