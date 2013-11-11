// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// HEADER FILES
// 
//


/**
 * HEADER FILES
 */
#include <barsc.h>
#include <bautils.h>

#include "msvdbadapter.h"
#include "msventryfreepool.h"
#include "msvsearchsortdbwrapper.h"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "msvsearchsortconstants.h"
#endif

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	#include "cmessageconvertermanager.h"
#endif

/**
 * CONSTANT DEFINITION
 */
#define SERVER_UID 0x1000484B
#define KMsvNumStdFolder 7

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#include "msvindexadapter.h"
	
	//Main (dummy) DB name.
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		_LIT(KMainDummyDBName, "\\messaging_master.db");
		_LIT(KMessagingDBName, "\\messaging.db");
	#else
		_LIT(KMainDummyDBName, "[1000484B]messaging_master.db");
		_LIT(KMessagingDBName, "[1000484B]messaging.db");
	#endif

	_LIT(KLogicalName, "DB");
	_LIT(KComma, ", ");
	_LIT(KDelimiter, " ;");
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


/**
 * LITERAL DEFINITION
 */
//Data definition queries
_LIT8(KEncodingText, "encoding=UTF-8");
_LIT8(KCreateIndexEntryTableQuery,	"CREATE TABLE IF NOT EXISTS IndexEntry ("
								"id INTEGER PRIMARY KEY,"
						  		"parentId INT,"
								"serviceId INT,"
								"mtmId INT,"
								"type INT,"
								"date INT64,"
								"data INT,"
								"size INT,"
								"error INT,"
								"mtmData1 INT,"
								"mtmData2 INT,"
								"mtmData3 INT,"
								"relatedId INT,"
								"bioType INT,"
								"pcSyncCount INT,"
								"reserved INT,"
								"visibleParent INT,"
								"description TEXT,"
						  		"details TEXT" ");"
	  );
	  
_LIT8(KCreateIndexQuery, "CREATE INDEX IF NOT EXISTS INDEX_PARENT ON IndexEntry(parentId);");
_LIT8(KCreateVersionTableQuery, "CREATE TABLE IF NOT EXISTS VersionTable ( "
									"version INTEGER PRIMARY KEY );"
		);
	
//Common database queries
_LIT8(KGetEntryExistsQuery, "SELECT parentId FROM IndexEntry WHERE id = :id;"); //
_LIT8(KGetVisibleIdQuery, "SELECT data, visibleParent FROM IndexEntry WHERE id = :id;"); //
_LIT8(KGetEntryQuery, "SELECT parentId, serviceId, mtmId, type, date, data, size, error, "
						"mtmData1, mtmData2, mtmData3, relatedId, bioType, pcSyncCount, "
						"reserved, visibleParent, description, details FROM IndexEntry WHERE id = :id;"); //
_LIT8(KGetChildEntriesQuery, "SELECT id, serviceId, mtmId, type, date, data, size, error, "
							"mtmData1, mtmData2, mtmData3, relatedId, bioType, pcSyncCount, "
							"reserved, visibleParent, description, details FROM IndexEntry "
							"WHERE parentId = :parentId order by id ASC;"); //
		
//Data manipulation queries
_LIT8(KCreateEntryQuery,	 "INSERT INTO IndexEntry VALUES ("
							 ":id, :parentId, :serviceId, :mtmId, :type, :date, :data, :size, :error,"
							 ":mtmData1, :mtmData2, :mtmData3, :relatedId, :bioType, :pcSyncCount,"
							 ":reserved, :visibleParent, :description, :details);"
	 ); //
_LIT8(KUpdateEntryQuery,	"UPDATE IndexEntry SET "
							"parentId = :parentId, serviceId = :serviceId, mtmId = :mtmId, type = :type, date = :date,"
							"data = :data, size = :size, error = :error, mtmData1 = :mtmData1, mtmData2 = :mtmData2,"
							"mtmData3 = :mtmData3, relatedId = :relatedId, bioType = :bioType, pcSyncCount = :pcSyncCount,"
							"reserved = :reserved, visibleParent = :visibleParent, description = :description,"
							"details = :details WHERE id = :id;"
	 ); //
_LIT8(KUpdateEntryNoParentQuery,	"UPDATE IndexEntry SET "
							"serviceId = :serviceId, mtmId = :mtmId, type = :type, date = :date,"
							"data = :data, size = :size, error = :error, mtmData1 = :mtmData1, mtmData2 = :mtmData2,"
							"mtmData3 = :mtmData3, relatedId = :relatedId, bioType = :bioType, pcSyncCount = :pcSyncCount,"
							"reserved = :reserved, visibleParent = :visibleParent, description = :description,"
							"details = :details WHERE id = :id;"
	 ); //

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
_LIT8(KCreateConversionStatusTableQuery, 	"CREATE TABLE IF NOT EXISTS ConversionStatus ("
								"lastId INT,"
						  		"serviceId INTEGER PRIMARY KEY,"
								"sourceVersion INT,"
								"targetVersion INT,"
								"mtmId INT,"
								"remainingCount INT );"
	 );	

_LIT8(KRemoveConversionStatusTable,  "DROP TABLE IF EXISTS ConversionStatus;");					



#endif
	 
	 
//Literals for parameter indexes
_LIT(KMsgParamId, ":id");
_LIT(KMsgParamParentId, ":parentId");
_LIT(KMsgParamServiceId, ":serviceId");
_LIT(KMsgParamMtmId, ":mtmId");
_LIT(KMsgParamType, ":type");
_LIT(KMsgParamDate, ":date");
_LIT(KMsgParamData, ":data");
_LIT(KMsgParamSize, ":size");
_LIT(KMsgParamError, ":error");
_LIT(KMsgParamMtmData1, ":mtmData1");
_LIT(KMsgParamMtmData2, ":mtmData2");
_LIT(KMsgParamMtmData3, ":mtmData3");
_LIT(KMsgParamRelatedId, ":relatedId");
_LIT(KMsgParamBiotype, ":bioType");
_LIT(KMsgParamPcSyncCount, ":pcSyncCount");
_LIT(KMsgParamReserved, ":reserved");
_LIT(KMsgParamVisibleParent, ":visibleParent");
_LIT(KMsgParamDescriptionTxt, ":description");
_LIT(KMsgParamDetailsTxt, ":details");

//Literals for column indexes
_LIT(KMsgColumnId, "id");
_LIT(KMsgColumnParentId, "parentId");
_LIT(KMsgColumnServiceId, "serviceId");
_LIT(KMsgColumnMtmId, "mtmId");
_LIT(KMsgColumnType, "type");
_LIT(KMsgColumnDate, "date");
_LIT(KMsgColumnData, "data");
_LIT(KMsgColumnSize, "size");
_LIT(KMsgColumnError, "error");
_LIT(KMsgColumnMtmData1, "mtmData1");
_LIT(KMsgColumnMtmData2, "mtmData2");
_LIT(KMsgColumnMtmData3, "mtmData3");
_LIT(KMsgColumnRelatedId, "relatedId");
_LIT(KMsgColumnBiotype, "bioType");
_LIT(KMsgColumnPcSyncCount, "pcSyncCount");
_LIT(KMsgColumnReserved, "reserved");
_LIT(KMsgColumnVisibleParent, "visibleParent");
_LIT(KMsgColumnDescriptionTxt, "description");
_LIT(KMsgColumnDetailsTxt, "details");


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	_LIT8(KGetChildIdsQuery, "SELECT id, type, data FROM IndexEntry WHERE parentId = :parentId order by id ASC;"); //
	_LIT8(KClearTableContents, "DELETE FROM SearchSortEntry;");
#else
	_LIT8(KDeleteTable,  "DROP TABLE IF EXISTS SearchSortEntry;");
	_LIT8(KGetChildIdsQuery, "SELECT id FROM IndexEntry WHERE parentId = :parentId order by id ASC;"); //
	_LIT8(KGetInPreparationIdQuery, "SELECT id, data FROM IndexEntry;");
#endif



// Changes for PREQ 1667.
//Data definition queries for SearchSort-Table
_LIT8(KCreateSearchSortEntryTableQuery,	"CREATE TABLE IF NOT EXISTS SearchSortEntry ("
						"Qid INT,"
				  		"id INT,"
						"sortfield TEXT" ");"
);

_LIT8(KCreateSearchSortQuery, "CREATE UNIQUE INDEX IF NOT EXISTS SearchSortEntry_UNIQUE ON SearchSortEntry (Qid, id,sortfield);");

//Common database queries
_LIT8(KGetEntryAsc, "SELECT DISTINCT id FROM SearchSortEntry WHERE Qid = :Qid ORDER by sortfield ASC;");
_LIT8(KGetEntryDesc, "SELECT DISTINCT id FROM SearchSortEntry WHERE Qid = :Qid ORDER by sortfield DESC;");
_LIT8(KGetEntryNumAsc, "SELECT DISTINCT id FROM SearchSortEntry WHERE Qid = :Qid ORDER by CAST(sortfield AS INT) ASC;");
_LIT8(KGetEntryNumDesc, "SELECT DISTINCT id FROM SearchSortEntry WHERE Qid = :Qid ORDER by CAST(sortfield AS INT) DESC;");

_LIT8(KGetLastNIdsAsc, "SELECT DISTINCT id FROM SearchSortEntry WHERE ROWID > (LAST_INSERT_ROWID() - :RowId) AND Qid = :Qid ORDER by sortfield ASC;");
_LIT8(KGetLastNIdsDesc, "SELECT DISTINCT id FROM SearchSortEntry WHERE ROWID > (LAST_INSERT_ROWID() - :RowId) AND Qid = :Qid ORDER by sortfield DESC;");
_LIT8(KGetLastNIdsNumAsc, "SELECT DISTINCT id FROM SearchSortEntry WHERE ROWID > (LAST_INSERT_ROWID() - :RowId) AND Qid = :Qid ORDER by CAST(sortfield AS INT) ASC;");
_LIT8(KGetLastNIdsNumDesc, "SELECT DISTINCT id FROM SearchSortEntry WHERE ROWID > (LAST_INSERT_ROWID() - :RowId) AND Qid = :Qid ORDER by CAST(sortfield AS INT) DESC;");

_LIT8(KGetQueryCount, "SELECT COUNT(DISTINCT id) FROM SearchSortEntry WHERE Qid = :Qid");

_LIT8(KGetFolderIds, "SELECT id FROM IndexEntry WHERE parentId = :parentId AND type LIKE :type");

_LIT8(KGetIterResultCastAsc, "SELECT id FROM SearchSortEntry WHERE Qid = :qid ORDER by CAST(sortfield AS INT) ASC LIMIT :lim OFFSET :off ");
_LIT8(KGetIterResultCastDesc, "SELECT id FROM SearchSortEntry WHERE Qid = :qid ORDER by CAST(sortfield AS INT) DESC LIMIT :lim OFFSET :off ");
_LIT8(KGetIterResultAsc, "SELECT id FROM SearchSortEntry WHERE Qid = :qid ORDER by sortfield ASC LIMIT :lim OFFSET :off ");
_LIT8(KGetIterResultDesc, "SELECT id FROM SearchSortEntry WHERE Qid = :qid ORDER by sortfield ASC LIMIT :lim OFFSET :off ");

//Data manipulation queries
_LIT8(KDeleteId, 	 "DELETE FROM SearchSortEntry WHERE id = ");
_LIT8(KCreateQueryEntry,	 "INSERT OR IGNORE INTO SearchSortEntry VALUES ("
							 ":Qid, :id, :sortfield);");

_LIT8(KDeleteQueryId, 	 "DELETE FROM SearchSortEntry WHERE Qid = ");
_LIT8(KAndId, " AND id = ");
_LIT8(KSemiColon, ";");

//Literals for parameter indexes
_LIT(KQueryQId, ":Qid");
_LIT(KQueryTMsvId, ":id");
_LIT(KQuerySortfield, ":sortfield");
_LIT(KQueryRowId, ":RowId");
_LIT(KOffset, ":off");
_LIT(KLimit, ":lim");

//Literals for column indexes
_LIT(KQueryColumnTMsvId, "id");



/**
 * MEMBER FUNCTIONS OF CMsvDBAdapter
 */



/**
 * CMsvDBAdapter()
 * The default Constructor. 
 */
CMsvDBAdapter::CMsvDBAdapter():isTransactionOpen(EFalse)
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		,iDbPtrInitialized(EFalse)
#endif
	{
	}




/**
 * ~CMsvDBAdapter()
 * The default Destructor. 
 */
CMsvDBAdapter::~CMsvDBAdapter()
	{
	//Close all RSqlStatement object.
	for(TInt index = ECreateEntry; index < EIndexSentinel; index++)
		{
		iStatement[index].Close();
		}

	for(TInt index = EQueryCreateEntry; index < EQuerySentinel; index++)
		{
		iQueryStatement[index].Close();
		}
		
	iDatabase.Close();
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(iDatabasePresent)
		{
		iDatabasePresent->Close();
		delete iDatabasePresent;
		}
#endif
	}




// Code added in PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

/**
 * NewL()
 * @param None.
 * @return None.
 * 
 * Allocates and constructs a new DBAdapter object, calls ConstructL 
 * to create the dummy database and returns a handle to the DBAdapter.
 */
CMsvDBAdapter* CMsvDBAdapter::NewL()
	{
	//Create the DBAdapter object.
	CMsvDBAdapter* self = new(ELeave) CMsvDBAdapter();
	CleanupStack::PushL(self);
	
	// Create the database.
	self->ConstructL();
	
	CleanupStack::Pop(self);
	return self;
	
	}




/**
 * ConstructL()
 * @param None.
 * @return None.
 * 
 * Checks if there is the main (dummy) DB in the system drive
 * and creates one if it is not present. Other databases will
 * be attached to this DB which will not get deleted during
 * the operation of the message server.
 */
void CMsvDBAdapter::ConstructL()
	{
	//Get the system drive.
	TInt systemDrive = (TInt)RFs::GetSystemDrive();
	
	RBuf dbFile;
	CleanupClosePushL(dbFile);
	dbFile.CreateL(50);	
	dbFile.Append(TDriveUnit(systemDrive).Name());
	dbFile.Append(KMainDummyDBName);
	
	//Check if the main DB exists. If not, then create it.
	// > KErrNotFound, KErrPathNotFound - DB doesn't exist.
	// > KErrNone, KErrInUse - DB exists.
	TRAPD(err, iDatabase.OpenL(dbFile));
	if(KErrNone != err)
		{
		
		// Leave in unfavorable condition.
		if(KErrNoMemory == err ||
		   KErrNotReady == err ||
		   KErrInUse    == err
		  )
			{
			User::Leave(err);
			}

		HBufC8* heapBuf = KEncodingText().AllocLC();
		
		if(KSqlErrCorrupt == err || KSqlErrNotDb == err)
			{
			RSqlDatabase::Delete(dbFile);
			}
		
		//Create the dummy DB.		
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		// Create the non-secure database.
		User::LeaveIfError(iDatabase.Create(dbFile, heapBuf));
#else
		// Using SID based security policy.
		TSecureId sid(SERVER_UID);
		const TSecurityPolicy KDefaultPolicy(sid);
	
		//Create security policy
		RSqlSecurityPolicy securityPolicy;
		CleanupClosePushL(securityPolicy);	
		
		User::LeaveIfError(securityPolicy.Create(KDefaultPolicy));
		User::LeaveIfError(iDatabase.Create(dbFile, securityPolicy, heapBuf));
		CleanupStack::PopAndDestroy(); //securityPolicy
#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)

		CleanupStack::PopAndDestroy(); //heapBuf
		}
	CleanupStack::PopAndDestroy(); //dbFile
			
	//Construct the data structure to store maximum TMsvIds in
	//a given driveId.
	iDatabasePresent = new(ELeave)RArray<TBool>(KMaxNumberOfDrives);
	//Initialisation.
	for(TInt index = 0; index < KMaxNumberOfDrives; ++index)
		{
		iDatabasePresent->Append(EFalse);
		}
	
	}
	



/**
 * AttachDBL()
 * @param TDriveUnit The drive information of the drive to be checked.
 * @param TUint The driveId of the drive (0-7).
 * @param TMsvId& An output parameter that gives the maximum TMsvId of an 
 *				  entry in the messaging database of that drive.
 * 
 * Attaches a database on the specified drive to the main (dummy) 
 * database on the system drive so that appropriate queries 
 * access entries in that database too. The function calls MaxTMsvIdL()
 * to query the particular database to get the maximum TMsvId of an entry 
 * stored in it. This TMsvId is maintained in a data structure which has 
 * the driveId as the index.
 *
 * The database is detached by using its logical name which can
 * be obtained from the driveId.
 * (E.g.: logical name of a database on driveId 3 is 'DB3'. A query on the
 * IndexEntry in this database should address the table as DB3.IndexEntry.)
 */
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
void CMsvDBAdapter::AttachDBL(TDriveUnit aDriveUnit, TUint aDriveId, TMsvId& aMaxId, CMsvMessageDBAdapter* iMessageDBAdapter/*default= NULL*/)
#else
void CMsvDBAdapter::AttachDBL(TDriveUnit aDriveUnit, TUint aDriveId, TMsvId& aMaxId)
#endif
	{
	//Construct the logical name of the database to be attached from 
	//the driveId.
	RBuf logicalName;
	CleanupClosePushL(logicalName);
	logicalName.CreateL(5);	
	logicalName.Append(KLogicalName);
	logicalName.AppendNum(aDriveId);
		
	//Construct the DB filename appending messaging DB name to the drive name.
	RBuf dbFile;
	CleanupClosePushL(dbFile);
	dbFile.CreateL(50);	
	dbFile.Append(aDriveUnit.Name());
	dbFile.Append(KMessagingDBName);
	
	//Attach the database to the connection.
	User::LeaveIfError(iDatabase.Attach(dbFile, logicalName));
	
	//Find the parameter and column indexes just once.
	if((KErrNotFound == iDatabasePresent->Find(ETrue)) && (!iDbPtrInitialized))
		{
		InitializeL();
		iDbPtrInitialized = ETrue;
		}
		
	//Find the maximum TMsvId in the database and store it.
	aMaxId = MaxTMsvIdL(aDriveId);
	(*iDatabasePresent)[aDriveId] = ETrue;
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iMessageDBAdapter)
		{
		iMessageDBAdapter->UpdateMetadataStructureL(aDriveId);		
		}	
#endif

	CleanupStack::PopAndDestroy(2); //logicalName, dbFile	
	}

	

	
/**
 * DetachDBL()
 * @param TUint The driveId (0-7) of the drive from which the database
 * 				is to be detached.
 * @return None.
 *
 * Detaches a database on the drive specified by the driveId from
 * the main database connection. This is done when:
 * a. The disk is removed from the drive.
 * b. The drive is removed from the preferred drive list.
 *
 * The database is detached by using its logical name which can
 * be obtained from the driveId.
 * (E.g.: logical name of a database on driveId 3 is 'DB3'.)
 */
void CMsvDBAdapter::DetachDBL(TUint aDriveId)
	{
	//Construct the logical name.
	RBuf logicalName;
	CleanupClosePushL(logicalName);
	logicalName.CreateL(5);	
	logicalName.Append(KLogicalName);
	logicalName.AppendNum(aDriveId);
	
	User::LeaveIfError(iDatabase.Detach(logicalName));
	
	CleanupStack::PopAndDestroy(); //logicalName
	
	(*iDatabasePresent)[aDriveId] = EFalse;
	
	}




