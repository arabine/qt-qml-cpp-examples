/****************************************************************************
 * Copyright (C) 2020 Anthony Rabine
 * Contact: anthony@rabine.fr
 * License: MIT, see LICENSE file
 ****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>
#include <QThreadPool>
#include <QMutex>
#include <QTimer>
#include <QSettings>

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

#include "GitRepo.h"

#include "ui_main.h"

class Window : public QDialog
{
    Q_OBJECT

public:
    Window();
    ~Window();

    void RefreshList();
    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();
    void chooseDirectory();

    void RefreshStatus();
private:
//    void createIconGroupBox();
//    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

    Ui::MainDialog mMainUi;

//    QGroupBox *iconGroupBox;
//    QLabel *iconLabel;
//    QComboBox *iconComboBox;
//    QCheckBox *showIconCheckBox;

//    QGroupBox *messageGroupBox;
//    QLabel *typeLabel;
//    QLabel *durationLabel;
//    QLabel *durationWarningLabel;
//    QLabel *titleLabel;
//    QLabel *bodyLabel;
//    QComboBox *typeComboBox;
//    QSpinBox *durationSpinBox;
//    QLineEdit *titleEdit;
//    QTextEdit *bodyEdit;
//    QPushButton *showMessageButton;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMutex mMutex;
    QTimer mTimer;
    QSettings mSettings;

    QVector<GitRepo *> mRepoList;

    void LoadConfiguration();
    void SaveConfiguration();
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
