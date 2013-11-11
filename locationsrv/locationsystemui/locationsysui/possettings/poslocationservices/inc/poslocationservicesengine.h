/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Class declaration of PosLocationServicesEngine.
*
*/
#ifndef POSLOCATIONSERVICESENGINE_H
#define POSLOCATIONSERVICESENGINE_H

#include "poslocationservicesengineprivate.h"
#include <QObject>



/**
 * PosLocationServicesEngine class provides interface to location services.
 * Using this interface. This interface provides services like list, delete
 * etc..
 */
class PosLocationServicesEngine : public QObject,
                                  public PosLocationServiceUpdateObserver
    {
    Q_OBJECT
public:
    /**
     * Constructor
     */
    PosLocationServicesEngine();

    /**
     * Destructor
     */
    virtual ~PosLocationServicesEngine();

    /**
     * Retrieves all location services info.
     *
     * @param[out] locationServiceList List of location
     *            services info.
     * @return  - KErrNone if location services information retrieved successfully.
     *          - KErrNotFound if there are no location services.
     *            otherwise system wide error codes.
     */
    int getLocationServices( QList<PosLocationServiceInfo>&
                             locationServiceList );

    /**
     * Removes location service based on the service Id.
     *
     * @param [in] serviceId Location service Id.
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if service is not found
     *      otherwise system wide error codes.
     */
    int removeLocationService( int serviceId );

public:// From PosLocationServiceUpdateObserver
    void handleLocationServiceUpdate();

signals:
    /**
     * This signal is emitted when there is update in the
     * location services.
     */
    void locationServiceUpdate();

private:// Data members
    /**
     * Pointer to PosLocationServicesEnginePrivate object.
     * Own
     */
    PosLocationServicesEnginePrivate* d_ptr;
    };

#endif /* POSLOCATIONSERVICESENGINE_H */
