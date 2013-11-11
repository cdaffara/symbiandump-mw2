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
#include <hbdevicemessagebox.h>
#include <hbdevicenotificationdialog.h>
#include <HbDeviceProgressDialog>
#include <hblistview.h>
#include <hbdialog.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbmessagebox.h>
#include <hbnotificationdialog.h>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <hbstringutil.h>
#include <hbextendedlocale.h>
#include <HbInputDialog>
#include <HbEditorInterface>
#include <hbinputdef.h>
#include <hbinputstandardfilters.h>
#include <xqserviceutil.h>
#include <QApplication>
#include <QTranslator>
#include <Qt>

#include "psuinotes.h"
#include "psuiutils.h"
#include "psuilogging.h"
#include "psuilocalisation.h"

/*!
  PsUiNotes::instance.
 */
PsUiNotes* PsUiNotes::instance()
{
    DPRINT << ": IN";
  
    static PsUiNotes theInstance;
    
    DPRINT << ", instance address: " << reinterpret_cast<int>(&theInstance);
    return &theInstance;
}
 
/*!
  PsUiNotes::PsUiNotes.
 */
PsUiNotes::PsUiNotes(): 
     QObject(NULL), 
     m_notesQueue(NULL),
     m_isNoteShowingOngoing(false),
     m_passwordDialog(NULL),
     m_passwordValidator(NULL)
    {
    DPRINT << ": IN";

    m_notesQueue = new QQueue<QObject*>();
    
    m_psuiSettings = new PsUiSettingsWrapper();
 
    // Localization file loading 
    m_localisation = new PsUiLocalisation(this); 
       
    // Install required translations
    m_localisation->installTranslator(
           PsUiLocalisation::
           TranslationFileCommon);
    m_localisation->installTranslator(
           PsUiLocalisation::
           TranslationFileTelephoneCp);
    DPRINT << ": OUT";
    }

/*!
  PsUiNotes::~PsUiNotes.
 */
PsUiNotes::~PsUiNotes()
{
    DPRINT << ": IN";

    delete m_localisation;
 
    qDeleteAll(*m_notesQueue);
    
    delete m_psuiSettings;
    
    delete m_notesQueue;
        
    DPRINT << ": OUT";
}

/*!
  PsUiNotes::showGlobalProgressNote.
 */
void PsUiNotes::showGlobalProgressNote(
    int &noteId, const QString& text)
{
    DPRINT << ": IN";
    
    HbDeviceProgressDialog *note = 
        new HbDeviceProgressDialog(HbProgressDialog::WaitDialog, this);
    note->setText(text);
    
    if(hbTrId("txt_common_info_requesting") == text){
        QAction *action = new QAction(hbTrId("txt_common_button_hide"), note);
        note->setAction(action, HbDeviceProgressDialog::CancelButtonRole );
    }
    
    noteId = reinterpret_cast<int>(note);
    DPRINT << ", NOTEID: " << noteId;
    QObject::connect(
        note, SIGNAL(aboutToClose()),
        this, SLOT(activeNoteAboutToClose()));
    QObject::connect(
        note, SIGNAL(cancelled()),
        this, SLOT(handleProgressNoteCanceled()));
    m_notesQueue->enqueue(note);    
    launchNextNoteIfReady();
    
    DPRINT << ": OUT";
}
    
/*!
  PsUiNotes::showGlobalNote.
 */
void PsUiNotes::showGlobalNote(int &noteId, const QString& text, 
    HbMessageBox::MessageBoxType messageBoxType)
{
    DPRINT << ": IN";

    HbDeviceMessageBox *note
        = new HbDeviceMessageBox(text, messageBoxType, this);
    if (messageBoxType == HbMessageBox::MessageTypeQuestion ||
        messageBoxType == HbMessageBox::MessageTypeInformation) {
        note->setTimeout(HbPopup::ConfirmationNoteTimeout);
    }
    else {
        note->setTimeout(0);
    }
    noteId = reinterpret_cast<int>(note);
    DPRINT << ", NOTEID: " << noteId;
    
    QObject::connect(
        note, SIGNAL(aboutToClose()),
        this, SLOT(activeNoteAboutToClose()));
    
    m_notesQueue->enqueue(note);
    launchNextNoteIfReady();
    
    DPRINT << ": OUT";
}


