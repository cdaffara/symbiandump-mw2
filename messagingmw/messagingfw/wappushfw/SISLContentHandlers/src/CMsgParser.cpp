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

#include <f32file.h>
#include <xml/documentparameters.h>
#include <xml/taginfo.h>
#include <xml/attribute.h>

using namespace Xml;
 
#include <xml/xmlparsererrors.h>
#include <xml/xmlframeworkerrors.h>

// User Include
#include "CMsgParser.h"
#include "errorcodes.h"
//text SL MIME type
_LIT(KSLTextContentType, "text/vnd.wap.sl");
_LIT(KSITextContentType, "text/vnd.wap.si");

_LIT8(KXmlParserDataType, "text/xml");
_LIT8(KWbxmlParserDataType, "text/wbxml");

/** 
 * Constructor
 * Copy CPushMessage pointer for reference during parse operations
 */
CMessageParser::CMessageParser ( CPushMessage& aPushMessage, 
								MWAPElementHandler& aElementHandler,
								const TStringTable* aTagsTable,
								const TStringTable* aAttributeTable,
								const TStringTable* aAttributeValueTable ) 
	: iPushMessage ( aPushMessage ),
	iElementHandler ( aElementHandler ),
	iTagsTable ( aTagsTable ),
	iAttributeTable ( aAttributeTable ),
	iAttributeValueTable ( aAttributeValueTable )
	{
	}

/** 
*/
CMessageParser::~CMessageParser()
	{
	delete iParser;
	}

/** 

*/
CMessageParser* CMessageParser::NewL( CPushMessage& aPushMessage, 
									MWAPElementHandler& aElementHandler,
									const TStringTable* aTagsTable,
									const TStringTable* aAttributeTable,
									const TStringTable* aAttributeValueTable )
	{
	CMessageParser* self = new ( ELeave ) CMessageParser ( aPushMessage, aElementHandler, aTagsTable, aAttributeTable, aAttributeValueTable );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ( self );
	return ( self );
	}

/** 

*/
void CMessageParser::ConstructL()
	{
	LoadPluginL ();
	RStringPool& pool = StringPool ();
	pool.OpenL ( *iTagsTable );
	pool.OpenL ( *iAttributeTable );
	pool.OpenL ( *iAttributeValueTable );
	}

void CMessageParser::LoadPluginL ()
	{
	TPtrC contentType;
	iPushMessage.GetContentType ( contentType );
	TBool xml = ETrue;
	if ( contentType.CompareF ( KSLTextContentType ) && contentType.CompareF ( KSITextContentType ) )
		{
		xml = EFalse;
		}
		
	iParser = CParser::NewL ( ( xml ) ? KXmlParserDataType() : KWbxmlParserDataType(), *this );
	}
/**

*/
TInt CMessageParser::ParseMessageL ()
	{	
	// Begin parsing
	iParser->ParseBeginL ();
	
	// Get the message body and do parsing.
	TPtrC8 msgBody;
	iPushMessage.GetMessageBody ( msgBody );
	
	iParser->ParseL ( msgBody );
	
	// End parsing
	iParser->ParseEndL ();
	
	return KErrNone;
	}

RStringPool& CMessageParser::StringPool ()
	{
	return iParser->StringPool ();
	}

