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
* Description: Class for holding collection data information while parsing
* a Landmark XML file to a landmark(s).
*
*/


#ifndef CPOSLMCOLLECTIONDATA_H
#define CPOSLMCOLLECTIONDATA_H

//  INCLUDES
#include <e32base.h>
#include <EPos_Landmarks.h>

// CLASS DECLARATION

/**
*  Class for holding collection data information while parsing
* a Landmark XML file to a landmark(s).
*
*/
class CPosLmCollectionData : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        */
        static CPosLmCollectionData* NewLC(
        /* IN */    const TDesC& aDesC,
        /* IN */    TPosLmCollectionDataId aId
        );

        /**
        * Destructor.
        */
        ~CPosLmCollectionData();

    public: // New functions

        /**
        * Retrieve the name of the collection data.
        *
        * @param aName The name of the collection.
        */
        void GetCollectionData(/* OUT */    TPtrC& aName) const;

        /**
        * Retrieve the id of the collection data.
        *
        * @return The id of the collection.
        */
        TPosLmCollectionDataId CollectionDataId() const;

    private:

        /**
        * C++ default constructor.
        */
        CPosLmCollectionData(TPosLmCollectionDataId aId);

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL(const TDesC& aDesC);

        // By default, prohibit copy constructor
        CPosLmCollectionData( const CPosLmCollectionData& );
        // Prohibit assigment operator
        CPosLmCollectionData& operator= ( const CPosLmCollectionData& );

    private:

        HBufC*                  iCollectionDataName;
        TPosLmCollectionDataId  iCollectionDataId;

    };


#endif      // CPOSLMCOLLECTIONDATA_H

// End of File
