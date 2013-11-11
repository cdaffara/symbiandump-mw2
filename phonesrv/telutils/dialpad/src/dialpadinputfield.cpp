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
* Description: Input field
*
*/

#include <QGraphicsLinearLayout>

#include <hblineedit.h>
#include <hbinputeditorinterface.h>
#include <hbinputstandardfilters.h>
#include <hbdeviceprofile.h>
#include <hbmainwindow.h>

#include "dialpadinputfield.h"
#include "dialpadbutton.h"

static const QLatin1String HbBackspaceIcon("qtg_mono_backspace2");
static const int DialpadAutoRepeatInterval = 150; // ms
static const int DialpadAutoRepeatDelay = 1000; // ms
static const qreal DialpadComponentMargin = 0.75; // units
static const qreal DialpadBackspaceWidthV = 9.8; // units
static const qreal DialpadBackspaceWidthH = 8.6; // units
static const qreal DialpadInputFieldHeightV = 7.2; // units
static const qreal DialpadInputFieldHeightH = 6.6; // units
static const int DialpadMaxEditStringLenght = 100;

DialpadInputField::DialpadInputField(
    const HbMainWindow& mainWindow,
    QGraphicsItem* parent)
    : HbWidget(parent),
      mMainWindow(mainWindow)
{
    // create editor
    mNumberEditor = new HbLineEdit(this);
    mNumberEditor->setObjectName(QLatin1String("numberEdit"));
    HbEditorInterface editorInterface(mNumberEditor);
    editorInterface.setFilter(HbPhoneNumberFilter::instance());
    editorInterface.setInputConstraints(HbEditorConstraintIgnoreFocus);
    mNumberEditor->setMaxLength(DialpadMaxEditStringLenght);
    mNumberEditor->setMinRows(1);
    mNumberEditor->setMaxRows(1);    
    mNumberEditor->setAdjustFontSizeToFitHeight(true);
    mNumberEditor->setFontSpec(HbFontSpec(HbFontSpec::Primary));

    // create backspace button
    mBackspace = new DialpadButton(this);
    mBackspace->setObjectName(QLatin1String("backspaceButton"));
    mBackspace->setButtonType(DialpadButton::FunctionButton); // for css
    mBackspace->setFocusPolicy(Qt::NoFocus);
    mBackspace->setFlag(QGraphicsItem::ItemIsFocusable,false);
    mBackspace->setIcon(HbIcon(HbBackspaceIcon));
    mBackspace->setEnabled(false);
    mBackspace->setAutoRepeat(true);
    mBackspace->setAutoRepeatInterval(DialpadAutoRepeatInterval);
    mBackspace->setAutoRepeatDelay(DialpadAutoRepeatDelay);
    mBackspace->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    connect(mNumberEditor,SIGNAL(contentsChanged()),
            SLOT(onEditorContentChanged()));

    // set input field layout
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
    layout->addItem(mNumberEditor);
    layout->setAlignment(mNumberEditor,Qt::AlignVCenter);
    layout->addItem(mBackspace);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    updateLayout(mMainWindow.orientation());
}

DialpadInputField::~DialpadInputField()
{
}

HbLineEdit& DialpadInputField::editor() const
{
    return *mNumberEditor;
}

DialpadButton& DialpadInputField::backspaceButton() const
{
    return *mBackspace;
}

void DialpadInputField::updateLayout(Qt::Orientation orientation)
{
    Q_ASSERT(layout());

    qreal unit = HbDeviceProfile::profile(&mMainWindow).unitValue();

    QGraphicsLinearLayout* mainLayout =
        static_cast<QGraphicsLinearLayout*>(layout());

    mainLayout->setSpacing(DialpadComponentMargin* unit);

    if (orientation==Qt::Vertical) {
        mHeight = DialpadInputFieldHeightV * unit;
        mBackspace->setPreferredWidth(DialpadBackspaceWidthV * unit);
    } else {
        mHeight = DialpadInputFieldHeightH * unit;
        mBackspace->setPreferredWidth(DialpadBackspaceWidthH * unit);
    }
}

void DialpadInputField::onEditorContentChanged()
{
    mBackspace->setEnabled(mNumberEditor->text().length());
}

QSizeF DialpadInputField::sizeHint(
    Qt::SizeHint which,
    const QSizeF & constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    // input field height is fixed
    QSizeF sh;
    sh.setHeight(mHeight);
    return sh;
}
