/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_UTILS_H__
#define __CMTF_TEST_ACTION_UTILS_H__



#include <e32base.h>
#include <barsc.h>
#include <bautils.h>
#include "msvutils.h"



class CMtfTestActionUtils : public CBase
	{
public:
	static void OpenResourceFileL( RResourceFile& rFile, RFs& anFs, const TDesC& aFileName );
	static void CloseResourceFile( TAny* aFile );
	};


#endif  // __CMTF_TEST_ACTION_UTILS_H__
