// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalTechnology
 @released
*/

#ifndef __ERRORCODES_H__
#define __ERRORCODES_H__

typedef enum TWapEngineStatus
{
	EWapErrGeneral								= -10000,
	EWapErrDocumentCorrupted					= -10002,
	EWapErrUnknownDocument						= -10024,
	EWapErrDTDUnavailable						= -10022,	// Document cannot be validated as DTD is not available
	EWapErrPluginNotFound						= -10018,	// couldn't find the requested plugin

	// Validator
#define XmlValidatorErrorBase -13100
	EWapErrXmlLibIllegalTagName					= XmlValidatorErrorBase,
	EWapErrXmlLibMissingRequiredAttribute		= XmlValidatorErrorBase -  2,
	EWapErrXmlLibMissingDocument				= XmlValidatorErrorBase -  4,
	EWapErrXmlLibInvalidDocumentStructure		= XmlValidatorErrorBase -  6,
	EWapErrXmlLibIllegalAttributeValue			= XmlValidatorErrorBase -  7,

	// Parser
#define XmlParserErrorBase -13000
	EWapErrXmlLibInvalidAttributeDeclaration	= XmlParserErrorBase -  1,
	EWapErrXmlLibEndTagMismatch					= XmlParserErrorBase -  2,	
	EWapErrXmlLibInvalidCharacterReference		= XmlParserErrorBase -  3,
	EWapErrXmlLibUnknownEntityReference			= XmlParserErrorBase -  4,
	EWapErrXmlLibMissingDocumentRootNode		= XmlParserErrorBase -  6,

#define XmlParserErrorBase -13000
	EWapErrXmlLibMissingCDATASectionEndTag		= XmlParserErrorBase,

	// XmlLib
#define XmlLibErrorBase -13200
	EWapErrXmlLibInvalidDocument				= XmlLibErrorBase,

}TWapEngineStatus;


#endif
