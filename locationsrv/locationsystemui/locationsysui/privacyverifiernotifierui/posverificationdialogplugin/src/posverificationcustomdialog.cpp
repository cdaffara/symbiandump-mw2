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
 * Description: This implements the custom dialog for positioning verification dialogs
 *
 */

#include "posverificationcustomdialog.h"
#include "customdocumentloader.h"

#include <QtDebug>
#include <qgraphicsgridlayout.h>
#include <qsizepolicy.h>

//---------------------------------------------------------------
//PosVerificationCustomDialog::PosVerificationCustomDialog
//---------------------------------------------------------------
PosVerificationCustomDialog::PosVerificationCustomDialog() :
    mHeadingLabel(NULL), mPrimaryAction(NULL), mQueryText(NULL), mQueryText2(
            NULL), mRequestorList(NULL), mRequestorListItems(NULL),
            mSecondryAction(NULL)
    {
    qDebug() << "PosVerificationCustomDialog::PosVerificationCustomDialog()";
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::~PosVerificationCustomDialog
//---------------------------------------------------------------
PosVerificationCustomDialog::~PosVerificationCustomDialog()
    {
    qDebug()
            << "+PosVerificationCustomDialog::~PosVerificationCustomDialog()";
    delete mQueryText2;
    mQueryText2 = NULL;

    qDebug()
            << "-PosVerificationCustomDialog::~PosVerificationCustomDialog()";
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::setProperties
// Set properties
//---------------------------------------------------------------

bool PosVerificationCustomDialog::setProperties(const QVariantMap& parameters)
    {
    qDebug() << "+PosVerificationCustomDialog::setProperties()";
    QVariantMap::const_iterator i = parameters.constBegin();
    while (i != parameters.constEnd())
        {
        QByteArray key = i.key().toAscii();
        qDebug() << "-PosVerificationCustomDialog::setProperties(): Key is "
                << key;
        qDebug() << "-PosVerificationCustomDialog::setProperties(): data is "
                << key.constData();
        //check for the validity of the data
        if (property(key.constData()).isValid())
            {
            //sets the property values
            setProperty(key.constData(), i.value());
            }
        else
            {
            qDebug()
                    << "-PosVerificationCustomDialog::setProperties(): value - false";
            return false;
            }
        ++i;
        }
    qDebug() << "-PosVerificationCustomDialog::setProperties()";
    return true;
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::setDeviceDialogParameters
//---------------------------------------------------------------
bool PosVerificationCustomDialog::setDeviceDialogParameters(
        const QVariantMap &parameters)
    {
    qDebug() << "+PosVerificationCustomDialog::setDeviceDialogParameters()";
    setModal(true);
    setTimeout(HbPopup::NoTimeout);
    qDebug() << "-PosVerificationCustomDialog::setDeviceDialogParameters()";
    return setProperties(parameters);
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::deviceDialogError
//---------------------------------------------------------------
int PosVerificationCustomDialog::deviceDialogError() const
    {
    qDebug() << "PosVerificationCustomDialog::deviceDialogError()";
    return 0;
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::closeDeviceDialog
//---------------------------------------------------------------
void PosVerificationCustomDialog::closeDeviceDialog(bool byClient)
    {
    qDebug() << "+PosVerificationCustomDialog::closeDeviceDialog()";
    Q_UNUSED(byClient);
    close();
    emit
    deviceDialogClosed();
    qDebug() << "-PosVerificationCustomDialog::closeDeviceDialog()";
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::deviceDialogWidget
//---------------------------------------------------------------
HbPopup* PosVerificationCustomDialog::deviceDialogWidget() const
    {
    qDebug() << "PosVerificationCustomDialog::deviceDialogWidget()";
    return const_cast<PosVerificationCustomDialog*> (this);
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::setHeading
//---------------------------------------------------------------
void PosVerificationCustomDialog::setHeading(QString& heading)
    {
    qDebug() << "+PosVerificationCustomDialog::setHeading()" << heading;
    mHeadingLabel->setPlainText(heading);
    qDebug() << "-PosVerificationCustomDialog::setHeading()";
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::getHeading
//---------------------------------------------------------------
QString PosVerificationCustomDialog::getHeading()
    {
    qDebug() << "PosVerificationCustomDialog::getHeading()";
    return mHeadingLabel->plainText();
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::setQueryText
//---------------------------------------------------------------
void PosVerificationCustomDialog::setQueryText(QString& textQuery)
    {
    qDebug() << "+PosVerificationCustomDialog::setQueryText()" << textQuery;
    mQueryText->setPlainText(textQuery);
    qDebug() << "-PosVerificationCustomDialog::setQueryText()";
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::getQueryText
//---------------------------------------------------------------
QString PosVerificationCustomDialog::getQueryText()
    {
    qDebug() << "PosVerificationCustomDialog::getQueryText()";
    return mQueryText->plainText();
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::setQueryText2
//---------------------------------------------------------------
void PosVerificationCustomDialog::setQueryText2(QString& textQuery)
    {
    qDebug() << "+PosVerificationCustomDialog::setQueryText2()" << textQuery;
    mQueryText2 = new HbLabel();
    if (mQueryText2)
        {
        mQueryText2->clear();
        mQueryText2->setPlainText(textQuery);
        mQueryText2->setFontSpec(HbFontSpec(HbFontSpec::Primary));
        }
    qDebug() << "-PosVerificationCustomDialog::setQueryText2()";
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::getQueryText2
//---------------------------------------------------------------
QString PosVerificationCustomDialog::getQueryText2()
    {
    qDebug() << "PosVerificationCustomDialog::getQueryText2()";
    if (mQueryText2)
        {
        return mQueryText2->plainText();
        }
    return "";
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::getWidgets
//---------------------------------------------------------------
void PosVerificationCustomDialog::getWidgets(
        const CustomDocumentLoader &loader)
    {
    qDebug() << "+PosVerificationCustomDialog::getWidgets()";

    mHeadingLabel = qobject_cast<HbLabel*> (loader.findWidget("heading"));
    Q_ASSERT_X((mHeadingLabel != 0), "PosVerificationCustomDialog", "invalid DocML file");

    mQueryText = qobject_cast<HbLabel*> (loader.findWidget("queryText_1"));
    Q_ASSERT_X((mQueryText != 0), "PosVerificationCustomDialog", "invalid DocML file");

    mRequestorList
            = qobject_cast<HbWidget*> (loader.findWidget("requestors"));
    Q_ASSERT_X((mRequestorList != 0), "PosVerificationCustomDialog", "invalid DocML file");

    mPrimaryAction = qobject_cast<HbAction*> (loader.findObject(
            "qtl_dialog_softkey_2_left"));
    Q_ASSERT_X((mPrimaryAction != 0), "PosVerificationCustomDialog", "invalid DocML file");
    QObject::connect(mPrimaryAction, SIGNAL(triggered()), this,
            SLOT(posVerificationdlgAccepted()));

    mSecondryAction = qobject_cast<HbAction*> (loader.findObject(
            "qtl_dialog_softkey_2_right"));
    Q_ASSERT_X((mSecondryAction != 0), "PosVerificationCustomDialog", "invalid DocML file");

    QObject::connect(mSecondryAction, SIGNAL(triggered()), this,
            SLOT(posVerificationdlgRejected()));
    qDebug() << "-PosVerificationCustomDialog::getWidgets()";
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::setRequestor
//---------------------------------------------------------------
void PosVerificationCustomDialog::setRequestor(QStringList& requestorList)
    {
    qDebug() << "+PosVerificationCustomDialog::setRequestor()";
    int rowadd = 1;
    mRequestorListItems = requestorList;
    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    if (!layout)
        {
        qDebug() << "setRequestor(): Creating layout failed";
        return;
        }

    qreal margin = 0;
    qreal leftmargin = 0;
    style()->parameter("hb-param-margin-gene-middle-vertical", margin);
    style()->parameter("hb-param-margin-gene-popup", leftmargin);
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    policy.setVerticalPolicy(QSizePolicy::Ignored);
    QSizePolicy text2Policy;
    text2Policy.setHorizontalPolicy(QSizePolicy::Preferred);
    text2Policy.setVerticalPolicy(QSizePolicy::Preferred);
    text2Policy.setHorizontalStretch(0);
    
    if (mQueryText2)
        {
        mQueryText2->setSizePolicy(text2Policy);
        mQueryText2->setAlignment(Qt::AlignTop);
        mQueryText2->setTextWrapping(Hb::TextWordWrap);
        layout->addItem(mQueryText2, 1, 0);
        layout->setRowSpacing(1, margin);
        layout->setContentsMargins(leftmargin, 0, 0, 0);
        rowadd = 2;
        }

    QStringList::const_iterator iterator;
    for (iterator = requestorList.begin(); iterator != requestorList.end(); ++iterator)
        {
        int i = iterator - requestorList.begin();
        HbLabel * requestorName = new HbLabel();
        if (requestorName)
            {
            qDebug() << "setRequestor()" << *iterator;
            requestorName->setFontSpec(HbFontSpec(HbFontSpec::Secondary));
            requestorName->setPlainText(*iterator);
            requestorName->setSizePolicy(policy);
            requestorName->setAlignment(Qt::AlignTop);

            layout->addItem(requestorName, i + rowadd, 0);
            layout->setRowSpacing(i + rowadd, margin);
            layout->setContentsMargins(leftmargin, 0, 0, 0);
            }
        }
    mRequestorList->setSizePolicy(policy);
    mRequestorList->setLayout(layout);
    qDebug() << "-PosVerificationCustomDialog::setRequestor()";
    }

//---------------------------------------------------------------
//PosVerificationCustomDialog::getRequestor
//---------------------------------------------------------------
QStringList PosVerificationCustomDialog::getRequestor()
    {
    qDebug() << "PosVerificationCustomDialog::getRequestor()";
    return mRequestorListItems;
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::posVerificationdlgAccepted
//---------------------------------------------------------------
void PosVerificationCustomDialog::posVerificationdlgAccepted()
    {
    qDebug() << "+PosVerificationCustomDialog::posVerificationdlgAccepted()";
    emit
    deviceDialogClosed();
    qDebug() << "-PosVerificationCustomDialog::posVerificationdlgAccepted()";
    }
//---------------------------------------------------------------
//PosVerificationCustomDialog::posVerificationdlgRejected
//---------------------------------------------------------------
void PosVerificationCustomDialog::posVerificationdlgRejected()
    {
    qDebug() << "+PosVerificationCustomDialog::posVerificationdlgRejected()";
    QVariantMap *data = new QVariantMap();
    QVariant value(-21);
    if (data)
        {
        data->insert("errorCode", value);
        emit
        deviceDialogData(*data);
        }
    //data is currently not being used in the client side
    emit
    deviceDialogClosed();
    delete data;
    qDebug() << "-PosVerificationCustomDialog::posVerificationdlgRejected()";
    }

// End of File

