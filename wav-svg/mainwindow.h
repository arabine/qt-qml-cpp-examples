/** @file mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotImportWav();
    void slotImportSvg();
    void slotExportWav();
    void slotExportSvg();
private:
    Ui::MainWindow *ui;

    QString mInputWavFileName;
    QString mInputSvgFileName;
};

#endif // MAINWINDOW_H
