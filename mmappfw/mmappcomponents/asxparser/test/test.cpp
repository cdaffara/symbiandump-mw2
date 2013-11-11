/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Simple v2/v3 ASX-fileparser
*
*/

// Version : %version: 9 %

#include <e32base.h>
#include <e32cons.h>
#include <AsxParser.h>

#include <e32svr.h>
#include <e32def.h>

#include <utf.h>

void PrintUrl( TPtrC8 aUrl )
{
    HBufC16* urlBuffer = HBufC16::NewL(100);
    TPtr16   url = urlBuffer->Des();

    CnvUtfConverter::ConvertToUnicodeFromUtf8(url,aUrl);
    RDebug::Print(_L("#MP# TestAsx url=%S"),&url);

    delete urlBuffer;
}


void DoTest( CConsoleBase* /*aConsole*/ )
{
    CAsxParser* parser;

    RDebug::Print(_L("#MP# TestAsx Test 1"));
    {
        _LIT(KFile,"c:\\test.asx");
        parser = CAsxParser::NewL(KFile);
        TUint i = 0;
        parser->GetUrlCount(i);
        __ASSERT_DEBUG(i == 2,1);
        TPtrC8 url;
        parser->GetUrl(1,url);
        PrintUrl(url);
        parser->GetUrl(2,url);
        PrintUrl(url);
        delete parser;
    }

    RDebug::Print(_L("#MP# TestAsx Test 2"));
    {
        _LIT(KFile,"c:\\test2.asx");
        parser = CAsxParser::NewL(KFile);
        TUint i = 0;
        parser->GetUrlCount(i);
        __ASSERT_DEBUG(i == 2,1);
        TPtrC8 url;
        TPtrC16 url2;
        parser->GetUrl(1,url);
        PrintUrl(url);
        parser->GetUrl(2,url);
        PrintUrl(url);
        delete parser;
    }

    RDebug::Print(_L("#MP# TestAsx Test 3"));
    {
        _LIT(KFile,"c:\\Corrupttest.asx");
        parser = CAsxParser::NewL(KFile);
        TUint i = 0;
        parser->GetUrlCount(i);
        __ASSERT_DEBUG(i == 0,1);
        TPtrC8 url;
        TPtrC16 url2;
        parser->GetUrl(1,url);
        PrintUrl(url);
        delete parser;
    }

    RDebug::Print(_L("#MP# TestAsx Test 4"));
    {
        _LIT(KFile,"c:\\Corrupttest2.asx");
        parser = CAsxParser::NewL(KFile);
        TUint i = 0;
        parser->GetUrlCount(i);
        __ASSERT_DEBUG(i == 1,1);
        TPtrC8 url;
        TPtrC16 url2;
        parser->GetUrl(1,url);
        PrintUrl(url);
        delete parser;
    }
    
    RDebug::Print(_L("#MP# TestAsx Test 5"));
    {
        _LIT(KFile,"c:\\multiURL_v3.asx");
        parser = CAsxParser::NewL(KFile);
        TUint i = 0;
        parser->GetUrlCount(i);
        __ASSERT_DEBUG(i == 1,1);
        TPtrC8 url;
        TPtrC16 url2;
        parser->GetUrl(1,url);
        PrintUrl(url);
        delete parser;
    }
}

void DoExampleL()
{
    CConsoleBase* console;

    //
    //  Make the console and push it on the cleanup stack.
    //
    console = Console::NewL(_L("Console"), TSize( KConsFullScreen, KConsFullScreen));
    CleanupStack::PushL(console);
    DoTest(console);
    CleanupStack::PopAndDestroy(console);
}

TInt E32Main()
{
    __UHEAP_MARK;
    //Create a cleanup stack
    CTrapCleanup* cleanup = CTrapCleanup::New();
    //Call some Leaving methods inside TRAP
    TRAPD(error, DoExampleL());
    __ASSERT_ALWAYS(!error, User::Panic(_L("Hello"), error));
    //Destroy cleanup stack
    delete cleanup;
    __UHEAP_MARKEND;
    return 0;
}
