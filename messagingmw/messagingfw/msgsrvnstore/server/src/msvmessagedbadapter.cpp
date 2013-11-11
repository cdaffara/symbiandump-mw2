// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors: Neeraj Nayan
//
// Description: The class provides interface to read/write header and
// body entry from the database.
//


/**
 * HEADER FILES
 */

#include "msvpreferreddrivelist.h"		// Preferred drive list.
#include "msvmessagedbadapter.h"		// Header file for this class.
#include "msvindexadapter.h"			// For GetDriveId()
#include "msvdbadapter.h"				// For BindL() and handle to CMsvDBAdapter while constructing


/**
 * LITERAL DEFINITION
 */
const TInt KMaxQueryLength = 1000;

// Macros to wrap DB error. KErrGeneral is thrown for
// all DB error. Make sure you declare 'err' before 
// using these macros.
#define MSG_WRAP_DB_LEAVE(x) TRAP(err, x); \
							 if(err<=KSqlErrGeneral) User::Leave(KErrGeneral); \
							 User::LeaveIfError(err);							 

#define MSG_WRAP_DB_ERROR(errTxt) err = errTxt; \
							if(err<=KSqlErrGeneral) User::Leave(KErrGeneral); \
							else User::LeaveIfError(err);



/**
 * MEMBER FUNCTIONS OF CMsvMessageDBAdapter
 */



/**
 * CMsvMessageDBAdapter()
 * The default Constructor. 
 */
CMsvMessageDBAdapter::CMsvMessageDBAdapter(CMsvDBAdapter* aDBAdapter):
		iDBAdapter(aDBAdapter), iDatabaseRef(aDBAdapter->iDatabase)
	{
	}




/**
 * ~CMsvMessageDBAdapter()
 * The default Destructor. 
 */
CMsvMessageDBAdapter::~CMsvMessageDBAdapter()
	{
	iHeaderDataList.ResetAndDestroy();
	}




/**
 * NewL()
 * 
 * @param CMsvDBAdapter* : A reference to database adapter class.
 * @return CMsvMessageDBAdapter* : A reference to CMsvMessageDBAdapter
 * class.
 * 
 * Allocates and constructs a new CMsvMessageDBAdapter object.
 */
CMsvMessageDBAdapter* CMsvMessageDBAdapter::NewL(CMsvDBAdapter* aDBAdapter,TBool aDbInCurrentDrive)
	{
	//Create the DBAdapter object.
	CMsvMessageDBAdapter* self = new(ELeave) CMsvMessageDBAdapter(aDBAdapter);
	CleanupStack::PushL(self);
	
	self->ConstructL(aDbInCurrentDrive);
	
	CleanupStack::Pop();
	return self;
	}


/*
 * ConstructL()
 * 
 * The function populates metadata structure. A metadata structure
 * contains information about metadata table which is later used to 
 * create and update header entries. The structure is updated whenever
 * a new header table is created in the database.
 */ 
void CMsvMessageDBAdapter::ConstructL(TBool aDbInCurrentDrive)
	{
	// check if the operation are required for only single drive
	// This is required for message store converter as it works only on 1 drive.
	
	if(aDbInCurrentDrive)
		{
		iNonAttachedDrive = ETrue;
		UpdateMetadataStructureL(KCurrentDriveId);
		}
	else
		{
		iNonAttachedDrive = EFalse;
		for(TInt index=1; index<KMaxNumberOfDrives; index++)
			{
			if((*(iDBAdapter->iDatabasePresent))[index])
				{
				UpdateMetadataStructureL(index);
				}
			}
		}
	}
	
	
	
	

void CMsvMessageDBAdapter::UpdateMetadataStructureL(TUint aDriveId)
	{
	TInt err;
	_LIT(KCheckHeaderTableQuery0, "SELECT tbl_name FROM DB");
	_LIT(KCheckHeaderTableQuery1, ".sqlite_master WHERE type = 'table' AND tbl_name LIKE 'Header_%';");
	
	// 1. Get all the header table names from the SQLITE_MASTER table.
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);

	// Create the query string.
	if(!iNonAttachedDrive)
		{
		headerTableQuery.Append(KCheckHeaderTableQuery0);
		headerTableQuery.AppendNum(aDriveId);
		headerTableQuery.Append(KCheckHeaderTableQuery1);
		}
	else
		{
		_LIT(KCheckHeaderTableQuery2, "SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name LIKE 'Header_%';");
		headerTableQuery.Append(KCheckHeaderTableQuery2);
		}

	//Prepare a statement for the query.
	RSqlStatement sqlStmt;
	RPointerArray<HBufC> headerTableNames;
	CleanupClosePushL(sqlStmt);
	CleanupClosePushL(headerTableNames);

	MSG_WRAP_DB_ERROR(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));
	// Prepare a list of header table names.
	HBufC* tableName = NULL;
	while((err = sqlStmt.Next()) == KSqlAtRow)
		{
		MSG_WRAP_DB_LEAVE(tableName = ColumnTextL(sqlStmt, 0));
		CleanupStack::PushL(tableName);
		
		headerTableNames.AppendL(tableName);
		CleanupStack::Pop();
		}
	MSG_WRAP_DB_ERROR(err);

	// 2. For each tablename read the table structure from the DB.
	_LIT(KGetHeaderSchema1, "SELECT * FROM DB");
	_LIT(KGetHeaderSchema2, ".");
	_LIT(KGetHeaderSchema3, ";");
	_LIT(KGetHeaderSchema4, "SELECT * FROM ");
	
	while(headerTableNames.Count())
		{
		HBufC* tableName = headerTableNames[0];
		
		TLex mtmId(tableName->Mid(tableName->Locate('_')+1));
		TUid uid;
		mtmId.Val(uid.iUid);
		
		// Don't insert meta-data if we already have it for this mtmId.
		if(GetMetadataEntryIndex(uid) != KErrNotFound)
			{
			delete tableName;
			headerTableNames.Remove(0);
			continue;
			}
		
		headerTableQuery.Zero();
		sqlStmt.Close();

		if(iNonAttachedDrive) // this is for msg converter. it refers to  a single drive
			{
			headerTableQuery.Append(KGetHeaderSchema4);
			}
		else
			{
			// This creates following string...
			// "SELECT * FROM DB<driveId>.HEADER_<MtmId>;" 
	
			headerTableQuery.Append(KGetHeaderSchema1);
			headerTableQuery.AppendNum(aDriveId);
			headerTableQuery.Append(KGetHeaderSchema2);
			}
		
		headerTableQuery.Append(tableName->Des());
		headerTableQuery.Append(KGetHeaderSchema3);

		MSG_WRAP_DB_ERROR(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));

		CHeaderMetaData* headerMetaData = new(ELeave) CHeaderMetaData();
		CleanupStack::PushL(headerMetaData);
			
		TInt metadataIndex = 0;
		headerMetaData->iMtmId.iUid = uid.iUid;
		
		TInt colCnt = sqlStmt.ColumnCount();
		// Record the header structure, ignore first two fields (Id, UID).
		TRAPD(err, 
			for(TInt index=2; index<colCnt; index++)
				{			
				CFieldClass* fieldObj = new(ELeave) CFieldClass();
				CleanupStack::PushL(fieldObj);
				
				User::LeaveIfError(sqlStmt.DeclaredColumnType(index, fieldObj->iType));
				TPtrC fieldName;
				User::LeaveIfError(sqlStmt.ColumnName(index, fieldName));
				fieldObj->iName = fieldName.AllocL();
				headerMetaData->iFieldList.AppendL(fieldObj);
				CheckStdColumnsAdded(fieldObj->iName, headerMetaData->iStdColumnStatus);
				
				CleanupStack::Pop(fieldObj);
				}
			);
		if(err)
			{			
			MSG_WRAP_DB_ERROR(err);
			}
		iHeaderDataList.InsertL(headerMetaData, metadataIndex);
		CleanupStack::Pop();			// headerMetaData

		delete tableName;
		headerTableNames.Remove(0);
		}		// while(headerTableNames.Count())

	CleanupStack::PopAndDestroy(3); // headerTableQuery, sqlStmt, headerTableNames
	}




