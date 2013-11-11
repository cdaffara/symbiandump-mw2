// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_MSG_SHARED_DATA_COMMON_H__
#define __T_MSG_SHARED_DATA_COMMON_H__


/* User includes */
#include "T_MsgSharedDataBase.h"
#include "messagingtestutility2.h"


/* Epoc include */
#include <mtclbase.h>

// Forward decalaration
class CMsvSearchSortQuery;
class CMsvSearchSortOperation;

/** 
 * This implements a class which creates a shared data object
 * Contains a virtual function to create an MTM object to be implemented
 * by the derived classes based on the UID specified
 */
class CT_MsgSharedDataCommon : public CT_MsgSharedDataBase
	{
public:
	/* Creates an object of CT_MsgSharedDataCommon */
	static CT_MsgSharedDataCommon* NewL();
	
protected:
	void ConstructL();

	CT_MsgSharedDataCommon();
	~CT_MsgSharedDataCommon();
	
	/** 
	 * Virtual function to be implemented by the derived classes to create
	 * the required MTM object based on the UID specified
	 */
	virtual CBaseMtm* NewMtmL() { return NULL; };

public:
	RMessagingTestUtilityServer2 iUtilityServer;
	CMsvSearchSortQuery *iSearchSortQuery;
	CMsvSearchSortOperation *iSearchSortOperation;
	RArray<TMsvEntry> iSearchSortResultArray;
	};
#endif /* __T_MSG_SHARED_DATA_COMMON_H__ */
