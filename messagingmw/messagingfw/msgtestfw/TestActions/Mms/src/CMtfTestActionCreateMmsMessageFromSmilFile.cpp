// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CreateMmsMessageFromSmilFile
// [Action Parameters]
// Session                <input>: Reference to the session.
// ServiceId              <input>: Value of the MMS service id.
// FolderId               <input>: Value of the local folder id where the message will be created.
// (ConfigIndexSmilFile)  <input>: Value of config index for Smil file. Default is 0.
// (ConfigIndexHeader)	  <input>: Value of config index for MMS header settings. Default is 0.
// MessageId             <output>: Value of the created MMS message id.
// [Action Description]
// Creates a MMS message on one of the local folders.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::Entry
// TMsvId::Id
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateMmsMessageFromSmilFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <msvuids.h>
#include <MmsClientMessage.h>
#include <MmsMediaObject.h>
#include <GmXmlDocument.h>
#include <GmXmlNode.h>
#include <GmXmlElement.h>


_LIT(KFrom,		"From");
_LIT(KTo,		"To");
_LIT(KSubject,	"Subject");
_LIT(KCc,		"Cc");
_LIT(KBcc,		"Bcc");

_LIT(KSmilImg,			"img");
_LIT(KSmilText,			"text");

_LIT(KMimeApplicationSmil,	"application/smil");
_LIT(KMimeImageGif,			"image/gif");
_LIT(KMimeTextPlain,		"text/plain");

_LIT(KAttributeNameSrc,	"src");


class CTraverseNode : public CBase
	{
public:
	CTraverseNode(CMDXMLNode& aNode,CMmsClientMessage& aMmsMessage) : iNode(aNode), iMmsMessage(aMmsMessage) {};
   ~CTraverseNode() {};

	void Start();

private:
	CMDXMLNode& iNode;
	CMmsClientMessage& iMmsMessage;
	};


void CTraverseNode::Start()
	{
	CMDXMLNode::TDOMNodeType nodeType = iNode.NodeType();
	if (nodeType == CMDXMLNode::EElementNode)
		{
		CMDXMLElement& element = reinterpret_cast<CMDXMLElement&>(iNode);
		TPtrC nodeName = element.NodeName();
		TPtrC attributeValue;
		if (!nodeName.Compare(KSmilImg))
			{
			if (element.GetAttribute(KAttributeNameSrc,attributeValue) == KErrNone)
				{
				iMmsMessage.CreateMediaObjectL(EFalse,KMimeImageGif,attributeValue);
				iMmsMessage.StoreL();
				}
			}
		else if (!nodeName.Compare(KSmilText))
			{
			if (element.GetAttribute(KAttributeNameSrc,attributeValue) == KErrNone)
				{
				iMmsMessage.CreateMediaObjectL(EFalse,KMimeTextPlain,attributeValue);
				iMmsMessage.StoreL();
				}
			}
		}

	if (iNode.HasChildNodes())
		{
		for (CMDXMLNode* node = iNode.FirstChild(); node; node = node->NextSibling())
			{
			CTraverseNode* traverseNode = new(ELeave) CTraverseNode(*node,iMmsMessage);
			traverseNode->Start();
			delete traverseNode;
			} 
		}
	}


CMtfTestAction* CMtfTestActionCreateMmsMessageFromSmilFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateMmsMessageFromSmilFile* self = new (ELeave) CMtfTestActionCreateMmsMessageFromSmilFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateMmsMessageFromSmilFile::CMtfTestActionCreateMmsMessageFromSmilFile(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCreateMmsMessageFromSmilFile::~CMtfTestActionCreateMmsMessageFromSmilFile()
	{
	delete iParserObserver;
	delete iXmlParser;
	delete iConfigParser;
	}


void CMtfTestActionCreateMmsMessageFromSmilFile::ExecuteActionL()
	{
	TInt paramConfigIndexSmilFile = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3),0);
	TPtrC SmilFileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmilFile,paramConfigIndexSmilFile);

	iFs.Connect();
	
	iStatus = KRequestPending;
	iSmilDtd = CSMILDtd::NewL();
	iParserObserver = new(ELeave) XmlParserObserver(&iStatus);
	iXmlParser = CMDXMLParser::NewL(iParserObserver,iSmilDtd);
	User::LeaveIfError(iXmlParser->ParseFile(iFs,SmilFileName));

	CActiveScheduler::Add(this);
	SetActive();
	}


void CMtfTestActionCreateMmsMessageFromSmilFile::DoCancel()
	{
	iXmlParser->Cancel();
	}


void CMtfTestActionCreateMmsMessageFromSmilFile::RunL()
	{
	iFs.Close();

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramFolderId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));
	TInt paramConfigIndexSmilFile = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3),0);
	TInt paramConfigIndexHeader = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4),0);
		   
	TPtrC SmilFileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmilFile,paramConfigIndexSmilFile);
	TPtrC headerFileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfMmsHeader,paramConfigIndexHeader);

	iConfigParser = CMtfTestActionUtilsConfigFileParser::NewL(headerFileName);

	TMsvEntry indexEntry;
	indexEntry.iType = KUidMsvMessageEntry;
	indexEntry.iMtm = KUidMsvLocalServiceMtm;
	indexEntry.iServiceId = paramServiceId;

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramFolderId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);

	entry->SetEntryL(paramFolderId);
	entry->CreateL(indexEntry);


	TMsvId paramMessageId = indexEntry.Id();

    CMmsClientMessage* mmsMessage = CMmsClientMessage::NewL(*paramSession,paramMessageId);
	CleanupStack::PushL(mmsMessage);

    CMmsHeaders& headers = mmsMessage->Headers();
	TPtrC from;      
	User::LeaveIfError(iConfigParser->GetFieldAsString(KFrom,from));
    headers.SetFromL(from);
	TPtrC to;          	
	User::LeaveIfError(iConfigParser->GetFieldAsString(KTo,to));
    headers.AddRecipientL(CMmsHeaders::ETo,to);
	TPtrC subject;	
	TInt err = iConfigParser->GetFieldAsString(KSubject,subject);
	if (err != KErrNotFound)
	    headers.SetSubjectL(subject);
	TPtrC cc;          	
	err = iConfigParser->GetFieldAsString(KCc,cc);
	if (err != KErrNotFound)
		headers.AddRecipientL(CMmsHeaders::ECc,cc);
	TPtrC bcc;        	
	err = iConfigParser->GetFieldAsString(KBcc,bcc);
	if (err != KErrNotFound)
	    headers.AddRecipientL(CMmsHeaders::EBcc,bcc);

    mmsMessage->CreateMediaObjectL(ETrue,KMimeApplicationSmil,SmilFileName);
	mmsMessage->StoreL();

	CMDXMLDocument* domTree = iXmlParser->DetachXMLDoc();	
	CleanupStack::PushL(domTree);
	CMDXMLElement* rootElement = domTree->DocumentElement();
	CTraverseNode* traverseNode = new(ELeave) CTraverseNode(*rootElement,*mmsMessage);
	traverseNode->Start();
	delete traverseNode;
	CleanupStack::PopAndDestroy(domTree);

	CleanupStack::PopAndDestroy(mmsMessage);
	CleanupStack::PopAndDestroy(entry);

	StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(5));

	TestCase().ActionCompletedL(*this);
	}

