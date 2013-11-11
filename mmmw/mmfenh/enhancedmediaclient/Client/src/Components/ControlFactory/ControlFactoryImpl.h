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
* Description:  Definition of the ControlFactoryImpl class.
*
*/


#ifndef C_CCONTROLFACTORYIMPL_H
#define C_CCONTROLFACTORYIMPL_H

#include <e32base.h>

namespace multimedia
    {
    // Forward declaration
    class MStreamControl;
    class MSourceControl;
    class MEffectControl;
    class MDataBuffer;
    class MSinkControl;
    class CMultimediaFactoryImpl;
    
    /**
    *  Implements the MultimediaFactor Implementation
    *  @lib EnhancedMediaClient.lib
    *  @since S60 v3.2
    */    
    class CMultimediaFactoryImpl : public CBase                                             
        {
        public:
            /**
             * Function to Create the Object.
             */
            static CMultimediaFactoryImpl* NewL();

            /**
            * Destructor.
            */
            ~CMultimediaFactoryImpl();
            
            /**
            * Creates the StreamControl 
            *
            * @since S60 v3.2
            * @param aType      UID of Stream Type
            * @param aControl   Actual Pointer returned
            */            
            TInt CreateStreamControl( TUid aType, MStreamControl*& aControl );
            /**
            * Deletes the StreamControl 
            *
            * @since S60 v3.2
            * @param aControl   Stream Control to be Deleted
            */            
            TInt DeleteStreamControl( MStreamControl*& aControl );
            /**
            * Creates the SourceControl 
            *
            * @since S60 v3.2
            * @param aType      UID of Source Type
            * @param aControl   Actual Pointer returned
            */            
            TInt CreateSourceControl( TUid aType, MSourceControl*& aControl );
            /**
            * Deletes the SourceControl 
            *
            * @since S60 v3.2
            * @param aControl   Source Control to be Deleted
            */            
            TInt DeleteSourceControl( MSourceControl*& aControl );
            /**
            * Creates the SinkControl 
            *
            * @since S60 v3.2
            * @param aType      UID of Sink Type
            * @param aControl   Actual Pointer returned
            */            
            TInt CreateSinkControl( TUid aType, MSinkControl*& aControl );
            /**
            * Deletes the SinkControl 
            *
            * @since S60 v3.2
            * @param aControl   Sink Control to be Deleted
            */            
            TInt DeleteSinkControl( MSinkControl*& aControl );
            /**
            * Creates the EffectControl 
            *
            * @since S60 v3.2
            * @param aType      UID of Effect Type
            * @param aControl   Actual Pointer returned
            */            
            TInt CreateEffectControl( TUid aType, MEffectControl*& aControl );
            /**
            * Deletes the EffectControl 
            *
            * @since S60 v3.2
            * @param aControl   Effect Control to be Deleted
            */            
            TInt DeleteEffectControl( MEffectControl*& aControl );
            /**
            * Creates the MDataBuffer
            *
            * @since S60 v3.2
            * @param aType      UID of Buffer Type
            * @param aSize      Size of the Buffer
            * @param aBuffer   Actual Pointer returned
            */            
            TInt CreateDataBuffer( TUid aType, TUint aSize, MDataBuffer*& aBuffer );
            /**
            * Deletes the MDataBuffer
            *
            * @since S60 v3.2
            * @param aControl   Buffer to be Deleted
            */            
            TInt DeleteDataBuffer( MDataBuffer*& aBuffer );
            
        private:
            CMultimediaFactoryImpl();
            void ConstructL();
                
        };
    } // namespace multimedia

#endif // C_CCONTROLFACTORYIMPL_H

// End of file
