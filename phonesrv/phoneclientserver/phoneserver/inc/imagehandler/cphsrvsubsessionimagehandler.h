/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Subsession for image handler.
*
*/

#ifndef CPHSRVSUBSESSIONIMAGEHANDLER_H
#define CPHSRVSUBSESSIONIMAGEHANDLER_H

// INCLUDES
#include    "cphsrvsubsessionbase.h" 
#include    "cphsrvimagesaver.h"
#include    "mphsrvimagesaverobserver.h"
#include   <cphcltbaseimageparams.h> 
#include    <bautils.h>
#include    <fbs.h>
#include    <phoneclientserverdomainpstypes.h> 
#include    <phoneclientserverdomainpskeys.h> 

// CLASS DECLARATION

/**
*  Image handler Subsession.
*
*  @since Series60 3.0
*/
class CPhSrvSubSessionImageHandler
:   public CPhSrvSubSessionBase, public MPhSrvImageSaverObserver
    {
    NONSHARABLE_CLASS( CPhSrvOperatorLogoContainer ) : public CBase
        {
        public:  // Constructors and destructor
        
            /**
            * Destructor.
            */
            ~CPhSrvOperatorLogoContainer();
            
            /**
            * Constructor.
            * @param aCountryCode country code for operator logo
            * @param aNetworkCode network code for operator logo
            * @param aBitmap bitmap to store.
            */
            CPhSrvOperatorLogoContainer( 
                const TPhCltCountryCode aCountryCode,
                const TPhCltNetworkCode aNetworkCode,
                const TPhCltExtOperatorLogoType aLogoType, 
                CFbsBitmap* aBitmap );
                
            // Stored image.
            CFbsBitmap* iBitmap;
            
            // Operator logo country code.
            TPhCltCountryCode iCountryCode;
            
            // Operator logo network code.
            TPhCltNetworkCode iNetworkCode;
            
            // Operator logo type.
            TPhCltExtOperatorLogoType iLogoType;                    
        };    
            
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * 
        * @param aSession The pre-opened Phone Server session.
        * @return CPhSrvSubSessionImageHandler subsession.
        */
        static CPhSrvSubSessionBase* NewL(CPhSrvSession& aSession );

        /**
        * Destructor.
        *
        */
        ~CPhSrvSubSessionImageHandler();
    
    public: // from MPhSrvImageSaverObserver
        /**
         * @see MPhSrvImageSaverObserver::ImageSaved
         */
        virtual void ImageSaved( TInt aResult );
                
    private: // Functions from base classes.

        /**
        * @see MPhSrvMessageDecoder
        */
        TBool PhSrvMessageDecoderCanProcessMessage( TInt aFunction );

        /**
        * @see MPhSrvMessageProcessor
        */
        void PhSrvMessageProcessorHandleMessageL( const RMessage2& aMessage );

    private: // SubSession function handlers.

        /**
        * Handle the close command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionClose( const RMessage2& aMessage );

        /**
        * Handle the save command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionSaveImagesL( const RMessage2& aMessage );

        /**
        * Handle the load command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionLoadImagesL( const RMessage2& aMessage );
                                          
        /**
         * Open default VT image file and transfer handle to client.
         */
        void CmdSubSessionOpenDefaultVtImageL( const RMessage2& aMessage );
        
        /**
         * Resolve default VT image file name.
         */
        TInt ResolveDefaultVtImageFileName( TDes& aFileName );
                         
        /**
         * Saves user defined VT image
         */
        void CmdSubSessionSaveVtImageL( const RMessage2& aMessage );
                         
        /**
        * Complete the load command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionCommitLoadL( const RMessage2& aMessage );

    private: // New functions.
    
        /** 
        * Updates Publish & Subscribe information with an event
        * happened to a operator logo.
        * @param aCountryCode country code for a logo
        * @param aNetworkCode network code for a logo
        * @param aEvent event that occurred
        * @return Symbian error code.
        */
        TInt NotifyListeners( 
            const TPhCltCountryCode aCountryCode, 
            const TPhCltNetworkCode aNetworkCode,
            const 
                TTelephonyOTALogoUpdate::TPSOperatorLogoUpdateEvent aEvent );
                
        /** 
        * Updates Publish & Subscribe information with an event
        * happened to a operator logo.
        * @param aCountryCode country code for a logo
        * @param aNetworkCode network code for a logo
        * @param aEvent event that occurred
        * @return Symbian error code.
        */
        TInt NotifyListeners( 
            const TPhCltCountryCode aCountryCode, 
            const TPhCltNetworkCode aNetworkCode,
            const 
                TTelOTALogoUpdate::TPSOperatorLogoUpdateEvent aEvent );
    
        /**
        * Parses operator logo file name and path.
        * @param aCountryCode country code to use in deletion
        * @param aNetworkCode network code to use in deletion
        * @param aLogoType type of logo to delete
        * @param aFileName file name in return
        */        
        void ConstructLogoFileNameL( 
            const TPhCltCountryCode aCountryCode, 
            const TPhCltNetworkCode aNetworkCode,
            const TPhCltExtOperatorLogoType aLogoType,
            TDes& aFileName );
    
        /**
        * Converts given logo type to file name id.
        * @param aLogoType logo type to convert'
        * @param aType type in return
        */
        void AppendLogoTypeAsTextL( 
            const TPhCltExtOperatorLogoType aLogoType, TDes& aType );
    
        /**
        * Checks logo deletion and deletes if needed.
        * @param aCountryCode country code to use in deletion
        * @param aNetworkCode network code to use in deletion
        * @param aLogoType type of logo to delete
        * @param aFileHandle file handle to be checked
        * @return ETrue if deleted
        */
        TBool DeleteLogoL( 
            const TPhCltCountryCode aCountryCode, 
            const TPhCltNetworkCode aNetworkCode, 
            const TPhCltExtOperatorLogoType aLogoType,
            const TInt aFileHandle );
    

        /**
        * Does saving of video telephony images.
        * 
        * @param aImages Image handle array.
        */
        void DoSaveVTImageL( TPhCltImageArray& aImages );

        /**
        * Loads a video telephony image from file.
        * @param aImages image handle array to return.
        */
        void DoLoadVTImagesL( TPhCltImageArray& aImages );

        /**
        * Reads video telephony images from private path.
        * @param aDefaultImage ETrue reads default image
        *                      EFalse reads user defined image
        */
        void DoReadVTImagesL( const TBool aDefaultImage );
        
        /**
        * Constructs search path for VT still/default image.
        * It contains image name with wild card.
        * @param aDefaultImage ETrue path for default image
        *                      EFalse path for still image
        * @param aPath returns plain drive+path w/o wild cards
        * @return search path
        */
        HBufC* ConstructVTImageSearchPathLC( 
            const TBool aDefaultImage,
            TPtrC& aPath ) const;

        /**
        * Does saving of operator logos.
        * 
        * @param aImages Image handle array.
        */
        void DoSaveOperatorLogoL( TPhCltImageArray& aImages );

        /**
        * Loads operator logo image from file.
        * @param aImages image handle array to return.
        */
        void DoLoadOperatorLogoL( TPhCltImageArray& aImages );

        /**
        * Completes operator logo loading, and releases resources.
        * @param aImages image handle array.        
        */
        void DoCommitLoadLogo( TPhCltImageArray& aImages );
        
        /**
        * Completes vt image loading, and releases resources.    
        */
        void DoCommitLoadVTL();

        /**        
        * Creates image storage directory.
        */
        void MakeImageDirL();        
        
    private: 

        /**
        * Constructor.
        *
        * @param aSession The session where this subsession belongs.
        */
        CPhSrvSubSessionImageHandler( CPhSrvSession& aSession );

        /**
        * Symbian constructor that may leave.
        */
        void ConstructL();

    private: // Data.

        // Stored VT bitmaps
        RPointerArray<CFbsBitmap> iVTBitMaps;
        
        // Stored operator logos.
        RPointerArray<CPhSrvOperatorLogoContainer> iOperatorLogos;
        
        // File server handle.
        RFs iFs;

        // Path to privates.
        TFileName iFilePath;
        
        // Used for defining pubsub key.
        TBool  iPropertyDefined;

        //System drive name 
        TFileName iDriveName;
                                      
        // Pointer to object performing image saving. Not owned.
        CPhSrvImageSaver* iImageSaver;          
        
        // message entry for VT image saving
        RMessage2 iSaveVtImageMessage;
    };

#endif // CPHSRVSUBSESSIONIMAGEHANDLER_H


// End of File
