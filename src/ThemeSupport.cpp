/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
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
#include <QFile>
#include <QTextStream>

#include <QWidget>

Nedrysoft::ThemeSupport::ThemeMode Nedrysoft::ThemeSupport::ThemeSupport::m_themeMode =
        Nedrysoft::ThemeSupport::ThemeMode::System;

Nedrysoft::ThemeSupport::ThemeSupport::ThemeSupport() {
    static auto eventProxyWidget = new QWidget;

#if (QT_VERSION_MAJOR<6)
    connect(qobject_cast<QApplication *>(QCoreApplication::instance()), &QApplication::paletteChanged, [=] (const QPalette &) {
        Q_EMIT themeChanged(Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode());
    });
#endif

    eventProxyWidget->installEventFilter(this);

    setMode(m_themeMode);
}

Nedrysoft::ThemeSupport::ThemeSupport::~ThemeSupport() {

}

#if (QT_VERSION_MAJOR>=6)
auto Nedrysoft::ThemeSupport::ThemeSupport::eventFilter(QObject *object, QEvent *event) -> bool  {
    switch(event->type()) {
        case QEvent::ApplicationPaletteChange: {
            Q_EMIT themeChanged(Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode());

            break;
        }

        default: {
            break;
        }
    }

    return QObject::eventFilter(object, event);
}
#endif

auto Nedrysoft::ThemeSupport::ThemeSupport::getColor(const QRgb colourPair[]) -> QColor {
    return QColor(colourPair[isDarkMode() ? 1 : 0]);
}

auto Nedrysoft::ThemeSupport::ThemeSupport::setMode(Nedrysoft::ThemeSupport::ThemeMode mode) -> void {
    switch(mode) {
        case Nedrysoft::ThemeSupport::ThemeMode::System: {
            break;
        }

        case Nedrysoft::ThemeSupport::ThemeMode::Light: {
            break;
        }

        case Nedrysoft::ThemeSupport::ThemeMode::Dark: {
            break;
        }
    }

    m_themeMode = mode;

    Q_EMIT themeChanged(Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode());
}

