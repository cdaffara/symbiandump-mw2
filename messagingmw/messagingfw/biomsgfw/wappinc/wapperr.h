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
// Defines for the specific error codes returned from WAP Access Point parser
// 
//



#if !defined (__WAPPERR_H__)
#define __WAPPERR_H__


// public error codes that the parser may generate in addition to EPOC32 error codes.
#define KWappBaseError					(-600)
#define KWappErrXMLVersion 				(KWappBaseError-1)
#define	KWappErrOutsideBoundaries		(KWappBaseError-2) // index in exceeds boundaries
#define KWappErrStringTable				(KWappBaseError-3) // Bad String table reference
#define KWappErrEOS						(KWappBaseError-4) // Reached the end of a key descriptor
#define KWappErrUnexpectedValue			(KWappBaseError-5) // Expecting a different tag, or content/attributes
#define KWappErrNoAttributes			(KWappBaseError-6) // Expecting attributes- missing
#define KWappErrMandatoryTagMissing		(KWappBaseError-7) // Search for a mandatory tag failed
#define KWappErrStoreNotFound			(KWappBaseError-8) // No store attached to this entry
#define KWappErrMsgUnparsed				(KWappBaseError-9) // Trying to process a message which is unparsed
#define KWappErrUnrecognised			(KWappBaseError-10) // Token or string is not a recognised value
#define KWappErrNullValue				(KWappBaseError-11) // Variable is null or has not been intialised
#define KWappErrContent					(KWappBaseError-12) // Empty element, particularly a problem if it's an empty characteristic- has no PARMS
#define KWappErrNoDBRecords				(KWappBaseError-13) // Could not find a first record in the COMMDB
#define KWappErrNotSupported			(KWappBaseError-14) // Not Supported - WAPP specific
#define KWappErrBadMessage				(KWappBaseError-15) // Data content/format of message is invalid
#define KWappErrNoTermination			(KWappBaseError-16) // Terminating byte missing from string or data element

#endif  // __WAPPERR_H__
