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
* Description:  CMnrpAppView class definition 
*
*/


#ifndef MNRP_APPVIEW_H
#define MNRP_APPVIEW_H

#include <aknview.h>

class CMnrpAppControl;

/** Empty view for the case when application is started
 *	from menu*/
class CMnrpAppView : public CAknView
    {
    public:

        static CMnrpAppView* NewL();
        virtual ~CMnrpAppView();

    public: // from CAknView

        TUid Id() const;

        void DoActivateL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        void DoDeactivate();

    private:

        CMnrpAppView();
        void ConstructL();
        
    private:
    
    	/** UI control */
        CMnrpAppControl* iControl;    

    };

#endif // MNRP_APPVIEW_H

