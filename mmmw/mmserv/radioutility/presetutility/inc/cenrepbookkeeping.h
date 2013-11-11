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
* Description:  This is a class, responsible of keeping track of existing presets
*               and their indexes.
*
*
*/

#ifndef CCENREPBOOKKEEPING_H_
#define CCENREPBOOKKEEPING_H_

#include <e32base.h>

class RWriteStream;
class RReadStream;

const TInt KMaxNumberOfPresetStations = 100;  // One temporarily used for internal book keeping

#define  LENGTHOFUSEDINDEXLWORDSARRAY ((KMaxNumberOfPresetStations/32) + 1)
/**
 *  it is a class, responsible of keeping track of existing presets and their indexes.
 */
NONSHARABLE_CLASS( CCenrepBookKeeping ) : public CBase
{
public:

    /**
    * Default class constructor.
    */
    CCenrepBookKeeping();

    /**
    * Destructor of CCenrepBookKeeping class.
    */
    ~CCenrepBookKeeping();

    /**
    * Get the current number of presets.
    * @return The number of presets.
    */
    TInt PresetCount() const;

    /**
    * Get the index of first preset.
    * @return The first preset index.
    */        
    TInt FirstIndex() const;

    /**
    * Get the index of the next preset.
    * @param aFromIndex -  Start from index after this.
    * @return the next preset index.
    */           
    TInt NextIndex( TInt aFromIndex = 0 );

    /**
    * Find an unused preset index.
    * @return Allocated index.
    */           
    TInt FindUnusedIndex() const;
    
    /**
    * Check if given index is used.
    * @return ETrue if used, otherwise EFalse 
    */           
    TBool IsIndexUsed( const TInt aIndex );

    /**
    * Add a preset.
    * @param aIndex -  Proposed index.
    * @return Used/Allocated index.
    */           
    void AddIndex( const TInt aIndex );

    /**
    * Remove a preset.
    * @param aIndex -  Index of the preset to be removed.
    * @return Used/Allocated index.
    */           
    void RemoveIndex( const TInt aIndex );

    /**
    * Clear all data.
    * @return The first preset index.
    */
    void Clear();
    
    /**
    * Gives descriptor pointer to bookkeeping's data.
    * @return TPtr8 Descriptor that contains bookkeeping data.
    */
    TPtr8 BookKeepingData();

private:

    // This data is copied to Central Repository Key (buffer) as such without
    // actual serialization.
    // Therefore in order to keep backward incombatibility don't
    // change the order of fields. Only the last array field can be allowed to grow.
    // If a new field is added to the end, the preceding array field cannot grow anymore.
    // Additionally the size of this structure should never shrink.
    class TBookKeepingData 
    {
    public:
        TInt iCount;    
        TUint32 iUsedIndexLWords[ LENGTHOFUSEDINDEXLWORDSARRAY ];
    } iBookKeepingData;

};

#endif // CCENREPBOOKKEEPING_H_
