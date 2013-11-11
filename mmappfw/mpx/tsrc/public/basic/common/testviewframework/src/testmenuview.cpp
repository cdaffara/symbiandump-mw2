/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  part of testviewframework.
*
*/

// INCLUDE FILES
#include <e32base.h>
#include "testbaseview.h"
#include "consolemain.h"
#include "testmenuview.h"

// CONSTANTS
const TInt KMenuOverhead = 4;              // overhead
const TInt KMaxLineLength = 80;            // Longest supported line length
const TInt KArrGranularity = 4;            // DesC array granularity

// LOCAL FUNCTION PROTOTYPES
LOCAL_C void LimitedAppend( TDes& aOriginal, const TDesC& aAppend);

// ==================== LOCAL FUNCTIONS =======================================

LOCAL_C void LimitedAppend( TDes& aOriginal, const TDesC& aAppend)
	{

	TInt spaceLeft = aOriginal.MaxLength() - aOriginal.Length();
    
	if (spaceLeft > aAppend.Length())
		{
		aOriginal.Append ( aAppend );
		}
	else
		{		
		aOriginal.Append ( aAppend.Left ( spaceLeft ) );
		}

	}

//
//=========================================================================== *
//                    Implementation for Class CTestBaseView                     *
//=========================================================================== *
//

// ---------------------------------------------------------------------------
//  Method: ~CTestMenuView
//  Description: Destructor
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C CTestMenuView::~CTestMenuView()
    {
    iItems->Reset();
    delete iItems;
    }
    
// ---------------------------------------------------------------------------
//  Method: DisplayViewL
//  Description: Prints the menu
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::DisplayViewL()
    {
    CTestBaseView::DisplayViewBase();
    
    TBuf<KMaxLineLength> line;

    // Clear display
    iConsole->ClearScreen();

    // Print menu title and header
    Print( Name() );
    
    TInt oldItemCount = iItemCount;
     
    iItemCount = ItemCount();

    // If first time in view, update start and end positions
    if( (iFirst == iLast) ||        // First time here..
        ( iLast >= iItemCount ) ||
        ( oldItemCount != iItemCount ) )      // View size changed
        {
        iLast = iItemCount - 1;
        iPosOnScreen = 0;

        // If "overflow", then adjust the end
        if (iLast > iScreenSize )
            {
            iLast = iScreenSize;
            }
        }
    
    // Print items
    for ( TInt i = iFirst; i <= iLast; i++ )
        {
        line.Zero();
        // Append text before line
        AppendBefore( i, line );
        // Get the menu line
		LimitedAppend ( line, ItemL(i) );
		// Print the line
        Print(line);
        }
    }

// -----------------------------------------------------------------------------
//  Method: SelectL
//  Description: Process keypresses in view. updates position
//  Parameters: TKeyCode aSelection       :in:      Key
//              TBool& aContinue          :out:     Has user pressed "Quit"
//  Return Values: None
// -----------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::SelectL( TKeyCode aSelection, TBool& aContinue )
    {
    iKey = aSelection;

//	iConsole->Printf(_L("%d"),iKey);
	
    switch ( (TInt) aSelection )
    {
    // SelectL item
    case EKeyEnter:
    case EKeyRightArrow:
        HandleRightKeyL();
        break;
    // Going back
    case EKeyLeftArrow:
        HandleLeftKeyL();
        break;
    // Number key
    case  92: // '1'
    case  97: // '2'
    case 100: // '3'
 //   case '2':
 //   case '3':
 //   case '4':
 //   case '5':
 //   case '6':
 //   case '7':
 //   case '8':
 //   case '9':
 //   case '0':
        HandleNumKeyL();
        break;
    // Go down
    case EKeyDownArrow:
        if ( iFirst + iPosOnScreen == iItemCount - 1 )
            {
            // If end of the list, go to beginning
            iLast = iLast - iFirst;
            iFirst = 0;
            iPosOnScreen = 0;
            }
        else 
            {
            if ( iPosOnScreen == iScreenSize )
                {
                // If in end of screen, update items
                if ( iLast != (iItemCount - 1) )
                    {
                    // If not end of the list, then update first and last
                    // but do not update position in screen.
                    iLast++;
                    iFirst++;
                    }
                }
            else
                {
                // Going down "in-screen", no need to update items
                iPosOnScreen++;
                }
            }
        break;
        
    // Go Up
    case EKeyUpArrow:
        if ( iFirst + iPosOnScreen == 0 )
            {
            // If in the beginning of the list

            if ( iItemCount <= iScreenSize ) 
                {
                // Wrap around when the list is not full
                iPosOnScreen = iItemCount-1;
                }
            else
                {
                // Wrap around when there are more items than
                // can be shown at once.            
                iPosOnScreen = iScreenSize;
                iLast = iItemCount-1;
                iFirst = iItemCount - iPosOnScreen - 1;
                }
            }
        else if ( iPosOnScreen == 0 )
            {
            // If not at the beginning of the list, then update first and
            // last  but do not update position in screen.
            if ( iFirst != 0 )
                {
                iLast--;
                iFirst--;
                }
            }
        else
            {
            // Going up "in-screen", no need to update items
            iPosOnScreen--;
            }
        
        break;

	// Additional keys
	case EKeyHome:
		iPosOnScreen = 0;
		iFirst = 0;
		iLast = iScreenSize;

		if (iLast > iItemCount-1 )
			{
			iLast = iItemCount-1;
			}
		break;

	case EKeyEnd:
		iPosOnScreen = iScreenSize;
		iLast = iItemCount-1;
		iFirst = iLast - iScreenSize;

		if (iFirst < 0)
			{
			iFirst = 0;
			iPosOnScreen = iLast-1;
			}
		break;

	case EKeyPageUp:
		iFirst = iFirst - iScreenSize - 1;
		iLast = iLast - iScreenSize - 1;

		if ( iFirst < 0 )
			{
			iFirst = 0;
			iPosOnScreen = 0;			
			iLast = iScreenSize;
			if (iLast > iItemCount-1 )
				{
				iLast = iItemCount-1;
				}
			}
		break;

	case EKeyPageDown:
		iFirst = iFirst + iScreenSize + 1;
		iLast = iLast + iScreenSize + 1;

		// Going too far
		if (iLast > iItemCount-1)
			{
			iLast = iItemCount-1;
			iFirst = iLast - iScreenSize;
			iPosOnScreen = iScreenSize;
			}

		// Ok, list is smaller than screen
		if (iFirst < 0 )
			{
			iFirst = 0;
			iLast = iItemCount-1;
			iPosOnScreen = iLast;
			}

		break;
    case EKeyEscape:
        aContinue = EFalse;
        CurrentViewDoneL();
        return;
    default:  // Bypass the keypress
        break;
    }
    // Continue normally and keep in the same menu
    aContinue = ETrue;
    }