/**
 * CreateDBL()
 * @param TDriveNumber The drive number of the drive on which a 
 *						a new messaging database is to be created.
 * @return None.
 *
 * Creates the messaging database on the drive specified by the TDriveNumber
 * using a temporary local database connection.
 * Also creates the IndexEntry table and an index on the parentId field in it,
 * and then the Root entry in the database.
 * Also creates a version table, which is a database with just one column
 * and row which stores the current database version for message store
 * versioning.
 */
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvDBAdapter::CreateDBL(TDriveNumber aDriveNumber, TBool aCreateVersionTable /*Default=ETrue*/, TBool aInvalid /*Default=EFalse*/)
#else
void CMsvDBAdapter::CreateDBL(TDriveNumber aDriveNumber)
#endif
	{
	//Construct the database filename.
	TParse parse;
	TPtrC drive = TDriveUnit(aDriveNumber).Name();
	parse.Set(KMessagingDBName, &drive, NULL);
	TFileName dbFileName = parse.FullName();
	
	RSqlDatabase tempDB; //A temporary local database connection.
	CleanupClosePushL(tempDB);

	HBufC8* heapBuf = KEncodingText().AllocLC();

	//Create the database.
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	User::LeaveIfError(tempDB.Create(dbFileName, heapBuf));
#else	
	// Using SID based security policy.
	TSecureId sid(SERVER_UID);
	const TSecurityPolicy KDefaultPolicy(sid);
	
	//Create security policy
	RSqlSecurityPolicy securityPolicy;
	CleanupClosePushL(securityPolicy);	
		
	User::LeaveIfError(securityPolicy.Create(KDefaultPolicy));
	User::LeaveIfError(tempDB.Create(dbFileName, securityPolicy, heapBuf));
	
	CleanupStack::PopAndDestroy(); //securityPolicy
#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)

	CleanupStack::PopAndDestroy(); //heapBuf
	
	//Create tables for the database
	TInt err = tempDB.Exec(KCreateIndexEntryTableQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
		
	//Create index on indexEnty table.
	//Index is created on parentId.
	err = tempDB.Exec(KCreateIndexQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	
	// Create root entry in DB.
	CreateRootEntryL(tempDB);	

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	if(aCreateVersionTable)
		{
#endif
	//Create the version table to store the message store version.
	err = tempDB.Exec(KCreateVersionTableQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	//Insert the version number into the above table.
	_LIT(KInsertVersionNumberQuery, " INSERT INTO VersionTable Values ( " );
	RBuf8 versionQuery;
	CleanupClosePushL(versionQuery);
	versionQuery.CreateL(50);
	versionQuery.Append(KInsertVersionNumberQuery);

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	if(aInvalid) //create an invalid database wrt version for testing
		versionQuery.AppendNum(999);
	else
		versionQuery.AppendNum(KCurrentDatabaseVersion);	
#else
	versionQuery.AppendNum(KCurrentDatabaseVersion);	
#endif

	versionQuery.Append(_L(");"));
	err = tempDB.Exec(versionQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}

	CleanupStack::PopAndDestroy();	//versionQuery
		
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)		
		}
#endif
	
	CleanupStack::PopAndDestroy(); // tempDB
	}
 
 

/**
 * CreateRootEntryL()
 *
 * Creates root entry in the DB.
 */
void CMsvDBAdapter::CreateRootEntryL(RSqlDatabase& tempDB)
	{
	_LIT(KCreateRootEntryQuery, "INSERT INTO IndexEntry VALUES (");
	_LIT(KComma, ", ");
	_LIT(KDelimiter, " );");
	
	TMsvEntry rootEntry;
	rootEntry.iType = KUidMsvRootEntry;
	rootEntry.iMtm = KUidMsvRootEntry;
	rootEntry.iDate.UniversalTime();
	rootEntry.SetId(KMsvRootIndexEntryId);
	rootEntry.SetParent(KErrNotFound);
	rootEntry.SetOwner(ETrue);
	rootEntry.iSize = 0;
	
	RBuf queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(300);	
	queryBuf.Append(KCreateRootEntryQuery);
	queryBuf.AppendNum(rootEntry.iId);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.Parent());
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iServiceId);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iMtm.iUid);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iType.iUid);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iDate.Int64());
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iData);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iSize);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iError);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iMtmData1);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iMtmData2);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iMtmData3);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iRelatedId);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iBioType);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iPcSyncCount);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(rootEntry.iReserved);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(KMsvRootIndexEntryId);
	queryBuf.Append(KComma);
	queryBuf.Append(_L("\"\""));
	queryBuf.Append(KComma);
	queryBuf.Append(_L("\"\""));
	queryBuf.Append(KDelimiter);
	
	TInt err = tempDB.Exec(queryBuf);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(); //queryBuf	
	}
	
	
	

/**
 * MaxTMsvIdL()
 * @param TDriveNumber The drive number of the drive to be queried.
 * @return None.
 *
 * Queries the specified drive's database for the maximum TMsvId
 * of an entry stored in it.
 */
TMsvId CMsvDBAdapter::MaxTMsvIdL(TUint aDriveId)
	{	 
	_LIT(KMaxTMsvIdQuery1, "SELECT MAX(id) FROM DB");
	_LIT(KMaxTMsvIdQuery2, ".IndexEntry;");
	
	RBuf queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(50);	
	
	queryBuf.Append(KMaxTMsvIdQuery1);
	queryBuf.AppendNum(aDriveId);
	queryBuf.Append(KMaxTMsvIdQuery2);
	
	//Create a select query
    TSqlScalarFullSelectQuery query(iDatabase);

	//Get the column data
    TMsvId maxTMsvId = query.SelectIntL(queryBuf);
    
    CleanupStack::PopAndDestroy(); //queryBuf
    
    if(KMsvRootIndexEntryId != maxTMsvId)
    	maxTMsvId = MaskTMsvId(aDriveId, maxTMsvId);
    return maxTMsvId;
	}




/**
 * ValidateDatabaseL()
 * @param TDriveNumber The drive number of the drive to be checked.
 * @return None.
 *
 * Validates a database by checking if:
 * a. The database file is in the appropriate location.
 * b. The database is readable and not corrupt.
 * c. The database is of latest supported version.
 * d. All standard folders (as specified in the messaging resource file)
 *    exist in the database.
 */
void CMsvDBAdapter::ValidateDatabaseL(TDriveNumber aDriveNumber)
	{
	//Check (a) and (b).
	TParse parse;
	TPtrC drive(TDriveUnit(aDriveNumber).Name());
	parse.Set(KMessagingDBName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	
	RSqlDatabase tempDB;
	CleanupClosePushL(tempDB);
	tempDB.OpenL(dbFile);
		
	//Check (d) with the connection still open.
	//Entries to be checked are Root, Local, Inbox, Outbox, Drafts,
	//Sent and Deleted.
	_LIT(KStandardEntryExistsQuery1, "SELECT count(*) FROM IndexEntry WHERE id <= ");
	RBuf queryBuf;	
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(150);
	queryBuf.Append(KStandardEntryExistsQuery1);
	queryBuf.AppendNum(KMsvDeletedEntryFolderEntryId);
	queryBuf.Append(KDelimiter);
	
	TSqlScalarFullSelectQuery query(tempDB);
	TInt folders = 0;
	folders = query.SelectIntL(queryBuf);
	
	//The database is invalid if the number of folders returned
	//from the above query is not 7.
	if(folders != KMsvNumStdFolder) //Number of standard folders + root. To be put in a macro or const
		{
		User::Leave(KSqlErrCorrupt); 
		}
	CleanupStack::PopAndDestroy(); //queryBuf	
	
	//(a), (b) and (d) are fine. Check (c).
	//Check the version number against the expected version number.
	//PREQ557 version - 1; Post-557 version - 2.
	_LIT(KSelectVersionQuery, "SELECT version FROM VersionTable;");
	TInt version = 0;
	
	version = query.SelectIntL(KSelectVersionQuery);
	if(version != KCurrentDatabaseVersion)
		{
		User::Leave(KErrNotSupported);
		}
	
	CleanupStack::PopAndDestroy();  // tempDB
	}




/**
 * GetVisibleFolderIdL()
 * @param TMsvId: Parent Id.
 * @param TMsvId&: Out parameter for most immediate visible folder id of
 *				   any child entry present under passed parent id.
 * @return None. 
 *
 * This functions returns the most immediate parent visible folder id.
 * There are two types of folders. A visible folder is one which is 
 * visible to the external user, but there are internal non-visible folders
 * created under each email message to organize the content of the email.
 * Parent entry of an entry is either root or a folder. A folder can be 
 * service folder (like LocalServices), user-defined folder/standard folder
 * like Inbox or a non-visible folder (created under an email message).
 * 
 * For a given parent Id, the function returns the most immediate visible
 * parent folder. The rules are as follows:
 * 1. If the parent entry is root or children of root, then the visibleFolder 
 * will be root itself. This is because service folders are considered 
 * invisible, since they are not seen by user. 
 * 2. If the parent entry itself is a visible folder then child's visibleFolder
 * id should be its parent id.
 * 3. If the parent entry is a non-visible folder then child's visibleFolder
 * id is same as its parent's visibleFolder id.
 */
void CMsvDBAdapter::GetVisibleFolderIdL(TMsvId aParentId, TMsvId& aVisibleFolderId)
	{
	// If the entry is either a root entry (parent=KErrNotFound) or 
	// child of root entry its visibleFolderId will be root entry itself.
	if(KErrNotFound == aParentId)
		{
		aVisibleFolderId = KMsvRootIndexEntryId;
		return;		
		}

	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aParentId);
	aParentId = UnmaskTMsvId(aParentId);

	if(KMsvRootIndexEntryId == aParentId)
		{
		aVisibleFolderId = KMsvRootIndexEntryId;
		}
	else
		{
		//Construct a query for the particular drive.
		_LIT8(KGetVisibleIdQuery1, "SELECT data, visibleParent FROM DB");
		_LIT8(KGetVisibleIdQuery2, ".IndexEntry WHERE id = ");
		
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(70);		
		queryBuf.Append(KGetVisibleIdQuery1);
		queryBuf.AppendNum(driveId);
		queryBuf.Append(KGetVisibleIdQuery2);
		queryBuf.AppendNum(aParentId);
		queryBuf.Append(KDelimiter);
		
		RSqlStatement visQuery;
		CleanupClosePushL(visQuery);
		User::LeaveIfError(visQuery.Prepare(iDatabase, queryBuf));
		TInt dataParam = GetColumnIndexL(visQuery, KMsgColumnData());
		TInt visParam = GetColumnIndexL(visQuery, KMsgColumnVisibleParent());
		
		if (KSqlAtRow != visQuery.Next())
			{
			User::Leave(KErrNotFound);
			}
		
		TInt32 data = ColumnInt(visQuery, dataParam);
		if( data & TMsvEntry::KMsvVisibleFolderNodeFlag)
			{
			// If visible folder flag is not set for parent entry.
			aVisibleFolderId = ColumnInt(visQuery, visParam);
			if(!IsStandardId(aVisibleFolderId))
				{
				aVisibleFolderId = MaskTMsvId(driveId, aVisibleFolderId);
				}
			}
		else
			{
			// If parent entry is visible
			if(IsStandardId(aParentId))
				{
				aVisibleFolderId = aParentId;				
				}
			else
				{
				aVisibleFolderId = MaskTMsvId(driveId, aParentId);
				}
			}		
		CleanupStack::PopAndDestroy(2); //visQuery, queryBuf
		}
	}




/**
 * CreateEntryL()
 * @param const TMsvEntry&: The entry to be created.
 * @param const TMsvId: Immediate visible parent folder of the entry
 * @return None.
 *
 * The function creates a new entry in the database.
 * No consisitency check is performed on the passed entry. The function may
 * leave if query execution fails.
 */
void CMsvDBAdapter::CreateEntryL(const TMsvEntry& aNewEntry, const TMsvId aImmediateVisibleFolder)
	{
	_LIT8(KCreateEntryQuery1,	 "INSERT INTO DB");
	_LIT8(KCreateEntryQuery2, ".IndexEntry VALUES ("
							  ":id, :parentId, :serviceId, :mtmId, :type, :date, :data, :size, :error,"
							  ":mtmData1, :mtmData2, :mtmData3, :relatedId, :bioType, :pcSyncCount,"
							  ":reserved, :visibleParent, :description, :details);"
		 );

	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aNewEntry.iId);
	TMsvId id = UnmaskTMsvId(aNewEntry.iId);
	TMsvId parentId = UnmaskTMsvId(aNewEntry.iParentId);
	TMsvId visFolder = UnmaskTMsvId(aImmediateVisibleFolder);
	TMsvId serviceId = UnmaskTMsvId(aNewEntry.iServiceId);
	TMsvId relatedId = UnmaskTMsvId(aNewEntry.iRelatedId);
	
	//Construct a query for the particular drive.
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(350);
	queryBuf.Append(KCreateEntryQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KCreateEntryQuery2);
	iStatement[ECreateEntry].Close(); //closing any statement prepared previously
	User::LeaveIfError(iStatement[ECreateEntry].Prepare(iDatabase, queryBuf));
	
	//Bind the entry data
	BindIntL(iStatement[ECreateEntry], iIdIndex[ECreateEntry], id);
	BindIntL(iStatement[ECreateEntry], iParentIdIndex[ECreateEntry], parentId);
	BindIntL(iStatement[ECreateEntry], iDataIndex[ECreateEntry], aNewEntry.iData);
	BindIntL(iStatement[ECreateEntry], iPcSyncCountIndex[ECreateEntry], aNewEntry.iPcSyncCount);
	BindIntL(iStatement[ECreateEntry], iReservedIndex[ECreateEntry], aNewEntry.iReserved);
	BindIntL(iStatement[ECreateEntry], iRelatedIdIndex[ECreateEntry], relatedId);
	BindIntL(iStatement[ECreateEntry], iTypeIndex[ECreateEntry], aNewEntry.iType.iUid);
	BindInt64L(iStatement[ECreateEntry], iDateIndex[ECreateEntry], aNewEntry.iDate.Int64());
	BindIntL(iStatement[ECreateEntry], iSizeIndex[ECreateEntry], aNewEntry.iSize);
	BindIntL(iStatement[ECreateEntry], iErrorIndex[ECreateEntry], aNewEntry.iError);
	BindIntL(iStatement[ECreateEntry], iBiotypeIndex[ECreateEntry], aNewEntry.iBioType);
	BindIntL(iStatement[ECreateEntry], iMtmData1Index[ECreateEntry], aNewEntry.iMtmData1);
	BindIntL(iStatement[ECreateEntry], iMtmData2Index[ECreateEntry], aNewEntry.iMtmData2);
	BindIntL(iStatement[ECreateEntry], iMtmData3Index[ECreateEntry], aNewEntry.iMtmData3);
	BindIntL(iStatement[ECreateEntry], iServiceIdIndex[ECreateEntry], serviceId);
	BindIntL(iStatement[ECreateEntry], iMtmIdIndex[ECreateEntry], aNewEntry.iMtm.iUid);
	BindIntL(iStatement[ECreateEntry], iVisibleParent[ECreateEntry], visFolder);
	BindTextL(iStatement[ECreateEntry],iDescriptionIndex[ECreateEntry],aNewEntry.iDescription);
	BindTextL(iStatement[ECreateEntry],iDetailsIndex[ECreateEntry],aNewEntry.iDetails);
	
	TInt err = iStatement[ECreateEntry].Exec();
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	iStatement[ECreateEntry].Close();
	CleanupStack::PopAndDestroy();
	}




/**
 * UpdateEntryL()
 * @param const TMsvEntry&: Entry to be updated.
 * @param const TMsvId: the visible folder Id of the updated entry.
 * @param TBool: ETrue if the parent of the entry has been changed,
 *				 EFalse if otherwise.
 * @return None.
 *
 * The function updates an entry in the database.
 * No consisitency check is performed on the passed entry. The function may
 * leave if query execution fails.
 */
void CMsvDBAdapter::UpdateEntryL(const TMsvEntry& aEntry, const TMsvId aImmediateVisibleFolder, TBool aIsParentIdUpdated/*DEFAULT=ETrue*/)
	{
	_LIT8(KUpdateEntryQuery1,	"UPDATE DB");
	_LIT8(KUpdateEntryQuery2, ".IndexEntry SET ");
	_LIT8(KUpdateEntryQueryWithParent,	"parentId = :parentId, ");
	_LIT8(KUpdateEntryQueryWithoutParent,	"serviceId = :serviceId, mtmId = :mtmId, type = :type, date = :date,"
							"data = :data, size = :size, error = :error, mtmData1 = :mtmData1, mtmData2 = :mtmData2,"
							"mtmData3 = :mtmData3, relatedId = :relatedId, bioType = :bioType, pcSyncCount = :pcSyncCount,"
							"reserved = :reserved, visibleParent = :visibleParent, description = :description,"
							"details = :details WHERE id = :id;"
		 );
	
	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aEntry.iId);
	TMsvId id = UnmaskTMsvId(aEntry.iId);
	TMsvId parentId = UnmaskTMsvId(aEntry.iParentId);
	TMsvId serviceId = UnmaskTMsvId(aEntry.iServiceId);
	TMsvId relatedId = UnmaskTMsvId(aEntry.iRelatedId);
	TMsvId visFolder = UnmaskTMsvId(aImmediateVisibleFolder);
	
	//Form the query for the particular drive.
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(500);	
	queryBuf.Append(KUpdateEntryQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KUpdateEntryQuery2);

	iStatementType updateStmt;
	TInt err = KErrNone;
	//Bind the entry data
	if(aIsParentIdUpdated)
		{
		updateStmt = EUpdateEntry;
		queryBuf.Append(KUpdateEntryQueryWithParent);
		queryBuf.Append(KUpdateEntryQueryWithoutParent);
		iStatement[updateStmt].Close(); //closing any statement prepared previously
		User::LeaveIfError(iStatement[updateStmt].Prepare(iDatabase, queryBuf));
		BindIntL(iStatement[updateStmt], iParentIdIndex[updateStmt], parentId);
		}
	else
		{
		updateStmt = EUpdateEntryNoParent;
		iStatement[updateStmt].Close(); //closing any statement prepared previously
		queryBuf.Append(KUpdateEntryQueryWithoutParent);
		User::LeaveIfError(iStatement[updateStmt].Prepare(iDatabase, queryBuf));
		}
	BindIntL(iStatement[updateStmt], iVisibleParent[updateStmt], visFolder);
	BindIntL(iStatement[updateStmt], iIdIndex[updateStmt], id);
	BindIntL(iStatement[updateStmt], iDataIndex[updateStmt], aEntry.iData);
	BindIntL(iStatement[updateStmt], iPcSyncCountIndex[updateStmt], aEntry.iPcSyncCount);
	BindIntL(iStatement[updateStmt], iReservedIndex[updateStmt], aEntry.iReserved);
	BindIntL(iStatement[updateStmt], iRelatedIdIndex[updateStmt], relatedId);
	BindIntL(iStatement[updateStmt], iTypeIndex[updateStmt], aEntry.iType.iUid);
	BindInt64L(iStatement[updateStmt], iDateIndex[updateStmt], aEntry.iDate.Int64());
	BindIntL(iStatement[updateStmt], iSizeIndex[updateStmt], aEntry.iSize);
	BindIntL(iStatement[updateStmt], iErrorIndex[updateStmt], aEntry.iError);
	BindIntL(iStatement[updateStmt], iBiotypeIndex[updateStmt], aEntry.iBioType);
	BindIntL(iStatement[updateStmt], iMtmData1Index[updateStmt], aEntry.iMtmData1);
	BindIntL(iStatement[updateStmt], iMtmData2Index[updateStmt], aEntry.iMtmData2);
	BindIntL(iStatement[updateStmt], iMtmData3Index[updateStmt], aEntry.iMtmData3);
	BindIntL(iStatement[updateStmt], iServiceIdIndex[updateStmt], serviceId);
	BindIntL(iStatement[updateStmt], iMtmIdIndex[updateStmt], aEntry.iMtm.iUid);
	BindTextL(iStatement[updateStmt], iDescriptionIndex[updateStmt],aEntry.iDescription);
	BindTextL(iStatement[updateStmt], iDetailsIndex[updateStmt],aEntry.iDetails);

	//Execute the prepared SQL statement.
	err = iStatement[updateStmt].Exec();
	if(err < 0)
		{
		User::Leave(err);
		}
	iStatement[updateStmt].Close();
	CleanupStack::PopAndDestroy(); //queryBuf
	}




/**
 * DeleteEntryL()
 * @param TMsvId: Entry Id to be deleted.
 *
 * The function deletes an entry with the passed Id. The function
 * may leave due to database error while execution. The function
 * does not leave if the entry does not exist.
 *
 * **WARNING** The function does NOT ensure that if a parent entry
 * is deleted all its child entries are also deleted.
 */
void CMsvDBAdapter::DeleteEntryL(TMsvId aEntryId)
	{
	_LIT8(KDeleteEntryQuery1, "DELETE FROM DB");
	_LIT8(KDeleteEntryQuery2, ".IndexEntry WHERE id = ");
		
	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aEntryId);
	aEntryId = UnmaskTMsvId(aEntryId);
	
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(100);	
	queryBuf.Append(KDeleteEntryQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KDeleteEntryQuery2);
	queryBuf.AppendNum(aEntryId);
	queryBuf.Append(KDelimiter);
	
	TInt err = iDatabase.Exec(queryBuf);
	CleanupStack::PopAndDestroy();
	if(err < 0)
		{		
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrNotFound);			
		}	
	}




