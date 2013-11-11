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
* Description:  Implementation of CDevSoundIf
*
*/

// Version : %version: 4 %



#ifndef __DEVSOUNDIF_H
#define __DEVSOUNDIF_H

class CMMFDevSound;

NONSHARABLE_CLASS(CDevSoundIf) 
{


    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CDevSoundIf* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CDevSoundIf();

    public: // New functions
        
        /**
        * Get number of volume steps
        * @since 9.2
        * @return number of volume steps
        */
        IMPORT_C TInt GetNumberOfVolumeSteps();

    private:

        /**
        * C++ default constructor.
        */
        CDevSoundIf();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        CMMFDevSound* iDevSound;        
};

#endif      // __DEVSOUNDIF_H
            
// End of File
