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

#ifndef __CSENDASMESSAGE_H__
#define __CSENDASMESSAGE_H__

#include <e32base.h>
#include <msvids.h>
#include <rsendasmessage.h>
#include <tsendasclientpanic.h>

#include "msendassenderobserver.h"
#include "msendasattachmentobserver.h"
#include "sendasserverdefs.h"

// forward declarations
class CSendAsSession;
class CSendAsAttachment;
class CSendAsSender;

/**
A message is represented by this object, one per message.

This class is derived from CObject, so that the object ID methods
can be used to handle messages in the correct subsession.

@internalComponent
@released
*/
class CSendAsMessage : public CObject, public MSendAsSenderObserver, public MSendAsAttachmentObserver
	{
public:
	static CSendAsMessage* NewL(CSendAsSession& aSession);
	virtual ~CSendAsMessage();

	TBool DoSubSessionServiceL(const RMessage2& aMsg);
	void CancelMessage();
private:
	CSendAsMessage(CSendAsSession& aSession);
	void ConstructL();
	//
	void CreateMessageForAccountL(const RMessage2& aMessage);
	void CreateMessageByTypeL(const RMessage2& aMessage);
	void ProgressL(const RMessage2& aMessage);
	void DeleteMessageL(const RMessage2& aMessage);
	void SetBodyTextL(const RMessage2& aMessage, TBool aFirstChunk = EFalse);
	void SetSubjectL(const RMessage2& aMessage);
	void SetBioTypeL(const RMessage2& aMessage);
	void TransferAttachmentFileL(const RMessage2& aMessage);
	void AddRecipientWithAliasL(const RMessage2& aMessage);
	void AddRecipientL(const RMessage2& aMessage);
	void AddAttachmentL(const RMessage2& aMessage);
	void AddAttachmentWithMimeTypeL(const RMessage2& aMessage);
	void AddLinkedAttachmentL(const RMessage2& aMessage);
	void AddLinkedAttachmentWithMimeTypeL(const RMessage2& aMessage);
	void CreateAttachmentL(const RMessage2& aMessage);
	void CreateAttachmentWithMimeTypeL(const RMessage2& aMessage);
	void Cancel();
	void LaunchEditorL(const RMessage2& aMessage);
	void SendMessageL(const RMessage2& aMessage, TBool aConfirmed, TBool aBackground = EFalse);
        void DoSaveMessageL();
	//
	void DoCreateMessageL(TUid aMtm, TSendAsAccount aAccount = KMsvRootIndexEntryId);
	void PrepareAddCreateAttachmentL(const RMessage2& aMessage);
	void DeleteClientMtm();
	void VerifyCallerCapabilitiesL(const RMessage2& aMessage, TBool& aConfirmed);
	
	/** 
	Sets the character encoding value. The character encoding value options are 7-bit,
	8-bit and 16-Bit Unicode. By default the character set encoding is 7 bit encoding.
	*/
	void SetMessageCharacterSetL(const RMessage2& aMessage);	

	void SetCharsetInfoForAttachment(const RMessage2& aMessage);
	//
	HBufC* GetDesParamLC(const RMessage2& aMsg, TInt aIndex);
	HBufC8* GetDesParam8LC(const RMessage2& aMsg, TInt aIndex);
	//
	void PanicClientL(const RMessage2& aMessage, TSendAsClientPanic aPanic) const;
	// methods from MSendAsSenderObserver
	virtual void SenderComplete(TInt aError);
	// methods from MSendAsAttachmentObserver
	virtual void AttachmentCompleteL(TInt aError, TBool aCompleteMessage);
private:
	enum TSendAsMessageState
		{
		EIdle,
		EMessageCreated,
		EAddingOrCreatingAttachment,
		ESendingMessage,
		ESendingMessageComplete,
		EMessageDeleted,
		EPendingClose
		};
private:
	TMsvSendOperationProgress	iProgress;
	TSendAsMessageState 		iState;
	CSendAsSession& 			iSession;
	TUid						iMessageType;
	CSendAsSender*				iSender;
	CBaseMtm*					iClientMtm;
	CSendAsAttachment*			iAttachment;
	RMessage2					iQueuedMessage;
	RFile						iAttachmentFile;
	TUint						iSignatureSize;
	TUint						iCharSet;
	};
		
#endif	// __CSENDASMESSAGE_H__
