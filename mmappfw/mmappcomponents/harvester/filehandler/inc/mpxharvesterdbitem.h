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
* Description:  Representation of each item in the database
*
*/


#ifndef CMPXHARVESTERDBITEM_H
#define CMPXHARVESTERDBITEM_H

/**
 *  Representation of each item in the db
 *
 *  @lib mpxfilehandler
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterDbItem ) : public CBase
    {
public:
    
    virtual ~CMPXHarvesterDbItem() 
        {
        delete iFile;
        };
        
    /**
    * Compares two CMPXHarvesterDbItem objects.
    * @param aFirst Reference to the first instance for comparison
    * @param aSecond Reference to the second instance for comparison
    * @return 0 if the compared instances are equal
    */
    static TInt Compare(const CMPXHarvesterDbItem& aFirst,
                        const CMPXHarvesterDbItem& aSecond)
        {
        return aFirst.iFile->CompareF( *( aSecond.iFile ) );
        }

public: // Member variables
    HBufC* iFile;
    TTime  iLastModifiedTime;
    TInt   iColId;
    TBool  iDrm;        
    };


#endif // CMPXHARVESTERDBITEM_H
