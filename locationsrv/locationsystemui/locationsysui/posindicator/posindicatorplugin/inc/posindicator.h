/*
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for positioning indicator plugin class
*
*/

#ifndef POS_INDICATOR_PLUGIN_H
#define POS_INDICATOR_PLUGIN_H

#include <QObject>
#include <QStringList>
#include <QVariant>

#include <hbindicatorplugininterface.h>
#include <hbindicatorinterface.h>


class PosIndicatorInfo;
class PosGeoAddress;
class QTranslator;

// Indicator Plugin class
class PosIndicatorPlugin : public QObject, public HbIndicatorPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(HbIndicatorPluginInterface)

public:
	 /**
    * Constructor 
    */
    PosIndicatorPlugin();
	 /**
    * Destructor 
    */    
    ~PosIndicatorPlugin();

   /**
    * Returns indicator types implemented by this plugin
    * @return List of strings that denote indicator types implemented
    */
    QStringList indicatorTypes() const;
    
   /**
    * creates an indicator of type indicatorType
    * @param[in] String representing indicator type
    * @return Pointer to HbIndicatorInterface
    */
    HbIndicatorInterface* createIndicator(const QString &indicatorType);
 
    /**
    * Virtual function to check for capabilities for calling client
    * @return true if client has Location capability
			  false if client does not have Location capability
    */
	bool accessAllowed(const QString &indicatorType, const QVariantMap &securityInfo) const;

   /**
    * Returns the last error code
    * @return error code integer
    */
    int error() const;
    
private:
    Q_DISABLE_COPY(PosIndicatorPlugin)
    int mError;
    QStringList mIndicatorTypes;
    /**
       * Holds localization translator handle
       * OWns
       */
    QTranslator* mTranslator;  
};

// Indicator class that the indicator plugin implements
class PosIndicator : public HbIndicatorInterface
{
    Q_OBJECT
public:
	 /**
    * Constructor 
    */
    PosIndicator(const QString &indicatorType);
	 
	 /**
    * Destructor 
    */
    ~PosIndicator();
	 
   /**
    * Returns the data element store based on role 
    * @param[in] role
    * @return Data stored for the role.
    */
    QVariant indicatorData(int role) const;

    /*
     * Called when Indicator menu is about to be shown
     */
    bool refreshData();
    
private slots:
    /*
     * Refreshes indicator pane when new position information is available
     */
    void refreshPosIndicatorPane(PosGeoAddress& posGeoAddress, int error);

private : 
    /*
     * Converts decimal coordinate information to Degrees, Minutes, 
     * Seconds value
     */
    void ConvertToDMSFormat(QString &dDisplayString, double decimal);
    
private :
    
    /**
     * String containing information to be displayed
     */    
    QString mDisplayString;
    
    /**
     * Owns
    **/
    PosIndicatorInfo* mPosIndicatorInfo;    
 
    /*
     * Toggle flag to check whether current signal is returns address or co-ordinates
     */
    bool processedAddess;
};

#endif // POS_INDICATOR_PLUGIN_H