void CMessageParser::OnStartDocumentL ( const Xml::RDocumentParameters& /* aDocParam */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageParser::OnEndDocumentL ( TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageParser::OnStartElementL ( const Xml::RTagInfo& aElement, const Xml::RAttributeArray& aAttributes, TInt aErrorCode )
	{	
	User::LeaveIfError( MapToWapXMLError ( aErrorCode ) );

	TInt nAttributes = aAttributes.Count();
	RString tag = aElement.LocalName();
	CheckTagInTableL ( tag );
 
	for ( TInt i = 0; i < nAttributes; ++i )
		{
		const RAttribute& attribute = aAttributes[i];
		const RTagInfo& nameInfo = attribute.Attribute();
		
		RString attr = nameInfo.LocalName();
		RString attributeValue = attribute.Value();
		CheckAttributeValuePairL ( attr, attributeValue );
		iElementHandler.HandleElementL ( tag, attr, attributeValue );
		}
	}
 
void CMessageParser::OnEndElementL ( const Xml::RTagInfo& /* aElement */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );	
	}

void CMessageParser::OnContentL ( const TDesC8& aBytes, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	iElementHandler.HandleContentL ( aBytes );
	}

void CMessageParser::OnStartPrefixMappingL ( const RString& /* aPrefix */, const RString& /* aUri */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageParser::OnEndPrefixMappingL ( const RString& /* aPrefix */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageParser::OnIgnorableWhiteSpaceL ( const TDesC8& /* aBytes */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageParser::OnSkippedEntityL ( const RString& /* aName */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageParser::OnProcessingInstructionL ( const TDesC8& /* aTarget */, const TDesC8& /* aData */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}
 
void CMessageParser::OnError ( TInt aErrorCode )
	{
	iLastError = MapToWapXMLError ( aErrorCode );
	}

TAny* CMessageParser::GetExtendedInterface ( const TInt32 /* aUid */ )
	{
	return NULL;
	}

void CMessageParser::CheckTagInTableL ( const RString& aTag )
	{
	// Check the tag is appearing in the tags table
	TInt index = aTag.Index ( *iTagsTable );
	
	if ( ( index == KErrNotFound ) || ( iTagPosition != index  ) )
		{
		// This tag name does not appear in tags table or tag is not in the right position as in DTD. 
		// Tag is illegal or shouldn't appear or not in the expected position in XML content .
		User::Leave ( EWapErrXmlLibIllegalTagName );
		}
		++iTagPosition; // Increment the tag position.
	}

void CMessageParser::CheckAttributeValuePairL ( const RString& aAttribute, const RString& aAttributeValue )
{
	// Check the attribute is appearing in attribute table.
	TInt attributeIndex = aAttribute.Index ( *iAttributeTable );
	
	if ( attributeIndex == KErrNotFound )
		{
		User::Leave ( EWapErrXmlLibMissingRequiredAttribute );
		}
	RStringPool& pool = iParser->StringPool();
	// Loop it around the attribute & value table. Value can be pre-defined and specific
	for ( TInt i = attributeIndex; i < iAttributeTable->iCount; ++i  )
		{
		// Check the attribute matches
		RString attribute = pool.String ( i, *iAttributeTable );
		if ( attribute != aAttribute )
			{
			break;
			}		
		
		RString attributeVal = pool.String ( i, *iAttributeValueTable );		
		if ( ( attributeVal.DesC().Length() == 0 ) || ( attributeVal == aAttributeValue ) )
			{
			// Attribute takes any attribute value or attribute value mataches with the table values.
			// Attribute value is valid.
			return;	
			}								
 		}
	
	User::Leave ( EWapErrXmlLibIllegalAttributeValue );	
	}

TInt CMessageParser::LastError ()
	{
	return iLastError;
	}

// Map the generic XML parser code to WAP XML errors. Description of the error code
// has taken from expat.
TInt CMessageParser::MapToWapXMLError ( TInt aXmlErrorCode )
	{
	
	TInt errorCode = aXmlErrorCode;
	
	switch ( aXmlErrorCode )
		{
		case EXmlParserError:	
		case EXmlSyntax:			
		errorCode = EWapErrGeneral;
		break;
		// The document contains no elements 
		// (XML requires all documents to contain exactly one top-level element)..
		case EXmlNoElements:
		errorCode = EWapErrXmlLibMissingDocumentRootNode;
		break;
		
		// Raised when an input byte could not properly be assigned to a character; 
		// for example, a NUL byte (value 0) in a UTF-8 input stream.
		case EXmlInvalidToken:
		errorCode = EWapErrXmlLibIllegalTagName;
		break;
		
		// Some token (such as a start tag) was not closed before the end of the stream or 
		// the next token was encountered.
		case EXmlUnclosedToken:
		// An end tag did not match the innermost open start tag.
		case EXmlTagMismatch:
		errorCode = EWapErrXmlLibEndTagMismatch;
		break;
		
		// An incomplete character was found in the input.
		case EXmlPartialChar:
		// A character reference referred to a character which is illegal in XML 
		// (for example, character 0, or `&#0;').
		case EXmlBadCharRef:
		errorCode = EWapErrXmlLibInvalidCharacterReference;
		break;		
		
		// An attribute was used more than once in a start tag.
		case EXmlDuplicateAttribute:
		errorCode = EWapErrXmlLibInvalidAttributeDeclaration;
		break;
		
		// Something other than whitespace occurred after the document element.
		case EXmlJunkAfterDocElement:		
		// A parameter entity reference was found where it was not allowed.
		case EXmlPeRef:
		errorCode = EWapErrDocumentCorrupted;
		break;
		
		// A reference was made to a entity which was not defined.
		case EXmlUndefinedEntity:
		// An entity reference contained another reference to the same entity; possibly via a 
		// different name, and possibly indirectly.
		case EXmlRecursiveEntity:		
		case EXmlAsyncEntity:
		// An entity reference in an attribute value referred to an external entity instead of an internal entity.
		case EXmlAttributeExternalEntityRef:
		case EXmlExternalEntityHandling:
		errorCode = EWapErrXmlLibUnknownEntityReference;
		break;

		// An entity reference referred to an entity which was declared with a notation, 
		// so cannot be parsed.
		case EXmlBinaryEntityRef:
		errorCode = EWapErrDocumentCorrupted;
		break;
		
		// An XML declaration was found somewhere other than the start of the input data.				
		case EXmlMisplacedPi:
		case EXmlIncorrectEncoding:
		errorCode = EWapErrXmlLibInvalidDocument;
		break;
		// The document encoding is not supported by Expat.				
		case EXmlUnknownEncoding:
		errorCode = EWapErrUnknownDocument; 
		break;		
		
		// A CDATA marked section was not closed.
		case EXmlUnclosedCdata:
		errorCode = EWapErrXmlLibMissingCDATASectionEndTag;
		break;

// The parser determined that the document was not ``standalone'' 
		// though it declared itself to be in the XML declaration, and the NotStandaloneHandler 
		// was set and returned 0.
		case EXmlNotStandalone:
		case EXmlUnexpectedState:
		case EXmlEntityDeclInPe:
		errorCode = EWapErrUnknownDocument;
		break;
		
		case EXmlDtdRequired:
		errorCode = EWapErrDTDUnavailable;
		break;
		
		// A behavioral change was requested after parsing started that can only be changed 
		// before parsing has started. This is (currently) only raised by UseForeignDTD().
		case EXmlFeatureLockedWhileParsing:
		errorCode = EWapErrGeneral;
		break;
		
		// Xml framework errors
		case KErrXmlStringDictionaryPluginNotFound:
		case KErrXmlParserPluginNotFound:
		case KErrXmlGeneratorPluginNotFound:
		case KErrXmlPluginNotFound:
		errorCode = EWapErrPluginNotFound;
		break;

		case KErrXmlBadCharacterConversion:
		case KErrXmlUnsupportedCharacterSet:
		case KErrXmlUnavailableCharacterSet:
		errorCode = EWapErrXmlLibInvalidCharacterReference;
		break;
		
		// MStringDictionary errors ( mainly wbxml errors )
		case KErrXmlUnsupportedElement:
		errorCode = EWapErrXmlLibIllegalTagName;
		break;
		
		case KErrXmlUnsupportedAttribute:
		errorCode = EWapErrXmlLibMissingRequiredAttribute;
		break;
		
		case KErrXmlUnsupportedAttributeValue:
		errorCode = EWapErrXmlLibIllegalAttributeValue;
		break;

		case KErrXmlMissingStringDictionary:
		errorCode = EWapErrXmlLibMissingDocument;
		break;

		case KErrXmlUnsupportedDocumentVersion:
		errorCode = EWapErrXmlLibInvalidDocumentStructure;
		break;
		
		case KErrXmlDocumentCorrupt:
		errorCode = EWapErrDocumentCorrupted;
		break;
		
		case KErrXmlStringPoolTableNotFound:
		case KErrXmlBadIndex:
		case KErrXmlUnsupportedExtInterface:
		case KErrXmlLast:
		errorCode = EWapErrGeneral;
		break;
		
		default:
		// Do nothing. any other kind of error.
		break;
		}
	return errorCode;
	}
