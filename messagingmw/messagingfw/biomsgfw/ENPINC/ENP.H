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
// ENP.H
//
/**
 * @file 
 * @internalComponent
 * @released
 */

#if !defined (__ENP_H__)
#define __ENP_H__


#if !defined(__BSP_H__)
#include <bsp.h>
#endif

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif

// Forward declarations
class CImBaseEmailSettings;
class CCnvCharacterSetConverter;
class CEmailNotificationParser : public CBaseScriptParser2
/**
@internalComponent
@released
*/
    {
public:
    IMPORT_C static CEmailNotificationParser* NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
    ~CEmailNotificationParser();

    void ParseL(TRequestStatus& aStatus, const TDesC& aSms);
    void ProcessL(TRequestStatus& aStatus);

private:
    void DoCancel();
    void RunL();

private:
    enum TParseSession
        { 
        EUnfoldMessage,     //  
        EParseMessage,      //  Parsing - Lexical analysis and fill the array.. 
        ECompleteMessage    //  Completing - writing array and new message body..
        };

    CEmailNotificationParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
    void ConstructL();

    void ChangeStateL(TParseSession aState);    // Advance state machine to next state
    void UnfoldMessageL();
    void ParseMessageL();
    void CompleteMessageL();

    void AddRequiredFieldL(const TDesC& aFieldName);
    void AddParsedFieldL(const TDesC& aFieldName, const TDesC& aFieldValue, TBool aMandatory);

	// fns to try and match notification msg to a server
	void GetEmailServicesL();
	TBool CalcHashValueL(CCnvCharacterSetConverter& aCharConv, const TDesC8& aUserName, const TDesC& aServerName, const TDesC8& aPortNum);

	// utility completion fn
    void RequestComplete(TRequestStatus& aStatus, TInt aError);

	//fix for DEF017686: EMail notification parser leaves when field data is empty  
	void LeaveIfEmptyFieldsL(const TDesC& aFieldName, const TLex& aTokenLex);

    TInt            iState;                 // Current session state
    HBufC*          iSmsBuf;                // Local copy of buffer passed to ParseL()

    TRequestStatus* iReport;
    TInt            iCompleted;             // Stores completion code from previous session state
    TInt            iMessageCount;          // Count of new messages on mail host
    TBool           iExtendedNotification;  // Flag indicating simple or extended notification
	TInt			iServerId;				// Extracted from email not msg used to identfy server
	HBufC8*			iServerName;			// name of server if we can get it from the server id etc.
	TMsvId			iServiceMsvId;			// id of associated service entry
	TBool			iFoundServer;
    };


#endif // __ENP_H__
