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
 * Description: This is for Location Services Settings plugin 
 *							for Positioning Settings's Advanced view
 *
 */

#include "poslocationservices.h"
#include "possettingsadvop.h"
#include "locationservicescustomviewitem.h"
#include "possettingssignalmapper.h"

#include <QtCore>
#include <QTranslator>
#include <QDebug>

#include <hbaction.h>
#include <hbpushbutton.h>
#include <hbmessagebox.h>
#include <hbicon.h>
#include <hbextendedlocale.h>

// constants
const int KMinToSecs = 60;
const int KHourToSecs = 3600;
const int KOneDay = 24;

// ---------------------------------------------------------------------------
// PosLocationServices::PosLocationServices
// ---------------------------------------------------------------------------
//
PosLocationServices::PosLocationServices() :
    mLocationServiceEngine(0), mGroup(0), mOperation(0), mSignalMapper(0),
            mStopQuery(0), mServiceId(0)
    {
    mLocationServiceEngine = new PosLocationServicesEngine();
    }

// ---------------------------------------------------------------------------
// PosLocationServices::~PosLocationServices
// ---------------------------------------------------------------------------
//
PosLocationServices::~PosLocationServices()
    {
    qDebug() << "+PosLocationServices::~PosLocationServices()";

    if (mLocationServiceEngine)
        {
        delete mLocationServiceEngine;
        mLocationServiceEngine = 0;
        }

    if (mGroup)
        {
        if (mOperation)
            {
            mOperation->removeItem(mGroup);
            }
        }

    if (mSignalMapper)
        {
        delete mSignalMapper;
        mSignalMapper = 0;
        }

    if (mStopQuery)
        {
        delete mStopQuery;
        mStopQuery = 0;
        }

    //Clear the list
    mServiceList.clear();

    //clear the signal mapper list
    int count = mSignalMapperList.count();
    qDebug() << "mSignalMapperList.count() = " << count;

    for (int i = 0; i < count; ++i)
        {
        delete mSignalMapperList[i];
        mSignalMapperList[i] = 0;
        }
    mSignalMapperList.clear();

    qDebug() << "-PosLocationServices::~PosLocationServices()";
    }

// ---------------------------------------------------------------------------
// PosLocationServices::initialise
// ---------------------------------------------------------------------------
//
int PosLocationServices::initialise(PosSettingsAdvOp* operation)
    {
    qDebug() << "+PosLocationServices::initialise()";

    int result = 0;

    // Assign to the member variable
    mOperation = operation;

    //get the prototype list from the data form and append the custom control to the list
    QList<HbAbstractViewItem *> protoTypeList;
    mOperation->itemPrototypes(protoTypeList);

    LocationServicesCustomViewItem* item =
            new LocationServicesCustomViewItem();

    protoTypeList.append(item);

    mOperation->setItemPrototypes(protoTypeList);

    // create the signal mapper
    mSignalMapper = new QSignalMapper();

    //get the list of location services
    mServiceList.clear();
    if (mLocationServiceEngine)
        {
        result = mLocationServiceEngine->getLocationServices(mServiceList);
        }

    qDebug() << "PosLocationServices::initialise(): result" << result;

    // if no error while getting the services list then add the group
    // for Location based services
    if (result == 0)
        {
        mGroup = new HbDataFormModelItem(HbDataFormModelItem::GroupItem,
                hbTrId("txt_loe_subtitle_background_positioning"), 0);

        //Add the group to the data form
        mOperation->appendDataFormItem(mGroup);

        //list all the location services inside the group
        listServices();
        }
    QObject::connect(mLocationServiceEngine, SIGNAL(locationServiceUpdate()),
            this, SLOT(updateList()));

    qDebug() << "-PosLocationServices::initialise()";
    return result;
    }

