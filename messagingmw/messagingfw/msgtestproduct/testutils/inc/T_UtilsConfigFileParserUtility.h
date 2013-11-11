/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgUtilsConfigFileParserUtility class
* 
*
*/



/**
 @file
*/



#ifndef __T_UTILS_CONFIG_FILE_PARSER_UTILITY_H__
#define __T_UTILS_CONFIG_FILE_PARSER_UTILITY_H__


// epoc includes
#include <e32base.h>


/**
This utility class used to parse a .txt(Email settings) file.
*/
class CT_MsgUtilsConfigFileParserUtility : public CBase
	{
public:
	 IMPORT_C static CT_MsgUtilsConfigFileParserUtility* NewL(const TDesC& aFileName);
	~CT_MsgUtilsConfigFileParserUtility();
	
	IMPORT_C TInt GetFieldAsInteger(const TDesC& aFieldName, TInt& aValue);
	IMPORT_C TInt GetFieldAsString(const TDesC& aFieldName, TPtrC& aString);
	IMPORT_C TInt GetFieldAsString8(const TDesC& aFieldName, TPtrC8& aString);

private:
	IMPORT_C CT_MsgUtilsConfigFileParserUtility();
	IMPORT_C void ConstructL(const TDesC& aFileName); 

private:
	RPointerArray<HBufC> iName;
	RPointerArray<HBufC> iContent;
	RPointerArray<HBufC8> iContent8;
	};
#endif //__T_UTILS_CONFIG_FILE_PARSER_UTILITY_H__
