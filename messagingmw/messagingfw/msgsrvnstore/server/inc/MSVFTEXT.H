// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MSVFTEXT_H__
#define __MSVFTEXT_H__

#include <e32base.h>

#include <mentact.h>
#include <mtclbase.h>
#include <mtmdef.h>

class CRichText;

class CMsvFindText : public CMsgActive
/** A utility class which searches through given plain text or rich text for a 
specified text string.

The class is an active object and searching can be done synchronously or asynchronously. 
An instance of this class can be re-used to perform multiple searches for 
text. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMsvFindText* NewLC(TInt aPriority = EPriorityStandard);
	IMPORT_C static CMsvFindText* NewL(TInt aPriority = EPriorityStandard);
	IMPORT_C ~CMsvFindText();
	//
	IMPORT_C void FindTextL(const TDesC& aFind, const TDesC& aSource, TMsvPartList aFlags, TRequestStatus& aStatus);
	IMPORT_C void FindRichTextL(const TDesC& aFind, const CRichText& aSource, TMsvPartList aFlags, TRequestStatus& aStatus);
	//
	IMPORT_C TBool FindTextL(const TDesC& aFind, const TDesC& aSource, TMsvPartList aFlags);
	IMPORT_C TBool FindRichTextL(const TDesC& aFind, const CRichText& aSource, TMsvPartList aFlags);
	//
	inline TBool FoundText() const;
	//
private:
	CMsvFindText(TInt aPriority);
	//
	void InitialiseL(const TDesC& aFind, const TDesC* aPlainSource, const CRichText* aRichSource, TMsvPartList aFlags);
	void DoFindAsyncL(TRequestStatus& aStatus);
	void DoComplete(TInt&);
	TBool DoFindSyncL();
	TBool DoFindStepL();
	TBool DoFindTextL();
	void DoRunL();
	void Check();
	//
private:
	HBufC* iBuf;
	HBufC* iFind;
	TInt iSourcePos;
	TInt iMaxRead;
	TInt iSourceLen;
	const CRichText* iRichText;
	const TDesC* iPlainText;
	TMsvPartList iFlags;
	TChar iPreChar;
	TChar iPostChar;
	TBool iFoundText;
	};

#include <msvftext.inl>

#endif
