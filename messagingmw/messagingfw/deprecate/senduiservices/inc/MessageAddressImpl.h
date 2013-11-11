/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implementation of CMessageAddress interface.
*                Encapsulates message real address and alias.
*
*/


#ifndef C_MESSAGEADDRESSIMPL_H
#define C_MESSAGEADDRESSIMPL_H

#include <e32base.h>

class RWriteStream;
class RReadStream;

const TInt KMaxLength = 128;

/**
 * Encapsulates message recipient real address and alias.
 *
 * @lib SendUi
 * @since Series 60 v3.0
 */ 
class CMessageAddressImpl : public CBase
{
public:  // Constructors and destructor

   /**
    * C++ default constructor.
    */
    CMessageAddressImpl();
    
   /**
    * Destructor.
    */
    virtual ~CMessageAddressImpl();

public: // New functions

   /**
    * Sets real address.
    *
    * @since Series 60 3.0
    * @param aAddress Real address.
    * @return None.
    */
    void SetAddressL( const TDesC& aAddress );

   /**
    * Returns real address.
    *
    * @since Series 60 3.0
    * @return Real address.
    */
    inline const TPtrC Address() const;

    /**
    * Sets alias for the real address.
    *
    * @since Series 60 3.0
    * @param aAlias Alias for the real address.
    * @return None.
    */
    void SetAliasL( const TDesC& aAlias );

   /**
     * Externalizes message address data
     *
     * @since S60 v3.2
     * @param aStream Open write stream 
     */    
     inline const TPtrC Alias() const;

    void ExternalizeL( RWriteStream& aStream ) const;
    
    /**
     * Internalizes message address data
     *
     * @since S60 v3.2
     * @param aStream Stream containing message address
     */    
    void InternalizeL( RReadStream& aStream );
    

 private:  // Data

    /**
     * Real address
     * Own.
     */
    HBufC*                      iRealAddress;

    /**
     * Alias for real address
     * Own.
     */
    HBufC*                      iAlias;

};

#include "CMessageAddressImpl.inl"

#endif      // C_MESSAGEADDRESSIMPL_H

// End of File
