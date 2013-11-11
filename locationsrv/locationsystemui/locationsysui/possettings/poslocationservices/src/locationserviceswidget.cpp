/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0""
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: This file is for creating the custom widget for location services 
 *              for Advanced view of Positioning settings 
 *
 */

#include "locationserviceswidget.h"

#include <QtGui>
#include <hbfontspec.h>
#include <hbstyleloader.h>
#include <hbstyle.h>
#include <hbtextitem.h>


static const QString FILE_PATH_WIDGETML =
        ":/data/locationserviceswidget.widgetml";
static const QString FILE_PATH_CSS = ":/data/locationserviceswidget.css";

// ---------------------------------------------------------------------------
// LocationServicesWidget::LocationServicesWidget
// ---------------------------------------------------------------------------
//
LocationServicesWidget::LocationServicesWidget(QGraphicsItem *parent) :
    HbAbstractButton(parent)
    {
    // Path for widgetml and css files.
    HbStyleLoader::registerFilePath(FILE_PATH_WIDGETML);
    HbStyleLoader::registerFilePath(FILE_PATH_CSS);

    //Create the primary and secondry text items
    mPrimaryTextItem = new HbTextItem(QString("New"), this);
    HbStyle::setItemName(mPrimaryTextItem, "text_box1");
    mPrimaryTextItem->setFontSpec(HbFontSpec(HbFontSpec::Primary));

    mSecondryTextItem = new HbTextItem(QString("New"), this);
    HbStyle::setItemName(mSecondryTextItem, "text_box2");
    mSecondryTextItem->setFontSpec(HbFontSpec(HbFontSpec::Secondary));

    mSecondry2TextItem = new HbTextItem(QString("New"), this);
    HbStyle::setItemName(mSecondry2TextItem, "text_box3");
    mSecondry2TextItem->setFontSpec(HbFontSpec(HbFontSpec::Secondary));
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::LocationServicesWidget
// ---------------------------------------------------------------------------
//
LocationServicesWidget::~LocationServicesWidget()
    {
    HbStyleLoader::unregisterFilePath(FILE_PATH_WIDGETML);
    HbStyleLoader::unregisterFilePath(FILE_PATH_CSS);
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::setPrimaryText
// ---------------------------------------------------------------------------
//
void LocationServicesWidget::setPrimaryText(const QString &primaryText)
    {
    mPrimaryTextItem->setText(primaryText);
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::primaryText
// ---------------------------------------------------------------------------
//
QString LocationServicesWidget::primaryText() const
    {
    return mPrimaryTextItem->text();
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::setSecText
// ---------------------------------------------------------------------------
//
void LocationServicesWidget::setSecText(const QString& secText)
    {
    mSecondryTextItem->setText(secText);
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::secText
// ---------------------------------------------------------------------------
//
QString LocationServicesWidget::secText() const
    {
    return mSecondryTextItem->text();
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::setSec2Text
// ---------------------------------------------------------------------------
//
void LocationServicesWidget::setSec2Text(const QString& secText)
    {
    mSecondry2TextItem->setText(secText);
    }
// ---------------------------------------------------------------------------
// LocationServicesWidget::sec2Text
// ---------------------------------------------------------------------------
//
QString LocationServicesWidget::sec2Text() const
    {
    return mSecondry2TextItem->text();
    }

// end of file
