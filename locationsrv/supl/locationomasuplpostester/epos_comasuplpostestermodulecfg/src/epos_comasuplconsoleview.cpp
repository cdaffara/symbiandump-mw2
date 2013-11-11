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
* Description:  Controls implementation selection from console
*
*/

#include "epos_comasuplconsoleview.h"


/**  Determines the number of elements to be shown in one screen. */
const TInt KDefaultListSize = 10;

/**  Determines the length of display name of implementation to be shown in
	 one screen. */
const TInt KDisplaySize = 35;


// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplConsoleView* COMASuplConsoleView::NewL(CConsoleBase& aConsole)
	{
	COMASuplConsoleView* self = new(ELeave)COMASuplConsoleView(aConsole);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplConsoleView::~COMASuplConsoleView()
	{
	
	}

TInt COMASuplConsoleView::DisplayList(const RImplInfoPtrArray& aPluginList)
	{
	TInt selectedIndex = -9999;
	TInt startIndex = 0;
	while(selectedIndex < 0 || selectedIndex >= aPluginList.Count())
		{
		selectedIndex = GetPluginForTest(aPluginList, startIndex);
		switch(selectedIndex)
			{
			case -1://error
				return -1;
				
			case -2://page up
				{
				if(startIndex - KDefaultListSize < 0)
					{
					startIndex = 0;
					}
				else
					{
					startIndex = startIndex - KDefaultListSize;
					}
				break;
				}
				
			case -3://page down
				{
				if(startIndex + KDefaultListSize >= aPluginList.Count())
					{
					//do nothing
					}
				else
					{
					startIndex += KDefaultListSize;
					}
				break;
				}
			case -4://Testing aborted
				{
				return -4;
				}
				
			default://any valid value
				return selectedIndex;
			}
		}
	return 0;	
	}

COMASuplConsoleView::COMASuplConsoleView(CConsoleBase& aConsole)
									:iConsole(aConsole)
	{
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplConsoleView::ConstructL()
	{
	}

void COMASuplConsoleView::ProcessDisplayName(const TDesC& aDisplayName
										, TDes& aDisplay)
	{
	if(aDisplayName.Length() <= KDisplaySize)
		{
		aDisplay.Copy(aDisplayName);
		}
	else
		{
		aDisplay.Copy(TPtrC(aDisplayName.Ptr(), KDisplaySize));
		}
	}

TInt COMASuplConsoleView::GetPluginForTest(const RImplInfoPtrArray& aPluginList,
										TInt aStartIndex)
	{
	TBuf<KDisplaySize> display;
	TBuf<KDisplaySize> toBeDispalyed;
	iConsole.ClearScreen();
	TInt PluginsDetected = aPluginList.Count();
	if(PluginsDetected > 1)
		{
		iConsole.Write(_L("*"));
		}
	for(TInt count = aStartIndex; count < KDefaultListSize + aStartIndex 
									&& count < PluginsDetected; count++)
		{
		ProcessDisplayName((aPluginList[count])->DisplayName(), display);		
		iConsole.Write(display);
		iConsole.Write(_L("\r\n"));
		}
	iConsole.SetPos(0,0);
	TChar input(TUint(iConsole.Getch()));
	TInt index = 0;
	while(input != EKeyRightArrow)
		{
		TInt yPos = iConsole.WhereY();
		switch(input)
			{
			case EKeyUpArrow:
					{
					input = 0;
					if(yPos - 1 < 0)
						{
						input = 0;
						return -2;
						}
					iConsole.ClearToEndOfLine();
					ProcessDisplayName((aPluginList[index + aStartIndex])->DisplayName(), toBeDispalyed);
					iConsole.Write(TPtrC(toBeDispalyed));
					index--;
					iConsole.SetPos(0, index);
					iConsole.Write(_L("*"));
					ProcessDisplayName((aPluginList[index + aStartIndex])->DisplayName(), toBeDispalyed);
					iConsole.Write(TPtrC(toBeDispalyed));
					iConsole.SetPos(0, index);
					break;
					}
			case EKeyDownArrow:
					{
					input = 0;
					if(yPos + 1 >=  KDefaultListSize || yPos + 1 >= PluginsDetected - aStartIndex)
						{
						input = 0;
						return -3;
						}
					iConsole.ClearToEndOfLine();
					ProcessDisplayName((aPluginList[index + aStartIndex])->DisplayName(), toBeDispalyed);
					iConsole.Write(TPtrC(toBeDispalyed));
					index++;
					iConsole.SetPos(0, index);
					iConsole.Write(_L("*"));
					ProcessDisplayName((aPluginList[index + aStartIndex])->DisplayName(), toBeDispalyed);
					iConsole.Write(TPtrC(toBeDispalyed));
					iConsole.SetPos(0, index);
					break;
					}
			case EKeyRightArrow:
					{
					return (index + aStartIndex);
					}
			case EKeyPageUp:
					{
					input = 0;
					return -2;
					}
					
			case EKeyPageDown:
					{
					input = 0;
					return -3;
					}
			case EKeyBackspace:
					{
					return -4;
					}
			default:break;
			
			}
		input = 0;

		input =+ TUint(iConsole.Getch());
		if(input == EKeyRightArrow)
			{
			return (index + aStartIndex);
			}
		
		}
	return aStartIndex;	
	}
