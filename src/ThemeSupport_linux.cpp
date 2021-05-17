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
#include <QLibrary>
#include <QSettings>
#include <QString>
#include <QStyle>

auto Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode() -> bool {
    if (m_activeTheme==Nedrysoft::ThemeSupport::Theme::System) {
        auto systemMode = systemMode();

        if (systemMode) {
            return systemMode==Nedrysoft::ThemeSupport::SystemMode::Dark;
        }

        // fallback, we compare the lightness of the text colour to the window background colour

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

auto Nedrysoft::ThemeSupport::ThemeSupport::initialise() -> bool {
    QSettings settings;

    if (settings.value("ThemeSupport/Override", true)) {
        auto platformTheme = settings.value("ThemeSupport/Environment/QT_QPA_PLATFORMTHEME");
        auto styleOverride = settings.value("ThemeSupport/Environment/QT_STYLE_OVERRIDE");

        qputenv("QT_QPA_PLATFORMTHEME", platformTheme);
        qputenv("QT_STYLE_OVERRIDE", styleOverride);
    }

    return true;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::systemMode() -> Nedrysoft::ThemeSupport::SystemMode {
    typedef void *(_gtk_settings_get_default)();
    typedef void *(_g_object_get)(void *, const char *, ...);
    typedef void (_g_free)(void *);

    // try to load libgtk and get settings, if we are running under a gtk then the settings can be read.

    _g_object_get *g_object_get =
            (_g_object_get *) QLibrary::resolve("libgobject-2.0", "g_object_get");

    _gtk_settings_get_default *gtk_settings_get_default =
            (_gtk_settings_get_default *) QLibrary::resolve(
                    "libgtk-x11-2.0",
                    "gtk_settings_get_default"
            );

    _g_free *g_free =
            (_g_free *) QLibrary::resolve("glib-2.0", "g_free");

    if (( g_object_get ) && ( gtk_settings_get_default ) && ( g_free )) {
        void *settings = gtk_settings_get_default();

        if (settings) {
            char *propertyValue = nullptr;

            g_object_get(settings, "gtk-theme-name", &propertyValue, nullptr);

            if (propertyValue) {
                QString resultString = QString::fromUtf8(propertyValue);

                g_free(propertyValue);

                if (*osSupportsThemes) {
                    *osSupportsThemes = true;
                }

                if (resultString.endsWith("-dark", Qt::CaseInsensitive)) {
                    return Nedrysoft::ThemeSupport::SystemMode::Dark;
                }

                return Nedrysoft::ThemeSupport::SystemMode::Light;
            }
        }
    }

    return Nedrysoft::ThemeSupport::SystemMode::Light;
}