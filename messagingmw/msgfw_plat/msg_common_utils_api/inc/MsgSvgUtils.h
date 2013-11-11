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
* Description:  
*           Miscellaneous SVG-T related utility methods.
*
*/




#ifndef MSGSVGUTILS_H
#define MSGSVGUTILS_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>
#include <SVGRequestObserver.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CFbsBitmap;
class MSvgError;

/**
* CMsgSvgUtils - Miscellaneous UI indenependent utilities.
*
* @lib MsgCommonUtils.lib
* @since 3.2
*/
NONSHARABLE_CLASS( CMsgSvgUtils ): public CBase,
                                   public MSvgRequestObserver
            
    {
     
    public:  // Constructors and destructor
        
        /**
        * C++ constructor
        *
        * @since 3.2
        */
        IMPORT_C CMsgSvgUtils();
        
        /**
        * Destructor.
        */
        virtual ~CMsgSvgUtils();
   
   public: // New functions 
        
        /**
        * Generates thumbnail from given SVG-T file
        * 
        * @since 3.2
        *
        * @param    aFile                   IN      Handle to SVG-T file.
        * @param    aBitmap                 IN      Bitmap where thumbnail is generated.
        * @param    aMask                   IN      Mask bitmap where thumbnail's mask is generated.
        * @param    aAutomaticDrmControl    IN      ETrue if SVG engine is responsible of handling DRM rights,
                                                    EFalse if manual control is wanted
        * @param    aContentSize            IN/OUT  IN : Should contain the maximum wanted size 
        *                                           OUT: Contains real thumbnail size.
        */
        IMPORT_C void GenerateThumbnailL( RFile& aFile,
                                          CFbsBitmap* aBitmap,
                                          CFbsBitmap* aMask,
                                          TBool aAutomaticDrmControl,
                                          TSize& aContentSize );
    
    public: // Functions from base classes
    
        /**
        * From MSvgRequestObserver. See SVGRequestObserver.h for more information.
        */
        void UpdateScreen();

        /**
        * From MSvgRequestObserver. See SVGRequestObserver.h for more information.
        */
        TBool ScriptCall( const TDesC& aScript, CSvgElementImpl* aCallerElement );
        
        /**
        * From MSvgRequestObserver. See SVGRequestObserver.h for more information.
        */
        TInt FetchImage( const TDesC& aUri, RFs& aSession, RFile& aFileHandle );
        
		/**
        * From MSvgRequestObserver. See SVGRequestObserver.h for more information.
        */
		TInt FetchFont( const TDesC& aUri, RFs& aSession, RFile& aFileHandle );

        /**
        * From MSvgRequestObserver. See SVGRequestObserver.h for more information.
        */
        void UpdatePresentation( const TInt32&  aNoOfAnimation );
    
    private:
        
        /**
        * Performs SVG engine error handling.
        *
        * @since 3.2
        */
        void HandleSVGEngineErrorL( MSvgError* aError ) const;
    
    private: // Data
    
    };

#endif   // MSGSVGUTILS_H

// End of file
