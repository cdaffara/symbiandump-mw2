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
* Description: Provides SAT user confim notes
*
*/

#ifndef SATAPPCONFIRMPROVIDER_H
#define SATAPPCONFIRMPROVIDER_H

// includes
#include <QObject>

// forward declarations
class HbMessageBox;
class SatAppAction;

class SatAppConfirmProvider : public QObject
{
    Q_OBJECT

public:
    /** Constructor */
    SatAppConfirmProvider(QObject *parent = 0);

    /** Destructor */
    virtual ~SatAppConfirmProvider();

public slots:

    /**
    * General confirmation request
    * @param userAccepted Indicates whether the command was accepted.
    */
    void confirmCommand(SatAppAction &action);

    /** clear the current dialog in screen */
    void clearScreen();

    /** reset some data members */
    void resetState();

private:

    /** Shows the confirm note about Open Channel */
    void showOpenChannelConfirm(SatAppAction &action);

    /** Shows the confirmation not about SetUpCall*/
    void showSetUpCallConfirm(SatAppAction &action);

private:

    /** Own.*/
    HbMessageBox *mConfirmQuery;
    
    SatAppAction *mAction;

    /** For unit testing.*/ 
    friend class ConfirmProviderTest;
};

#endif /* SATAPPCONFIRMPROVIDER_H */
