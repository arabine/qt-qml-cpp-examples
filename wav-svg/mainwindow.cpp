/** @file mainwindow.cpp
 *  @author Anthony Rabine
 *  @copyright Copyright (C) 2018 Anthony Rabine
 *
 * This file is part of the 'DrawSound' program (wav-svg)
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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

#include "Converter.h"

#define DRAWSOUND_VERSION "1.0.0"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("DrawSound " DRAWSOUND_VERSION);

    connect(ui->pushButtonImportWav, &QPushButton::clicked, this, &MainWindow::slotImportWav);
    connect(ui->pushButtonImportSvg, &QPushButton::clicked, this, &MainWindow::slotImportSvg);
    connect(ui->pushButtonExportWav, &QPushButton::clicked, this, &MainWindow::slotExportWav);
    connect(ui->pushButtonExportSvg, &QPushButton::clicked, this, &MainWindow::slotExportSvg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotImportWav()
{
    mInputWavFileName = QFileDialog::getOpenFileName(this,
          tr("Open WAV"), "", tr("Image Files (*.wav)"));

    QFileInfo inf(mInputWavFileName);
    ui->labelFileNameWav->setText(inf.fileName());
}

void MainWindow::slotImportSvg()
{
    mInputSvgFileName = QFileDialog::getOpenFileName(this,
          tr("Open SVG"), "", tr("Image Files (*.svg)"));

    QFileInfo inf(mInputSvgFileName);
    ui->labelFileNameSvg->setText(inf.fileName());
}

void MainWindow::slotExportWav()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Export to WAV"),
                                                    "",
                                                    tr("Images (*.wav)"));
    int bitDepth = 16;
    uint32_t sampleRate = 44100;
    if (ui->radioButton44K->isChecked())
    {
        sampleRate = 44100;
    }
    else if(ui->radioButton48K->isChecked())
    {
        sampleRate = 48000;
    }
    else
    {
        sampleRate = 96000;
    }

    if (ui->radioButton8b->isChecked())
    {
        bitDepth = 8;
    }
    else if(ui->radioButton16b->isChecked())
    {
        bitDepth = 16;
    }
    else
    {
        bitDepth = 24;
    }

    SvgToWav(mInputSvgFileName.toStdString(), outputFileName.toStdString(), sampleRate, bitDepth);
}

void MainWindow::slotExportSvg()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Export to SVG"),
                                                     "",
                                                     tr("Images (*.svg)"));

    WavToSvg(mInputWavFileName.toStdString(), outputFileName.toStdString());
}