/*!
  PsUiNotes::cancelNote.
 */
void PsUiNotes::cancelNote(int noteId)
{
    DPRINT << ": IN";
    
    if (!m_notesQueue->isEmpty()) {
        QObject *note = m_notesQueue->head();
        if(note == reinterpret_cast<QObject *>(noteId)) {
            int index = m_notesQueue->indexOf(reinterpret_cast<QObject *>(noteId));
            Q_ASSERT(-1 < index);
            QObject *note = m_notesQueue->at(index);
            DPRINT << ": NOTEID: " << noteId;
            if (qobject_cast<HbDeviceProgressDialog *>(note)) {
                static_cast<HbDeviceProgressDialog *>(note)->close();
            } else if (qobject_cast<HbDeviceMessageBox *>(note)) {
                static_cast<HbDeviceMessageBox *>(note)->close();
            } else {
                DPRINT << ", UNKNOWN NOTE";
                Q_ASSERT(false);
            }
        }
        else {
            DPRINT << ": remove from queue, noteId: " << noteId;
            m_notesQueue->removeOne(reinterpret_cast<QObject *>(noteId));
        }
    }
    
    DPRINT << ": OUT";
}

/*!
  PsUiNotes::noteShowing.
 */
bool PsUiNotes::noteShowing()
{
    return !m_notesQueue->isEmpty();
}

/*!
  PsUiNotes::showGlobalErrorNote.
 */
void PsUiNotes::showGlobalErrorNote(int &noteId, int errorcode)
{
    DPRINT << ": IN";
    
    QString errorText = "";
    PsUiUtils::errorCodeTextMapping(errorcode, errorText);

    HbDeviceMessageBox *note 
        = new HbDeviceMessageBox(errorText, HbMessageBox::MessageTypeWarning, this);
    note->setTimeout(0);
    noteId = reinterpret_cast<int>(note);
    DPRINT << ", NOTEID: " << noteId;
    QObject::connect(
        note, SIGNAL(aboutToClose()),
        this, SLOT(activeNoteAboutToClose()));
    m_notesQueue->enqueue(note);
    launchNextNoteIfReady();
    
    DPRINT << ": OUT";
} 

/*!
  PsUiNotes::showCallDivertDetails.
 */
void PsUiNotes::showCallDivertDetails(
    const QList<PSCallDivertingStatus*> &divertingStatusList)
{
    DPRINT << ": IN";
    
    DPRINT << ": Status " << divertingStatusList.first()->iStatus;
    DPRINT << ": ServiceGroup " << divertingStatusList.first()->iServiceGroup;
    
    QString status;
    QString content;
    
    switch( divertingStatusList.first()->iStatus )
        {
        case DivertingStatusActive:   
            formatActiveDivertsNoteText(divertingStatusList,content);
            break;
        case DivertingStatusInactive:
        case DivertingStatusNotRegistered:
            status.append(hbTrId("txt_phone_dpopinfo_divert_not_active")); 
            break;
        case DivertingStatusNotProvisioned:
            status.append(hbTrId("txt_phone_info_not_allowed"));
            break;
        case DivertingStatusUnknown:
        default:
            status.append(hbTrId("txt_phone_info_request_not_completed"));
            break;
        } 
    if (divertingStatusList.first()->iStatus != DivertingStatusActive) {
        showGlobalNotificationDialog(status);
    }
    else {     
        HbDeviceMessageBox * note = 
            new HbDeviceMessageBox(content, HbMessageBox::MessageTypeQuestion, this);
        note->setIconVisible(EFalse);
        note->setTimeout(0);
        HbAction *backAction = 
            new HbAction(hbTrId("txt_common_button_close_singledialog") );
        note->setAction(backAction, HbDeviceMessageBox::AcceptButtonRole );
        note->setAction(NULL, HbDeviceMessageBox::RejectButtonRole);
        
        QObject::connect(
            note, SIGNAL(aboutToClose()),
            this, SLOT(activeNoteAboutToClose()));
        
        m_notesQueue->enqueue(note);
        launchNextNoteIfReady();      
    }
 
    DPRINT << ": OUT";
}

