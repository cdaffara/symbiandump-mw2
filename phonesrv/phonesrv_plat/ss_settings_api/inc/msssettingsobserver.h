/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for getting observer events.
*
*/


#ifndef MSSSETTINGSOBSERVER_H
#define MSSSETTINGSOBSERVER_H

//  INCLUDES
#include <rsssettings.h> 

// CLASS DECLARATION

/**
* It's the observer interface for some phone related settings.
* @lib sssettings.lib
* @since 1.0
*/
class MSSSettingsObserver
    {
    public: // New functions
        /*
        * Observer functoin 
        * @param aSetting Current setting
        * @param aNewValue New value for the setting
        */
        virtual void PhoneSettingChanged( 
            TSSSettingsSetting aSetting, 
            TInt aNewValue ) = 0;
    };

#endif //MSSSETTINGSOBSERVER_H 
            
// End of File
