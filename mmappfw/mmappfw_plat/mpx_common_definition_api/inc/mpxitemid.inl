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
* Description:  inline function definitions for TMPXItemId
*
*/


// ----------------------------------------------------------------------------
// TInt() operator
// ----------------------------------------------------------------------------
//
inline TMPXItemId::operator TInt() const 
    { 
    return iId1; 
    }
 
// ----------------------------------------------------------------------------
// TInt32() operator
// ----------------------------------------------------------------------------
//       
inline TMPXItemId::operator TInt32() const 
    { 
    return iId1; 
    }

// ----------------------------------------------------------------------------
// TUint() operator
// ----------------------------------------------------------------------------
//            
inline TMPXItemId::operator TUint() const 
    { 
    return iId1; 
    }        

// ----------------------------------------------------------------------------
// TUint32() operator
// ----------------------------------------------------------------------------
//
inline TMPXItemId::operator TUint32() const
    { 
    return iId1; 
    }
         
// ----------------------------------------------------------------------------
// TInt64() operator
// ----------------------------------------------------------------------------
//
inline TMPXItemId::operator TInt64() const 
    { 
    return iId1; 
    }        

// ----------------------------------------------------------------------------
// TUint64() operator
// ----------------------------------------------------------------------------
//
inline TMPXItemId::operator TUint64() const 
    { 
    return iId1; 
    }

// ----------------------------------------------------------------------------
// == operator
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::operator==( const TMPXItemId& aOtherId ) const 
    {
    return (iId1 == aOtherId.iId1) &&  (iId2 == aOtherId.iId2);               
    };

// ----------------------------------------------------------------------------
// == operator
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::operator==( TInt aOtherId ) const 
    {
    return (iId1 == aOtherId);               
    };

// ----------------------------------------------------------------------------
// == operator
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::operator==( TInt32 aOtherId ) const 
    {
    return (iId1 == aOtherId);               
    };

// ----------------------------------------------------------------------------
// == operator
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::operator==( TUint32 aOtherId ) const 
    {
    return (iId1 == aOtherId);               
    };

// ----------------------------------------------------------------------------
// != operator
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::operator!=( const TMPXItemId& aOtherId ) const 
    {
    return (iId1 != aOtherId.iId1) ||  (iId2 != aOtherId.iId2);               
    };

// ----------------------------------------------------------------------------
// != operator
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::operator!=( TInt aOtherId ) const 
    {
    return (iId1 != aOtherId);
    };   

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
inline TMPXItemId::TMPXItemId() 
    {
    iId1 = 0; 
    iId2 = 0;    
    };     

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
inline TMPXItemId::TMPXItemId( TUint32 aId ) 
    {
    iId1 = aId; 
    iId2 = aId;    
    };

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
inline TMPXItemId::TMPXItemId( TUint32 aId1, TUint32 aId2 )
    {
    iId1 = aId1; 
    iId2 = aId2;
    };

// ----------------------------------------------------------------------------
// Almost equal function
// ----------------------------------------------------------------------------
//
inline TBool TMPXItemId::ApproxEqual( const TMPXItemId& aOtherId ) const
    {
    return (iId1 == aOtherId.iId1) ||  (iId2 == aOtherId.iId2);   
    }
    
// ----------------------------------------------------------------------------
// Creates an invalid ID
// ----------------------------------------------------------------------------
//
inline TMPXItemId TMPXItemId::InvalidId()
    { 
    TMPXItemId id;
    id.iId1=KMaxTUint32; 
    id.iId2=KMaxTUint32; 
    return id; 
    }

// END OF FILE
