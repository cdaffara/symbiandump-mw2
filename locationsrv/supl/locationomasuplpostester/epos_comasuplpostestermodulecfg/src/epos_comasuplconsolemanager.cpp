/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manages console.
*
*/

#include "epos_comasuplconsolemanager.h"
#include "epos_comasuplconsoleview.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
 COMASuplConsoleManager* COMASuplConsoleManager::NewL(
									const RImplInfoPtrArray& aImplInfoArray)
	{
	COMASuplConsoleManager* self = new(ELeave)COMASuplConsoleManager(aImplInfoArray);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
 COMASuplConsoleManager::~COMASuplConsoleManager()
	{
	if(NULL != iConsole)
		{
		iConsole->ClearScreen();
		}
	
	delete iConsole;
	delete iConsoleView;
	}

 TInt COMASuplConsoleManager::GetUserSelection()
	{
	if(0 == iImplementationList.Count())
		{
		//There is no implementation for Interface
		iSelectedIndex = -1;
		}
	else 
		{
		if(1 == iImplementationList.Count())
			{
			iSelectedIndex = 0;
			}
		else
			{			
			iSelectedIndex = iConsoleView->DisplayList(iImplementationList);
			}	
		}
	return iSelectedIndex;
	}

TInt COMASuplConsoleManager::GetSelectedIndex()
	{
	return iSelectedIndex;
	}

CConsoleBase& COMASuplConsoleManager::GetConsole()
 	{
 	return *iConsole;
 	}
 
COMASuplConsoleManager::COMASuplConsoleManager(const RImplInfoPtrArray& aImplInfoArray)
					:iImplementationList(aImplInfoArray)
	{
	iSelectedIndex = -1;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplConsoleManager::ConstructL()
	{
	iConsole = Console::NewL(KConsoleTitle, TSize(KConsFullScreen, KConsFullScreen));
	iConsoleView = COMASuplConsoleView::NewL(*iConsole);	
	}
