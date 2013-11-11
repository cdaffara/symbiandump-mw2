// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MsgUrlHandlerAppView.cpp
// This file contains the implementation for the class defined in 
// TestMsgUrlHandlerApp.h
// 
//

/**
 @file
 @see	TestMsgUrlHandlerApp.h
*/

#include "TestMsgUrlHandlerApp.H"

//
//             Constructor for the view.
//
CTestMsgUrlHandlerAppView::CTestMsgUrlHandlerAppView()
	{
	}


//             Static NewL() function to start the standard two
//             phase construction.
//
CTestMsgUrlHandlerAppView* CTestMsgUrlHandlerAppView::NewL(const TRect& aRect)
	{
	CTestMsgUrlHandlerAppView* self = new(ELeave) CTestMsgUrlHandlerAppView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}


//
//             Destructor for the view.
//
CTestMsgUrlHandlerAppView::~CTestMsgUrlHandlerAppView()
	{
	delete iText;
	}


//             Second phase construction.
//
void CTestMsgUrlHandlerAppView::ConstructL(const TRect& aRect)
    {
			   // Fetch the text from the resource file.
	iText = iEikonEnv->AllocReadResourceL(R_TEXT);
	           // Control is a window owning control
	CreateWindowL();
	           // Extent of the control. This is
	           // the whole rectangle available to application.
	           // The rectangle is passed to us from the application UI.
	SetRect(aRect);
			   // At this stage, the control is ready to draw so
	           // we tell the UI framework by activating it.
	ActivateL();
	}


//             Drawing the view - in this application, 
//             consists of drawing a simple outline rectangle
//             and then drawing the text in the middle.
//             We use the Normal font supplied by the UI.

void CTestMsgUrlHandlerAppView::Draw(const TRect& /*aRect*/) const
	{
               // Window graphics context
	CWindowGc& gc = SystemGc();
	           // Area in which we shall draw
	TRect      drawRect = Rect();
			   // Font used for drawing text
	const CFont*     fontUsed;
	
	           // Start with a clear screen
	gc.Clear();
			   // Draw an outline rectangle (the default pen
	           // and brush styles ensure this) slightly
	           // smaller than the drawing area.
	drawRect.Shrink(10,10);		   	
	gc.DrawRect(drawRect);
               // Use the title font supplied by the UI
	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);
			   // Draw the text in the middle of the rectangle.
	TInt   baselineOffset=(drawRect.Height() - fontUsed->HeightInPixels())/2; 
	gc.DrawText(*iText,drawRect,baselineOffset,CGraphicsContext::ECenter, 0);
               // Finished using the font
	gc.DiscardFont();
	}



