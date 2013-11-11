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
//

#ifndef __RSENDASMESSAGE_H__
#define __RSENDASMESSAGE_H__

#include <msvapi.h>
#include <csendasaccounts.h>
#include <cmsvrecipientlist.h>

class RSendAs;
class CRichText;
class RFile;

/**
Send as progress

@publishedAll
@released
*/
typedef CMsvSendOperation::TSendOperationProgress TSendAsProgress;

/**
Encapsulates creating and sending a message.  This class requires
a connected RSendAs session.

A client application can have several RSendAsMessage objects created simultaneously.
Each RSendAsMessage object corresponds to a single message.

With the RSendAsMessage class, a client application can perform the following
operations:

-Creating and managing a message

-Modifying a message created using SendAs

-Sending a message

-Launching a message editor

Client applications do not require any capabilities in order to use the RSendAs
and RSendAsMessage APIs. However, if an application does not have the necessary security
capabilities to send a message, the SendAs server ensures that a message is only sent
after the user's authorization.

A message created using RSendAsMessage can be saved, sent, or passed to the appropriate
message editor. There are two types of send operation:  

-A confirmed send is forwarded to the UI MTM for the message type. Typically this
results in the user being asked to confirm that the message should be sent.

-An unconfirmed send fails if the client application does not have the
required capabilities to use the server MTM for the message type.

There are asynchronous and synchronous forms of these sending methods. Progress information for  
sending is available when the asynchronous forms are used. The synchronous forms, and the
methods for saving the message and for launching it in a message editor, all close the handle to the
RSendAsMessage, so the message cannot be further accessed through the API.

@publishedAll
@released
*/
class RSendAsMessage : public RSubSessionBase
	{
public:
/**
Defines the recipient types available when adding a recipient to a message
*/
	enum TSendAsRecipientType
		{
		/**
		Designates a 'To' recipient.
		*/
		ESendAsRecipientTo = EMsvRecipientTo,
			
		/**
		Designates a 'Cc' recipient.
		*/
		ESendAsRecipientCc,
		
		/**
		Designates a 'Bcc' recipient.
		*/
		ESendAsRecipientBcc
		};
public:
	IMPORT_C void CreateL(RSendAs& aSendAs, TSendAsAccount aAccount);
	IMPORT_C void CreateL(RSendAs& aSendAs, TUid aMessageType);
	//
	IMPORT_C void SendMessage(TRequestStatus& aStatus);
	IMPORT_C void SendMessageAndCloseL(); // Sends in background
	IMPORT_C void SendMessageConfirmed(TRequestStatus& aStatus);
	IMPORT_C void SendMessageConfirmedAndCloseL(); // Sends in background
	IMPORT_C void LaunchEditorAndCloseL();
	IMPORT_C void SaveMessageAndCloseL();
	//
	IMPORT_C void ProgressL(TSendAsProgress& aProgress);

	IMPORT_C void SetBodyTextL(const CRichText& aBody);
	IMPORT_C void SetBodyTextL(const TDesC& aBody);
	IMPORT_C void SetSubjectL(const TDesC& aSubject);
	IMPORT_C void SetBioTypeL(TUid aBioType);
	//
	IMPORT_C void AddRecipientL(const TDesC& aAddress, TSendAsRecipientType aRecipientType);
	IMPORT_C void AddRecipientL(const TDesC& aAddress, const TDesC& aAlias, TSendAsRecipientType aRecipientType);
	//
	IMPORT_C void AddAttachment(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C void AddAttachment(const TDesC& aFilePath, const TDesC8& aMimeType, TRequestStatus& aStatus);
	IMPORT_C void AddAttachment(const TDesC& aFilePath, TRequestStatus& aStatus);

	IMPORT_C void AddAttachment(RFile& aFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C void AddAttachment(RFile& aFile, const TDesC8& aMimeType, TRequestStatus& aStatus);
	IMPORT_C void AddAttachment(RFile& aFile, TRequestStatus& aStatus);

	IMPORT_C void AddLinkedAttachment(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C void AddLinkedAttachment(const TDesC& aFilePath, const TDesC8& aMimeType, TRequestStatus& aStatus);
	IMPORT_C void AddLinkedAttachment(const TDesC& aFilePath, TRequestStatus& aStatus);
	
	IMPORT_C void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile);
	IMPORT_C void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, const TDesC8& aMimeType);
	IMPORT_C void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, const TDesC8& aMimeType, TUint aCharset);
	
	//
	IMPORT_C void Cancel();
	IMPORT_C void Close();
	
	/** 
	Sets the character encoding value. The character encoding value options are 7-bit,
	8-bit and 16-Bit Unicode. By default the character set encoding is 7 bit encoding.
	*/
	IMPORT_C void SetCharacterSetL(const TUint aCharset);


private:
	TAny* iAny; // Future proofing.
	};

#endif	//  __RSENDASMESSAGE_H__
