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
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbwidget.h>
#include <hbframeitem.h>
#include <hbframedrawer.h>

#include "dialpadtest.h"
#include "dialpadbutton.h"

class TestDialpadButton : public DialpadButton
{
public:
    TestDialpadButton() {};
    ~TestDialpadButton() {};

    bool testSceneEvent(QEvent *event) { return DialpadButton::sceneEvent(event); };
};

class ut_DialpadButton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testButtonType();
    void testUpdatePrimitives();
    void testSceneEvent();

private:
    TestDialpadButton *mButton;
};

void ut_DialpadButton::initTestCase()
{
    mButton = new TestDialpadButton();
}

void ut_DialpadButton::cleanupTestCase()
{
    delete mButton;
}

void ut_DialpadButton::testButtonType()
{
    QVERIFY(mButton->buttonType()==DialpadButton::FunctionButton);
    mButton->setButtonType(DialpadButton::CallButton);
    QVERIFY(mButton->buttonType()==DialpadButton::CallButton);
}

void ut_DialpadButton::testUpdatePrimitives()
{
    // function button

    mButton->setEnabled(false);
    mButton->setButtonType(DialpadButton::FunctionButton);
    HbFrameItem* frame =
        qgraphicsitem_cast<HbFrameItem*>(static_cast<HbWidget*>(mButton)->primitive("background"));
    QVERIFY(frame->frameDrawer().frameGraphicsName()=="qtg_fr_input_btn_function_disabled");

    mButton->setEnabled(true);
    QVERIFY(frame->frameDrawer().frameGraphicsName()=="qtg_fr_input_btn_function_normal");

    mButton->setDown(true);
    QVERIFY(frame->frameDrawer().frameGraphicsName()=="qtg_fr_input_btn_function_pressed");
    mButton->setDown(false);

    // call button
    mButton->setEnabled(false);
    mButton->setButtonType(DialpadButton::CallButton);
    QVERIFY(frame->frameDrawer().frameGraphicsName()=="qtg_fr_input_btn_function_disabled");

    mButton->setEnabled(true);
    QVERIFY(frame->frameDrawer().frameGraphicsName()=="qtg_fr_btn_green_normal");

    mButton->setDown(true);
    QVERIFY(frame->frameDrawer().frameGraphicsName()=="qtg_fr_btn_green_pressed");
    mButton->setDown(false);
}

void ut_DialpadButton::testSceneEvent()
{
    QEvent event(QEvent::UngrabMouse);
    QSignalSpy spy( mButton, SIGNAL( clicked()) );
    QSignalSpy spy2( mButton, SIGNAL( released()) );
    mButton->setVisible(false);
    mButton->setDown(false);
    mButton->testSceneEvent(&event);
    QVERIFY(spy.count()==0);
    QVERIFY(spy2.count()==0);

    QEvent eventNone(QEvent::None);
    mButton->testSceneEvent(&eventNone);
    QVERIFY(spy.count()==0);
    QVERIFY(spy2.count()==0);

    mButton->setVisible(true);
    mButton->setDown(true);
    mButton->testSceneEvent(&event);
    QVERIFY(spy.count()==1);
    QVERIFY(spy2.count()==1);
    QVERIFY(mButton->isDown()==false);
}

DIALPAD_TEST_MAIN(ut_DialpadButton)
#include "ut_dialpadbutton.moc"
