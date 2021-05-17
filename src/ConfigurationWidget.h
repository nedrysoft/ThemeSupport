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

#include <QWidget>

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
    class ThemeSupportConfigurationWidget :
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
             * @brief       Applies the current settings.
             *
             * @returns     true if the settings were applied (saved); otherwise false.
             */
            auto applyConfiguration() -> bool;

        private:
            //! @cond

            Ui::ThemeSupportConfigurationWidget *ui;

            //! @endcond
    };
}}

#endif // NEDRYSOFT_THEMESUPPORT_THEMESUPPORT_CONFIGURATIONWIDGET_H