/**
 * DeleteEntryL()
 * @param const CMsvEntrySelection&: Entry Id list to be deleted.
 * @return None.
 * 
 * The function deletes all entries mentioned in the passed Id list.
 * The function may leave due to database error while execution or
 * if an entry does not exist.
 *
 * **WARNING** The function does NOT ensure that if a parent entry
 * is deleted all its child entries are also deleted.
 */
void CMsvDBAdapter::DeleteEntryL(const CMsvEntrySelection& aEntryIdList)
	{	
	TInt count;
	if( 0 == (count = aEntryIdList.Count()) )
		{
		return;
		}

	_LIT(KQuery1, "DELETE FROM DB");
	_LIT(KQuery2, ".IndexEntry WHERE id IN (");
	_LIT(KEndDelimiter, ");");
	
	//Delete maximum 100 entries at a time so as to not overflow IPC buffer
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	TInt maxEntries = 50; //for testing
#else
	TInt maxEntries = 100;
#endif

	CMsvEntrySelection* entryList = aEntryIdList.CopyL(); //separate copy of the list to work on
	CleanupStack::PushL(entryList);
	
	for(TInt index1 = 0; index1<count; ++index1)
		{
		TInt driveId = GetDriveId(entryList->At(index1));
		TInt numDeleted = 1;
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(70 + ((10+2)*maxEntries));		
		queryBuf.Append(KQuery1);
		queryBuf.AppendNum(driveId);
		queryBuf.Append(KQuery2);
		queryBuf.AppendNum(UnmaskTMsvId(entryList->At(index1)));
		for(TInt index2 = index1 + 1; index2<count && numDeleted<maxEntries; ++index2)
			{
			if(driveId == GetDriveId(entryList->At(index2)))
				{
				queryBuf.Append(KComma);
				queryBuf.AppendNum(UnmaskTMsvId(entryList->At(index2)));
				++numDeleted;
				entryList->Delete(index2);
				--index2;
				--count;
				}
			}
		entryList->Delete(index1);
		--index1;
		--count;
		queryBuf.Append(KEndDelimiter);
		TInt err = iDatabase.Exec(queryBuf);
		CleanupStack::PopAndDestroy(); //queryBuf
		if(err < 0)
			{
			User::Leave(err);
			}
		}
	CleanupStack::PopAndDestroy(); //entryList
	}




/**
 * GetEntryL()
 * @param TMsvId: Id of the entry to be fetched.
 * @param CMsvCacheEntry*&: The newly fetched entry.
 * @param TMsvId&: Out parameter for value of VisibleParent column.
 * @return None.
 *
 * Fetch an entry from the database. Memory to aEntry
 * should not already be assigned.
 */
void CMsvDBAdapter::GetEntryL(TMsvId aId, CMsvCacheEntry*& aEntry, TMsvId& aVisibleParentEntryId)
	{
	_LIT8(KGetEntryQuery1, "SELECT parentId, serviceId, mtmId, type, date, data, size, error, "
						"mtmData1, mtmData2, mtmData3, relatedId, bioType, pcSyncCount, "
						"reserved, visibleParent, description, details FROM DB");
	_LIT8(KGetEntryQuery2, ".IndexEntry WHERE id = ");

	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aId);
	
	//Construct a query for the particular drive.
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	TInt len = KGetEntryQuery1().Length() + KGetEntryQuery2().Length() + 15;
	queryBuf.CreateL(len);	
	queryBuf.Append(KGetEntryQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KGetEntryQuery2);
	queryBuf.AppendNum(UnmaskTMsvId(aId));
	queryBuf.Append(KDelimiter);
	
	//Prepare a statement for the query.
	RSqlStatement getStmt;
	CleanupClosePushL(getStmt);
	User::LeaveIfError(getStmt.Prepare(iDatabase, queryBuf));
		
	if(KSqlAtRow != getStmt.Next())
		{
		User::Leave(KErrNotFound);
		}

	//Get the entry from the statement
	aEntry = CMsvEntryFreePool::Instance()->EntryL();
	CleanupStack::PushL(cacheEntry);
	aEntry->Entry().iId = aId;
	
	LoadEntryFromStatementL(getStmt, EGetEntry, aVisibleParentEntryId, *aEntry);
	
	//We do not return non-standard visible folders from a non-current drive's database.
	if(driveId != KCurrentDriveId 		&&
	   aEntry->Entry().Visible() 		&&
	   !IsStandardId(aEntry->GetId()) 	&&
	   KUidMsvFolderEntryValue == aEntry->Entry().iType.iUid
	  )
		{
		CMsvEntryFreePool::Instance()->ReleaseEntryL(aEntry);
		User::Leave(KErrNotFound);
		}
	
	// Unmask iId's of standard Ids.
	if(IsStandardId(aEntry->GetId()))
		{
		aEntry->Entry().SetId(UnmaskTMsvId(aEntry->GetId()));
		}
	
	// Mask iParentId if it is a non-standard id.
	if(!IsStandardId(aEntry->Entry().iParentId))
		{
		aEntry->Entry().iParentId = MaskTMsvId(driveId, aEntry->Entry().iParentId);
		}
	
	// Mask visible folder if it is a non-standard id.
	if(!IsStandardId(aVisibleParentEntryId))
		{
		aVisibleParentEntryId = MaskTMsvId(driveId, aVisibleParentEntryId);
		}
	
	// Mask iServiceId if it is a non-standard id.
	if(!IsStandardId(aEntry->Entry().iServiceId))
		{
		aEntry->Entry().iServiceId = MaskTMsvId(driveId, aEntry->Entry().iServiceId);
		}
	
	// Mask iRelatedId if it is a non-standard id.
	if(!IsStandardId(aEntry->Entry().iRelatedId))
		{
		aEntry->Entry().iRelatedId = MaskTMsvId(driveId, aEntry->Entry().iRelatedId);
		}

	CleanupStack::Pop(cacheEntry);
	CleanupStack::PopAndDestroy(2); //getStmt, queryBuf
	}




/**
 * GetChildrenL()
 * @param TMsvId: Parent Id of the child entries to be fetched.
 * @param RPointerArray<TMsvEntry>&: List of child entries to be filled.
 * @return None.
 *
 * Fetch child entries from the database for a given parent Id.
 * This does not check if the parent entry already exist. The 
 * entries will be added in ascending order of their TMsvId.
 */
void CMsvDBAdapter::GetChildrenL(TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aChildArray)
	{
	_LIT8(KGetChildEntriesQuery1, "SELECT id, serviceId, mtmId, type, date, data, size, error, "
							"mtmData1, mtmData2, mtmData3, relatedId, bioType, pcSyncCount, "
							"reserved, visibleParent, description, details FROM DB");
	_LIT8(KGetChildEntriesQuery2, ".IndexEntry WHERE parentId = ");
	_LIT8(KGetChildEntriesQuery3, " order by id ASC;");
	
	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aParentId);
	aParentId = UnmaskTMsvId(aParentId);
	
	//Construct the query for the particular drive.
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(350);	
	queryBuf.Append(KGetChildEntriesQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KGetChildEntriesQuery2);
	queryBuf.AppendNum(aParentId);
	queryBuf.Append(KGetChildEntriesQuery3);
	
	//Prepare a statement for the query.
	RSqlStatement getStmt;
	CleanupClosePushL(getStmt);
	User::LeaveIfError(getStmt.Prepare(iDatabase, queryBuf));
	
	while(KSqlAtRow == getStmt.Next())
		{
		// Do not return non-standard visible folders from non-current drive.
			// If it is a non-current drive...
		if( (driveId != KCurrentDriveId) 										     	  &&
			// If it is a folder entry.
		    (KUidMsvFolderEntryValue == ColumnInt(getStmt, iTypeIndex[EGetChildEntries])) &&
		    // If it is a visible folder.
			(IsVisibleFolder(ColumnInt(getStmt, iDataIndex[EGetChildEntries])))		      &&
			// If it is a non-standard folder.
			(!IsStandardId(ColumnInt(getStmt, iIdIndex[EGetChildEntries])))
		  )		  		
			{
			continue;
			}
			
		TMsvId aVisibleParentEntryId;
		CMsvCacheEntry *cacheEntry = CMsvEntryFreePool::Instance()->EntryL();
		CleanupStack::PushL(cacheEntry);
		if(!IsStandardId(aParentId))
			{
			aParentId = MaskTMsvId(driveId, aParentId);
			}
		cacheEntry->Entry().iParentId = aParentId;
		LoadEntryFromStatementL(getStmt, EGetChildEntries, aVisibleParentEntryId, *cacheEntry);
		
		//Add the drive mask to the TMsvId.
		if(!IsStandardId(cacheEntry->GetId()))
			{
			cacheEntry->Entry().iId = MaskTMsvId(driveId, cacheEntry->Entry().iId);
			}
		
		// Mask iServiceId if it is a non-standard id.
		if(!IsStandardId(cacheEntry->Entry().iServiceId))
			{
			cacheEntry->Entry().iServiceId = MaskTMsvId(driveId, cacheEntry->Entry().iServiceId);
			}
		
		// Mask iRelatedId if it is a non-standard id.
		if(!IsStandardId(cacheEntry->Entry().iRelatedId))
			{
			cacheEntry->Entry().iRelatedId = MaskTMsvId(driveId, cacheEntry->Entry().iRelatedId);
			}
	
		//Add the child entry to the array given
		aChildArray.AppendL(cacheEntry);
		CleanupStack::Pop(cacheEntry);
		}
	CleanupStack::PopAndDestroy(2); //getStmt, queryBuf
	}




/**
 * GetChildrenIdL()
 * @param TMsvId: Parent Id of the child entries to be fetched.
 * @param CMsvEntrySelection&: List of child entry Id's.
 * @return None.
 *
 * Fetch child entry Id's from the database for a given parent Id.
 * This does not check if the parent entry already exist. The 
 * entries will be added in ascending order of their TMsvId.
 */
void CMsvDBAdapter::GetChildrenIdL(TMsvId aParentId, CMsvEntrySelection& aChildArray)
	{
	_LIT8(KGetChildIdsQuery1, "SELECT id, type, data FROM DB");
	_LIT8(KGetChildIdsQuery2, ".IndexEntry WHERE parentId = ");
	_LIT8(KGetChildIdsQuery3, " order by id ASC;");
	
	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aParentId);
	aParentId = UnmaskTMsvId(aParentId);
	
	//Construct the query for the particular drive.
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(100);	
	queryBuf.Append(KGetChildIdsQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KGetChildIdsQuery2);
	queryBuf.AppendNum(aParentId);
	queryBuf.Append(KGetChildIdsQuery3);
	
	//Prepare a statement for the query.
	RSqlStatement getStmt;
	CleanupClosePushL(getStmt);
	User::LeaveIfError(getStmt.Prepare(iDatabase, queryBuf));
	
	while(KSqlAtRow == getStmt.Next())
		{
		// Do not return non-standard visible folders from non-current drive.
			// If it is a non-current drive...
		if( (driveId != KCurrentDriveId) 					 				     	  &&
			// If it is a folder entry.
		    (KUidMsvFolderEntryValue == ColumnInt(getStmt, iTypeIndex[EGetChildIds])) &&
		    // If it is a visible folder.
			(IsVisibleFolder(ColumnInt(getStmt, iDataIndex[EGetChildIds])))		      &&
			// If it is a non-standard folder.
			(!IsStandardId(ColumnInt(getStmt, iIdIndex[EGetChildIds])))
		  )		  		
			{
			continue;
			}
		
		TMsvId id = ColumnInt(getStmt, iIdIndex[EGetChildIds]);
		if(!IsStandardId(id))
			{
			id = MaskTMsvId(driveId, id);
			}
		//Add the child Id to the array given
		aChildArray.AppendL(id);
		}
	
	CleanupStack::PopAndDestroy(2); //getStmt, queryBuf
	}




/**
 * GetChildrenL()
* @param const TDesC&: Partial query string. (Where clause condition).
 *               The query string should start with 'AND' and should 
 *               end with a semicolon. Examples are:
 *               "AND ID BETWEEN 100 and 200 ORDER BY ID ASC;"
 * @param TMsvId: Parent Id of the child entries to be fetched.
 * @param RPointerArray<TMsvEntry>&: List of child entries to be filled.
 * @return None.
 *
 * Fetch child entries from the database for a given parent Id.
 * This does not check if the parent entry already exist. The 
 * entries will be added in ascending order of their TMsvId.
 */
void CMsvDBAdapter::GetChildrenL(const TDesC8& aQueryStr, TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aChildArray)
	{
	_LIT8(KQueryString1, "SELECT id, serviceId, mtmId, type, date, data, size, error, "
			"mtmData1, mtmData2, mtmData3, relatedId, bioType, pcSyncCount, "
			"reserved, visibleParent, description, details FROM DB");
	_LIT8(KQueryString2, ".IndexEntry WHERE parentId = ");

	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aParentId);
	aParentId = UnmaskTMsvId(aParentId);
	
	//Construct a query for the particular drive.
	TInt len = KQueryString1().Length() + KQueryString2().Length() + aQueryStr.Length();
	len += 30;
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(len);	
	queryBuf.Append(KQueryString1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KQueryString2);
	queryBuf.AppendNum(aParentId);
	queryBuf.Append(aQueryStr);
	
	//Prepare a statement for the query.
	RSqlStatement condGetChildrenStmnt;
	CleanupClosePushL(condGetChildrenStmnt);
	
	User::LeaveIfError(condGetChildrenStmnt.Prepare(iDatabase, queryBuf));
	TInt index = 0;

	while(KSqlAtRow == condGetChildrenStmnt.Next())
		{
		TInt type = ColumnInt(condGetChildrenStmnt, 3);
		TInt32 data = ColumnInt(condGetChildrenStmnt, 5);

		// Do not return non-standard visible folders from non-current drive.
			// If it is a non-current drive...
		if( (driveId != KCurrentDriveId) 	  &&
			// If it is a folder entry.
		    (KUidMsvFolderEntryValue == type) &&
		    // If it is a visible folder.
			(IsVisibleFolder(data))		      &&
			// If it is a non-standard folder.
			(!IsStandardId(ColumnInt(condGetChildrenStmnt, 0)))
		  )
		  	{
			continue;
			}
		
		CMsvCacheEntry* cacheEntry = CMsvEntryFreePool::Instance()->EntryL();
		CleanupStack::PushL(cacheEntry);
		if(!IsStandardId(aParentId))
			{
			aParentId = MaskTMsvId(driveId, aParentId);
			}
		cacheEntry->Entry().iParentId = aParentId;

		index = 0;
		TMsvId id = ColumnInt(condGetChildrenStmnt, index);
		if(IsStandardId(id))
			{
			cacheEntry->Entry().iId = id;
			}
		else
			{
			cacheEntry->Entry().iId = MaskTMsvId(driveId, id);
			}		
		cacheEntry->Entry().iServiceId = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtm.iUid = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iType.iUid = type;
		cacheEntry->Entry().iDate = ColumnInt64(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iData = data;
		cacheEntry->Entry().iSize = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iError = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtmData1 = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtmData2 = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtmData3 = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iRelatedId = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iBioType = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iPcSyncCount = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iReserved = ColumnInt(condGetChildrenStmnt, ++index);
		TMsvId aVisibleParentEntryId = ColumnInt(condGetChildrenStmnt, ++index);
		
		cacheEntry->iDescriptionBuffer = ColumnTextL(condGetChildrenStmnt, ++index); 	
		cacheEntry->Entry().iDescription.Set(*cacheEntry->iDescriptionBuffer);
		
		cacheEntry->iDetailsBuffer = ColumnTextL(condGetChildrenStmnt, ++index); 	
		cacheEntry->Entry().iDetails.Set(*cacheEntry->iDetailsBuffer);
		
		
		// Mask iServiceId if it is a non-standard id.
		if(!IsStandardId(cacheEntry->Entry().iServiceId))
			{
			cacheEntry->Entry().iServiceId = MaskTMsvId(driveId, cacheEntry->Entry().iServiceId);
			}
		
		// Mask iRelatedId if it is a non-standard id.
		if(!IsStandardId(cacheEntry->Entry().iRelatedId))
			{
			cacheEntry->Entry().iRelatedId = MaskTMsvId(driveId, cacheEntry->Entry().iRelatedId);
			}
		
		//Add the child entry to the array given
		aChildArray.AppendL(cacheEntry);
		CleanupStack::Pop(cacheEntry);
		}
	CleanupStack::PopAndDestroy(2);		// condGetChildrenStmnt,queryBuf
	}




/**
 * EntryExistsL()
 * @param TMsvId: Entry Id to check.
 * @return TBool: ETrue if the entry exists, otherwise EFalse.
 *
 * Checks if an entry with the specified TMsvId exists in the database.
 */
TBool CMsvDBAdapter::EntryExistsL(TMsvId aId)
	{
	_LIT8(KEntryExistsQuery1, "SELECT id FROM DB");
	_LIT8(KEntryExistsQuery2, ".IndexEntry WHERE id = ");
		
	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aId);
	aId = UnmaskTMsvId(aId);
	
	//Construct a query for the particular drive.
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	TInt len = KEntryExistsQuery1().Length() + 10 + KEntryExistsQuery2().Length(); //10 = space for TMsvId
	queryBuf.CreateL(len);	
	queryBuf.Append(KEntryExistsQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KEntryExistsQuery2);
	queryBuf.AppendNum(aId);
	queryBuf.Append(KDelimiter);
	
	TSqlScalarFullSelectQuery query(iDatabase);
	TMsvId id = NULL;
	TRAPD(err, id = query.SelectIntL(queryBuf));
	
	CleanupStack::PopAndDestroy(); //queryBuf
	if(err!= KErrNone && err!= KErrNotFound)
		{
		User::Leave(err);
		}
	if(id)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}



	
/**
 * GetInPreparationIdL()
 * @param CMsvEntrySelection&: List of the TMsvEntries 
 * 		                       whose KMsvEntryInPreparationFlag is set.
 * @return None.
 *
 * Gets the TMsvId's of all the entries whose KMsvEntryInPreparationFlag
 * is set in the database.
 */
void CMsvDBAdapter::GetInPreparationIdL(CMsvEntrySelection& aSelection, TUint aDriveId /* DEFAULT = 0 */)
	{
	if(aDriveId)
		{
		DoGetInPreparationIdL(aSelection, aDriveId);
		}
	else
		{
		//Iterate through all the dives that might have a database.
		for(TInt driveId = 1; driveId < KMaxNumberOfDrives; ++driveId)
			{
			DoGetInPreparationIdL(aSelection, driveId);		
			}
		}	
	}




void CMsvDBAdapter::DoGetInPreparationIdL(CMsvEntrySelection& aSelection, TUint aDriveId)
	{
	_LIT8(KGetInPreparationIdQuery1, "SELECT id, data FROM DB");
	_LIT8(KGetInPreparationIdQuery2, ".IndexEntry;");
	
	TInt paramIdColumn = 0;
	TInt paramDataColumn = 1;
			
	//Check if the drive has the database present.
	//The iDatabasePresent array will have a value for only those driveIds which
	//have been added through AttachDBL().
	if((*iDatabasePresent)[aDriveId])
		{
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(40);		
		queryBuf.Append(KGetInPreparationIdQuery1);
		queryBuf.AppendNum(aDriveId);
		queryBuf.Append(KGetInPreparationIdQuery2);
		
		RSqlStatement statement;
		CleanupClosePushL(statement);
		User::LeaveIfError(statement.Prepare(iDatabase, queryBuf));
		
		TMsvId id;
		TInt32 data;
		while (KSqlAtRow == statement.Next())
			{
			data = ColumnInt(statement, paramDataColumn);
			if( data & TMsvEntry::KMsvEntryInPreparationFlag )
				{
				id = ColumnInt(statement, paramIdColumn);
				if(!IsStandardId(id))
					{
					id = MaskTMsvId(aDriveId, id);
					}
				aSelection.AppendL(id);
				}
			}
		CleanupStack::PopAndDestroy(2); //statement, queryBuf
		}
	}

	
	

/**
 * GetVisibleFlagL()
 * @param TMsvId: the Id of the entry whose flag is to be checked.
 * @return TBool: ETrue if the entry is visible, EFalse otherwise.
 *
 * Returns the status of the visible flag of an entry in the DB.
 */
