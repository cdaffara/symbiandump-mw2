/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef PSUILOCALISATION_H_
#define PSUILOCALISATION_H_

#include <QObject>
#include <QList>

// Forward declarations 
class QTranslator; 

#ifdef BUILD_PSUINOTES
#define PSUINOTES_EXPORT Q_DECL_EXPORT
#else
#define PSUINOTES_EXPORT Q_DECL_IMPORT
#endif

class PSUINOTES_EXPORT PsUiLocalisation: public QObject
    {
    Q_OBJECT
    
public: 
    
    enum TranslationFileId {
        TranslationFileCommon, 
        TranslationFileTelephoneCp
    }; 
        
public:
    
    PsUiLocalisation(QObject *parent = NULL);
    ~PsUiLocalisation();

    /**
     * Installs specific translation file
     * @param translationFileId Id of file to be installed
     */
    bool installTranslator(TranslationFileId translationFileId);
    
    /**
     * Removes installed translators
     */
    void removeTranslators();

private:     
    
    QList<QTranslator *> m_translators;
    };


#endif /* PSUILOCALISATION_H_ */
