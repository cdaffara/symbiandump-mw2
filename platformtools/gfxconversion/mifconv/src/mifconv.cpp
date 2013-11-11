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
* Description:  Mifconv main.
*
*/


#include "mifconv.h"
#include "mifconv_exception.h"
#include "mifconv_argumentmanager.h"
#include "mifconv_convertermanager.h"
#include "mifconv_util.h"

int main( int argc, char *argv[] )
{    
    cout << endl << "Mifconv v" << MifConvVersion << " - " << MifConvDate << endl;
	// Instantiate the argument manager and mif creator:
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    MifConvConverterManager* converterMgr = MifConvConverterManager::Instance();

	try {
		// Read arguments to string vector:
		MifConvStringList strList;
		for( int i = 1; i < argc; ++i )
		{            
			strList.push_back(MifConvString(argv[i]));
		}

		// Initialize arguments:
		argMgr->Init(strList);

        // Initialize converters:
        converterMgr->Init();

		// Run converters for source files:
		converterMgr->ProcessIcons();
		
		// Cleanup temp files etc:
		converterMgr->Cleanup();
	}
	catch( MifConvException& e ) {
		// In case of error, print exception to stderr, cleanup temp files and exit:
        std::stringstream strForInt;
        strForInt << e.Line();

        MifConvUtil::DebugLog(e.File() + MifConvString(":") + strForInt.str() + MifConvString(":") + e.String());
		cerr << e.String() << endl;
		converterMgr->Cleanup(true);
        converterMgr->Reset();
        argMgr->Reset();
		exit(MIFCONV_ERROR);
	}
	catch(...)
	{
		// Unknown error:
        MifConvUtil::DebugLog("MifConv: Unknown error");
		cerr << "MifConv: Unknown error" << endl;
		converterMgr->Cleanup(true);
        converterMgr->Reset();
        argMgr->Reset();
		exit(MIFCONV_ERROR);
	}
    
    converterMgr->Reset();
    argMgr->Reset();
	return MIFCONV_NO_ERROR;
}
