/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/



#ifndef CCLFITEMIMPL_H
#define CCLFITEMIMPL_H

//  INCLUDES
#include <MCLFModifiableItem.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <mdesession.h>

// FORWARD DECLARATIONS
class CMdEObject;
class CCLFContainerItem;
class CCLFDbItemContainer;

// CLASS DECLARATION

/**
*  This class implements MCLFModifiableItem interface.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFItemImpl ) : public CBase,
                                    public MCLFItem
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFItemImpl* NewLC( CCLFContainerItem& aContainerItem,
                                    CCLFDbItemContainer& aDbItemContainer );

        /**
        * Two-phased constructor.
        */
        static CCLFItemImpl* NewL( CCLFContainerItem& aContainerItem,
                                   CCLFDbItemContainer& aDbItemContainer );

        /**
        * Destructor.
        */
        virtual ~CCLFItemImpl();

    protected:  // Functions from base classes

        /**
        * From MCLFItem
        */
        virtual TCLFItemId ItemId() const;

        /**
        * From MCLFItem
        */
        virtual TCLFItemDataType DataType( TCLFFieldId aFieldId ) const;

        /**
        * From MCLFItem
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TPtrC& aData ) const;

        /**
        * From MCLFItem
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TInt32& aData ) const;

        /**
        * From MCLFItem
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TTime& aData ) const;

    private:

        TBool DeduceFieldFromURI( const TCLFFieldId aFieldId, TPtrC& aValue ) const;

        /**
        * Map MdE object definition to CLF media type.
        * @param aObjectDef Object definition
        * @param aValue     Media type
        * @return           KErrNone if media type was identified.
        *                   KErrNotFound if media type was unknown.
        */
        TInt GetMediaTypeL( CMdEObjectDef& aObjectDef, TInt32& aValue ) const;

        void DetermineIfMusicL( CMdEObjectDef& aObjectDef, TInt32& aValue ) const;


        /**
        * Map CLF field to MdE property.
        * @param aField Field ID.
        * @return       Property name
        */
        static const TDesC& MapFieldToMdEProperty( TCLFFieldId aField );

        /**
        * C++ default constructor.
        */
        CCLFItemImpl( CCLFContainerItem& aContainerItem,
                             CCLFDbItemContainer& aDbItemContainer );

    private:    // Data
        
        // Ref
        CCLFContainerItem& iItem;
        // Ref
        const CMdEObject& iMdEObject;
        // Ref
        CCLFDbItemContainer& iDbItemContainer;
    };

// CLASS DECLARATION

/**
*  This class implements MCLFModifiableItem interface.
*
*  @lib N/A
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFModifiableItemImpl ) : public CBase,
                                              public MCLFModifiableItem
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFModifiableItemImpl* NewL();

        /**
        * Destructor.
        */
        virtual ~CCLFModifiableItemImpl();

    protected:  // Functions from base classes

        /**
        * From MCLFModifiableItem
        */
        virtual TCLFItemId ItemId() const;

        /**
        * From MCLFModifiableItem
        */
        virtual TCLFItemDataType DataType( TCLFFieldId aFieldId ) const;

        /**
        * From MCLFModifiableItem
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TPtrC& aData ) const;

        /**
        * From MCLFModifiableItem
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TInt32& aData ) const;

        /**
        * From MCLFModifiableItem
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TTime& aData ) const;

        /**
        * From MCLFModifiableItem
        */
        virtual void AddFieldL( TCLFFieldId aFieldID, const TDesC& aValue );

        /**
        * From MCLFModifiableItem
        */
        virtual void AddFieldL( TCLFFieldId aFieldID, TInt32 aValue );

        /**
        * From MCLFModifiableItem
        */
        virtual void AddFieldL( TCLFFieldId aFieldID, const TTime& aValue );
        
    private:

        /**
        * C++ default constructor.
        */
        inline CCLFModifiableItemImpl();

    private:    // Data

        RHashMap<TCLFFieldId, TInt32> iIntMap;
        RHashMap<TCLFFieldId, HBufC*> iTextMap;
        RHashMap<TCLFFieldId, TInt64> iTimeMap;
        
    };

#endif      // CCLFITEMIMPL_H

// End of File
