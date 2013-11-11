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
* Description:  CMnrpAppControl class definition 
*
*/


#ifndef MNRP_APPCONTROL_H
#define MNRP_APPCONTROL_H

#include <coecntrl.h>
class CMnrpAppView;

/** Empty view for the case when application is started
 *	from menu */
class CMnrpAppControl : public CCoeControl
    {
    public:

        static CMnrpAppControl* NewL( CMnrpAppView& aView, const TRect& aRect );

        virtual ~CMnrpAppControl();

    public:  // From CCoeControl

        TInt CountComponentControls() const;
        CCoeControl* ComponentControl( TInt aIndex ) const;
        void HandleResourceChange( TInt aType );

        void Draw( const TRect& aRect ) const;
        
    private:

        void ConstructL( const TRect& aRect );
        CMnrpAppControl( CMnrpAppView& aView );
        
    private:
        CMnrpAppView&   iView;
    };

#endif // MNRP_APPCONTROL_H

