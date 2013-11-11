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

// Local includes
//
#include "ROAPTContentHandler.h"
#include "MessageComposer.h"

//Rights object text MIME types
_LIT8(KROAPTTextContentType, "application/vnd.oma.drm.roap-trigger+xml");
_LIT(KROAPTBinContentType, "application/vnd.oma.drm.roap-trigger+wbxml");

#if defined(_DEBUG)
_LIT(KErrPushMsgNull,	"NULL CPushMessage");
#endif

//temp output file
_LIT(KWapPushDir, "C:\\system\\data\\wappush\\");
_LIT(KTempROAPTFile, "roaptrigger.Dat");

// Constants
_LIT(KReserved, "Reserved");

const TInt KXMLHeader=12;

void CROAPTContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CROAPTContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/**
 * The ROAP Trigger Content handler private constructor.
 */ 
CROAPTContentHandler::CROAPTContentHandler(): CContentHandlerBase(), iPushMsgAction(KErrNotFound)
	{
	//Do nothing
	}

/**
 *  This will complete initialization of the object
 */
void CROAPTContentHandler::ConstructL()
	{
	CActiveScheduler::Add(this);
	User::LeaveIfError(iFs.Connect());		
	}

/**
 * Static Factory Construction
 *
 * Version of NewL which leaves nothing
 * on the cleanup stack
 */
CROAPTContentHandler* CROAPTContentHandler::NewL()
	{
	CROAPTContentHandler* self = new(ELeave) CROAPTContentHandler;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Default d'tor
 */
CROAPTContentHandler::~CROAPTContentHandler()
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: Destructor Called"); 
	delete iCAFInterface;
	delete iData;
	}

/**
 * HandleMessage Async. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *	@param aPushMsg
 *		CPushMessage to process
 *	@param aStatus
 *		request status variable for use in asynchronous operations
 */
void CROAPTContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: HandleMessage Async Func. Called"); 
	__ASSERT_DEBUG( aPushMsg != NULL, User::Panic(KErrPushMsgNull, KErrNone));

	iMessage = aPushMsg;
	SetConfirmationStatus(aStatus);

	iState= EConverting;
	IdleComplete();
	}

/**
 * HandleMessage Sync. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *
 *	Initial State: Set data members then go to the next state 
 *	@param aPushMsg
 *		CPushMessage to process
 */
void CROAPTContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: HandleMessage Sync Func. Called");
	__ASSERT_DEBUG( aPushMsg != NULL, User::Panic(KErrPushMsgNull, KErrNone));
	
	iMessage = aPushMsg;

	iState =EConverting;
	IdleComplete();
	}

/** 
 *	Convert the ROAP Trigger message from WBXML to XML if necessary
 *	@leave KErrNotFound
 *		indicates general processing error and to abandon processing
 *	@leave KErrCorrupt
 *		there is no body in the message
 *	@leave TDesC::AllocL
 
 */
void CROAPTContentHandler::ConvertPushMsgL()
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: ConvertPushMsgL Called"); 

	TPtrC contentType;
	iMessage->GetContentType(contentType);
	if(contentType.CompareF(KROAPTBinContentType) == 0)
		{
		//This is a WBXML Push Msg, we need to convert it into XML format before passing to DRM agent
		CMessageComposer* msgComposer = CMessageComposer::NewL ( *iMessage );
		CleanupStack::PushL ( msgComposer );
		const TDesC8& convertedMsg = msgComposer->ParseMessageL ();
		User::LeaveIfError ( msgComposer->LastError() );
		HBufC8* msgBody = convertedMsg.AllocLC();
		TPtrC8 hdr;
		iMessage->GetHeader(hdr);
		HBufC8* hdrBuf = hdr.AllocLC();
		delete iMessage;
		iMessage = NULL;
		
		//CPushMessage now takes ownership of hdrBuf and convertedBody
		//replace received Binary Push message with newly converted XML Massage
		iMessage = CPushMessage::NewL(hdrBuf, msgBody);

		CleanupStack::Pop(2,msgBody); //msgBody, hdrBuf
		CleanupStack::PopAndDestroy( msgComposer );
		}
	iState = EStoring;
	IdleComplete();
	}


/** 
 *	Save push message in message store.
 * 	Creates link to message service provided by wap push utils
 *	and uses this and the associated push entry class to save
 *	the push message.
 */
