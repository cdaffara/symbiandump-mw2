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
* Description: provides handling of indexing in database
*
*
*/


#ifndef EPOS_CPOSLMDBINDEXMANAGER_H_
#define EPOS_CPOSLMDBINDEXMANAGER_H_

#include <e32base.h>

class CPosLmServer;
class CPosLmNameIndex;
class CPosLmLocalDbAccess;

class CPosLmIndexManager : public CBase
    {
    class CItem : public CBase
        {
        public:
            ~CItem();
            
            HBufC* iDbUri;
            CPosLmLocalDbAccess* iDbAccess;
            CPosLmNameIndex* iIndex;
            TInt iRefCount;
            TBool iHasReadLock;
            TBool iHasWriteLock;
        };

    public:
        static CPosLmIndexManager* NewL( CPosLmServer& aServer );
    	~CPosLmIndexManager();
    
        TInt Find( const TDesC& aUri ) const;
        CPosLmNameIndex& Index( const TDesC& aUri );
        
        void AddIndexUserL( const TDesC& aUri );
        void RemoveIndexUser( const TDesC& aUri );
        
        TBool HasAnyLock( const TDesC& aUri ) const;
        TBool HasWriteLock( const TDesC& aUri ) const;

    protected:
    
        CPosLmIndexManager( CPosLmServer& aServer );
        void ConstructL();
    
        void LoadIndexL( const TDesC& aUri );
        void SaveIndexL( CItem& aItem );

        TTime DatabaseStampL( const TDesC& aDbUri ) const;
        TBool IsIndexValidL( const TTime& aDbStamp, const TTime& aIndexStamp ) const;
        
    private:
        RPointerArray<CItem>    iIndexes;
        CPosLmServer&			iServer;
    };

#endif /*EPOS_CPOSLMDBINDEXMANAGER_H_*/
