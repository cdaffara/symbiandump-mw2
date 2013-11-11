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
* Description: This class is a handle to server-side landmark name index
*
*
*/


#ifndef RPOSLMLOCALNAMEINDEX_H
#define RPOSLMLOCALNAMEINDEX_H

#include <e32base.h>
#include <EPos_Landmarks.h>

class CPosLandmark;
class RPosLmLocalSubsession;
class RPosLmLocalAccessSubsession;

/**
*  This class is a handle to server-side landmark name index
*
*  @lib eposlmlocalaccess.lib
*  @since S60 3.2
*/
class RPosLmLocalNameIndex
    {
    public:
    
        class CIndexItem : public CBase
            {
            public:
                CIndexItem();
                ~CIndexItem();
            
                static CIndexItem* NewL( TPosLmItemId aId, const TDesC& aName );
                static CIndexItem* NewL( RReadStream& aStream );
        
                inline TPosLmItemId Id() const;
                inline TPtrC Name() const;
                
            private:
                TPosLmItemId iId;
                HBufC* iName;
            };
    
    public:

        /**
        * C++ default constructor.
        * @param[in] aSession Parent session to server.
        */
        RPosLmLocalNameIndex( RPosLmLocalAccessSubsession& aSession );

        /**
        * Creates a local access subsession with the Landmarks Server.
        *
        * @return @p KErrNone if successful, otherwise one of the system-wide
        * error codes.
        */
        TInt Open();

        /**
        * Closes the subsession.
        */
        void Close();

        /** Returns amount of items in the index */
        IMPORT_C TInt Count() const;
        
        /** Returns amount of items in the index */
        IMPORT_C TInt Status() const;

        /** Reads part of sorted landmark ids from server's index 
         *  @param[out] aIdArray Target buffer for landmarks IDs
         *  @param[in] aFirst index of first ID to read 
         *  @param[in] aCount amount of IDs to read (size of aIdArray buffer)
         *  @param[out] aRemainder On completion contains number of IDs left
         *      in the index
         *  @return If negative - error code, otherwise - number of actual
         *      IDs read to aIdArray. 0 means that no more IDs available */
        IMPORT_C TInt ReadSortedIds( 
            TPosLmItemId* aIdArray, 
            TInt aFirst, 
            TInt aCount, 
            TInt& aRemainder );
        
        /** Reads part of sorted landmarks from server's index.
         *  Upon completion target array aArray will contain 
         *  partial landmark data: ID and Name.
         *  @param[in] aFirst index of first landmark to read 
         *  @param[in] aCount amount of landmarks to read
         *  @param[out] aArray Target buffer for landmarks
         *  @param[out] aRemainder On completion contains number of landmarks left
         *      in the index */
        IMPORT_C void ReadSortedLandmarksL( 
            TInt aFirst, 
            TInt aCount, 
            RPointerArray<CIndexItem>& aArray, 
            TInt& aRemainder );

        /** Starts transaction on server-side landmark name index.
         *  Only used for modifications */
        TInt BeginTransaction();

        /** Commits transaction on server-side landmark name index */
        TInt CommitTransaction();

        /** Cancels transaction on server-side landmark name index */
        TInt RollbackTransaction();
        
        /** Informs server about newly added landmark 
         *  @param aLandmark Landmark data */
        void AddL( const CPosLandmark& aLandmark );
        
        /** Informs server about newly added landmarks
         *  @param aLandmarks List of landmarks */
        void AddL( RPointerArray<CPosLandmark>& aLandmarks );
        
        /** Informs server about modified landmark
         *  @param aLandmark Landmark data */
        void UpdateL( const CPosLandmark& aLandmark );

        /** Informs server about modified landmarks
         *  @param aLandmarks List of landmarks */
        void UpdateL( RPointerArray<CPosLandmark>& aLandmarks );
        
        /** Informs server about removed landmark
         *  @param aLmid ID of the landmark */
        void RemoveL( TPosLmItemId aLmid );
        
        /** Informs server about removed landmarks
         *  @param aLmids List of landmark IDs */
        void RemoveL( RArray<TPosLmItemId>& aLmids );

    protected:

        /** Packs landmark IDs and Names and sends to server 
         *  @param aFunction IPC function, which is to handle the message
         *  @param aLandmarks List of landmarks */
        void SendLandmarkDataL( 
            TInt aFunction,
            RPointerArray<CPosLandmark>& aLandmarks );

        /** Returns size of basic landmark data in the array
         * size of total count + size of ID, name and name length per
         * each landmark */
        TInt CalculateLandmarkBasicDataSize( 
            RPointerArray<CPosLandmark>& aLandmarks );

    private:

        // By default, prohibit copy constructor
        RPosLmLocalNameIndex( const RPosLmLocalNameIndex& );
        // Prohibit assigment operator
        RPosLmLocalNameIndex& operator=( const RPosLmLocalNameIndex& );

    private:    // Data

        RPosLmLocalAccessSubsession&    iSession;
        HBufC8*                         iBuffer;
    };

#include "epos_rposlmlocalnameindex.inl"    
    
#endif      // RPOSLMLOCALNAMEINDEX_H

   