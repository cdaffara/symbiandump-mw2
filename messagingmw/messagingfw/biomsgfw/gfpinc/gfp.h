// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Header file for the Generic File parser
// 
//

#if !defined (__GFP_H__)
#define __GFP_H__

#include <bsp.h>

class CMsvGenericFileParser : public CBaseScriptParser2
/**
@internalComponent
@released
*/
    {
public:
    IMPORT_C static CMsvGenericFileParser* NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);

    ~CMsvGenericFileParser();

    void ParseL(TRequestStatus& aStatus, const TDesC& aSms);
    void ProcessL(TRequestStatus& aStatus);

private:
    void DoCancel();
    void RunL();

private:
    enum TParseSession
        { 
        //EUnfoldMessage,     //  Don't have to unfold the message, just save it
        EParseMessage,      //  Parsing 
        ECompleteMessage    //  Completing ..
        };

    CMsvGenericFileParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
    void ConstructL();

    void ChangeStateL(TParseSession aState);    // Advance state machine to next state
    void ParseMessageL();
    void CompleteMessageL();
	void GetResourceFileL(TFileName& aFileName);

    void RequestComplete(TRequestStatus& aStatus, TInt aError);

    TInt            iState;                 // Current session state
	TBuf<256>		iDescriptionText;		// Holds on to the description text.

    TRequestStatus* iReport;
    TInt            iCompleted;             // Stores completion code from previous session state
	TInt32			iBioType;
    };


#endif // __GFP_H__
