/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file which contains the utility to map the string to the 
* corresponding enumerator value
* 
*
*/



/**
 @file
*/


#ifndef __T_UTILS_ENUM_CONVERTER_H__
#define __T_UTILS_ENUM_CONVERTER_H__


// User Include
#include <t_testinstrumentation.h>

// epoc inlcudes
#include <cdbcols.h>
#include <imapset.h>
#include <smtpset.h>
#include <pop3set.h>
#include <imapcmds.h>
#include <smtpcmds.h>
#include <msvsearchsortquery.h>

// STR_SWITCH... STR_CASE... STR_SWITCH_END macro can be used as 
// switch case that would take string.
#define STR_SWITCH(str)  {\
        TBuf<100> __ps(str);\
        while(1) {

#define STR_SWITCH_END()  break; } }

#define STR_CASE(str) if(__ps == str)

#define DEFAULT_CASE()


/**
CT_MsgUtilsEnumConverter is utility class used to map the string to the 
corresponding enumerator value.
*/
class CT_MsgUtilsEnumConverter
	{
public:
	IMPORT_C CT_MsgUtilsEnumConverter();
	~CT_MsgUtilsEnumConverter();
	
	//Utility functions

	// Maps the local folder names to the corresponding folder Ids
	IMPORT_C static TMsvId FindFolderIdByName( TDesC& aFolderName );

	// Maps the  pop3 command names with the corresponding values
	IMPORT_C static TPop3Cmds ConvertDesToTPop3Cmds( const TDesC& aPopCmdName );

	// Maps the  Imap4 command names with the corresponding values
	IMPORT_C static TImap4Cmds ConvertDesToTImap4Cmds(TDesC& aImapCmdName);
	
	// Maps the  SMTP command names with the corresponding values
	IMPORT_C static TSmtpCmds ConvertDesToTSmtpCmdsL(TDesC& aSmtpCmdName);

	// Maps the TCommDbDialogPref enumerations		
	IMPORT_C static TCommDbDialogPref ConvertDesToTCommDbDialogPref( TDesC& aDialogPrefStr );
	
	// Maps the TImap4GetMailOptions enumerations	
	IMPORT_C static TImap4GetMailOptions ConvertDesToTImap4GetMailOptions( TDesC& aMailOption );

	// Maps the TFolderSyncType Enumerations	
	IMPORT_C static TFolderSyncType ConvertDesToTFolderSyncType( TDesC& aFolderSyncType );
	
	// Maps the TFolderSubscribeType Enumerations	
	IMPORT_C static TFolderSubscribeType ConvertDesToTFolderSubscribeType( TDesC& aFolderSubscriberType );

	// Maps the TImImap4PartialMailOptions Enumerations
	IMPORT_C static TImImap4PartialMailOptions ConvertDesToTImImap4PartialMailOptions( TDesC& aPartialMailOption );

	// Maps the TMsgOutboxBodyEncoding enumerations							
	IMPORT_C static TMsgOutboxBodyEncoding ConvertDesToTMsgOutboxBodyEncoding( TDesC& aOutboxBodyEncoding );  

	// Maps the TImSMTPSendMessageOption Enumerations
	IMPORT_C static TImSMTPSendMessageOption ConvertDesToTImSMTPSendMessageOption( TDesC& aSendMsgOption );

	// Maps the TImSMTPSendCopyToSelf Enumerations
	IMPORT_C static TImSMTPSendCopyToSelf ConvertDesToTImSMTPSendCopyToSelf( TDesC& aSmtpCopyToSelf );
	
	// Maps the TPop3GetMailOptions Enumerations
	IMPORT_C static TPop3GetMailOptions ConvertDesToTPop3GetMailOptions( TDesC& aPop3MailOption );

	// Maps the Eventcode strings to their numeric value
	IMPORT_C static TInstrumentationPoint ConvertDesToEventCode(TDesC& aEventCodeString);

	// Maps bearer types strings to their numeric values
	IMPORT_C static TUint32 ConvertDesToBearerTypes(const TDesC& aBearerTypesString);
	
	// Maps search sort types strings to their numeric values
	IMPORT_C static TInt ConvertDesToSearchSortOption(const TDesC& aSearchSortString);

	// Maps TMsvMessagePart Enumerations
	IMPORT_C static TMsvMessagePart ConvertDesToMessagePart(const TDesC& aMessagePart);
	
	// Maps TMsgRelationOp Enumerations
	IMPORT_C static TMsvRelationOp ConvertDesToMsgRelationOp(const TDesC& aRelationOp);
	
	// Maps TMsvPriority Enumerations
	IMPORT_C static TMsvPriority ConvertDesToMsgPriority(const TDesC& aMsgPriority);

	// Maps TSortingOption Enumerations
	IMPORT_C static TMsvSortOrder ConvertDesToSortOrder(const TDesC& aSortOrder);
	};
#endif //__T_UTILS_ENUM_CONVERTER_H__
