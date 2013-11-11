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
 * Description: This header file is for creating the custom widget for location services 
 *              for Advanced view of Positioning settings 
 *
 */

#ifndef LOCATIONSERVICESWIDGET_H
#define LOCATIONSERVICESWIDGET_H

#include <QObject>
#include <hbabstractbutton.h>

class HbTextItem;

class LocationServicesWidget : public HbAbstractButton
    {
Q_OBJECT

// It is possible to set initial value to properties from the application designer.
Q_PROPERTY(QString primarytext READ primaryText WRITE setPrimaryText)
Q_PROPERTY(QString sectext READ secText WRITE setSecText)
Q_PROPERTY(QString sec2text READ sec2Text WRITE setSec2Text)

public:
    /**
     * Constructor
     */
    LocationServicesWidget(QGraphicsItem *parent = 0);
    
    /**
     * Destructor
     */
    ~LocationServicesWidget();

public:
    
    /**
     * Sets the primary text for the widget
     * @param [in] - primaryText, primary text for the widget
     */
    void setPrimaryText(const QString &primaryText);
    
    /**
     * Gets the primary text for the widget
     * @returns - QString containing the primary text
     */
    QString primaryText() const;
    
    /**
     * Sets the primary text for the widget
     * @param [in] - secText, secondry(second line) text for the widget
     */
    void setSecText(const QString &secText);
    
    /**
     * Gets the primary text for the widget
     * @returns - QString containing the secondry text
     */
    QString secText() const;
    
    /**
     * Sets the primary text for the widget
     * @param [in] - secText, secondry(third line) text for the widget
     */
    void setSec2Text(const QString &secText);
    
    /**
     * Gets the primary text for the widget
     * @returns - QString containing the secondry2 text
     */
    QString sec2Text() const;

private:
    
    /**
     * Primary Text item
     * Does not own
     */
    HbTextItem* mPrimaryTextItem;
    
    /**
     * Secondry Text item
     * Does not own
     */
    HbTextItem* mSecondryTextItem;
    
    /**
     * Secondry (third item) Text item
     * Does not own
     */
    HbTextItem* mSecondry2TextItem;
    };

#endif // LOCATIONSERVICESWIDGET_H
