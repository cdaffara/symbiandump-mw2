// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <centralrepository.h>
#include <f32file.h>

#include <cmsvtechnologytypedefaultmtmsettings.h>
#include "MTSR.H"
#include "MSVRUIDS.H"
#include "MCLIENT.H"
#include "MSVREG.H"

const TInt KUidDefaultMtmRepositoryFileValue = 0x10274556;
const TUid KUidDefaultMtmRepositoryFile = { KUidDefaultMtmRepositoryFileValue };
			
/** 
Constructor. 

@param aFs Reference to a RFs. The RFs should be connected. 
*/
CMsvTechnologyTypeDefaultMtmSettings::CMsvTechnologyTypeDefaultMtmSettings( RFs& aFs ) :
	iFs( aFs )
	{
	}

/** 
Destructor. 
*/
EXPORT_C CMsvTechnologyTypeDefaultMtmSettings::~CMsvTechnologyTypeDefaultMtmSettings( )
	{
	delete iRepository;
		
	iTechnologyTypeDefaultMtmArray.Reset( );
	iRemovedTechnologyTypes.Reset( );	
	}

/** 
NewL factory function to create a CMsvTechnologyTypeDefaultMtmSettings object. 

@param aFs Reference to a RFs. The RFs should be connected. 
@return A newly created CMsvTechnologyTypeDefaultMtmSettings object
*/
EXPORT_C CMsvTechnologyTypeDefaultMtmSettings* CMsvTechnologyTypeDefaultMtmSettings::NewL(RFs& aFs)
	{
	CMsvTechnologyTypeDefaultMtmSettings* self = new(ELeave) CMsvTechnologyTypeDefaultMtmSettings(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);	
	return self;	
	}

/**
Second phase construction.
*/
void CMsvTechnologyTypeDefaultMtmSettings::ConstructL()
	{
	iRepository = CRepository::NewL(KUidDefaultMtmRepositoryFile);
	}

/** 
Get the default MTM. 

This function returns the UID of the default MTM that is set for the specified technology type.

@param aTechnologyTypeUid UID of the Technology Type 
@param aMtmUid returns the UID of the MTM that is set as default 
@return KErrNone if successful, KErrNotFound if a default MTM is not set for the specified aTechnologyTypeUid 
*/
EXPORT_C TInt CMsvTechnologyTypeDefaultMtmSettings::DefaultMtm( TUid aTechnologyTypeUid, TUid& aMtmUid ) const
	{
	TMsvTechnologyTypeDefaultMtmInfo technologyTypeDefaultMtmInfo;
	TInt error = FindDefaultMtm( aTechnologyTypeUid, technologyTypeDefaultMtmInfo );
	if ( error >= KErrNone )
		{
		error = KErrNone;
		}
		
	aMtmUid = technologyTypeDefaultMtmInfo.iMtmUid;
	return error;
	}

/**
Set the default MTM. 

This function sets the default MTM for the specified technology type to the specified MTM UID.

@param aTechnologyTypeUid UID of the Technology Type 
@param aMtmUid UID of the MTM that is set as default
@leave KErrArgument if aMtmUid is not part of the specified technology type group (aTechnologyTypeUid) 
*/
EXPORT_C void CMsvTechnologyTypeDefaultMtmSettings::SetDefaultMtmL( TUid aTechnologyTypeUid, TUid aMtmUid )
	{
	// Check whether the MTM with aMtmUid belongs to a group with aTechnologyTypeUid
	// The function leaves if the test fails.
	CheckIfMtmIsPartOfTechnologyTypeGroupL( aTechnologyTypeUid, aMtmUid, KUidMtmServerComponent );
	
	TMsvTechnologyTypeDefaultMtmInfo defaultTechnologyTypeMtm;
	TInt indexAndErrorValue = FindDefaultMtm( aTechnologyTypeUid, defaultTechnologyTypeMtm );
		
	defaultTechnologyTypeMtm.iTechnologyTypeUid = aTechnologyTypeUid;	
	defaultTechnologyTypeMtm.iMtmUid = aMtmUid;
	
	if ( indexAndErrorValue == KErrNotFound )
		{
		iTechnologyTypeDefaultMtmArray.AppendL( defaultTechnologyTypeMtm );
		
		// Remove aTechnologyTypeUid from the iRemovedTechnologyTypes, to ensure that the order
		// in the Central Repository does not change (The key isn't deleted)
		indexAndErrorValue = iRemovedTechnologyTypes.Find( aTechnologyTypeUid );
		if ( indexAndErrorValue >= KErrNone )
			{
			iRemovedTechnologyTypes.Remove( indexAndErrorValue );
			}
		}
	else
		{
		iTechnologyTypeDefaultMtmArray[ indexAndErrorValue ] = defaultTechnologyTypeMtm;
		}	
	}

/** 
Removes the default MTM. 

This function removes the UID that is set as default MTM from the list. If DefaultMtm() is called
after this function, KErrNotFound will be returned

@param aTechnologyTypeUid UID of the Technology Type 
@param aMtmUid returns the UID of the MTM that is set as default 
@return KErrNone if successful 
*/
EXPORT_C TInt CMsvTechnologyTypeDefaultMtmSettings::RemoveDefaultMtm( TUid aTechnologyTypeUid )
	{
	TInt indexAndErrorValue = iRemovedTechnologyTypes.Append( aTechnologyTypeUid );
	if ( indexAndErrorValue >= KErrNone )
		{
		TMsvTechnologyTypeDefaultMtmInfo technologyTypeDefaultMtmInfo;
		indexAndErrorValue = FindDefaultMtm( aTechnologyTypeUid, 
											 technologyTypeDefaultMtmInfo );
		if ( indexAndErrorValue >= KErrNone )
			{
			iTechnologyTypeDefaultMtmArray.Remove( indexAndErrorValue );	
			}
		indexAndErrorValue = KErrNone;
		}
	return indexAndErrorValue;
	}

