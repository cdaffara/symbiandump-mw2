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

#ifndef __CSENDASATTACHMENT_H__
#define __CSENDASATTACHMENT_H__

#include <e32base.h>
#include <f32file.h>

// forward declarations
class MSendAsAttachmentObserver;
class CBaseMtm;

/**
@internalComponent
@released
*/
class CSendAsAttachment : public CActive
	{
public:
	static CSendAsAttachment* NewL(MSendAsAttachmentObserver& aObserver, CBaseMtm& aClientMtm);
	virtual ~CSendAsAttachment();
	//
	void AddExistingFileAttachmentL(RFile& aAttachment);
	void AddExistingFileAttachmentWithMimeTypeL(RFile& aAttachment, const TDesC8& aMimeType, TUint aCharset);
	void AddFileLinkAttachmentL(const TDesC& aFileName);
	void AddFileLinkAttachmentWithMimeTypeL(const TDesC& aFileName, const TDesC8& aMimeType, TUint aCharset);
	void CreateNewFileAttachmentL(RFile& aAttachment, const TDesC& aFileName);
	void CreateNewFileAttachmentWithMimeTypeL(RFile& aAttachment, const TDesC& aFileName, const TDesC8& aMimeType, TUint aCharset);
private:
	CSendAsAttachment(MSendAsAttachmentObserver& aObserver, CBaseMtm& aClientMtm);
	//
	void CompleteL(TInt aError);
	// from CSendAsActive
	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);
private:
	enum TSendAsAttachmentState
		{
		EIdle				 = 0,
		EAddingAttachment,
		ECreatingAttachment,
		EAttachmentComplete
		};
private:
	MSendAsAttachmentObserver*	iObserver;
	CBaseMtm&					iClientMtm;
	TSendAsAttachmentState		iState;
	};

#endif	// __CSENDASATTACHMENT_H__
