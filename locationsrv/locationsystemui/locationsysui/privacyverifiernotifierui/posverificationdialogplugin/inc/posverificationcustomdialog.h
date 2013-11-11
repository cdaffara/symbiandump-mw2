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
 * Description: Header file for Custom PosVerification dialog
 *
 */

#ifndef POSVERIFICATIONCUSTOMDIALOG_H
#define POSVERIFICATIONCUSTOMDIALOG_H

#include <hbdialog.h>
#include <hbdevicedialoginterface.h>
#include <hblabel.h>
#include <hbaction.h>


class CustomDocumentLoader;

class PosVerificationCustomDialog : public HbDialog,
        public HbDeviceDialogInterface

    {
Q_OBJECT
Q_PROPERTY( QString heading READ getHeading WRITE setHeading )
Q_PROPERTY( QString queryText READ getQueryText WRITE setQueryText )
Q_PROPERTY( QString queryText2 READ getQueryText2 WRITE setQueryText2 )
Q_PROPERTY( QStringList requestor READ getRequestor WRITE setRequestor )

public:

    /*
     * Constructor
     */
    PosVerificationCustomDialog();

    /**
     * Destructor
     */
    ~PosVerificationCustomDialog();

public:
    /**
     * This method gets the widget from the application desginer docml file.
     * param[in]- loader, instance of CustomDocumentLoader
     */
    void getWidgets(const CustomDocumentLoader &loader);

    //from HbDeviceDialogInterface
public:
    /**
     * Set device dialog widget parameters. This function may be called after the widget is created 
     * to update widget parameters. parameters contains widget parameters.The structure and meaning
     * of parameters is a contract between the plugin and a client. 
     * @param[in] - parameters, QVariantMap, containing the key value pair for the properties
     * @returns- bool, Returns true on success or false on failure.
     */
    bool setDeviceDialogParameters(const QVariantMap &parameters);

    /**
     * Returns an error last occurred. Error code ranges are defined in HbDeviceDialog.
     * The code is passed to a client by device dialog framework
     * @returns- int, error code 
     */
    int deviceDialogError() const;

    /**
     * Closes a device dialog widget gracefully. This function may called due to device dialog client
     * calling HbDeviceDialog::cancel() or by device dialog service if it decides that a dialog needs 
     * to be closed. Device dialog widget may also close itself without this function being called. 
     * byClient is true if client initiated the closing. Otherwise the closing was initiated by the device
     * dialog framework. The widget may also be deleted at any time ungracefully without closeDeviceDialog()
     * being called.
     */
    void closeDeviceDialog(bool byClient);

    /**
     * Returns a pointer to a widget that implements the user interface of the device dialog.
     * @returns-  HbPopup, pointer to the widget
     */
    HbPopup * deviceDialogWidget() const;
    
signals:
    void deviceDialogClosed();
    void deviceDialogData(QVariantMap data);

private:

    /**
     * This method is used to set the name of requestors 
     * requesting for position, in the verification dialog
     * @param [in] - requestorList, list of names of the requestors
     */
    void setRequestor(QStringList& requestorList);

    /**
     * This method is used to gets the name of requestors 
     * requesting for position, in the verification dialog
     * @returns - QStringList, list of names of the requestors
     */
    QStringList getRequestor();

    /**
     * This method is used to set the heading for the dialog
     * @param [in] - heading, string to be set as the heading for the dialog
     */
    void setHeading(QString& heading);

    /**
     * This method is used to get the heading for the dialog
     * @returns - QString, heading for the dialog
     */
    QString getHeading();

    /**
     * This method is used to set the query text to be displayed, depending on the 
     * policy.
     * @param [in] - textQuery, string to be set as the query for the dialog
     */
    void setQueryText(QString& textQuery);

    /**
     * This method is used to get the query text displayed, depending on the 
     * policy.
     * @returns - QString, query for the dialog
     */
    QString getQueryText();

    /**
     * This method is used to set the query text to be displayed, depending on the 
     * policy.
     * @param [in] - textQuery, string to be set as the query line 2 for the dialog
     */
    void setQueryText2(QString& textQuery);

    /**
     * This method is used to get the query text line 2 displayed, depending on the 
     * policy.
     * @returns - QString, query line 2 for the dialog
     */
    QString getQueryText2();

    /**
     * This method sets the properties for the widget.
     * The property value is validated before the property is set.
     * @param[in] - parameters, QVariantMap, containing the key value pair for the properties
     * @returns- bool, Returns true on success or false on failure.
     */
    bool setProperties(const QVariantMap& parameters);

private slots:
    /**
     * This slot is called on acceting the query dialog
     */
    void posVerificationdlgAccepted();

    /**
     * This slot is called on rejecting the query dialog 
     */
    void posVerificationdlgRejected();

    //member data
private:
    /**
     * heading label for the device dialog
     * Owns
     */
    HbLabel * mHeadingLabel;
    
    /**
     * primary action (SK1) for the device dialog
     * owns
     */
    HbAction * mPrimaryAction;

    /**
     * query text label
     * owns
     */
    HbLabel * mQueryText;

    /**
     * query text label (line 2)
     * Changes as per the first query text
     * owns
     */
    HbLabel * mQueryText2;

    /**
     * Widget containing the list of the requestors
     * owns
     */
    HbWidget * mRequestorList;
    
    /**
     * list of names of the reqestors
     * owns
     */
    QStringList mRequestorListItems;

    /**
     * secondry action (SK2) for the device dialog
     * owns
     */
    HbAction * mSecondryAction;

    };

#endif // POSVERIFICATIONCUSTOMDIALOG_H
