// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __BIOOP_H__
#define __BIOOP_H__


#include <msvapi.h>


// package buffer for progress info.
typedef TPckgBuf<TBioProgress> TBioProgressBuf;

// forward declarations
class CBaseScriptParser2;


class CBIOOperation : public CMsvOperation
/** BIO client MTM Parser Active Object.

The BIO client MTM Parser Active Object is responsible for parsing and processing BIO files.
This activity was originally carried out in the BIO Server MTM but has been relocated to
the Client MTM for Platsec capability reasons.  The parsing and processing is carried out
in an Active Object rather than synchronously in order to avoid making a UI unresponsive.
@internalTechnology
@released
*/
	{
public:
	enum TBioOperationState 
		{ 
		EBiooWaiting,
		EBiooCreating, 
		EBiooParsing, 
		EBiooProcessing, 
		EBiooComplete 
		};
	enum TBioOperation 
		{ 
		EBioParseOnly, 
		EBioParseAndProcess, 
		EBioProcess 
		};
	enum TBioMessageState 
		{ 
		EBioMsgNotParsed=0, 
		EBioMsgParsed, 
		EBioMsgProcessed 
		};
public:
	static CBIOOperation* NewL(RFs aRFs, CMsvSession& aSession, CBIODatabase* aBDB, TRequestStatus& aCompletionStatus);
	~CBIOOperation();
protected:
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);
public:	
	// from CMsvOperation
	virtual const TDesC8& ProgressL();
public:
	// new functions
	void StartCommand(const CMsvEntrySelection& aSelection, TInt aCommand, TRequestStatus& aStatus);
private:
	CBIOOperation(RFs aRFs, CMsvSession& aSession, CBIODatabase* aBDB, TRequestStatus& aCompletionStatus);
	void ConstructL();
	//
	void ParseL(const CMsvEntrySelection& aSelection, TBool aCommit);
	void CreateParserL();
	void StartParserL();
	void ExtractMessageBodyL();
	void ProcessL(const CMsvEntrySelection& aSelection);
	void ProcessL();
private:	
	RFs					iFs;
	CBIODatabase*		iBioDatabase;
	TRequestStatus*		iReportStatus;
	TBioProgress		iProgress;
	TBioProgressBuf		iProgressBuf;
	TBool				iInOperation;
	TUid				iUid;
	TBioOperationState	iBioOperationState;
	TBioOperation		iBioOperationOperation;
    TMsvId              iCurrentMsvId;
	//
	CRegisteredParserDll*	iRegisteredParserDll;
	CBaseScriptParser2*	iParser;
	HBufC*				iMessageBody;
	CMsvEntry*			iCurrentEntry;
	};


#endif // __BIOOP_H__
