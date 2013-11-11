/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Helper class for unzipping KMZ file.
*
*/


#ifndef CPOSLMUNZIPUTIL_H
#define CPOSLMUNZIPUTIL_H

#include <zipfile.h>


// CLASS DECLARATION

/**
*  Helper class for unzipping KMZ file.
*/
class PosLmUnzipUtil
    {
    public: // New functions
    
    //
    static void DoUnzipL(const TDesC& aFileHandle);
    
    private:
    static void UnzipFileL(RFs& aFs, TFileName* aFileName);
    static void ExtractFileL(RFs& aFs, CZipFileMember* aMember, CZipFile* aZipFile, TFileName* aFileName);
    
    static TPtrC GetDirName(RFile& aFile);
     
    };
    
#endif


