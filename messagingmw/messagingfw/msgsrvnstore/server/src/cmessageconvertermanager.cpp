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


/**
 * HEADER FILES
 */

#include "cmsvversion0version1converter.h"
#include "cmsvversion1version2converter.h"
//#include "cmsvconverterwaiter.h"


// literals
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	_LIT(KMessagingDBName, "\\messaging.db");
#else
	_LIT(KMessagingDBName,"[1000484B]messaging.db");
	_LIT(KMessagingDBFilePath,"\\Private\\1000484b\\Mail2\\");
#endif


/**
NewL()
Constructs a new message converter object. This is singleton instatiation.	
   
@param aDriveNumber:
@return None
@internalComponent
*/
CMessageConverterManager* CMessageConverterManager::NewL(TDriveNumber aDriveNumber)

	{
	CMessageConverterManager* self = new(ELeave) CMessageConverterManager(aDriveNumber);
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop(self);
	return self;
	}

/**
StartConversion()
Fetches the drive number requested for conversion, constructs converter manager object
and initiates the conversion for that drive.

@param aDrive: Requested Drive for conversion.
@return TInt: System wide error codes.
@internalComponent
*/
TInt CMessageConverterManager::StartConversion(TAny* aDrive)
	{
    __UHEAP_MARK;
     CTrapCleanup* cleanup = CTrapCleanup::New();
   	if(!cleanup) 
    	{ 
   		return KErrNoMemory; 
        } 
      
    
    TInt err= KErrNone;
    TDriveNumber driveNum = *(static_cast<const TDriveNumber*>(aDrive));
	CMessageConverterManager* self = NULL;
	TRAP(err, self = CMessageConverterManager::NewL(driveNum));  
	
	// do the conversion
	if(!err)
		{
		CleanupStack::PushL(self);
		TRAP(err , self->DoConversionL());
		CleanupStack::PopAndDestroy();
		}
	
	delete  cleanup;
	__UHEAP_MARKEND;
	
	return err;
	}

/**
CMessageConverterManager()
Default constructor

@param None
@return None
@internalComponent
*/
CMessageConverterManager::CMessageConverterManager(TDriveNumber aDriveNumber):iDrive(aDriveNumber)
	{
	}

/**
ConstructL()
Second phase constructor.

@param aDriveNumber:
@param conversionResume:
@return None
@internalComponent
*/
void CMessageConverterManager::ConstructL()
	{
	User::LeaveIfError(ifsSession.Connect()); 
	iConversionTablePresent = ETrue;
	iDeleteDatabase = -1;
	iInitialVersion = -1;
	}

/**
 ~CMessageConverterManager()
   Destructor
   
 @param None
 @return None
 @internalComponent
 */
CMessageConverterManager::~CMessageConverterManager()
	{
	delete iMessageDbAdapter;
	delete iDbAdapter;
	
	if(iDeleteDatabase == 0)
		{
		TRAP_IGNORE(RollBackVersion0Version1ConversionL());
		}
	else if(iDeleteDatabase == 1)
		{
		TRAP_IGNORE(RollBackVersion1Version2ConversionL());
		}
	
	}

