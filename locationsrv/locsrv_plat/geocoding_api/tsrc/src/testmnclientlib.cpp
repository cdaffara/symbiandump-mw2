/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test Module DLL to Geocoding API
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>


#include "testmnclientlib.h"
#include "testmngeocoder.h"


// -----------------------------------------------------------------------------
// Ctestmnclientlib::Ctestmnclientlib
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestMNClientLib::CTestMNClientLib( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// Ctestmnclientlib::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestMNClientLib::ConstructL()
    {
    iLog = CStifLogger::NewL( KtestmnclientlibLogPath, 
                          KtestmnclientlibLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
	
	Install(KProviderSisxPath);
	                          
	iTestMnGeocoder = CTestMnGeocoder::NewL(iLog);
    }

// -----------------------------------------------------------------------------
// Ctestmnclientlib::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestMNClientLib* CTestMNClientLib::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CTestMNClientLib* self = new (ELeave) CTestMNClientLib( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CTestMNClientLib::~CTestMNClientLib()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

// Dependency Provider Install
TInt CTestMNClientLib::Install( const TDesC& aPackagePath )
    {
    iLog->Log(_L("Installing [%S]..."), &aPackagePath );

    TInt err = KErrNone;
    SwiUI::RSWInstSilentLauncher installer;

	err = installer.Connect();
	if( KErrNone == err )
    	iLog->Log(_L("Connection to Installer Passed..."));

    SwiUI::TInstallOptionsPckg options;
    options().iUpgrade = SwiUI::EPolicyAllowed;
    options().iPackageInfo = SwiUI::EPolicyAllowed;
    options().iOverwrite = SwiUI::EPolicyAllowed;
    options().iKillApp = SwiUI::EPolicyAllowed;
    
    iLog->Log(_L("B4 Installation Passed..."));
    err = installer.SilentInstall( aPackagePath , options );
	if( KErrNone == err )
    	iLog->Log(_L("Installation Passed with %d..."),err);
	else
    	iLog->Log(_L("Installation Failed with %d..."),err);

    installer.Close();
    return err;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CTestMNClientLib::NewL( aTestModuleIf );

    }


//  End of File
