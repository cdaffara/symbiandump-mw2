/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __MTF_TEST_ACTION_UTILS_USER_H__
#define __MTF_TEST_ACTION_UTILS_USER_H__


#include <e32base.h>
#include <msvapi.h>


class MtfTestActionUtilsUser 
	{
public:
	static TInt CopyDescriptor16To8(const TDesC16& aDesc16,TDes8& aDesc8);
	static TInt FinalProgressStatus(CMsvOperation& aOperation,const TRequestStatus& aStatus);
	};


#endif
