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
//

#if !defined (__CBCP_H__)
#define __CBCP_H__


#include <bsp.h>


class CBusinessCardParser : public CBaseScriptParser2
/**
@internalComponent
@released
*/
    {
public:
    IMPORT_C static CBusinessCardParser* NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
    ~CBusinessCardParser();

    void ParseL(TRequestStatus& aStatus, const TDesC& aSms);
    void ProcessL(TRequestStatus& aStatus);

private:
    void DoCancel();
    void RunL();

private:
    enum TParseSession
        { 
        EUnfoldMessage,     //  Perform MIME unfolding of message.
        EParseMessage,      //  Parsing - Lexical analysis and fill the array.
        ECompleteMessage    //  Completing - writing array and new message body.
        };

	CBusinessCardParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
    void ConstructL();

    void ChangeStateL(TParseSession aState);    // Advance state machine to next state
    void ParseMessageL();
	void StripCarriageReturnsL();
    void CompleteMessageL();

    void AddRequiredFieldL(const TDesC& aFieldName);
    void AddParsedFieldL(const TDesC& aFieldName, const TDesC& aFieldValue, TBool aMandatory);

	void CreateDataL();
	void AppendItemL(const TDesC& aItem);
	void AppendTelItemL(const TDesC& aTel, const TDesC& aDefaultLabel, const TDesC& aCellLabel);
	TBool IsValidTelephoneCharacter() const;


    void RequestComplete(TRequestStatus& aStatus, TInt aError);

    TInt            iState;                 // Current session state
	TInt			iExtraLines;			// Number of extra lines of text after standard address.
	TInt			iNumberOfTel;			// Number of telephone numbers.
	TInt			iNumberOfFax;			// Number of fax numbers.
	TInt			iNumberOfCbcFields;		// Number of fields.

    TRequestStatus* iReport;
    TInt            iCompleted;             // Stores completion code from previous session state
	HBufC*			iNotesBuffer;			// Pointer to notes buffer.
    };


#endif // __CBCP_H__
