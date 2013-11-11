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
// Telephone Voice Mailbox number settings parser
// 
//

#ifndef __TVMP_H__
#define __TVMP_H__


class CTelVoiceMailboxParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	CTelVoiceMailboxParser();
	~CTelVoiceMailboxParser();
	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CMsvEntry& aEntry); 

private:
	HBufC*		 iTel;
	};
#endif  
