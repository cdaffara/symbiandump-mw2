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
* Description:  CPosWriteBufStorage class definition file
*
*/


#ifndef __EPOS_CPOSWRITEBUFSTORAGE_H_
#define __EPOS_CPOSWRITEBUFSTORAGE_H_

#include <e32base.h>

/**
 * Internal class, supports writing data elements to fixed-size buffer.
 */
NONSHARABLE_CLASS( CPosWriteBufStorage ) : public CBase
    {
    public:
        static CPosWriteBufStorage* NewLC( TInt aMaxLen );
        
        ~CPosWriteBufStorage();

    public:        
        static TInt PackedSize( const TDesC& aText );
        static TInt PackedSize( const TDesC8& aText );
        
        void Put( TInt aData );
        void Put( TUint32 aData );
        void Put( TUint16 aData );
        void Put( const TDesC16& aData );
        void Put( const TDesC8& aData );
        
        HBufC8* FinalizeL();

    protected:
        CPosWriteBufStorage();
        void Put( TUint8* aSrc, const TInt aSize );
            
    protected:
        HBufC8* iBuffer;
    };

#endif // __EPOS_CPOSWRITEBUFSTORAGE_H_