/*!
  PsUiNotes::showPasswordQueryDialog.
 */
void PsUiNotes::showPasswordQueryDialog(
    const QString &title, 
    const QValidator &validator,
    int maxPasswordLength)
{
    DPRINT << ": IN";
    
    QScopedPointer<HbInputDialog> passwordDialog(new HbInputDialog());
    
    // configure editor so that only digits can be inputted
    passwordDialog->setPromptText(title);
    passwordDialog->setEchoMode(HbLineEdit::Password);
    passwordDialog->actions().at(0)->setEnabled(false);
    HbLineEdit *hbLineEdit = passwordDialog->lineEdit();
    hbLineEdit->setMaxLength(maxPasswordLength);
    hbLineEdit->setInputMethodHints(Qt::ImhDigitsOnly);
    
    m_passwordValidator = &validator;
    
    connect(
        hbLineEdit, SIGNAL(contentsChanged()), 
        this, SLOT(passwordTextChanged()));
    
    passwordDialog->open(this, SLOT(finishedPasswordQueryDialog(HbAction*)));
    if(m_passwordDialog) {
        m_passwordDialog->deleteLater();
        m_passwordDialog = NULL;
    }
    m_passwordDialog = passwordDialog.take();
    m_passwordDialog->setParent(this);
    DPRINT << ": OUT";
}

/*!
  PsUiNotes::finishedPasswordQueryDialog.
 */
void PsUiNotes::finishedPasswordQueryDialog(HbAction* action)
{
    bool ok;
    QString password;
    if(m_passwordDialog) {
        if (action == m_passwordDialog->actions().at(1)) {
            ok = false;
        } else {
            ok = true;
            password = m_passwordDialog->value().toString();
        }
        
        disconnect(
            m_passwordDialog->lineEdit(), SIGNAL(contentsChanged()), 
            this, SLOT(passwordTextChanged()));
        
        m_passwordDialog->deleteLater();
        m_passwordDialog = NULL;
        m_passwordValidator = NULL;
        
        emit passwordQueryCompleted(password, ok);
    }
}

/*!
  PsUiNotes::formatPhoneNumber.
  Formats phone number according to locale specific rules.
 */
QString PsUiNotes::formatPhoneNumber(QString number) const
{
    DPRINT << ": IN";
    
    QString formattedNumber = number;
    
    if (m_psuiSettings->numberGroupingSupported() == true) {
         //TODO: utilize HbNumberGrouping API when available
    }
    
    // TODO: digit conversion e.g. into arabic-indic
//    HbExtendedLocale locale = HbExtendedLocale::system();
//    HbStringUtil::convertDigitsTo(formattedNumber, ArabicIndicDigit);
    
    DPRINT << ": OUT";

    return formattedNumber;
}

/*!
  PsUiNotes::launchNextNoteIfReady.
 */
void PsUiNotes::launchNextNoteIfReady()
{
    DPRINT << ": IN";
    
    if (m_notesQueue->isEmpty()) {
        DPRINT << ", QUEUE EMPTY";
        return;
    }
    if (!m_isNoteShowingOngoing) {
        m_isNoteShowingOngoing = true;
        // note is left in the queue so that it can be cancelled at request
        QObject *note = m_notesQueue->head();
        DPRINT << ", note: " << reinterpret_cast<int>(note);
        if (qobject_cast<HbDeviceProgressDialog *>(note)) {
            DPRINT << ", show HbDeviceProgressDialog";
            static_cast<HbDeviceProgressDialog *>(note)->show();
        } else if (qobject_cast<HbDeviceMessageBox *>(note)) {
            DPRINT << ", show HbDeviceMessageBox";    
            static_cast<HbDeviceMessageBox *>(note)->show();
        } else {
            DPRINT << ", UNKNOWN NOTE";
            Q_ASSERT(false);
        }
    } else {
        DPRINT << ", BUSY";
    }
    
    DPRINT << ": OUT";
}

