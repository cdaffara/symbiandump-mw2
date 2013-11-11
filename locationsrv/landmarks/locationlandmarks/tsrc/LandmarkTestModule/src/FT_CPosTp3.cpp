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

#include "FT_CPosTp3.h"
#include <EPos_CPosLandmarkDatabase.h>
  
     
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp3::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp3::StartL()
    {
        
    _LIT(KDbUri5, "http://c:\\system\\data\\eposlm.ldb");
       
    _LIT(KHttpErr, "http is not a supported protocol");
    
     RemoveDefaultDbL();
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    delete lmd;

    
    TInt err=0;
    TRAP(err, TestOpenDbL(KDbUri5));
    AssertTrueSecL(err == KErrNotSupported, KHttpErr);

	}

// ---------------------------------------------------------
// CPosTp3::TestOPenDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp3::TestOpenDbL(const TDesC& aDbURI)
    {
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(aDbURI);
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
 
    CleanupStack::PopAndDestroy();  
    }

//  End of File
