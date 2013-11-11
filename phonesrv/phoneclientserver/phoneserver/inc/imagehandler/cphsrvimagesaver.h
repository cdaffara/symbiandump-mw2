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
* Description:  Image saver header
*
*/


#ifndef C_CPHSRVIMAGESAVER_H
#define C_CPHSRVIMAGESAVER_H

#include <e32base.h>

class RFs;
class MPhSrvImageSaverObserver;
class CFileMan;
class CImageDecoder;
class CBitmapScaler;

/**
*  Image saver.
*
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CPhSrvImageSaver )
:   public CActive
    {
public: // constructor
    
    static CPhSrvImageSaver* NewL( 
        RFs& aFs,
        const TFileName& aPrivatePath,
        MPhSrvImageSaverObserver& aObserver );
    
    ~CPhSrvImageSaver();

public: // new methods

    void SaveImageL( const TDesC& aFileName );

    void CopyImageL( const TDesC& aFileName );
    
    void ScaleImageL();
    
    TInt IsPrimaryFileInUse( TBool& aIsInUse );
    
    TInt ConstructVtFileSaveName( 
        const TDesC& aSourceFileName, 
        TName& aTargetFileName );

private: // from CActive

    void RunL();
    
    void DoCancel();    
    
private:

    void ConstructL();
        
    CPhSrvImageSaver( RFs& aFs, 
        const TFileName& aPrivatePath,
        MPhSrvImageSaverObserver& aObserver);
        
private:

    // file server session
    RFs& iFs;

    // path to private directory
    const TFileName& iPath;
    
    // for checking image files
    CFileMan* iFileManager;
    
    // for decoding image file
    CImageDecoder* iImageDecoder;
    
    // for scaling single frame image
    CBitmapScaler* iScaler;
    
    // for notifying save completion
    MPhSrvImageSaverObserver& iObserver;
    
    

    };

#endif // C_CPHSRVIMAGESAVER_H


// End of File