/** 
Load the default MTM settings from the central repository. 

This function loads the list of default MTMs that is stored in the central repository. The list
is first emptied. 
*/		
EXPORT_C void CMsvTechnologyTypeDefaultMtmSettings::LoadSettingsL( )
	{
	iTechnologyTypeDefaultMtmArray.Reset( );
	iRemovedTechnologyTypes.Reset( );
		
	RArray<TUint32> foundKeys;
	CleanupClosePushL( foundKeys );

	TInt error = iRepository->FindNeqL( 0, 0, 0, foundKeys );
	if ( error < KErrNotFound )
		{
		User::Leave( error );
		}
	
	TInt keyCount = foundKeys.Count( );
	while ( keyCount-- != 0 )
		{
		TMsvTechnologyTypeDefaultMtmInfo technologyTypeDefaultMtmInfo;
		technologyTypeDefaultMtmInfo.iTechnologyTypeUid.iUid = foundKeys[keyCount];
		
		User::LeaveIfError( iRepository->Get( (TUint32) technologyTypeDefaultMtmInfo.iTechnologyTypeUid.iUid, 
											 (TInt&) technologyTypeDefaultMtmInfo.iMtmUid.iUid ) ) ;
		iTechnologyTypeDefaultMtmArray.AppendL( technologyTypeDefaultMtmInfo );
		}

	CleanupStack::PopAndDestroy(&foundKeys);	
	}	

/** 
Save the default MTM settings to the central repository. 

This function saves the list of default MTMs that is stored in the central repository. Entries that 
are set to delete will be removed first. The function will leave with error code 
KErrLocked if another save is in progress. 
*/
EXPORT_C void CMsvTechnologyTypeDefaultMtmSettings::SaveSettingsL( )
	{	
	User::LeaveIfError(iRepository->StartTransaction(CRepository::EReadWriteTransaction));
	iRepository->CleanupRollbackTransactionPushL( );

	TInt mtmCount = iRemovedTechnologyTypes.Count( );
	while ( mtmCount-- != 0 )
		{
		iRepository->Delete(iRemovedTechnologyTypes[mtmCount].iUid);
		}
		
	TMsvTechnologyTypeDefaultMtmInfo technologyTypeDefaultMtmInfo;	
	mtmCount = iTechnologyTypeDefaultMtmArray.Count( );
	while ( mtmCount-- != 0 )
		{
		technologyTypeDefaultMtmInfo = iTechnologyTypeDefaultMtmArray[mtmCount];		
		iRepository->Set((TUint32)technologyTypeDefaultMtmInfo.iTechnologyTypeUid.iUid, 
						 		 			 (TInt)technologyTypeDefaultMtmInfo.iMtmUid.iUid);
		}

	CleanupStack::Pop();
	TUint32 errorId = 0;	
	User::LeaveIfError(iRepository->CommitTransaction(errorId));
	}

void CMsvTechnologyTypeDefaultMtmSettings::CheckIfMtmIsPartOfTechnologyTypeGroupL( const TUid aTechnologyTypeUid,
																const TUid aMtmUid, const TUid aMtmTypeUid ) const	
	{
	CRegisteredMtmDllArray* regMtmDllArray = new (ELeave) CRegisteredMtmDllArray( );
	CleanupStack::PushL( regMtmDllArray );

	RMsvServerSession session;
	User::LeaveIfError(session.Connect(iFs));
	
	session.FillRegisteredMtmDllArray(aMtmTypeUid, *regMtmDllArray, KMsvDefaultTimeoutMicroSeconds32);
	session.Close();
	
	CRegisteredMtmDll* regMtmDll = NULL;
	TInt error = KErrArgument;
	TInt mtmCount = regMtmDllArray->Count( );

	while ( mtmCount-- != 0 && error != KErrNone)
		{
		regMtmDll = regMtmDllArray->At( mtmCount );
		if ( regMtmDll->MtmTypeUid( ) == aMtmUid && regMtmDll->TechnologyTypeUid( ) == aTechnologyTypeUid )
			{
			error = KErrNone;
			}
		}
	CleanupStack::PopAndDestroy( regMtmDllArray );	
	User::LeaveIfError( error );
	}

TInt CMsvTechnologyTypeDefaultMtmSettings::FindDefaultMtm( const TUid aTechnologyTypeUid, 
													 TMsvTechnologyTypeDefaultMtmInfo& aTechnologyTypeDefaultMtmInfo ) const
	{
	TMsvTechnologyTypeDefaultMtmInfo technologyTypeMtmInfo;
	
	TInt count = iTechnologyTypeDefaultMtmArray.Count( );
	while ( count-- > 0 )
		{
		technologyTypeMtmInfo = iTechnologyTypeDefaultMtmArray[count];
		if ( technologyTypeMtmInfo.iTechnologyTypeUid == aTechnologyTypeUid )
			{
			aTechnologyTypeDefaultMtmInfo = technologyTypeMtmInfo;
			return KErrNone;
			}
		}
	return KErrNotFound;
	}
