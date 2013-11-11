#include <QtTest/QtTest>
#include "vmbxqtuihandler.h"

VmbxQtUiHandler::VmbxQtUiHandler(QObject* parent): QObject(parent)
{
    qDebug("mock VmbxQtUiHandler");
}

VmbxQtUiHandler::~VmbxQtUiHandler()
{
    qDebug("mock ~VmbxQtUiHandler");
}


void VmbxQtUiHandler::showVmbxQueryDialog(const TVmbxType& /*aType*/,
                          QString& /*aNumber*/, int& aResult)
{
    qDebug("mock showVmbxQueryDialog");
    aResult = KErrNone;
}


void VmbxQtUiHandler::showDefineSelectionDialog(
    TVmbxType& /*aType*/, int& aResult)
{
    qDebug("mock showDefineSelectionDialog");
    // Make sure no leave in CVoiceMailboxImpl::QueryVmbxTypeL(
    aResult = KErrNotFound;
}

void VmbxQtUiHandler::showInformationNote(const TVmbxNoteType /*aType*/)
{
    qDebug("mock showInformationNote");    
}

void VmbxQtUiHandler::showCallSelectionDialog(
            const QList<CVoiceMailboxEntry *> /*entryList*/,
            TVoiceMailboxParams &/*params*/, int &/*result*/ )
{
    qDebug("mock showCallSelectionDialogL");  
}

void VmbxQtUiHandler::updatePrimaryAction(const QString &/*aInput*/)
{
	qDebug("mock updatePrimaryAction");  
}

bool VmbxQtUiHandler::isQtEnvSupported()
{
	qDebug("mock isQtEnvSupported");
	return true;
}

void  VmbxQtUiHandler::init()
{
	qDebug("mock init");
}

//End of file
