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

#ifndef C_COMASUPLCONSOLEMANAGER_H
#define C_COMASUPLCONSOLEMANAGER_H

#include <e32base.h>
#include <e32cons.h>
#include <ecom/implementationinformation.h>

class COMASuplConsoleView;
class MPosEventObserver;
class CPosTesterModule;

/**  Title of the console. */
_LIT(KConsoleTitle, "UserInterface");

/**
 *  Manages console and user input for selecting any implementation.
 *
 *  Provides console for displaying implementations and monitors user input
 *  for selecting any implementation. If there is only one implementation 
 *  then it is considered selected.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class COMASuplConsoleManager : public CBase
	{
public:
	 static COMASuplConsoleManager* NewL(const RImplInfoPtrArray& aImplInfoArray);
	 virtual ~COMASuplConsoleManager();
	
	/**
     * Identifies the implementation selected by the user from console.
     *
     * @since S60 v3.1
     * @return TInt Index of the implementation selected. 
     */
	 TInt GetUserSelection();
	 
	/**
     * Index of the implementation selected.
     *
     * @since S60 v3.1
     * @return TInt Index of the implementation selected.
     */
	 TInt GetSelectedIndex();

	/**
     * Returns the console which is used for displaying an monitoring
     * user input.
     *
     * @since S60 v3.1
     * @return CConsoleBase& Reference to the console.
     */
	 CConsoleBase& GetConsole();

private:
	COMASuplConsoleManager(const RImplInfoPtrArray& aImplInfoArray);
						
	void ConstructL();

private:
    /**
     * Points to the console.
     * Own.
     */
	CConsoleBase* iConsole;

    /**
     * Points to the console view which displays list of implementations.
     * Own.
     */
	COMASuplConsoleView* iConsoleView;

    /**
     * Reference to an array of implementations.
     */
	const RImplInfoPtrArray& iImplementationList;

    /**
     * Index selected by the user.
     */
	TInt iSelectedIndex;
	};
#endif