void CROAPTContentHandler::SaveROAPTToXMLFileL() 
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: SaveROAPTToXMLFile called");

	TPtrC8 body;
	iMessage->GetMessageBody(body);
	// If there is no body in the message leave with an error
	if (body.Length() ==0)
		{
		User::Leave(KErrCorrupt);
		}
		
	HBufC8* bodyBuf = body.AllocLC();
	//Create file to store the message in Wap Push temp area 
	TFileName roaptfilename;
	RFile file;
	roaptfilename.Append(KWapPushDir);
	roaptfilename.Append(KTempROAPTFile);
	iFs.MkDirAll(KWapPushDir);
	User::LeaveIfError(file.Replace(iFs, roaptfilename, EFileWrite));
	CleanupClosePushL(file);

	//Create a heap buffer to store message with header.
	//Agent expects the 12 bytes of header to store the message.
	//First four bytes to store the offset of the rights message size in the buffer.
	//Next four bytes to store the offset of the content message size in the buffer.
	//Last four bytes to store the size of rights message.
	TInt size=bodyBuf->Length();
	iData=HBufC8::NewL(size+KXMLHeader);
	TPtr8 bufPtr(iData->Des());
	TPckg<TInt> rightsOffset(8);	//There is no content. the rights message size is at offset of 8 bytes.
	bufPtr.Append(rightsOffset);
	TPckg<TInt> contentOffset(0);
	bufPtr.Append(contentOffset);	//No content. Hence set it to zero.
	TPckg<TInt> rightsSize(size);
	bufPtr.Append(rightsSize);		//Append the size of the rights message
	//Append the xml message
	bufPtr.Append(*bodyBuf);
	
	//Now write this buffer to file
	User::LeaveIfError(file.Write(*iData));
	CleanupStack::PopAndDestroy(&file);
	CleanupStack::PopAndDestroy(bodyBuf);
	
	iState = ECAFInterface;
	IdleComplete();
	}

void CROAPTContentHandler::ProcessToCAFL() 
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: ProcessToCAF called");

	iCAFInterface = new(ELeave) CROCHCAFInterface(iFs);

	//Register with the CAF session - this initialises the generic variables

	iCAFInterface->RegisterL(KROAPTTextContentType);
	
	if(!iCAFInterface->Registered())
		{
		//No CAF agent registered which can handle this mime type.
		User::Leave(KErrNotFound);
		}
	
	//Initialise the CAF session - this initialises the interface variables and import file
	iCAFInterface->PrepareProcessingL();
	//Then start the CAF processing
	if(iCAFInterface->Processing())
		{
		//Write the data to CAF
		iCAFInterface->WriteDataL(*iData);
		iCAFInterface->EndProcessingL();
		}
	
	iState = EDeleteFile;
	IdleComplete();	
	}

void CROAPTContentHandler::DeleteTempROAPTFile()
	{
	TFileName roaptFileName;
	roaptFileName.Append(KWapPushDir);
	roaptFileName.Append(KTempROAPTFile);
	iFs.Delete(roaptFileName);
	iState = EDone;
	IdleComplete();
	}

/** 
 *	Same functionality as DoCancel()
 */
void CROAPTContentHandler::CancelHandleMessage()
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: CancelHandleMessage called");
	Complete(KErrCancel);
	}

/** 
 *	Cancels the handling of the message and revokes the active status
 *	of the handler 
 */
void CROAPTContentHandler::DoCancel()
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: DoCancel Called");
	Complete(KErrCancel);	
	}

/** 
 *	Step through the various representative states for handling a message 
 *	1. Conversion of WBXML if necessary
 *	2. Store the xml file
 *	3. Pass file to CAF
 *	4. Delete File
 *	5. Complete
 */
void CROAPTContentHandler::RunL()
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: RunL Called");
	

	// use active state machine routine to manage activites:
	switch (iState)
		{
	case EConverting : 	
		ConvertPushMsgL();
		break;
	case EStoring:
		SaveROAPTToXMLFileL();
		break;
	case ECAFInterface:
		ProcessToCAFL();
		break;
	case EDeleteFile:		
		DeleteTempROAPTFile();
		break;	
	case EDone:
		Complete(KErrNone);
		break;
	default:
		break;
		}
	}

/** 
 * CPluginActiveBase methods
 */
TInt CROAPTContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CROAPTContentHandler:: RunError Called"); 
	iState=EDone;
	Complete(aError);
	return KErrNone;
	}
