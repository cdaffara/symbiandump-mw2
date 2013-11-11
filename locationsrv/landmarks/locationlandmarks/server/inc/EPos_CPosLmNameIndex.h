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
* Description: Declaration of CPosLmNameIndex class
*
*
*/


#ifndef CPOSLMNAMEINDEX_H_
#define CPOSLMNAMEINDEX_H_

#include <e32base.h>
#include <EPos_Landmarks.h>

class CPosLandmark;
class CPosLmLocalDbAccess;
class RDbNamedDatabase;
class RDbTable;
class RWriteStream;

class CPosLmNameIndex : public CBase
    {
    public:

        class CItem : public CBase
            {
            public:
                virtual TPosLmItemId Id() const = 0;
                virtual TPtrC Name() const = 0;
    
                virtual TInt Size() const = 0;
                virtual void ExternalizeL( RWriteStream& aOut ) const = 0;
            };

    protected:
    
        class CIndexItem : public CItem
            {
            public:
                enum TState
                    {
                    EStateTemp      = 0,
                    EStateValid     = 1,
                    EStateInvalid   = 2
                    };
            
                CIndexItem();
                CIndexItem( TPosLmItemId aId );
                CIndexItem( TPosLmItemId aId, HBufC* aName );
                ~CIndexItem();
        
                static CIndexItem* NewLC( TPosLmItemId aId, const TDesC& aName );
                static CIndexItem* NewLC( RReadStream& aIn );
        
            public: // from CItem
                inline TPosLmItemId Id() const;
                TPtrC Name() const;

                TInt Size() const;
                void ExternalizeL( RWriteStream& aOut ) const;
                
            public:    
                static TInt CompareById( const CIndexItem& aLeft, const CIndexItem& aRight );
                
                inline TUint32* IdPtr();
                inline HBufC* NamePtr();
                
                inline void SetId( TPosLmItemId aLmid );

                inline TBool IsValid();
                inline TBool IsTemp();
                inline void SetValid();
                inline void SetInvalid();
                
            protected:    
                TPosLmItemId    iId;
                HBufC*          iName;
                TInt32          iState;
            };
    
        class TLmIndexNameKey : public TKeyArrayFix
            {
            public:
                TLmIndexNameKey();
                TAny *At( TInt anIndex ) const;
            };
            
        class TLmIndexIdKey : public TKeyArrayFix
            {
            public:
                TLmIndexIdKey();
                TAny *At( TInt anIndex ) const;
            };

    public:
    
        static CPosLmNameIndex* NewL( CPosLmLocalDbAccess& aDbAccess );
        static CPosLmNameIndex* NewLC( CPosLmLocalDbAccess& aDbAccess );
        ~CPosLmNameIndex();
        
        /** Stores index data to database
         *  @param[in] aDrive The disk where database is located. This method
         *      will check if the drive has enough space to store the index. 
         *  @leave KErrDiskFull if database drive cannot accomodate index data. */
        void SaveL( TChar aDrive );
        
        /** Performs one step of index evaluation and returns progress. */
        TReal32 Evaluate();
        
        /** Starts transaction on the index. From now on and until 
         *  transaction is committed or rolled back, all inserted items
         *  are left in EStateTemp state and all removed items are
         *  only marked as EStateInvalid */
        void StartTransaction();
        /** Marks all temp items as valid and removes all invalid ones */
        void CommitTransaction();
        /** Removes all invalid and temp items */
        void RollbackTransaction();
        
        /** @return status of the index object: 
         *  - KErrNone - ready
         *  - KErrNotReady - evaluating or in transaction
         *  - other - error. */
        inline TInt Status() const;
        
        /** @return timestamp of the index store */
        inline TTime TimeStamp() const;

        /** Inserts landmark to the correct place in the index. 
         * ID and name must be valid */
        void InsertL( const CPosLandmark& aLandmark );
        /** Inserts ID and name pair to the correct place in the index. */
        void InsertL( TPosLmItemId aId, const TDesC& aName );
        /** Inserts ID and name pair to the correct place in the index. 
         * This object takes ownership of the aName */
        void InsertL( TPosLmItemId aLmid, HBufC* aName );
        /** Updates name for the given ID and moves it to the correct place in the index. */
        void UpdateL( TPosLmItemId aId, const TDesC& aName );
        /** Updates name for the given ID and moves it to the correct place in the index. 
         * This object takes ownership of the aName */
        void UpdateL( TPosLmItemId aLmid, HBufC* aName );
        /** Removes an item from the index */
        void Remove( TPosLmItemId aId );

        /** Total amount of items in index */
        TInt Count() const;
        /** Access to a particular item in index */
        const CItem& Item( TInt aIndex ) const;
        /** Returns index of valid item with given ID, or KErrNotFound if not found */
        TInt Find( TPosLmItemId aLmid ) const;
        
        /** Returns list of all IDs sorted by landmark name */
        CArrayFixSeg<TPosLmItemId>* GetSortedIdsLC() const;
        /** Returns a part of the list of IDs sorted by landmark name */
        CArrayFixSeg<TPosLmItemId>* GetSortedIdsLC( TInt aFirst, TInt aCount ) const;
        /** Returns a part of the list of IDs sorted by landmark name 
         *  @param[out] aTarget pointer to array of TPosLmItemId elements. 
         *      It must be able to accomodate aCount items 
         *  @param[out] aRemider on return contains amount of items left in the index
         *      (this is Count() - (aFirst + aCount) ) */
        TInt GetSortedIds( 
            TInt aFirst, TInt aCount, 
            TPosLmItemId* aTarget, TInt& aReminder ) const;
        
        /** @return size of this index if externalized */
        TInt DataSize() const;
        
        /** @return size of given item if externalized */
        TInt DataSize( const CItem& aItem ) const;

        /** Clears the index */
        void Reset();
        
        /** Writes single item onto given stream.
         *  Stream will contain ID, length of the name, name data */
        void ExternalizeL( RWriteStream& aOut, const CItem& aItem ) const;
        
    protected:
        CPosLmNameIndex( CPosLmLocalDbAccess& aDbAccess );
        
        void ConstructL();
        
        void ExternalizeL( RWriteStream& aOut ) const;
        void InternalizeL( RReadStream& aIn );

        /** Inserts ID and name pair to the correct place in the index. 
         *  Takes ownership of the aName */
        CIndexItem* DoInsertL( TPosLmItemId aLmid, HBufC* aName );

        void DoEvaluateL( TReal32& aProgress );
        void StopEvaluation();
        
        void LoadL();
        void CreateByIdSortingL();
        void RemoveInvalidItems();

        CIndexItem* ItemById( TPosLmItemId aId ) const;
        void AppendL( TPosLmItemId aLmid, HBufC* aName );
        void AppendL( CIndexItem* aLandmark );
        void DoRemove( TInt aIndex );
        
    private:
    
    	CArrayPtrSeg<CIndexItem>*   iArray;
        
        TInt                        iDataSize;

        CPosLmLocalDbAccess&        iDbAccess;
        RDbNamedDatabase            iDatabase;
        RDbTable                    iTable;
        TInt                        iTablePosition;
        TBool						iIsTableOpen;
        
        TInt                        iStatus;
        TTime                       iTimeStamp;

        RPointerArray<CIndexItem>   iIdSortedArray; // does not own items!
        CIndexItem*                 iKeyItem;
        
        TBool                       iInTransaction;
    };

#include "epos_cposlmnameindex.inl"    
    
#endif /*CPOSLMNAMEINDEX_H_*/