TBool CMsvDBAdapter::GetVisibleFlagL(TMsvId aEntryId)
	{
	_LIT8(KGetVisibleFlagQuery1, "SELECT data FROM DB");
	_LIT8(KGetVisibleFlagQuery2, ".IndexEntry WHERE id = ");
	
	//Get the driveId and unmask the TMsvId supplied.
	TInt driveId = GetDriveId(aEntryId);
	aEntryId = UnmaskTMsvId(aEntryId);
	
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(60);	
	queryBuf.Append(KGetVisibleFlagQuery1);
	queryBuf.AppendNum(driveId);
	queryBuf.Append(KGetVisibleFlagQuery2);
	queryBuf.AppendNum(aEntryId);
	queryBuf.Append(KDelimiter);
	
	TSqlScalarFullSelectQuery query(iDatabase);
	TInt32 data = query.SelectIntL(queryBuf);
	CleanupStack::PopAndDestroy();		// queryBuf
	if(data & TMsvEntry::KMsvEntryInvisibleFlag )
		{
		return EFalse;
		}
	else
		{
		return ETrue;
		}
	}




/**
 * UpdateVisibleFolderL()
 * @param CMsvEntrySelection*: List of entries whose visible parent is
 *							   to be updated.
 * @param TMsvId: The new visible folder Id.
 * @return None.
 *
 * Updates the visible parent of the entries passed through the selection.
 */
 void CMsvDBAdapter::UpdateVisibleFolderL(CMsvEntrySelection* aEntryList, TMsvId aNewVisibleFolderId)
 	{
 	_LIT8(KUpdateVisibleFolderQuery1,	"UPDATE DB");
 	_LIT8(KUpdateVisibleFolderQuery2, ".IndexEntry SET visibleParent = ");
	_LIT8(KUpdateVisibleFolderQuery3,	" WHERE id IN ( ");
	_LIT8(KEndDelimiter, ");");
	
	TInt count = aEntryList->Count();
	if(0 == count)
		{
		return;
		}
	
	//Get the driveId and unmask the TMsvId supplied.
	aNewVisibleFolderId = UnmaskTMsvId(aNewVisibleFolderId);
	CMsvEntrySelection* list = aEntryList->CopyL();
	CleanupStack::PushL(list);
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	TInt maxEntries = 50; //for testing
#else
	TInt maxEntries = 100;
#endif
	
	BeginTransactionL();
	for(TInt index1 = 0; index1<count; ++index1)
		{
		TInt driveId = GetDriveId(list->At(index1));
		
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(50 + ((10+2)*maxEntries));		
		queryBuf.Append(KUpdateVisibleFolderQuery1);
		queryBuf.AppendNum(driveId);
		queryBuf.Append(KUpdateVisibleFolderQuery2);
		queryBuf.AppendNum(aNewVisibleFolderId);
		queryBuf.Append(KUpdateVisibleFolderQuery3);
		queryBuf.AppendNum(UnmaskTMsvId(list->At(index1)));
		TInt num = 1;
		for(TInt index2 = index1 + 1; index2<count && num<maxEntries; ++index2)
			{
			if(driveId == GetDriveId(list->At(index2)))
				{
				queryBuf.Append(KComma);
				queryBuf.AppendNum(UnmaskTMsvId(list->At(index2)));
				++num;
				list->Delete(index2);
				--index2;
				--count;
				}
			}
		list->Delete(index1);
		--index1;
		--count;
		queryBuf.Append(KEndDelimiter);
		TInt err = iDatabase.Exec(queryBuf);
		CleanupStack::PopAndDestroy(); //queryBuf
		if(err < 0)
			{
			User::Leave(err);
			}
		}
	
	CommitTransactionL();
	CleanupStack::PopAndDestroy(); //list
	}




/**
 * UpdateOwnerStatusL()
 *
 * @param TMsvId: the Id of an entry which has had a child added
 *				   under it and needs its owner flag updated.
 * @param const TMsvEntry&: the parent entry.
 * @param TBool: ETrue if the owner flag should be set, EFalse if otherwise.
 * @return None.
 *
 * The function updates the owner flag of an entry which has had a
 * child added or deleted under it.
 */
void CMsvDBAdapter::UpdateOwnerStatusL(TMsvId aId, const TMsvEntry& aParent, TBool aSet)
	{
	_LIT8(KUpdateOwnershipQuery1,	"UPDATE DB");
	_LIT8(KUpdateOwnershipQuery2, ".IndexEntry SET data = ");
	_LIT8(KUpdateOwnershipQuery3,	" WHERE id = ");
	
	// Owner flag has already been set. Return to caller.
	if(aParent.Owner() && aSet)
		{
		return;
		}
	// Otherwise update the owner flag for the entry in the DB.
	else
		{
		//Get the driveId and unmask the TMsvId supplied.
		TInt driveId = GetDriveId(aId);
		TMsvId parentId = UnmaskTMsvId(aId);
		
		TInt32 data;
		if(aSet)
			{
			data = aParent.iData | TMsvEntry::KMsvEntryOwnerFlag;
			}
		else
			{
			data = aParent.iData & ~(TMsvEntry::KMsvEntryOwnerFlag);
			}
		TInt len = KUpdateOwnershipQuery1().Length() + KUpdateOwnershipQuery2().Length();
		len += KUpdateOwnershipQuery3().Length() + 100; //data and TMsvId.
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(len);		
		queryBuf.Append(KUpdateOwnershipQuery1);
		queryBuf.AppendNum(driveId);
		queryBuf.Append(KUpdateOwnershipQuery2);
		queryBuf.AppendNum(data);
		queryBuf.Append(KUpdateOwnershipQuery3);
		queryBuf.AppendNum(parentId);
		queryBuf.Append(KDelimiter);
		TInt err = iDatabase.Exec(queryBuf);
		if(err < 0)
			{
			User::Leave(err);
			}
		CleanupStack::PopAndDestroy(); //queryBuf
		}
	}




#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)




/**
 * NewL()
 * @param TFileName: DB File Name.
 * @return The newly created database adapter object.
 *
 * The function creates a new secure database in syslib private folder.
 * It takes database name as input arg and returns instance of 
 * DB adpater class.
 */
CMsvDBAdapter* CMsvDBAdapter::NewL(const TFileName& aDbFileName)
	{
	//Create an messaging DB adaptor object.
	CMsvDBAdapter* self = new(ELeave) CMsvDBAdapter();
	CleanupStack::PushL(self);
	
	// Create the database.
	TBool isDBCreated = EFalse;
	TRAPD(err, self->ConstructNewL(aDbFileName, isDBCreated));
	if( KErrNone != err )
		{
		if( isDBCreated )
			{
			DeleteDB(aDbFileName);
			}
		User::Leave(err);
		}

	CleanupStack::Pop(self);
	return self;
	}




/**
 * OpenL()
 * @param TFileName: DB File Name.
 * @return The newly created database adapter object.
 *
 * The function opens an already existing database. The database file
 * is searched in syslib private folder. It takes database name as 
 * input arg and returns instance of DB adpater class. The function
 * leaves if the DB file is not found or if the file is corrupted.
 */
CMsvDBAdapter* CMsvDBAdapter::OpenL(const TFileName& aDbFileName)
	{
	//Create an messaging DB adaptor object 
	CMsvDBAdapter* self = new(ELeave) CMsvDBAdapter();
	CleanupStack::PushL(self);
	
	// Open the database.
	self->ConstructOpenL(aDbFileName);
	
	CleanupStack::Pop(self);
	return self;
	}




/**
 * DeleteDB()
 * @param TFileName: DB File Name.
 * @return error.
 *
 * This is a static function, hence can be called without creating
 * an instance of this class. The function deletes the database
 * file as mentioned in the input argument. The database file is
 * searched in syslib private folder. 
 */
TInt CMsvDBAdapter::DeleteDB(const TFileName& aDbFileName)
	{
	return RSqlDatabase::Delete(aDbFileName);
	}




/**
 * ConstructOpenL()
 * @param TFileName: DB File Name.
 * 
 * This functions is called by OpenL() to open an existing database.
 * The function leaves if there is an error while opening the DB with
 * the leave code same as error code. It takes DB filename as input 
 * argument. The database file is searched in syslib private folder. 
 */
void CMsvDBAdapter::ConstructOpenL(const TFileName& aDbFileName)
	{
	// >>> Pass configuration parameter, while opening the DB.
	User::LeaveIfError(iDatabase.Open(aDbFileName));
	ClearDBContentsL();
	ConstructSortTableL();
	
	//Initialize the database adapter class
	InitializeL();

	//Initialize the Search Sort database adapter class
	InitializeSortTableL();
	}




/**
 * ConstructNewL()
 * @param TFileName: DB File Name.
 * 
 * This functions is called by NewL() to create a new database.
 * The function leaves if there is an error while creating the DB with
 * the leave code same as error code. It takes DB filename as input 
 * argument. The database file is created in syslib private folder. 
 * It then create standard tables as well as indexes in the database.
 */
