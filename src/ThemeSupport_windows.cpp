/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
 *
 * Created by Adrian Carpenter on 31/03/2021.
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

#include "ThemeSupport.h"

#include <QApplication>
#include <QStyle>

auto Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode() -> bool {
    if (m_activeTheme==Nedrysoft::ThemeSupport::Theme::System) {
        auto textLightness = QApplication::palette().text().color().lightnessF();
        auto backgroundLightness = QApplication::palette().window().color().lightnessF();

        if (textLightness > backgroundLightness) {
            return true;
        }

        return false;
    }

    return m_activeTheme==Nedrysoft::ThemeSupport::Theme::Dark;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::getHighlightedBackground() -> QColor {
    return qobject_cast<QApplication *>(QCoreApplication::instance())->style()->standardPalette().color(QPalette::Highlight);
}

auto Nedrysoft::ThemeSupport::ThemeSupport::systemMode() -> Nedrysoft::ThemeSupport::SystemMode {
    return Nedrysoft::ThemeSupport::SystemMode::Unsupported;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::initialisePlatform(bool beforeApplicationInstantiated) -> bool {
    return true;
}