/*
 * IsHeaderTableExistsL()
 *
 * @param aMtmId: MTM ID of the header table.
 * @return TBool: ETrue/EFalse
 * 
 * The function returns ETrue if the header table for 
 * passed MTM ID already exists in the database. The 
 * function works for only message server current drive.
 */
TBool CMsvMessageDBAdapter::IsHeaderTableExistsL(const TUid& aMtmId)
	{
	_LIT16(KCheckHeaderTableQuery1, "SELECT COUNT(*) FROM DB");
	_LIT16(KCheckHeaderTableQuery2, ".SQLITE_MASTER WHERE NAME LIKE 'Header_");
	_LIT16(KCheckHeaderTableQuery3, "';");
	
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);

	// Create the query string.
	// Ex: SELECT COUNT(*) FROM DB<drive-id>.SQLITE_MASTER WHERE 
	// NAME LIKE 'Header_<mtm-id>';
	headerTableQuery.Append(KCheckHeaderTableQuery1);
	headerTableQuery.AppendNum(KCurrentDriveId);
	headerTableQuery.Append(KCheckHeaderTableQuery2);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KCheckHeaderTableQuery3);

	TInt count = 0;
	TSqlScalarFullSelectQuery query(iDatabaseRef);
	TRAPD(err, count = query.SelectIntL(headerTableQuery));
	CleanupStack::PopAndDestroy();			// headerTableQuery
	
	// If the table exists, count will have value 1.
	if(err == KErrNone && count == 1)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}






/*
 * CreateHeaderTableL()
 *
 * @param aMtmId: MTM ID of the table.
 * @param aFieldDetails: Table structure details.
 * @return aErrorMessage: Error message text, in case
 * header table creation fails.
 *
 * The function creates separate header table for each MTM type. 
 * If the table already exist, it simply exits without returning an error.
 */
void CMsvMessageDBAdapter::CreateHeaderTableL(const TUid& aMtmId, const RPointerArray<CFieldPair>& aFieldDetails, TPtrC& aErrorMessage)
	{
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);	
	
	CHeaderMetaData* headerMetaData = new(ELeave) CHeaderMetaData();
	CleanupStack::PushL(headerMetaData);
		
	TInt metadataIndex = GetMetadataEntryIndex(aMtmId);
	// Insert a new metadata entry if it doesn't already exist.
	if(metadataIndex == KErrNotFound)
		{
		iHeaderDataList.InsertL(headerMetaData, 0);
		}
	headerMetaData->iMtmId = aMtmId;
	
	if(iNonAttachedDrive)
		{
		_LIT16(KCreateHeaderTableQuery1, "CREATE TABLE IF NOT EXISTS ");
		_LIT16(KCreateHeaderTableQuery2, "Header_");
		
		// This creates following string...
		// "CREATE TABLE IF NOT EXISTS DB<DriveId>.Header_<MtmId> ( id INT, uid INT," 
		headerTableQuery.Append(KCreateHeaderTableQuery1);
		headerTableQuery.Append(KCreateHeaderTableQuery2);
		}
	else
		{
		_LIT16(KCreateHeaderTableQuery1, "CREATE TABLE IF NOT EXISTS DB");
		_LIT16(KCreateHeaderTableQuery2, ".Header_");
		// "CREATE TABLE IF NOT EXISTS DB<DriveId>.Header_<MtmId> ( id INT, uid INT," 
		headerTableQuery.Append(KCreateHeaderTableQuery1);
		headerTableQuery.AppendNum(KCurrentDriveId);
		headerTableQuery.Append(KCreateHeaderTableQuery2);
		}
	_LIT16(KCreateHeaderTableQuery3, " (id INT, uid INT, ");
	_LIT16(KCreateHeaderTableQuery4, "Details TEXT, PRIMARY KEY (id, uid));");	
	aErrorMessage.Set(_L(""));
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KCreateHeaderTableQuery3);
	
	// Add the column details to the query.
	TRAPD(err, DoCreateHeaderTableCreationQueryL(aFieldDetails, headerTableQuery, headerMetaData));
	if(err)
		{
		CleanupStack::Pop(headerMetaData);
		delete headerMetaData;
		iHeaderDataList.Remove(0);
		User::Leave(err);
		}
	headerTableQuery.Append(KCreateHeaderTableQuery4);
	
	// Execute the query.
	// Ignore error if table already exist.
	err = iDatabaseRef.Exec(headerTableQuery);
	if(err < 0)
		{
		aErrorMessage.Set(iDatabaseRef.LastErrorMessage());

		CleanupStack::Pop(headerMetaData);
		delete headerMetaData;
		iHeaderDataList.Remove(0);
		MSG_WRAP_DB_ERROR(err);
		}

	CleanupStack::Pop(headerMetaData);					// headerMetaData
	if(metadataIndex != KErrNotFound)
		delete headerMetaData;
	CleanupStack::PopAndDestroy();						// headerTableQuery
	}







