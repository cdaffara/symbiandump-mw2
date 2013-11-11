/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



/**
 @file
*/
 
#ifndef __CENTRALREPOUTILS_H__
#define __CENTRALREPOUTILS_H__

class CClientMtmRegistry;
class CBaseMtm;
class CMsvSession;

class CentralRepoUtils
{
public:
	static CBaseMtm* CreateMtmL(TUid aMtmUid, CClientMtmRegistry* mtmRegistry);
	static void DeleteServiceL(CMsvSession* aSession);
	static void CreateServiceL(CMsvSession* aSession, const TDesC& aDescription, const TDesC& aDetails);
	static void SetSmsServiceSettingsL(TDesC& aFileName);
	static void SetSchSendSettingsL(TDesC& aFileName);
};

#endif //__CENTRALREPOUTILS_H__
