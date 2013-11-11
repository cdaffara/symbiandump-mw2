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
// This file contains the CWapPushMsgGen for creating push messages
// 
//

/**
 @file WapPushMsgGen.h
*/

#ifndef __WAPPUSHMSGGEN_H__
#define __WAPPUSHMSGGEN_H__


#include <e32base.h>

class CWapPushCorruptMessageTest;

/**
* Push Message Generator Utility 

Use this to create push messages. Call StartNewMsgL to start a type of message. Then call AppendFieldL to append data to
it. Finally call CreateMsgLC to create the whole message. Then just use StartNewMsgL to start the next message
*/
class CWapPushMsgGen : public CBase
	{
public:
	enum TMsgType
		{
		EServiceIndication = 0,
		EServiceLoad = 1,
		ECorruptMsgTest, 
		ECorruptXmlVersionTest,
		EEmptyMsgBody
		};

	enum TMsgField
		{
		EHRef,						// si or sl href
		EServiceIndicationID ,		// si_id
		EServiceIndicationCreated,	// created
		EServiceIndicationExpires,	// si-expires
		EServiceIndicationText,		// indication text that occurs between >  and </indication>
		EAction,					// si or sl action
		EBadHRef,						// si or sl href
		EBadServiceIndicationID ,		// si_id
		EBadServiceIndicationCreated,	// created
		EBadServiceIndicationExpires,	// si-expires
		EBadAction,					// si or sl action
		EXMLNoFieldStr,
		EWBXMLNoFieldStr
		};


	static CWapPushMsgGen* NewL();
	~CWapPushMsgGen();
	void AppendFieldL(TMsgField aField, const TDesC8& aFieldValue);
	HBufC8* CreateMsgLC();
	void StartNewMsgL(TMsgType aMsgType);
	void AppendMsgDesL(const TDesC8& aDes);
private:
	friend class CWapPushCorruptMessageTest;
	enum TWapPushMsgGenPanic
		{
		EInvalidPushMessageType,
		EInvalidPushMessageField
		};
	
	void AppendMsgFieldDesL(const TDesC8& aDes);
	void Panic(TWapPushMsgGenPanic aPanicCode);
private:
	HBufC8* iMsgBuffer;
	HBufC8* iMsgFieldBuffer;
	TMsgType iMsgType;
	};





#endif // __WAPPUSHMSGGEN_H__