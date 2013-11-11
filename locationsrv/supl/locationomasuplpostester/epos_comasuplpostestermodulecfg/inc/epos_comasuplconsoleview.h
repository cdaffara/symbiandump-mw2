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
* Description:  Displays the list of all implementations.
*
*/

#ifndef C_COMASUPLCONSOLEVIEW_H
#define C_COMASUPLCONSOLEVIEW_H

#include <e32base.h>
#include <e32cons.h>
#include <ecom/implementationinformation.h>



/**
 *  Displays the Display name of implementations in the array.
 *
 *  Displays the Display name of implementations in the array. Use right
 *  arrow key for selecting an implementation, up/down arrow for moving 
 *  on the page and Page up/down for moving one page up/down.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class COMASuplConsoleView : public CBase
	{
public:
	static COMASuplConsoleView* NewL(CConsoleBase& aConsole);
	virtual ~COMASuplConsoleView();
	
	/**
     * displays the list of implementation provided as argument.
     *
     * @since S60 v3.1
     * const RImplInfoPtrArray& aPluginList List to be displayed.
     * @return TInt Index of the implementation selected by the user.
     */
	TInt DisplayList(const RImplInfoPtrArray& aPluginList);

private:
	COMASuplConsoleView(CConsoleBase& aConsole);
	void ConstructL();
	
	/**
     * Returns a first KDisplaySize characters of the display name.
     *
     * @since S60 v3.1
     * @param aDisplayName Complete display name as in the 
     * implementation in the list.
     * @param aDisplay Left most part of the display
     * name of length KDisplaySize or display name length, whichever less.
     * @return void
     */
	void ProcessDisplayName(const TDesC& aDisplayName, TDes& aDisplay);
	
	/**
     * Controls the display of list on the console.
     *
     * @since S60 v3.1
     * @param aPluginList List of implementations.
     * @param aStartIndex Index from which list is to be displayed.
     * @return TInt Either the selected index or indication to display 
     * next or previous page.
     */
	TInt GetPluginForTest(const RImplInfoPtrArray& aPluginList, TInt aStartIndex);
	
private:
	/**
     * Reference to the console provided while creating object.
     */
	CConsoleBase& iConsole;
	};
#endif