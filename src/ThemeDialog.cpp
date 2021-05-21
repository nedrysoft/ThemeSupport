/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 21/05/2021.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGlobal>

#include "ThemeDialog.h"

#include "ThemeSupport.h"
#if defined(Q_OS_MACOS)
#include "MacHelper"
#endif

Nedrysoft::ThemeSupport::ThemeDialog::ThemeDialog(QWidget *parent) :
        QDialog(parent) {

#if defined(Q_OS_MACOS)
    auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

    updateTitlebar();

    m_themeChangedConnection = connect(
            themeSupport,
            &Nedrysoft::ThemeSupport::ThemeSupport::themeChanged,
            [=](bool isDarkMode) {

        updateTitlebar();
    });
#endif
}

auto Nedrysoft::ThemeSupport::ThemeDialog::updateTitlebar() -> void {
#if defined(Q_OS_MACOS)
    Nedrysoft::MacHelper::MacHelper macHelper;

    auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

    if (themeSupport->isForced()) {
        QColor colour;

        if (themeSupport->isDarkMode()) {
            colour = QColor::fromRgbF(0.23, 0.22, 0.23);
        } else {
            colour = QColor::fromRgbF(0.91, 0.90, 0.91);
        }

        macHelper.setTitlebarColour(
                this,
                colour,
                themeSupport->isDarkMode()
        );
    } else {
        macHelper.clearTitlebarColour(this, themeSupport->isDarkMode());
    }
#endif
}

Nedrysoft::ThemeSupport::ThemeDialog::~ThemeDialog() {
#if defined(Q_OS_MACOS)
    disconnect(m_themeChangedConnection);
#endif
}