// -----------------------------------------------------------------------------
//  Method: TimerUpdate
//  Description: TimerUpdate
//  Parameters: None
//  Return Values: None
// -----------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::TimerUpdate()
    {    
    TInt count = ItemCount();
	// If list is empty, do not scroll.
	if ( count < 1 )
		{
		return;
		}
    else if( iFirst + iPosOnScreen > count )
        {
        iFirst = iPosOnScreen = 0;
        }

	// If item selection is updated, then restart scrolling
	if (iPrevPos != iFirst + iPosOnScreen)
		{
		iPrevPos = iFirst + iPosOnScreen;
		iStart = 0;
		iDirection = 1;
		}

	// If menu item have not been changed after last timer, then
	// start scrolling	
	TName name(KNullDesC);
	TRAPD(err, name = ItemL(iFirst + iPosOnScreen));
	if(err != KErrNone)
	    return;
	if ( name.Length() > iSize.iWidth)
		{

		TInt y = iConsole->WhereY();
		TInt x = iConsole->WhereX();
		TBuf<80> iTmp;				

		iStart = iStart + iDirection;

		// "Right end"
		if ( iStart + iSize.iWidth > name.Length() +KMenuOverhead )
			{
			iStart--;
			iDirection = -1;
			}
		
		// "Left end"
		if ( iStart == -1 )
			{
			iStart++;
			iDirection = 1;
			}

		iTmp=_L(" *");
		LimitedAppend( iTmp, name.Mid ( iStart ) );
			
		iConsole->SetPos( 0, iPosOnScreen+1);		
		iConsole->Printf ( iTmp.Left( iSize.iWidth -2 )  );

		iConsole->SetPos(x,y);
		}    
    }

// ---------------------------------------------------------------------------
//  Method: CTestMenuView
//  Description: C++ default constructor
//  Parameters: CConsoleMain* aConsole  :in: Pointer to main console
//              CTestBaseView* aParent     :in: Parent View
//              const TDesC& aName      :in: Menu name
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C CTestMenuView::CTestMenuView(CConsoleMain* aConsoleMain,
                                   CTestBaseView* aParent,
                                   const TDesC& aName)
    : CTestBaseView(aConsoleMain, aParent, aName)
    {
	iDirection = 1;
	iPrevPos  = -1;		// Invalid starting value
    }
    
// ---------------------------------------------------------------------------
//  Method: ConstructL
//  Description: Second level constructor.
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::ConstructL()
    {
    iItems = new (ELeave) CDesCArrayFlat(KArrGranularity);
    
    // Get display size
    const TInt KOverHead = 5;
    iSize = iConsole->ScreenSize();
    iScreenSize = iSize.iHeight - KOverHead;   // Size available for menus
    
    InitializeViewL();
    }
    
