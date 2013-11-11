/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/

#ifndef __IM_CLT_CV_RECV_H__
#define __IM_CLT_CV_RECV_H__


// User includes
#include "Local_imcvrecv.h"


// Epoc includes
#include <msvapi.h>


/**
Implements the Rfc822 specication for creating a message
*/
class CImCltRecvConvert : public CLocalImRecvConvert
	{
friend class CLocalMimeParser;
public:
	/*IMPORT_C*/ static CImCltRecvConvert* NewLC( RFs& anFs,CMsvEntry* aClientEntry,TUid aMsgType,TMsvId aEmailServiceId);
	/*IMPORT_C*/ static CImCltRecvConvert* NewL( RFs& anFs,CMsvEntry* aClientEntry,TUid aMsgType,TMsvId aEmailServiceId);
	/*IMPORT_C*/ ~CImCltRecvConvert();

	/*IMPORT_C*/ void ResetL();
	/*IMPORT_C*/ void ResetForHeadersL();

	/*IMPORT_C*/ TInt ParseNextFieldL(const TDesC8& aSourceLine);
	/*IMPORT_C*/ TMsvId MessageCompleteL();
	/*IMPORT_C*/ void MessageCompleteL(TMsvEmailEntry aEmailEntry);
	/*IMPORT_C*/ TMsvEmailEntry MessageEntryDetailsL();

	inline void SetMsvId(TMsvId aId);
	inline CMsvEntry& ClientEntry();
	inline TMsvId EntryId();	

private:
	CImCltRecvConvert(RFs& anFs,CMsvEntry* aClientEntry,TUid aMsgType,TMsvId aEmailServiceId);
	void ConstructL(RFs& anFs);

	void ParseNextLineL(const TDesC8& aSourceLine);
	void ParseBodyLineL(const TDesC8& aSourceLine);
	void ParseMimeLineL(const TDesC8& aSourceLine);
	void EndOfHeaderProcessingL();
	TBool CreateNonMIMEFolderEntryL(TMsvId aCurrentId);

	TInt Value(const TUint8*&);
	void Whitespace(const TUint8*&);

	TBool CreateAttachmentL();
	void WriteToAttachmentL(const TDesC8& text);

	void MoveUpFromFolderEntryL();
	void MoveToParentEntryL();
	void UpdateMultipartDataL();

	TBool StoreEntryDataL();

	void StoreEntryStreamsL();
	void StoreEntryStreamsL(TInt aSettings);
	void CreateEntryL();
	void StoreMessageEntryDetailsL();
	
private:
	CMsvEntry* iClientEntry;
	CMsvServerEntry* iNullServerEntry;
	};

#include "ImCltCvRecv.inl"

#endif 