/**
DoConversionL()
Initialises message store conversion and starts conversion between the required versions.	

@param None
@return None
@internalComponent
*/
void CMessageConverterManager::DoConversionL()
	{
	// Start from beginning or Resume
	InitialiseMessageStoreConversionL();
	
	switch(iConversionVersion)
		{
		case 1:  // version 0 version 1 conversion
			//Start from beginning or resume Resume v0 to v1
			{
			iInitialVersion = 0;
			
			RFile file;
			TInt error = file.Open(ifsSession, iFilePath, EFileShareAny|EFileWrite);
			
			// check disk space
			TVolumeInfo info;
			ifsSession.Volume(info,iDrive);
			
			TInt indexFileSize;
			file.Size(indexFileSize);
			
			TInt reqDiskSpace;
			reqDiskSpace = indexFileSize * 2;
		
			file.Close();
			ifsSession.Close();		
			
			if(reqDiskSpace * 2 < info.iFree)
				{
													
				iIndexFileConverter = CMsvVersion0Version1Converter::NewL(*iDbAdapter,iDrive,iConversionResume);
				CleanupStack::PushL(iIndexFileConverter);
								
				TRAP(error,iIndexFileConverter->ConvertMessageStoreL());
				
				if(error != KErrNone)
					{
					iDeleteDatabase = 0;
					User::Leave(error);
					}
								
				if(iConversionResume)
					{
					iConversionResume = EFalse;
					}
				CleanupStack::PopAndDestroy();  //iIndexFileConverter
		
				iDbAdapter->ChangeVersionL(1);  //We have a version 1 db now
				}
			else // disk check failed. Lets leave conversion status table if there is one.
				{
				User::Leave(KErrDiskFull);
				}
			}
	
		case 2:	// version 1 version 2 conversion
			{
			if(iInitialVersion !=0 )
				{
				iInitialVersion = 1;
				}
				
			iMessageConverter = CMsvVersion1Version2Converter::NewL(*iDbAdapter,*iMessageDbAdapter,iDrive,iConversionResume);
			CleanupStack::PushL(iMessageConverter);
					
			TRAPD(error,iMessageConverter->ConvertMessageStoreL());
			if(error)
				{
				if(iInitialVersion == 1)
					{
					iDeleteDatabase = 1;
					}
				else
					{
					iDeleteDatabase = 0;
					}
				User::Leave(error);
				}
						
			CleanupStack::PopAndDestroy(iMessageConverter);  //iMessageConverter
			
			iDbAdapter->ChangeVersionL(2);
			}
		}
		// We are done. Delete the status table.
		iDbAdapter->RemoveConversionStatusTableL();
		
		// remove index file
		RemoveIndexFileL(iDrive);
	}

/**
InitialiseMessageStoreConversionL()
Checks if the conversion is to be resumed. If yes, than identifies the message store version
to be converted.
If its a new conversion request than it creates the necessary tables and initiases the adapter
classes accordingly.

@param None
@return None
@internalComponent
*/	
void CMessageConverterManager::InitialiseMessageStoreConversionL()
	{
	TParse parse;
	TPtrC drive = TDriveUnit(iDrive).Name();
	parse.Set(KMessagingDBName, &drive, NULL);
	iDBFileName = parse.FullName();
	
	iFilePath.Append(drive);
	iFilePath.Append(KIndexFilePath);
	
	iConversionResume = ResumeConversionL();
	/*
	iConversionResume can have the following conditions set.
	ETrue  -  Conversion is to be resumed. 
	EFalse -  New conversion Request.
	*/		
	if(iConversionResume)
		{
		if(iConversionTablePresent)
			{
			iDbAdapter = CMsvDBAdapter::OpenL(iDBFileName);	// open the db	
			
			// get resumption point from conversion resume table.
			iDbAdapter->GetConversionStatusL(iResumeStatus); 

			if(iResumeStatus.targetVersion == 1)
				{
				iConversionVersion = 1;
				}
			else
				{
				iConversionVersion = 2;
				}
			}
		else
			{
			// new request for version 1 version 2 conversion.
			// Index file already migrated to SQlite DB.
			iDbAdapter = CMsvDBAdapter::OpenL(iDBFileName);	// open the db	
			iDbAdapter->CreateConversionStatusTableL();
			iConversionVersion = 2;
			iConversionResume = EFalse;
			}
		}
	else
		{
		// New request for version 1 version 2 conversion.
		// Index file still in file.
		
		CMsvDBAdapter::CreateDBL(iDrive);
		iDbAdapter = CMsvDBAdapter::OpenL(iDBFileName);
		iDbAdapter->ConstructSortTableL();
		iDbAdapter->CreateConversionStatusTableL();
		iConversionVersion = 1;
		}
	iDbAdapter->InitializeL();
	// change to invalid versionss
	iDbAdapter->ChangeVersionL(-1);
	// Get a handle to message adapter	
	iMessageDbAdapter = CMsvMessageDBAdapter::NewL(iDbAdapter,ETrue);	
	// Our handles are created . and we are good to start
	}

