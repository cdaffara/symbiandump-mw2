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

#ifndef POSVERIFICATION_DIALOG_PLUGIN_H
#define POSVERIFICATION_DIALOG_PLUGIN_H

#include <hbdevicedialogplugin.h>
#include <qtranslator.h>

class PosVerificationDialogPlugin : public HbDeviceDialogPlugin
    {
Q_OBJECT

public:
    /**
     * Constructor
     */
    PosVerificationDialogPlugin();

    /**
     * Destrcutor
     */
    ~PosVerificationDialogPlugin();

    /**
     * Checks if client is allowed use the device dialog that the plugin creates. 
     * Device dialog service calls this function before creating a device dialog or 
     * attaching into an existing one if flag HbDeviceDialogPlugin::SecurityCheck is set. 
     * The implementation is operating system dependent. 
     * @returns- bool, Returns true if client is allowed to use the device dialog.
     * @param[in] - deviceDialogType  Device dialog type.  
     * @param[in] - parameters  Device dialog parameters.  
     * @param[in] - securityInfo  Information for security check. Content is operating system dependent.
     * The format is <key, value> pairs. Currently only Symbian is defined.
     */

    bool
            accessAllowed(const QString &notificationType,
                    const QVariantMap &parameters,
                    const QVariantMap &securityInfo) const;

    /**
     * Create a device dialog instance compatible with the version requested. 
     * This could always be the latest version if it is backwards compatible with older versions. 
     * Device dialog framework is unaware of version numbers in type strings. It performs string 
     * comparison of the whole string when searching for a plugin
     * @param[in] - deviceDialogType  Device dialog type.
     * @param[in] - parameters  Device dialog parameters.  
     * @returns - HbDeviceDialogInterface, device dialog instance
     */

    HbDeviceDialogInterface *createDeviceDialog(
            const QString &deviceDialogType, const QVariantMap &parameters);

    /**
     * Gets information of the device dialog created by the plugin. Device dialog manager calls 
     * this function before creating the device dialog widget to check HbDeviceDialogPlugin::DeviceDialogGroup, 
     * HbDeviceDialogPlugin::DeviceDialogFlag and priority. 
     * @returns - true if device dialog information returned is valid.
     */

    bool deviceDialogInfo(const QString &deviceDialogType,
            const QVariantMap &parameters, DeviceDialogInfo *info) const;

    /**
     * Returns a list of device dialog types the plugin implements. A plugin may implement 
     * several device dialog types. By convention device dialog type strings should follow 
     * inverted domain name format
     * @returns - list of device dialog types
     */
    QStringList deviceDialogTypes() const;

    /**
     * @returns - plugin flags
     */
    HbDeviceDialogPlugin::PluginFlags pluginFlags() const;

    /**
     * Returns an error last occurred. Error code ranges are defined in HbDeviceDialog. The code 
     * is passed to a client by device dialog framework
     * @returns- errorcode
     */
    int error() const;

private:

    /**
     * Fix for ou1cimx1#462081
     * Loads the qt message file for localization and installs the translator.
     */
    void installTranslator();

private:

    /**
     * instance of translator
     * owns
     */
    QTranslator * mTranslator;
    };

#endif // POSVERIFICATION_DIALOG_PLUGIN_H