void CMsvDBAdapter::ConstructNewL(const TFileName& aDbFileName, TBool& aIsDBCreated)
	{
	HBufC8* heapBuf = KEncodingText().AllocLC();

	//Create the database.
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	User::LeaveIfError(iDatabase.Create(aDbFileName, heapBuf));
#else	
	// Using SID based security policy.
	TSecureId sid(SERVER_UID);
	const TSecurityPolicy KDefaultPolicy(sid);
	
	//Create security policy
	RSqlSecurityPolicy securityPolicy;
	CleanupClosePushL(securityPolicy);	
		
	User::LeaveIfError(securityPolicy.Create(KDefaultPolicy));
	User::LeaveIfError(iDatabase.Create(aDbFileName, securityPolicy, heapBuf));
	
	// Destroy securityPolicy.
	CleanupStack::PopAndDestroy();		
#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)

	// Destroy heapBuf
	CleanupStack::PopAndDestroy();
	aIsDBCreated = ETrue;

	//Create tables for the database
	TInt err = iDatabase.Exec(KCreateIndexEntryTableQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
		
	//Create index on indexEnty table.
	//Index is created on parentId.
	err = iDatabase.Exec(KCreateIndexQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	
	//Create the version table to store the message store version.
	err = iDatabase.Exec(KCreateVersionTableQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	//Insert the version number into the above table.
	_LIT8(KInsertVersionNumberQuery, " INSERT INTO VersionTable Values ( " );
	_LIT8(KDelimiter, " );");
	RBuf8 versionQuery;
	CleanupClosePushL(versionQuery);
	versionQuery.CreateL(50); //a small value is more than enough for the above query.	
	versionQuery.Append(KInsertVersionNumberQuery);
	versionQuery.AppendNum(KCurrentDatabaseVersion);
	versionQuery.Append(KDelimiter);
	err = iDatabase.Exec(versionQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(); //versionQuery

	ConstructSortTableL();
	
	//Initialize the database adapter class
	InitializeL();

	//Initialize the Search Sort database adapter class
	InitializeSortTableL();
	}




/**
 * GetVisibleFolderIdL()
 * @param TMsvId: Parent Id.
 * @return Most immediate visible folder id of any child entry 
 *         present under passed parent id.
 *
 * This functions returns the most immediate parent visible folder id.
 * There are two types of folders. A visible folder is one which is 
 * visible to the external user, but there are internal non-visible folders
 * created under each email message to organize the content of the email.
 * Parent entry of an entry is either root or a folder. A folder can be 
 * service folder (like LocalServices), userdefined folder/standard folder
 * like Inbox or a non-visible folder (created under an email message).
 * 
 * For a given parent Id, the function returns the most immediate visible
 * parent folder. The rules are as follows:
 * 1. If the parent entry is root or children of root, then the visibleFolder 
 * will be root itself. This is because service folders are considered 
 * invisible, since they are not seen by user. 
 * 2. If the parent entry itself is a visible folder then child's visibleFolder
 * id should be its parent id.
 * 3. If the parent entry is a non-visible folder then child's visibleFolder
 * id is same as its parent's visibleFolder id.
 */
void CMsvDBAdapter::GetVisibleFolderIdL(TMsvId aParentId, TMsvId& aVisibleFolderId)
	{
	// If the entry is either a root entry (parent=KErrNotFound) or 
	// child of root entry its visibleFolderId will be root entry itself.
	if((KErrNotFound == aParentId) || (KMsvRootIndexEntryId == aParentId))
		{
		aVisibleFolderId = KMsvRootIndexEntryId;
		}
	else
		{
		// Get the visible flag of parent entry.
		
		// If parent entry is visible, then immediateVisibleFolder of child
		// should be parent Id. And if it is invisible then child's immediateVisibleFolder 
		// should be same as parent's immediateVisibleFolder.
		
		//Bind the msv id
		BindIntL(iStatement[EGetVisibleId], iIdIndex[EGetVisibleId], aParentId);

		// If parent entry does not exists leave with KErrNotFound.
		if (KSqlAtRow != iStatement[EGetVisibleId].Next())
			{
			iStatement[EGetVisibleId].Reset();
			User::Leave(KErrNotFound);
			}

		TInt32 data = ColumnInt(iStatement[EGetVisibleId], iDataIndex[EGetVisibleId]);
		if( data & TMsvEntry::KMsvVisibleFolderNodeFlag)
			{
			// If visible folder flag is not set for parent entry.
			aVisibleFolderId = ColumnInt(iStatement[EGetVisibleId], iVisibleParent[EGetVisibleId]);
			}
		else
			{
			// If parent entry is visible
			aVisibleFolderId = aParentId;
			}
		//Reset the RSqlStatement
		User::LeaveIfError(iStatement[EGetVisibleId].Reset());
		}
	}




/**
 * CreateEntryL()
 * @param const TMsvEntry&: DB File Name.
 * @return TMsvId: Immediate visible parent folder of the entry
 *
 * The function creates a new entry in the database. It gets the immediate
 * visible folder id of the entry before inserting them into the database.
 * No consisitency check is performed on the passed entry. The function may
 * leave if query execution fails.
 */
void CMsvDBAdapter::CreateEntryL(const TMsvEntry& aNewEntry, const TMsvId aImmediateVisibleFolder)
	{
	//Bind the entry data
	BindIntL(iStatement[ECreateEntry], iIdIndex[ECreateEntry], aNewEntry.iId);
	BindIntL(iStatement[ECreateEntry], iParentIdIndex[ECreateEntry], aNewEntry.iParentId);
	BindIntL(iStatement[ECreateEntry], iDataIndex[ECreateEntry], aNewEntry.iData);
	BindIntL(iStatement[ECreateEntry], iPcSyncCountIndex[ECreateEntry], aNewEntry.iPcSyncCount);
	BindIntL(iStatement[ECreateEntry], iReservedIndex[ECreateEntry], aNewEntry.iReserved);
	BindIntL(iStatement[ECreateEntry], iRelatedIdIndex[ECreateEntry], aNewEntry.iRelatedId);
	BindIntL(iStatement[ECreateEntry], iTypeIndex[ECreateEntry], aNewEntry.iType.iUid);
	BindInt64L(iStatement[ECreateEntry], iDateIndex[ECreateEntry], aNewEntry.iDate.Int64());
	BindIntL(iStatement[ECreateEntry], iSizeIndex[ECreateEntry], aNewEntry.iSize);
	BindIntL(iStatement[ECreateEntry], iErrorIndex[ECreateEntry], aNewEntry.iError);
	BindIntL(iStatement[ECreateEntry], iBiotypeIndex[ECreateEntry], aNewEntry.iBioType);
	BindIntL(iStatement[ECreateEntry], iMtmData1Index[ECreateEntry], aNewEntry.iMtmData1);
	BindIntL(iStatement[ECreateEntry], iMtmData2Index[ECreateEntry], aNewEntry.iMtmData2);
	BindIntL(iStatement[ECreateEntry], iMtmData3Index[ECreateEntry], aNewEntry.iMtmData3);
	BindIntL(iStatement[ECreateEntry], iServiceIdIndex[ECreateEntry], aNewEntry.iServiceId);
	BindIntL(iStatement[ECreateEntry], iMtmIdIndex[ECreateEntry], aNewEntry.iMtm.iUid);
	BindIntL(iStatement[ECreateEntry], iVisibleParent[ECreateEntry], aImmediateVisibleFolder);
	BindTextL(iStatement[ECreateEntry],iDescriptionIndex[ECreateEntry],aNewEntry.iDescription);
	BindTextL(iStatement[ECreateEntry],iDetailsIndex[ECreateEntry],aNewEntry.iDetails);
	//Execute the prepared SQL statement.
	
	TInt err = iStatement[ECreateEntry].Exec();
	if(err < 0)
		{
		iStatement[ECreateEntry].Reset();
		User::Leave(err);
		}
	else if(!err)
		{
		iStatement[ECreateEntry].Reset();
		User::Leave(KErrGeneral);
		}
	
	//Reset the RSqlStatement
	User::LeaveIfError(iStatement[ECreateEntry].Reset());	
	}




/**
 * UpdateEntryL()
 * @param const TMsvEntry&: Entry to be updated.
 * @param TBool: ETrue if the parent of the entry has been changed,
 *				 EFalse if otherwise.
 * @param TMsvId&: the visible folder Id of the updated entry.
 *				   If aParentChanged is ETrue then aImmediateVisibleFolder
 *                 is an output parameter, and input parameter if otherwise.
 * @return None.
 *
 * The function updates an entry in the database. It gets the immediate
 * visible folder id of the entry before inserting them into the database.
 * No consisitency check is performed on the passed entry. The function may
 * leave if query execution fails.
 */
void CMsvDBAdapter::UpdateEntryL(const TMsvEntry& aEntry, const TMsvId aImmediateVisibleFolder, const TBool aIsParentIdUpdated/*DEFAULT=ETrue*/)
	{
	iStatementType updateStmt;
	//Bind the entry data
	if(aIsParentIdUpdated)
		{
		updateStmt = EUpdateEntry;
		BindIntL(iStatement[updateStmt], iParentIdIndex[updateStmt], aEntry.iParentId);
		BindIntL(iStatement[updateStmt], iVisibleParent[updateStmt], aImmediateVisibleFolder);
		}
	else
		{
		updateStmt = EUpdateEntryNoParent;
		}
	BindIntL(iStatement[updateStmt], iIdIndex[updateStmt], aEntry.iId);
	BindIntL(iStatement[updateStmt], iDataIndex[updateStmt], aEntry.iData);
	BindIntL(iStatement[updateStmt], iPcSyncCountIndex[updateStmt], aEntry.iPcSyncCount);
	BindIntL(iStatement[updateStmt], iReservedIndex[updateStmt], aEntry.iReserved);
	BindIntL(iStatement[updateStmt], iRelatedIdIndex[updateStmt], aEntry.iRelatedId);
	BindIntL(iStatement[updateStmt], iTypeIndex[updateStmt], aEntry.iType.iUid);
	BindInt64L(iStatement[updateStmt], iDateIndex[updateStmt], aEntry.iDate.Int64());
	BindIntL(iStatement[updateStmt], iSizeIndex[updateStmt], aEntry.iSize);
	BindIntL(iStatement[updateStmt], iErrorIndex[updateStmt], aEntry.iError);
	BindIntL(iStatement[updateStmt], iBiotypeIndex[updateStmt], aEntry.iBioType);
	BindIntL(iStatement[updateStmt], iMtmData1Index[updateStmt], aEntry.iMtmData1);
	BindIntL(iStatement[updateStmt], iMtmData2Index[updateStmt], aEntry.iMtmData2);
	BindIntL(iStatement[updateStmt], iMtmData3Index[updateStmt], aEntry.iMtmData3);
	BindIntL(iStatement[updateStmt], iServiceIdIndex[updateStmt], aEntry.iServiceId);
	BindIntL(iStatement[updateStmt], iMtmIdIndex[updateStmt], aEntry.iMtm.iUid);
	BindTextL(iStatement[updateStmt], iDescriptionIndex[updateStmt],aEntry.iDescription);
	BindTextL(iStatement[updateStmt], iDetailsIndex[updateStmt],aEntry.iDetails);

	//Execute the prepared SQL statement.
	TInt err = iStatement[updateStmt].Exec();
	if(err < 0)
		{
		iStatement[updateStmt].Reset();
		User::Leave(err);
		}
	else if(!err)
		{
		iStatement[updateStmt].Reset();
		User::Leave(KErrNotFound);
		}

	//Reset the RSqlStatement
	User::LeaveIfError(iStatement[updateStmt].Reset());
	}




/**
 * DeleteEntryL()
 * @param TMsvId: Entry Id to be deleted.
 *
 * The function deletes an entry with the passed Id. The function
 * may leave due to database error while execution. The function
 * does not leave if the entry does not exist.
 *
 * **WARNING** The function does NOT ensure that if a parent entry
 * is deleted all its child entries are also deleted.
 */
void CMsvDBAdapter::DeleteEntryL(TMsvId aEntryId)
	{
	_LIT8(KDeleteEntryQuery, "DELETE FROM IndexEntry WHERE id = ");
	_LIT8(KSemiColon, ";");
	
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(KDeleteEntryQuery().Length() + 20);
	queryBuf.Append(KDeleteEntryQuery);
	queryBuf.AppendNum(aEntryId);
	queryBuf.Append(KSemiColon);
	
	TInt err = iDatabase.Exec(queryBuf);
	if(err < 0)
		{		
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrNotFound);			
		}
	CleanupStack::PopAndDestroy();		// queryBuf
	}




/**
 * DeleteEntryL()
 * @param CMsvEntrySelection: Entry Id list to be deleted.
 *
 * The function deletes all entries mentioned in the passed Id list.
 * The function may leave due to database error while execution or
 * if an entry does not exist.
 *
 * **WARNING** The function does NOT ensure that if a parent entry
 * is deleted all its child entries are also deleted.
 */
void CMsvDBAdapter::DeleteEntryL(const CMsvEntrySelection& aEntryIdList)
	{
	TInt count;
	if( 0 == (count = aEntryIdList.Count()) )
		{
		return;
		}

	_LIT(KQuery, "DELETE FROM IndexEntry WHERE id IN (");
	_LIT(KComma, ", ");
	_LIT(KEndDelimeter, ");");
	
	RBuf8 buf;
	CleanupClosePushL(buf);
	TInt index = 0;
	//Delete maximum 100 entries at a time so as to not overflow IPC buffer
	TInt maxEntries = 100;
	TInt iterRequired = count/maxEntries;
	if(count%maxEntries)
		{
		iterRequired++;
		}
	if(iterRequired) 
		{
		maxEntries = count/iterRequired;
		}
	for(TInt iter = 0; iter < iterRequired; ++iter)
		{
		if(iter && iter+1 == iterRequired)
			{
			maxEntries = count - index;
			}		
		buf.CreateL( 40 + ((7 + 2) * maxEntries) + 2); //KQuery + (TMsvId+KComma)*100 + KEndDelimiter		
		buf.Append(KQuery);
		TBool firstId = ETrue;
		TInt num = 0;
		while(num < maxEntries)
			{
			if(!firstId)
				{
				buf.Append(KComma);
				}
			firstId = EFalse;
			buf.AppendNum(aEntryIdList.At(index));
			++index;
			++num;
			}
		buf.Append(KEndDelimeter);
		User::LeaveIfError(iDatabase.Exec(buf));		
		buf.Close();
		}
	CleanupStack::PopAndDestroy(); //buf
	}




/**
 * GetEntryL()
 * @param TMsvId: Id of the entry to be fetched.
 * @return TMsvId: Value of VisibleParent column.
 * @return TMsvEntry*: The newly fetched entry.
 *
 * Fetch an entry from the database. Memory to aEntry
 * should not already be assigned.
 */
void CMsvDBAdapter::GetEntryL(TMsvId aId, CMsvCacheEntry*& aEntry, TMsvId& aVisibleParentEntryId)
	{
	//Bind the msv id of the entry to be retrieved
	BindIntL(iStatement[EGetEntry], iIdIndex[EGetEntry], aId);

	if (KSqlAtRow != iStatement[EGetEntry].Next())
		{
		iStatement[EGetEntry].Reset();
		User::Leave(KErrNotFound);
		}

	//Get the entry from the statement
	aEntry = CMsvEntryFreePool::Instance()->EntryL();
	CleanupStack::PushL(aEntry);
	aEntry->Entry().iId = aId;
	LoadEntryFromStatementL(iStatement[EGetEntry], EGetEntry, aVisibleParentEntryId, *aEntry);
	CleanupStack::Pop(aEntry);
	//Reset the RSqlStatement
	User::LeaveIfError(iStatement[EGetEntry].Reset());
	}




/**
 * GetChildrenL()
 * @param TMsvId: Parent Id of the child entries to be fetched.
 * @return RPointerArray<TMsvEntry>&: List of child entries.
 *
 * Fetch child entries from the database for a given parent Id.
 * This does not check if the parent entry already exist. The 
 * entries will be added in ascending order of their TMsvId.
 */
void CMsvDBAdapter::GetChildrenL(TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aChildArray)
	{
	//Bind the msv id of the entry whose children have to be retrieved
	BindIntL(iStatement[EGetChildEntries], iParentIdIndex[EGetChildEntries], aParentId);

	while(KSqlAtRow == iStatement[EGetChildEntries].Next())
		{
		TMsvId aVisibleParentEntryId;
		CMsvCacheEntry *cacheEntry = CMsvEntryFreePool::Instance()->EntryL();
		CleanupStack::PushL(cacheEntry);
		cacheEntry->Entry().iParentId = aParentId;
		LoadEntryFromStatementL(iStatement[EGetChildEntries], EGetChildEntries, aVisibleParentEntryId, *cacheEntry);
		//Add the child entry to the array given
		aChildArray.AppendL(cacheEntry);
		CleanupStack::Pop(cacheEntry);
		}

	//Reset the RSqlStatement
	User::LeaveIfError(iStatement[EGetChildEntries].Reset());
	}




/**
 * GetChildrenIdL()
 * @param TMsvId: Parent Id of the child entries to be fetched.
 * @return RPointerArray<TMsvEntry>&: List of child entries.
 *
 * Fetch child entries from the database for a given parent Id.
 * This does not check if the parent entry already exist. The 
 * entries will be added in ascending order of their TMsvId.
 */
void CMsvDBAdapter::GetChildrenIdL(TMsvId aParentId, CMsvEntrySelection& aChildArray)
	{
	//Bind the msv id of the entry whose children have to be retrieved
	BindIntL(iStatement[EGetChildIds], iParentIdIndex[EGetChildIds], aParentId);

	while(KSqlAtRow == iStatement[EGetChildIds].Next())
		{
		TMsvId id = ColumnInt(iStatement[EGetChildIds], iIdIndex[EGetChildIds]);
		//Add the child Id to the array given
		aChildArray.AppendL(id);
		}

	//Reset the RSqlStatement	
	User::LeaveIfError(iStatement[EGetChildIds].Reset());
	}




/**
 * GetChildrenL()
 * @param TDesc: Partial query string. (Where clause condition).
 *               The query string should start with 'AND' and should 
 *               end with a semicolon. Examples are:
 *               "AND ID BETWEEN 100 and 200 ORDER BY ID ASC;"
 * @param TMsvId: Parent Id of the child entries to be fetched.
 * @return RPointerArray<TMsvEntry>&: List of child entries.
 *
 * Fetch child entries from the database for a given parent Id.
 * This does not check if the parent entry already exist. The 
 * entries will be added in ascending order of their TMsvId.
 */
void CMsvDBAdapter::GetChildrenL(const TDesC8& aQueryStr, TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aChildArray)
	{
	_LIT8(KQueryString, "SELECT id, serviceId, mtmId, type, date, data, size, error, "
			"mtmData1, mtmData2, mtmData3, relatedId, bioType, pcSyncCount, "
			"reserved, visibleParent, description, details FROM IndexEntry "
			"WHERE parentId = :parentId ");

	RSqlStatement condGetChildrenStmnt;
	CleanupClosePushL(condGetChildrenStmnt);
	TInt len = KQueryString().Length() + aQueryStr.Length();
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(len);	
	queryBuf.Append(KQueryString);
	queryBuf.Append(aQueryStr);
		
	User::LeaveIfError(condGetChildrenStmnt.Prepare(iDatabase, queryBuf));
	TInt index = 0;

	//Bind the msv id of the entry whose children have to be retrieved
	BindIntL(condGetChildrenStmnt, index, aParentId);

	while(KSqlAtRow == condGetChildrenStmnt.Next())
		{
		CMsvCacheEntry* cacheEntry = CMsvEntryFreePool::Instance()->EntryL();
		CleanupStack::PushL(cacheEntry);
		cacheEntry->Entry().iParentId = aParentId;

		index = 0;
		cacheEntry->Entry().iId = ColumnInt(condGetChildrenStmnt, index);
		cacheEntry->Entry().iServiceId = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtm.iUid = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iType.iUid = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iDate = ColumnInt64(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iData = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iSize = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iError = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtmData1 = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtmData2 = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iMtmData3 = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iRelatedId = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iBioType = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iPcSyncCount = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->Entry().iReserved = ColumnInt(condGetChildrenStmnt, ++index);
		TMsvId aVisibleParentEntryId = ColumnInt(condGetChildrenStmnt, ++index);
		cacheEntry->iDescriptionBuffer = ColumnTextL(condGetChildrenStmnt, ++index); 	
		cacheEntry->Entry().iDescription.Set(*cacheEntry->iDescriptionBuffer);
		
		cacheEntry->iDetailsBuffer = ColumnTextL(condGetChildrenStmnt, ++index); 	
		cacheEntry->Entry().iDetails.Set(*cacheEntry->iDetailsBuffer);
		
		//Add the child entry to the array given
		aChildArray.AppendL(cacheEntry);
		CleanupStack::Pop(cacheEntry);
		}
	CleanupStack::PopAndDestroy(2);		// condGetChildrenStmnt,queryBuf
	}




/**
 * EntryExistsL()
 * @param TMsvId: Entry Id to check.
 * @return TBool: ETrue if the entry exists, otherwise EFalse.
 */
TBool CMsvDBAdapter::EntryExistsL(TMsvId aId)
	{	
	_LIT8(KEntryExistsQuery, "SELECT id FROM IndexEntry WHERE id = ");
	_LIT8(KEndDelimeter, ";");
	
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	TInt len = KEntryExistsQuery().Length() + 10 + KEndDelimeter().Length(); //10 = space for TMsvId
	queryBuf.CreateL(len);
	queryBuf.Append(KEntryExistsQuery);
	queryBuf.AppendNum(aId);
	queryBuf.Append(KEndDelimeter);
	TSqlScalarFullSelectQuery query(iDatabase);
	TMsvId id = NULL;
	TRAPD(err, id = query.SelectIntL(queryBuf));
	CleanupStack::PopAndDestroy();		// queryBuf
	if(err!= KErrNone && err!= KErrNotFound)
		{
		User::Leave(err);
		}
	if(id)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}
	
	
	
	
/**
 * GetMaxTMsvIdL()
 * @param TMsvId&: Out parameter for maximum TMsvId in the database.
 * @return None.
 * 
 * If there are no entries in the database (DB is empty),
 * the aMaxTMsvId is set to NULL.
 */
void CMsvDBAdapter::GetMaxTMsvIdL(TMsvId& aMaxTMsvId)
	{
	_LIT8(KQuery, "SELECT MAX(id) FROM IndexEntry;");
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(35);
	queryBuf.Append(KQuery);

	//Create a select query
    TSqlScalarFullSelectQuery query(iDatabase);

	//Get the column data
    aMaxTMsvId = query.SelectIntL(queryBuf);
    CleanupStack::PopAndDestroy();
	}




/**
 * GetInPreparationIdL()
 * @param aSelection CMsvEntrySelection of the TMsvEntries 
 * 					 whose KMsvEntryInPreparationFlag is set
 * @return void
 */
void CMsvDBAdapter::GetInPreparationIdL(CMsvEntrySelection& aSelection)
	{
	RSqlStatement statement;	
	User::LeaveIfError(statement.Prepare(iDatabase, KGetInPreparationIdQuery));
	
	TInt paramIdColumn = GetColumnIndexL(statement, KMsgColumnId());
	TInt paramDataColumn = GetColumnIndexL(statement, KMsgColumnData());
	
	TMsvId Id;
	TInt32 data;
	while (KSqlAtRow == statement.Next())
		{
		data = ColumnInt(statement, paramDataColumn);
		if( data & TMsvEntry::KMsvEntryInPreparationFlag )
			{
			Id = ColumnInt(statement, paramIdColumn);
			aSelection.AppendL(Id);
			}
		}	
	statement.Close();
	}




/**
 * GetVisibleFlagL()
 *@param TMsvId : the Id of the entry whose flag is to be checked.
 *@return TBool : ETrue if the entry is visible, EFalse otherwise.
 *
 * Returns the status of the visible flag of an entry in the DB.
 */
TBool CMsvDBAdapter::GetVisibleFlagL(TMsvId entryId)
	{
	_LIT8(KGetVisibleFlagQuery, "SELECT data FROM IndexEntry "
								"WHERE id = ");
	_LIT8(KEndDelimeter, ";");
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(60);	
	queryBuf.Append(KGetVisibleFlagQuery);
	queryBuf.AppendNum(entryId);
	queryBuf.Append(KEndDelimeter);
	TSqlScalarFullSelectQuery query(iDatabase);
	TInt32 data = 0;
	TRAPD(err, data = query.SelectIntL(queryBuf));
	CleanupStack::PopAndDestroy(); //queryBuf
	if(err!= KErrNone && err!= KErrNotFound)
		{
		User::Leave(err);
		}
	if(data & TMsvEntry::KMsvEntryInvisibleFlag )
		{
		return EFalse;
		}
	else
		{
		return ETrue;
		}
	}




/**
 * UpdateVisibleFolderL()
 * @param RBuf8 : the ID's of the entries of whose visible parent is to 
 *				  be updated.
 * @return None.
 *
 * Updates the visible parent of the entries passed through the RBuf* idList.
 */
 void CMsvDBAdapter::UpdateVisibleFolderL(CMsvEntrySelection* entryList, TMsvId newVisibleFolderId)
 	{
 	_LIT8(KUpdateVisibleFolderQuery1,	"UPDATE IndexEntry SET "
										"visibleParent = ");
	_LIT8(KUpdateVisibleFolderQuery2,	" WHERE id IN ( ");
	_LIT8(KComma, ", ");
	_LIT8(KDelimiter, ");");
	
	BeginTransactionL();
						
	TInt count = entryList->Count();
	TInt arrIndex = 0;
	while(arrIndex < count)
		{
		TInt len = KUpdateVisibleFolderQuery1().Length() + KUpdateVisibleFolderQuery2().Length(); 
		TInt temp;
		//Update 50 entries or lesser at a time.
		
		TInt totalEntries = count - arrIndex;
		
		if(totalEntries < 50)
			{
			len += totalEntries*12 + 15; //number of entries*length of a TMsvId + length of visibleParent id
			temp = totalEntries;
			}
		else
			{
			len += 50*12 + 15; //number of entries*length of a TMsvId + length of visibleParent id
			temp = 50;
			}
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(len);
		queryBuf.Append(KUpdateVisibleFolderQuery1);
		queryBuf.AppendNum(newVisibleFolderId);
		queryBuf.Append(KUpdateVisibleFolderQuery2);
		TInt index = arrIndex;
		for(; index < arrIndex + temp; ++index)
			{
			queryBuf.AppendNum(entryList->At(index));
			if(index != arrIndex + temp - 1)
				{
				queryBuf.Append(KComma);
				}
			}
		queryBuf.Append(KDelimiter);
		arrIndex += temp;	
		TInt err = iDatabase.Exec(queryBuf);
		CleanupStack::PopAndDestroy();	// queryBuf
		if(err < 0)
			{			
			RollbackTransactionL();
			User::Leave(err);
			}
		else if(!err) //no entries updated
			{
			RollbackTransactionL();
			User::Leave(KErrNotFound);
			}
		}
	CommitTransactionL();
	}




/**
 * UpdateOwnerStatusL()
 *
 * @param TMsvId : the Id of an entry which has had a child added
 *				   under it and needs its owner flag updated.
 * @return None.
 *
 * The function updates the owner flag of an entry which has had a
 * child added under it.
 */
void CMsvDBAdapter::UpdateOwnerStatusL(TMsvId aId, const TMsvEntry& aParent, TBool aSet)
	{
	_LIT8(KUpdateOwnershipQuery1,	"UPDATE IndexEntry SET "
										" data = ");
	_LIT8(KUpdateOwnershipQuery2,	" WHERE id = ");
	_LIT8(KDelimiter, ";");
	
	// Owner flag has already been set. Return to caller.
	if(aParent.Owner() && aSet)
		{
		return;
		}
	// Otherwise update the owner flag for the entry in the DB.
	else
		{
		TInt32 data;
		if(aSet)
			{
			data = aParent.iData | TMsvEntry::KMsvEntryOwnerFlag;
			}
		else
			{
			data = aParent.iData & ~(TMsvEntry::KMsvEntryOwnerFlag);
			}
		TInt len = KUpdateOwnershipQuery1().Length() + KUpdateOwnershipQuery2().Length();
		len += 100; //data and TMsvId.
		RBuf8 queryBuf;
		CleanupClosePushL(queryBuf);
		queryBuf.CreateL(len);		
		queryBuf.Append(KUpdateOwnershipQuery1);
		queryBuf.AppendNum(data);
		queryBuf.Append(KUpdateOwnershipQuery2);
		queryBuf.AppendNum(aId);
		queryBuf.Append(KDelimiter);
		TInt err = iDatabase.Exec(queryBuf);
		if(err < 0)
			{
			User::Leave(err);
			}
		CleanupStack::PopAndDestroy(); //queryBuf
		}
	}



#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)




/**
 * InitializeL()
 *
 * This functions prepares (compiles) frequetly used SQL queries,
 * and create indexes on the variable arguments. When the query
 * needs to be executed, we just have to bind the variables with
 * the values and execute them. This increases the performance of
 * query execution.
 */
void CMsvDBAdapter::InitializeL()
	{
	//Prepare the frequently used SQL statements
	User::LeaveIfError(iStatement[ECreateEntry].Prepare(iDatabase, KCreateEntryQuery));
	User::LeaveIfError(iStatement[EUpdateEntry].Prepare(iDatabase, KUpdateEntryQuery));
	User::LeaveIfError(iStatement[EUpdateEntryNoParent].Prepare(iDatabase, KUpdateEntryNoParentQuery));
	User::LeaveIfError(iStatement[EGetEntry].Prepare(iDatabase, KGetEntryQuery));
	User::LeaveIfError(iStatement[EGetChildEntries].Prepare(iDatabase, KGetChildEntriesQuery));
	User::LeaveIfError(iStatement[EGetVisibleId].Prepare(iDatabase, KGetVisibleIdQuery));
	User::LeaveIfError(iStatement[EEntryExists].Prepare(iDatabase, KGetEntryExistsQuery));
	User::LeaveIfError(iStatement[EGetChildIds].Prepare(iDatabase, KGetChildIdsQuery));

	//Get the indices - This maps the parameter name to its offset
	//in the SQL statement templates.
	for(TInt index = ECreateEntry; index < EIndexSentinel; ++index)
		{
		switch(index)
			{
			case ECreateEntry:			// KCreateEntryQuery
			case EUpdateEntry:			// KUpdateEntryQuery
				//For creating and updating entries
				iParentIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamParentId());
				iVisibleParent[index] = GetParamIndexL(iStatement[index], KMsgParamVisibleParent());
				iIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamId());				
				iServiceIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamServiceId());
				iMtmIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamMtmId());
				iTypeIndex[index] = GetParamIndexL(iStatement[index], KMsgParamType());
				iDateIndex[index] = GetParamIndexL(iStatement[index], KMsgParamDate());
				iDataIndex[index] = GetParamIndexL(iStatement[index], KMsgParamData());
				iSizeIndex[index] = GetParamIndexL(iStatement[index], KMsgParamSize());
				iErrorIndex[index] = GetParamIndexL(iStatement[index], KMsgParamError());
				iMtmData1Index[index] = GetParamIndexL(iStatement[index], KMsgParamMtmData1());
				iMtmData2Index[index] = GetParamIndexL(iStatement[index], KMsgParamMtmData2());
				iMtmData3Index[index] = GetParamIndexL(iStatement[index], KMsgParamMtmData3());
				iRelatedIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamRelatedId());
				iBiotypeIndex[index] = GetParamIndexL(iStatement[index], KMsgParamBiotype());
				iPcSyncCountIndex[index] = GetParamIndexL(iStatement[index], KMsgParamPcSyncCount());
				iReservedIndex[index] = GetParamIndexL(iStatement[index], KMsgParamReserved());
				iDescriptionIndex[index] = GetParamIndexL(iStatement[index], KMsgParamDescriptionTxt());
				iDetailsIndex[index] = GetParamIndexL(iStatement[index], KMsgParamDetailsTxt());
				break;
			case EUpdateEntryNoParent:
				iVisibleParent[index] = GetParamIndexL(iStatement[index], KMsgParamVisibleParent());
				iIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamId());				
				iServiceIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamServiceId());
				iMtmIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamMtmId());
				iTypeIndex[index] = GetParamIndexL(iStatement[index], KMsgParamType());
				iDateIndex[index] = GetParamIndexL(iStatement[index], KMsgParamDate());
				iDataIndex[index] = GetParamIndexL(iStatement[index], KMsgParamData());
				iSizeIndex[index] = GetParamIndexL(iStatement[index], KMsgParamSize());
				iErrorIndex[index] = GetParamIndexL(iStatement[index], KMsgParamError());
				iMtmData1Index[index] = GetParamIndexL(iStatement[index], KMsgParamMtmData1());
				iMtmData2Index[index] = GetParamIndexL(iStatement[index], KMsgParamMtmData2());
				iMtmData3Index[index] = GetParamIndexL(iStatement[index], KMsgParamMtmData3());
				iRelatedIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamRelatedId());
				iBiotypeIndex[index] = GetParamIndexL(iStatement[index], KMsgParamBiotype());
				iPcSyncCountIndex[index] = GetParamIndexL(iStatement[index], KMsgParamPcSyncCount());
				iReservedIndex[index] = GetParamIndexL(iStatement[index], KMsgParamReserved());
				iDescriptionIndex[index] = GetParamIndexL(iStatement[index], KMsgParamDescriptionTxt());
				iDetailsIndex[index] = GetParamIndexL(iStatement[index], KMsgParamDetailsTxt());
				break;
			case EGetEntry:				// KGetEntryQuery
				//Read an entry from DB
				iIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamId());
				iParentIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnParentId());
				iServiceIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnServiceId());
				iMtmIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmId());
				iTypeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnType());
				iDateIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnDate());
				iDataIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnData());
				iSizeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnSize());
				iErrorIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnError());
				iMtmData1Index[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmData1());
				iMtmData2Index[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmData2());
				iMtmData3Index[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmData3());
				iRelatedIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnRelatedId());
				iBiotypeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnBiotype());
				iPcSyncCountIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnPcSyncCount());
				iReservedIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnReserved());
				iVisibleParent[index] = GetColumnIndexL(iStatement[index], KMsgColumnVisibleParent());
				iDescriptionIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnDescriptionTxt());
				iDetailsIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnDetailsTxt());
				break;
			case EGetChildEntries:		// KGetChildEntriesQuery
				//Read child entries for a given parent Id.
				iParentIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamParentId());
				iIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnId());
				iServiceIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnServiceId());
				iMtmIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmId());
				iTypeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnType());
				iDateIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnDate());
				iDataIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnData());
				iSizeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnSize());
				iErrorIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnError());
				iMtmData1Index[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmData1());
				iMtmData2Index[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmData2());
				iMtmData3Index[index] = GetColumnIndexL(iStatement[index], KMsgColumnMtmData3());
				iRelatedIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnRelatedId());
				iBiotypeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnBiotype());
				iPcSyncCountIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnPcSyncCount());
				iReservedIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnReserved());
				iVisibleParent[index] = GetColumnIndexL(iStatement[index], KMsgColumnVisibleParent());
				iDescriptionIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnDescriptionTxt());
				iDetailsIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnDetailsTxt());
				break;
			case EGetVisibleId:			// KGetVisibleIdQuery
				//For getting visible Id related information
				iIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamId());
				iDataIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnData());	
				iVisibleParent[index] = GetColumnIndexL(iStatement[index], KMsgColumnVisibleParent());
				break;
			case EEntryExists:			// KGetEntryExistsQuery
				//To check if an entry exists in DB.
				iIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamId());
				iParentIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnParentId());
				break;
			case EGetChildIds:			// KGetChildIdsQuery
				//To check if an entry exists in DB.
				iParentIdIndex[index] = GetParamIndexL(iStatement[index], KMsgParamParentId());
				iIdIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnId());

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				iTypeIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnType());
				iDataIndex[index] = GetColumnIndexL(iStatement[index], KMsgColumnData());
