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
	
#ifndef __WBXMLCONVERTER_H
#define __WBXMLCONVERTER_H

// System Include
#include <e32base.h>
#include <xml/contenthandler.h>
#include <xml/parser.h>
#include <push/pushmessage.h>


/** 
Class for parsing the XML document and handles the callbacks from generic XML parser
@internalTechnology
@released
 */
class CMessageComposer : public CBase, public Xml::MContentHandler
	{
public:
	static CMessageComposer* NewL ( CPushMessage& aPushMsg);
	
	/** parses and validates and returns error code */
	const TDesC8& ParseMessageL ();	

	~CMessageComposer ();
	
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
	CMessageComposer( CPushMessage& aPushMsg);
	void ConstructL ();
	void LoadPluginL ();
	TInt MapToWapXMLError ( TInt aXmlErrorCode );
	void AppendDataL ( const TDesC8& aData );
private:
	CPushMessage&				iPushMessage;	// reference
	Xml::CParser*				iParser;
	TInt iLastError;
	RBuf8 iXmlBuf;
	TBool iIsKeyValue;
	};


#endif // MessageComposer.h
