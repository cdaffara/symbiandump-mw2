/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/


//  INCLUDES

#include "FT_CPosTp2.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <ecom.h>    
          
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp2::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp2::StartL()
    {
    _LIT(KMoveDllErr, "Move of the Local Acces Provider dll failed with %d");
    _LIT(KMoveRscErr, "Move of the Local Acces Provider rsc failed with %d");
    _LIT(KLeaveErr, "CPosLandmarkDatabase::OpenL() doesn't leave with KErrNotSupported");
    _LIT(KOpenErr, "CPosLandmarkDatabase::OpenL() failed with %d");
    
    _LIT(KMoveToPath, "c:\\system\\test\\");
    _LIT(KPluginMovedToPathDll, "c:\\system\\test\\eposlmlocalaccessprovider.dll");
    _LIT(KPluginMovedToPathRsc, "c:\\system\\test\\eposlmlocalaccessprovider.RSC");

    /*
    // Before
    //_LIT(KPluginMovedToPathRsc, "c:\\system\\test\\101FDF89.RSC");   
        
    _LIT(KPluginsPath, "c:\\system\\libs\\plugins\\");
    _LIT(KDll, "c:\\system\\libs\\plugins\\eposlmlocalaccessprovider.dll");
    _LIT(KRsc,"c:\\system\\libs\\plugins\\101FDF89.RSC");
    */
    
    _LIT(KPluginsPath, "c:\\sys\\bin\\");
    _LIT(KPluginsResourcePath, "c:\\resource\\plugins\\");
    _LIT(KDll, "c:\\sys\\bin\\eposlmlocalaccessprovider.dll");
    _LIT(KRsc,"c:\\resource\\plugins\\eposlmlocalaccessprovider.RSC");

//#ifdef __WINS__
//    _LIT(KCanceledErr, "Test canceled");
//    _LIT(KMoveDll, "Move Epoc32\\release\\winscw\\udeb\\eposlmlocalaccessprovider.dll to c:\\sys\\bin\\ \
//    and z:\\resource\\plugins\\eposlmlocalaccessprovider.rsc to c:\\resource\\plugins, press OK when done.");
//    TUtfwUserAnswer answer;
//    answer = iUserInfo->ShowDialog(KMoveDll, EUtfwDialogTypeOkCancel, EFalse);
//    AssertTrueSecL(answer == EUtfwUserAnswerOk, KCanceledErr, KErrCancel);
//#endif

    CPosLandmarkDatabase* lmd=CPosLandmarkDatabase::OpenL();
    delete lmd;
    
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    REComSession ecom = REComSession::OpenL();
    CleanupClosePushL(ecom);
    
    TRequestStatus status;
   
    TInt err;
    err = fileMan->Move(KDll, KMoveToPath, CFileMan::EOverWrite);
    AssertTrueSecL(err == KErrNone, KMoveDllErr, err);
    err = fileMan->Move(KRsc, KMoveToPath, CFileMan::EOverWrite);
    AssertTrueSecL(err == KErrNone, KMoveRscErr, err);

    ecom.NotifyOnChange(status);
    User::WaitForRequest(status); // Wait for Ecom

    CPosLandmarkDatabase* lmd2=NULL;
    TRAP(err, lmd2=CPosLandmarkDatabase::OpenL());
    delete lmd2;
    if (err)
        {
        AssertTrueSecL(err == KErrNotFound || err == KErrNotSupported, KLeaveErr, err);
        }
    else
        {
        AssertTrueSecL(err == KErrNotFound || err == KErrNotSupported, KLeaveErr);
        }

    err = fileMan->Move(KPluginMovedToPathDll, KPluginsPath, CFileMan::EOverWrite);
    AssertTrueSecL(err == KErrNone, KMoveDllErr, err);
    err = fileMan->Move(KPluginMovedToPathRsc, KPluginsResourcePath, CFileMan::EOverWrite);
    AssertTrueSecL(err == KErrNone, KMoveRscErr, err);

    ecom.NotifyOnChange(status);
    User::WaitForRequest(status); // Wait for Ecom
    
    CPosLandmarkDatabase* lmd3=NULL;
    TRAP(err, lmd3 = CPosLandmarkDatabase::OpenL());
    delete lmd3;
    AssertTrueSecL(err == KErrNone, KOpenErr, err);
    
    CleanupStack::PopAndDestroy(3, &fs);
    
//    #ifdef __WINS__
//    _LIT(KRestoreTest, "Restore c:\\sys\\bin\\eposlmlocalaccessprovider.dll to  Epoc32\\release\\winscw\\udeb\\ and \
//    c:\\resource\\plugins\\eposlmlocalaccessprovider.rsc to  z:\\resource\\plugins ");
//    answer = iUserInfo->ShowDialog(KRestoreTest, EUtfwDialogTypeOk, EFalse);
//    #endif

	}

// ---------------------------------------------------------
// CPosTp2::WaitForEcomL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp2::WaitForEcomL()
	{
	REComSession ecom = REComSession::OpenL();
    CleanupClosePushL(ecom);
    TRequestStatus status;
    ecom.NotifyOnChange(status);
    User::WaitForRequest(status);
    CleanupStack::PopAndDestroy(&ecom);
	}
	
//  End of File
