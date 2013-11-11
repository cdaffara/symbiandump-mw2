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

#ifndef __DRMSTRINGDICT00_H__
#define __DRMSTRINGDICT00_H__

#include <e32std.h>
#include <stringpool.h>
#include <xml/plugins/stringdictionary.h>


class DRMStringDict00 : public CBase, public Xml::MStringDictionary
/**
The DRMStringDict00 class is a derived class defining the API of a single string dictionary. 

Derived classes must add all elements, attributes names, and attribute values that belong
to this string dictionary as static string tables to the StringPool. WBXML token mappings between 
strings and WBXML tokens should also be maintained.

@internalTechnology
@prototype This is the first drop of this component.
*/
	{
public:
	static Xml::MStringDictionary* NewL(TAny* aStringPool);
	virtual ~DRMStringDict00();
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
		// -//OMA//DTD DRMREL 1.0//EN
		ERights 						= 0x05,
		EContext						= 0x06,
		EVersion						= 0x07,
		EUid							= 0x08,
		EAgreement						= 0x09,
		EAsset							= 0x0A,
		EKeyInfo						= 0x0B,
		EKeyValue						= 0x0C,
		EPermission						= 0x0D,
		EPlay							= 0x0E,
		EDisplay						= 0x0F,
		EExecute						= 0x10,
		EPrint							= 0x11,
		EConstraint						= 0x12,
		ECount							= 0x13,
		EDateTime						= 0x14,
		EStart							= 0x15,
		EEnd							= 0x16,
		EInterval						= 0x17
	};

	enum TAttributeToken {
		// CodePage 00
		// -//OMA//DTD DRMREL 1.0//EN
		EAttributeEx 					= 0x05,
		EAttributeDd 					= 0x06,
		EAttributeDs					= 0x07
	};

	enum TAttributeValueToken {
		// CodePage 00
		// -//OMA//DTD DRMREL 1.0//EN
		EAttributeValueEx			 		= 0x85,
		EAttributeValueDd 					= 0x86,
		EAttributeValueDs					= 0x87
	};



private:

	DRMStringDict00(RStringPool* aStringPool);

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
	{DRMStringDict00::ERights,
	 DRMStringDict00::EContext,
	 DRMStringDict00::EVersion,
	 DRMStringDict00::EUid,
	 DRMStringDict00::EAgreement,
	 DRMStringDict00::EAsset,
	 DRMStringDict00::EKeyInfo,
	 DRMStringDict00::EKeyValue,
	 DRMStringDict00::EPermission,
	 DRMStringDict00::EPlay,
	 DRMStringDict00::EDisplay,
	 DRMStringDict00::EExecute,
	 DRMStringDict00::EPrint,
	 DRMStringDict00::EConstraint,
	 DRMStringDict00::ECount,
	 DRMStringDict00::EDateTime,
	 DRMStringDict00::EStart,
	 DRMStringDict00::EEnd,
	 DRMStringDict00::EInterval,
	 0}; // ok to end with zero as this is used by a global token


const TInt iAttributeCodePage00[] = 
	{DRMStringDict00::EAttributeEx,
	 DRMStringDict00::EAttributeDd,
	 DRMStringDict00::EAttributeDs,
	 0}; // ok to end with zero as this is used by a global token



const TInt iAttributeValueCodePage00[] = 
	{DRMStringDict00::EAttributeValueEx,
	 DRMStringDict00::EAttributeValueDd,
	 DRMStringDict00::EAttributeValueDs,
	 0}; // ok to end with zero as this is used by a global token


#endif //__DRMSTRINGDICT00_H__