// ---------------------------------------------------------------------------
// PosLocationServices::listServices
// ---------------------------------------------------------------------------
//
void PosLocationServices::listServices()
    {
    qDebug() << "+PosLocationServices::listServices()";
    qDebug() << "No of Location Services = " << mServiceList.count();

    QList<PosLocationServiceInfo>::const_iterator serviceIterator;

    qDebug() << "Appending the services in group ";

    // Get the location services names and display
    for (serviceIterator = mServiceList.begin(); serviceIterator
            != mServiceList.end(); ++serviceIterator)
        {
        qDebug() << "serviceIterator->mServiceId = "
                << serviceIterator->mServiceId;

        qDebug() << "serviceIterator->mServiceName = "
                << serviceIterator->mServiceName;

        qDebug() << "serviceIterator->mEndDateTime = "
                << serviceIterator->mEndDateTime.date().toString();

        qDebug() << "serviceIterator->mEndDateTime.isValid() = "
                << serviceIterator->mEndDateTime.date().isValid();

        qDebug() << "serviceIterator->mUpdateInterval = "
                << serviceIterator->mUpdateInterval;

        // if any of the fields are invalid then skip listing
          if (serviceIterator->mUpdateInterval > 0)
            {
            qDebug() << "creating services";

            //for each services create new custom item 
            HbDataFormModelItem
                    * service =
                            new HbDataFormModelItem(
                                    static_cast<HbDataFormModelItem::DataItemType> (LocationServicesCustomItem));
                                    	
            if(!serviceIterator->mServiceName.isEmpty())
            //set the widget data to the server name
            service->setContentWidgetData("primarytext",
                    serviceIterator->mServiceName);
            else
            service->setContentWidgetData("primarytext",
                    QString(hbTrId("txt_loe_info_unknown_requestor")));	        

            QString updateIntervalString;
            //get the update interval String
            getUpdateIntervalString(serviceIterator->mUpdateInterval,
                    updateIntervalString);
            service->setContentWidgetData("sectext", updateIntervalString);

            //get the validity end date from the engine and convert to string
            QString validity(serviceIterator->mEndDateTime.date().toString(
                    Qt::ISODate));

            service->setContentWidgetData("sec2text", QString(hbTrId(
                    "txt_loe_list_valid_until_1")).arg(validity));

            qDebug() << "Adding service into the group";
            mOperation->appendDataFormItem(service, mGroup);

            //create an object of dummy signal mapper class
            //this is done to send the index info to the slots
            //since checkbox is not of QObject type and dummy class is 
            // created.
            PosSettingsSignalMapper* signalMapper =
                    new PosSettingsSignalMapper();

            qDebug() << "Adding the signalMapper into mSignalMapperList";
            mSignalMapperList.append(signalMapper);

            //add connection for clicked for the custom item, depending on which the
            //add connections to launch the dialog for the stopping the service  
            mOperation->addConnection(
                    service,
                    SIGNAL(clicked(bool)),
                    mSignalMapperList[serviceIterator - mServiceList.begin()],
                    SLOT(signalmap()));

            QObject::connect(mSignalMapperList[serviceIterator
                    - mServiceList.begin()], SIGNAL(signalmapped()),
                    mSignalMapper, SLOT(map()));

            // set mapping of each check box item to its index
            if (mSignalMapper)
                {
                qDebug() << "mSignalMapper->setMapping ";
                mSignalMapper->setMapping(mSignalMapperList[serviceIterator
                        - mServiceList.begin()], (serviceIterator
                        - mServiceList.begin()));
                }
            }
        }

    qDebug() << "mGroup->childCount() = " << mGroup->childCount();

    if (mGroup->childCount() == 0)
        {
        qDebug() << "mOperation->removeItem mGroup";
        mOperation->removeItem(mGroup);
        mGroup = NULL;
        }
    else
        {
        //use signal mapper to connect
        qDebug() << "use signal mapper to use stop service";
        QObject::connect(mSignalMapper, SIGNAL(mapped(int)), this,
                SLOT(stopService(int)));
        }
    qDebug() << "-PosLocationServices::listServices()";
    }

// ---------------------------------------------------------------------------
// PosLocationServices::updateList
// ---------------------------------------------------------------------------
//
void PosLocationServices::updateList()
    {
    qDebug() << "+PosLocationServices::updateList()";
    int result = 0;
    int oldChildCount = 0;

    //store the current state (expanded) of the group
    if (mGroup)
        {
        qDebug() << "Store the group info";

        oldChildCount = mGroup->childCount();

        // remove all connections
        int count = mSignalMapperList.count();
        qDebug() << "mSignalMapperList.count() = " << count;

        if (count > 0)
            {
            for (TInt i = 0; i < count; ++i)
                {
                QObject::disconnect(mSignalMapperList[i],
                        SIGNAL(signalmapped()), mSignalMapper, SLOT(map()));
                }
            QObject::disconnect(mSignalMapper, SIGNAL(mapped(int)), this,
                    SLOT(stopService(int)));
            }
        mSignalMapperList.clear();

        qDebug() << " oldChildCount" << oldChildCount;
        if (oldChildCount > 0)
            {
            qDebug() << "Removing the old children";
            //mGroup->removeChildren(0, oldChildCount);
            //ToDo: Hack from HbDataForm team         
            for (int i = 0; i < oldChildCount; ++i)
                {
                qDebug() << "remove chile index " << i;
                mGroup->removeChild(0);
                }
            }

        }
    else
        {
        // this part of code is called when the services are added and 
        // Advanced view is in the background. Since there is no group
        // a new group has to be created to add the services.
        qDebug() << "Creating new group";
        mGroup = new HbDataFormModelItem(HbDataFormModelItem::GroupItem,
                hbTrId("txt_loe_subtitle_background_positioning"), 0);
        mOperation->appendDataFormItem(mGroup);
        }

    qDebug() << "oldChildCount = " << oldChildCount;
    qDebug() << "mServiceList.count() before update = "
            << mServiceList.count();

    //clear the list and get the list of server present for positioning
    mServiceList.clear();
    if (mLocationServiceEngine)
        {
        result = mLocationServiceEngine->getLocationServices(mServiceList);
        }

    qDebug() << "PosLocationServices::listServices(): result" << result;

    //update the new list to the group
    if (result == 0)
        {
        listServices();
        }
    if (result == -1)
        {
        //remove the group
        mOperation->removeItem(mGroup);
        mGroup = NULL;
        }

    qDebug() << "-PosLocationServices::updateList()";
    }