#endif
				break;
			default:
				break;
			}
		}
	}




/**
 * BeginTransactionL()
 * 
 * Starts a new DB transaction.
 */
void CMsvDBAdapter::BeginTransactionL()
	{
	if(!isTransactionOpen)
		{
		_LIT8(KBeginTransaction, "BEGIN;");
		User::LeaveIfError(iDatabase.Exec(KBeginTransaction));
		isTransactionOpen = ETrue;
		}
	}




/**
 * CommitTransactionL()
 * 
 * Commits an already opened transaction.
 */
void CMsvDBAdapter::CommitTransactionL()
	{
	if(isTransactionOpen)
		{
		_LIT8(KCommitTransaction,	 "COMMIT;");
		isTransactionOpen = EFalse;
		User::LeaveIfError(iDatabase.Exec(KCommitTransaction));		
		}
	}


/**
 * RollbackTransactionL()
 * 
 * Rollback an already opened transaction.
 */
void CMsvDBAdapter::RollbackTransactionL()
	{
	if(isTransactionOpen)
		{
		_LIT8(KRollbackTransaction,	 "ROLLBACK;");
		isTransactionOpen = EFalse;
		User::LeaveIfError(iDatabase.Exec(KRollbackTransaction));		
		}
	}





/**
 * LoadEntryFromStatementL()
 * @param RSqlStatement&: a reference to the RSqlStatement to work against.
 * @param TInt: Index of the SQL statement.
 * @param TMsvId&: Out parameter for value of VisibleParent column.
 * @param CMsvCacheEntry&: The newly fetched entry to be filled.
 * @return None.
 *
 * Creates a CMsvCacheEntry from the executed RSqlStatement.
 */
void CMsvDBAdapter::LoadEntryFromStatementL(RSqlStatement& aStmt, TInt aIndex, TMsvId& aVisibleParentEntryId, CMsvCacheEntry& aEntry)
	{
	if(EGetEntry != aIndex)
		{
		aEntry.Entry().iId = ColumnInt(aStmt, iIdIndex[aIndex]);
		}
	if(EGetChildEntries != aIndex)
		{
		aEntry.Entry().iParentId = ColumnInt(aStmt, iParentIdIndex[aIndex]);
		}
	
	aEntry.Entry().iData = ColumnInt(aStmt, iDataIndex[aIndex]);
	aEntry.Entry().iPcSyncCount = ColumnInt(aStmt, iPcSyncCountIndex[aIndex]);
	aEntry.Entry().iReserved = ColumnInt(aStmt, iReservedIndex[aIndex]);
	aEntry.Entry().iRelatedId = ColumnInt(aStmt, iRelatedIdIndex[aIndex]);
	aEntry.Entry().iType.iUid = ColumnInt(aStmt, iTypeIndex[aIndex]);
	aEntry.Entry().iDate = ColumnInt64(aStmt, iDateIndex[aIndex]);
	aEntry.Entry().iSize = ColumnInt(aStmt, iSizeIndex[aIndex]);
	aEntry.Entry().iError = ColumnInt(aStmt, iErrorIndex[aIndex]);
	aEntry.Entry().iBioType = ColumnInt(aStmt, iBiotypeIndex[aIndex]);
	aEntry.Entry().iMtmData1 = ColumnInt(aStmt, iMtmData1Index[aIndex]);
	aEntry.Entry().iMtmData2 = ColumnInt(aStmt, iMtmData2Index[aIndex]);
	aEntry.Entry().iMtmData3 = ColumnInt(aStmt, iMtmData3Index[aIndex]);
	aEntry.Entry().iServiceId = ColumnInt(aStmt, iServiceIdIndex[aIndex]);
	aEntry.Entry().iMtm.iUid = ColumnInt(aStmt, iMtmIdIndex[aIndex]);
	aEntry.iDescriptionBuffer = ColumnTextL(aStmt, iDescriptionIndex[aIndex]);
	aEntry.Entry().iDescription.Set(*aEntry.iDescriptionBuffer);
	aEntry.iDetailsBuffer = ColumnTextL(aStmt, iDetailsIndex[aIndex]);
	aEntry.Entry().iDetails.Set(*aEntry.iDetailsBuffer);
	
	//Get the visible parent id
	aVisibleParentEntryId = ColumnInt(aStmt, iVisibleParent[aIndex]);
	}




/**
 * GetNextSiblingL()
 *
 * @param TMsvId: TMsvId of the entry whose sibling id is to be found.
 * @param TMsvId: TMsvId of the parent entry of aId.
 * @param TMsvId&: TMsvId of the sibling of aId, this is an o/p parameter.
 * @return TBool
 *
 * Gets the id of the sibling of aId.
 */
TBool CMsvDBAdapter::GetNextSiblingL(TMsvId aId,TMsvId aParentId,TMsvId& aNextSiblingId)
	{
	TBool foundSibling=EFalse;
	//get all the children for this parent
	CMsvEntrySelection* children=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);
	GetChildrenIdL(aParentId,*children);
	TInt childCount=children->Count();
	
	for(TInt i=0;i<childCount;++i)
		{
		if(children->At(i)==aId && i<childCount-1)
			{
			aNextSiblingId=children->At(i+1);
			foundSibling=ETrue;
			break;
			}
		}
	CleanupStack::PopAndDestroy(children);
	return foundSibling;
	}



/**
 * GetFirstChildIdL()
 *
 * @param TMsvId: TMsvId of the parent entry of aId.
 * @param TMsvId: TMsvId of the child of aParentId, this is an o/p parameter.
 * @return TBool.
 *
 * Gets the id of first child of aParentId.
 */
TBool CMsvDBAdapter::GetFirstChildIdL(TMsvId aParentId,TMsvId& aFirstChild)
	{
	TBool foundChild=EFalse;
	//get all the children for this parent
	CMsvEntrySelection* children=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);
	GetChildrenIdL(aParentId,*children);
	TInt childCount=children->Count();
	
	if(childCount>0)
		{
		aFirstChild=children->At(0);
		foundChild=ETrue;
		}
	CleanupStack::PopAndDestroy(children);
	return foundChild;
	}


/**
 * ConstructSortTableL()
 * @param none.
 * #return none.
 * 
 * This functions is called to create a new table,
 * which stores the information for the search/sort
 * operations performed.
 */ 
void CMsvDBAdapter::ConstructSortTableL()
	{
		
	//Create Search Sort Cache table. This stores the results of
	// search sort result	
	TInt err = iDatabase.Exec(KCreateSearchSortEntryTableQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(err == 0)
		{
		ClearDBContentsL();
		}
#endif
	
	//Create Unique index's on indexEnty table.
	//Unique Index is created on all columns to defer duplicates.
	err = iDatabase.Exec(KCreateSearchSortQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	}




/**
 * InitializeSortTableL()
 * @param none.
 * #return none.
 * 
 * This functions prepares (compiles) frequetly used SQL queries,
 * and create indexes on the variable arguments. When the query
 * needs to be executed, we just have to bind the variables with
 * the values and execute them. This increases the performance of
 * query execution.
 */
void CMsvDBAdapter::InitializeSortTableL()
	{						 
	//Prepare the frequently used SQL statements
	User::LeaveIfError(iQueryStatement[EQueryCreateEntry].Prepare(iDatabase, KCreateQueryEntry));
	User::LeaveIfError(iQueryStatement[EQueryGetEntryAsc].Prepare(iDatabase, KGetEntryAsc));
	User::LeaveIfError(iQueryStatement[EQueryGetEntryDesc].Prepare(iDatabase, KGetEntryDesc));
	User::LeaveIfError(iQueryStatement[EQueryGetEntryNumAsc].Prepare(iDatabase, KGetEntryNumAsc));
	User::LeaveIfError(iQueryStatement[EQueryGetEntryNumDesc].Prepare(iDatabase, KGetEntryNumDesc));
	User::LeaveIfError(iQueryStatement[EQueryGetLastNIdsAsc].Prepare(iDatabase, KGetLastNIdsAsc));
	User::LeaveIfError(iQueryStatement[EQueryGetLastNIdsDesc].Prepare(iDatabase, KGetLastNIdsDesc));
	User::LeaveIfError(iQueryStatement[EQueryGetLastNIdsNumAsc].Prepare(iDatabase, KGetLastNIdsNumAsc));
	User::LeaveIfError(iQueryStatement[EQueryGetLastNIdsNumDesc].Prepare(iDatabase, KGetLastNIdsNumDesc));
	User::LeaveIfError(iQueryStatement[EGetFolderIds].Prepare(iDatabase, KGetFolderIds));
	User::LeaveIfError(iQueryStatement[EQueryCount].Prepare(iDatabase, KGetQueryCount));
	User::LeaveIfError(iQueryStatement[EQueryIteratorNumAsc].Prepare(iDatabase, KGetIterResultCastAsc));
	User::LeaveIfError(iQueryStatement[EQueryIteratorNumDesc].Prepare(iDatabase, KGetIterResultCastDesc));
	User::LeaveIfError(iQueryStatement[EQueryIteratorAsc].Prepare(iDatabase, KGetIterResultAsc));
	User::LeaveIfError(iQueryStatement[EQueryIteratorDesc].Prepare(iDatabase, KGetIterResultDesc));

	//Get the indices - This maps the parameter name to its offset
	//in the SQL statement templates.
	for(TInt index = EQueryCreateEntry; index < EQuerySentinel; ++index)
		{
		switch(index)
			{							 
			case EQueryCreateEntry:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryTMsvId());
				iSortfieldQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQuerySortfield());
				break;
				}

			case EQueryGetEntryAsc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());
				break;
				}

			case EQueryGetEntryDesc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());
				break;
				}
				
			case EQueryGetEntryNumAsc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());
				break;
				}

			case EQueryGetEntryNumDesc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());
				break;
				}
				
			case EQueryGetLastNIdsAsc:
				{
				iRowIdIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryRowId());
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());
				break;
				}
				
			case EQueryGetLastNIdsDesc:
				{
				iRowIdIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryRowId());
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());
				break;
				}
		
			case EQueryGetLastNIdsNumAsc:
				{
				iRowIdIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryRowId());
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());	
				break;
				}
			
			case EQueryGetLastNIdsNumDesc:
				{
				iRowIdIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryRowId());
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIdQueryIndex[index] = GetColumnIndexL(iQueryStatement[index], KQueryColumnTMsvId());	
				break;
				}
				
			case EGetFolderIds:
				{
				iParentIdfield = GetParamIndexL(iQueryStatement[index], KMsgParamParentId());
				iTMsvidfield = GetParamIndexL(iQueryStatement[index], KMsgParamType());
				break;
				}
					
			case EQueryCount:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				break;
				}
			
			case EQueryIteratorNumAsc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIterOffset[index] =GetParamIndexL(iQueryStatement[index], KOffset());
				iIterLimit[index] =GetParamIndexL(iQueryStatement[index], KLimit());
				break;
				}
			
			case EQueryIteratorNumDesc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIterOffset[index] =GetParamIndexL(iQueryStatement[index], KOffset());
				iIterLimit[index] =GetParamIndexL(iQueryStatement[index], KLimit());
				break;
				}
			
			case EQueryIteratorAsc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIterOffset[index] =GetParamIndexL(iQueryStatement[index], KOffset());
				iIterLimit[index] =GetParamIndexL(iQueryStatement[index], KLimit());
				break;
				}
			
			case EQueryIteratorDesc:
				{
				iQIdQueryIndex[index] = GetParamIndexL(iQueryStatement[index], KQueryQId());
				iIterOffset[index] =GetParamIndexL(iQueryStatement[index], KOffset());
				iIterLimit[index] =GetParamIndexL(iQueryStatement[index], KLimit());
				break;
				}

			default:
				break;
			}
		}
	}



	
/**
 * CreateQueryEntryL()
 * @param TInt&: The Id of the Search/Sort Query
 * @param TMsvIdWithSortField&: Fields to be added under the query id specified
 * @return none.
 *
 * This Function adds the Entries into the DB under the Queryid specified
 */
void CMsvDBAdapter::CreateQueryEntryL(const TInt& aQueryId,const TMsvIdWithSortField& aEntry)
	{
	//Bind the entry data
	BindIntL(iQueryStatement[EQueryCreateEntry], iQIdQueryIndex[EQueryCreateEntry], aQueryId);
	
	#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		BindIntL(iQueryStatement[EQueryCreateEntry], iIdQueryIndex[EQueryCreateEntry], UnmaskTMsvId(aEntry.iMessageId));
	#else
		BindIntL(iQueryStatement[EQueryCreateEntry], iIdQueryIndex[EQueryCreateEntry], aEntry.iMessageId);
	#endif		
	
	
	BindTextL(iQueryStatement[EQueryCreateEntry], iSortfieldQueryIndex[EQueryCreateEntry], aEntry.iContentMessagePart);
		
	TInt err = iQueryStatement[EQueryCreateEntry].Exec();
	if(err < 0)
		{
		iQueryStatement[EQueryCreateEntry].Reset();
		User::Leave(err);
		}
	
	//Reset the RSqlStatement
	User::LeaveIfError(iQueryStatement[EQueryCreateEntry].Reset());
	}
	



/**
 * GetSortedEntriesL()
 * @param TInt&: The Id of the Search/Sort Query
 * @param RArray<TMsvIdWithSortField>&: this array will be filled with the resultant ids
 * #param TBool: Order of the Sort operation, default value is EFalse which performs sort in Ascending order
 * @return none.
 *
 * This Function retrieves all the ids under the specified Query Id
 */
void CMsvDBAdapter::GetSearchSortEntriesL(const TInt& aQueryId, RArray<TMsvId>& aEntrySelection, const TBool aOrder/*Default = EFalse*/,const TBool aSortAsInt/*Default = EFalse*/)
	{
	TInt sortOrder;
	TMsvId entryId;
	
	if(aSortAsInt)
		{
		if(aOrder)
			{
			sortOrder = EQueryGetEntryNumAsc;
			}
		else
			{
			sortOrder = EQueryGetEntryNumDesc;
			}
		}
	else
		{
		if(aOrder)
			{
			sortOrder = EQueryGetEntryAsc;
			}
		else
			{
			sortOrder = EQueryGetEntryDesc;
			}
		}
	
	//Bind the msv id of the entry to be retrieved
	BindIntL(iQueryStatement[sortOrder], iQIdQueryIndex[sortOrder], aQueryId);

	while(KSqlAtRow == iQueryStatement[sortOrder].Next())
		{
		entryId = ColumnInt(iQueryStatement[sortOrder], iIdQueryIndex[sortOrder]);
		
		#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			entryId = MaskTMsvId(KCurrentDriveId,entryId);
		#endif
		
		aEntrySelection.AppendL(entryId);
		}

	//Reset the RSqlStatement
	User::LeaveIfError(iQueryStatement[sortOrder].Reset());
	}




/**
 * GetLastNTMsvIdsfromTableL()
 * @param TInt&: The Id of the Search/Sort Query
 * @param RArray<TMsvIdWithSortField>&: this array will be filled with the resultant ids
 * @param TInt aLastCount: The Number of entries from the last rowid to be fetched from
 * @param TBool: Order of the Sort operation, default value is EFalse which performs sort in Ascending order
 * @return none.
 *
 * This Function retrieves the last N ids under the specified Query Id
 */
void CMsvDBAdapter::GetLastNTMsvIdsfromTableL(const TInt& aQueryId, RArray<TMsvId>& aEntrySelection, TInt aLastCount, const TBool aOrder/*Default = ETrue*/,const TBool aSortAsInt/*Default = EFalse*/)
	{
	TInt sortOrder;
	TMsvId entryId;

	if(aSortAsInt)
		{
		if(aOrder)
			{
			sortOrder = EQueryGetLastNIdsAsc;
			}
		else
			{
			sortOrder = EQueryGetLastNIdsDesc;
			}
		}
	else
		{
		if(aOrder)
			{
			sortOrder = EQueryGetLastNIdsNumAsc;
			}
		else
			{
			sortOrder = EQueryGetLastNIdsNumDesc;
			}
		}
	
	//Bind the count value
	BindIntL(iQueryStatement[sortOrder], iRowIdIndex[sortOrder], aLastCount);
	
	//Bind the msv id of the entry to be retrieved
	BindIntL(iQueryStatement[sortOrder], iQIdQueryIndex[sortOrder], aQueryId);

	while(KSqlAtRow == iQueryStatement[sortOrder].Next())
		{
		entryId = ColumnInt(iQueryStatement[sortOrder], iIdQueryIndex[sortOrder]);
		
		#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			entryId = MaskTMsvId(KCurrentDriveId, entryId);
		#endif
		
		aEntrySelection.AppendL(entryId);
		}

	//Reset the RSqlStatement
	User::LeaveIfError(iQueryStatement[sortOrder].Reset());
	}




