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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __PLUGINKILLER_H__
#define __PLUGINKILLER_H__


#include <e32base.h>

//Forward Declarations
class CPushHandlerBase;

/** 
Utility class used by a WAP Push Application plugin to delete itself. 

@publishedPartner
@released
*/
class CPluginKiller: public CActive 
	{
public:
	virtual ~CPluginKiller();

	CPluginKiller(CPushHandlerBase* aPushPlugin);
	IMPORT_C void KillPushPlugin();

protected:
	virtual void DoCancel();
	virtual void RunL();

private:
	CPushHandlerBase*		iPushPlugin;
	TBool					iDeletePushPlugin;
	};

#endif //_PLUGINKILLER_H