// ---------------------------------------------------------------------------
// PosLocationServices::stopService
// ---------------------------------------------------------------------------
//
void PosLocationServices::stopService(int serviceId)
    {
    qDebug() << "+PosLocationServices::stopService() service Id" << serviceId;

    mServiceId = serviceId;

    //Construct the display string
    QString displayString;
    displayString.append(QString(hbTrId("txt_loe_info_stop_service_1")).arg(
            mServiceList[serviceId].mServiceName));

    //create the message mStopQuery dialog
    //HbMessageBox * mStopQuery = new HbMessageBox();

    if (mStopQuery == 0)
        {
        mStopQuery = new HbMessageBox();
        mStopQuery->setTimeout(HbDialog::NoTimeout);
   
        //the secondry action that will come in the SK2 of the Dialog box
        mStopQuery->addAction(new HbAction(QString(hbTrId(
                "txt_common_button_cancel"))));

        //To-Do: set the actual stop service icon after released in sdk 
        QIcon stopServiceIcon(":/resources/qgn_note_warning.svg");
        mStopQuery->setIcon(HbIcon(stopServiceIcon));
        }

    //set the mStopQuery text
    mStopQuery->setText(displayString);
    mStopQuery->open(this,SLOT(stopSession(HbAction*)));

    qDebug() << "-PosLocationServices::stopService()";
    }

// ---------------------------------------------------------------------------
// PosLocationServices::stopSession
// ---------------------------------------------------------------------------
//
void PosLocationServices::stopSession(HbAction* action)
    {
    qDebug() << " + PosLocationServices::stopSession()";
    
    //if LSK is pressed
    if ((action == mStopQuery->actions().at(0)) && mLocationServiceEngine)
        {
        //stop the session
        mLocationServiceEngine->removeLocationService(
                mServiceList[mServiceId].mServiceId);
        }
    qDebug() << " - PosLocationServices::stopSession()";
    }

// ---------------------------------------------------------------------------
// PosLocationServices::getUpdateIntervalString
// ---------------------------------------------------------------------------
//
void PosLocationServices::getUpdateIntervalString(int updateInterval,
        QString& updateIntervalString)
    {
    qDebug()
            << "+PosLocationServices::getUpdateIntervalString(): updateInterval"
            << updateInterval;

    //To-Do: clarification needed from chelli
    if (updateInterval / KMinToSecs < 1)
        {
        //seconds
        QVariant var(updateInterval);
        updateIntervalString = QString(hbTrId(
                "txt_loe_list_updates_every_l1_seconds")).arg(var.toString());
        }
    else if (updateInterval / KHourToSecs < 1)
        {
        //minutes
        int minutes = (updateInterval / KMinToSecs);
        int seconds = (updateInterval % KMinToSecs);
        QVariant varMinutes(minutes);
        QVariant varSeconds(seconds);
        updateIntervalString = QString(hbTrId(
                "txt_loe_list_updates_every_l1_minutes_and_l2_sec")).arg(
                varMinutes.toString()).arg(varSeconds.toString());
        }
    else if (updateInterval / (KHourToSecs * KOneDay) < 1)
        {
        //hour
        int hours = (updateInterval / KHourToSecs);
        int minutes = (updateInterval % KHourToSecs) / KMinToSecs;
        QVariant varhour(hours);
        QVariant varMinutes(minutes);

        updateIntervalString = QString(hbTrId(
                "txt_loe_list_updates_every_l1_hours_and_l2_minut")).arg(
                varhour.toString()).arg(varMinutes.toString());
        }
    else
        {
        //days
        int days = (updateInterval / (KHourToSecs * KOneDay));
        int hours = (updateInterval % (KHourToSecs * KOneDay)) / KMinToSecs;
        QVariant varDays(days);
        QVariant varHours(hours);
        updateIntervalString = QString(hbTrId(
                "txt_loe_list_updates_every_l1_days_and_l2_hours")).arg(
                varDays.toString()).arg(varHours.toString());
        }

    qDebug() << "-PosLocationServices::updateInterval()";

    }

Q_EXPORT_PLUGIN2(poslocationservices, PosLocationServices)
;

