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
* Description:  Implementation of the PhCltExtension Image handler interface.
*
*/



#ifndef CPHCLTIMAGEHANDLERIMPLEMENTATION_H
#define CPHCLTIMAGEHANDLERIMPLEMENTATION_H


//  INCLUDES
#include    <cphcltimagehandler.h> 
#include    "rphcltimagehandler.h" 
#include    "rphcltserver.h" 

/**
*  Implementation of image handler interface.
*
*  @lib PhoneClient.lib
*  @since Series60 3.2
*/
NONSHARABLE_CLASS ( CPhCltImageHandlerImplementation )
:   public CPhCltImageHandler, public MPhCltImageObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPhCltImageHandlerImplementation* NewL();

        /**
        * Destructor.
        */
        ~CPhCltImageHandlerImplementation();

    public: // Functions from base classes

        /**
        * @see CPhCltImageHandler.
        */        
        TInt SaveImages( CPhCltImageParams& aParams );
            
        /**
        * @see CPhCltImageHandler.
        */
        TInt LoadImages( CPhCltImageParams*& aParams );

        /**
        * @see CPhCltImageHandler::CPhCltBaseImageParamsL().
        */
        virtual CPhCltImageParams* 
            CPhCltBaseImageParamsL( const TPhCltImageType aType ); 
        
        /**
        * @see CPhCltImageHandler::OpenDefaultVtImage().
        */
        virtual TInt OpenDefaultVtImage( RFile& aFile );
        
        /**
        * @see CPhCltImageHandler::SaveVtImage()
        */
        virtual void SaveVtImageL( MPhCltImageObserver& aObserver,
            const TDesC& aImagePath );

        /**
        * @see CPhCltImageHandler::CancelSaveVtImage
        */
        virtual void CancelSaveVtImage();
                
        /**
        * @see CPhCltImageHandler::OpenVtImage()
        */
        virtual TInt OpenVtImage( RFile& aFile );
    
    public: // from MPhCltImageObserver    
    
        virtual void HandleImageSaveCompleteL( TInt aResult );
        
    private:

        /**
        * C++ constructor.
        */
        CPhCltImageHandlerImplementation();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Common code for opening default and user defined image.
        */                               
        TInt OpenVtImageCommon( 
            RFile& aFile, 
            const TBool aDefaultImage );

        /**
        * Internal class for handling asynchronous image saving.
        */
        class CVtImageSaver : public CActive
        {
        public:

            /** Saves VT image. Deletes itself after observer callback 
             * or cancellation */
            static CVtImageSaver* SaveLD(
                MPhCltImageObserver& aObserver,
                RPhCltImageHandler& aHandler,
                const TDesC& aImagePath );
            
        private:
            CVtImageSaver( 
                MPhCltImageObserver& aObserver,
                RPhCltImageHandler& aHandler );
            
            ~CVtImageSaver();
        
        private:
            void DoSave( const TDesC& aImagePath );
            
        private: // from CActive
        
            /** handles observer callback */
            void RunL();
            
            /**  */
            void DoCancel();
            
        private:
        
        MPhCltImageObserver& iObserver;
        RPhCltImageHandler& iHandler;
        };
        
    private: // Data
        RPhCltImageHandler iStillHandler;
        
        //RPhCltServer class instance creates phonesession
        RPhCltServer iServer;

        // Observer for async image saving
        MPhCltImageObserver* iObserver;
                
        // not owned
        CVtImageSaver* iVtImageSaver;
        

    };

#endif      // CPHCLTIMAGEHANDLERIMPLEMENTATION_H

  
// End of File
