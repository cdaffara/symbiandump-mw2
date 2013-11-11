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
 * Description: Declaration of the Positioning Methods engine class
 *							used to get the list of available Psy & to manipulate 
 *							the state of the Psy.
 *							
 *
 */

#ifndef POSPSYSETTINGSENGINE_H
#define POSPSYSETTINGSENGINE_H

#include "pospsysettingsengineprivate.h"

#include <QObject>
#include <qglobal.h>

class PosPsySettingsEngine : public QObject,
        public PositionTechnologyChangeObserver
    {
Q_OBJECT

public:
    /**
     * Constructor
     */
    PosPsySettingsEngine();

    /**
     * Destructor
     */

    ~PosPsySettingsEngine();

    /**
     * This method lists all the PSY that are available
     * @param[out] psyList - Contains the list of psys that are available
     * @return - KErrNone - if successfully finds any Psy
     *           KErrNotFound - if no Psy available.
     */
    int listPsys(QList<PosPsyInfo>& psyList);

    /**
     * This method is used to change the state of the Psy based on the user's
     * action of either enabling/disabling the Psy.
     * @param[in] psyModuleId - the uid of the Psy whose state is to be changed.
     * @param[in] psyState - The value of state to which the Psy has to be changed.
     */

    int changePsyState(TUid psyModuleId, PosPsyState psyState);
    
public: // from PositionTechnologyChangeObserver
    void handlePositionTechnologyChange();

private:
signals:
    /**
     * This signal is emitted when there is change in PSY wrt to 
     * its availabilty, state etc.. 
     */
    void PSYChange();
private:
    Q_DECLARE_PRIVATE_D(d_ptr, PosPsySettingsEngine)
private:
    /**
     * POinter to the PosPsySettingsEnginePrivate
     * Owns
     */
    PosPsySettingsEnginePrivate* d_ptr;
    };

#endif /* POSPSYSETTINGSENGINE_H */
