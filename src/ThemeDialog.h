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

#ifndef NEDRYSOFT_THEMESUPPORT_THEMEDIALOG_H
#define NEDRYSOFT_THEMESUPPORT_THEMEDIALOG_H

#include "ThemeSupportSpec.h"

#include <QDialog>
#include <QMetaObject>
#include <QWidget>

namespace Nedrysoft { namespace ThemeSupport {
    /**
     * @brief       ThemeDialog is a QDialog subclass that automatically applies the Light/Dark settings and responds
     *              to changes of the application style or system changes.
     */
    class NEDRYSOFT_THEMESUPPORT_DLLSPEC ThemeDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ThemeDialog that is a child of the parent.
             *
             * @param[in]   parent the parent of this child.
             */
            ThemeDialog(QWidget *parent=nullptr);

            /**
             * @brief       Destroys the ThemeDialog.
             */
            ~ThemeDialog();

        protected:
            /**
             * @brief       Updates the titlebar on macOS when the theme is being forced.
             */
            auto updateTitlebar() -> void;
    };
}};

#endif // NEDRYSOFT_THEMESUPPORT_THEMEDIALOG_H

