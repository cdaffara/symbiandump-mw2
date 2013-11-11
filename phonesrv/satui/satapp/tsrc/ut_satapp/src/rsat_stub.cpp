/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Stub implementations for RSatSession and RSatUiSession
*              (used in unit testing the SAT APP)
*
*
*/


#include <msatuiobserver.h>
#include <rsatuisession.h>
#include <rsatsession.h>
#include <msatuiadapter.h>

// adapter 
class CSatCCommandProcessor: public MSatUiAdapter {
    void MenuSelection(TInt aMenuItem, TBool aHelpRequested)
    {
    }

    void SessionTerminated(TInt8 aType)
    {
    }
};


// RSatSession

RSatSession::RSatSession()
{
}

void RSatSession::ConnectL()
{
}

// RSatUiSession

RSatUiSession::RSatUiSession()
{
}

void RSatUiSession::RegisterL(const RSatSession&,MSatUiObserver*)
{
    iCommandProcessor = new CSatCCommandProcessor();
}

void RSatUiSession::Close()
{
    delete iCommandProcessor;
}

MSatUiAdapter* RSatUiSession::Adapter() const
{
    return iCommandProcessor;
}



//End of file
