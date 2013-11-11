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
// bifbase.h
//
/**
 * @file 
 * Base classes for BIO-message Information File (BIF)
 *
 * @internalTechnology
 * @released
 */

#ifndef __BIFBASE_H__
#define __BIFBASE_H__

#include <e32base.h>
#include <s32std.h>				// for TSwizzle
#include <apmrec.h>				// for TRecognitionConfidence

// Constants
/** 
 * Indicates that when opening the message no application should be launched.
 *
 * @internalTechnology
 * @released
 */
const TUid KUidBioUseNoApp={0x00000000};			
/** 
 * Indicates that when opened the message is to be viewed by the default application.
 *
 * @internalTechnology
 * @released
 */
const TUid KUidBioUseDefaultApp={0x100052b2};

/** Base classes for BIO-message Information File (BIF) Reader

@internalTechnology
@released
*/
class CBioInfoFile : public CBase
	{
public:
	~CBioInfoFile();

protected:
	CBioInfoFile(RFs& aFs);
	void ConstructL();

protected:
	class CIdHeader : public CBase
		{
	public:
		CIdHeader();
		~CIdHeader();

		void InternalizeL(RReadStream& aStream);			
		void ExternalizeL(RWriteStream& aStream) const;		

	public:
		TBioMsgIdType iType;
		CApaDataRecognizerType::TRecognitionConfidence iConfidence;
		TSwizzle<HBufC> iText;
		TUint16 iPort;
		TUid iCharacterSet;
		TInt16 iGeneralIdData;
		};

	class TIconZoomLevel
		{
	public:
		void InternalizeL(RReadStream& aStream);			
		void ExternalizeL(RWriteStream& aStream) const;		

	public:
		TInt16 iZoomLevel;
		};

protected:
	RFs& iFs;

	//
	// Simple data to be read/write to/from BIF file
	TUid iMessageTypeUid;

	HBufC* iMessageParserName;
	
	TUid iMessageAppUid;
	HBufC* iMessageAppCtrlName;	
	HBufC* iDescription;
	TInt16 iGeneralData1;
	TInt16 iGeneralData2;
	TInt16 iGeneralData3;
	HBufC* iFileExtension;
	HBufC* iIconsFilename;
		
	//
	// Complex data to be read/write to/from BIF file
	CArrayPtrFlat<CIdHeader>* iIdHeaderArray;
	CArrayFix<TIconZoomLevel>* iIconZoomLevelArray;
	};

#endif	// __BIFBASE_H__
