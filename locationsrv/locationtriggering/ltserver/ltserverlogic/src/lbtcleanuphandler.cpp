/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This file defines the clean up handler
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <lbtcommon.h>
#include <f32file.h>
#include <s32file.h>
#include <centralrepository.h>
#include "lbtcleanuphandler.h"
#include "lbtcontainer.h"
#include "lbtcontainerupdatefilter.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbttriggeringsettingskeys.h"
#include "lbtlogger.h"

// LOCAL ENUMERATIONS

enum
	{
	TLbtNone,
	TLbtContainerCleaningUp,
	TLbtWaitingForTimer
	};

// CONSTANTS
_LIT(KLbtAppCleanupFileName, "lbtappcleanup.dat");

// ======== LOCAL FUNCTIONS ========
TInt CleanupItemOrder( const CLbtCleanupItem& aFirst, const CLbtCleanupItem& aSecond )
	{
	FUNC_ENTER("CleanupItemOrder");
	const TTime& aFirstTime = aFirst.GetTime();
	const TTime& aSecondTime = aSecond.GetTime();
	
	if( aFirstTime == aSecondTime )
		{
		return 0;
		}
	else if( aFirstTime < aSecondTime )
		{
		return -1;
		}
	else
		{
		return 1;
		}
	}

// ============ CLASS METHODS ==========

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::NewL
// ---------------------------------------------------------------------------
//
CLbtCleanupHandler* CLbtCleanupHandler::NewL( CLbtContainer& aContainer )
	{
	FUNC_ENTER("CLbtCleanupHandler::NewL");	
	CLbtCleanupHandler* self = new (ELeave) CLbtCleanupHandler( aContainer );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtCleanupHandler::ConstructL()
	{
	FUNC_ENTER("CLbtCleanupHandler::ConstructL");
	CTimer::ConstructL();	
	iOperation = TLbtNone;

	CRepository* repository = CRepository::NewL(TUid::Uid(KCRUidLocationTriggeringSettings));
	repository->Get(ELbtDaysForCleanup, iDaysForCleanup);
	delete repository;

	ReadCleanupDataFromFileL();
	CleanupNextItemL();
	}
	
// ---------------------------------------------------------------------------
// CLbtCleanupHandler::AddTriggersForCleanupL
// ---------------------------------------------------------------------------
//	
void CLbtCleanupHandler::AddTriggersForCleanupL( const RArray<TLbtTriggerId>& aTriggers )
	{
	FUNC_ENTER("CLbtCleanupHandler::AddTriggersForCleanupL");
	CLbtCleanupItem* item = new(ELeave) CLbtCleanupItem;
	
	TTime utcTime;
	utcTime.UniversalTime();
	
	TTimeIntervalDays cleanupDays(iDaysForCleanup);
	utcTime += cleanupDays;
		
	item->AddCleanupItem( aTriggers, utcTime );
	
	TLinearOrder<CLbtCleanupItem> order(CleanupItemOrder);
	iCleanupItems.InsertInOrder( item, order );

	WriteCleanupDataToFileL();
	CleanupNextItemL();
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::RemoveTriggersFromCleanupL
// ---------------------------------------------------------------------------
//	
void CLbtCleanupHandler::RemoveTriggersFromCleanupL( const RArray<TLbtTriggerId>& aTriggers )
	{
	FUNC_ENTER("CLbtCleanupHandler::RemoveTriggersFromCleanupL");
	// Bool to check if at the end of removal, current cleanup have to be aborted
	TBool cancelCleanup = EFalse;
	
	// Parse through cleanup items
	for( TInt j=0;j<iCleanupItems.Count();++j )
		{
		RArray<TLbtTriggerId>& array = iCleanupItems[j]->GetTriggers();
		for( TInt i=0;i<aTriggers.Count();++i )
			{
			TInt index = array.Find( aTriggers[i] );
			if( index != KErrNotFound )
				{
				array.Remove(index);
				}
			}

		if( array.Count() == 0 )
			{
			CLbtCleanupItem* item = iCleanupItems[j];
			iCleanupItems.Remove(j);
			delete item;
			if( j==0 )
				{
				cancelCleanup = ETrue;
				}			
			}
		}
	
	if( cancelCleanup )
		{
		Cancel();
		}
	WriteCleanupDataToFileL();
	CleanupNextItemL();
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::CleanupNextItemL
// ---------------------------------------------------------------------------
//	
void CLbtCleanupHandler::CleanupNextItemL()
	{
	FUNC_ENTER("CLbtCleanupHandler::CleanupNextItemL");
	if( iCleanupItems.Count() > 0 &&
	    !IsActive() )
		{
		const TTime& utcTime = iCleanupItems[0]->GetTime();
		
		// First get the universal time
		TTime time;
		time.UniversalTime();
		
		if( time > utcTime )
			{
			// The UT has already elapsed
			LOG("Provided UTC has already elapsed. Cleaning up Immediately");
			iOperation = TLbtContainerCleaningUp;
			
			// Get the trigger ids
			RArray<TLbtTriggerId>& triggers = iCleanupItems[0]->GetTriggers();
			
			CLbtTriggerFilterByAttribute* filterByAttribute = CLbtTriggerFilterByAttribute::NewL();
			CleanupStack::PushL(filterByAttribute);
			
			// Cleanup startup triggers
			filterByAttribute->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
			
			// Add the trigger ids trhat need to be cleanedup
			for( TInt i=0;i<triggers.Count();++i )
				{
				filterByAttribute->AddTriggerIdL( triggers[i] );
				}
				
			CLbtContainerUpdateFilter* updateFilter = CLbtContainerUpdateFilter::NewL( filterByAttribute ,NULL );		
			iContainer.DeleteTriggers( updateFilter, iContainerOp, iStatus );
			SetActive();
			
			CleanupStack::Pop(filterByAttribute);
			}
		else
			{
			if( !IsActive() )
				{
				LOG("Waiting for Provided UTC")
				// Wait for timer expiry
				iOperation = TLbtWaitingForTimer;
				AtUTC(utcTime);
				}			
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::Cancel
// ---------------------------------------------------------------------------
//	
void CLbtCleanupHandler::Cancel()
	{
	FUNC_ENTER("CLbtCleanupHandler::Cancel");
	
	if( IsActive() )
		{
		if( iOperation == TLbtContainerCleaningUp )
			{
			iContainer.CancelAsyncOperation( iContainerOp );		
			}
		else if( iOperation == TLbtWaitingForTimer )
			{
			CTimer::Cancel();
			}
		}
	iOperation = TLbtNone;
	}

	
// ---------------------------------------------------------------------------
// CLbtCleanupHandler::RunL
// ---------------------------------------------------------------------------
//	
void CLbtCleanupHandler::RunL()
	{
	FUNC_ENTER("CLbtCleanupHandler::RunL");
	LOG1("iOperation %d",iOperation);
	
	if( iStatus.Int() != KErrNone )
		{
		LOG1("CLbtCleanupHandler, iStatus = %d", iStatus.Int());
		return;
		}

	switch(iOperation)
		{
		case TLbtContainerCleaningUp: 
			{
			CLbtCleanupItem* item = iCleanupItems[0];
			iCleanupItems.Remove(0);
			delete item;
			iOperation = TLbtNone;
			WriteCleanupDataToFileL();
			CleanupNextItemL();
			break;
			}
		case TLbtWaitingForTimer:
			{
			LOG("Timer has expired. Cleaning triggers");
			iOperation = TLbtContainerCleaningUp;
			
			// Get the trigger ids
			RArray<TLbtTriggerId>& triggers = iCleanupItems[0]->GetTriggers();
			
			CLbtTriggerFilterByAttribute* filterByAttribute = CLbtTriggerFilterByAttribute::NewL();
			CleanupStack::PushL(filterByAttribute);
			
			// Cleanup startup triggers
			filterByAttribute->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
			
			// Add the trigger ids trhat need to be cleaned up
			for( TInt i=0;i<triggers.Count();++i )
				{
				filterByAttribute->AddTriggerIdL( triggers[i] );
				}
				
			CLbtContainerUpdateFilter* updateFilter = CLbtContainerUpdateFilter::NewL( filterByAttribute ,NULL );		
			iContainer.DeleteTriggers( updateFilter, iContainerOp, iStatus );
			SetActive();
			
			CleanupStack::Pop(filterByAttribute);
			break;
			}
		default:
			{
			iOperation = TLbtNone;
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::RunError
// ---------------------------------------------------------------------------
//	
TInt CLbtCleanupHandler::RunError( TInt aError )
	{
	FUNC_ENTER("CLbtCleanupHandler::RunError");
	ERROR("CLbtCleanupHandler::RunL left with error code", aError);
	return KErrNone;
	}
	
// ---------------------------------------------------------------------------
// CLbtCleanupHandler::WriteCleanupDataToFileL
// ---------------------------------------------------------------------------
//
void CLbtCleanupHandler::WriteCleanupDataToFileL()
	{
	FUNC_ENTER("CLbtCleanupHandler::WriteCleanupDataToFileL");	
	
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	
	CleanupClosePushL( fs );
    
    // Obtain the file path
    TFileName file;
    
    // Gets the path in which the file can be created
    fs.SessionPath(file);

    // Create the file Directory ie the private directory of the process
    fs.MkDirAll(file);
    
    // Append the name of the file
    file.Append(KLbtAppCleanupFileName);
    
    // Open write stream to write to the file
    RFileWriteStream writeStream;
    
    // Open the file to replace the contents. If the file is not preset
    // this method will create the file
    TInt error = writeStream.Replace( fs, file, EFileWrite );    
    if( error != KErrNone )
    	{
    	ERROR("Opening of cleanup file failed with : %d", error);
    	writeStream.Close();
    	CleanupStack::PopAndDestroy(); //fs
    	User::Leave(error);
    	}
    CleanupClosePushL( writeStream );
    
    // First write the number of cleanup items
    writeStream.WriteInt16L( iCleanupItems.Count() );

	for(TInt i=0;i<iCleanupItems.Count();++i)
		{
		RArray<TLbtTriggerId>& triggers = iCleanupItems[i]->GetTriggers();
		
		// Write the trigger ids into the file
		writeStream.WriteInt16L( triggers.Count() );
		for(TInt j=0;j<triggers.Count();++j)
			{
			writeStream.WriteUint32L( triggers[j] );
			}
		
		// Write the time into the file
		const TDateTime dateTime = iCleanupItems[i]->GetTime().DateTime();
		
		// Write the year
		writeStream.WriteInt32L( dateTime.Year() );
		
		// Write the month
		writeStream.WriteInt32L( dateTime.Month() );
		
		// Write the day
		writeStream.WriteInt32L( dateTime.Day() );
		}
    
    CleanupStack::PopAndDestroy(2); //fs and writeSteam    
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::ReadCleanupDataFromFileL
// ---------------------------------------------------------------------------
//
void CLbtCleanupHandler::ReadCleanupDataFromFileL()
	{
	FUNC_ENTER("CLbtCleanupHandler::ReadCleanupDataFromFileL");
	
	// First reset the cleanup data 
	iCleanupItems.ResetAndDestroy();
	
	// Open handle to file system
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	CleanupClosePushL(fs);
	
	// Obtain the file path
    TFileName file;
    
    // Gets the path in which the file can be created
    fs.SessionPath(file);

    // Create the file Directory ie the private directory of the process
    fs.MkDirAll(file);
    
    // Append the name of the file
    file.Append(KLbtAppCleanupFileName);
    
    // Open read stream
    RFileReadStream readStream;
    
    TInt error = readStream.Open( fs, file, EFileRead );
    if( error != KErrNone )
    	{
    	// File may not exists
    	ERROR("Opening of cleanup file failed with : %d", error);
    	readStream.Close();
    	CleanupStack::PopAndDestroy(); //fs
    	return;
    	}
    CleanupClosePushL( readStream );
    
    // Get the count in the cleanup array
    TInt count = readStream.ReadInt16L();    
    for(TInt i=0; i<count; ++i)
    	{
    	RArray<TLbtTriggerId> triggers;
    	CleanupClosePushL(triggers);
    	
    	// Read the trigger ids
    	TInt triggerCount = readStream.ReadInt16L();
    	for(TInt j=0;j<triggerCount;++j)
    		{
    		triggers.Append( readStream.ReadUint32L() );
    		}
    	
    	// Read the universal time
    	TInt year = readStream.ReadInt32L();
    	TMonth month = static_cast<TMonth>( readStream.ReadInt32L() );
    	TInt day = readStream.ReadInt32L();
    	
    	TDateTime dateTime( year,
    						month,
    						day,
    						0,
    						0,
    						0,
    						0 );
    	TTime time( dateTime );
    	
    	CLbtCleanupItem* cleanupItem = new (ELeave) CLbtCleanupItem();
    	cleanupItem->AddCleanupItem( triggers, time );
    	
    	CleanupStack::Pop(); // triggers
    	triggers.Close();
    	
    	// Append cleanup item to the cleaup items array
    	iCleanupItems.Append( cleanupItem );
    	}

    CleanupStack::PopAndDestroy(2); // fs and readStream
	}

// ---------------------------------------------------------------------------
// CLbtCleanupHandler::~CLbtCleanupHandler
// ---------------------------------------------------------------------------
//	
CLbtCleanupHandler::~CLbtCleanupHandler()
	{
	FUNC_ENTER("CLbtCleanupHandler::~CLbtCleanupHandler");
	Cancel();	
	iCleanupItems.ResetAndDestroy();
	}
	
// ---------------------------------------------------------------------------
// CLbtCleanupHandler::CLbtCleanupHandler
// ---------------------------------------------------------------------------
//
CLbtCleanupHandler::CLbtCleanupHandler( CLbtContainer& aContainer ): 
					CTimer(CActive::EPriorityLow),
					iContainer( aContainer )
	{
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CLbtCleanupItem::AddCleanupItem
// ---------------------------------------------------------------------------
//
void CLbtCleanupItem::AddCleanupItem( const RArray<TLbtTriggerId>& aTriggers, TTime& aUTCTime )
	{
	FUNC_ENTER("CLbtCleanupItem::AddCleanupItem");
	iTriggers.Reset();	
	for(TInt i=0;i<aTriggers.Count();++i)
		{
		TInt error = iTriggers.Append( aTriggers[i] );
		if( error != KErrNone )
		    {
            LOG1("Failing to append triggers to the array:%d",error );
            return;
		    }
		}
	iUTCTime = aUTCTime;
	}

// ---------------------------------------------------------------------------
// CLbtCleanupItem::GetTime
// ---------------------------------------------------------------------------
//
const TTime& CLbtCleanupItem::GetTime() const
	{
	return iUTCTime;
	}
	
// ---------------------------------------------------------------------------
// CLbtCleanupItem::GetTriggers
// ---------------------------------------------------------------------------
//
RArray<TLbtTriggerId>& CLbtCleanupItem::GetTriggers()
	{
	return iTriggers;
	}
	
// ---------------------------------------------------------------------------
// CLbtCleanupItem::~CLbtCleanupItem
// ---------------------------------------------------------------------------
//
CLbtCleanupItem::~CLbtCleanupItem()
	{
	FUNC_ENTER("CLbtCleanupItem::~CLbtCleanupItem");
	iTriggers.Close();
	}

// end of file