/*
 * DoCreateHeaderTableCreationQueryL()
 *
 * @param aFieldDetails: Table structure details.
 * @return aHeaderTableQuery: Header-table creation query string.
 * @return aHeaderMetaData: Header metadata structure.
 *
 * The function appends the column details to the header
 * creation query string. It also updates the metadata structure
 * for the header entry.
 */
void CMsvMessageDBAdapter::DoCreateHeaderTableCreationQueryL(const RPointerArray<CFieldPair>& aFieldDetails, RBuf& aHeaderTableQuery, CHeaderMetaData*& aHeaderMetaData)
	{
	_LIT16(KIntField, " INT, ");
	_LIT16(KTextField, " TEXT, ");
	_LIT16(KDateField, " INT64, ");
	_LIT16(KQuote, "\"");
	
	CFieldClass *fieldObj = NULL;
	// Browse through each column names in the field-list.
	TInt fieldDetailsCount = aFieldDetails.Count();
	for(TInt fieldIndex=0; fieldIndex<fieldDetailsCount; fieldIndex++)
		{			
		if(!aFieldDetails[fieldIndex]->iFieldName)
			{
			User::Leave(KErrArgument);
			}
		
		// Create a metadata entry.
		fieldObj = new(ELeave) CFieldClass;
		CleanupStack::PushL(fieldObj);
		fieldObj->iName = aFieldDetails[fieldIndex]->iFieldName->Des().AllocL();
		
		// Append column name under quotes.
		aHeaderTableQuery.Append(KQuote);
		aHeaderTableQuery.Append(aFieldDetails[fieldIndex]->iFieldName->Des());
		aHeaderTableQuery.Append(KQuote);
		
		// Append appropriate column type.
		switch(aFieldDetails[fieldIndex]->iFieldType)
			{
			case EIntegerField:
				aHeaderTableQuery.Append(KIntField);
				fieldObj->iType = ESqlInt;
				break;
			case ETextField:
				aHeaderTableQuery.Append(KTextField);
				fieldObj->iType = ESqlText;
				break;
			case EDateField:
				aHeaderTableQuery.Append(KDateField);
				fieldObj->iType = ESqlInt64;
				break;
			default:
				User::Leave(KErrArgument);
				break;
			}
		
		aHeaderMetaData->iFieldList.AppendL(fieldObj);
		CleanupStack::Pop();			// fieldObj
		
		// Update the status of standard header columns 
		// in the metadata structure.
		CheckStdColumnsAdded(aFieldDetails[fieldIndex]->iFieldName, aHeaderMetaData->iStdColumnStatus);
		}		// for(TInt fieldIndex=0; fieldIndex<aFieldDetails.Count(); fieldIndex++)


	// Adding the 'Details' column to the query string.
	_LIT(KDetails, "Details");
	fieldObj = new(ELeave) CFieldClass;
	CleanupStack::PushL(fieldObj);

	fieldObj->iType = ESqlText;
	fieldObj->iName = KDetails().AllocL();
	aHeaderMetaData->iFieldList.AppendL(fieldObj);		// Added for Details column.

	CleanupStack::Pop();								// fieldObj
	}
	
	
	
	
	

/*
 * CreateHeaderEntryL()
 *
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Metadata Entry Id.
 * @param aFieldPairList: Header details.
 *
 * The function creates a new header entry in the header
 * table. The header details passed to the function in 
 * aFieldPairList should either have values for all the
 * columns in the header table or should have value for
 * only one column (The default column "Details").
 *
 * The function can accept multiple header portion (each 
 * associated with a unique UID. These header portion will 
 * be stored in a separate row in the header table.
 */

void CMsvMessageDBAdapter::CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TInt err = KErrNone;
	
	// Get the metadata entry index for the MTM.
	TInt metadataEntryIndex = GetMetadataEntryIndex(aMtmId);
	User::LeaveIfError(metadataEntryIndex);
	TBool isLocalTransaction = EFalse;
	
	// Open the DB transaction, if it is not already open.
	if(!iDBAdapter->isTransactionOpen && !iNonAttachedDrive)
		{
		MSG_WRAP_DB_LEAVE(iDBAdapter->BeginTransactionL());
		isLocalTransaction = ETrue;
		}
	
	// For each entry in header details, create a separate
	// row in the header table.
	TInt fieldPairListCount = aFieldPairList.Count();
	TRAP(err, 
		for(TInt index=0; index<fieldPairListCount; index++)
			{
			CHeaderFields* headerFields = aFieldPairList[index];
			DoInsertHeaderEntryL(aMtmId, aEntryId, headerFields, metadataEntryIndex);
			}		
		);

	// Handle error and close the transaction.
	if(err && !iNonAttachedDrive)
		{
		if(isLocalTransaction)
			{
			iDBAdapter->RollbackTransactionL();
			}
		MSG_WRAP_DB_ERROR(err);
		}
	else if (!iNonAttachedDrive)
		{
		if(isLocalTransaction)
			{
			MSG_WRAP_DB_LEAVE(iDBAdapter->CommitTransactionL());
			}
		}
	}







/*
 * DoInsertHeaderEntryL()
 *
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Metadata Entry Id.
 * @param aHeaderFields: Header entry details.
 * @param aMetadataEntryIndex: Header table metadata.
 *
 * The function creates a new header entry in the header
 * table. The header details passed to the function in 
 * aHeaderFields should either have values for all the
 * columns in the header table or should have value for
 * only one column (The default column "Details").
 */
