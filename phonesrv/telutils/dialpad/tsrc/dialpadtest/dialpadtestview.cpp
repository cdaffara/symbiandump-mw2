/*!
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include <QtGui>

//#define DIALPAD_ENABLE_TESTABILITY
#ifdef DIALPAD_ENABLE_TESTABILITY
    #include <QtPlugin>
    #include <QPluginLoader>
    #include <QLibraryInfo>
    #include "testabilityinterface.h"
#endif

#include <hbtoolbar.h>
#include <hbaction.h>
#include <hblabel.h>
#include <hbdialog.h>
#include <hbmainwindow.h>
#include <hbmenu.h>
#include <hblistwidget.h>
#include <hblistwidgetitem.h>
#include <hblineedit.h>
#include <hbmessagebox.h>
#include <hbinputsettingproxy.h>
#include <hbextendedlocale.h>

#include "dialpadtestview.h"
#include "dialpad.h"
#include "dialpadkeyhandler.h"

DialpadTestView::DialpadTestView( HbMainWindow& mainWindow ) :
    mMainWindow(mainWindow), mTapOutsideDismiss(0), mArabicMode(0), mArabicDigit(0)
{
    setTitle("DialpadTest");

    // create toolbar with launch dialer action
    HbToolBar* tb = toolBar();
    if (mMainWindow.orientation()==Qt::Horizontal) {
        tb->setOrientation(mMainWindow.orientation());
    }

    tb->addAction("");
    tb->addAction(HbIcon(":/qtg_large_tb_dialler.svg"),"",this,SLOT(openDialpad()));
    tb->addAction("");


    // create menu
    menu()->addAction("Switch orientation",this,SLOT(switchOrientation()));
    connect(&mMainWindow,
            SIGNAL(orientationChanged(Qt::Orientation)),
            SLOT(onOrientationChange(Qt::Orientation)));

    menu()->addAction("Tap outside dismiss",this,SLOT(setTapOutsideDismiss()));

    menu()->addAction("Arabic mode",this,SLOT(setArabicMode()));

    menu()->addAction("Arabic digit",this,SLOT(setArabicDigit()));

    // create view widget (recent calls list mockup)
    createListWidget();

    // create dialpad popup
    mDialpad = new Dialpad(); // Note! no parent for popup
    mDialpad->setCallButtonEnabled(false);
    connect(mDialpad,SIGNAL(aboutToClose()),SLOT(onDialpadClosed()));
    connect(&mDialpad->editor(),SIGNAL(contentsChanged()),
            SLOT(onEditorContentChanged()));

    // create key event handler
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()),this,SLOT(handleLongKeyPress()));
    
    // mKeyhandler = new DialpadKeyHandler(mDialpad, mMainWindow, this);
    mMainWindow.installEventFilter(this);
}

DialpadTestView::~DialpadTestView()
{
    delete mDialpad;
#ifdef DIALPAD_ENABLE_TESTABILITY
    delete mTestabilityInterface;
#endif
}

void DialpadTestView::openDialpad()
{
    toolBar()->hide();
    setDialpadPosition();
    mDialpad->openDialpad();
}

void DialpadTestView::onDialpadClosed()
{
    toolBar()->show();
    mDialpad->editor().setText(QString());
}

void DialpadTestView::onEditorContentChanged()
{
    mDialpad->setCallButtonEnabled(
        mDialpad->editor().text().length());
}

void DialpadTestView::setDialpadPosition()
{
    QRectF screenRect(mMainWindow.layoutRect());

    if (mMainWindow.orientation() == Qt::Horizontal) {
        // dialpad takes half of the screen
        mDialpad->setPos(QPointF(screenRect.width()/2,
                                 this->scenePos().y()));
        mDialpad->resize(screenRect.width()/2,
                         (screenRect.height()-scenePos().y()));
    } else {
        qreal screenHeight = screenRect.height();
        mDialpad->setPos(0, screenHeight/2.25);
        mDialpad->resize(screenRect.width(),screenHeight - screenHeight/2.25);
    }
}

void DialpadTestView::switchOrientation()
{
    if (mMainWindow.orientation()==Qt::Vertical) {
        mMainWindow.setOrientation(Qt::Horizontal);

    } else {
        mMainWindow.setOrientation(Qt::Vertical);
    }
}

void DialpadTestView::onOrientationChange(Qt::Orientation orientation)
{
    if (orientation==Qt::Horizontal) {
        toolBar()->setOrientation(Qt::Horizontal);
    }

    setDialpadPosition();
}

void DialpadTestView::createListWidget()
{
    mListWidget = new HbListWidget(this);
    for (int i=0; i<10; i++) {
        HbListWidgetItem* listItem = new HbListWidgetItem;
        QString logEvent("Log event ");
        QString index; index.setNum(i+1);
        logEvent.append(index);
        listItem->setText(logEvent);
        QString phoneNum("+35850123456");
        phoneNum.append(index);
        listItem->setSecondaryText(phoneNum);
        HbIcon icon(":/qgn_prop_pb_comm_call_large.svg");
        listItem->setIcon(icon);
        mListWidget->addItem(listItem);
    }
    setWidget(mListWidget);
}

bool DialpadTestView::eventFilter(QObject* watched, QEvent * event)
{
    if (watched!=&mMainWindow) {
        return false;
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "KeyEventHandler: key press:" << keyEvent->key();

        mPressedKey = keyEvent->text();

        if (keyEvent->key()>=Qt::Key_0 && keyEvent->key()<=Qt::Key_9) {
            mLongPressTimer->stop();
            mLongPressTimer->start(1500);
        }

        return false;
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "KeyEventHandler: key release:" << keyEvent->key();

        mLongPressTimer->stop();

        if (keyEvent->key()==Qt::Key_Yes ||
            keyEvent->key()==Qt::Key_Enter) {
            handleDial();
        }

        return false;
    }

    return false;
}

bool DialpadTestView::event(QEvent * event)
{
    if (event->type() == QEvent::LayoutDirectionChange) {
        setDialpadPosition();
    }

    return HbView::event(event);
}

void DialpadTestView::handleLongKeyPress()
{
    QString msg;
    msg.append("Long press: ");
    msg.append(mPressedKey);

    HbMessageBox::information(msg);
}

void DialpadTestView::handleDial()
{
    QString msg;
    if (mDialpad->editor().text().length()) {
        msg.append("Dialing to number: ");
        msg.append(mDialpad->editor().text());
    } else {
        msg.append("Please, enter phone number first");
    }

    HbMessageBox::information(msg);
}

void DialpadTestView::setTapOutsideDismiss()
{
    mTapOutsideDismiss = !mTapOutsideDismiss;
    mDialpad->setTapOutsideDismiss(mTapOutsideDismiss);
}

void DialpadTestView::setArabicMode()
{
    mArabicMode = !mArabicMode;

    if (mArabicMode) {
        HbInputLanguage lang(QLocale::Arabic);
        HbInputSettingProxy::instance()->setGlobalInputLanguage(lang);
    } else {
        HbInputLanguage lang(QLocale::English, QLocale::UnitedKingdom);
        HbInputSettingProxy::instance()->setGlobalInputLanguage(lang);
    }
}

void DialpadTestView::setArabicDigit()
{
    mArabicDigit = !mArabicDigit;

    if (mArabicDigit) {
        HbExtendedLocale::system().setZeroDigit(ArabicIndicDigit);
    } else {
        HbExtendedLocale::system().setZeroDigit(WesternDigit);
    }
}

void DialpadTestView::loadTestabilityPlugin()
{
#ifdef DIALPAD_ENABLE_TESTABILITY
    QString testabilityPlugin = "testability/testability";
    QString testabilityPluginPostfix = ".dll";

    testabilityPlugin = QLibraryInfo::location(QLibraryInfo::PluginsPath) +
                                               QObject::tr("/") + testabilityPlugin +
                                               testabilityPluginPostfix;

    QPluginLoader loader(testabilityPlugin.toLatin1().data());

    QObject *plugin = loader.instance();
    if (plugin) {
            qDebug("Testability plugin loaded successfully!");
            mTestabilityInterface = qobject_cast<TestabilityInterface *>(plugin);

            if (mTestabilityInterface) {
                    qDebug("Testability interface obtained!");
                    mTestabilityInterface->Initialize();
            } else {
                    qDebug("Failed to get testability interface!");
            }
    } else {
            qDebug("Testability plugin %s load failed with error:%s",
                   testabilityPlugin.toLatin1().data(),
                   loader.errorString().toLatin1().data());
    }
#endif
}

