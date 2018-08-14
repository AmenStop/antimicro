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

#include "joycontrolstickeditdialoghelper.h"

#include "messagehandler.h"
#include "joybuttonslot.h"
#include "joybuttontypes/joycontrolstickbutton.h"

#include <QDebug>


JoyControlStickEditDialogHelper::JoyControlStickEditDialogHelper(JoyControlStick *stick, QObject *parent) :
    QObject(parent)
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    Q_ASSERT(stick);

    this->stick = stick;
}

void JoyControlStickEditDialogHelper::setPendingSlots(QHash<JoyControlStick::JoyStickDirections,
                                                      JoyButtonSlot *> *tempSlots)
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    pendingSlots.clear();

    QHashIterator<JoyControlStick::JoyStickDirections, JoyButtonSlot*> iter(*tempSlots);
    while (iter.hasNext())
    {
        iter.next();

        JoyButtonSlot *slot = iter.value();
        JoyControlStick::JoyStickDirections tempDir = iter.key();
        pendingSlots.insert(tempDir, slot);
    }
}

void JoyControlStickEditDialogHelper::clearPendingSlots()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    pendingSlots.clear();
}

void JoyControlStickEditDialogHelper::setFromPendingSlots()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    if (!getPendingSlots().isEmpty())
    {
        QHashIterator<JoyControlStick::JoyStickDirections, JoyButtonSlot*> iter(getPendingSlots());
        while (iter.hasNext())
        {
            iter.next();

            JoyButtonSlot *slot = iter.value();
            if (slot)
            {
                JoyControlStick::JoyStickDirections tempDir = iter.key();
                JoyControlStickButton *button = stick->getDirectionButton(tempDir);
                if (button)
                {
                    button->clearSlotsEventReset(false);
                    button->setAssignedSlot(slot->getSlotCode(), slot->getSlotCodeAlias(),
                                            slot->getSlotMode());
                }

                slot->deleteLater();
            }
        }
    }
}

void JoyControlStickEditDialogHelper::clearButtonsSlotsEventReset()
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    QHash<JoyControlStick::JoyStickDirections, JoyControlStickButton*> *buttons = stick->getButtons();
    QHashIterator<JoyControlStick::JoyStickDirections, JoyControlStickButton*> iter(*buttons);
    while (iter.hasNext())
    {
        JoyControlStickButton *button = iter.next().value();
        if (button)
        {
            button->clearSlotsEventReset();
        }
    }
}

void JoyControlStickEditDialogHelper::updateControlStickDelay(int value)
{
    qInstallMessageHandler(MessageHandler::myMessageOutput);

    int temp = value * 10;
    if (stick->getStickDelay() != temp)
    {
        stick->setStickDelay(temp);
    }
}

QHash<JoyControlStick::JoyStickDirections, JoyButtonSlot*> const& JoyControlStickEditDialogHelper::getPendingSlots() {

    return pendingSlots;
}
