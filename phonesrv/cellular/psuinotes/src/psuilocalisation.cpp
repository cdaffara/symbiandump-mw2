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
 * Description:  
 *
 */
#include "psuilocalisation.h"
#include "psuilogging.h"
#include <QTranslator>
#include <QLocale>
#include <QApplication>


// Constant definitions 
const char *TS_FILE_TELEPHONE_CP = "telephone_cp";
const char *TS_FILE_COMMON = "common"; 

/*!
    \class PsUiLocalisation
    \brief Localisation utility class for 
           Telephony control panel plugins. 

    Use installTranslator function for installing 
    needed translation files. 
    
    Takes ownership of the created QTranslator objects
    and destroys them when PsUiLocalisation 
    object is destructed. 
*/


/*!
  PsUiLocalisation::PsUiLocalisation() 
 */
PsUiLocalisation::PsUiLocalisation(QObject *parent)
 :QObject(parent)
{
    DPRINT; 
}


/*!
  PsUiLocalisation::~PsUiLocalisation() 
*/
PsUiLocalisation::~PsUiLocalisation()
{
    DPRINT << ":IN"; 
    removeTranslators();
    DPRINT << ":OUT";
}


/*!
  PsUiLocalisation::installTranslator() 
*/
bool PsUiLocalisation::installTranslator(
        TranslationFileId translationFileId)
{
    DPRINT << ": IN";
   
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    bool translatorLoaded(false);  

    QString fileName; 
    switch (translationFileId) {
        case TranslationFileTelephoneCp: 
            fileName = TS_FILE_TELEPHONE_CP; 
            break; 
        case TranslationFileCommon:
            fileName = TS_FILE_COMMON; 
            break;
        default: 
            DWARNING << ": Unknown enum value!";
            break; 
    }

    if (!fileName.isEmpty()) {
        QTranslator* translator = new QTranslator;
        translatorLoaded = translator->load(
                path + fileName + "_" + lang);
        if (translatorLoaded) {
            m_translators.append(translator); 
            qApp->installTranslator(translator);
            DPRINT << ": translator installed: " << fileName; 
        } else {
            delete translator; 
            translator = NULL;
            DWARNING << ": Translator not loaded!";
        }
    }

    DPRINT << ": OUT";
    return translatorLoaded;
}


/*!
  PsUiLocalisation::removeTranslators()
*/
void PsUiLocalisation::removeTranslators()
{
    DPRINT << ": IN";

    foreach (QTranslator *translator, m_translators) {
        qApp->removeTranslator(translator);
    }    
    qDeleteAll(m_translators);
    m_translators.clear();
    
    DPRINT << ": OUT";
}


// End of File. 
