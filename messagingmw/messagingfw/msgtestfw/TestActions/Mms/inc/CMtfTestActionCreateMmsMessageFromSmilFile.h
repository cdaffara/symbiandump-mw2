/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_CREATE_MMS_MESSAGE_FROM_SMIL_FILE_H__
#define __CMTF_TEST_ACTION_CREATE_MMS_MESSAGE_FROM_SMIL_FILE_H__


#include "CMtfTestAction.h"
#include "CMtfTestActionUtilsConfigFileParser.h"

#include <f32file.h>
#include <GmXmlParser.h>
#include <SmilDtd.h>


_LIT(KTestActionCreateMmsMessageFromSmilFile,"CreateMmsMessageFromSmilFile");


class XmlParserObserver : public MMDXMLParserObserver
	{
public:
	XmlParserObserver(TRequestStatus* aStatus) {iStatus = aStatus;}
	virtual ~XmlParserObserver() {};
	void ParseFileCompleteL() {User::RequestComplete(iStatus,KErrNone);}

private:
	TRequestStatus* iStatus;
	};


class CMtfTestActionCreateMmsMessageFromSmilFile : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCreateMmsMessageFromSmilFile(); 

public:
	virtual void ExecuteActionL();

protected:
	void DoCancel();
	void RunL();

private:
	CMtfTestActionCreateMmsMessageFromSmilFile(CMtfTestCase& aTestCase);

private:
	RFs iFs;
	CSMILDtd* iSmilDtd; 
	XmlParserObserver* iParserObserver;	
	CMDXMLParser* iXmlParser;	
	CMtfTestActionUtilsConfigFileParser* iConfigParser;
	};


#endif
