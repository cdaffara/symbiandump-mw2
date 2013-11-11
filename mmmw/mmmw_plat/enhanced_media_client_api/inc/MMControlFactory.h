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
* Description:  Header of ControlFactory class.
*
*/


#ifndef MMCONTROLFACTORY_H
#define MMCONTROLFACTORY_H

#include <e32base.h>

namespace multimedia
    {
    // Forward declaration
    class MStreamControl;
    class MSourceControl;
    class MSinkControl;
    class MEffectControl;
    class MDataBuffer;
    class CMultimediaFactoryImpl;
    
    class CMultimediaFactory
        {
        public:
            IMPORT_C static TInt CreateFactory( CMultimediaFactory*& aFactory );
            IMPORT_C ~CMultimediaFactory();
                    
            IMPORT_C TInt CreateStreamControl( TUid aType, MStreamControl*& aControl );
            IMPORT_C TInt DeleteStreamControl( MStreamControl*& aControl );
            IMPORT_C TInt CreateSourceControl( TUid aType, MSourceControl*& aControl );
            IMPORT_C TInt DeleteSourceControl( MSourceControl*& aControl );
            IMPORT_C TInt CreateSinkControl( TUid aType, MSinkControl*& aControl );
            IMPORT_C TInt DeleteSinkControl( MSinkControl*& aControl );
            IMPORT_C TInt CreateEffectControl( TUid aType, MEffectControl*& aControl );
            IMPORT_C TInt DeleteEffectControl( MEffectControl*& aControl );
            IMPORT_C TInt CreateDataBuffer( TUid aType, TUint aSize, MDataBuffer*& aBuffer );
            IMPORT_C TInt DeleteDataBuffer( MDataBuffer*& aBuffer );
        private:
            CMultimediaFactory();
            void ConstructL();
        
        private:
            CMultimediaFactoryImpl* iCMultimediaFactoryImpl;
                        
        };
    } // namespace multimedia

#endif // MMCONTROLFACTORY_H

// End of file
