/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Resolves sending service information and stores it to 
*                Central Repository.    
*
*/



#include "senduisrapp.h"
#include "senduisrdoc.h"

static const TUid KUidSendUiSrApp = {0x102823b5};

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CApaDocument* CSendUiSrApp::CreateDocumentL()
    {  
    // Create an SendUiSr document, and return a pointer to it
    CApaDocument* document = CSendUiSrDocument::NewL(*this);
    return document;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TUid CSendUiSrApp::AppDllUid() const
    {
    // Return the UID for the SendUiSr application
    return KUidSendUiSrApp;
    }
