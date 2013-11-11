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
// Description: The class provides interface to create message header
// store.

/**
 * HEADER FILES
 */
#include <msvapi.h>



/**
 * MEMBER FUNCTIONS OF CMsvHeaderStore
 */


/**
 * CMsvHeaderStore()
 * The default Constructor. 
 *
 * @param aMtmId: The MTM-Id of the header store to be created.
 * @param aSession: A reference to the session object. 
 */
CMsvHeaderStore::CMsvHeaderStore(TUid aMtmId, CMsvSession& aSession)
:iMtmUid(aMtmId),iSessionRef(aSession)
	{
	}




/**
 * ~CMsvHeaderStore()
 *
 * The default Destructor.
 */
CMsvHeaderStore::~CMsvHeaderStore()
	{
	if(iLastErrorMessage)
		{
		delete iLastErrorMessage;		
		}

	iFieldDetails.ResetAndDestroy();
	iFieldDetails.Close();
	}





/**
 * NewLC()
 * 
 * Allocates and constructs a new CMsvHeaderStore object.
 *
 * @param aMtmId: The MTM-Id of the header store to be created.
 * @param aSession: A reference to the session object.
 * @leave KErrNoMemory Not enough memory to create object. 
 * 
 @publishedAll
 @released
 */
EXPORT_C CMsvHeaderStore* CMsvHeaderStore::NewLC(const TUid& aMtmId, CMsvSession& aSession)
	{
	//Create the DBAdapter object.
	CMsvHeaderStore* self = new(ELeave) CMsvHeaderStore(aMtmId, aSession);
	CleanupStack::PushL(self);
	return self;
	}
	
	
	



/**
 * NewL()
 * 
 * Allocates and constructs a new CMsvHeaderStore object.
 *
 * @param aMtmId: The MTM-Id of the header store to be created.
 * @param aSession: A reference to the session object.
 * @leave KErrNoMemory Not enough memory to create object. 
 * 
 @publishedAll
 @released
 */
EXPORT_C CMsvHeaderStore* CMsvHeaderStore::NewL(const TUid& aMtmId, CMsvSession& aSession)
	{
	//Create the DBAdapter object.
	CMsvHeaderStore* self = new(ELeave) CMsvHeaderStore(aMtmId, aSession);
	return self;
	}





/**
 * AddFieldL()
 *
 * Adds a new field to the header store.
 *
 * @param aFieldName: Name of the header field.
 * @param aFieldType: Data type of the field.
 * @leave KErrNoMemory Not enough memory to create object.
 * 
 @publishedAll
 @released
 */
EXPORT_C void CMsvHeaderStore::AddFieldL(HBufC* aFieldName, EFieldType aFieldType)
	{
	if(NULL == aFieldName)
		{
		User::Leave(KErrArgument);
		}

	CFieldPair *fieldObject = new(ELeave) CFieldPair();
	CleanupStack::PushL(fieldObject);
	
	fieldObject->iFieldType = aFieldType;
	fieldObject->iFieldName = aFieldName;
	
	iFieldDetails.AppendL(fieldObject);
	CleanupStack::Pop(fieldObject);
	}
	
	



/**
 * AddFieldL()
 *
 * Adds a new standard field to the header store. It is recommended
 * that user calls this version of AddField() to add standard fields 
 * to header store. The function might leave with KErrArgument or KErrNoMemory.
 *
 * @param aCommonHeaderFieldName: Standard header fields, To, CC, BCC, From, Subject.
 * @leave KErrNoMemory Not enough memory to create object. 
 * @leave KErrArgument Invalid argument passed.
 *
 @publishedAll
 @released
 */	
EXPORT_C void CMsvHeaderStore::AddFieldL(TCommonHeaderField aCommonHeaderFieldName)
	{
	CFieldPair *fieldObject = new(ELeave) CFieldPair();
	CleanupStack::PushL(fieldObject);
	
	fieldObject->iFieldType = ETextField;
	switch (aCommonHeaderFieldName)
		{
		case EFrom:
			_LIT16(KFromText, "From");
			fieldObject->iFieldName = KFromText().AllocL();			
			break;
		
		case ETo:
			_LIT16(KToText, "To");
			fieldObject->iFieldName = KToText().AllocL();
			break;
		
		case ECC:
			_LIT16(KCCText, "CC");
			fieldObject->iFieldName = KCCText().AllocL();
			break;
		
		case EBCC:
			_LIT16(KBCCText, "BCC");
			fieldObject->iFieldName = KBCCText().AllocL();
			break;
	
		case ESubject:
			_LIT16(KSubjectText, "Subject");
			fieldObject->iFieldName = KSubjectText().AllocL();
			break;
		
		default:
			User::Leave(KErrArgument);
		}
	
	iFieldDetails.AppendL(fieldObject);
	CleanupStack::Pop(fieldObject);
	}





/**
 * CreateStoreL()
 *
 * The API creates the header store in the message store in the message 
 * server current drive. The caller must call AddFieldL() APIs to specify
 * the header structure before calling this API.
 *
 * A headerStore is associated with a mtm type and stores message header
 * data. Message server creates a separate store for each MTM type. A 
 * header-entry in the headerStore is essentially a list of UID-Data entries.
 * For example an email header-entry can have CImHeader, MIME header and 
 * encoded header stored as a separate UID-Data entry in the header store.
 * By default a UID-Data entry will have a "Default" field which store the
 * content of 'Data'. Moreover one such UID-Data pair can also specify mutiple
 * fields such that it can store individual portion of data in such fields 
 * separately. 
 *
 * @leave All system wide leave code.
 *
 * @capability WriteDeviceData
 *
 @publishedAll
 @released
 */
EXPORT_C void CMsvHeaderStore::CreateStoreL()
	{
	iSessionRef.Session().CreateStoreL(iMtmUid, iFieldDetails);
	}
	
	
	




/**
 * DoesStoreExists()
 *
 * The function checks the existence of a header store
 * for a given MTM Id. The function can be called to 
 * check if the header store already exist before creating
 * the store.
 *
 * @param aMtmId: The MTM UID under consideration.
 * @param aSession: A reference to session with the server.
 * @return TBool: ETrue, if the header store for passed MTM
 * ID exists, otherwise returns EFalse.
 * @leave: All system wide error code.
 *
 * @capability ReadDeviceData 
 *
 @publishedAll
 @released
 */
EXPORT_C TBool CMsvHeaderStore::DoesStoreExistsL(const TUid& aMtmId, CMsvSession& aSession)
	{
	return aSession.Session().DoesStoreExistsL(aMtmId);
	}
	
	
	

	
/**
 * LastErrorMessage()
 *
 * The function returns the detail error message 
 * received while creating the header store.
 *
 * @return TPtrC: The detail error message.
 * @leave All system wide leave code.
 *
 @publishedAll
 @released
 */
EXPORT_C TPtrC CMsvHeaderStore::LastErrorMessageL()
	{
	if(iLastErrorMessage)
		{
		delete iLastErrorMessage;
		iLastErrorMessage = NULL;
		}
	iSessionRef.Session().LastErrorMessageL(iLastErrorMessage);
	return iLastErrorMessage->Des();
	}



