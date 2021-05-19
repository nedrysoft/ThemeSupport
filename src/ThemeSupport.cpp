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
#include <QRegularExpression>
#include <QSettings>
#include <QStyle>
#include <QTextStream>
#include <QWidget>

#if defined(Q_OS_MACOS)
constexpr auto HostPlatform = "macos";
#elif defined(Q_OS_WINDOWS)
constexpr auto HostPlatform = "windows";
#elif defined(Q_OS_LINUX)
constexpr auto HostPlatform = "linux";
#else
#error Unknown platform!
#endif

//! @cond

Nedrysoft::ThemeSupport::Theme Nedrysoft::ThemeSupport::ThemeSupport::m_activeTheme =
        Nedrysoft::ThemeSupport::Theme::System;

//! @endcond

Nedrysoft::ThemeSupport::ThemeSupport::ThemeSupport() {
    static auto eventProxyWidget = new QWidget;

#if (QT_VERSION_MAJOR<6)
    connect(qobject_cast<QApplication *>(QCoreApplication::instance()), &QApplication::paletteChanged, [=] (const QPalette &) {
        Q_EMIT themeChanged(Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode());
    });
#endif

    eventProxyWidget->installEventFilter(this);

    selectActive(m_activeTheme);
}

Nedrysoft::ThemeSupport::ThemeSupport::~ThemeSupport() {

}

auto Nedrysoft::ThemeSupport::ThemeSupport::getInstance() -> Nedrysoft::ThemeSupport::ThemeSupport * {
    static Nedrysoft::ThemeSupport::ThemeSupport themeSupport;

    return &themeSupport;
}

