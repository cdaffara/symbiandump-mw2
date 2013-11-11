// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMESSAGECONVERTERMANAGER_H__
#define __CMESSAGECONVERTERMANAGER_H__

//  Header files
#include <e32base.h>// CBase
#include <e32std.h>	// TBuf
#include "f32file.h"
#include "msvdbadapter.h"
#include "msvmessagedbadapter.h"



// fwd declarations
class CMsvVersion0Version1Converter;
class CMsvVersion1Version2Converter;
class CMsvDBAdapter;


//constants
const TUid KUidMsvSmsMtm 			= { 0x1000102c };
const TUid KUidMsgTypePOP3			= {0x10001029};	// 268439593
const TUid KUidMsgTypeIMAP4			= {0x1000102A};	// 268439594
const TUid KUidMsgTypeSMTP			= {0x10001028};	// 268439592

//Literals
_LIT(KIndexFilePath,"\\Private\\1000484b\\Mail2\\Index");


/**
TConversionResumeStatus
A structure having fields as member variables that are required to mainatin and communicate
caonversion status information

@internalAll 
@released
*/
struct TConversionResumeStatus
	{
	TMsvId lastId;
	TMsvId serviceId;
	TInt sourceVersion;
	TInt targetVersion;
	TUid mtmId;
	TInt remainingCount;
	};

/**
MMsvMessageStoreConverter
A pure virtual class that acts a base class for different version message store conversion
classes. Any class that converts messages from one verion to another must inherit from this.

@internalAll 
@released
*/
class MMsvMessageStoreConverter
	{
public:
	virtual void ConvertMessageStoreL() = 0;
	virtual ~MMsvMessageStoreConverter()
		{
		};
	};


/**
CMessageConverterManager
This class manages the conversion of message store from one version to another.
Starts a new conversion or resumes a conversion from the last point.
Owns and initiates conversion of version 0 and version 1 message store.

@internalAll 
@released
*/
NONSHARABLE_CLASS (CMessageConverterManager) : public CBase 
	{
	public:
		static CMessageConverterManager* NewL(TDriveNumber aDriveNumber);
		static TInt StartConversion(TAny* aDrive);
		~CMessageConverterManager();
		void DoConversionL() ;
		
	private: 
		CMessageConverterManager(TDriveNumber aDriveNumber);
		void ConstructL();
		TBool ResumeConversionL();	
		void InitialiseMessageStoreConversionL();
		void RemoveIndexFileL(TDriveNumber aDrive);
		void RollBackVersion0Version1ConversionL();
		void RollBackVersion1Version2ConversionL();		
	private: 
		 RFs ifsSession;
		 CMsvDBAdapter* iDbAdapter;
		 CMsvMessageDBAdapter* iMessageDbAdapter;
		 CMsvVersion0Version1Converter* iIndexFileConverter;
		 CMsvVersion1Version2Converter* iMessageConverter;
		 
		 TDriveNumber iDrive;
		 TInt iConversionVersion;
		 TConversionResumeStatus iResumeStatus;
		 TBool iConversionResume;
		 TBool iConversionTablePresent;
		 TBuf<64> iFilePath;
		 TFileName iDBFileName;
		 TInt iInitialVersion;
		 TInt iDeleteDatabase;
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)	
	friend class CTestMessageConverterManager;
#endif
	};

#endif  // __CMESSAGECONVERTERMANAGER_H__




  

	

