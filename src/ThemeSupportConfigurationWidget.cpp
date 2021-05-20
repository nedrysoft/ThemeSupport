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
#include "ui_ThemeSupportConfigurationWidget.h"

#include <QSettings>

Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::ThemeSupportConfigurationWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Nedrysoft::ThemeSupport::Ui::ThemeSupportConfigurationWidget) {

    QSettings settings;

    ui->setupUi(this);

    auto defaultValue =
        QVariant::fromValue<Nedrysoft::ThemeSupport::Theme>(Nedrysoft::ThemeSupport::Theme::System);

    ui->themeComboBox->addItem("System", QVariant::fromValue(Nedrysoft::ThemeSupport::Theme::System));
    ui->themeComboBox->addItem("Dark", QVariant::fromValue(Nedrysoft::ThemeSupport::Theme::Dark));
    ui->themeComboBox->addItem("Light", QVariant::fromValue(Nedrysoft::ThemeSupport::Theme::Light));

    connect(ui->themeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int index) {
#if defined(W_OS_MACOS)
        auto theme = ui->themeComboBox->itemData(index, Qt::UserRole).value<Nedrysoft::ThemeSupport::Theme>();

        auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

        themeSupport->selectActive(theme);
#endif
    });

    auto platformTheme = settings.value("ThemeSupport/Theme","System").toString();

    ui->themeComboBox->blockSignals(true);
    ui->themeComboBox->setCurrentText(platformTheme);
    ui->themeComboBox->blockSignals(false);

    auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

    addPlatformOptions(ui->formLayout);
}

Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::~ThemeSupportConfigurationWidget() {
    delete ui;
}

auto Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::acceptSettings() -> bool {
    QSettings settings;

    settings.setValue("ThemeSupport/Theme", ui->themeComboBox->currentText());

    applyPlatformOptions();

    auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

    themeSupport->selectActive(ui->themeComboBox->currentData().value<Nedrysoft::ThemeSupport::Theme>());

    return true;
}

auto Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::canAcceptSettings() -> bool {

    return true;
}

auto Nedrysoft::ThemeSupport::ThemeSupportConfigurationWidget::initialise() -> bool {
    return true;
}