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

#ifndef __IACPERR_H__
#define __IACPERR_H__

const TUid KUidSmartMessageMtm = {0x10001262};

enum TIacpAssertError
	{
	EIacpEmptyBuffer,
	EIacpInvalidEntry
	};

// public error codes that the parser may generate in addition to EPOC32 error codes.
#define	KIacpBaseError              -510
#define	KIacpUnknownSmsType			KIacpBaseError		// Left token not starting with neither 'M' nor 'I'
#define	KIacpBIOMsgTypeNotSupported	KIacpBaseError-1	// this Bio msg is not suppported.
#define	KIacpMandatoryDataNotSet	KIacpBaseError-2    // a mandatory Field/Value has not been found in SMS message
#define	KIacpUnknownMailProtocol	KIacpBaseError-3	// Not a Pop3 nor Imap4
#define	KIacpErrRightToken			KIacpBaseError-4	// improper right token i.e not equal t/f (True/False)
#define	KIacpErrLeftToken			KIacpBaseError-5	// Unknown Left Token i.e not starting with 'M' for Mail or 'I' for Internet configuration
#define	KIacpErrSmsDataNotParsed	KIacpBaseError-6	// sms data should be parsed before commiting
#define	KIacpErrSmsDataNotRestored	KIacpBaseError-7	// sms data should be parsed before commiting
#define	KIacpScriptErrISPNotFound   KIacpBaseError-8	// Internet Service not found in Dial Out DB
#define	KIacpErrScriptNotDefined    KIacpBaseError-9	// Sript not included in sms
#define	KIacpErrScriptNotFoundInDB  KIacpBaseError-10	// There is no script in DB to append to.
#define	KIacpErrScriptAlreadyAdd	KIacpBaseError-11   // Script cannot be add more than ounce for a each Bio Msg
#endif //__IACPERR_H__
