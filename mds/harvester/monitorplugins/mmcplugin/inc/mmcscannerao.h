/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Scans MMC after phone reboot for file changes*
*/

#ifndef CMMCSCANNERAO_H
#define CMMCSCANNERAO_H

#include <e32base.h>	// For CActive, link against: euser.lib

#include <e32std.h>		// For RTimer, link against: euser.lib
#include <harvesterdata.h>
#include "mdeharvestersession.h"
#include "mmcfilelist.h"
#include "harvestereventmanager.h"

// FORWARD DECLARATION
class MMonitorPluginObserver;

class CMmcScannerAO : public CActive 
	{
public:
	// Cancel and destroy
	~CMmcScannerAO();

	// Two-phased constructor.
	static CMmcScannerAO* NewL( TUint32 aMediaId, CMdEHarvesterSession* aMdEClient,
			MMonitorPluginObserver* aObserver, CHarvesterPluginFactory* aHarvesterPluginFactory,
			const TInt aPriority );

public:

private:
	// C++ constructor
	CMmcScannerAO( TUint32 aMediaId, CMdEHarvesterSession* aMdEClient, 
			MMonitorPluginObserver* aObserver, CHarvesterPluginFactory* aHarvesterPluginFactory,
			const TInt aPriority );

	// Second-phase constructor
	void ConstructL();

private:
	enum TCMmcScannerAOState
		{
		EUninitialized, // Uninitialized
		EReadFiles,
		EProcessFiles,
		EHarvestFiles,
		ERemoveNPFiles,
		EDone,
		EError // Error condition
		};

private:
	// From CActive
	// Handle completion
	void RunL();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError( TInt aError );
	
	void HandleReharvestL();
	
	void SetState( TCMmcScannerAOState aState );		

    /**
    * From CActive
    */
    void DoCancel();
	
private:
	TInt iState; // State of the active object
	
	TUint32 iMediaId;
	
	// Not owned
	CMdEHarvesterSession* iMdEClient;
	
	RFs iFs;
	
	TBuf<2> iDrive;
	
	CMmcFileList* iMmcFileList;
	
	// Not owned
	CHarvesterPluginFactory* iHarvesterPluginFactory;
	
	RPointerArray<CPlaceholderData> iEntryArray;
	RPointerArray<CPlaceholderData> iHarvestEntryArray;
	
	// Not owned
	MMonitorPluginObserver* iObserver;
	
	RPointerArray<CHarvesterData> iHdArray;
	
    CHarvesterEventManager* iHEM;
    
    RTimer iTimer;
	};

#endif // CMMCSCANNERAO_H

