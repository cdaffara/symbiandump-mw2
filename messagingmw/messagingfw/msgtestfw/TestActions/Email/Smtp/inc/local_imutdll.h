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
//

#if !defined (__LOCAL_IMUTDLL_H__)
#define __LOCAL_IMUTDLL_H__

#if !defined (__E32STD_H__)
#include <e32std.h>
#endif

#if !defined (__BARSC_H__)
#include <barsc.h>		// RResourceFile
#endif

enum TImutErrorReason {	KPanicNoRfc822Header = 1,
						KPanicNoRichText,
						EPanicNullTokenizer,
						KPanicBoundaryTooLong,
						KPanicUnknownSendingMethod,
						KPanicInvalidSMTPLine,
						KPanicAttachmentPathNotSet,
						KPanicNoMimeClassAvailable,
						KPanicUnknownAlgorithm,
						KPanicSetAlgorithmLNotCalled,
						KPanicNoResourceFile,
						KPanicDescriptorToSmall,
						KPanicReadLengthTooLarge,
						KPanicFileNotFlushed,
						KPanicEntryIsNotMessage,
						KPanicMessageEntryNotCalled,
						KPanicMessageEntryIdHasChanged,
						KPanicServiceIdNotValid,
						KPanicIncorrectResetState,
						KPanicNoMimeHeader,
						KPanicInvalidDefaultCharacterSet,
						KPanicUnknownFolderType,
						KPanicInvalidEncodingType,
						EPanicInvalidHeaderState,
						EImskSocketNotConnected,
						EImskSocketStillActive,
						EImskSocketOpen,
						EImskUnknownState,
						EImskInvalidPortNumber,
						EImskInvalidSocketEncryptionMethod,
						EImskSecurityFailed,
						EImskDatabaseTypeUnspecified,
						EImskNotSupported,
	 					EImskUnknownBearer,
						EImskNoTLSResponseString	
						};

// and some error codes
const TInt KErrCorruptHeader = 1;

GLDEF_C void gPanic( TImutErrorReason aReason );

GLREF_C void OpenResourceFileL(RResourceFile& aFile, RFs& anFs);

GLDEF_C void CloseResourceFile( TAny* aResourceFile );


#endif // __LOCAL_IMUTDLL_H__
