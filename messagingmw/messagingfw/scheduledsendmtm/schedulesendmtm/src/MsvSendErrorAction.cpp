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
//

#include <e32base.h>
#include <msvstd.h>
#include <msventry.h>
#include <barsread.h>
#include <msvsenderroraction.h>
#include <schsend_panic.h>


//
//
//	TMsvSendErrorAction Implementation
//
//

/**
Default constructor.
*/

EXPORT_C TMsvSendErrorAction::TMsvSendErrorAction()
	{
	Reset();
	}


/*
EXPORT_C TMsvSendErrorAction::TMsvSendErrorAction(const TMsvSendErrorAction& aErrorAction)
	{
	iAction			= aErrorAction.iAction;
	iError			= aErrorAction.iError;
	iMaxRetries		= aErrorAction.iMaxRetries;
	iRetries		= aErrorAction.iRetries;
	iRetrySpacing	= aErrorAction.iRetrySpacing;
	iVersion		= aErrorAction.iVersion;
	}
*/



/**
Sets all member data to default values.
*/

EXPORT_C void TMsvSendErrorAction::Reset()
	{
	iError				= KErrNone;
	SetMaxRetries(3);
	iAction				= ESendActionFail;
	iRetries			= ESendRetriesFixed;
	iRetrySpacing		= ESendRetrySpacingStatic;
	}


/**
Sets the maximum number of retries.

@param aMaxRetries
Maximum number of retries.

@panic ScheduleSend-DLL 21
The maximum number of retries is negative.
*/

EXPORT_C void TMsvSendErrorAction::SetMaxRetries(const TInt16 aMaxRetries)
	{
	__ASSERT_ALWAYS(aMaxRetries >= 0, gPanic(EMaxRetriesMustBePositive));

	iMaxRetries = aMaxRetries;
	}


/**
Gets the maximum number of retries.

@return Maximum number of retries
*/

EXPORT_C TInt16 TMsvSendErrorAction::MaxRetries() const
	{
	return iMaxRetries;
	}


//
//
// CMsvSendErrorActions Implementation
//
//

/**
Allocates and creates a new CMsvSendErrorActions object.

@return New object
*/
EXPORT_C CMsvSendErrorActions* CMsvSendErrorActions::NewL()
	{
	CMsvSendErrorActions* self = CMsvSendErrorActions::NewLC();
	CleanupStack::Pop(self); 
	return self;
	}