void CMsvMessageDBAdapter::DoInsertHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, CHeaderFields* aHeaderFields, TInt aMetadataEntryIndex)
	{
	TInt err = KErrNone;
	_LIT16(KGetHeaderSchema5, "INSERT INTO HEADER_");
	_LIT16(KGetHeaderSchema1, "INSERT INTO DB");
	_LIT16(KGetHeaderSchema2, ".HEADER_");
	_LIT16(KGetHeaderSchema3, " VALUES (:id, :uid");
	_LIT16(KGetHeaderSchema4, " (id, uid, Details) VALUES (:id, :uid, :details);");
	_LIT16(KComma, ", :");
	_LIT16(KDelimiter, ");");
		
	TInt KDefaultColumnSize = 1;
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	
	// Create header entry creation query
	// Ex: INSERT INTO DB<driveId>.HEADER_<mtmId>
	headerTableQuery.CreateL(KMaxQueryLength);
	if(!iNonAttachedDrive)
		{
		headerTableQuery.Append(KGetHeaderSchema1);
		headerTableQuery.AppendNum(GetDriveId(aEntryId));
		headerTableQuery.Append(KGetHeaderSchema2);		
		}
	else
		{
		headerTableQuery.Append(KGetHeaderSchema5);
		}
	headerTableQuery.AppendNum(aMtmId.iUid);
	
	// Append column names to the query string.
	// Column names are stored in the header metadata structure.
	CHeaderMetaData* headerMetadata = iHeaderDataList[aMetadataEntryIndex];	
	TInt fieldPairListCount = aHeaderFields->iFieldPairList.Count();

	// If header entry has more than one field...
	if(KDefaultColumnSize < fieldPairListCount)
		{
		headerTableQuery.Append(KGetHeaderSchema3);		
		
		// Check if number of values passed for the header entry
		// matches with the number of columns in the header table.
		RPointerArray<CFieldClass>& fieldList = headerMetadata->iFieldList;		
		if(fieldList.Count() != fieldPairListCount)
			{
			User::Leave(KErrArgument);
			}
		// Append column names to the query string.
		TInt fieldListCount = fieldList.Count();
		for(TInt fieldCount=0; fieldCount<fieldListCount; fieldCount++)
			{
			headerTableQuery.Append(KComma);
			headerTableQuery.Append(fieldList[fieldCount]->iName->Des());		
			}
		headerTableQuery.Append(KDelimiter);
		}		// if(KDefaultColumnSize < aHeaderFields->iFieldPairList.Count())
	else
		{
		// If the header entry has just one field
		// It must be the value for default column (Details).
		headerTableQuery.Append(KGetHeaderSchema4);
		}
	
	// Prepare the SQL statement for execution.
	RSqlStatement sqlStmt;
	CleanupClosePushL(sqlStmt);
	MSG_WRAP_DB_ERROR(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));

	// Bind values against column names.
	TInt fieldIndex = 0;
	MSG_WRAP_DB_LEAVE(BindIntL(sqlStmt, fieldIndex++, UnmaskTMsvId(aEntryId)));
	MSG_WRAP_DB_LEAVE(BindIntL(sqlStmt, fieldIndex++, aHeaderFields->iUid.iUid));
	
	TInt index = 0;
	for(; index < fieldPairListCount-1; index++, fieldIndex++)
		{
		switch(headerMetadata->iFieldList[index]->iType)
			{
			case ESqlInt:
				BindIntL(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[index]->iFieldNumValue);
				break;
			case ESqlInt64:
				BindInt64L(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[index]->iFieldNumValue);
				break;
			case ESqlText:
				if(!aHeaderFields->iFieldPairList[index]->iFieldTextValue)
					{
					User::Leave(KErrArgument);
					}
				BindTextL(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[index]->iFieldTextValue->Des());
				break;
			}
		}
	if(aHeaderFields->iFieldPairList[index]->iFieldTextValue)
		{
		BindTextL(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[index]->iFieldTextValue->Des());
		}
	else
		{
		User::Leave(KErrArgument);
		}
		
	// Execute the query. Return KErrGeneral,
	// if entry is not inserted.	
	err = sqlStmt.Exec();
	if(!err)
		{
		User::Leave(KErrGeneral);
		}
	if(err == KSqlErrConstraint)
		{
		User::Leave(KErrAlreadyExists);
		}
	MSG_WRAP_DB_ERROR(err);
	CleanupStack::PopAndDestroy(2); //sqlStmt, headerTableQuery	
	}








/*
 * LoadHeaderEntryL()
 * 
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Metadata Entry Id.
 * @return aFieldPairList: Header entry details.
 *
 * The function loads the header entry from the header
 * table. If the header entry has multiple row, all of 
 * them will be loaded and stored as a separate element 
 * of CHeaderFields.
 */	
