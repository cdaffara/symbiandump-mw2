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
* Description:  TMPXitemId definition
*
*/


#ifndef T_MPXITEMID_H
#define T_MPXITEMID_H


/**
* Structure to hold a the representation of an Item ID
* The structure contains 2 ids, and can be extensible to more
* The iId1 ID MUST be unique within the collection path.
* The IId2 ID can be duplicated in the collection path, 
* and can be use for extended purposes.
*
* @lib mpxcommon.lib
*/
NONSHARABLE_CLASS( TMPXItemId )
    {
    public:
        /**
        * TInt operator, returns iId1.
        *
        * @since S60 3.2.3
        */
        inline operator TInt() const; 
        
        /**
        * TInt32 operator, returns iId1.
        *
        * @since S60 3.2.3
        */
        inline operator TInt32() const; 
        
        /**
        * TUint32 operator, returns iId1.
        *
        * @since S60 3.2.3
        */
        inline operator TUint() const;
        
        /**
        * TUint32 operator, returns iId1.
        *
        * @since S60 3.2.3
        */
        inline operator TUint32() const; 
        
        /**
        * TInt64 operator, returns iId1.
        *
        * @since S60 3.2.3
        */
        inline operator TInt64() const;
        
        /**
        * TUint64 operator, returns iId1.
        *
        * @since S60 3.2.3
        */
        inline operator TUint64() const;
        
        /**
        * == operator.
        *
        * @since S60 3.2.3
        * @param aOtherId TMPXItemId to compare
        */
        inline TBool operator==( const TMPXItemId& aOtherId ) const ;
        
        /**
        * == operator.
        *
        * @since S60 3.2.3
        * @param aOtherId TMPXItemId to compare
        */
        inline TBool operator==( TInt aOtherId ) const ;
        
        /**
        * == operator. 
        *
        * @since S60 3.2.3
        * @param aOtherId TInt32 to compare to public id
        */
        inline TBool operator==( TInt32 aOtherId ) const ;
        
        /**
        * == operator. 
        *
        * @since S60 3.2.3
        * @param aOtherId TUint32 to compare to public id
        */
        inline TBool operator==( TUint32 aOtherId ) const ;
        
        /**
        * != operator. 
        *
        * @since S60 3.2.3
        * @param aOtherId TMPXItemId to compare
        */
        inline TBool operator!=( const TMPXItemId& aOtherId ) const; 
        
        /**
        * != operator. 
        *
        * @since S60 3.2.3
        * @param aOtherId TInt to compare to public id
        */
        inline TBool operator!=( TInt aOtherId ) const; 
        
        /**
        * An approximately equal operator. If either iId1 or iId2 matches, 
        * ETrue is returned.
        *
        * @since S60 3.2.3
        * @param aOtherId TMPXItemId to compare
        */
        inline TBool ApproxEqual( const TMPXItemId& aOtherId ) const;
        
        /**
        * Constructor.
        * Public and private ids iniitialized to 0.
        *
        * @since S60 3.2.3
        */
        inline TMPXItemId(); 
        
        /**
        * Constructor. 
        *
        * @since S60 3.2.3
        * @param aId id to initialize both public and private id
        */
        inline TMPXItemId( TUint32 aId ); 
        
        /**
        * Constructor.
        *
        * @since S60 3.2.3
        * @param aPublicId public ID
        * @param aPrivateId privateID
        */
        inline TMPXItemId( TUint32 aId1, TUint32 aId2 );
        
        /**
        * Static function to create an invalid ID.
        *
        * @since S60 3.2.3
        */
        static inline TMPXItemId InvalidId();
        
    public:
        TUint32 iId1;
        TUint32 iId2;    
    };

#define KMPXInvalidItemId TMPXItemId::InvalidId()

#include "mpxitemid.inl"

#endif // T_MPXITEMID_H
