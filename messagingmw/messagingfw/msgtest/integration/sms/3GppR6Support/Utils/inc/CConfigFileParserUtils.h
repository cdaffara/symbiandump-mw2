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


#ifndef __CCONFIGFILEPARSERUTILS_H__
#define __CCONFIGFILEPARSERUTILS_H__

#include <e32base.h>

class CConfigFileParserUtils: public CBase
	{
public:
	static CConfigFileParserUtils* NewL(const TDesC& aFileName);
   ~CConfigFileParserUtils();

public:
   TInt GetFieldAsInteger(const TDesC& aFieldName,TInt& aValue);
   TInt GetFieldAsString(const TDesC& aFieldName,TPtrC& aString);
   TInt GetFieldAsString8(const TDesC& aFieldName,TPtrC8& aString);

private:
	CConfigFileParserUtils();
	void ConstructL(const TDesC& aFileName); 

private:
	RPointerArray<HBufC> iName;
	RPointerArray<HBufC> iContent;
	RPointerArray<HBufC8> iContent8;
	};

#endif //__CCONFIGFILEPARSERUTILS_H__