/* antimicro Gamepad to KB+M event mapper
 * Copyright (C) 2015 Travis Nickles <nickles.travis@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "capturedwindowinfodialog.h"
#include "ui_capturedwindowinfodialog.h"

#include "messagehandler.h"

#include <QPushButton>
#include <QWidget>
#include <QDebug>

#ifdef Q_OS_WIN
#include "winextras.h"
#elif defined(Q_OS_UNIX)
#include "x11extras.h"
#endif



#ifdef Q_OS_WIN
CapturedWindowInfoDialog::CapturedWindowInfoDialog(QWidget *parent) :
#elif defined(Q_OS_UNIX)
CapturedWindowInfoDialog::CapturedWindowInfoDialog(long window, QWidget *parent) :
#endif
    QDialog(parent),
    ui(new Ui::CapturedWindowInfoDialog)
{
    ui->setupUi(this);

    qInstallMessageHandler(MessageHandler::myMessageOutput);
    setAttribute(Qt::WA_DeleteOnClose);

    selectedMatch = WindowNone;

#ifdef Q_OS_UNIX
    X11Extras *info = X11Extras::getInstance();
    ui->winPathChoiceComboBox->setVisible(false);
#endif

    bool setRadioDefault = false;
    fullWinPath = false;

#ifdef Q_OS_WIN
    ui->winClassCheckBox->setVisible(false);
    ui->winClassLabel->setVisible(false);
    ui->winClassHeadLabel->setVisible(false);
#elif defined(Q_OS_UNIX)
    winClass = info->getWindowClass(static_cast<Window>(window));
    ui->winClassLabel->setText(winClass);
    if (winClass.isEmpty())
    {
        ui->winClassCheckBox->setEnabled(false);
        ui->winClassCheckBox->setChecked(false);
    }
    else
    {
        ui->winClassCheckBox->setChecked(true);
        setRadioDefault = true;
    }

    ui->winPathChoiceComboBox->setVisible(false);

#endif

#ifdef Q_OS_WIN
    winName = WinExtras::getCurrentWindowText();
#elif defined(Q_OS_UNIX)
    winName = info->getWindowTitle(static_cast<Window>(window));
#endif

    ui->winTitleLabel->setText(winName);
    if (winName.isEmpty())
    {
        ui->winTitleCheckBox->setEnabled(false);
        ui->winTitleCheckBox->setChecked(false);
    }
    else if (!setRadioDefault)
    {
        ui->winTitleCheckBox->setChecked(true);
        setRadioDefault = true;
    }

    ui->winPathLabel->clear();
#ifdef Q_OS_WIN
    winPath = WinExtras::getForegroundWindowExePath();
    ui->winPathLabel->setText(winPath);

    if (winPath.isEmpty())
    {
        ui->winPathCheckBox->setEnabled(false);
        ui->winPathCheckBox->setChecked(false);
    }
    else
    {
        ui->winPathCheckBox->setChecked(true);
        ui->winTitleCheckBox->setChecked(false);
        setRadioDefault = true;
    }

#elif defined(Q_OS_UNIX)
    int pid = info->getApplicationPid(static_cast<Window>(window));
    if (pid > 0)
    {
        QString exepath = X11Extras::getInstance()->getApplicationLocation(pid);
        if (!exepath.isEmpty())
        {
            ui->winPathLabel->setText(exepath);
            winPath = exepath;
            if (!setRadioDefault)
            {
                ui->winTitleCheckBox->setChecked(true);
                setRadioDefault = true;
            }
        }
        else
        {
            ui->winPathCheckBox->setEnabled(false);
            ui->winPathCheckBox->setChecked(false);
        }
    }
    else
    {
        ui->winPathCheckBox->setEnabled(false);
        ui->winPathCheckBox->setChecked(false);
    }
#endif

    if (winClass.isEmpty() && winName.isEmpty() &&
        winPath.isEmpty())
    {
        QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
        button->setEnabled(false);
    }

    connect(this, &CapturedWindowInfoDialog::accepted, this, &CapturedWindowInfoDialog::populateOption);
}

CapturedWindowInfoDialog::~CapturedWindowInfoDialog()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    delete ui;
}

void CapturedWindowInfoDialog::populateOption()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    if (ui->winClassCheckBox->isChecked())
    {
        selectedMatch = selectedMatch | WindowClass;
    }

    if (ui->winTitleCheckBox->isChecked())
    {
        selectedMatch = selectedMatch | WindowName;
    }

    if (ui->winPathCheckBox->isChecked())
    {
        selectedMatch = selectedMatch | WindowPath;

        if (ui->winPathChoiceComboBox->currentIndex() == 0)
        {
            fullWinPath = true;
        }
        else
        {
            fullWinPath = false;
        }
    }
}

CapturedWindowInfoDialog::CapturedWindowOption CapturedWindowInfoDialog::getSelectedOptions()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return selectedMatch;
}

QString CapturedWindowInfoDialog::getWindowClass()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return winClass;
}

QString CapturedWindowInfoDialog::getWindowName()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return winName;
}

QString CapturedWindowInfoDialog::getWindowPath()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return winPath;
}

bool CapturedWindowInfoDialog::useFullWindowPath()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    return fullWinPath;
}
