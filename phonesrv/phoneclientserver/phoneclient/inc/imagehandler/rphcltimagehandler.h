/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  PhoneServer interface for image handling.
*
*/

#ifndef RPHCLTIMAGEHANDLER_H
#define RPHCLTIMAGEHANDLER_H

//  INCLUDES
#include    <e32base.h>
#include    <phclttypes.h> 
#include    "cphcltoperatorlogocontainer.h" 
#include    "cphcltbaseimageparams.h" 


//  FORWARD DECLARATIONS
class RPhCltServer;


/**
*  PhoneServer interface for image handling.
*
*  @lib PhoneClient.lib
*  @since S60 v3.2
*/
class RPhCltImageHandler 
:   public RSubSessionBase
    {
    public: // Constructors and destructors.

        /**
         * Constructor.
         */
        RPhCltImageHandler();

    public: // New functions.

        /**
        * Open subsession to server.
        * 
        * @param aServer Pre-opened server session.
        * @return Symbian OS error code of the success of the opening.
        */
        TInt Open( RPhCltServer& aServer );

        /**
        * Close subsession.
        */
        void Close();
    
        /**
        * Save image(s).
        *
        * @param aParam An array of bitmap handles passed.
        * @return KErrNone if successful, Symbian error code otherwise.
        */
        TInt SaveImages( CPhCltImageParams& aParams );
            
        /**
        * Load image(s).
        *
        * @param aParam An array of bitmap handles passed.
        * @return KErrNone if successful, Symbian error code otherwise.
        */
        TInt LoadImages( CPhCltImageParams*& aParams );

        /**
        * Saves VT still image.
        */
        void SaveVtImage( TRequestStatus& aStatus, const TDesC& aFilePath );
        
        /**
         * Cancels image saving
         */
        void CancelSaveVtImage();

        /**
        * Opens VT still image file. Returns file handle that can be
        * used with AdoptFromServer.
        * @param aFileHandle file handle
        * @param aOpenDefault if true, opens predefined, if false opens
        * user defined image file.
        */
        TInt OpenVtImage( TInt& aFileHandle, const TBool aOpenDefault = EFalse );
        
    private:

        // Prohibit copy constructor if not deriving from CBase.
        RPhCltImageHandler( const RPhCltImageHandler& );

        // Prohibit assigment operator if not deriving from CBase.
        RPhCltImageHandler& operator=( 
            const RPhCltImageHandler& );
            
    private: // New functions.
        
        /**
        * Finalizes image load.
        * @param aImages handle array
        * @return Symbian error code.
        */
        TInt FinalizeLoad( TPckgBuf< TPhCltImageArray >& aImages );

        /**
        * Creates a copy of image.
        * @param aImages handle array
        * @return copied bitmap.
        */
        CFbsBitmap* CreateBitMapL( const TInt aHandle );
        
        /**
        * Ducplicates vt still images.
        * @param aImages handle array
        */
        void CopyStillsL( TPhCltImageArray& aImages );  
        
        /**
        * Duplicates operator logos.
        * @param aImages handle array
        */
        void CopyLogosL( TPhCltImageArray& aImages );
        
            
    private: // Data
    
        // Duplicated VT images.
        RPointerArray<CFbsBitmap> iVTBitMaps;
        
        // Duplicated operator logos.
        RPointerArray<CPhCltOperatorLogoContainer> iOperatorLogos;

    };

#endif    // RPHCLTIMAGEHANDLER_H

// End of File
