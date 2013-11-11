/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/

#ifndef HARVESTEREVENTENUM_H_
#define HARVESTEREVENTENUM_H_

enum HarvesterEventObserverType
	{
	EHEObserverTypePlaceholder = 1,  // Observing placeholder changes to normal object
	EHEObserverTypeMMC = 2,          // Observing MMC handling start and stop
	EHEObserverTypeOverall = 4       // Observing overall status of the harvesting
	};

enum HarvesterEventState
	{
	EHEStateStarted,                 // Harvesting starting
	EHEStateHarvesting,              // Harvesting on-going
	EHEStateFinished,                // Harvesting finished
	EHEStateUninitialized,           // Harvesting not initialized
	EHEStatePaused,                  // Harvesting paused
	EHEStateResumed                  // Harvesting resumed
	};

#endif /*HARVESTEREVENTENUM_H_*/
