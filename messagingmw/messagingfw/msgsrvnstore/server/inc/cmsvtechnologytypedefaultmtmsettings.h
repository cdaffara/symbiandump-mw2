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

#ifndef __CMSVTECHNOLOGYTYPEDEFAULTMTMSETTINGS_H__
#define __CMSVTECHNOLOGYTYPEDEFAULTMTMSETTINGS_H__

#include <e32base.h>
#include <e32cmn.h>

class RFs;
class CRepository;


/**
Stores default MTM settings for a technology type to the Central Repository.

@publishedAll
@released
*/
class CMsvTechnologyTypeDefaultMtmSettings : public CBase
	{
private:
	struct TMsvTechnologyTypeDefaultMtmInfo
		{
		TUid iTechnologyTypeUid;
		TUid iMtmUid;
		};

public:
	IMPORT_C static CMsvTechnologyTypeDefaultMtmSettings* NewL( RFs& aFs );
	IMPORT_C ~CMsvTechnologyTypeDefaultMtmSettings( );
	
	IMPORT_C TInt DefaultMtm( TUid aTechnologyTypeUid, TUid& aMtmUid ) const;
	IMPORT_C void SetDefaultMtmL( TUid aTechnologyTypeUid, TUid aMtmUid );
	IMPORT_C TInt RemoveDefaultMtm( TUid aTechnologyTypeUid );
	
	// Stores and Restores data in the central repository
	IMPORT_C void LoadSettingsL( );
	IMPORT_C void SaveSettingsL( );

private:
	CMsvTechnologyTypeDefaultMtmSettings( RFs& aFs );
	void ConstructL();
	void CheckIfMtmIsPartOfTechnologyTypeGroupL( const TUid aTechnologyTypeUid,
												 const TUid aMtmUid, const TUid aMtmTypeUid ) const;
	TInt FindDefaultMtm( const TUid aTechnologyTypeUid, 
						 TMsvTechnologyTypeDefaultMtmInfo& aTechnologyTypeDefaultMtmInfo ) const;

private:
	CRepository* iRepository;
	RArray<TMsvTechnologyTypeDefaultMtmInfo> iTechnologyTypeDefaultMtmArray;
	RArray<TUid> iRemovedTechnologyTypes;
	RFs& iFs;
	};
#endif //__CMSVTECHNOLOGYTYPEDEFAULTMTMSETTINGS_H__