/*!
  PsUiNotes::handleActiveDiverts   
 */
void PsUiNotes::formatActiveDivertsNoteText(
    const QList<PSCallDivertingStatus*> &divertingStatusList,
    QString &text)
{
    text.append(hbTrId("txt_phone_title_active_for") + "\n\n"); 
             
    int entries = divertingStatusList.count(); 
    DPRINT << ": entries " << entries;
    bool dataSet = false;
    bool voiceSet = false;
    for (int i = 0; entries > i; i++) {
       PSCallDivertingStatus *entry = divertingStatusList.at(i);
       
       if (((entry->iServiceGroup & ServiceGroupVoice) && !voiceSet ) ||
           ((entry->iServiceGroup & ServiceGroupData)) && !dataSet) {   
           if ((entry->iServiceGroup & ServiceGroupVoice) && !voiceSet) {
               voiceSet = true;
               text.append(hbTrId("txt_phone_info_voice_calls") + "\n"); 
              
           }
           else if ((entry->iServiceGroup & ServiceGroupData) && !dataSet) {
               dataSet = true;
               text.append(hbTrId("txt_phone_info_video_calls") + "\n"); 
           }
                
           QString phoneNumber = formatPhoneNumber(divertingStatusList.at(i)->iNumber);
           text.append(hbTrId("txt_phone_info_to_numbernl1").arg(phoneNumber) +"\n");
           if (0 < entry->iTimeout) {
               text.append(hbTrId("txt_phone_info_delay_timenln_seconds",
                   entry->iTimeout) + "\n\n");     
           }
           else {
               text.append("\n");
           }
       }
    }
    // Convert plain text to html 
    text = Qt::convertFromPlainText(text);
}

/*!
  PsUiNotes::activeNoteAboutToClose.
 */
void PsUiNotes::activeNoteAboutToClose()
{
    DPRINT << ": IN";


    if (m_isNoteShowingOngoing) {
        m_isNoteShowingOngoing = false;
        QObject* note(NULL);
        if (!m_notesQueue->isEmpty()) {
            note = m_notesQueue->dequeue();
        }
        if(note) {
            launchNextNoteIfReady();
            note->disconnect(this);
            DPRINT << ", delete note: " << reinterpret_cast<int>(note);
            HbDeviceProgressDialog *pNote = 
                qobject_cast<HbDeviceProgressDialog *>(note);
            note->deleteLater();
        }
    }

    DPRINT << ": OUT";
}

/*!
  PsUiNotes::handleProgressNoteCanceled().
 */
void PsUiNotes::handleProgressNoteCanceled()
{
    DPRINT << ": IN";

    emit progressNoteCanceled();

    DPRINT << ": OUT";
}

/*!
  PsUiNotes::passwordTextChanged().
 */
void PsUiNotes::passwordTextChanged()
{
    DPRINT << ": IN";
    Q_ASSERT(m_passwordDialog && m_passwordValidator);

    HbLineEdit *hbLineEdit = m_passwordDialog->lineEdit();
    int position = 0;
    QString password = hbLineEdit->text();
    bool isPasswordValid = 
        (QValidator::Acceptable == m_passwordValidator->validate(
            password, position));
    m_passwordDialog->actions().at(0)->setEnabled(isPasswordValid);

    DPRINT << ": OUT";
}


/*!
  PsUiNotes::showNotificationDialog.
 */
void PsUiNotes::showNotificationDialog(const QString& text)
{
    DPRINT << ": IN";

    HbNotificationDialog *notifDialog = new HbNotificationDialog();
    
    notifDialog->setDismissPolicy(HbPopup::TapAnywhere);
    notifDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    notifDialog->setTitle(text);
    notifDialog->show();

    DPRINT << ": OUT";
}

/*!
  PsUiNotes::showGlobalNotificationDialog   
 */
void PsUiNotes::showGlobalNotificationDialog(const QString& text)
{
    DPRINT << ": IN";
        
    HbDeviceNotificationDialog notifDialog;

    notifDialog.setTitle(text);
    notifDialog.show();

    DPRINT << ": OUT";    
}

// End of File.
