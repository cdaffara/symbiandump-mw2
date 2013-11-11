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


#ifndef __CMTF_TEST_ACTION_UTILS_CONFIG_FILE_PARSER_H__
#define __CMTF_TEST_ACTION_UTILS_CONFIG_FILE_PARSER_H__

#include <e32base.h>


class CMtfTestActionUtilsConfigFileParser : public CBase
	{
public:
	static CMtfTestActionUtilsConfigFileParser* NewL(const TDesC& aFileName);
   ~CMtfTestActionUtilsConfigFileParser();

public:
   TInt GetFieldAsInteger(const TDesC& aFieldName,TInt& aValue);
   TInt GetFieldAsString(const TDesC& aFieldName,TPtrC& aString);
   TInt GetFieldAsString8(const TDesC& aFieldName,TPtrC8& aString);

private:
	CMtfTestActionUtilsConfigFileParser();
	void ConstructL(const TDesC& aFileName); 

private:
	RPointerArray<HBufC> iName;
	RPointerArray<HBufC> iContent;
	RPointerArray<HBufC8> iContent8;
	};


#endif
