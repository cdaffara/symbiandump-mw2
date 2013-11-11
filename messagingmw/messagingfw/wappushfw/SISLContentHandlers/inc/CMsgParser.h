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
	
#ifndef __CMSGPARSER_H
#define __CMSGPARSER_H

#include <e32base.h>
#include <xml/contenthandler.h>
#include <xml/parser.h>
#include <push/pushmessage.h>

class MWAPElementHandler
	{
	public:
	virtual void HandleElementL ( const RString& aTag, const RString& aAttributeName, const RString& aAttributeValue ) = 0;
	virtual void HandleContentL ( const TDesC8& aBytes ) = 0;
	};

 /**
 TODO
 */
//##ModelId=3B659EE90381
class CMessageParser : public CBase, public Xml::MContentHandler
/**
@internalTechnology
@released
*/
	{
public:
	//##ModelId=3B659EEA0032
	static CMessageParser* NewL ( CPushMessage& aPushMsg, 
								MWAPElementHandler& aElementHandler,
								const TStringTable* aTagsTable,
								const TStringTable* aAttributeTable,
								const TStringTable* aAttributeValueTable );	
	/** parses and validates and returns error code */
	//##ModelId=3B659EEA0028
	TInt ParseMessageL ();	

	//##ModelId=3B659EEA0027
	~CMessageParser ();
	
	RStringPool& StringPool ();
	
	// Methods from Xml::MContentHandler
	void OnStartDocumentL ( const Xml::RDocumentParameters &aDocParam, TInt aErrorCode );
	void OnEndDocumentL ( TInt aErrorCode );	
	void OnStartElementL ( const Xml::RTagInfo &aElement, const Xml::RAttributeArray &aAttributes, TInt aErrorCode );
	void OnEndElementL ( const Xml::RTagInfo &aElement, TInt aErrorCode );
	void OnContentL ( const TDesC8 &aBytes, TInt aErrorCode );
	void OnStartPrefixMappingL ( const RString &aPrefix, const RString &aUri, TInt aErrorCode );
	void OnEndPrefixMappingL ( const RString &aPrefix, TInt aErrorCode );
	void OnIgnorableWhiteSpaceL ( const TDesC8 &aBytes, TInt aErrorCode );
	void OnSkippedEntityL ( const RString &aName, TInt aErrorCode );
	void OnProcessingInstructionL ( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode );
	void OnError ( TInt aErrorCode );
	TAny* GetExtendedInterface ( const TInt32 aUid );
	
	TInt LastError ();	
private:
	//##ModelId=3B659EE903D3
	CMessageParser( CPushMessage& aPushMsg, 
					MWAPElementHandler& aElementHandler,
					const TStringTable* aTagsTable,
					const TStringTable* aAttributeTable,
					const TStringTable* aAttributeValueTable );
	//##ModelId=3B659EE903D2
	void ConstructL ();
	//##ModelId=3B659EE903D1	
	void LoadPluginL ();
	
	void CheckTagInTableL ( const RString& aTag );
	void CheckAttributeValuePairL ( const RString& aAttribute, const RString& aAttributeValue );
 	
	TInt MapToWapXMLError ( TInt aXmlErrorCode );
	
private:
	//##ModelId=3B659EE9038F

	CPushMessage&				iPushMessage;	// reference
	MWAPElementHandler&			iElementHandler;

	Xml::CParser*				iParser;
	
	// Pointer to the static string pool table for tags, attribute and attribute value
	// We don't own this
	const TStringTable*			iTagsTable;
	const TStringTable*			iAttributeTable;
	const TStringTable*			iAttributeValueTable;
	
	// Tag position in the XML document and the mapping in the tags string table.
	TInt 						iTagPosition; 
	
	TInt iLastError;
	};


#endif // __CMSGPARSER_H
