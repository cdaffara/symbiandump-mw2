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
* Description:    Data container for a single MIME multipart part.
*
*/




#ifndef C_SIMPLECONTENT2_H
#define C_SIMPLECONTENT2_H


// Includes
#include <e32std.h>
#include <e32base.h>

#include "msimplecontent.h"
#include "simplecommon.h"

/**
 *  CSimpleContent
 *
 *  Data container for a single MIME multipart part.
 *
 *  @lib simplexmlutils
 *  @since S60 v3.2
 */
class CSimpleContent : public CBase, public MSimpleContent
    {        
public:

    static CSimpleContent* NewL( 
        const TDesC8& aContentID, const TDesC8& aContentType );

    static CSimpleContent* NewLC(
        const TDesC8& aContentID, const TDesC8& aContentType );    

    virtual ~CSimpleContent();

// from base class MSimpleContent

    /**
     * Destructor
     */
    void Close();

    /**
     * Data accessor
     *
     * @since S60 ?S60_version
     * @return data
     */
    TPtrC8 Body();
    
    /**
     * Data accessor
     *
     * @since S60 ?S60_version
     * @return data, ownership is transferred
     */
    HBufC8* GiveBodyOwnerShip(); 
    
    /**
     * Data setter
     *
     * @since S60 ?S60_version
     * @param aData data to be copied
     */
    void CopyBodyL( const TDesC8& aData );    
    
    /**
     * Data setter
     *
     * @since S60 ?S60_version
     * @param aData data, ownetship is taken.
     */
    void SetBody( HBufC8* aData ); 
    
    /**
     * content-id accessor
     * @return content-id
     */
    TPtrC8 ContentID(); 
    
    /**
     * content-type accessor
     * @return content-type
     */
    TPtrC8 ContentType();
              
                      
private:

    CSimpleContent();

    void ConstructL(
        const TDesC8& aContentID, const TDesC8& aContentType );     


private: // data

    /**
     * ContentID
     * Own.
     */
    HBufC8* iContentID;
    
    /**
     * ContentType
     * Own.
     */
    HBufC8* iContentType;    
    
    /**
     * Body
     * Own.
     */
    HBufC8* iBody;            
};



#endif // ? C_CLASSNAME_H
