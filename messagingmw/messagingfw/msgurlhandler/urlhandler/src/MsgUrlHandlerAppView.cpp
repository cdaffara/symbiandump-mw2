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
// This file contains the implementation for the class defined in 
// MsgUrlHandlerApp.h
// 
//

/**
 @file
 @see	MsgUrlHandlerApp.h
*/

#include "MsgUrlHandlerApp.h"


const TInt KSize = 10;

//
//             Constructor for the view.
//
CMsgUrlHandlerAppView::CMsgUrlHandlerAppView()
	{
	}


//             Static NewL() function to start the standard two
//             phase construction.
//
CMsgUrlHandlerAppView* CMsgUrlHandlerAppView::NewL(const TRect& aRect)
	{
	CMsgUrlHandlerAppView* self = new(ELeave) CMsgUrlHandlerAppView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}


//
//             Destructor for the view.
//
CMsgUrlHandlerAppView::~CMsgUrlHandlerAppView()
	{
	delete iText;
	}


//             Second phase construction.
//
void CMsgUrlHandlerAppView::ConstructL(const TRect& aRect)
    {
			   // Fetch the text from the resource file.
	iText = iEikonEnv->AllocReadResourceL(R_MYAPP_TEXT);
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

void CMsgUrlHandlerAppView::Draw(const TRect& /*aRect*/) const
	{
               // Window graphics context
	CWindowGc& gc = SystemGc();
	           // Area in which we shall draw
	TRect      drawRect = Rect();
	
	           // Start with a clear screen
	gc.Clear();
			   // Draw an outline rectangle (the default pen
	           // and brush styles ensure this) slightly
	           // smaller than the drawing area.
	drawRect.Shrink(KSize,KSize);		   	
	gc.DrawRect(drawRect);

				   // Font used for drawing text
	const CFont*     fontUsed;
               // Use the title font supplied by the UI
	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);
			   // Draw the text in the middle of the rectangle.
	TInt   baselineOffset=(drawRect.Height() - fontUsed->HeightInPixels())/2; 
	gc.DrawText(*iText,drawRect,baselineOffset,CGraphicsContext::ECenter, 0);
               // Finished using the font
	gc.DiscardFont();
	}



