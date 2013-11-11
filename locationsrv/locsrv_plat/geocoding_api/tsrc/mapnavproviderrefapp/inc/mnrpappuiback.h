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
* Description:   class definition
*
*/


#ifndef MNRP_APPUIBACK_H
#define MNRP_APPUIBACK_H

#include <aknviewappui.h>

/** Simple App UI class of the MnRefGeocoder component */
class CMnrpAppUiBack : public CAknAppUi
    {
    public:
        void ConstructL();

        /**  Moves application to foreground */
        void MoveForeground();
        
        /**  Moves application to background */
        void MoveBackground();

    public: // from CEikAppUi

        void HandleCommandL(TInt aCommand);
    };

#endif // MNRP_APPUIBACK_H

