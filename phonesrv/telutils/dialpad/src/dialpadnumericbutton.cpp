/*!
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
* Description: Extends HbInputButton
*
*/


#include "dialpadnumericbutton.h"

DialpadNumericButton::DialpadNumericButton(
    int keyCode, const QPoint &position, const QSize &size)
    : HbInputButton(keyCode, position, size)
{
}

DialpadNumericButton::~DialpadNumericButton()
{
}

void DialpadNumericButton::setIcon(const HbIcon& icon)
{
    mIcon = icon;
}

HbIcon& DialpadNumericButton::icon()
{
    return mIcon;
}

QString DialpadNumericButton::iconName() const
{
    return mIcon.iconName();
}

void DialpadNumericButton::setPrimaryLabel(const QString& value)
{
    mPrimaryLabel = value;
}

QString DialpadNumericButton::primaryLabel() const
{
    return mPrimaryLabel;
}

void DialpadNumericButton::setSecondaryLabel(const QString& value)
{
    mSecondaryLabel = value;
}

QString DialpadNumericButton::secondaryLabel() const
{
    return mSecondaryLabel;
}

void DialpadNumericButton::setSecondaryLabelRow2(const QString& value)
{
    mSecondaryLabelRow2 = value;
}

QString DialpadNumericButton::secondaryLabelRow2() const
{
    return mSecondaryLabelRow2;
}
