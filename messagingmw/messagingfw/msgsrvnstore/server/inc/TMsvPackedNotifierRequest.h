// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TMsvPackedNotifierRequest.h
//
/**
 * @file 
 * @publishedPartner
 * @released
 */

#ifndef __TMSVPACKEDNOTIFIERREQUEST_H__
#define __TMSVPACKEDNOTIFIERREQUEST_H__

#include <msvipc.h>

/**
DRM Mime type

@publishedPartner
@released
*/
_LIT8(KEpocMimeTypeDrm,"X-Epoc/DRM.Receipt");

/**
Packs a selection and security info object into the IPC buffer.

@publishedPartner
@released
*/
class TMsvPackedNotifierRequest

	{
public:
	IMPORT_C TMsvPackedNotifierRequest(HBufC8*& aBuffer);
	IMPORT_C TInt Pack(const CMsvEntrySelection& aSelection, const TSecurityInfo& aSecurityInfo);
	IMPORT_C static void UnpackL(const TDesC8& aBuffer, CMsvEntrySelection& aSelection, TSecurityInfo& aSecurityInfo);
private:
	HBufC8*& iBuffer;
	};
	
#endif	// __TMSVPACKEDNOTIFIERREQUEST_H__
