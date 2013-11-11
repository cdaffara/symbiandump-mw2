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

#include <e32std.h>
#include <ecom/implementationproxy.h>

#include <xml/plugins/dictionarycodepage.h>

#include <xml/xmlframeworkerrors.h>

#include "Sl1_0StringDict00.h"
#include "sl1_0stringdict00tagtable.h"
#include "sl1_0stringdict00attributetable.h"
#include "sl1_0stringdict00attributevaluetable.h"

using namespace Xml;

MStringDictionary* CSl1_0StringDict00::NewL(TAny* aStringPool)
	{
	CSl1_0StringDict00* self = new(ELeave) CSl1_0StringDict00(reinterpret_cast<RStringPool*>(aStringPool));
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return (static_cast<MStringDictionary*>(self));
	}



CSl1_0StringDict00::CSl1_0StringDict00(RStringPool* aStringPool)
:	iStringPool(*aStringPool)
	{
	// do nothing;
	}



void CSl1_0StringDict00::ConstructL()
	{
	// we don't own this stringpool
	iStringPool.OpenL(Sl1_0StringDict00TagTable::Table);
	iStringPool.OpenL(Sl1_0StringDict00AttributeTable::Table);
	iStringPool.OpenL(Sl1_0StringDict00AttributeValueTable::Table);

	iCodepage00Table = CDictionaryCodePage::NewL(
		&Sl1_0StringDict00TagTable::Table, 
		&Sl1_0StringDict00AttributeTable::Table, 
		&Sl1_0StringDict00AttributeValueTable::Table, 
		0); // codepage

	// Construct the correlation mapping
	iCodepage00Table->ConstructIndexMappingL(iTagCodePage00, CDictionaryCodePage::EStringTypeElement);
	iCodepage00Table->ConstructIndexMappingL(iAttributeCodePage00, CDictionaryCodePage::EStringTypeAttribute);
	iCodepage00Table->ConstructIndexMappingL(iAttributeValueCodePage00, CDictionaryCodePage::EStringTypeAttributeValue);
	}




void CSl1_0StringDict00::Release()
	{
	delete (this);
	}




CSl1_0StringDict00::~CSl1_0StringDict00()
	{
	delete iCodepage00Table;
	}



void CSl1_0StringDict00::ElementL(TInt aToken, RString& aElement) const
	{
	TInt index = iCodepage00Table->StringPoolIndexFromToken(
					aToken, CDictionaryCodePage::EStringTypeElement);

	if (index == KErrXmlStringPoolTableNotFound)
		{
		User::Leave(KErrXmlUnsupportedElement);
		}

	// when we have multiple codepages per string dictionary we'd do something like iCodepageTable[n]->StringTable()
	aElement = iStringPool.String(
		index,
		*(iCodepage00Table->StringTable(CDictionaryCodePage::EStringTypeElement)));
	}



void CSl1_0StringDict00::AttributeL(TInt aToken, RString& aAttribute) const
	{
	TInt index = iCodepage00Table->StringPoolIndexFromToken(
					aToken, CDictionaryCodePage::EStringTypeAttribute);

	if (index == KErrXmlStringPoolTableNotFound)
		{
		User::Leave(KErrXmlUnsupportedAttribute);
		}

	// when we have multiple codepages per string dictionary we'd do something like iCodepageTable[n]->StringTable()
	aAttribute = iStringPool.String(
		index,
		*(iCodepage00Table->StringTable(CDictionaryCodePage::EStringTypeAttribute)));
	};



void CSl1_0StringDict00::AttributeValuePairL(TInt aToken, RString& aAttribute, RString& aValue) const
	{
	AttributeL(aToken, aAttribute);
	AttributeValueL(aToken, aValue);
	};
	


void CSl1_0StringDict00::AttributeValueL(TInt aToken, RString& aValue) const
	{
	TInt index = iCodepage00Table->StringPoolIndexFromToken(
					aToken, CDictionaryCodePage::EStringTypeAttributeValue);

	if (index == KErrXmlStringPoolTableNotFound)
		{
		User::Leave(KErrXmlUnsupportedAttributeValue);
		}

	// when we have multiple codepages per string dictionary we'd do something like iCodepageTable[n]->StringTable()
	aValue = iStringPool.String(
		index,
		*(iCodepage00Table->StringTable(CDictionaryCodePage::EStringTypeAttributeValue)));
	};



TBool CSl1_0StringDict00::CompareThisDictionary(const RString& aDictionaryDescription) const
	{
	// If this string dictionary has many codepages then all these comparisons should go here.
	// Remember, the string dictionary loads up all the RStringTables into its RStringPool
	// on construction. So if the comparison fails we do not have it.
	return (
		(aDictionaryDescription == iStringPool.String(Sl1_0StringDict00TagTable::EUri, Sl1_0StringDict00TagTable::Table))		||
		(aDictionaryDescription == iStringPool.String(Sl1_0StringDict00TagTable::EPublicId, Sl1_0StringDict00TagTable::Table))||
		(aDictionaryDescription == iStringPool.String(Sl1_0StringDict00TagTable::EFormalPublicId, Sl1_0StringDict00TagTable::Table)));
	}



TInt CSl1_0StringDict00::SwitchCodePage(TInt aCodePage)
	{
	// We only have one codepage sp can't switch
	if (aCodePage != iCodePage)
		{
		return KErrXmlMissingStringDictionary;
		}
	return iCodePage;
	}

		


		
/**
This method obtains the public identifier from the StringTable.
Either the formal or non formal public id will do.
The stringDictionary .rss files must list both these as wbxml
documents have one or the other.

@param				aPubId The public identifier for this string 
					dictionary.
*/

void CSl1_0StringDict00::PublicIdentifier(RString& aPubId)
	{
	aPubId = iStringPool.String(
		Sl1_0StringDict00TagTable::EFormalPublicId,
		*(iCodepage00Table->StringTable(CDictionaryCodePage::EStringTypeElement)));
	}


/**
The element types in the Device Information DTD are defined within
a namespace associated with the Uri/Urn available from the StringTable.
The RString need not be closed, but closing is harmless. 

@param				aUri The associated namespace for this string 
					dictionary.
*/

void CSl1_0StringDict00::NamespaceUri(RString& aUri)
	{
	aUri = iStringPool.String(
		Sl1_0StringDict00TagTable::EUri,
		*(iCodepage00Table->StringTable(CDictionaryCodePage::EStringTypeElement)));
	}



const TInt KUidSLStringDictPlugin = 0x10282652;

// __________________________________________________________________________
// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] = {
	IMPLEMENTATION_PROXY_ENTRY(KUidSLStringDictPlugin, CSl1_0StringDict00::NewL)
};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

#ifndef EKA2
/** DLL Entry point */
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
#endif
