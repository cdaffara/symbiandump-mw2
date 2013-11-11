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

#include "FT_CPosTp17.h"
#include <f32file.h>
#include <bautils.h>

// CONSTANTS
const TInt KMaxDllFileNameLength = 100;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp17::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp17::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP17 - Memory footprint");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp17::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp17::CloseTest()
    {
    iFootprints.Close();
    iDllNames.Close();
    }

// ---------------------------------------------------------
// CPosTp17::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp17::StartL()
    {
#ifdef __WINS__
    _LIT(KNotSupported, "Test case is not supported on WINS, exucute it on target!");
    LogErrorAndLeave(KNotSupported);
#else
    _LIT(KErrors, "One or several Dll's have an incorrect size");
    SetupTestDataL();
 
    RFs session;
    User::LeaveIfError(session.Connect());
    CleanupClosePushL(session);
 
    RFile file;
    TBuf<1> drive= _L("c");
    TBuf<KMaxDllFileNameLength> name;
    name.Copy(iDllNames[0]);
    name.Replace(0,1,drive); 
 
    if (file.Open(session, name, EFileRead) != KErrNone)
        {
        iLog->Put(_L("(Flashed) Use Drive Z:"));
        drive.Copy(_L("z"));
        }
	else iLog->Put(_L("(Nonflashed) Use Drive C:"));
 
    file.Close();
    TBool errors=EFalse;
    for (TInt i=0; i<iDllNames.Count(); i++)
        {
        name.Copy(iDllNames[i]);
        name.Replace(0,1,drive);
        //User::LeaveIfError(file.Open(session, name, EFileRead));
        TInt err = file.Open(session, name, EFileRead);
        if (err != KErrNone)
            {
            TBuf<70> buffe;
            buffe.Append(_L("Error when opening file "));
            buffe.Append(name);
			LogErrorAndLeave(buffe, err);
            }

        CleanupClosePushL(file);
        TInt fileSize=0;
        file.Size(fileSize);
       
        if (fileSize > iFootprints[i])
            {
            errors = ETrue;
            
            _LIT(KDllSizeErr, "ERROR: %S is to big (%d), Max allowed ROM footprint is %d");
            TBuf<255> info;
            HBufC* temp = name.Alloc();
            info.Format(KDllSizeErr, temp, fileSize, iFootprints[i]);
            delete temp;
            iLog->Put(info);
            }
        else 
            {
			// Always log .dll size
            _LIT(KDllSizeDebug, "%S is (%d), Max allowed ROM footprint is %d");
            TBuf<255> info;
            HBufC* temp = name.Alloc();
            info.Format(KDllSizeDebug, temp, fileSize, iFootprints[i]);
            delete temp;
            iLog->Put(info);
            }
        
        CleanupStack::PopAndDestroy(&file);
        } 
    CleanupStack::PopAndDestroy(&session);

    if (errors)
        {
        LogErrorAndLeave(KErrors);
        }
#endif
    }

// ---------------------------------------------------------
// CPosTp17::SetupTestDataL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp17::SetupTestDataL()
    {  
    iLog->Put(_L("SetupTestDataL"));
    
    _LIT(KLANDMARK,"z:\\sys\\bin\\eposlandmarks.dll");
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    //check if landmark is flashed
     if (BaflUtils::FileExists(fs, KLANDMARK))
        {
         //Landmark is flashed
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlandmarks.dll")));        
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmsearchlib.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmmultidbsearch.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmlocalsearchprovider.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmlocalaccessprovider.dll"))); 
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmlocalaccess.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmservlib.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmserver.exe")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmdbreg.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmintservices.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmconverter.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmrecognizer.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmdbmanlib.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmdbmanprov.dll")));
        User::LeaveIfError(iDllNames.Append(_L("z:\\sys\\bin\\eposlmlocaldbmanprovider.dll")));
        }
     else
        {
         //Landmark is not flashed
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlandmarks.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmsearchlib.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmmultidbsearch.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmlocalsearchprovider.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmlocalaccessprovider.dll"))); 
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmlocalaccess.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmservlib.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmserver.exe")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmdbreg.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmintservices.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmconverter.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmrecognizer.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmdbmanlib.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmdbmanprov.dll")));
        User::LeaveIfError(iDllNames.Append(_L("!:\\sys\\bin\\eposlmlocaldbmanprovider.dll")));
        }

    User::LeaveIfError(iFootprints.Append(9000));  //eposlandmarks.dll (Landmarks Client Library)
    // Note that eposlmmultidbsearch.dll + eposlmsearchlib.dll must not be bigger than 17000
    User::LeaveIfError(iFootprints.Append(8500)); //eposlmsearchlib.dll (Landmarks Search Client Library)
    User::LeaveIfError(iFootprints.Append(8500)); //eposlmmultidbsearch.dll (Landmarks Search Client Library)
    User::LeaveIfError(iFootprints.Append(19000)); //eposlmlocalsearchprovider.dll (Landmarks Local Search Provider)
    // Note that eposlmlocalaccessprovider.dll + eposlmlocalaccess.dll must not be bigger than 23000
    User::LeaveIfError(iFootprints.Append(400));   //eposlmlocalaccessprovider.dll (Landmarks Local Access Provider)
    User::LeaveIfError(iFootprints.Append(23000)); //eposlmlocalaccess.dll (Landmarks Local Access)
    User::LeaveIfError(iFootprints.Append(2000));  //eposlmservlib.dll (Landmarks Server Client Library)
    User::LeaveIfError(iFootprints.Append(26000)); //eposlmserver.exe (Landmarks Server)
    User::LeaveIfError(iFootprints.Append(6000));  //eposlmdbreg.dll (Landmarks Database Registry)
    User::LeaveIfError(iFootprints.Append(23000)); //eposlmintservices.dll (Landmarks Internal Services)
    User::LeaveIfError(iFootprints.Append(22000)); //eposlmconverter,dll (Landmarks Content Format Converter)
    User::LeaveIfError(iFootprints.Append(2000));  //eposlmrecognizer.mdl (Landmarks Content Format Recognizer)
    User::LeaveIfError(iFootprints.Append(3000));  //eposlmdbmanlib.dll (Landmarks DB Management Client)
    User::LeaveIfError(iFootprints.Append(6000));  //eposlmdbmanprov.dll (Landmarks Database Management Provider)
    User::LeaveIfError(iFootprints.Append(5000));  //eposlmlocaldbmanprovider.dll (Landmarks Local Database Management Provider)

    CleanupStack::PopAndDestroy(2, &fs);
    }

//  End of File
