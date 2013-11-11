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
* Description:    SIMPLE data document
*
*/




#ifndef M_simplecontent_H
#define M_simplecontent_H

#include <e32std.h>
// #include <S32strm.h>

// FORWARD DECLARATION



/**
 *  MSimpleContent
 *
 *  SIMPLE data direct content
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class MSimpleContent 
    {

public:

    /**
     * Destructor
     */
    virtual void Close() = 0;

    /**
     * Data accessor
     *
     * @since S60 ?S60_version
     * @return data
     */
    virtual TPtrC8 Body() = 0;
    
    /**
     * Data accessor
     *
     * @since S60 ?S60_version
     * @return data, ownership is transferred
     */
    virtual HBufC8* GiveBodyOwnerShip() = 0;
    
    /**
     * Data setter
     *
     * @since S60 ?S60_version
     * @param aData data to be copied
     */
    virtual void CopyBodyL( const TDesC8& aData ) = 0;
    
    /**
     * Data setter
     *
     * @since S60 ?S60_version
     * @param aData data, ownetship is taken.
     */
    virtual void SetBody( HBufC8* aData ) = 0;
    
    /**
     * content-id accessor, without "<"...">" characters
     * @return content-id
     */
    virtual TPtrC8 ContentID() = 0;
    
    /**
     * content-type accessor
     * @return content-type
     */
    virtual TPtrC8 ContentType() = 0;
                                 

    };

#endif

// End of File
