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
* Description: Provides and maintains SAT main view (selection list)s
*
*/

#ifndef SATAPPMENUPROVIDER_H
#define SATAPPMENUPROVIDER_H

#include <QObject>
#include <msatuiobserver.h> //for TSatUiResponse

class SatAppAction;
class HbMainWindow;
class HbListWidgetItem;
class HbView;
class QEventLoop;
class QTimer;
class HbMainWindow;
class HbAction;
class HbListWidget;
class HbLabel;
class QPixmap;

class SatAppMenuProvider : public QObject
{
    Q_OBJECT

public:

    SatAppMenuProvider(HbMainWindow *window, QObject *parent = 0);

    virtual ~SatAppMenuProvider();

    /** takes a screen shot of the menu */
    QPixmap takeScreenShot();

public slots:

    /** signal request to display a menu */
    void setUpMenu(SatAppAction &action);
    
    /** requests a list selection menu to be displayed */
    void selectItem(SatAppAction &action);

    void resetState();

private slots:
    void menuItemSelected(HbListWidgetItem *item);

private:

    void loadMainView();
    void switchView();
    void connectItem();
    void disconnectItem();

private:

    QList<QObject *> mObjects;
    HbMainWindow *mMainWindow;    
    HbView *mSetupMenuView;
    HbView *mSelectItemView;
    HbAction *mSoftKeyQuitAction;
    HbAction *mSoftKeyBackAction;
    HbListWidget *mListWidget;
    HbListWidget *mSelectListWidget;
    HbLabel *mSubTitle;
    SatAppAction* mCurrentAction; // not own

    // for unit testing
    friend class MenuProviderTest;
};

#endif /* SATAPPMENUPROVIDER_H */