/**
 * DeleteTMsvIdFromAllQueriesL()
 * @param TMsvId&: The id to be deleted
 * @return none.
 *
 * This Function delete a id under all Queryid's
 */
void CMsvDBAdapter::DeleteTMsvIdFromAllQueriesL(const TMsvId& aId)
	{	
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(KDeleteId().Length() + 20);
	queryBuf.Append(KDeleteId);
	queryBuf.AppendNum(aId);
	queryBuf.Append(KSemiColon);
	
	TInt err = iDatabase.Exec(queryBuf);
	CleanupStack::PopAndDestroy();		// queryBuf
	if(err < 0)
		{		
		User::Leave(err);
		}
	}




/**
 * DeleteQueryFromTableL()
 * @param TInt&: The Id of the Search/Sort Query
 * @return none.
 *
 * This Function deletes a particular Query from DB
 */
void CMsvDBAdapter::DeleteQueryFromTableL(const TInt& aQueryId)
	{
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(KDeleteQueryId().Length() + 20);
	queryBuf.Append(KDeleteQueryId);
	queryBuf.AppendNum(aQueryId);
	queryBuf.Append(KSemiColon);
	
	TInt err = iDatabase.Exec(queryBuf);
	CleanupStack::PopAndDestroy();		// queryBuf
	if(err < 0)
		{		
		User::Leave(err);
		}
	}




/**
 * DeleteTMsvIdFromQueryL()
 * @param TInt&: The Id of the Search/Sort Query
 * @param TMsvId&: The TMsvId of the entry to be deleted 
 * @return none.
 *
 * This Function deletes a particular id under the specified QueryId
 */
void CMsvDBAdapter::DeleteTMsvIdFromQueryL(const TInt& aQueryId,const TMsvId& aId)
	{
	RBuf8 queryBuf;
	CleanupClosePushL(queryBuf);
	queryBuf.CreateL(KDeleteQueryId().Length()+KAndId().Length() + 20);
	queryBuf.Append(KDeleteQueryId);
	queryBuf.AppendNum(aQueryId);
	queryBuf.Append(KAndId);
	queryBuf.AppendNum(aId);
	queryBuf.Append(KSemiColon);
	
	TInt err = iDatabase.Exec(queryBuf);
	CleanupStack::PopAndDestroy();		// queryBuf
	if(err < 0)
		{		
		User::Leave(err);
		}
	}




/**
 * AddEntriesfromIndexTableL()
 * @param TDesC8&: The Query to fill the SearchSortTable with the IndexEntry
 * @return number of entries successfully added.
 *
 * This Function fills the SearchSortTable with the query specified on the IndexEntryTable
 */
TInt CMsvDBAdapter::AddEntriesfromIndexTableL(const TDesC8& aQuery)
	{
	TInt err = iDatabase.Exec(aQuery);
	
	TPtrC errMsg = iDatabase.LastErrorMessage();
	
	if(err < 0)
		{
		User::Leave(err);
		}

	return err;
	}




/**
 * ReturnIdCountInDBL()
 * @param TInt&: The Id of the Search/Sort Query
 * @return TInt: The number of entries stored under the QueryId specified
 *
 * This Function returns the number of id's stored under the specified QueryId
 */
TInt CMsvDBAdapter::ReturnIdCountInDBL(const TInt& aQueryId)
	{
	TInt count=0;
	//Bind the msv id of the entry to be retrieved
	BindIntL(iQueryStatement[EQueryCount], iQIdQueryIndex[EQueryCount], aQueryId);

	while(KSqlAtRow == iQueryStatement[EQueryCount].Next())
		{
		count = ColumnInt(iQueryStatement[EQueryCount], iIdQueryIndex[EQueryCount]);
		}

	//Reset the RSqlStatement
	User::LeaveIfError(iQueryStatement[EQueryCount].Reset());
	return count;
	}




/**
 * SearchTMsvIdL()
 * @param aQuery        An object representing the the search sort query.
 * @param aIdSelection  RArray holding the resultant TMsvId list. 
 * 
 * This function should be used to retrieve TMsvId list of result set.					 
 * Queries the table holding TMsvEntry and retrieves id list of matched results.
 */
void CMsvDBAdapter::SearchTMsvIdL(const TDesC8& aQuery, RArray<TMsvId>& aIdSelection)
	{
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, aQuery));
	
	TInt idColumnIndex = GetColumnIndexL(queryStmt, KMsgColumnId());
	while(KSqlAtRow == queryStmt.Next())
		{
		
		TMsvId id = ColumnInt(queryStmt, idColumnIndex);
		
		#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			id = MaskTMsvId(KCurrentDriveId, id);
		#endif
		
		aIdSelection.AppendL(id);
		}

	CleanupStack::PopAndDestroy();
	}




/**
 * GetFolderIdsL()
 * @param aParentId      Parent Id of the folder.
 * @param aFolderIdList  RArray holding the resultant TMsvId list. 
 * 
 * This function returns the id's of all the KUidMsvMessageEntry type under the parent.
 */
void CMsvDBAdapter::GetFolderIdsL(const TMsvId aParentId, RArray<TMsvId>& aFolderIdList)
	{
	TMsvId entryId;
	TUid folderType = KUidMsvFolderEntry;
	
	#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		{
		_LIT8(KGetFolderIds, "SELECT id FROM DB");
		_LIT8(KGetFolderQuery,".IndexEntry WHERE parentId = ");
	
		_LIT8(KTypeLike," AND type = ");

		RBuf8 folderIdQuery;
		CleanupClosePushL(folderIdQuery);
		folderIdQuery.CreateL(500);		
		folderIdQuery.Append(KGetFolderIds);
		folderIdQuery.AppendNum(KCurrentDriveId);
		folderIdQuery.Append(KGetFolderQuery);
		folderIdQuery.AppendNum(aParentId);
		folderIdQuery.Append(KTypeLike);
		folderIdQuery.AppendNum(folderType.iUid);
		folderIdQuery.Append(KSemiColon);
	
		RSqlStatement folderquerystatement;
	
		User::LeaveIfError(folderquerystatement.Prepare(iDatabase, folderIdQuery));
	
		while(KSqlAtRow == folderquerystatement.Next())
			{
			entryId = ColumnInt(folderquerystatement, 0);
			aFolderIdList.AppendL(entryId);
			}
		folderquerystatement.Close();
		CleanupStack::PopAndDestroy();  //folderIdQuery
		}
	#else
		{
		//Bind the msv id of the entry to be retrieved
		BindIntL(iQueryStatement[EGetFolderIds], iParentIdfield, aParentId);
		BindIntL(iQueryStatement[EGetFolderIds], iTMsvidfield, folderType.iUid);
		
		while(KSqlAtRow == iQueryStatement[EGetFolderIds].Next())
			{
			entryId = ColumnInt(iQueryStatement[EGetFolderIds], iParentIdfield);
			aFolderIdList.AppendL(entryId);
			}
		//Reset the RSqlStatement
		User::LeaveIfError(iQueryStatement[EGetFolderIds].Reset());
		}
	#endif
	}




/**
 * ClearDBContentsL()
 * @param none.
 * @param none.
 * 
 * This function clears the DB contents.
 */
void CMsvDBAdapter::ClearDBContentsL()
	{
#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	RSqlDatabase temp;
	CleanupClosePushL(temp);
	//Get the system drive.
	TInt systemDrive = (TInt)RFs::GetSystemDrive();
	
	RBuf dbFile;
	CleanupClosePushL(dbFile);
	dbFile.CreateL(50);	
	dbFile.Append(TDriveUnit(systemDrive).Name());
	dbFile.Append(KMainDummyDBName);
	
	TRAPD(err, temp.OpenL(dbFile));
	if(err == KErrNone)
		{
		err = temp.Exec(KClearTableContents);
		if(err < 0)
			{
			User::Leave(err);
			}
		}
	temp.Close();
	CleanupStack::PopAndDestroy(2); //temp, dbFile
#else
	TInt err = iDatabase.Exec(KDeleteTable);
	if(err < 0)
		{
		User::Leave(err);
		}
#endif	
	
	}




/**
 * ReturnIdsInIteratorL()
 * @param aQueryId      Query ID of the query to be executed.
 * @param prevCount     The previous value returned to the caller. 
 * @param aId           If successfull, than this contains the result.
 
 * This function returns the TMsvId's for each unique Query ID.
   Upon called for the first time, it binds the query id to the statement and returns 
   next result in each subsequent execution. If the statement fails for some reason, than it returns the 
   error code.	
 */
void CMsvDBAdapter::ReturnIdsInIteratorL(const TInt& aQueryId, const TBool aOrder/*Default = ETrue*/,const TBool aSortAsInt/*Default = EFalse*/,const TInt aLimit/*Default = 1*/)
	{
	if(aSortAsInt)
		{
		if(aOrder)
			{
			iIterQuery = EQueryIteratorNumAsc;
			}
		else
			{
			iIterQuery = EQueryIteratorNumDesc;
			}
		}
	else
		{
		if(aOrder)
			{
			iIterQuery = EQueryIteratorAsc;
			}
		else
			{
			iIterQuery = EQueryIteratorDesc;
			}
		}
		
	iOffsetValue = 0;
	iQueryId = aQueryId;
	iLimit = aLimit;
	
	iIdColumnIndex = GetColumnIndexL(iQueryStatement[iIterQuery], KMsgColumnId());	
	}

/**
 * GetNextL()
 * @param aId           If successfull, than this contains the result.
 
 * This function returns the next TMsvId's from the Iterator	
 */	
TInt CMsvDBAdapter::GetNextL(TMsvId& aId)
	{
	BindIntL(iQueryStatement[iIterQuery], iQIdQueryIndex[iIterQuery], iQueryId);
	BindIntL(iQueryStatement[iIterQuery], iIterLimit[iIterQuery], iLimit);
	BindIntL(iQueryStatement[iIterQuery], iIterOffset[iIterQuery], iOffsetValue);
	
	while(iQueryStatement[iIterQuery].Next() == KSqlAtRow)
		{
		aId = ColumnInt(iQueryStatement[iIterQuery],iIdColumnIndex);
		
		#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			aId = MaskTMsvId(KCurrentDriveId, aId);
		#endif
			
		++iOffsetValue;
		}
	
	User::LeaveIfError(iQueryStatement[iIterQuery].Reset());
	return iOffsetValue;
	}
	

/**
 * GetPrevL()
 * @param aId           If successfull, than this contains the result.
 
 * This function returns the previous TMsvId's from the Iterator	
 */
TInt CMsvDBAdapter::GetPrevL(TMsvId& aId)
	{
	BindIntL(iQueryStatement[iIterQuery], iQIdQueryIndex[iIterQuery], iQueryId);
	BindIntL(iQueryStatement[iIterQuery], iIterLimit[iIterQuery], iLimit);
	BindIntL(iQueryStatement[iIterQuery], iIterOffset[iIterQuery], iOffsetValue);
	
	while(iQueryStatement[iIterQuery].Next() == KSqlAtRow)
		{
		aId = ColumnInt(iQueryStatement[iIterQuery],iIdColumnIndex);
		#if(defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			aId = MaskTMsvId(KCurrentDriveId, aId);
		#endif
		
		--iOffsetValue;
		}
	
	User::LeaveIfError(iQueryStatement[iIterQuery].Reset());
	return iOffsetValue;
	}





#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 * ValidateDatabaseL()
 * 
 * @param TFileName& The database filename to be checked.
 * @return None.
 *
 * Checks the database version to see if it is supported.
 * Supported version is KCurrentDatabaseVersion (1) only.
 */
void CMsvDBAdapter::ValidateDatabaseL(const TFileName& aDbFile)
	{
	_LIT8(KSelectVersionQuery, "SELECT version FROM VersionTable;");
	
	//Use a temporary local database connection to open the file for now.
	RSqlDatabase temp;
	CleanupClosePushL(temp);
	temp.OpenL(aDbFile);	
	TSqlScalarFullSelectQuery query(temp);

	TInt version = 0;
	version = query.SelectIntL(KSelectVersionQuery);
	if(KCurrentDatabaseVersion != version)
		{
		User::Leave(KErrNotSupported);
		}
	CleanupStack::PopAndDestroy();  // temp
	}
#endif



#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
/** 
* GetMtmIdsL()
*
* @param aMtmIdTable: aMtmIdTable will be updated with MtmIds
* @param aQuery: QueryString which is prepared depending on SearchSort Query
* return NONE
*  
*  Function to get MtmId/s for given parent id.
*/ 
void CMsvDBAdapter::GetMtmIdsL(const TDesC8& aQuery, RArray<TUid>& aMtmIdArray)
	{
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, aQuery));
	
	TInt mtmIdColumnIndex = GetColumnIndexL(queryStmt, KMsgColumnMtmId());
	
	TUid tempMtmId ;
	TInt32 mtmId = KErrNotFound;
	
	while(KSqlAtRow == queryStmt.Next())
		{
		mtmId = ColumnInt(queryStmt, mtmIdColumnIndex);
		
		tempMtmId.iUid = mtmId;
		aMtmIdArray.AppendL(tempMtmId);
		}
	
	CleanupStack::PopAndDestroy();
	}



/***************************** Converter************************/

/**
OpenL()
Opens a database and returns a handle to it.

@param aDbFileName: DB name.
@retun :CMsvDBAdapter object.
*/	
CMsvDBAdapter* CMsvDBAdapter::OpenL(const TFileName& aDbFileName)
	{
	//Create an messaging DB adaptor object 
	CMsvDBAdapter* self = new(ELeave) CMsvDBAdapter();
	CleanupStack::PushL(self);
	
	// Open the database.
	self->ConstructOpenDBL(aDbFileName);
	
	CleanupStack::Pop(self);
	return self;
	}
	
/**
ConstructOpenDBL()

@param aDbFileName: DB name.
@retun None
*/
void CMsvDBAdapter::ConstructOpenDBL(const TFileName& aDbFileName)
	{
	User::LeaveIfError(iDatabase.Open(aDbFileName));
	iDatabasePresent = new(ELeave)RArray<TBool>(KMaxNumberOfDrives);
	for(TInt index = 0; index < KMaxNumberOfDrives; ++index)
		{
		iDatabasePresent->Append(EFalse);
		}
	}

	
/**
 * InitializeL()
 *
 * This functions prepares (compiles) frequetly used SQL queries,
 * and create indexes on the variable arguments. When the query
 * needs to be executed, we just have to bind the variables with
 * the values and execute them. This increases the performance of
 * query execution.
 */
void CMsvDBAdapter::InitializeLConverterStatementL()
	{
	//Prepare the frequently used SQL statements
	User::LeaveIfError(iStatement[ECreateEntry].Prepare(iDatabase, KCreateEntryQuery));
	
	//For creating and updating entries
	iParentIdIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamParentId());
	iVisibleParent[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamVisibleParent());
	iIdIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamId());				
	iServiceIdIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamServiceId());
	iMtmIdIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamMtmId());
	iTypeIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamType());
	iDateIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamDate());
	iDataIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamData());
	iSizeIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamSize());
	iErrorIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamError());
	iMtmData1Index[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamMtmData1());
	iMtmData2Index[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamMtmData2());
	iMtmData3Index[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamMtmData3());
	iRelatedIdIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamRelatedId());
	iBiotypeIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamBiotype());
	iPcSyncCountIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamPcSyncCount());
	iReservedIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamReserved());
	iDescriptionIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamDescriptionTxt());
	iDetailsIndex[ECreateEntry] = GetParamIndexL(iStatement[ECreateEntry], KMsgParamDetailsTxt());
	}


/**
CreateIndexEntryL()
Inserts an entry in IndexEntry table.  

@param aNewEntry: TMsvEntry.
@param aImmediateVisibleFolder: Visible Parent.
@retun None
*/
void CMsvDBAdapter::CreateIndexEntryL(const TMsvEntry& aNewEntry, const TMsvId aImmediateVisibleFolder)
	{
	//Bind the entry data
	BindIntL(iStatement[ECreateEntry], iIdIndex[ECreateEntry], aNewEntry.iId);
	BindIntL(iStatement[ECreateEntry], iParentIdIndex[ECreateEntry], aNewEntry.iParentId);
	BindIntL(iStatement[ECreateEntry], iDataIndex[ECreateEntry], aNewEntry.iData);
	BindIntL(iStatement[ECreateEntry], iPcSyncCountIndex[ECreateEntry], aNewEntry.iPcSyncCount);
	BindIntL(iStatement[ECreateEntry], iReservedIndex[ECreateEntry], aNewEntry.iReserved);
	BindIntL(iStatement[ECreateEntry], iRelatedIdIndex[ECreateEntry], aNewEntry.iRelatedId);
	BindIntL(iStatement[ECreateEntry], iTypeIndex[ECreateEntry], aNewEntry.iType.iUid);
	BindInt64L(iStatement[ECreateEntry], iDateIndex[ECreateEntry], aNewEntry.iDate.Int64());
	BindIntL(iStatement[ECreateEntry], iSizeIndex[ECreateEntry], aNewEntry.iSize);
	BindIntL(iStatement[ECreateEntry], iErrorIndex[ECreateEntry], aNewEntry.iError);
	BindIntL(iStatement[ECreateEntry], iBiotypeIndex[ECreateEntry], aNewEntry.iBioType);
	BindIntL(iStatement[ECreateEntry], iMtmData1Index[ECreateEntry], aNewEntry.iMtmData1);
	BindIntL(iStatement[ECreateEntry], iMtmData2Index[ECreateEntry], aNewEntry.iMtmData2);
	BindIntL(iStatement[ECreateEntry], iMtmData3Index[ECreateEntry], aNewEntry.iMtmData3);
	BindIntL(iStatement[ECreateEntry], iServiceIdIndex[ECreateEntry], aNewEntry.iServiceId);
	BindIntL(iStatement[ECreateEntry], iMtmIdIndex[ECreateEntry], aNewEntry.iMtm.iUid);
	BindIntL(iStatement[ECreateEntry], iVisibleParent[ECreateEntry], aImmediateVisibleFolder);
	BindTextL(iStatement[ECreateEntry],iDescriptionIndex[ECreateEntry],aNewEntry.iDescription);
	BindTextL(iStatement[ECreateEntry],iDetailsIndex[ECreateEntry],aNewEntry.iDetails);
	//Execute the prepared SQL statement.
	
	TInt err = iStatement[ECreateEntry].Exec();
	if(err < 0)
		{
		iStatement[ECreateEntry].Reset();
		User::Leave(err);
		}
	else if(!err)
		{
		iStatement[ECreateEntry].Reset();
		User::Leave(KErrGeneral);
		}
	
	//Reset the RSqlStatement
	User::LeaveIfError(iStatement[ECreateEntry].Reset());	
	}


/**
ChangeVersionL()
Changes message store version of messaging database.

@param aVersion: version
@retun None
*/	
void CMsvDBAdapter::ChangeVersionL(TInt aVersion)
	{
	_LIT(KUpdateVersionQuery, "UPDATE VersionTable SET version = "); 
		
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KUpdateVersionQuery);
	buf.AppendNum(aVersion);
	buf.Append(KDelimiter);
	
	TInt err = iDatabase.Exec(buf);
	if(err < 0)
		{
		User::Leave(err);
		}
	CleanupStack::PopAndDestroy(); //buf
	}

	
/**************************** Conversion Status Table API's **************************/
/**
CreateConversionStatusTableL()
Creates conversion status table.

@param None: 
@retun None
*/
void CMsvDBAdapter::CreateConversionStatusTableL()
	{
	TInt err = iDatabase.Exec(KCreateConversionStatusTableQuery);
	if(err < 0)
		{
		User::Leave(KErrGeneral);
		}
	}

/**
RemoveConversionStatusTableL()
Removes conversion status table.

@param None: 
@retun None
*/	
void CMsvDBAdapter::RemoveConversionStatusTableL()
	{
	TInt err = iDatabase.Exec(KRemoveConversionStatusTable);
	if(err < 0)
		{
		User::Leave(err);
		}
	}

/**
GetConversionStatusL()
Fetches conversion resume state from the conversion status table.

@param aConversionResumeStatus: Conversion sresume status entry.
@retun None
*/	
void CMsvDBAdapter::GetConversionStatusL(TConversionResumeStatus& aConversionResumeStatus)
	{
	// We shld not get more than one mtm results for this query
	_LIT16(KGetConversionStatusQuery, "SELECT * FROM ConversionStatus WHERE remainingCount != 0;"); 
	
	RSqlStatement statement;
	CleanupClosePushL(statement);
	TInt index =0;
	
	User::LeaveIfError(statement.Prepare(iDatabase, KGetConversionStatusQuery));
		
	while(KSqlAtRow ==  statement.Next())
		{
		aConversionResumeStatus.lastId = ColumnInt(statement, index);
		aConversionResumeStatus.serviceId = ColumnInt(statement, ++index);
		aConversionResumeStatus.sourceVersion = ColumnInt(statement, ++index);
		aConversionResumeStatus.targetVersion = ColumnInt(statement, ++index);
		aConversionResumeStatus.mtmId.iUid = ColumnInt(statement, ++index);
		aConversionResumeStatus.remainingCount = ColumnInt(statement, ++index);
		index =0;
		}
	CleanupStack::PopAndDestroy(); //statement
	}

