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
 * Description: Declaration of the Positioning Methods plugin to the 
 *							Avanced settings view
 *
 */

#ifndef POSPSYSETTINGSPLUGIN_H
#define POSPSYSETTINGSPLUGIN_H


#include "possettingsadvinterface.h"
#include "pospsysettingscommon.h"
#include "possettingssignalmapper.h"

#include <QObject>
#include <QSignalMapper>
#include <hbdataformmodelitem.h>

// Forward Declaration
class PosPsySettingsEngine;
class PosSettingsAdvOp;
class PosSettingsSignalMapper;

class PosPsySettingsPlugin : public QObject, public PosSettingsAdvInterface
{
Q_OBJECT
    Q_INTERFACES( PosSettingsAdvInterface )
public: 
   /**
   * Constructor
   */
    PosPsySettingsPlugin();
  /**
   * Destructor
   */
   ~PosPsySettingsPlugin();
public:
    /**
     * Derived from PosSettingsAdvInterface   
     */ 
    int initialise( PosSettingsAdvOp* operation );
    
private slots:
		/**
		* This will be called when any of the positioning methods checkbox is clicked.
		* @param[in] state - indicates the state of the checkbox,whether enabled/disabled
		* @param[in] psyUid - indicates the Uid of the Psy that has been clciked.
		*/
		void onPsyPressed(int index);
		/**
		 * This method will be called on a change in any of the psy info.
		 * It willbe used to update the positioning methods group to reflect
		 * the latest available info of psys.
		 */
		void updateGroup();
		
private:
		/**
		 * adds items to the group & make connections
		 */
		void addItems();
		
		
private:
		/**
		* Pointer to the PosPsySettingsEngine
		* Owns
		*/
		PosPsySettingsEngine* mEngine;
		/**
		* List of available Positioning methods
		*/
		QList<PosPsyInfo> mPsyList;
		/**
		 * pointer to the HbDataFormModelItem
		 * Does not own
		 */
		HbDataFormModelItem* mGroup;
		/**
		 * Signal mapper used to map the signal emitted from
		 * each of the psy check box to the slot
		 * Owns
		 */
		QSignalMapper *mSignalMapper;
		/**
		 * List of psy signal mapper class
		 */
		QList<PosSettingsSignalMapper*> mPsySignalMapperList;
		/**
		 * reference to PosSettingsAdvOp
		 */
		PosSettingsAdvOp* mOperation;
   
};
#endif // POSPSYSETTINGSPLUGIN_H