/**
Allocates and creates a new CMsvSendErrorActions object.

@return New object
*/
EXPORT_C CMsvSendErrorActions* CMsvSendErrorActions::NewLC()
	{
	CMsvSendErrorActions* self = new (ELeave) CMsvSendErrorActions();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

//
//	CMsvSendErrorActions Constructor
//

CMsvSendErrorActions::CMsvSendErrorActions()
	{
	iDefault.Reset();
	}


void CMsvSendErrorActions::ConstructL()
	{
	iErrors = new (ELeave) CArrayFixFlat<TMsvSendErrorAction>(1);
	}


/**
Destructor.
*/

EXPORT_C CMsvSendErrorActions::~CMsvSendErrorActions()
	{
	delete iErrors;
	}
	
EXPORT_C void CMsvSendErrorActions::Reset()
	{
	iDefault.Reset();
	iErrors->Reset();
	}


/**
Adds a TMsvSendErrorAction object. 

If a TMsvSendErrorAction already exists in the collection for the same error, 
then it is replaced by aAction.

@param aErrorAction
Error action to add.

@leave Any error code but KErrNone and KErrNotFound
We tried to delete the error action if it already exists and 
RemoveSendErrorAction() returned an error.

@leave KErrAlreadyExists
A duplicate was found when inserting the error action in the array.
*/

EXPORT_C void CMsvSendErrorActions::AddSendErrorActionL(const TMsvSendErrorAction& aErrorAction)
	{
	//Delete the error if it already exists.
	TInt error = RemoveSendErrorAction(aErrorAction.iError);

	if (error != KErrNone && error != KErrNotFound)
		{
		User::Leave(error);
		}

	TInt offSet = _FOFF(TMsvSendErrorAction, iError);
	TKeyArrayFix key(offSet, ECmpTInt);

	//Insert the error action into the array of errors sorted by iError.
	//No duplicates allowed
	iErrors->InsertIsqL(aErrorAction, key);
	}


/**
Gets an error action for a specified error.

@param aError
Error to find.

@param aErrorAction
On return, the corresponding TMsvSendErrorAction.

@return KErrNotFound if there is no corresponding TMsvSendErrorAction 
for aError.
*/

EXPORT_C TInt CMsvSendErrorActions::GetSendErrorAction(const TInt aError, TMsvSendErrorAction& aErrorAction) const
	{
	TInt index = 0;
	TInt error = Find(aError, index);

	if (error == KErrNone)
		{
		aErrorAction = iErrors->At(index);
		}

	return error;
	}


/*
	Find

	Returns the index of the TMsvSendErrorAction in iErrors where
	TMsvSendErrorAction::iError == aError.

	Return KErrNotFound if the aError is not found in iErrors
*/

TInt CMsvSendErrorActions::Find(const TInt aError, TInt& aIndex) const
	{
	TMsvSendErrorAction tempAction;
	tempAction.iError = aError;

	TInt offSet = _FOFF(TMsvSendErrorAction, iError);
	TKeyArrayFix key(offSet, ECmpTInt);

	TInt found = iErrors->FindIsq(tempAction, key, aIndex);

	if (found != 0) //Not Found
		{
		return KErrNotFound;
		}

	return KErrNone;
	}


/**
Removes an error action.

@param aError
Error to remove.

@return KErrNotFound if there is no corresponding TMsvSendErrorAction for 
aError
*/

EXPORT_C TInt CMsvSendErrorActions::RemoveSendErrorAction(const TInt aError)
	{
	TInt index = 0;
	
	const TInt error = Find(aError, index);

	if (error == KErrNone)
		{
		iErrors->Delete(index);
		}

	return error;
	}




/**
Restores the object from a resource. 

The resource must be of type SEND_ERROR_ACTIONS.

@param aReader
Resource reader at appropriate resource.

@leave AddSendErrorActionL()
*/

EXPORT_C void CMsvSendErrorActions::RestoreFromResourceL(TResourceReader& aReader)
	{
	iErrors->Reset();

	RestoreErrorActionL(aReader, 1, ETrue);

	TInt actionTotal = aReader.ReadInt16();

	RestoreErrorActionL(aReader, actionTotal, EFalse);
	}


void CMsvSendErrorActions::RestoreErrorActionL(TResourceReader& aReader, TInt aActionCount, const TBool aDefault)
	{
	while (aActionCount--)
		{
		TInt actionFlags = aReader.ReadInt8();
		TMsvSendErrorAction errorAction;

		errorAction.iAction = (TMsvSendAction) (actionFlags & KActionMask);
		errorAction.iRetries = (TMsvSendRetries) (actionFlags & KRetriesMask);
		errorAction.iRetrySpacing = (TMsvSendRetrySpacing) (actionFlags & KRetrySpacingMask);

		errorAction.SetMaxRetries((TInt16) aReader.ReadInt16());

		TInt errorCount = aReader.ReadInt16();

		while (errorCount--)
			{
			errorAction.iError = aReader.ReadInt32();
			aDefault ? SetDefault(errorAction) : AddSendErrorActionL(errorAction);
			}
		}
	}



/**
Gets the object's array of error-actions.

@return The object's array of error-actions
*/

EXPORT_C const CArrayFixFlat<TMsvSendErrorAction>& CMsvSendErrorActions::Errors() const
	{
	return *iErrors;
	}


/**
Sets the object to an array of error-actions.

@param aErrors
Array to use.

@leave AddSendErrorActionL()
*/

EXPORT_C void CMsvSendErrorActions::SetErrorsL(const CArrayFixFlat<TMsvSendErrorAction>& aErrors)
	{
	iErrors->Reset();

	const TInt count = aErrors.Count();

	for (TInt i = 0; i < count; i++)
		{
		AddSendErrorActionL(aErrors[i]);
		}
	}


/**
Gets the default error-action.

The default is used if a specified error-action is not found for the error.

@return Default error-action
*/

EXPORT_C const TMsvSendErrorAction& CMsvSendErrorActions::Default() const
	{
	return iDefault;
	}


/**
Sets the default error-action.

@param aAction
Default error-action.
*/

EXPORT_C void CMsvSendErrorActions::SetDefault(const TMsvSendErrorAction& aAction)
	{
	iDefault = aAction;
	}

