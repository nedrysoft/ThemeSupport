/*
 * Copyright (C) 2020 Adrian Carpenter
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

#ifndef NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H
#define NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H

#include <QtGlobal>

#include <QColor>
#include <QObject>
#include <QPalette>
#include <QRgb>

#if ( defined(NEDRYSOFT_LIBRARY_THEMESUPPORT_EXPORT))
#define NEDRYSOFT_THEMESUPPORT_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_THEMESUPPORT_DLLSPEC Q_DECL_IMPORT
#endif

namespace Nedrysoft { namespace ThemeSupport {
    enum class ThemeMode {
        System,
        Light,
        Dark
    };

    enum class ActiveMode {
        Default,,
        SystemLight,
        SystemDark,
        ForcedLight,
        ForcedDark
    };

    /**
     * @brief       The ThemeSupport class provides information about the operating system theme.
     */
    class NEDRYSOFT_THEMESUPPORT_DLLSPEC ThemeSupport :
            public QObject {

        public:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ThemeSupport instance.
             */
            explicit ThemeSupport();

            /**
             * @brief       Destroys the ThemeSupport.
             */
            ~ThemeSupport();

            /**
             * @brief       Sets the application theme mode.
             *
             * @param[in]   mode the mode to set.
             */
            auto setMode(ThemeMode mode) -> void;

            /**
             * @brief       Returns the active theme.
             *
             * @note        The active theme is determined by the OS theme (if available) and the use selected
             *              theme.  This should be used when drawing controls to ensure that whereever possible
             *              controls are drawn natively by the OS.
             *
             *              If the return value is ForcedLight or ForcedDark then the control must be drawn in the
             *              appropriate colours by the application, all other colours the OS should be used to draw
             *              the control.
             *
             * @returns     a value from Nedrysoft::Themesupport::ActiveMode.
             */
            auto activeMode() -> Nedrysoft::ThemeSupport::ActiveMode;

            /**
             * @brief       Returns the mode
             *
             * @returns     true if dark mode; otherwise false.
             */
            static auto isDarkMode(bool *isSystemMode=nullptr) -> bool;

            /**
             * @brief       Returns the current OS theme mode.
             *
             * @returns     true if dark mode; otherwise false.
             */
            static auto isDarkMode() -> bool;

            /**
             * @brief       This signal is emitted when OS theme is changed.
             *
             * @param[in]   isDarkMode true if dark mode; otherwise false.
             */
            Q_SIGNAL void themeChanged(bool isDarkMode);

            /**
             * @brief       Returns the Dark or Light colour for the given array.
             *
             * @note        The array consists of 2 QRgb entries, one entry is for dark and the other for light mode.
             *
             * @returns     the colour.
             */
            static auto getColor(const QRgb PushButtonColor[]) -> QColor;

            /**
             * @brief       Returns the highlighted text background color
             *
             * @note        Qt does not return the correct color when using QPalette, this function directly queries
             *              the macOS to discover the real highlighted background color.
             *
             * @returns     the colour
             */
            static auto getHighlightedBackground() -> QColor;

            /**
             * @brief       Loads a saved palette.
             *
             * @param[in]   filename the filename of the palette to load.
             *
             * @returns     true if loaded; otherwise false.
             */
            auto loadPalette(const QString &filename) -> bool;

            /**
             * @brief       Saves a palette to a file.
             *
             * @param[in]   filename the filename of the saved palette.
             *
             * @returns     true if the palette was saved; otherwise false.
             */
            auto savePalette(QString filename) -> bool;

        protected:
            /**
             * @brief       Returns the map for converting from a color role string to ColorRole.
             *
             * @return      a map that contains the lookup.
             */
            auto roleMap() -> QMap<QString, QPalette::ColorRole>;

            /**
             * @brief       Returns the map for converting from a color role string to ColorGroup.
             *
             * @return      a map that contains the lookup.
             */
            auto groupMap() -> QMap<QString, QPalette::ColorGroup>;

            /**
             * @brief       Returns the current active operating system theme.
             *
             * @param[out]  valid is set to true if the OS supports themes; otherwise false;
             *
             * @returns     the theme mode.
             */
            static auto systemTheme(bool *valid) -> Nedrysoft::ThemeSupport::ThemeMode;

#if (QT_VERSION_MAJOR>=6)
            /**
             * @brief       Reimplements: QObject::event(QEvent *event).
             *
             * @param[in]   event the event information.
             *
             * @returns     true if the event was handled; otherwise false.
             */
             auto eventFilter(QObject *object, QEvent *event) -> bool override;
#endif
        private:
            //! @cond

            static ThemeMode m_themeMode;
            static QWidget *m_eventProxyWidget;

            //! @endcond
    };
}}

#endif // NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H