void CMsvMessageDBAdapter::LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TInt err = KErrNone;
	_LIT16(KGetHeaderSchema1, "SELECT uid");
	_LIT16(KGetHeaderSchema2, " FROM DB");
	_LIT16(KGetHeaderSchema3, ".HEADER_");
	_LIT16(KGetHeaderSchema4, " WHERE id = :id;");
	_LIT16(KComma, ", \"");
	_LIT16(KQuote, "\"");
	
	aFieldPairList.Reset();
	// Get the metadata entry index for the MTM.
	TInt metadataEntryIndex = GetMetadataEntryIndex(aMtmId);
	User::LeaveIfError(metadataEntryIndex);
	RPointerArray<CFieldClass>&	fieldList = iHeaderDataList[metadataEntryIndex]->iFieldList;
	
	// Create header query string
	// Ex: SELECT uid, 
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);
	headerTableQuery.Append(KGetHeaderSchema1);
	
	// Append header column name to the query.
	TInt fieldListCount = fieldList.Count();
	for(TInt index=0; index<fieldListCount; index++)
		{
		headerTableQuery.Append(KComma);
		headerTableQuery.Append(fieldList[index]->iName->Des());		
		headerTableQuery.Append(KQuote);
		}
	
	// Append "FROM DB<driveId>.HEADER_<mtmId> WHERE ID = :ID;
	headerTableQuery.Append(KGetHeaderSchema2);
	headerTableQuery.AppendNum(GetDriveId(aEntryId));
	headerTableQuery.Append(KGetHeaderSchema3);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KGetHeaderSchema4);
	
	//Prepare the SQL statement.
	RSqlStatement sqlStmt;
	CleanupClosePushL(sqlStmt);

	MSG_WRAP_DB_ERROR(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));
	MSG_WRAP_DB_LEAVE(BindIntL(sqlStmt, 0, UnmaskTMsvId(aEntryId)));
	
	// Load the header entry into header structure.
	TRAP(err, DoLoadHeaderEntryL(sqlStmt, fieldList, aFieldPairList));
	
	// Handle errors if any.
	if(err<0)
		{
		for(TInt index=0; index<aFieldPairList.Count(); index++)
			{
			CHeaderFields* headerRow = aFieldPairList[0];
			aFieldPairList.Remove(0);
			delete headerRow;
			}
		MSG_WRAP_DB_ERROR(err);
		}
	
	if(!aFieldPairList.Count())
		{
		User::Leave(KErrNotFound);
		}
	CleanupStack::PopAndDestroy(2); //sqlStmt, headerTableQuery
	}	
	
	
	
	
	
/*
 * DoLoadHeaderEntryL()
 * 
 * @param aSqlStmt: Query statement.
 * @param aMetadataList: Metadata Entry for header table.
 * @return aFieldPairList: Header entry details.
 *
 * The function loads the header entry from the header
 * table. If the header entry has multiple row, all of 
 * them will be loaded and stored as a separate element 
 * of CHeaderFields.
 */	
void CMsvMessageDBAdapter::DoLoadHeaderEntryL(RSqlStatement& aSqlStmt, const RPointerArray<CFieldClass>& aMetadataList, RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TInt err = KErrNone;
	// Fetch each header-table row in the loop.
	while(ETrue)
		{
		err = aSqlStmt.Next();
		if(KSqlAtRow != err)
			{
			if(KSqlAtEnd == err)
				{
				break;
				}
			else
				{
				MSG_WRAP_DB_ERROR(err);
				}		
			}

		// Create header structure to store a single row.
		CHeaderFields* headerRow = new(ELeave) CHeaderFields();
		CleanupStack::PushL(headerRow);
		
		TBool isPrimaryRow = EFalse;
		TInt count = aMetadataList.Count();
		headerRow->iUid.iUid = ColumnInt(aSqlStmt, 0);
		// Read individual fields and populate the header structure.
		for(TInt index=0; index<count; index++)
			{
			CFieldPair* fieldObj = new(ELeave) CFieldPair();
			CleanupStack::PushL(fieldObj);
			
			fieldObj->iFieldName = aMetadataList[index]->iName->Des().AllocL();
			switch(aMetadataList[index]->iType)
				{
				case ESqlInt:
					fieldObj->iFieldNumValue = ColumnInt(aSqlStmt, index+1);
					if(!isPrimaryRow && fieldObj->iFieldNumValue != NULL && index != count-1)
						{
						isPrimaryRow = ETrue;
						}
					break;
				case ESqlInt64:
					fieldObj->iFieldNumValue = ColumnInt64(aSqlStmt, index+1);
					if(!isPrimaryRow && fieldObj->iFieldNumValue != NULL && index != count-1)
						{
						isPrimaryRow = ETrue;
						}
					break;
				case ESqlText:
					TRAP(err, fieldObj->iFieldTextValue = ColumnTextL(aSqlStmt, index+1));
					if(err < 0)
						{
						User::Leave(KErrGeneral);
						}
					if(!isPrimaryRow && (index != count-1) && fieldObj->iFieldTextValue->Des().Compare(_L("")) )
						{
						isPrimaryRow = ETrue;
						}
					break;
				}		// switch(fieldList[index]->iType)

			headerRow->iFieldPairList.AppendL(fieldObj);
			CleanupStack::Pop(fieldObj);
			}		// for(TInt index=0; index<fieldList.Count(); index++)
		
		// If it is not a primary entry, remove extra fields
		// from the header row. 
		// A header table can store two types of entry, primary 
		// entry, one which has values in all the columns in the 
		// table, and another which has values in only default columns.
		if(!isPrimaryRow)
			{
			for(TInt index=0; index<count-1; index++)
				{
				delete headerRow->iFieldPairList[0];
				headerRow->iFieldPairList.Remove(0);
				}
			}
		aFieldPairList.AppendL(headerRow);
		CleanupStack::Pop(headerRow);
		}
	}
	
	
	
	



/*
 * UpdateHeaderEntryL()
 * 
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Entry Id.
 * @param aFieldPairList: Header entry details.
 *
 * The function updates the header entry in the header table with the 
 * passed header structure. A header entry is essentially a list of 
 * UID-String combination which is stored as a separate element in the 
 * passed header structure. Each such header portion is stored in separate
 * rows in the header table. 
 
 * If the number of rows in the passed header structure is more than what
 * is present in the DB, the function inserts the new rows in the header
 * table. And if the few rows are missing from the passed header structure
 * the function deletes the extra row from the header table.
 */ 
