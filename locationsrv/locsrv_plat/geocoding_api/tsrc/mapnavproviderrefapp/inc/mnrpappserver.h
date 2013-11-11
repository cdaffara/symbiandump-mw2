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
* Description:  CMnrpAppServer class definition
*
*/


#ifndef MNRP_APPSERVER_H
#define MNRP_APPSERVER_H

#include <aknserverapp.h>

class CMnrpAppUi;
class CMnrpNavigationService;
class CMnrpMapViewService;

/** Application Server class of MnRefProvider component */
class CMnrpAppServer : public CAknAppServer
    {
    public:
        ~CMnrpAppServer();
        
        CMnrpAppUi* AppUi();

    public: // from CAknAppServer
    	/** Creates new service class and returns pointer to it */
        IMPORT_C CApaAppServiceBase* CreateServiceL(TUid aServiceType) const;
        
        /** Overrides base function. Doesn't close application evenb if all
         *	clients have disconnected until user closes it herself. 
         *	It is important for standalone execution, when calling application
         *	may be closed, but user still wants to continue with Map or Navigation.
         */
        IMPORT_C void HandleAllClientsClosed();
        
    protected:        
    	/** Creates new navigation service class and returns pointer to it */
        CMnrpNavigationService* CreateNavigationServiceL() const;
    	/** Creates new map view service class and returns pointer to it */
        CMnrpMapViewService* CreateMapViewServiceL() const;
    };

#endif // MNRP_APPSERVER_H

