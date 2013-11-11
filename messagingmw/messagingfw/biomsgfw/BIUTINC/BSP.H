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
// BSP.H (Base Script Parser)
// 
//

/**
 * @file 
 * Abstract class for different Parsers.
 *
 * @publishedAll
 * @released
 */
#if !defined(__BSP_H__)
#define __BSP_H__

#if !defined(__MTCLREG_H__)
#include <mtclreg.h>
#endif

#if !defined(__MTCLBASE_H__)
#include <mtclbase.h>
#endif

#include <e32base.h>
#if !defined(__S32STRM_H__)
#include <s32strm.h>
#endif

#include <msvstd.h>
// CRichText etc. includes
#include <txtrich.h>
#include <txtfmlyr.h>

#include <bif.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif

// Parsed field class for use by parsers.
class CParsedField : public CBase
/** Represents a single token-value pair for a given field in a BIO/smart message 
grammar. 
@publishedAll
@released
*/
{
public:
	IMPORT_C CParsedField();
	IMPORT_C ~CParsedField();

	IMPORT_C TPtrC FieldName() const;
	IMPORT_C void SetFieldNameL( const TDesC& aFieldName);
	IMPORT_C TPtrC FieldValue() const;
	IMPORT_C void SetFieldValueL( const TDesC& aFieldValue);
	IMPORT_C TBool MandatoryField() const;
	IMPORT_C void SetMandatoryField(TBool aMandatoryField);

	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
private:
	void Reset();
private:
	HBufC* iFieldName;
	HBufC* iFieldValue;
	TBool  iMandatoryField;
};

/**
 * Forward declarations:
 * @publishedAll
 * @released
 */
class CMsvServerEntry;
class CMsvEntry;
class CRegisteredParserDll;
class RMsvReadStream;
class RMsvWriteStream;
class CMsvStore;
class CSmsMessage;

/** Base class for BIO message parsers V2.

Concrete derived classes are implemented in parser DLL's to parse particular 
types of BIO message.

On receiving an appropriate command (see TBioOperation), the BIO server MTM 
loads the appropriate parser and passes the message body to it for interpretation. 
In fact, the parser interface expects the parser to divide its operation into 
two stages: 

1. parsing: which involves extracting information from the raw message body and 
storing it in a structured format. The parsing stage can also alter the message 
body, and create files in the directory associated with the message (e.g. 
parsing a ring tones message will generate a ring tone file).

2. processing: which involves using the extracted information to achieve the 
purpose of the BIO message (e.g. setting some phone configuration setttings).

This separation allows, for example, a UI to display the parsed information 
to the user for confirmation, before it is acted upon. For some parsers, however, 
this two stage division is not sensible, in which case they implement only 
the first.

The base class provides a pointer iSettings to reference the raw message data, 
and an array of token-value pairs, iParsedFieldArray, for storing parsed information 
(if this is appropriate). 
@publishedAll
@released
*/
class CBaseScriptParser2: public CActive
	{
public:
	IMPORT_C CBaseScriptParser2(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
	IMPORT_C ~CBaseScriptParser2();

	/** Called by the BIO server MTM to asynchronously parse message body data.
	
	When parsing is complete, the function should indicate this by setting the 
	message's index field iMtmData3 to 1.
	
	The function should leave if the buffer cannot be parsed successfully.
	
	@param aStatus Asynchronous status word
	@param aSms Buffer to parse */
	virtual void ParseL(TRequestStatus& aStatus, const TDesC& aSms)=0; //parses sms data into CParsedField
	/** Called by the BIO server MTM to asynchronously process the parsed data.
	
	The function takes appropriate parser-specific action on the results of a 
	previous call to ParseL().
	
	When processing is complete, the function should indicate this by setting 
	the message's index field iMtmData3 to 2.
	
	The function should leave if processing is not successful.
	
	@param aStatus Asynchronous status word */
	virtual void ProcessL(TRequestStatus& aStatus)=0; //stores parsed data into streams and data base

	IMPORT_C TUid ParserUid();
	IMPORT_C void RestoreL(CMsvStore& aMessageStore);
	IMPORT_C void StoreL(CMsvStore& aMsvStore) const;
	IMPORT_C void RestoreL(const TFileName& aFileName);
	IMPORT_C void StoreL(const TFileName& aFileName) const;
	IMPORT_C void ResetL();
	
protected:
//	Parsing:
	IMPORT_C void UnfoldMessageL();

//	Streaming operations:
	void InternalizeL(RMsvReadStream& aStream);
	void ExternalizeL(RMsvWriteStream& aStream) const;

protected:
	/** Object that loaded the parser. It contains a reference counter of the use of 
	the parser. */
	CRegisteredParserDll&		iRegisteredParserDll;
	/** The message entry the parser should parse. */
	CMsvEntry&					iEntry;
	/** Connected file server handle. */
	RFs&						iFs;

	/** Lexer intended for Smart Message use.
	
	This is not used by the base class. */
	TLex						iSms;
	/** Array of token-value pairs.
	
	Derived classes can use this for storing parsed information (if this is appropriate). */
	CArrayPtrSeg<CParsedField>* iParsedFieldArray;

	/** Flag intended for Smart Message use.
	
	This is not used by the base class. */
	TBool						iSmsParsed;
	/** ID of iEntry. */
	TMsvId						iEntryId;
	
	/** Pointer to message data.
	
	This is not set by the base class. */
	HBufC*						iSettings;
	/** Pointer to SMS data (intended for Smart Message use).
	
	This is not set by the base class. */
    HBufC*						iSmsBuf;                // Local copy of buffer passed to ParseL()
	/** Temporary pointer used by RestoreL(). */
	HBufC8*						iReadBuffer;			// used to restore data from file
	};

#endif