/**
ResumeConversionL()
Checks if the conversion is to be resumed by looking for conversion status table.

@param None
@return TBool :Boolean flag specifying if conversion is to be resumed.
@internalComponent
*/	
TBool CMessageConverterManager::ResumeConversionL()
	{
	// DB exists, Check for conversion Status Table
	_LIT16(KFindConversionStatusTableQuery, "SELECT COUNT(*) FROM SQLITE_MASTER WHERE NAME LIKE 'ConversionStatus';");
	
	RSqlDatabase temp;
	CleanupClosePushL(temp);

	TRAPD(err , temp.OpenL(iDBFileName));
	
	if(err == KErrNone)
		{
		// Database found. Check if are resuming.		
		RBuf16 headerTableQuery;
		CleanupClosePushL(headerTableQuery);
		headerTableQuery.CreateL(80);

		headerTableQuery.Append(KFindConversionStatusTableQuery);
			
		TInt count = 0;
		TSqlScalarFullSelectQuery query(temp);
		count = query.SelectIntL(headerTableQuery);
		
		headerTableQuery.Close();
		temp.Close();
		CleanupStack::PopAndDestroy(); //headerTableQuery
		CleanupStack::PopAndDestroy(); // temp
		
		if(count != 1)
			{
			 // we did not find the status table. version 1 version 2 conversion
			iConversionTablePresent = EFalse;			
			}
		// version 1 version 2 resume.
		return ETrue;	
		}
	// database is not present. This is version 0 to version 1 conversion
	temp.Close();
	CleanupStack::PopAndDestroy(); // temp
	return EFalse;
	}
	
/**
RemoveIndexFileL()
Removes the index file. This a called once the conversion completes successfully for a drive.

@param aDrive: Drive Number.
@return None
*/	
void CMessageConverterManager::RemoveIndexFileL(TDriveNumber aDrive)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);		
	TPtrC drive(TDriveUnit(aDrive).Name());
		
	RBuf path;
	CleanupClosePushL(path);	
	path.CreateL(KMaxPath);
		
	path.Append(drive);
	path.Append(KIndexFilePath);

	TInt error = fs.Delete(path);			
	CleanupStack::PopAndDestroy(2); // path	
	}

/**
RollBackVersion0Version1ConversionL()
Rolls back any changes done during version 0 to version 1 conversion

@param None: 
@return None
@internalComponent
*/	
void CMessageConverterManager::RollBackVersion0Version1ConversionL()
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);		
	
	TPtrC drive(TDriveUnit(iDrive).Name());
	
	RBuf path;
	CleanupClosePushL(path);	
	path.CreateL(KMaxPath);
	
	path.Append(drive);
	
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		path.Append(KMessagingDBName);
	#else
		{
		path.Append(KMessagingDBFilePath);
		path.Append(KMessagingDBName);
		}
	#endif
	//TInt err = fs.Delete(path);			
	TInt err = RSqlDatabase::Delete(path);
	CleanupStack::PopAndDestroy(2); // path	,fs

	fs.Close();	
	}

/**
RollBackVersion1Version2ConversionL()
Rolls back any changes done during version 1 to version 2 conversion

@param None: 
@return None
@internalComponent
*/	
void CMessageConverterManager::RollBackVersion1Version2ConversionL()
	{
	RSqlDatabase temp;
	CleanupClosePushL(temp);
	
	TPtrC drive(TDriveUnit(iDrive).Name());
	
	RBuf path;
	CleanupClosePushL(path);	
	path.CreateL(KMaxPath);
	
	path.Append(drive);
	
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		path.Append(KMessagingDBName);
	#else
		{
		path.Append(KMessagingDBFilePath);
		path.Append(KMessagingDBName);
		}
	#endif

	temp.OpenL(path);
	
	// lets delete the tables we have created.
	
	_LIT16(KDropTableQuerySmtp, "DROP table if exists Header_268439592;");
	_LIT16(KDropTableQueryPop, "DROP table if exists Header_268439593;");
	_LIT16(KDropTableQueryImap, "DROP table if exists Header_268439594;");
	_LIT16(KDropTableQuerySms, "DROP table if exists Header_268439596;");
	_LIT16(KDropTableQueryConversionStatus, "DROP table if exists ConversionStatus;");
			
	TInt err = temp.Exec(KDropTableQuerySmtp);
	err = temp.Exec(KDropTableQueryPop);
	err = temp.Exec(KDropTableQueryImap);
	err = temp.Exec(KDropTableQuerySms);
	err = temp.Exec(KDropTableQueryConversionStatus);	
	
	temp.Close();
	CleanupStack::PopAndDestroy(2); // temp	
	}
