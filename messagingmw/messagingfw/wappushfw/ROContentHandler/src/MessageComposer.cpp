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

#include <f32file.h>
#include <xml/documentparameters.h>
#include <xml/taginfo.h>
#include <xml/attribute.h>

using namespace Xml;

// System Include
//For base64 encoding
#include <tconvbase64.h>
#include <xml/xmlparsererrors.h>
#include <xml/xmlframeworkerrors.h>
#include <xml/parserfeature.h>

// User Include
#include "MessageComposer.h"
#include "errorcodes.h"

const TInt KXmlBufferlength = 128;
//text DRM MIME type
_LIT(KDRMTextContentType, "application/vnd.oma.drm.rights+xml");

//prefix and suffix to be appended.
_LIT8(KXmlParserDataType, "text/xml");
_LIT8(KWbxmlParserDataType, "text/wbxml");
_LIT8(KOpeningBracket, "<");
_LIT8(KClosingBracket, ">");
_LIT8(KOpeningEndBracket, "</");
_LIT8(KOpeningQuote, "=\"");
_LIT8(KClosingQuote, "\"");
_LIT8(KSpace," ");
/** 
 * Constructor
 * Copy CPushMessage pointer for reference during parse operations 
 */
CMessageComposer::CMessageComposer ( CPushMessage& aPushMessage )
				: iPushMessage ( aPushMessage )
	{
	iIsKeyValue = EFalse;
	}

/** 
* Destructor
* Delete the parser object and close the buffer which store DRM message.
*/
CMessageComposer::~CMessageComposer()
	{
	delete iParser;
	iXmlBuf.Close();
	}

/** 
*/
CMessageComposer* CMessageComposer::NewL( CPushMessage& aPushMessage)
	{
	CMessageComposer* self = new ( ELeave ) CMessageComposer ( aPushMessage );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ( self );
	return ( self );
	}

/** 
*/
void CMessageComposer::ConstructL()
	{
	iXmlBuf.CreateL(KXmlBufferlength);
	LoadPluginL ();
	}

/*
* Load the plugin based on incoming message (XML or WBXML plugin)
*/
void CMessageComposer::LoadPluginL ()
	{
	TPtrC contentType;
	iPushMessage.GetContentType ( contentType );
	iParser = CParser::NewL ( ( contentType.CompareF ( KDRMTextContentType ) ) ?
				KWbxmlParserDataType() : KXmlParserDataType(), *this );		
	}
	
/** 

*/
const TDesC8& CMessageComposer::ParseMessageL ()
	{
	// Begin parsing
	iParser->ParseBeginL ();
	
	// Get the message body and do parsing.
	TPtrC8 msgBody;
	if(iPushMessage.GetMessageBody ( msgBody ))
		{
		iParser->EnableFeature ( ERawContent );
		iParser->ParseL ( msgBody );
		}
	else
		{
		//Leave with KErrNotFound error code as message body not found
		User::Leave ( KErrNotFound );
		}	
	
	// End parsing
	iParser->ParseEndL ();
	
	return iXmlBuf;
	}

RStringPool& CMessageComposer::StringPool ()
	{
	return iParser->StringPool ();
	}

void CMessageComposer::OnStartDocumentL ( const Xml::RDocumentParameters&  /*aDocParam*/ , TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageComposer::OnEndDocumentL ( TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageComposer::OnStartElementL ( const Xml::RTagInfo& aElement, const Xml::RAttributeArray& aAttributes, TInt aErrorCode )
	{	
	User::LeaveIfError( MapToWapXMLError ( aErrorCode ) );
	
	if(aElement.LocalName().DesC().Compare(_L8("ds:KeyValue")) == 0)
		{
		iIsKeyValue = ETrue;
		}

	//Append '<' before the element
	AppendDataL ( KOpeningBracket );
	//Append the element
	AppendDataL ( aElement.LocalName().DesC() );
	
	//Now append all the attributes and attribute values in loop
	TInt nAttributes = aAttributes.Count();
	for ( TInt i = 0; i < nAttributes; ++i )
		{
		const RAttribute& attribute = aAttributes[i];
		const RTagInfo& nameInfo = attribute.Attribute();		
		//Append a blanck space before appending attribute name
		AppendDataL ( KSpace );
		//Append the attribute name
		AppendDataL ( nameInfo.LocalName().DesC() );
		//Append '="' before appending attribute value
		AppendDataL ( KOpeningQuote	);
		//Append attribue Value
		AppendDataL ( attribute.Value().DesC() );		
		//Append '"' after the attribute value
		AppendDataL ( KClosingQuote );
		}//End of for loop, appended all attribures of this element		
		//Append '>' after the element
		AppendDataL ( KClosingBracket );
	}

void CMessageComposer::OnEndElementL ( const Xml::RTagInfo&  aElement , TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );	

	AppendDataL ( KOpeningEndBracket );
	AppendDataL ( aElement.LocalName().DesC() );
	AppendDataL ( KClosingBracket );
	}

void CMessageComposer::OnContentL ( const TDesC8& aBytes, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	
	if(iIsKeyValue)
		{
		// Got to base64 encode the 128-bit encryption key.
		iIsKeyValue = EFalse;
		TBase64 codec;
		TInt length = aBytes.Length();
		// Calculate the length of the base64 encoded key and create a buffer of that length.
		HBufC8* encodedBytes = HBufC8::NewLC(((length%3)>0)?((length/3)+length+3):((length/3)+length+2));
		TPtr8 encodedBytesPtr(encodedBytes->Des());
		codec.Encode(aBytes, encodedBytesPtr);
		AppendDataL(*encodedBytes);
		CleanupStack::PopAndDestroy(encodedBytes);		
		}
	else
		{
		//Append the content.
		AppendDataL ( aBytes );
		}		
	}

void CMessageComposer::OnStartPrefixMappingL ( const RString& /* aPrefix */, const RString& /* aUri */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageComposer::OnEndPrefixMappingL ( const RString& /* aPrefix */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageComposer::OnIgnorableWhiteSpaceL ( const TDesC8& /* aBytes */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );	
	}

void CMessageComposer::OnSkippedEntityL ( const RString& /* aName */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageComposer::OnProcessingInstructionL ( const TDesC8& /* aTarget */, const TDesC8& /* aData */, TInt aErrorCode )
	{
	User::LeaveIfError ( MapToWapXMLError ( aErrorCode ) );
	}

void CMessageComposer::OnError ( TInt aErrorCode )
	{
	iLastError = MapToWapXMLError ( aErrorCode );
	}

TAny* CMessageComposer::GetExtendedInterface ( const TInt32 /* aUid */ )
	{
	return NULL;
	}

TInt CMessageComposer::LastError ()
	{
	return iLastError;
	}

void CMessageComposer::AppendDataL ( const TDesC8& aData )
	{
	TInt xmlBufMaxLen = iXmlBuf.MaxLength ();
	TInt xmlBufLen = iXmlBuf.Length ();
	TInt dataLen = aData.Length ();
	if ( xmlBufMaxLen < ( xmlBufLen + dataLen ) )
		{
		// Need to allocate more space.
		TInt lenToAllocate = (dataLen > KXmlBufferlength) ? dataLen : KXmlBufferlength;
		iXmlBuf.ReAllocL ( xmlBufMaxLen + lenToAllocate );
		}	
	iXmlBuf.Append ( aData );
	}
	
// Map the generic XML parser code to WAP XML errors. Description of the error code
// has taken from expat.
TInt CMessageComposer::MapToWapXMLError ( TInt aXmlErrorCode )
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