void CMsvMessageDBAdapter::UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TInt err = KErrNone;
	_LIT16(KGetHeaderSchema1, "SELECT uid FROM DB");
	_LIT16(KGetHeaderSchema2, ".HEADER_");
	_LIT16(KGetHeaderSchema3, " WHERE id = :id;");

	// First find out the number of rows present in the DB
	// against this entry, and store their UID in a array.
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);
		
	// This creates following string...
	// Ex: SELECT uid FROM DB<driveId>.HEADER_<mtmId> WHERE id = :id;
	headerTableQuery.Append(KGetHeaderSchema1);
	headerTableQuery.AppendNum(GetDriveId(aEntryId));
	headerTableQuery.Append(KGetHeaderSchema2);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KGetHeaderSchema3);

	//Prepare a statement for the query.
	RSqlStatement sqlStmt;
	CleanupClosePushL(sqlStmt);
	
	MSG_WRAP_DB_ERROR(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));
	MSG_WRAP_DB_LEAVE(BindIntL(sqlStmt, 0, UnmaskTMsvId(aEntryId)));
	
	// Execute the query and form the UID list.
	RArray<TInt> uidList;
	CleanupClosePushL(uidList);
	do
		{
		err = sqlStmt.Next();
		if(err == KSqlAtRow)
			{
			uidList.AppendL(ColumnInt(sqlStmt, 0));	
			}		
		}
	while(err == KSqlAtRow);
	
	// If no entry is found in DB, return KErrNotFound.
	MSG_WRAP_DB_ERROR(err);
	if(!uidList.Count())
		{
		User::Leave(KErrNotFound);
		}

	// Open a DB transaction if it is not already open.
	TBool isLocalTransaction = EFalse;
	if(!iDBAdapter->isTransactionOpen)
		{
		MSG_WRAP_DB_LEAVE(iDBAdapter->BeginTransactionL());
		isLocalTransaction = ETrue;
		}
	
	// Process all the header portion one by one.
	TRAP(err, DoProcessHeaderEntryL(aMtmId, aEntryId, aFieldPairList, uidList));
	
	// Close the transaction and handle error.
	if(err)
		{
		if(isLocalTransaction)
			{
			iDBAdapter->RollbackTransactionL();
			}
		MSG_WRAP_DB_ERROR(err);
		}
	else
		{
		if(isLocalTransaction)
			{
			MSG_WRAP_DB_LEAVE(iDBAdapter->CommitTransactionL());
			}
		}

	CleanupStack::PopAndDestroy(3); //uidList, sqlStmt, headerTableQuery
	}






/*
 * DoProcessHeaderEntryL()
 * 
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Entry Id.
 * @param aFieldPairList: Header entry details.
 * @param aUidList: UID list of the header entry present in DB.
 *
 * The function is called by UpdateHeaderEntryL() only, and it updates
 * the header entry in the header table. For details, see the comment 
 * of UpdateHeaderEntryL().
 */ 
void CMsvMessageDBAdapter::DoProcessHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList, RArray<TInt>& aUidList)
	{
	TInt metadataEntryIndex = GetMetadataEntryIndex(aMtmId);
	User::LeaveIfError(metadataEntryIndex);
	
	// Processing each header row.
	TInt fieldPairListCount = aFieldPairList.Count();
	for(TInt index=0; index < fieldPairListCount; index++)
		{
		// Search the entry UID in the UID list.
		CHeaderFields* headerRow = aFieldPairList[index];
		TInt rowIndex = aUidList.Find(headerRow->iUid.iUid);
		// If the entry is not present in DB, create it.
		if(KErrNotFound == rowIndex)
			{
			// Insert the entry
			DoInsertHeaderEntryL(aMtmId, aEntryId, headerRow, metadataEntryIndex);
			}
		else
			{
			// else, update the entry
			DoUpdateHeaderEntryL(aMtmId, aEntryId, headerRow, metadataEntryIndex);
			aUidList.Remove(rowIndex);
			}
		}
	
	// If there is extra entries in the DB, 
	// delete them.
	if(aUidList.Count())
		{
		_LIT16(KGetHeaderSchema1, "DELETE FROM DB");
		_LIT16(KGetHeaderSchema2, ".HEADER_");
		_LIT16(KGetHeaderSchema3, " WHERE id = :id and uid in (");
		_LIT16(KGetHeaderSchema4, ");");
		_LIT16(KComma, ", ");

		// Create the delete query string.
		RBuf headerTableQuery;
		CleanupClosePushL(headerTableQuery);
		headerTableQuery.CreateL(KMaxQueryLength);
			
		// This creates following string...
		headerTableQuery.Append(KGetHeaderSchema1);
		headerTableQuery.AppendNum(GetDriveId(aEntryId));
		headerTableQuery.Append(KGetHeaderSchema2);
		headerTableQuery.AppendNum(aMtmId.iUid);
		headerTableQuery.Append(KGetHeaderSchema3);
		headerTableQuery.AppendNum(aUidList[0]);
		TInt uidListCount = aUidList.Count();
		for(TInt index=1; index < uidListCount; index++)
			{
			headerTableQuery.Append(KComma);
			headerTableQuery.AppendNum(aUidList[index]);
			}
		headerTableQuery.Append(KGetHeaderSchema4);
		
		//Prepare a statement for the query.
		RSqlStatement sqlStmt;
		CleanupClosePushL(sqlStmt);
		User::LeaveIfError(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));
		BindIntL(sqlStmt, 0, UnmaskTMsvId(aEntryId));
		
		// Execute the query and handle error.
		TInt err = sqlStmt.Exec();
		if(!err)
			{
			User::Leave(KErrNotFound);
			}

		User::LeaveIfError(err);
		CleanupStack::PopAndDestroy(2); //sqlStmt, headerTableQuery	
		}
	}






/*
 * DoUpdateHeaderEntryL()
 * 
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Entry Id.
 * @param aHeaderFields: Header entry details.
 * @param aMetadataEntryIndex: UID list of the header entry present in DB.
 *
 * The function is called by UpdateHeaderEntryL() only, and it updates
 * the header entry in the header table. For details, see the comment 
 * of UpdateHeaderEntryL().
 */ 
