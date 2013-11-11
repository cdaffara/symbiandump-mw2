/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Image storing and loading interface.
*
*/


#ifndef CPHCLTIMAGEHANDLER_H
#define CPHCLTIMAGEHANDLER_H

#include <e32base.h>
#include "cphcltimageparams.h" 

// FORWARD DECLARATIONS
class RPhCltServer;
class CPhCltImageParams;
class RFile;

/**
* Interface for image saving callback.
*/
class MPhCltImageObserver
    {
public:
    /**
    * Callback for image saving.
    * @param aResult KErrNone if image saving succeeded, or a Symbian OS
    *        error code in failure.
    */
    virtual void HandleImageSaveCompleteL( TInt aResult ) = 0;
    };
    
/**
*  Interface used in image storing to PhoneServer.
*
*  @lib PhoneClient.lib
*  @since S60 v3.2
*/
class CPhCltImageHandler : public CBase
    {
    
    public:  // Constructors 
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhCltImageHandler* NewL();
        
    public: // New functions

        /**
        * Save image(s)
        * @param aParam parameters passed 
        * @return KErrNone if successful, Symbian error code otherwise.
        * NOTE: can be used to delete operator logos,
        * @see CPhCltExtOperatorLogoParams::SetCodesL
        * @see CPhCltExtOperatorLogoParams::AddImageL        
        */
        virtual TInt SaveImages( CPhCltImageParams& aParams ) = 0;
            
        /**
        * Load image(s)
        * @param aParam parameters in return passed 
        * @return KErrNone if successful, Symbian error code otherwise.
        */
        virtual TInt LoadImages( CPhCltImageParams*& aParams ) = 0;

        /**
        * Creates image parameter class.
        * @param aType type of the implementation class.
        * @return CPhCltBaseImageParams implementation class. 
        *         NULL if not supported.
        *         Ownership transferred.
        */
        IMPORT_C virtual CPhCltImageParams* 
            CPhCltBaseImageParamsL( const TPhCltImageType aType ) = 0; 
        
        
        /**
        * Saves user defined video call still image file. This makes copy
        * of original file to secure place.
        * @param aObserver callback for notifying result of the operation
        * @param aImagePath full path to image to be saved
        * @since S60 v5.0
        */
        virtual void SaveVtImageL( MPhCltImageObserver& aObserver,
            const TDesC& aImagePath ) = 0;
                        
        /**
        * Cancels pending image saving. This may be no-op from saving point
        * of view (image is actually saved) but resets internal state to such
        * that no operation is pending.
        * @since S60 v5.0
        */
        virtual void CancelSaveVtImage() = 0;
        
        /**
         * Opens user defined video call still image file.
         * @param aFile file handle that shall to point to image file
         *        Client should close the file after use.
         * @return KErrNone if successful, Symbian error code otherwise.
         * @since S60 v5.0
         */
        virtual TInt OpenVtImage( RFile& aFile ) = 0;
                
       /**
        * Opens predefined (read-only) video call still image file.
        * @param aFile file handle that shall point to image file.
        *        Client should close the file after use.
        * @return KErrNone if successful, Symbian error code otherwise.
        * @since S60 v5.0
        */
        virtual TInt OpenDefaultVtImage( RFile& aFile ) = 0;

      protected:
                               
        /**
        * C++ constructor.
        */
       CPhCltImageHandler();
    };


#endif      // CPHCLTIMAGEHANDLER_H

// End of File
