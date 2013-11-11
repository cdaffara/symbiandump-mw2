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
* Description:  CPosReadBufStorage class definition file
*
*/


#ifndef __EPOS_CPOSREADBUFSTORAGE_H_
#define __EPOS_CPOSREADBUFSTORAGE_H_

#include <e32base.h>
#include <lbsfields.h>

/**
 * Internal class, supports reading data elements from fixed-size buffer.
 */
NONSHARABLE_CLASS(CPosReadBufStorage) : public CBase
    {
    public:
        static CPosReadBufStorage* NewLC( const TDesC8& aBuffer );
        
        void GetL( TInt& aData );
        void GetL( TUint32& aData );
        void GetL( TUint16& aData );
        void GetL( TPtrC& aData );
        void GetL( TDes8& aData );
        
    protected:
        CPosReadBufStorage( const TDesC8& aBuffer );
            
        void GetL( TUint8* aTrg, TUint aSize );
    
    protected:
        const TDesC8& iBuffer;
        const TUint8* iPointer;
        const TUint8* iBoundary;
    };
    
#endif // __EPOS_CPOSREADBUFSTORAGE_H_
