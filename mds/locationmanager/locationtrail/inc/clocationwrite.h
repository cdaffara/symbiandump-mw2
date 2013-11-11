/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Writes location info to image's EXIF data.
*
*/

#ifndef C_CLOCATIONWRITE_H
#define C_CLOCATIONWRITE_H

#include <e32base.h>

class CExifModify;
class CLocationRecord;
class TPosition;
/**
*  This class writes GPS details to image's EXIF data.  
*
*  @since S60 3.1
*/
class CLocationWrite : public CBase
    {
public:  
    /**
     * 2-phased constructor.
     */
    IMPORT_C static CLocationWrite* NewL( CLocationRecord& aLocRecord );
    
    /**
     * C++ destructor.
     */
    IMPORT_C virtual ~CLocationWrite();
    
public: 
    /**
     * Write location information to the image.
     * @since S60 3.1
     * @param aName, The name and path of the image, in which the location
     *               information will be updated.
     */
    IMPORT_C void WriteLocationL( const TDesC& aName );
            
private:
    /**
     * C++ constructor.
     */  
    CLocationWrite( CLocationRecord& aLocRecord );
    
    /**
     * 2nd phase constructor.
     */
    void ConstructL();
    
private:
    /**
     * Write GPS tags to EXIF.
     */
    void WriteGPSTagsToExifL( CExifModify& aExif, 
                              const TPosition& aPosition );
    
    /**
     * Create EXIF tag. 
     */
    void CreateTag( const TInt aTagId, 
                    const TPosition& aPosition, 
                    TDes8& aTagData );
    
    /**
     * Append integer to descriptor.
     */
    void AppendIntegerToDesc( TDes8& aTagDesc, const TUint aValue );
    
private:
    /**
     * The location trail record class.
     * Not own.
     */
    CLocationRecord& iLocationRecord;
    };

#endif // C_CLOCATIONWRITE_H

// End of file.
