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
    class ThemeSupportConfigurationWidget;

    enum class Theme {
        System,
        Light,
        Dark
    };

    enum class SystemMode {
        Unsupported,
        Light,
        Dark
    };

    /**
     * @brief       The ThemeSupport class provides information about the operating system theme.
     *
     * @class       Nedrysoft::ThemeSupport::ThemeSupport ThemeSupport.h <ThemeSupport>
     */
    class NEDRYSOFT_THEMESUPPORT_DLLSPEC ThemeSupport :
            public QObject {

        public:
            Q_OBJECT

        private:
            /**
             * @brief       Constructs a new ThemeSupport instance.
             */
            explicit ThemeSupport();

        public:
            /**
             * @brief       Destroys the ThemeSupport.
             */
            ~ThemeSupport();

            /**
             * @brief       Sets the active application theme.
             *
             * @param[in]   mode the to set as active.
             */
            auto selectActive(Theme theme) -> void;

            /**
             * @brief       Returns whether the OS is in dark mode.
             *
             * @returns     true if dark mode; otherwise false.
             */
            auto isDarkMode() -> bool;

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
            auto getColor(const QRgb PushButtonColor[]) -> QColor;

            /**
             * @brief       Returns the highlighted text background color
             *
             * @note        Qt does not return the correct color when using QPalette, this function directly queries
             *              the macOS to discover the real highlighted background color.
             *
             * @returns     the colour
             */
            auto getHighlightedBackground() -> QColor;

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

            /**
             * @brief       Initialise the theme.
             *
             * @note        This function should be called as early as possible in the application to initialise
             *              the theming.  This may be an empty function depending on the platform and the specific
             *              mechanisms used to switch between light and dark mode.
             *
             * @returns     true if the platform was initialised ok; otherwise false.
             */
            auto initialise() -> bool;

            /**
             * @brief       Returns a configuration widget that the application can use.
             *
             * @returns     the configuration widget (the widget is owned by the caller)
             */
            auto configurationWidget() -> Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget *;

            /**
             * @brief       Returns the current selected mode if OS supports light/dark mode.
             *
             * @returns     returns the currently active system mode, if the OS does not support light/dark modes then
             *              this function will return Nedrysoft::ThemeSupport::SystemMode::Unsupported.
             */
            auto systemMode() -> Nedrysoft::ThemeSupport::SystemMode;

            /**
             * @brief       Returns whether the currently selected theme is forced.
             *
             * @note        This implements the logic to deal with system mode light/dark themes.
             *
             * @returns     true if the selected theme is forced; otherwise false.
             */
            auto isForced() -> bool;

            /**
             * @brief       Initialises the saved state for the platform specific configuration.
             *
             * @returns     true if the configuration was restored; otherwise false.
             */
            auto initialisePlatform() -> bool;

            /**
             * @brief       Returns the singleton instance to the ThemeSupport object.
             *
             * @returns     the singleton instance
             */
            static auto getInstance() -> Nedrysoft::ThemeSupport::ThemeSupport *;

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

            static Theme m_activeTheme;
            static QWidget *m_eventProxyWidget;

            //! @endcond
    };
}}

Q_DECLARE_METATYPE(Nedrysoft::ThemeSupport::SystemMode)
Q_DECLARE_METATYPE(Nedrysoft::ThemeSupport::Theme)

#endif // NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_H
