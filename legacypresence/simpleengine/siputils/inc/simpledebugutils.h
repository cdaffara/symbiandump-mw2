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
* Description:    DEBUG utilities, for DEBUG version only
*
*/




#ifndef simpledebugutils_H
#define simpledebugutils_H

#include <e32std.h>
#include <s32file.h>

// FORWARD DECLARATION
class RFs;
class RFileReadStream;

#ifdef _DEBUG

//**********************************
// TSimpleLogger
//**********************************
class TSimpleLogger
    {
public:
    static void Log(TRefByValue<const TDesC> aFmt,...); //lint !e960
    static void Dump(  const TDesC8& aData, RFs& aFs, TInt aMode  );
    };

#endif // _DEBUG

//**********************************
// CSimpleSettingFile
// Constructor and destructor must be present in DEBUG version
// also in order to keep ordinal numbers identical between UDEB 
// and UREL versions, although this class is used in UDEB method only.
//**********************************
class CSimpleSettingFile: public CBase
    {
public:

    virtual ~CSimpleSettingFile();

#ifdef _DEBUG

    /**
     * Constructor.
     */
    static CSimpleSettingFile* NewL(
        RFs& aFs );
        
    /**
     * Opens a file. It is closed in destructor!
     */
    void OpenL( TDesC& aResFile );

    /**
     * Get the key value. Leave with KErrNotFound if not found.
     */
    TPtrC8 KeyValueL( const TDesC8& aKey );

#endif // _DEBUG      

private:
    CSimpleSettingFile( RFs& aFs );
#ifdef _DEBUG    
    void ReadRowL( TPtrC8& aKey, TPtrC8& aValue );
#endif // _DEBUG     
    
private:
    // Data
    RFs& iFs;
    RFileReadStream iReader;
    TBuf8<128> iRowBuffer;
    TFileName iFileName;
    TBool     iOpen;
    };

#endif      // simpledebugutils_H

// End of File
