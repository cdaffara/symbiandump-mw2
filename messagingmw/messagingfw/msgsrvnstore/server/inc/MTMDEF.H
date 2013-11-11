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

/**
 @file
 @publishedAll
 @released
*/

#if !defined(__MTMDEF_H__)
#define __MTMDEF_H__
#if !defined(__MTCLBASE_H__)
#include <mtclbase.h>
#endif
#if !defined(__MTMDEF_HRH__)
#include <mtmdef.hrh>
#endif

// no matching message parts

const TMsvPartList KMsvMessagePartNone			= 0x00000000;
// message parts

// Message body 
const TMsvPartList KMsvMessagePartBody			= 0x00000001;

// Message recipients 

const TMsvPartList KMsvMessagePartRecipient		= 0x00000002;

// Message originator 

const TMsvPartList KMsvMessagePartOriginator	= 0x00000004;
// Message description 
const TMsvPartList KMsvMessagePartDescription	= 0x00000008;
// Message date 

const TMsvPartList KMsvMessagePartDate			= 0x00000010;
// Message attachments 

const TMsvPartList KMsvMessagePartAttachments	= 0x00000020;

//Flag to specify case-sensitive search. 
const TMsvPartList KMsvFindCaseSensitive		= 0x80000000;

// Flag to specify search for whole words. 
const TMsvPartList KMsvFindWholeWord			= 0x40000000;


// Maximum length (in characters) of find text. 
const TInt KMsvMaxFindTextLength = 500;

// These constants are used to do advance search in the message store
// The old implementation does not make use of these constants
// A search operation performed using these constants wont give any results

// Message Part To
const TMsvPartList KMsvMessagePartTo            = 0x00000040;

// Message Part From
const TMsvPartList KMsvMessagePartFrom          = 0x00000080;

//Message Part Cc
const TMsvPartList KMsvMessagePartCc            = 0x00000100;

//Message Part Bcc
const TMsvPartList KMsvMessagePartBcc           = 0x00000200;

//Message Part Subject
const TMsvPartList KMsvMessagePartSubject       = 0x00000400;

// Flag to specify wild card characters
const TMsvPartList KMsvFindUsingWildCard		= 0x00000800;


#endif  // mtmdef#
