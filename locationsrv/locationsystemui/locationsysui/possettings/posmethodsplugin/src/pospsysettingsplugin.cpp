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
 * Description:  Implementation of psy settings plugin class. 
 *
 */
#include "pospsysettingsplugin.h"
#include "pospsysettingsengine.h"

#include <QDebug>

//---------------------------------------------------------------------
// PosPsySettingsPlugin::PosPsySettingsPlugin()
// Constructor
//---------------------------------------------------------------------
PosPsySettingsPlugin::PosPsySettingsPlugin():
mGroup(NULL),
mSignalMapper(NULL),
mOperation(NULL)
    {  
   qDebug() << "================ PSY Plug-in loaded ==============================";	
   qDebug() << "+ PosPsySettingsPlugin::PosPsySettingsPlugin()";	
    mEngine = new PosPsySettingsEngine();
   qDebug() << "- PosPsySettingsPlugin::PosPsySettingsPlugin()";	
    }

//---------------------------------------------------------------------
// PosPsySettingsPlugin::~PosPsySettingsPlugin()
// Destructor
//---------------------------------------------------------------------
PosPsySettingsPlugin::~PosPsySettingsPlugin()
    {
    qDebug() << "- PosPsySettingsPlugin::~PosPsySettingsPlugin()";	
    //Note: no need to call mOperation->removeConnection() as it is already handled in advview's destructor
    if (mEngine)
        {
        delete mEngine;
        mEngine = NULL;
        }
    
    // delete the group
    if(mGroup)
        {
        mOperation->removeItem( mGroup );
        }

    // clear the list of available psys
    mPsyList.clear();
    
    QObject::disconnect(mSignalMapper, SIGNAL(mapped(int)), this,
            SLOT(onPsyPressed(int))); 
    // delete the QSignalMapper instance
    if (mSignalMapper)
        {
        delete mSignalMapper;
        mSignalMapper = NULL;
        }

    // delete the signal mapper instances 
    int cnt = mPsySignalMapperList.count();
    for (int i = 0; i < cnt ; ++i)
        {
        QObject::disconnect(mPsySignalMapperList[i], SIGNAL(signalmapped()),
                mSignalMapper, SLOT(map()));
        delete mPsySignalMapperList[i];
        mPsySignalMapperList[i] = NULL;
        }
    // clear the list of signal mapper
    mPsySignalMapperList.clear();
    
    qDebug() << "- PosPsySettingsPlugin::~PosPsySettingsPlugin()";	
    qDebug() << "================ PSY Plug-in unloaded ==============================";	   
    }

//---------------------------------------------------------------------
// PosPsySettingsPlugin::initialise( PosSettingsAdvOp* operation )
// 
//---------------------------------------------------------------------
int PosPsySettingsPlugin::initialise(PosSettingsAdvOp* operation)
    {
    qDebug() << "+ PosPsySettingsPlugin::initialise()";	
    mOperation = operation;
    mPsyList.clear();
    // get the list of psys 
    int error = mEngine->listPsys(mPsyList);
    // if the list is not available return error
    if (error == KErrNone)
        {
        // create group
        mGroup = new HbDataFormModelItem(HbDataFormModelItem::GroupItem, tr(
                "Positioning methods"), 0);
        // append the group to data form using operation
        operation->appendDataFormItem(mGroup);

        // add group items
        addItems();

        // make connection to update the group inase of change in availbale list of psy
        QObject::connect(mEngine, SIGNAL(PSYChange()), this,
                SLOT(updateGroup()));
        qDebug() << "- PosPsySettingsPlugin::initialise()";	
        return KErrNone;
        }
    qDebug() << "- PosPsySettingsPlugin::initialise()";	    
    return error;
    }

//---------------------------------------------------------------------
// PosPsySettingsPlugin::onPsyPressed( int state, int psyUid )
// 
//---------------------------------------------------------------------
void PosPsySettingsPlugin::onPsyPressed(int index)
    {
    qDebug() << "+ PosPsySettingsPlugin::onPsyPressed()";	
    // get the check box item using the index
    HbDataFormModelItem *checkBox = mGroup->childAt(index);
    // get the psy name for the check box
    QVariant name(checkBox->contentWidgetData("text"));
    QString psyName = name.toString();
    qDebug() << "Psy to be changed:" << psyName;
   
    // get the psyuid for the psy 
    TUid psyUid = mPsyList.at(index).mPsyModuelId;

    // get the state of the check box
    QVariant state(checkBox->contentWidgetData(("checkState")));
    int psyState = state.toInt();

    // change the psy state based on check box state
    if (psyState == Qt::Checked)
        {
        qDebug() << "Psy value:true";	
        int error = mEngine->changePsyState(psyUid, PsyEnable);
        }
    else
        {
        qDebug() << "Psy value:false";		
        int error = mEngine->changePsyState(psyUid, PsyDisable);
        }
	qDebug() << "- PosPsySettingsPlugin::onPsyPressed()";		
    }

