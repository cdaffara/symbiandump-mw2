/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Stub header for CallUiEngine testing.
*
*/


#ifndef FEATMGR_STUB_H
#define FEATMGR_STUB_H


//  INCLUDES
#include    <e32base.h>


/**
* FeatureManager
*/
class FeatureManager
    {
    public:

        static void InitializeLibL();
        static void UnInitializeLib();
        static TBool FeatureSupported( TInt aFeature );


    public:

        static TBool iFeatureIdCsVideoTelephony;
        static TBool iBtAudioSupported;
        static TBool iHelpSupported;

    };


#endif // FEATMGR_STUB_H
            
// End of File