void CMsvMessageDBAdapter::DoUpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, CHeaderFields* aHeaderFields, TInt aMetadataEntryIndex)
	{
	_LIT16(KGetHeaderSchema1, "UPDATE DB");
	_LIT16(KGetHeaderSchema2, ".HEADER_");
	_LIT16(KGetHeaderSchema3, " SET ");
	_LIT16(KGetHeaderSchema4, " WHERE id = :id and uid = :uid;");
	_LIT16(KGetHeaderSchema5, " details = :details WHERE id = :id and uid = :uid;");
	_LIT16(KComma, ", ");
	_LIT16(KColon, "\" = :");
	_LIT16(KQuote, "\"");
		
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	
	// Create the query string:
	// Ex: UPDATE DB<driveId>.HEADER_<mtmId> SET
	headerTableQuery.CreateL(KMaxQueryLength);
	headerTableQuery.Append(KGetHeaderSchema1);
	headerTableQuery.AppendNum(GetDriveId(aEntryId));
	headerTableQuery.Append(KGetHeaderSchema2);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KGetHeaderSchema3);
	
	TInt columnIndex = -1;
	// Append column information to the query
	CHeaderMetaData* headerMetadata = iHeaderDataList[aMetadataEntryIndex];
	TInt fieldPairListCount = aHeaderFields->iFieldPairList.Count();
	if(fieldPairListCount > 1)
		{
		// Check if all the field values are passed.
		if(fieldPairListCount != headerMetadata->iFieldList.Count())
			{
			User::Leave(KErrArgument);
			}
		for(TInt index=0; index < fieldPairListCount; index++)
			{
			if(index)
				{
				headerTableQuery.Append(KComma);	
				}
			headerTableQuery.Append(KQuote);
			headerTableQuery.Append(headerMetadata->iFieldList[index]->iName->Des());
			headerTableQuery.Append(KColon);
			headerTableQuery.Append(headerMetadata->iFieldList[index]->iName->Des());
			columnIndex++;
			}
		headerTableQuery.Append(KGetHeaderSchema4);		
		}
	else
		{
		headerTableQuery.Append(KGetHeaderSchema5);
		columnIndex++;
		}
	
	// Prepare the query and Bind the values.
	RSqlStatement sqlStmt;
	CleanupClosePushL(sqlStmt);
	User::LeaveIfError(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));

	TInt fieldIndex = 0;
	for(; fieldIndex<columnIndex; fieldIndex++)
		{
		switch(headerMetadata->iFieldList[fieldIndex]->iType)
			{
			case ESqlInt:
				BindIntL(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[fieldIndex]->iFieldNumValue);
				break;
			case ESqlInt64:
				BindInt64L(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[fieldIndex]->iFieldNumValue);
				break;
			case ESqlText:
				if(!aHeaderFields->iFieldPairList[fieldIndex]->iFieldTextValue)
					{
					User::Leave(KErrArgument);
					}
				BindTextL(sqlStmt, fieldIndex, aHeaderFields->iFieldPairList[fieldIndex]->iFieldTextValue->Des());
				break;
			}
		}

	if(aHeaderFields->iFieldPairList[fieldIndex]->iFieldTextValue)
		{
		BindTextL(sqlStmt, columnIndex++, aHeaderFields->iFieldPairList[fieldIndex]->iFieldTextValue->Des());	
		}
	else
		{
		User::Leave(KErrArgument);
		}

	BindIntL(sqlStmt, columnIndex++, UnmaskTMsvId(aEntryId));
	BindIntL(sqlStmt, columnIndex, aHeaderFields->iUid.iUid);

	// Execute the query and process the error.
	TInt err = sqlStmt.Exec();
	if(!err)
		{
		User::Leave(KErrGeneral);
		}
	
	User::LeaveIfError(err);
	CleanupStack::PopAndDestroy(2); //sqlStmt, headerTableQuery	
	}








/*
 * CheckStdColumnsAdded()
 * 
 * @param aFieldName: Column name
 * @return aColStatus: Column status flag.
 *
 * The function checks the existence of standard header fields
 * as defined in CMsvHeaderStore::TCommonHeaderField in the 
 * header table and sets the appropriate flag n aColStatus.
 */ 
void CMsvMessageDBAdapter::CheckStdColumnsAdded(HBufC* aFieldName, TInt& aColStatus)
	{
	// If it is a standard FORM field...
	if(!aFieldName->Des().Compare(_L("From")))
		{
		aColStatus |= CMsvHeaderStore::EFrom;
		}
	else
		{
		if(!aFieldName->Des().Compare(_L("To")))
			{
			aColStatus |= CMsvHeaderStore::ETo;			
			}
		else
			{
			if(!aFieldName->Des().Compare(_L("CC")))
				{
				aColStatus |= CMsvHeaderStore::ECC;				
				}
			else
				{
				if(!aFieldName->Des().Compare(_L("BCC")))
					{
					aColStatus |= CMsvHeaderStore::EBCC;
					}
				else
					{
					if(!aFieldName->Des().Compare(_L("Subject")))
						{
						aColStatus |= CMsvHeaderStore::ESubject;						
						}
					}	// BCC
				}	// CC
			}	// To
		}	// From
	}








/*
 * DeleteHeaderEntryL()
 * 
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Entry Id.
 *
 * The function deletes the header entry from the corresponding
 * header table. If multiple rows are associated with the header
 * entry, all such rows are deleted from the header table.
 */ 
void CMsvMessageDBAdapter::DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId)
	{
	TInt err = KErrNone;
	_LIT16(KGetHeaderSchema1, "DELETE FROM DB");
	_LIT16(KGetHeaderSchema2, ".HEADER_");
	_LIT16(KGetHeaderSchema3, " WHERE id = :id;");

	// Leave with KErrNotFound, if the header table does not exist.
	User::LeaveIfError(GetMetadataEntryIndex(aMtmId));
	
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);
		
	// This creates following string...
	// Ex: DELETE FROM DB<driveId>.HEADER_<mtmId> WHERE id = :id;
	headerTableQuery.Append(KGetHeaderSchema1);
	headerTableQuery.AppendNum(GetDriveId(aEntryId));
	headerTableQuery.Append(KGetHeaderSchema2);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KGetHeaderSchema3);

	//Prepare a statement for the query.
	RSqlStatement sqlStmt;
	CleanupClosePushL(sqlStmt);
	
	MSG_WRAP_DB_ERROR(sqlStmt.Prepare(iDatabaseRef, headerTableQuery));
	MSG_WRAP_DB_LEAVE(BindIntL(sqlStmt, 0, UnmaskTMsvId(aEntryId)));
	
	// Execute the query and handle the error.
	err = sqlStmt.Exec();
	if(!err)
		{
		User::Leave(KErrNotFound);
		}
	
	MSG_WRAP_DB_ERROR(err);
	CleanupStack::PopAndDestroy(2); //sqlStmt, headerTableQuery	
	}
	



