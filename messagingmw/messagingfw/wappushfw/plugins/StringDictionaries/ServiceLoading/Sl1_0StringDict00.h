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
// Plugin interface
// 
//

#ifndef __SLSTRINGDICT00_H__
#define __SLSTRINGDICT00_H__

#include <e32std.h>
#include <stringpool.h>
#include <xml/plugins/stringdictionary.h>


class CSl1_0StringDict00 : public CBase, public Xml::MStringDictionary
/**
The Sl1_0StringDict00 class is a derived class defining the API of a single string dictionary. 

Derived classes must add all elements, attributes names, and attribute values that belong
to this string dictionary as static string tables to the StringPool. WBXML token mappings between 
strings and WBXML tokens should also be maintained.

@internalTechnology
@prototype This is the first drop of this component.
*/
	{
public:
	static Xml::MStringDictionary* NewL(TAny* aStringPool);
	virtual ~CSl1_0StringDict00();
	void ConstructL();

	void ElementL(TInt aToken, RString& aElement) const;
	void AttributeL(TInt aToken, RString& aAttribute) const;
	void AttributeValuePairL(TInt aToken, RString& aAttribute, RString& aValue) const;
	void AttributeValueL(TInt aToken, RString& aValue) const;

	TBool CompareThisDictionary(const RString& aDictionaryDescription) const;

	TInt SwitchCodePage(TInt aCodePage);
	void PublicIdentifier(RString& aPubId);
	void NamespaceUri(RString& aUri);

	void Release();

public:

	// CodePage tokens.
	//
	// Note: There will be correlation between these token values and their
	// strings via the CDictionaryCodePage using the RStringPool.

	enum TTagToken {
		// CodePage 00
		// -//WAPFORUM//DTD SL 1.0//EN
		ETagTokenSl 						= 0x05
	};


	enum TAttributeToken {
		// CodePage 00
		// -//WAPFORUM//DTD SL 1.0//EN
		EAttributeTokenAction1 				= 0x05,
		EAttributeTokenAction2 				= 0x06,
		EAttributeTokenAction3 				= 0x07,
		EAttributeTokenHref1 				= 0x08,
		EAttributeTokenHref2 				= 0x09,
		EAttributeTokenHref3 				= 0x0A,
		EAttributeTokenHref4 				= 0x0B,
		EAttributeTokenHref5 				= 0x0C,
	};



	enum TAttributeValueToken {
		// CodePage 00
		// -//WAPFORUM//DTD SL 1.0//EN
		EAttributeValueTokenAction1 		= 0x05,
		EAttributeValueTokenAction2 		= 0x06,
		EAttributeValueTokenAction3 		= 0x07,
		EAttributeValueTokenHref1			= 0x08,
		EAttributeValueTokenHref2 			= 0x09,
		EAttributeValueTokenHref3 			= 0x0A,
		EAttributeValueTokenHref4 			= 0x0B,
		EAttributeValueTokenHref5 			= 0x0C,
		EAttributeValueTokenCom 			= 0x85,
		EAttributeValueTokenEdu 			= 0x86,
		EAttributeValueTokenNet 			= 0x87,
		EAttributeValueTokenOrg 			= 0x88,
	};



private:

	CSl1_0StringDict00(RStringPool* aStringPool);

private:

/**
The StringPool for this string dictionary.
We don't own this.
*/
	RStringPool				iStringPool;

	Xml::CDictionaryCodePage*	iCodepage00Table;

	TInt					iCodePage;

	};


const TInt iTagCodePage00[] = 
	{CSl1_0StringDict00::ETagTokenSl,
	 0}; // ok to end with zero as this is used by a global token


const TInt iAttributeCodePage00[] = 
	{CSl1_0StringDict00::EAttributeTokenAction1,
	 CSl1_0StringDict00::EAttributeTokenAction2,
	 CSl1_0StringDict00::EAttributeTokenAction3,
	 CSl1_0StringDict00::EAttributeTokenHref1,
	 CSl1_0StringDict00::EAttributeTokenHref2,
	 CSl1_0StringDict00::EAttributeTokenHref3,
	 CSl1_0StringDict00::EAttributeTokenHref4,
	 CSl1_0StringDict00::EAttributeTokenHref5,
	 0}; // ok to end with zero as this is used by a global token



const TInt iAttributeValueCodePage00[] = 
	{CSl1_0StringDict00::EAttributeValueTokenAction1,
	 CSl1_0StringDict00::EAttributeValueTokenAction2,
	 CSl1_0StringDict00::EAttributeValueTokenAction3,
	 CSl1_0StringDict00::EAttributeValueTokenHref1,
	 CSl1_0StringDict00::EAttributeValueTokenHref2,
	 CSl1_0StringDict00::EAttributeValueTokenHref3,
	 CSl1_0StringDict00::EAttributeValueTokenHref4,
	 CSl1_0StringDict00::EAttributeValueTokenHref5,
	 CSl1_0StringDict00::EAttributeValueTokenCom,
	 CSl1_0StringDict00::EAttributeValueTokenEdu,
	 CSl1_0StringDict00::EAttributeValueTokenNet,
	 CSl1_0StringDict00::EAttributeValueTokenOrg,
	 0}; // ok to end with zero as this is used by a global token


#endif //__SLSTRINGDICT00_H__
