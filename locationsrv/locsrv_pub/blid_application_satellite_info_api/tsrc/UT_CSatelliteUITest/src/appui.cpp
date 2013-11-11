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
* Description: This file contains the app ui implementaion
*
*/
 
//#include <internaleventdispatcher.h>
#include "appui.h"



// -----------------------------------------------------------------------------
// CAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAppUi::ConstructL()
{
    // Initialise app UI with no resource file.
    BaseConstructL( ENoAppResourceFile );

    // Hide S60 signal strength icons etc.
    StatusPane()->MakeVisible( EFalse );
}

// -----------------------------------------------------------------------------
// CAppUi::CAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CAppUi::CAppUi() : iEventReceiver( NULL )
{
  
}

// -----------------------------------------------------------------------------
// CAppUi::~CAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CAppUi::~CAppUi()
{
    
}

// -----------------------------------------------------------------------------
// CAppUi::HandleWsEventL
// Takes care of WsSwssion events
// -----------------------------------------------------------------------------
//
void CAppUi::HandleWsEventL( const TWsEvent &aEvent, CCoeControl *aDestination )
{
    CAknAppUi::HandleWsEventL( aEvent,aDestination );
}

void CAppUi::HandleForegroundEventL(TBool aForeground)
	{
	
	}

// -----------------------------------------------------------------------------
// CAppUi::HandleCommandL()
// Sets the event reciever
// -----------------------------------------------------------------------------
//
void CAppUi::SetEventReceiver( CEventReceiver& aEventReceiver )
{
    iEventReceiver = &aEventReceiver;
}
//end of file