#if (QT_VERSION_MAJOR>=6)
auto Nedrysoft::ThemeSupport::ThemeSupport::eventFilter(QObject *object, QEvent *event) -> bool  {
    switch(event->type()) {
        case QEvent::ApplicationPaletteChange: {
            setMode(setMode);

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
#include <QDebug>
auto Nedrysoft::ThemeSupport::ThemeSupport::selectActive(Nedrysoft::ThemeSupport::Theme theme) -> void {
    auto activeMode = systemMode();
    bool clearTheme = false;
    bool forceTheme = false;

    if (theme==Nedrysoft::ThemeSupport::Theme::System) {
        clearTheme = true;
    } else {
        switch(activeMode) {
            case Nedrysoft::ThemeSupport::SystemMode::Unsupported: {
                if (theme==Nedrysoft::ThemeSupport::Theme::Dark) {
                    forceTheme = true;
                } else {
                    clearTheme = true;
                }

                break;
            }

            case Nedrysoft::ThemeSupport::SystemMode::Light: {
                if (theme==Nedrysoft::ThemeSupport::Theme::Light) {
                    clearTheme = true;
                } else {
                    forceTheme = true;
                }

                break;
            }

            case Nedrysoft::ThemeSupport::SystemMode::Dark: {
                if (theme==Nedrysoft::ThemeSupport::Theme::Dark) {
                    clearTheme = true;
                } else {
                    forceTheme = true;
                }

                break;
            }
        }
    }

    if (clearTheme) {
        qApp->blockSignals(true);
        qApp->setPalette(qApp->style()->standardPalette());
        qApp->setStyleSheet("");
        qApp->blockSignals(false);
    }

    if (forceTheme) {
        if (theme==Nedrysoft::ThemeSupport::Theme::Light) {
            loadPalette("light");
        } else {
            loadPalette("dark");
        }
    }

    m_activeTheme = theme;

    Q_EMIT themeChanged(Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode());
}

auto Nedrysoft::ThemeSupport::ThemeSupport::loadPalette(const QString &name) -> bool {
    QString paletteFilename(QString(":/Nedrysoft/ThemeSupport/themes/%1_%2.palette").arg(HostPlatform, name));
    QString stylesheetFilename(QString(":/Nedrysoft/ThemeSupport/themes/%1_%2.qss").arg(HostPlatform, name));
    QSettings *paletteSettings = nullptr;
    QString stylesheet;

    if (QFile::exists(paletteFilename)) {
        paletteSettings = new QSettings(paletteFilename, QSettings::IniFormat);
    }

    if (QFile::exists(stylesheetFilename)) {
         QFile stylesheetFile(stylesheetFilename);

        if (stylesheetFile.open(QFile::ReadOnly)) {
            stylesheet = QString::fromUtf8(stylesheetFile.readAll());
        }
     }

    auto fieldMatch = QRegularExpression(
            "\\/\\*\\s*(.*)\\s*\\*\\/",
            QRegularExpression::CaseInsensitiveOption);

    int matchPosition = 0;

    /*while ((matchPosition = stylesheet.indexOf(fieldMatch, matchPosition)) != -1) {
        // check if in map.
    }*/

    QPalette palette = qGuiApp->palette();

    if (paletteSettings) {
        auto groups = groupMap();
        auto roles = roleMap();

        for (auto group : paletteSettings->childGroups()) {
            paletteSettings->beginGroup(group);

            for (auto key : paletteSettings->allKeys()) {
                auto colourString = paletteSettings->value(key).toString();

                palette.setColor(groups[group], roles[key], colourString);

                if (!stylesheet.isEmpty()) {
                    auto fullRegex = QRegularExpression(
                            QString("\\/\\*\\s*%1\\s*\\*\\/").arg(key + "." + group));

                    auto keyRegex = QRegularExpression(
                            QString("\\/\\*\\s*%1\\s*\\*\\/").arg(key));

                    stylesheet = stylesheet.replace(fullRegex, colourString);
                    stylesheet = stylesheet.replace(keyRegex, colourString);
                }
            }

            paletteSettings->endGroup();
        }

        delete paletteSettings;
    }

    qApp->blockSignals(true);
    qApp->setPalette(palette);
    qApp->setStyleSheet(stylesheet);
    qApp->blockSignals(false);

    return true;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::savePalette(QString filename) -> bool {
    QSettings palette(filename, QSettings::IniFormat);

    QMap<QString, QPalette::ColorGroup> groups = groupMap();
    QMap<QString, QPalette::ColorRole> roles = roleMap();

    QMapIterator<QString, QPalette::ColorGroup> groupIterator(groups);

    while(groupIterator.hasNext()) {
        auto group = groupIterator.next();

        QMapIterator<QString, QPalette::ColorRole> roleIterator(roles);

        while(roleIterator.hasNext()) {
            auto role = roleIterator.next();

            QColor colour = qGuiApp->palette().color(group.value(), role.value());

            QString colourString = QString("#%1%2%3%4")
                    .arg(colour.alpha(), 2, 16, QLatin1Char('0'))
                    .arg(colour.red(), 2, 16, QLatin1Char('0'))
                    .arg(colour.green(), 2, 16, QLatin1Char('0'))
                    .arg(colour.blue(), 2, 16, QLatin1Char('0'));

            palette.setValue(QString("%1/%2").arg(group.key()).arg(role.key()), colourString);
        }
    }

    return true;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::groupMap() -> QMap<QString, QPalette::ColorGroup> {
    QMap<QString, QPalette::ColorGroup> groups;

    groups["Disabled"] = QPalette::Disabled;
    groups["Active"] = QPalette::Active;
    groups["Inactive"] = QPalette::Inactive;
    groups["Normal"] = QPalette::Normal;

    return groups;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::roleMap() -> QMap<QString, QPalette::ColorRole> {
    QMap<QString, QPalette::ColorRole> roles;

    roles["Window"] = QPalette::Window;
    roles["WindowText"] = QPalette::WindowText;
    roles["Base"] = QPalette::Base;
    roles["AlternateBase"] = QPalette::AlternateBase;
    roles["ToolTipBase"] = QPalette::ToolTipBase;
    roles["ToolTipText"] = QPalette::ToolTipText;
    roles["PlaceholderText"] = QPalette::PlaceholderText;
    roles["Text"] = QPalette::Text;
    roles["Button"] = QPalette::Button;
    roles["ButtonText"] = QPalette::ButtonText;
    roles["BrightText"] = QPalette::BrightText;

    roles["Light"] = QPalette::Light;
    roles["Midlight"] = QPalette::Midlight;
    roles["Dark"] = QPalette::Dark;
    roles["Mid"] = QPalette::Mid;
    roles["Shadow"] = QPalette::Shadow;
    roles["Highlight"] = QPalette::Highlight;
    roles["HighlightedText"] = QPalette::HighlightedText;

    roles["Link"] = QPalette::WindowText;
    roles["LinkVisited"] = QPalette::WindowText;

    return roles;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::isForced() -> bool {
    if (systemMode()==Nedrysoft::ThemeSupport::SystemMode::Unsupported) {
        if (Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode()) {
            return true;
        }
    } else {
        if (systemMode() == Nedrysoft::ThemeSupport::SystemMode::Dark) {
            if (!Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode()) {
                return true;
            }
        } else {
            if (Nedrysoft::ThemeSupport::ThemeSupport::isDarkMode()) {
                return true;
            }
        }
    }

    return false;
}

auto Nedrysoft::ThemeSupport::ThemeSupport::initialise() -> bool {
    QSettings settings;
    auto activeTheme = Nedrysoft::ThemeSupport::Theme::System;

    auto platformTheme = settings.value(
            "ThemeSupport/Theme",
            "System").toString();

    if (platformTheme=="Dark") {
        activeTheme = Nedrysoft::ThemeSupport::Theme::Dark;
    } else if (platformTheme=="Light") {
        activeTheme = Nedrysoft::ThemeSupport::Theme::Light;
    }

    selectActive(activeTheme);

    return initialisePlatform();
}
