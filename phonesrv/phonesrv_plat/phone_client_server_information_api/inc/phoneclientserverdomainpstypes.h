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
* Description:  Data types passed through Publish and Subscribe
*
*/


#ifndef PHONECLIENTSERVERDOMAINPSTYPES_H
#define PHONECLIENTSERVERDOMAINPSTYPES_H

/**
* Details how OTA operator logo has changed.
* Data is used as key value with KTelOperatorLogoUpdated.
* Note: Size of the data must not exceed RProperty::KMaxPropertySize.
*/
struct TTelOTALogoUpdate 
    {
    /**
    * EPSTelephonyLogoChanged - Logo was added or replaced
    * EPSTelephonyLogoDeleted - Logo was deleted
    */
    enum TPSOperatorLogoUpdateEvent
        {
        EPSTelephonyLogoChanged,
        EPSTelephonyLogoDeleted,
        EPSTelephonyAllLogosDeleted
        };
    
    // Event type
    TPSOperatorLogoUpdateEvent iEvent;

    /**
    * Country code associated to logo, N/A if event
    * is EPSTelephonyAllLogosDeleted
    */ 
    TPhCltCountryCode iMCC;

    /**
    * Network code associated to logo, N/A if event
    * is EPSTelephonyAllLogosDeleted
    */ 
    TPhCltNetworkCode iMNC;

    typedef TPckg<TTelOTALogoUpdate> TOTALogoUpdatePckg;
    };
    
#endif    // PHONECLIENTSERVERDOMAINPSTYPES_H

// End of file