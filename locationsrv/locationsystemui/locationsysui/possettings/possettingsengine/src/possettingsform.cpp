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
* Description:  This class is for first basic view for positioning settings
*
*/


#include "possettingsform.h"
#include "possettingspushbuttonitem.h"
#include "possettingsgroupboxitem.h"
#include "checkcustomviewitem.h"
#include "possettingsengine.h"
#include "possettingsadvview.h"
#include <QDebug>
#include <hbdataformmodelitem.h>
#include <hbdataformmodel.h>
#include <hbinstance.h>
#include <hbaction.h>
#include <hbstyleloader.h>

static const QString FILE_PATH_CSS = ":/data";

// ---------------------------------------------------------------------------
// PosSettingsForm::PosSettingsForm
// Constructor
// ---------------------------------------------------------------------------
PosSettingsForm::PosSettingsForm( 
                                       PosSettingsEngine& settingsEngine,
                                       QGraphicsItem *parent ):
                                       HbDataForm( parent ),mSettingsEngine( settingsEngine )
    {
    qDebug() << "+ PosSettingsForm::PosSettingsForm()";
    //initialize the form model
    initSettingModel(); 
    
    // slots for required signals on required controls
    addConnection( mGpsPosType, SIGNAL(released()),
                    this, SLOT(onPressedGps()) );
    addConnection( mWirelessPosType, SIGNAL(released()),
                        this, SLOT(onPressedWireless()) );
    addConnection( mBgPosType, SIGNAL(released()),
                        this, SLOT(onPressedBackground()) );
    addConnection( mAdvancedSettings, SIGNAL(clicked()),
                        this, SLOT(onPressedAdvanced()) );   
    
    //to check runtime if any positioning technology installed/removed or if its visibility is changed.
    connect(&mSettingsEngine, SIGNAL(positionTechnologyChange()),this, SLOT(validateVisibility()) );
    
    //validate visibility of controls based on visibility of positioning technology
    validateVisibility();
    qDebug() << "- PosSettingsForm::PosSettingsForm()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::~PosSettingsForm
// Destructor
// ---------------------------------------------------------------------------
PosSettingsForm::~PosSettingsForm()
    {
    qDebug() << "+ PosSettingsForm::PosSettingsForm()";
    // Remove custom layouts
    HbStyleLoader::unregisterFilePath(FILE_PATH_CSS);
    
    //TODO Disconnect of mSettingsEngine positionTechnologyChange
    
    removeConnection( mAdvancedSettings, SIGNAL(clicked()),
                            this, SLOT(onPressedAdvanced()) );
    removeConnection( mGpsPosType, SIGNAL(released),
                        this, SLOT(onPressedGps()) );
    removeConnection( mWirelessPosType, SIGNAL(released),
                        this, SLOT(onPressedWireless()) );
    removeConnection( mBgPosType, SIGNAL(released),
                        this, SLOT(onPressedBackground()) );
    
    //ToDo: removeAllConnection crashes with Kern-Exec3 error 
    //removeAllConnection();
    qDebug() << "- PosSettingsForm::PosSettingsForm()";     
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::initSettingModel
// ---------------------------------------------------------------------------
void PosSettingsForm::initSettingModel()
    {
    qDebug() << "+ PosSettingsForm::initSettingModel()";
    HbDataFormModel *model = new HbDataFormModel(0);
    this->setModel(model);
    
    //set prototype for custom controls
    QList<HbAbstractViewItem *> protoTypeList = this->itemPrototypes();
    protoTypeList.append(new PosSettingsPushButtonItem(this) );
    protoTypeList.append(new CheckCustomViewItem());  
    protoTypeList.append(new PosSettingsGroupboxItem(this));  
    this->setItemPrototypes(protoTypeList);
    
    // Set custom layouts
    HbStyleLoader::registerFilePath(FILE_PATH_CSS);    
    
    //Heading                   
    mHeading = model->appendDataFormItem(static_cast<HbDataFormModelItem::DataItemType>(GroupboxItem),
                                                  QString(),
                                                  model->invisibleRootItem());
    mHeading->setContentWidgetData("heading",
                               hbTrId("txt_loe_subtitle_positioning_settings"));

         
         
    //GPS
    createPosTypeModelItem(PosTypeIndexGps);
    model->appendDataFormItem(mGpsPosType, model->invisibleRootItem());    
    //Wireless
    createPosTypeModelItem(PosTypeIndexWireless);
    model->appendDataFormItem(mWirelessPosType, model->invisibleRootItem());           
    //Background
    createPosTypeModelItem(PosTypeIndexBackground);
    model->appendDataFormItem(mBgPosType, model->invisibleRootItem());       
 
    //Advance                   
    mAdvancedSettings = model->appendDataFormItem(static_cast<HbDataFormModelItem::DataItemType>(PushButtonItem),
                                                  QString(),
                                                  model->invisibleRootItem());
    mAdvancedSettings->setContentWidgetData("text",
                               hbTrId("txt_loe_button_advanced"));

         
    setPosTypeState(PosTypeIndexGps);
    setPosTypeState(PosTypeIndexWireless);
    setPosTypeState(PosTypeIndexBackground);
    qDebug() << "- PosSettingsForm::initSettingModel()";  
	}

// ---------------------------------------------------------------------------
// PosSettingsForm::onPressedAdvanced
// ---------------------------------------------------------------------------
void PosSettingsForm::onPressedAdvanced()
{
    qDebug() << "+ PosSettingsForm::onPressedAdvanced()";
    mAdvancedView = new PosSettingsAdvView();
    HbMainWindow* mainWindow = hbInstance->allMainWindows().at(0);

    mainWindow->addView(mAdvancedView);
    mainWindow->setCurrentView(mAdvancedView);
     
    //take back action from controlpanel main window.
    mBackAction = new HbAction(Hb::BackNaviAction ,mAdvancedView);
    mAdvancedView->setNavigationAction(mBackAction);

    connect(mBackAction, SIGNAL(triggered()), this, SLOT(closeAdvancedView()));
    mAdvancedView->show();
    qDebug() << "- PosSettingsForm::onPressedAdvanced()"; 
}
// ---------------------------------------------------------------------------
// PosSettingsForm::closeAdvancedView
// ---------------------------------------------------------------------------
//
void PosSettingsForm::closeAdvancedView()
    {
    qDebug() << "+ PosSettingsForm::closeAdvancedView()";
    disconnect(mBackAction, SIGNAL(triggered()), this, SLOT(closeAdvancedView()));

    HbMainWindow* mainWindow = hbInstance->allMainWindows().at(0);
    mainWindow->removeView(mAdvancedView);

    mAdvancedView->setParent(NULL);
    
    delete mAdvancedView;
    mAdvancedView = NULL;
    
    // refresh first view 
    setPosTypeState( PosTypeIndexGps);
    setPosTypeState( PosTypeIndexWireless);
    setPosTypeState( PosTypeIndexBackground);
    validatePosTypeVisibility(PosTypeIndexBackground);  
    qDebug() << "- PosSettingsForm::closeAdvancedView()"; 
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::onPressedGps
// ---------------------------------------------------------------------------
void PosSettingsForm::onPressedGps()
    { 
    qDebug() << "+ PosSettingsForm::onPressedGps()";	
    validatePosTypeVisibility(PosTypeIndexBackground);  	
    
     int state = mGpsPosType->contentWidgetData("checkState").toInt();
     if(state == Qt::Checked)
        {    //if Gps enable
        mSettingsEngine.setPositionTechnologyState(StateEnable, PositionTechnologyGps);
        }
    else 
        {    //if Gps disable
        mSettingsEngine.setPositionTechnologyState(StateDisable, PositionTechnologyGps);
        }
    qDebug() << "- PosSettingsForm::onPressedGps()";    
    }
	
// ---------------------------------------------------------------------------
// PosSettingsForm::onPressedWireless
// ---------------------------------------------------------------------------
void PosSettingsForm::onPressedWireless()
    {  
    qDebug() << "+ PosSettingsForm::onPressedWireless()";
    validatePosTypeVisibility(PosTypeIndexBackground); 
    int state = mWirelessPosType->contentWidgetData("checkState").toInt();
     if(state == Qt::Checked)
        {
        //if Wireless enable
        mSettingsEngine.setPositionTechnologyState(StateEnable, PositionTechnologyNetwork);
        }
    else 
        {    //if Wireless disable
        mSettingsEngine.setPositionTechnologyState(StateDisable, PositionTechnologyNetwork);
        }
    qDebug() << "- PosSettingsForm::onPressedWireless()"; 
    }
	
// ---------------------------------------------------------------------------
// PosSettingsForm::onPressedBackground
// ---------------------------------------------------------------------------
void PosSettingsForm::onPressedBackground()
    {
    qDebug() << "+ PosSettingsForm::onPressedBackground()";
    int state = mBgPosType->contentWidgetData("checkState").toInt();	
    if(state == Qt::Checked)
        {
        //if Bg enable
        mSettingsEngine.setBackGroundPositioningState(StateEnable);
        mSettingsEngine.setCRBackgroundPositioningStatus(StateEnable);
        }
     else 
        {    //if Bg disable;
        mSettingsEngine.setBackGroundPositioningState(StateDisable);
        mSettingsEngine.setCRBackgroundPositioningStatus(StateDisable);
        }
    qDebug() << "- PosSettingsForm::onPressedBackground()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::validatePosTypeVisibility
// ---------------------------------------------------------------------------
//
void PosSettingsForm::validatePosTypeVisibility(PosTypeIndex posTypeModelItemIndex)
    {   
    qDebug() << "+ PosSettingsForm::validatePosTypeVisibility()";
    bool valid = false;
    HbDataFormModelItem* posTypeModelItem = NULL; //does not own
    int posIndex = posTypeModelItemIndex;
    switch(posTypeModelItemIndex) {
        case PosTypeIndexGps: {
            valid = mSettingsEngine.isPositionTechnologyAvailable(PositionTechnologyGps);
            posTypeModelItem = mGpsPosType;
            break;
            }
        case PosTypeIndexWireless: {
            valid = mSettingsEngine.isPositionTechnologyAvailable(PositionTechnologyNetwork);
            posTypeModelItem = mWirelessPosType;
            HbDataFormModel* model =  static_cast<HbDataFormModel*>(this->model());
            //if GPS is not there, it will be at position 1
             if(!model->indexFromItem(mGpsPosType).isValid()) {
             	 --posIndex ;
            	}
            break;
            }
        case  PosTypeIndexBackground: {
            //if Gps and Wireless are not marked, then make Background invisible            
            valid = (mGpsPosType->contentWidgetData("checkState").toInt() == Qt::Checked ||
                        mWirelessPosType->contentWidgetData("checkState").toInt() == Qt::Checked);
            posTypeModelItem = mBgPosType;
            HbDataFormModel* model =  static_cast<HbDataFormModel*>(this->model());
            
             //if GPS is not there, decremenet position
            if(!model->indexFromItem(mGpsPosType).isValid()) {
             		--posIndex;
            	}
            //if Wireless is not there, decremenet position 	
            if(!model->indexFromItem(mWirelessPosType).isValid()) {
             		--posIndex;
				}
			 //if no gps no wireless, quit control-panel application	
			if(posIndex == PosTypeIndexGps-1) { 
             		qApp->quit();
             		}
            break;
            }
        default:{
            break;
            }
        
        }
   qDebug() << "valid =" << valid;	    
   insertOrRemovePosTypeModelItem(valid, posTypeModelItem, posTypeModelItemIndex, posIndex);
   qDebug() << "- PosSettingsForm::validatePosTypeVisibility()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::InsertOrRemovePosTypeModelItem
// ---------------------------------------------------------------------------
//
void PosSettingsForm::insertOrRemovePosTypeModelItem(bool insert, HbDataFormModelItem*  posTypeModelItem, 
														PosTypeIndex posTypeModelItemIndex, int posIndex)
    {
    qDebug() << "+ PosSettingsForm::insertOrRemovePosTypeModelItem()";
    HbDataFormModel* model =  static_cast<HbDataFormModel*>(this->model());
    if(!insert) {
        if(model->indexFromItem(posTypeModelItem).isValid()) {
            if(model->removeItem(posTypeModelItem)) {
                //Note: In removeItem, modelitem is removed and then deleted too.
                // So next time when we want to check if it is present, we have to create dummy modelitem
                // else with existing handle, we will get kern exec 3 error
                createPosTypeModelItem(posTypeModelItemIndex);  
                if(posTypeModelItemIndex == PosTypeIndexBackground) {
                        mSettingsEngine.setBackGroundPositioningState(StateDisable);
                    }
                }
            }
        }
    else {
        if(!model->indexFromItem(posTypeModelItem).isValid()) { 
                model->insertDataFormItem(posIndex,posTypeModelItem,model->invisibleRootItem());  
                addPosTypeConnection(posTypeModelItem);
                
                if(posTypeModelItemIndex == PosTypeIndexBackground){
                    //Restore to old state in case of background positioning.
                    if(mSettingsEngine.cRBackgroundPositioningStatus()) {
                        mSettingsEngine.setBackGroundPositioningState(StateEnable);
                        }
                    else {
                        mSettingsEngine.setBackGroundPositioningState(StateDisable);                    
                        }
                    setPosTypeState(PosTypeIndexBackground);
                    }
                else //Restore old state of GPS/Wireless
                    setPosTypeState(posTypeModelItemIndex); 
           }
        }
    qDebug() << "- PosSettingsForm::insertOrRemovePosTypeModelItem()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::createPosTypeModelItem
// ---------------------------------------------------------------------------
//
void PosSettingsForm::createPosTypeModelItem(PosTypeIndex posTypeModelItemIndex)
    {
    qDebug() << "+ PosSettingsForm::createPosTypeModelItem()";
    HbDataFormModel* model =  static_cast<HbDataFormModel*>(this->model());
    switch(posTypeModelItemIndex) {
        case PosTypeIndexGps: {         
           
        mGpsPosType = new HbDataFormModelItem(HbDataFormModelItem::CheckBoxItem,
                        QString());     
           mGpsPosType->setData(HbDataFormModelItem::DescriptionRole, hbTrId("txt_loe_info_enable_for_most_accurate_positioning"));
           mGpsPosType->setContentWidgetData("text", hbTrId("txt_loe_list_gps"));
            break;
            }
        case PosTypeIndexWireless: {          
           
            mWirelessPosType = new HbDataFormModelItem(HbDataFormModelItem::CheckBoxItem,
                                QString());    
            mWirelessPosType->setData(HbDataFormModelItem::DescriptionRole, hbTrId("txt_loe_info_use_wifi_and_mobile_networks_to_get"));
            mWirelessPosType->setContentWidgetData("text", hbTrId("txt_loe_list_wireless_networks"));   
           break;
           }
        case PosTypeIndexBackground:{  
            mBgPosType = new HbDataFormModelItem(HbDataFormModelItem::CheckBoxItem,
                                QString());
		    mBgPosType->setData(HbDataFormModelItem::DescriptionRole, hbTrId("txt_loe_info_enable_applications_and_services_upda"));
		    mBgPosType->setContentWidgetData("text", hbTrId("txt_loe_list_background_positioning")); 		  	
           break;
           }
        default: {
            break;
           }
        }
    qDebug() << "- PosSettingsForm::createPosTypeModelItem()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::validateVisibility
// ---------------------------------------------------------------------------
//
void PosSettingsForm::validateVisibility()
    {  
    qDebug() << "+ PosSettingsForm::validateVisibility()";
    validatePosTypeVisibility(PosTypeIndexGps);
    validatePosTypeVisibility(PosTypeIndexWireless);
    validatePosTypeVisibility(PosTypeIndexBackground);
    qDebug() << "- PosSettingsForm::validateVisibility()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::setPosTypeState
// ---------------------------------------------------------------------------
void PosSettingsForm::setPosTypeState(PosTypeIndex posTypeModelItemIndex)
    {
    qDebug() << "+ PosSettingsForm::setPosTypeState()";
    int state = Qt::Unchecked;
    switch(posTypeModelItemIndex){
        case PosTypeIndexGps: {        
            if(mSettingsEngine.isPositionTechnologyEnabled(PositionTechnologyGps))
                state = Qt::Checked;
            mGpsPosType->setContentWidgetData("checkState", state);      
            break;
            }
        case PosTypeIndexWireless: {
            if(mSettingsEngine.isPositionTechnologyEnabled(PositionTechnologyNetwork))
                   state = Qt::Checked;
            mWirelessPosType->setContentWidgetData("checkState",state);                    
            break;
            }
        case PosTypeIndexBackground: {
            if(mSettingsEngine.isBackGroundPositioningEnabled())
                  state = Qt::Checked;
            mBgPosType->setContentWidgetData("checkState",state);      
            break;
            }
        default : {
            break;
            }
        }
    qDebug() << "- PosSettingsForm::setPosTypeState()";
    }

// ---------------------------------------------------------------------------
// PosSettingsForm::addPosTypeConnection
// ---------------------------------------------------------------------------
void PosSettingsForm::addPosTypeConnection(HbDataFormModelItem*  posTypeModelItem)
    {
    qDebug() << "+ PosSettingsForm::addPosTypeConnection()";
    if(posTypeModelItem == mGpsPosType)
        {       
        addConnection( mGpsPosType, SIGNAL(released()),
                                                  this, SLOT(onPressedGps()) );
        qDebug() << "- PosSettingsForm::addPosTypeConnection()";
        return;
        }
    
    if(posTypeModelItem == mWirelessPosType)        
       {
       addConnection( mWirelessPosType, SIGNAL(released()),
                                                 this, SLOT(onPressedWireless()) );
       qDebug() << "- PosSettingsForm::addPosTypeConnection()";
       return;
       }
    if(posTypeModelItem == mBgPosType) 
       {
       addConnection( mBgPosType, SIGNAL(released()),
                                                 this, SLOT(onPressedBackground()) );
       qDebug() << "- PosSettingsForm::addPosTypeConnection()";
       return;
       }        
    }
//EOF
