// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// BIF.H
//
/**
 * @file 
 * BIO information file (BIF)
 *
 * @publishedAll
 * @released
 */
#ifndef __BIF_H__
#define __BIF_H__

#include <apmrec.h>

class CBifEntry;
		
/** 
 * Maximum length of ID text field.
 * @publishedAll
 * @released
 */
const TInt KMaxBioIdText=128;

/** Data types.
 * Buffer to hold a BIF ID identifier text field.
 * @publishedAll
 * @released
 */
typedef TBuf<KMaxBioIdText> TBioMsgIdText;

/** Identifies bearers for the BIF ID type field. 
 * It is the C++ equivalent of the resource enum ID_TYPE.
 *
 * @publishedAll
 * @released
 */
enum TBioMsgIdType 
	{
	/** Unknown.
	
	Item will not be used in matching. */
	EBioMsgIdUnknown,

	/** MIME type.
	
	Specifies that a bearer that transports MIME messages (e.g. e-mail) should 
	match the MIME type against the ID text field. */
	EBioMsgIdIana,

	/** Narrow band (SMS).
	
	Specifies that the SMS bearer should match messages against the ID port field. */
	EBioMsgIdNbs,

	/** WAP WDP.
	
	Specifies that the WAP bearer should match messages against the ID port field. */
	EBioMsgIdWap,

	/** WAP secure WDP.
	
	Specifies that the WAP bearer should match messages against the ID port field. */
	EBioMsgIdWapSecure,

	/** WAP WSP.
	
	Specifies that the WAP bearer should match messages against the ID port field. */
	EBioMsgIdWsp,

	/** WAP secure WSP.
	
	Specifies that the WAP bearer should match messages against the ID port field. */
	EBioMsgIdWspSecure
	};

/** Encapsulates an ID from a BIF (BIO_INFO_FILE).
 *
 * @publishedAll
 * @released
 */
class TBioMsgId 
	{
public:
	/** Identifies the bearer that should use this resource to match messages. */
	TBioMsgIdType iType;
	/** Confidence level. */
	CApaDataRecognizerType::TRecognitionConfidence iConfidence;			// from APMREC.H
	/** Text field that can be used to match the message to this BIO type. */
	TBioMsgIdText iText;
	/** Receiving port number that can be used to match the message to this BIO type. */
	TUint16 iPort;
	/** Expected receiving character set of messages. */
	TUid iCharacterSet;
	/** This is available for 3rd party use for BIO message type identity specific 
	data. */
	TInt16 iGeneralIdData;
	};
	
// NOTE - this is included here cos the CBioInfoFile requires some of the enums 
// defined above.
#include "bifbase.h"			// for CBioInfoFile

/** BIO information file (BIF) reader. 

Allows reading (all but the icons) from an existing BIF file.

@publishedAll
@released
*/
class CBioInfoFileReader : public CBioInfoFile
	{
public:
	//
	// Construction/destruction
	IMPORT_C static CBioInfoFileReader* NewLC(RFs& aFs,const TDesC& aFileName,TUid aMsgTypeUid=KNullUid);
	IMPORT_C static CBioInfoFileReader* NewL(RFs& aFs,const TDesC& aFileName,TUid aMsgTypeUid=KNullUid);
	IMPORT_C virtual ~CBioInfoFileReader();

	//
	// Data getters
	IMPORT_C TUid MessageTypeUid() const;
	IMPORT_C const TPtrC MessageParserName() const;
	IMPORT_C TUid MessageAppUid() const;
	IMPORT_C TUid MessageAppCtrlUid() const;
	IMPORT_C const TPtrC MessageAppCtrlName() const;
	IMPORT_C TInt16 GeneralData1() const;
	IMPORT_C TInt16 GeneralData2() const;
	IMPORT_C TInt16 GeneralData3() const;
	IMPORT_C const TPtrC Description() const;			
	IMPORT_C const TPtrC FileExtension() const;			

	IMPORT_C const TPtrC IconsFilename() const;			
	IMPORT_C const CArrayFixFlat<TInt16>* ZoomLevelsLC() const;
	IMPORT_C TInt ZoomLevelsCount() const;			

	IMPORT_C const CArrayFixFlat<TBioMsgId>* IdsLC() const;
	
	IMPORT_C const CBifEntry& BifEntry() const;

private:
	CBioInfoFileReader(RFs& aFs);
	void ConstructL(const TDesC& aFileName, TUid aMsgTypeUid);

private:
	void InternalizeL(RReadStream& aStream);
	void LoadResourceL(TUid aMsgTypeUid);
	void LoadDataL(TUid aMsgTypeUid);
	TBool IsResFileL() const;
	TBool IsDatFile(const TUidType& aUidType) const;
    //
    HBufC*		iFileName;
    CBifEntry*	iEntry;
	};

#endif // __BIF_H__
