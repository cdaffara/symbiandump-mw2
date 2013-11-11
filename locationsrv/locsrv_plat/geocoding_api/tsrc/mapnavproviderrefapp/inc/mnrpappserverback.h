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
* Description:  CMnrpAppServerBack class definition
*
*/


#ifndef MNRP_APPSERVERBACK_H
#define MNRP_APPSERVERBACK_H

#include <aknserverapp.h>

class CMnrpGeocodingService;
class CMnrpEngine;

#ifdef RD_MAPNAV_BITMAP_ACCESS
class CMnrpMapImageService;
#endif

/** Application Server class of MnRefGeocoder component
 *	It ows Engine instance.
 */
class CMnrpAppServerBack : public CAknAppServer
    {
    public:
        static CMnrpAppServerBack* NewL();
        ~CMnrpAppServerBack();

    public: // from CAknAppServer
    	/** Creates new geocoding service class and returns pointer to it */
        IMPORT_C CApaAppServiceBase* CreateServiceL( TUid aServiceType ) const;

    protected:
        void ConstructL();

    	/** Creates new geocoding service class and returns pointer to it */
        CMnrpGeocodingService* CreateGeocodingServiceL() const;

#ifdef RD_MAPNAV_BITMAP_ACCESS
    	/** Creates new map image service class and returns pointer to it */
        CMnrpMapImageService* CreateMapImageServiceL() const;
#endif

    private:
        CMnrpEngine* iEngine;
    };

#endif // MNRP_APPSERVERBACK_H

