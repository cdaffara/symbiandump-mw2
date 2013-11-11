/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  P&S key for harvester server pause
*
*/

#ifndef HARVESTERPAUSEPS_H_
#define HARVESTERPAUSEPS_H_

const TUid KPsHarvesterPauseCategory = { 0x200009F5 };

// observed by harvester server
const TUint KPsHarvesterPauseKey            = 0x00000001;

enum TPsHarvesterPauseState
	{
	EPsHarvesterPauseResume = 0,  // Resume harvesting
	EPsHarvesterPausePause        // Pause harvesting
	};

#endif /*HARVESTERPAUSEPS_H_*/

