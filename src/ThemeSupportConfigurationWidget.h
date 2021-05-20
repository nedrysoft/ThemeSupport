/*
 * Copyright (C) 2021 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 17/05/2021.
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

#ifndef NEDRYSOFT_THEMESUPPORT_THEMESUPPORTCONFIGURATIONWIDGET_H
#define NEDRYSOFT_THEMESUPPORT_THEMESUPPORTCONFIGURATIONWIDGET_H

#include <QtGlobal>

#include "ThemeSupportSpec.h"

#include <QWidget>

class QComboBox;
class QFormLayout;

namespace Nedrysoft { namespace ThemeSupport {
    namespace Ui {
        class ThemeSupportConfigurationWidget;
    }

    /**
     * @brief       Provides a configuration widget that can be used in the host application to display & allow
     *              changing of and theme settings.
     *
     * @class       Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget ThemeSupportConfigurationWidget.h <ThemeSupportConfigurationWidget>
     */
    class NEDRYSOFT_THEMESUPPORT_DLLSPEC ThemeSupportConfigurationWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ThemeSupportConfigurationWidget.
             *
             * @param[in]   parent the parent for this child.
             */
            explicit ThemeSupportConfigurationWidget(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the ThemeSupportConfigurationWidget.
             */
            ~ThemeSupportConfigurationWidget() override;

            /**
             * @brief       Initialises the settings widget.
             *
             * @returns     true if initialised; otherwise false.
             */
            auto initialise() -> bool;

            /**
             * @brief       Returns whether the current settings can be applied.
             *
             * @note        On macOS, there is no concept of whether settings can be applied, as a setting is changed
             *              it is immediately applied.
             *
             * @returns     true if the settings are valid; otherwise false.
             */
            auto canAcceptSettings() -> bool;

            /**
             * @brief       Applies the current settings.
             *
             * @returns     true if the settings were applied (saved); otherwise false.
             */
            auto acceptSettings() -> bool;

            /**
             * @brief       Add platform specific options to the existing form.
             */
            auto addPlatformOptions(QFormLayout *layout) -> void;

            /**
             * @brief       Apply platform scific options.
             */
            auto applyPlatformOptions() -> void;

        private:
            //! @cond

            Ui::ThemeSupportConfigurationWidget *ui;

#if defined(Q_OS_LINUX)
            QComboBox *m_themeComboBox;
#endif

            //! @endcond
    };
}}

#endif // NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_CONFIGURATIONWIDGET_H
