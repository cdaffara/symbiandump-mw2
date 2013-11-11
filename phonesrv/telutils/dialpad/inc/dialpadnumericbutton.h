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

#ifndef DIALPADNUMERICBUTTON_H
#define DIALPADNUMERICBUTTON_H

#include <hbicon.h>
#include <hbinputbutton.h>

class DialpadNumericButton : public HbInputButton
{
    Q_OBJECT

    // use label instead of text to override input button properties
    Q_PROPERTY(QString iconName READ iconName)
    Q_PROPERTY(QString primaryLabel READ primaryLabel WRITE setPrimaryLabel)
    Q_PROPERTY(QString secondaryLabel READ secondaryLabel WRITE setSecondaryLabel)
    Q_PROPERTY(QString secondaryLabelRow2 READ secondaryLabelRow2 WRITE setSecondaryLabelRow2)

public:
    explicit DialpadNumericButton(int keyCode, const QPoint &position, const QSize &size = QSize(1, 1));
    virtual ~DialpadNumericButton();

    void setIcon(const HbIcon& icon);
    HbIcon& icon();
    QString iconName() const;

    void setPrimaryLabel(const QString& value);
    QString primaryLabel() const;

    void setSecondaryLabel(const QString& value);
    QString secondaryLabel() const;

    void setSecondaryLabelRow2(const QString& value);
    QString secondaryLabelRow2() const;

private:
    HbIcon mIcon;
    QString mPrimaryLabel;
    QString mSecondaryLabel;
    QString mSecondaryLabelRow2;
};

#endif // DIALPADNUMERICBUTTON_H
