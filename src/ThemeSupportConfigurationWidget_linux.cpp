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

#include "ThemeSupportConfigurationWidget.h"

#include "ThemeSupport.h"

#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QSettings>
#include <QStyleFactory>

auto Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::addPlatformOptions(QFormLayout *layout) -> void {
    QSettings settings;

    m_themeComboBox = new QComboBox;

    auto styles = QStyleFactory::keys();

    for (auto style : styles) {
        m_themeComboBox->addItem(style);
    }

    layout->addRow("Style", m_themeComboBox);

    auto platformStyle = settings.value("ThemeSupport/Style","Fusion").toString();

    auto index = m_themeComboBox->findData(platformStyle);

    if (index>=0) {
        m_themeComboBox->blockSignals(true);
        m_themeComboBox->setCurrentIndex(index);
        m_themeComboBox->blockSignals(false);
    }
}

auto Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::applyPlatformOptions()  -> void {
    qApp->setStyle(m_themeComboBox->currentText());
}