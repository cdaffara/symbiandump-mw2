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
 * Description: Declaration of the Positioning Settings signal mapper 
 *              used to map the signals of each check box item to the index
 *
 */

#ifndef POSSETTINGSSIGNALMAPPER_H
#define POSSETTINGSSIGNALMAPPER_H

#include <QObject>

class PosSettingsSignalMapper : public QObject
    {
    Q_OBJECT
public:
    /*
     * constructor
     */
    PosSettingsSignalMapper();
    /**
     * destructor
     */
    ~PosSettingsSignalMapper();
public:
signals:
    /**
     * signal emitted by the slot of this class
     */
    void signalmapped();
public slots:
    /**
     * slot to be called on a change in state of checkbox.
     * this in turn calls the signal signalmapped()
     */
    void signalmap();
    };

#endif //POSSETTINGSSIGNALMAPPER_H