//---------------------------------------------------------------------
// PosPsySettingsPlugin::updateGroup
// 
//---------------------------------------------------------------------
void PosPsySettingsPlugin::updateGroup()
    {
	qDebug() << "+ PosPsySettingsPlugin::updateGroup()";		
    // remove all connections
    int cnt= mGroup->childCount();
    for (int i = 0; i < cnt; ++i)
        {
        mOperation->removeConnection(mGroup->childAt(i),
                SIGNAL(stateChanged(int)), mPsySignalMapperList[i],
                SLOT(signalmap()));
        }
		cnt = mPsySignalMapperList.count();
    for (int i = 0; i < cnt; ++i)
        {
        QObject::disconnect(mPsySignalMapperList[i], SIGNAL(signalmapped()),
                mSignalMapper, SLOT(map()));
        delete mPsySignalMapperList[i];
        mPsySignalMapperList[i] = NULL;        
        }
 		mPsySignalMapperList.clear();
 		
    QObject::disconnect(mSignalMapper, SIGNAL(mapped(int)), this,
            SLOT(onPsyPressed(int)));

    // delete the QSignalMapper instance
    if (mSignalMapper)
        {
        delete mSignalMapper;
        mSignalMapper = NULL;
        }    

    // clear list of psys
    mPsyList.clear();
    // get the list of psys 
    int error = mEngine->listPsys(mPsyList);
    // if the list is not available return error
    if (error == KErrNone )
        {
      	// remove all group's children
       //mGroup->removeChildren(0, oldChildCount);
       //ToDo: Hack from HbDataForm team
        int cnt = mGroup->childCount();
        for(int i=0;i<cnt; ++i)	{
        	mGroup->removeChild(0);	
        	}
        // add the group items
        addItems();
        if (mOperation->isExpanded(mOperation->indexFromItem(mGroup)))
            {
            // set as expanded since group was already expanded
            mOperation->setExpanded(mOperation->indexFromItem(mGroup), true);
            }
        }
	qDebug() << "- PosPsySettingsPlugin::updateGroup()";		
    }

//---------------------------------------------------------------------
// PosPsySettingsPlugin::addItems
// 
//---------------------------------------------------------------------
void PosPsySettingsPlugin::addItems()
    {
    qDebug() << "+ PosPsySettingsPlugin::addItems()";		
    // create the signal mapper 
    if(mSignalMapper)
        {
        delete mSignalMapper;
        mSignalMapper = NULL;
        }
    mSignalMapper = new QSignalMapper();

    // for each of the psys in the list
    // - add a check box item with name
    // - set the checkbox state according to the psy's current state
    // - make connections
    // foreach( PosPsyInfo psy, mPsyList )
    int cnt = mPsyList.count();
    qDebug() << "No of Psys: " << cnt;
    for (int i = 0; i < cnt; ++i)
        {
        // create the check box item
        HbDataFormModelItem* checkBox = new HbDataFormModelItem(
                    HbDataFormModelItem::CheckBoxItem, QString());
        QString psyName = mPsyList.at(i).mPsyName;
        qDebug() << "Psy Name: " << psyName;
        checkBox->setContentWidgetData("text", psyName);

        // set the state of checkbox
        if (mPsyList.at(i).mPsyState == PsyEnable)
            {
            checkBox->setContentWidgetData("checkState", Qt::Checked);
            }
        else
            {
            checkBox->setContentWidgetData("checkState", Qt::Unchecked);
            }
        // append the check box item to the group
        mGroup->appendChild(checkBox);

        // create a new psySignalMapper for each checkbox created
        PosSettingsSignalMapper* psySignalMapper =
                new PosSettingsSignalMapper();
        mPsySignalMapperList.append(psySignalMapper);

        // add connections on change of check box state
        mOperation->addConnection(checkBox, SIGNAL(stateChanged(int)),
                mPsySignalMapperList[i], SLOT(signalmap()));
        QObject::connect(mPsySignalMapperList[i], SIGNAL(signalmapped()),
                mSignalMapper, SLOT(map()));
        // set mapping of each check box item to its index
        mSignalMapper->setMapping(mPsySignalMapperList[i], i);

        }
    QObject::connect(mSignalMapper, SIGNAL(mapped(int)), this,
            SLOT(onPsyPressed(int)));
	qDebug() << "- PosPsySettingsPlugin::addItems()";		
    }

Q_EXPORT_PLUGIN2(pospsysettings, PosPsySettingsPlugin)
;