// ---------------------------------------------------------------------------
//  Method: Print
//  Description: Prints one line text and changes to next line. If line is
//  too long, overhead is not printed..
//  Parameters: TDesC& aPrint   :in:    Text to be printed
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::Print( const TDesC& aPrint )
    {
    iConsole->Printf( aPrint.Left( iSize.iWidth - KMenuOverhead ) );
    iConsole->Printf(_L("\n"));
    }

// ---------------------------------------------------------------------------
//  Method: PrintMulti
//  Description: Prints text. If line is too long, it will be continued to
//  following lines.
//  Parameters: const TDesC& aPrint       :in:      Text to print
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::PrintMulti( const TDesC& aPrint )
    {
    // Get current line
    const TInt KMenuOverHead = 2;
    TInt y = iConsole->WhereY();

    const TInt KLineLen =iSize.iWidth - 4;
    TBuf<KMaxLineLength+1> oneLine;

    // Loop through the line
    for (TInt i = 0; i < aPrint.Length(); i++)
        {
        oneLine.Append( aPrint[i] );

        // Print one line
        if (oneLine.Length() == KLineLen )
            {
            oneLine.Append (_L("\n"));
            iConsole->Printf(oneLine);
            oneLine=_L("");
            y++;
            }
    
        // Prevent display scrolling
        if ( y == iScreenSize + KMenuOverHead )
            {
            oneLine=_L("");
            break;
            }
        }

    // Print last part if required
    if ( oneLine.Length() != 0 )
        {
        oneLine.Append (_L("\n"));
        iConsole->Printf(oneLine);
        }
    }
    
// ---------------------------------------------------------------------------
//  Method: AddItem
//  Description: Add new item to menu
//  Parameters: TDesC& aItem              :in:      descriptor to be added
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::AddItemL(const TDesC& aItem)
    {
    iItems->AppendL(aItem);
    }

// ---------------------------------------------------------------------------
//  Method: DeleteItem
//  Description: Delete an item from menu
//  Parameters: TInt    aIndex  :in:    position of item in Items
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::DeleteItem( TInt aIndex )
    {
    iItems->Delete(aIndex);
    iItems->Compress();
    }

// ---------------------------------------------------------------------------
//  Method: LastKeyPressed
//  Description: Returns last key pressed
//  Parameters: None
//  Return Values: TKeyCode     The last key pressed
// ---------------------------------------------------------------------------
//
EXPORT_C TKeyCode CTestMenuView::LastKeyPressed()
    {
    return iKey;
    }
// ---------------------------------------------------------------------------
//  Method: ItemsCreated
//  Description: Determine whether there are items added to iItems
//  Parameters: None
//  Return Values: TBool        Whether iItems contain content
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CTestMenuView::ItemsCreated()
    {
    return ( iItems->Count() != 0 );
    }

// ---------------------------------------------------------------------------
//  Method: CurrentIndex
//  Description: Get the position that the cursor is point to in the items
//  Parameters: None
//  Return Values: TInt     Position of cursor in items array
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CTestMenuView::CurrentIndex()
    {
    return iFirst + iPosOnScreen;
    }

// ---------------------------------------------------------------------------
//  Method: CurrentPosition
//  Description: Get the position that the cursor is point to on the screen
//  Parameters: None
//  Return Values: TInt     Position of cursor on screen
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CTestMenuView::CurrentPosition()
    {
    return iPosOnScreen;
    }

// ---------------------------------------------------------------------------
//  Method: Menu
//  Description: Get the item
//  Parameters: TInt aIndex     :in     index of the entry
//  Return Values: TDesC&       entry descriptor
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CTestMenuView::ItemL(TInt aIndex)
    {
    if(aIndex < 0 || iItemCount <= aIndex)
        {
        User::Leave(KErrArgument);
        }
    //return iItems->operator[](iFirst + aIndex);
    return iItems->operator[](aIndex);
    }

// -----------------------------------------------------------------------------
// Method: ItemCount
// Count the number of items in the current view
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CTestMenuView::ItemCount() const
    {
    return iItems->Count();
    }

// -----------------------------------------------------------------------------
// Method: ReadString
// Reads user input into the start of the descriptor aDes
// -----------------------------------------------------------------------------
//
EXPORT_C void CTestMenuView::ReadString( TDes& aDes )
    {
    iConsoleMain->ReadString(aDes);
    }
    
// -----------------------------------------------------------------------------
//  Method: AppendBefore
//  Description: Append text before line.
//  Parameters: TInt aLine  :in: line number 
//              TDes& aLine :in: line text
//  Return Values: None
// -----------------------------------------------------------------------------
//
void CTestMenuView::AppendBefore( TInt aLineNum, TDes& aLine )
    {
    if( ( aLine.MaxLength() - aLine.Length() ) < 2 )
        {
        return;
        }        
    
    // If printing active line, print the marker
    if ( aLineNum == iPosOnScreen + iFirst )
        {
        aLine.Append( _L(" *") );
        }
    else
        {
        aLine.Append( _L("  ") );
        }   
    }