/**
AddConversionStatusEntryL()
Inserts a conversion status entry in the conversion status table.

@param aConversionResumeStatus: Conversion sresume status entry.
@retun None
*/	
void CMsvDBAdapter::AddConversionStatusEntryL(TConversionResumeStatus& aConversionResumeStatus)
	{
	_LIT8(KAddConversionStatusEntryQuery, "INSERT INTO ConversionStatus VALUES ("
							 ":lastId, :serviceId, :sourceVersion, :targetVersion, :mtmId, :remainingCount);"
		 ); //
			 
	RSqlStatement statement;
	CleanupClosePushL(statement);
	TInt index =0;
	
	User::LeaveIfError(statement.Prepare(iDatabase,KAddConversionStatusEntryQuery));
	BindIntL(statement, index++, aConversionResumeStatus.lastId);
	BindIntL(statement, index++, aConversionResumeStatus.serviceId);
	BindIntL(statement, index++, aConversionResumeStatus.sourceVersion);
	BindIntL(statement, index++, aConversionResumeStatus.targetVersion);
	BindIntL(statement, index++, aConversionResumeStatus.mtmId.iUid);
	BindIntL(statement, index++, aConversionResumeStatus.remainingCount);
	
	TInt err = statement.Exec();
	
	CleanupStack::PopAndDestroy(); //statement

	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err )
		{
		User::Leave(KErrGeneral);
		}
	}

/**
UpdateConversionStatusEntryL()
Updates a conversion status entry in the conversion status table.

@param aConversionResumeStatus:Conversion sresume status entry.
@retun None
*/	
void CMsvDBAdapter::UpdateConversionStatusEntryL(TConversionResumeStatus& aConversionResumeStatus)	
	{
	_LIT(KUpdateConversionStatusQuery1,	"UPDATE ConversionStatus SET remainingCount = "); //
	_LIT(KUpdateConversionStatusQuery2,	" WHERE mtmId = "); //
	_LIT(KUpdateConversionStatusQuery3,	" AND serviceId = "); //
	 
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KUpdateConversionStatusQuery1);
	buf.AppendNum(aConversionResumeStatus.remainingCount);
	buf.Append(KUpdateConversionStatusQuery2);
	buf.AppendNum(aConversionResumeStatus.mtmId.iUid);
	buf.Append(KUpdateConversionStatusQuery3);
	buf.AppendNum(aConversionResumeStatus.serviceId);
	buf.Append(KDelimiter); 
	 
	TInt err = iDatabase.Exec(buf);
	CleanupStack::PopAndDestroy(); //buf
	
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err )
		{
		User::Leave(KErrGeneral);
		}
	}

/**
RemoveConversionStatusEntriesL()
Removes all conversion status entries in the conversion status table.

@param None: 
@retun None
*/		
void CMsvDBAdapter::RemoveConversionStatusEntriesL()	
	{
 	_LIT(KRemoveConversionStatusEntry,	"DELETE FROM ConversionStatus;");
	
	RSqlStatement statement;
	CleanupClosePushL(statement);
	User::LeaveIfError(statement.Prepare(iDatabase, KRemoveConversionStatusEntry));
	TInt err = statement.Exec();
	
	CleanupStack::PopAndDestroy(); // statement
	
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(err != KErrNone)
		{
		User::Leave(KErrGeneral);
		}
	}

/**
GetRemainingServiceIdsL()
Fetches all service ids for which the conversion is not complete from the conversion status table.

@param aIdArray:Array of TMsvId.
@retun None
*/	
void CMsvDBAdapter::GetRemainingServiceIdsL(RArray<TMsvId>& aIdArray)
	{
	_LIT(KGetToResumeServiceIds, "SELECT serviceId FROM ConversionStatus WHERE remainingCount != 0;"); 	
	
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, KGetToResumeServiceIds));
	
	TInt serviceColumnIndex = GetColumnIndexL(queryStmt, KMsgColumnServiceId());
	
	while(KSqlAtRow == queryStmt.Next())
		{
		TMsvId serviceId = ColumnInt(queryStmt, serviceColumnIndex);
		aIdArray.AppendL(serviceId);
		}
	CleanupStack::PopAndDestroy(); // stmt
	}

/**
GetAllMessageIdsL()
Gets all message ids for a service id. If the conversion is resumed than retrives the message ids
after the last id that was converted. 

@param aServiceId: service id.
@param aIdArray: Array of TMsvId.
@param lastId: last id that was converted and commited in the database.
@retun None
*/	
void CMsvDBAdapter::GetAllMessageIdsL(TMsvId aServiceId, RArray<TMsvId>& aIdArray,TInt32 aMtm,TMsvId lastId)
	{
	_LIT16(KSelectMessageId, "Select id FROM IndexEntry WHERE type = ");
	_LIT16(KMessageMtmId," AND mtmId = ");
	_LIT16(KSelectServiceId," AND serviceId = ");
	_LIT16(KMessageId, " AND id > ");
	_LIT16(KOrderByString, " order by id ASC");
	
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(200);
	buf.Append(KSelectMessageId);
	buf.AppendNum(KUidMsvMessageEntryValue);
	buf.Append(KMessageMtmId);
	buf.AppendNum(aMtm);
	buf.Append(KSelectServiceId);
	buf.AppendNum(aServiceId);
	buf.Append(KMessageId);
	buf.AppendNum(lastId);
	buf.Append(KOrderByString);
	buf.Append(KDelimiter);
	
	RSqlStatement Idquerystatement;
	CleanupClosePushL(Idquerystatement);
	User::LeaveIfError(Idquerystatement.Prepare(iDatabase, buf));
	
	TInt Idindex = GetColumnIndexL(Idquerystatement, KMsgColumnId());
	
	while(KSqlAtRow == Idquerystatement.Next())
		{
		aIdArray.AppendL(ColumnInt(Idquerystatement, Idindex));
		}
		
	CleanupStack::PopAndDestroy(2);  //buf,Idquerystatement
	}

/**
GetAllMimeIdsL()
Gets all attachment ids for a message.

@param aId: id
@param aIdArray: Array of TMsvId.
@retun None
*/
void CMsvDBAdapter::GetAllMimeIdsL(TMsvId aId, RArray<TMsvId>& aIdArray)
	{
	_LIT(KGetChild, "Select id, type FROM IndexEntry WHERE parentId = ");
	
	const TInt32 KUidMsvEmailTextEntryValue = 0x100038F1;
	const TInt32 KUidMsvEmailHtmlEntryValue = 0x100057C0; // 268457920
	const TInt32 KUidMsvEmailRtfEntryValue = 0x1028368F; // 271070863

	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KGetChild);
	buf.AppendNum(aId);
	buf.Append(KDelimiter);
	
	RSqlStatement stmt;
	CleanupClosePushL(stmt);
	User::LeaveIfError(stmt.Prepare(iDatabase, buf));
	
	TInt idIndex = 0;
	TInt typeIndex = 1;
		
	// For every child..
	while(KSqlAtRow == stmt.Next())
		{
		// Get the type
		TMsvId childId = ColumnInt(stmt, idIndex);
		TInt32 type = ColumnInt(stmt, typeIndex);
		
		// Append TMsvId's which may have MIME header
		if( type == KUidMsvAttachmentEntryValue ||
			type == KUidMsvEmailTextEntryValue ||
			type == KUidMsvEmailHtmlEntryValue ||
			type == KUidMsvEmailRtfEntryValue
			)
			{
			aIdArray.AppendL(childId);
			}
		// Recursion for alternate folders
		else if(type == KUidMsvFolderEntryValue)
			{
			aIdArray.AppendL(childId);
			// also get the childs for these messages
			GetAllMimeIdsL(childId, aIdArray);
			}
		}
		
	CleanupStack::PopAndDestroy(2); // stmt, buf
	}

/**
GetMessageCountL()
Gets the count of messages under a service id.

@param aServiceId: service id
*/
TInt CMsvDBAdapter::GetMessageCountL(TMsvId aServiceId)
	{
	_LIT(KSelectMessageId, "Select count(*) FROM IndexEntry WHERE type = ");
	_LIT(KServiceId," AND serviceId = ");
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KSelectMessageId);
	buf.AppendNum(KUidMsvMessageEntryValue);
	buf.Append(KServiceId);
	buf.AppendNum(aServiceId);
	buf.Append(KDelimiter);
	
	TSqlScalarFullSelectQuery query(iDatabase);
	TInt msgcount = 0;
	msgcount = query.SelectIntL(buf);
	
	CleanupStack::PopAndDestroy();  //buf
	return msgcount;
	}

/**
GetOwningServiceL()
Gets the ids owning service.

@param aId: id
@param aOwningId: owning service id reference variable.
@retun None
*/
void CMsvDBAdapter::GetOwningServiceL(TMsvId aId, TMsvId& aOwningId)
	{
	_LIT(KSelectServiceId, "Select parentId,type FROM IndexEntry WHERE id = ");
	
	TBool found = EFalse;
	TMsvId entryid = aId;
	TMsvId parentId = 0;
	TMsvId typeId = 0;
	
	while (!found)
		{
		RBuf buf;
		CleanupClosePushL(buf);
		buf.CreateL(100);
		buf.Append(KSelectServiceId);
		buf.AppendNum(entryid);
		buf.Append(KDelimiter);
		
		RSqlStatement parentIdquerystatement;
		CleanupClosePushL(parentIdquerystatement);
		User::LeaveIfError(parentIdquerystatement.Prepare(iDatabase, buf));
		
		TInt parentIdindex = GetColumnIndexL(parentIdquerystatement, KMsgColumnParentId());
		TInt typeIdindex = GetColumnIndexL(parentIdquerystatement, KMsgColumnType());
	
		while(KSqlAtRow == parentIdquerystatement.Next())
			{
			parentId  = ColumnInt(parentIdquerystatement, parentIdindex);
			typeId = ColumnInt(parentIdquerystatement, typeIdindex);
			}
		
		CleanupStack::PopAndDestroy(2);  //buf,parentIdquerystatement
			
		if (typeId == KUidMsvServiceEntryValue)
			{
			found = ETrue;
			}
		else
			{
			entryid = parentId;
			}
		}
	aOwningId = entryid;
	}
	
/**
GetAllServiceIDsForMtmL()
Gets all service ids for the specified mtm.

@param aMtm: mtm id.
@param aIdArray&: Array of TMsvId.
@retun None
*/
void CMsvDBAdapter::GetAllServiceIDsForMtmL(TInt32 aMtm, RArray<TMsvId>& aIdArray)
	{
	// if sms, than dont get smtp id else, get the smtp id also
	_LIT16(KSelectServiceIds, "Select distinct serviceId FROM IndexEntry where type = ");
	_LIT16(KMtmIdQuery," AND mtmId = ");

	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(200);
	buf.Append(KSelectServiceIds);
	buf.AppendNum(KUidMsvServiceEntryValue);
	buf.Append(KMtmIdQuery);
	buf.AppendNum(aMtm);
	buf.Append(KDelimiter);
	
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, buf));
	TInt serviceColumnIndex = GetColumnIndexL(queryStmt, KMsgColumnServiceId());
	
	while(KSqlAtRow == queryStmt.Next())
		{
		TMsvId serviceId = ColumnInt(queryStmt, serviceColumnIndex);
		aIdArray.AppendL(serviceId);
		}
	CleanupStack::PopAndDestroy(2); //buf, stmt
	
	// lets also get all messages in the root with this service id
	if(aMtm == 268439592 ) //0x1000102C sms
		{
		aIdArray.AppendL(KMsvGlobalOutBoxIndexEntryIdValue);	
		aIdArray.AppendL(KMsvDraftEntryIdValue);
		aIdArray.AppendL(KMsvSentEntryIdValue);	
		}
	}

/**
GetAllIndexEntryTableIdsL()
Gets all ids present in index entry table.

@param aIdArray: Array of TMsvId.
@retun None
*/
void CMsvDBAdapter::GetAllIndexEntryTableIdsL(RArray<TMsvId>& aIdArray)	
	{
	_LIT(KSelectAllIds, "Select * FROM IndexEntry;");
	
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, KSelectAllIds));
	
	TInt serviceColumnIndex = GetColumnIndexL(queryStmt, KMsgColumnId());
	
	while(KSqlAtRow == queryStmt.Next())
		{
		TMsvId Id = ColumnInt(queryStmt, serviceColumnIndex);
		aIdArray.AppendL(Id);
		}
	CleanupStack::PopAndDestroy(); //queryStmt
	}

/**
GetVisibleParentL()
Gets the visible parent of the specified id.

@param aId: id
@param aVisibleParentId: visible parent id.
@retun None
*/
 
void CMsvDBAdapter::GetVisibleParentL(TMsvId aId, TMsvId& aVisibleParentId)
	{
	_LIT(KSelectServiceId, "Select parentId,type FROM IndexEntry WHERE id = ");
	
	TMsvId entryid = aId;
	TMsvId parentId = 0;
		
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KSelectServiceId);
	buf.AppendNum(entryid);
	buf.Append(KDelimiter);
	
	RSqlStatement parentIdquerystatement;
	CleanupClosePushL(parentIdquerystatement);
	
	User::LeaveIfError(parentIdquerystatement.Prepare(iDatabase, buf));
		
	TInt parentIdindex = GetColumnIndexL(parentIdquerystatement, KMsgColumnParentId());
		
	while(KSqlAtRow == parentIdquerystatement.Next())
		{
		parentId  = ColumnInt(parentIdquerystatement, parentIdindex);
		}
	
	CleanupStack::PopAndDestroy(2);  //buf,parentIdquerystatement
	
	GetVisibleParentIdL(parentId,aVisibleParentId);
	}

/**
GetVisibleParentIdL()
This functions returns the most immediate parent visible folder id.
There are two types of folders. A visible folder is one which is 
visible to the external user, but there are internal non-visible folders
created under each email message to organize the content of the email.
Parent entry of an entry is either root or a folder. A folder can be 
service folder (like LocalServices), userdefined folder/standard folder
like Inbox or a non-visible folder (created under an email message).

For a given parent Id, the function returns the most immediate visible
parent folder. The rules are as follows:
1. If the parent entry is root or children of root, then the visibleFolder 
will be root itself. This is because service folders are considered 
invisible, since they are not seen by user. 
2. If the parent entry itself is a visible folder then child's visibleFolder
id should be its parent id.
3. If the parent entry is a non-visible folder then child's visibleFolder
id is same as its parent's visibleFolder id.

@param TMsvId: Parent Id.
@return Most immediate visible folder id of any child entry present under passed parent id.
*/
void CMsvDBAdapter::GetVisibleParentIdL(TMsvId aParentId, TMsvId& aVisibleFolderId)
	{
	// If the entry is either a root entry (parent=KErrNotFound) or 
	// child of root entry its visibleFolderId will be root entry itself.
	if((KErrNotFound == aParentId) || (KMsvRootIndexEntryId == aParentId))
		{
		aVisibleFolderId = KMsvRootIndexEntryId;
		}
	else
		{
		// Get the visible flag of parent entry.
		
		// If parent entry is visible, then immediateVisibleFolder of child
		// should be parent Id. And if it is invisible then child's immediateVisibleFolder 
		// should be same as parent's immediateVisibleFolder.
		
		//Bind the msv id
		BindIntL(iStatement[EGetVisibleId], iIdIndex[EGetVisibleId], aParentId);

		// If parent entry does not exists leave with KErrNotFound.
		if (KSqlAtRow != iStatement[EGetVisibleId].Next())
			{
			iStatement[EGetVisibleId].Reset();
			User::Leave(KErrNotFound);
			}

		TInt32 data = ColumnInt(iStatement[EGetVisibleId], iDataIndex[EGetVisibleId]);
		if( data & TMsvEntry::KMsvVisibleFolderNodeFlag)
			{
			// If visible folder flag is not set for parent entry.
			aVisibleFolderId = ColumnInt(iStatement[EGetVisibleId], iVisibleParent[EGetVisibleId]);
			}
		else
			{
			// If parent entry is visible
			aVisibleFolderId = aParentId;
			}
		//Reset the RSqlStatement
		User::LeaveIfError(iStatement[EGetVisibleId].Reset());
		}
	}

/**
UpdateVisibleFolderIdL()
Updates visible parent and any other new changes in TMsvEntry structure post PREQ 1189. 

@param aId: id. 
@param visibleId: visible parent id.
@retun None
*/	
void CMsvDBAdapter::UpdateVisibleFolderIdL(TMsvId aId,TMsvId visibleId)
	{
	_LIT(KUpdateVisibleParentQuery,"UPDATE IndexEntry SET visibleParent = ");
	_LIT(KWhere, " WHERE ");
	_LIT(KUpdateVisibleParentQuery1, "id = ");
	 	
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KUpdateVisibleParentQuery);
	buf.AppendNum(visibleId);
	buf.Append(KWhere);
	buf.Append(KUpdateVisibleParentQuery1);
	buf.AppendNum(aId);
	buf.Append(KDelimiter);
	
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, buf));
	
	TInt err = queryStmt.Exec();
	
	CleanupStack::PopAndDestroy(2); //stmt
	
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	}

TBool CMsvDBAdapter::GetParentVisibleFlagL(TMsvId parentId)
	{
	_LIT(KSelectServiceId, "Select data FROM IndexEntry WHERE id = ");
	
	TInt data = KErrNotFound;		
	RBuf buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KSelectServiceId);
	buf.AppendNum(parentId);
	buf.Append(KDelimiter);
	
	RSqlStatement parentDataQuerystatement;
	CleanupClosePushL(parentDataQuerystatement);
	
	User::LeaveIfError(parentDataQuerystatement.Prepare(iDatabase, buf));
		
	TInt parentIdindex = GetColumnIndexL(parentDataQuerystatement, KMsgColumnData());
		
	while(KSqlAtRow == parentDataQuerystatement.Next())
		{
		data  = ColumnInt(parentDataQuerystatement, parentIdindex);
		}
	CleanupStack::PopAndDestroy(2);  //buf,parentIdquerystatement
	
	return !(data & TMsvEntry::KMsvVisibleFolderNodeFlag);
			
	}
	
/**
GetAllIndexEntryTableIdsL()
Gets all ids present in index entry table.

@param aIdArray: Array of TMsvId.
@retun None
*/
void CMsvDBAdapter::GetRemainingIdsL(RArray<TMsvId>& aIdArray)	
	{
	_LIT(KSelectAllIds, "Select id FROM IndexEntry where visibleParent = -99;");
	
	RSqlStatement queryStmt;
	CleanupClosePushL(queryStmt);
	User::LeaveIfError(queryStmt.Prepare(iDatabase, KSelectAllIds));
	
	TInt serviceColumnIndex = GetColumnIndexL(queryStmt, KMsgColumnId());
	
	while(KSqlAtRow == queryStmt.Next())
		{
		TMsvId Id = ColumnInt(queryStmt, serviceColumnIndex);
		aIdArray.AppendL(Id);
		}
	CleanupStack::PopAndDestroy(); //queryStmt
	}

/**
DeleteTable()
Deletes the header table identified by the UID

@param aMtmId: MTM id.
@retun None
*/
void CMsvDBAdapter::DeleteTableL(TUid aMtmId)	
	{
	_LIT16(KDropTableQuery, "DROP table if exists Header_");
	
	RBuf16 buf;
	CleanupClosePushL(buf);
	buf.CreateL(100);
	buf.Append(KDropTableQuery);
	buf.AppendNum(aMtmId.iUid);
	buf.Append(KDelimiter);
	
	TInt err = iDatabase.Exec(buf);

	CleanupStack::PopAndDestroy(); //buf
	}
 
 /**
DeleteTable()
Deletes the header table identified by the UID

@param None
@retun None
*/
void CMsvDBAdapter::DeleteSearchSortTable()	
	{
	_LIT16(KDropSearchSortTableQuery, "DROP table if exists SearchSortEntry;");
	
	TInt err = iDatabase.Exec(KDropSearchSortTableQuery);
	}
 
 
#endif //#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)



