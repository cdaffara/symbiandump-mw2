// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __IMPMACRO_H__
#define __IMPMACRO_H__

/*

Macros to support migration from 8.1 - 9.0 - 9.1 for the IACP parser.

Due to the fact that this parser is being migrated from server-side MTM to client-side MTM,
any use of the CMsvServerEntry defined in CBaseScriptParser, needs to be changed to use 
CMsvEntry, now defined in CBaseScriptParser2. Unfortunately, CMsvServerEntry member functions
are not leaving functions, but CMsvEntry's are. To support these changes with the minimum of 
#ifdef'ing in the original code, the following macros are used.

*/


#define CMSVENTRY CMsvEntry

#define SETENTRYL(s) 		aEntry.SetEntryL(s)
#define CHANGEENTRYL(s) 	aEntry.ChangeL(s)
#define SETSORT(s) 			TRAPD(ignor,aEntry.SetSortTypeL(s))
#define CREATEENTRYL(s) 	aEntry.CreateL(s)
#define DELETEENTRYL(s) 	aEntry.DeleteL(s)



#endif // __IMPMACRO_H__