/*
 * CopyHeaderEntryL()
 * 
 * @param aMtmId The MTM UId to identify the header table.
 * @param aSrcEntryId The source entry's TMsvId.
 * @param aDestEntryId The destination source entry's TMsvId.
 * @return None.
 * @leave KErrNoMemory while creating the buffer for the query.
 *		  
 *
 * The function copies header entry(s) of an entry from the corresponding
 * header table. If multiple rows are associated with the entry's TMsvId,
 * then copies of all such rows are made.
 * This is 
 */ 
void CMsvMessageDBAdapter::CopyHeaderEntryL(const TUid& aMtmId, const TMsvId& aSrcEntryId, const TMsvId& aDestEntryId)
	{
	_LIT(KComma, ", ");
	_LIT(KDelimiter, ";");
	_LIT(KQuote, "\"");
	_LIT(KCopyHeader1, "INSERT INTO DB");			// INSERT INTO DB1
	_LIT(KCopyHeader2, ".HEADER_");					// .HEADER_100
	_LIT(KCopyHeader3, " (id, uid");				//  (id, uid, field3, ...
	_LIT(KCopyHeader4, ") SELECT ");				// ) SELECT aDestEntryId
	_LIT(KCopyHeader5, ",uid");						// , uid, field3, ...
	_LIT(KCopyHeader6, " FROM HEADER_");			//  FROM HEADER_100
	_LIT(KCopyHeader7, " WHERE id = ");				//  WHERE id = aSrcEntryId;
	
	// Get metadata for the appropriate header
	TInt driveId = GetDriveId(aSrcEntryId);
	TInt metadataEntryIndex = GetMetadataEntryIndex(aMtmId);
	User::LeaveIfError(metadataEntryIndex);
	
	/*
	 We're looking to construct a query that looks like:
	 		INSERT INTO DB<driveId>.HEADER_<mtmId> (id, uid, field3...)
				SELECT aDestEntryId, uid, field3...
				WHERE id = aSrcEntryId;
	*/
	
	// Start query construction.
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);
	headerTableQuery.Append(KCopyHeader1);
	headerTableQuery.AppendNum(driveId);
	headerTableQuery.Append(KCopyHeader2);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KCopyHeader3);
	
	
	// Append column names to the query string.
	// Column names are stored in the header metadata structure.
	// We wrap the column names in quotes in case the column names are SQLite keywords. Eg: "From".
	CHeaderMetaData* headerMetadata = iHeaderDataList[metadataEntryIndex];
	RPointerArray<CFieldClass>& fieldList = headerMetadata->iFieldList;	
	TInt fieldListCount = fieldList.Count();
	TInt index = 0;
	for(; index < fieldListCount; ++index)
		{
		headerTableQuery.Append(KComma);
		headerTableQuery.Append(KQuote);
		headerTableQuery.Append(fieldList[index]->iName->Des());
		headerTableQuery.Append(KQuote);
		}
	
	headerTableQuery.Append(KCopyHeader4);
	headerTableQuery.AppendNum(UnmaskTMsvId(aDestEntryId));
	headerTableQuery.Append(KCopyHeader5);
	
	for(index = 0; index < fieldListCount; ++index)
		{
		headerTableQuery.Append(KComma);
		headerTableQuery.Append(KQuote);
		headerTableQuery.Append(fieldList[index]->iName->Des());
		headerTableQuery.Append(KQuote);
		}
	
	headerTableQuery.Append(KCopyHeader6);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KCopyHeader7);
	headerTableQuery.AppendNum(UnmaskTMsvId(aSrcEntryId));
	
	headerTableQuery.Append(KDelimiter);
	
	TInt err = iDatabaseRef.Exec(headerTableQuery);
	if(!err)
		{ // 0 returned by Exec is due to SELECT part of the query not returning any result to the INSERT part.
		User::Leave(KErrNotFound);
		}
	
	if(KSqlErrConstraint == err)
		{
		User::Leave(KErrAlreadyExists);
		}
	
	MSG_WRAP_DB_ERROR(err);
	CleanupStack::PopAndDestroy(); //headerTableQuery
	}



/*
 * CheckHeaderTableAndStdColumnFields()
 *
 * @param aMtmId: MtmId for related Header table
 * @param aHeaderFilelds: Header field specified in search query
 *
 * Check HeaderTable And StdColumn are exist. 
 */	
TBool CMsvMessageDBAdapter::CheckHeaderTableAndStdColumnFields(const TUid& aMtmId, TInt aHeaderFields)
	{
	TBool flag = EFalse;
	
	TInt index = GetMetadataEntryIndex(aMtmId);
	if(index != KErrNotFound)
		{
		// needs to check all standard columns given in a query are avilble in header table
		if( (iHeaderDataList[index]->iStdColumnStatus & aHeaderFields) == aHeaderFields)
			{
			flag = ETrue;
			}
		}
	return flag;
	}




TBool CMsvMessageDBAdapter::DoesAnyStoreExistsL(TMsvId aId, TUid aMtmId)
	{
	_LIT16(KCheckHeaderTableQuery1, "SELECT COUNT(*) FROM DB");
	_LIT16(KCheckHeaderTableQuery2, ".HEADER_");
	_LIT16(KCheckHeaderTableQuery3, " WHERE id = ");
	_LIT16(KSemiColon, ";");
	
	RBuf headerTableQuery;
	CleanupClosePushL(headerTableQuery);
	headerTableQuery.CreateL(KMaxQueryLength);

	headerTableQuery.Append(KCheckHeaderTableQuery1);
	headerTableQuery.AppendNum(GetDriveId(aId));
	headerTableQuery.Append(KCheckHeaderTableQuery2);
	headerTableQuery.AppendNum(aMtmId.iUid);
	headerTableQuery.Append(KCheckHeaderTableQuery3);
	headerTableQuery.AppendNum(UnmaskTMsvId(aId));
	headerTableQuery.Append(KSemiColon);

	TInt count = 0;
	TSqlScalarFullSelectQuery query(iDatabaseRef);
	TRAPD(err, count = query.SelectIntL(headerTableQuery));
	CleanupStack::PopAndDestroy();			// headerTableQuery
	
	if(err == KErrNone && count > 0)
		{
		return ETrue;
		}
	return EFalse;
	}
