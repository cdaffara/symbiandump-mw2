// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// IMCVRECV.INL
// Class CRfc822Token Inline functions
// 
//

inline HBufC8* CLocalRfc822Token::OutputLine()
	{
	return iOutputLine;
	}

inline TBool CLocalRfc822Token::LastToken()
	{
	return iLastToken;
	}

inline void CLocalRfc822Token::SetImRecvConvert( CLocalImRecvConvert* aImRecvConvert )
	{
	iImRecvConvert = aImRecvConvert;
	}


/****************************************************************************
	Class CLocalImRecvConvert Inline functions
*****************************************************************************/

inline void CLocalImRecvConvert::SetAttachmentPathL(const TDesC& aFullPath)
	{
	// get full path to current message file = directory path
	if (iAttachmentFullPath)
		{
		delete iAttachmentFullPath;
		iAttachmentFullPath=NULL;
		}
	iAttachmentFullPath=aFullPath.AllocL();
	}

inline void CLocalImRecvConvert::SetMsvId(TMsvId aId)
	{
	iServerEntry->SetEntry(aId);
	iPopulateMessage = (iServerEntry->Entry().iType == KUidMsvMessageEntry);
	iRootEntryId = aId;
	}

inline const TTime CLocalImRecvConvert::Date() const
	{
	return iTimeDate;
	}

inline const TMsvPriority CLocalImRecvConvert::Priority() const
	{
	return iImPriority;
	}

inline void CLocalImRecvConvert::SaveAllAttachments(TBool aSave)
	{
	iSavingAttachments=aSave;
	}

inline const TInt CLocalImRecvConvert::ReceiveError() const
	{
	return iReceiveError;
	}

inline CMsvServerEntry& CLocalImRecvConvert::ServerEntry()
	{
	return *iServerEntry;
	}

inline const TBool CLocalImRecvConvert::ValidCompleteHeader() const
	{
	return (iEmptyHeaderSize<(iOutputHeader->DataSize()) && iFinishedHeader);
	}

inline const CImHeader& CLocalImRecvConvert::Header() const
	{
	return *iOutputHeader;
	}

inline const TMsvId CLocalImRecvConvert::EntryId() const
	{
	// If we are temporarily on the null entry then return the saved entry
	if (iServerEntry->Entry().Id() == KMsvNullIndexEntryId)
		return iSavedEntryId;
	else
		return iServerEntry->Entry().Id();
	}

inline TBool CLocalImRecvConvert::NotFinishedRfc822Header()
	{
	return iNotFinishedRfc822Header;
	}

inline CImConvertCharconv& CLocalImRecvConvert::CharacterConverter()
	{
	return *iCharConv;
	}

/****************************************************************************
	Class CLocalMimeParser Inline functions
*****************************************************************************/
inline void CLocalMimeParser::StoreMimeHeaderL(CMsvStore& entryStore)
	{
	iMimeHeader->StoreL(entryStore);
	}

inline void CLocalMimeParser::StoreMimeHeaderWithoutCommitL(CMsvStore& entryStore)
	{
	iMimeHeader->StoreL(entryStore);
	}

inline void CLocalMimeParser::RestoreMimeHeaderL(CMsvStore& entryStore)
	{
	RestoreMimeParserL(entryStore);
	}

inline TMimeContentType CLocalMimeParser::ContentType()
	{
	return iContentType;
	}

inline TImEncodingType CLocalMimeParser::ContentEncoding()
	{
	return iContentEncoding;
	}

inline const TBool CLocalMimeParser::MessageIsMime() const
	{
	return isMime;
	}

inline const TDesC& CLocalMimeParser::ContentDescription() const
	{
	return iContentDescription;
	}

inline const TPtrC CLocalMimeParser::ContentLocation() const
	{
	return iMimeHeader->ContentLocation();
	}

inline const TPtrC8 CLocalMimeParser::ContentId() const
	{
	return iMimeHeader->ContentID();
	}

inline const TPtrC8 CLocalMimeParser::ContentDisposition() const
	{
	return iMimeHeader->ContentDisposition();
	}

inline const TBool CLocalMimeParser::IsTerminatingBoundary() const
	{
	return iTerminatingBoundary;
	}

inline const TBool CLocalMimeParser::BoundaryExists() const
	{
	return (iBoundaryLength!=0);
	}

inline const TBool CLocalMimeParser::BoundaryFound() const
	{
	return iBoundaryFound;
	}

inline const TInt CLocalMimeParser::MimeHeaderSize() const
	{
	return iEmptyMimeHeaderSize-iMimeHeader->Size();
	}

inline const TInt CLocalMimeParser::ReceiveError() const
	{
	return iReceiveError;
	}

inline TBool CLocalMimeParser::IsMessageDigest()
	{
	return (ContentType()==EMimeMultipart && ContentSubType().Compare(KImcvDigest)==0);
	}

inline TBool CLocalMimeParser::MimeFieldsExist() const
	{
	return iMimeFieldsExist;
	}

inline void CLocalMimeParser::ResetMimeFieldsExist()
	{
	iMimeFieldsExist=EFalse;
	}
