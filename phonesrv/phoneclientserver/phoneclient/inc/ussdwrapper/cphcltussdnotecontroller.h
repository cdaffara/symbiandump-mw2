/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsulates an active object used in controlling notes.
*
*/


#ifndef CPHCLTUSSDNOTECONTROLLER_H
#define CPHCLTUSSDNOTECONTROLLER_H

// INCLUDES
#include <e32base.h>
#include <hbdeviceprogressdialogsymbian.h>
#include "mphcltussdnotecontrollercallback.h" 
#include "cphcltussdcommonconstant.h"


// CLASS DECLARATION

/**
*   Encapsulates an active object used in controlling notes.
*
*  @lib phoneclient.lib
*/
NONSHARABLE_CLASS( CPhCltUssdNoteController ) : public CBase,
                                                public MHbDeviceProgressDialogObserver
    {
    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * 
        * @param aCallBack a callback to be notified events from note controller.
        * @param aPriority active object priority.
        * @return Pointer to created CPhCltUssdImp instance.
        */
        static CPhCltUssdNoteController* NewL( 
                MPhCltUssdNoteControllerCallBack& aCallBack );

        /**
        * Destructor.
        */
        ~CPhCltUssdNoteController();

    public:

        /**
        * Shows global Information note.
        */
        void ShowGlobalInformationNoteL( const TPhCltUssdInformationType aInfoType );

        /**
        * Shows global wait note.
        */
        void ShowGlobalWaitNoteL();

        /**
        * Destroys global wait note.
        */
        void DestroyGlobalWaitNote();

        /**
        * From base class MHbDeviceProgressDialogObserver
        */
        void ProgressDialogCancelled(
            const CHbDeviceProgressDialogSymbian* aProgressDialog);

        /**
        * From base class MHbDeviceProgressDialogObserver
        */
        void ProgressDialogClosed(
            const CHbDeviceProgressDialogSymbian* aProgressDialog);

    private:

        /**
        * C++ constructor.
        */
        CPhCltUssdNoteController( 
            MPhCltUssdNoteControllerCallBack& aCallBack );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

    private:    // Data

        /**
        * Own,Global wait note.
        */
        CHbDeviceProgressDialogSymbian* iGlobalWaitNote;

        /**
        * Not own,callback to be notified when wait note is dismissed.
        */
        MPhCltUssdNoteControllerCallBack& iCallBack;

    };

#endif // CPHCLTUSSDNOTECONTROLLER_H

// End of File
