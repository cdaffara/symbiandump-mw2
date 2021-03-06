// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_corruptMpr_h__
#define __T_corruptMpr_h__

const TUint8 KMsgHeaderMultipartRelated[]=
	{
	0xB3,  //Content type "application/vnd.wap.multipart.related"
	0x94,  // Date
	0x04,  // length of field
	0x35,  // Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0xAF,  //X-Wap-Application-ID
	0x82  // Short int - 0x02 represent x-wap-application:push.ua
	};

// the 1st-Body is Corrupt in this Muli-part/Related msg	
const TUint8 KCorruptMsgBodyMultipartRelated[]=
	{
	0x02,	// number of parts
	0x0A,	// part 1 header size
	0x1D,	// part 1 body size
	0xB0,	// content type application/vnd.wap.slc ... the headers:
	0x94,	// Date
	0x04,	// length of field
	0x35,	// Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0x3F,	
	0x45,
	0x11,
// Corrupt 1st-Body  - slc (www.schmookie.com)
	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
// 2nd Header
	0x0A,	// part 2 header size
	0x1D,	// part 2 body size
	0xB0,	// content type application/vnd.wap.slc ... the headers:
	0x94,	// Date
	0x04,	// length of field
	0x35,	// Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0x3F,	
	0x45,
	0x11,
//2nd Body - slc (www.schmookie.com)
	0x01, 0x06, 0x6A, 0x00, 0x85, 0x09, 0x03, 0x6D, 0x6D, 0x6D,
	0x2E, 0x73, 0x63, 0x68, 0x6D, 0x6F, 0x6F, 0x6B, 0x69, 0x65,
	0x2E, 0x63, 0x6F, 0x64, 0x65, 0x2F, 0x00, 0x06, 0x01
};


#endif
