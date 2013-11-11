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
 * Description:Implementation of the Positioning Settings Advanced View class 
 *
 */

#include "possettingsadvview.h"
#include "possettingspluginsloader.h"
#include "possettingsgroupboxitem.h"
#include <QDebug>
#include <QFileSystemWatcher>

#ifdef Q_OS_SYMBIAN
#define PLUGIN_PATH QString("\\resource\\qt\\plugins\\positioningsettings")

#else
#define PLUGIN_PATH QString("C:\\qt\\plugins\\positioningsettings")
#endif 

//---------------------------------------------------------------------
// PosSettingsAdvView::PosSettingsAdvView()
// Constructor
//---------------------------------------------------------------------
PosSettingsAdvView::PosSettingsAdvView( QGraphicsItem *parent ):
CpBaseSettingView(0,parent)
    {
    qDebug() << "+ PosSettingsAdvView::PosSettingsAdvView()";
    // create instance of HbDataForm
    mAdvancedDataForm = new HbDataForm();
    
    // Create an instance of HbDataFormModel
    HbDataFormModel* advancedDataFormModel = new HbDataFormModel();

    // set the model
    mAdvancedDataForm->setModel( advancedDataFormModel );

    // set the Advanced settings form for this view
    this->setWidget( mAdvancedDataForm );
    
    //set prototype for custom controls
    QList<HbAbstractViewItem *> protoTypeList = mAdvancedDataForm->itemPrototypes();
    protoTypeList.append(new PosSettingsGroupboxItem(this));  
    mAdvancedDataForm->setItemPrototypes(protoTypeList);
    
    //Heading                   
     mHeading = advancedDataFormModel->appendDataFormItem(static_cast<HbDataFormModelItem::DataItemType>(GroupboxItem),
                                                   QString(),
                                                   advancedDataFormModel->invisibleRootItem());
     mHeading->setContentWidgetData("heading",
                                hbTrId("txt_loe_subtitle_advanced_positioning_settings"));

    
    
    // Create an instance of PosSettingsAdvancedOperation
    mAdvancedOperation = new PosSettingsAdvOp(this);
    

    mPluginsLoader = new PosSettingsPluginsLoader();
    // load the plugins
    mPluginsList = mPluginsLoader->loadPlugins();
    
    // for each plugin detected call the plugin's initialize method
   
    int cnt = mPluginsList.count();
    int error = 0;
    for(int i=0;i<cnt;i++)
        {
				error = mPluginsList[i]->initialise(mAdvancedOperation );
        //pluging is unloaded if the there is an error, with exception for not found.
        if (error != 0 && error != -1)
            {
            delete mPluginsList[i];
            mPluginsList[i] = 0;
            mPluginsList.removeAt(i);
            --cnt;
            --i;
            }
        }
    // create a QFileSystemWatcher instance
    mWatcher = new QFileSystemWatcher();
    mWatcher->addPath(PLUGIN_PATH);
    
    // make connection to update the advanced view on change in the plugins
    QObject::connect(mWatcher,SIGNAL(fileChanged ( const QString & path )),
                    this,SLOT(updateAdvView()));
    qDebug() << "- PosSettingsAdvView::PosSettingsAdvView()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvView::~PosSettingsAdvView()
// Destructor
//---------------------------------------------------------------------
PosSettingsAdvView::~PosSettingsAdvView()
    {   
    qDebug() << "+ PosSettingsAdvView::~PosSettingsAdvView()";
  //  mAdvancedDataForm->removeAllConnection();	 
    delete mAdvancedDataForm;
    mAdvancedDataForm = NULL;
    
    
    delete mAdvancedOperation;
    mAdvancedOperation = NULL;
    
    delete mPluginsLoader;
    mPluginsLoader = NULL;
    
    // delete the list of plugins
    foreach( PosSettingsAdvInterface *plugin, mPluginsList )
            {
            delete plugin;
            plugin = 0;
            }
    mPluginsList.clear();
    
    //ToDo: disconnect mWatcher'signal
    delete mWatcher;
    mWatcher = NULL;
    qDebug() << "- PosSettingsAdvView::~PosSettingsAdvView()";    
    }

//---------------------------------------------------------------------
// PosSettingsAdvView::updateAdvView()
// 
//---------------------------------------------------------------------
void PosSettingsAdvView::updateAdvView()
    {
    qDebug() << "+ PosSettingsAdvView::updateAdvView()";
    // clear the plugins list
    foreach( PosSettingsAdvInterface *plugin, mPluginsList )
        {
        delete plugin;
        plugin= NULL;
        }
    mPluginsList.clear();
    
    // reload the plugins again
    mPluginsList = mPluginsLoader->loadPlugins();
    
    int count = mPluginsList.count();
    
    // for each plugin detected call the plugin's initialize method
    for( int i=0;i<count;++i )
        {
        int error = mPluginsList[i]->initialise( mAdvancedOperation );
        if( error != KErrNone )
            {
            delete mPluginsList[i];
            mPluginsList[i] =NULL;
            }
        }
    qDebug() << "- PosSettingsAdvView::updateAdvView()";
    }
