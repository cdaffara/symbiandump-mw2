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
* Description:  Positioning settings view class. 
*
*/

#ifndef POSSETTINGSFORM_H
#define POSSETTINGSFORM_H

#include <hbdataform.h>

class HbDataFormModelItem;
class PosSettingsEngine;
class PosSettingsAdvView;
class HbAction; 

class PosSettingsForm : public HbDataForm
    {
    Q_OBJECT
private:
    /*
     *  Enumarator for the positioning category type
     */
    enum PosTypeIndex
        {
        /*
         *  The position of GPS in HbDataForm
         */
        PosTypeIndexGps         = 1, 
        
        /*
         *  The position of Wireless in HbDataForm
         */
        PosTypeIndexWireless    = 2 ,
        
        /*
         *  The position of Background in HbDataForm
         */
        PosTypeIndexBackground  = 3
        };
public: 
    /**
     * Constructor.
     */
    explicit PosSettingsForm( PosSettingsEngine& settingsEngine,
                                      QGraphicsItem *parent = 0 );
    
    /**
     *  Destructor
     */
    ~PosSettingsForm();

private:
    /**
     * It will initiate the basic setting view 
     */
    void initSettingModel();
    
    /**
     * It will validate visibility of Positioning type 
     * 
     * posTypeModelItemIndex[in] - The position at which control should be placed   
     */
    void validatePosTypeVisibility(PosTypeIndex posTypeModelItemIndex);
    
    /**
     * It will insert/remove Positioning type control in HbDataForm
     * 
     * insert[in] - if true insert control else remove it
     * posTypeModelItem[in] - HbDataFormModelItem handle for  position type
     * posTypeModelItemIndex[in] - The position at which control should be placed 
     * posIndex[in] - The absolute poisition index at which control should be placed. 
     *								It will different from posTypeModelItemIndex for Wireless and Background 
     *								depending on GPS, Wireless are visible.
     */
    void insertOrRemovePosTypeModelItem(bool insert, HbDataFormModelItem*  posTypeModelItem, 
                    PosTypeIndex posTypeModelItemIndex, int posIndex);
    /*
     * It will create HbDataFormModelItem based on the position specified
     * 
     * posTypeModelItemIndex [in] - The position at which control should be placed     
     */
    void createPosTypeModelItem(PosTypeIndex posTypeModelItemIndex);
    
  	/**
     * It will mark/unmark positioning category checkbox
     * 
     * posTypeModelItemIndex [in] - The position at which control should be placed  
     */
    void setPosTypeState(PosTypeIndex posTypeModelItemIndex);
    
    /**
     * It will add connection for the given model item
     * 
     * posTypeModelItem [in] - HbDataFormModelItem handle for  position type
     */
    void addPosTypeConnection(HbDataFormModelItem*  posTypeModelItem);
    
private slots:
   /**
    * It will be called when 'Advanced' push button is pressed   
    */
    void onPressedAdvanced();
    
    /**
     * It will be called when 'GPS' checkbox is clicked   
     */
    void onPressedGps();
    
    /**
     * It will be called when 'Wirelss' checkbox is clicked      
     */
    void onPressedWireless();
    
    /*
     * It will be called when 'Background' checkbox is clicked   
     */
    void onPressedBackground();
    /*
     * It will be called when the Back Button is pressed to dismiss the 
     * advanced view
     */
    void closeAdvancedView();

 		/**
     * It will validate visibility of all positioning type controls
     * It will also be called when new positioning technology is 
     * added at runtime
     */
    void validateVisibility();
    
private: // Data memebers
    /**
     * Handle to engine
     * Not Own
     */
    PosSettingsEngine& mSettingsEngine; 
    
    /**
     * Handle to 'GPS' checkbox
     * Not Own
     */
    HbDataFormModelItem *mGpsPosType;      

    /**
     * Handle to 'Wireless' checkbox
     * Not Own
     */
    HbDataFormModelItem *mWirelessPosType; 
   
    /**
     * Handle to 'Background' checkbox
     * Not Own
     */
    HbDataFormModelItem *mBgPosType;
  
    /**
     * Handle to 'Advanced' push button
     * Not Own
     */
    HbDataFormModelItem *mAdvancedSettings;
   
    /**
     * Handle to Heading groupbox
     * Not Own
     */
    HbDataFormModelItem *mHeading;
    
    /**
     * Pointer to Advanced view class
     * Owns this pointer.
     */
    PosSettingsAdvView* mAdvancedView;  
    
    /**
     * handle to the Back Action
     */
    HbAction* mBackAction;

    };
#endif // POSSETTINGSFORM_H
