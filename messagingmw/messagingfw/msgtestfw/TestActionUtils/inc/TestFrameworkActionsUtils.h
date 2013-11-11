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

#ifndef __TEST_FRAMEWORK_ACTIONS_UTILS_H__
#define __TEST_FRAMEWORK_ACTIONS_UTILS_H__

#include <e32def.h> 
#include <e32std.h>
#include <e32base.h>
#include <s32file.h>


class TestFrameworkActionsUtils
	{
public:
	static TBool CheckIfWatchersAlreadyRunningL();
	static TBool CheckIfSmsWatcherAlreadyRunningL();
	static TInt CompareFilesL(TPtrC aFileOne, TPtrC aFileTwo,
		TBool aIsUnicode, CArrayFixFlat<TUint16>* aIgnoreCharList, TBool& aDiffFlag);

	static TBool ReadDataL(RFileReadStream& aFileReadStream,
		TPtr& aPtrLineBuffer, TBool aIsUnicode, CArrayFixFlat<TUint16>* aIgnoreCharList);

	static TUint8 GetByteFromUnicodeHexTextChar(const TUint16 *input);
	};

#endif
