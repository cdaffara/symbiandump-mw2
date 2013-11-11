// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CreateSmtpMessageFromParamFile
// [Action Parameters]
// Session					<input>: Reference to the session.
// ServiceId				<input>: Value of the SMTP service id.
// FolderId					<input>: Value of the local folder id where the message will be created.
// mailInfo					<input>: Reference to a CPop3MailInfo object containing mail info
// MessageId				<output>: Value of the created SMTP message id.
// [Action Description]
// Creates a SMTP message on one of the local folders.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::Entry
// TMsvId::Id
// CImCltRecvConvert::SetMsvId
// CImCltRecvConvert::ResetL
// CImCltRecvConvert::MessageCompleteL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <msvapi.h>
#include <miutset.h>

#include "CMtfTestActionCreateSmtpMessageFromParamEmailFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CPop3MailInfo.h"
#include "ImCltCvRecv.h"


CMtfTestAction* CMtfTestActionCreateSmtpMessageFromParamEmailFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmtpMessageFromParamEmailFile* self = 
		new (ELeave) CMtfTestActionCreateSmtpMessageFromParamEmailFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSmtpMessageFromParamEmailFile::CMtfTestActionCreateSmtpMessageFromParamEmailFile(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSmtpMessageFromParamEmailFile::~CMtfTestActionCreateSmtpMessageFromParamEmailFile()
	{
	}


#include <mtuireg.h>
#include <msvstd.h>
#include <mtmuibas.h>


TMsvId CMtfTestActionCreateSmtpMessageFromParamEmailFile::CreateAndSendMessageL( CPop3MailInfo &mailInfo )
	{
	CSendAs* sendAs = CSendAs::NewL( *this ); 
	CleanupStack::PushL( sendAs );
    
	sendAs->SetMtmL( KUidMsgTypeSMTP );
	sendAs->SetService(0);
	sendAs->CreateMessageL(KMsvGlobalOutBoxIndexEntryId); 


	// these next 2 literals will come from mail info params eventually
	_LIT( KRecipient, "matthewf@msexchange2k.closedtest.intra" );
	sendAs->AddRecipientL( KRecipient );

	_LIT( KSubject, "test subject" );
	sendAs->SetSubjectL( KSubject );

	//Create and read the file data int a CRichText object
	TCharFormat charForm; 
	TCharFormatMask charFormatMask; 
	charFormatMask.SetAll();

	CParaFormatLayer* paraFormat = CParaFormatLayer::NewL( ); 
	CleanupStack::PushL( paraFormat );
	CCharFormatLayer* charFormat = CCharFormatLayer::NewL( charForm, charFormatMask ); 
	CleanupStack::PushL( charFormat );
	CRichText* rText = CRichText::NewL( paraFormat, charFormat );
	CleanupStack::PushL( rText );

//
	RFs fs;
	fs.Connect();
	RFile file;
	mailInfo.GetFileName();
	User::LeaveIfError( file.Open(fs, mailInfo.GetFileName(), EFileRead ) );

	const TInt KLineLen = 1024;
	TBuf8<KLineLen> line;
	TBuf8<1> aChar;
	TBool finished = FALSE;
	TInt pos = 0;

	do 
		{
		line.FillZ();
		line.SetLength(0);
		do 
			{
			file.Read(aChar, 1);
			if(aChar.Length())
				line.Append(aChar);
			else
				finished = TRUE;
			} while(aChar.Length() && aChar[0] != 0x0A);
			if(!line.Length())
				break;

			TBuf<KLineLen> line16;
			line16.Copy( line );
			rText->InsertL( pos, line16 );
			pos += line16.Length();
		} while(!finished);

	sendAs->SetBodyL( *rText );
	sendAs->SaveMessageL();
	TMsvId entryId = sendAs->ClientMtm().Entry().EntryId();
	CleanupStack::PopAndDestroy( 4 ); // sendAs, paraFormat, charFormat, rText
	return entryId;
	}

void CMtfTestActionCreateSmtpMessageFromParamEmailFile::ExecuteActionL()
	{

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramFolderId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));

	CPop3MailInfo* mailInfo =  ObtainParameterReferenceL<CPop3MailInfo>(TestCase(),ActionParameters().Parameter(3));

	TMsvId paramMessageId = CreateAndSendMessageL( *mailInfo );

	StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(4));
	TestCase().ActionCompletedL(*this);
	}

