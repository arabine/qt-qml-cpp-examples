/****************************************************************************
 * Copyright (C) 2020 Anthony Rabine
 * Contact: anthony@rabine.fr
 * License: MIT, see LICENSE file
 ****************************************************************************/

#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QThreadPool>
#include <QTimer>

#include "git2.h"
#include "GitRepo.h"


class GitStatusTask : public QRunnable
{
public:
    explicit GitStatusTask(GitRepo *repo)
        : mRepo(repo)
    {

    }

    void run() override
    {
        mRepo->Status();
    }

private:
    GitRepo *mRepo;
};


class RefreshUiTask : public QRunnable
{
public:
    explicit RefreshUiTask(Window *app)
        : mApp(app)
    {

    }

    void run() override
    {
        mApp->RefreshList();
    }

private:
    Window *mApp;
};

Window::Window()
{
    git_libgit2_init();

    QCoreApplication::setOrganizationName("D8S");
    QCoreApplication::setOrganizationDomain("d8s.eu");
    QCoreApplication::setApplicationName("Gitus");

    LoadConfiguration();

    mMainUi.setupUi(this);

   // iconLabel->setMinimumWidth(durationLabel->sizeHint().width());

    createActions();
    createTrayIcon();

//    connect(showMessageButton, &QAbstractButton::clicked, this, &Window::showMessage);
//    connect(showIconCheckBox, &QAbstractButton::toggled, trayIcon, &QSystemTrayIcon::setVisible);
//    connect(iconComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &Window::setIcon);
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Window::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);


    connect(mMainUi.pushButtonAddRepository, &QPushButton::clicked, this, &Window::chooseDirectory);

//    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(iconGroupBox);
//    mainLayout->addWidget(messageGroupBox);
//    setLayout(mainLayout);

//    iconComboBox->setCurrentIndex(1);
    trayIcon->show();

    setIcon(0);
    RefreshStatus();

    setWindowTitle(tr("Gitus"));
    resize(400, 300);

    connect(&mTimer, &QTimer::timeout, this, QOverload<>::of(&Window::RefreshStatus));

    mTimer.setInterval(2000);
    mTimer.start();
}

Window::~Window()
{
     git_libgit2_shutdown();
}

void Window::LoadConfiguration()
{
    int size = mSettings.beginReadArray("repos");
    for (int i = 0; i < size; ++i)
    {
        mSettings.setArrayIndex(i);
        QString dirPath = mSettings.value("path").toString();
        QDir dir(dirPath);
        GitRepo *repo = new GitRepo(std::string(dirPath.toLocal8Bit()), dir.exists());

        repo->SetEnabled(mSettings.value("enabled").toBool());

        mRepoList.push_back(repo);
    }
    mSettings.endArray();
}

void Window::SaveConfiguration()
{
    mSettings.beginWriteArray("repos");
    int i = 0;
    for (const auto rep : mRepoList)
    {
        mSettings.setArrayIndex(i++);
        mSettings.setValue("path", QString(rep->GetPath().data()));
        mSettings.setValue("enabled", rep->IsEnabled());
    }
    mSettings.endArray();
}

void Window::chooseDirectory()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir dir(dirPath);
    if (dir.exists())
    {
        mMutex.lock();
        mRepoList.push_back(new GitRepo(std::string(dirPath.toLocal8Bit()), true));
        SaveConfiguration();
        mMutex.unlock();

        RefreshList();
    }
}

void Window::RefreshList()
{
    mMutex.lock();
    mMainUi.tableWidget->setRowCount(mRepoList.size());

    int row = 0;
    for (auto rep : mRepoList)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString(rep->GetPath().data()));
        mMainUi.tableWidget->setItem(row, 0, newItem);

        QTableWidgetItem *statusItem = new QTableWidgetItem(tr("Satus"));
        statusItem->setTextAlignment(Qt::AlignVCenter);

        if (rep->IsValid())
        {
            statusItem->setIcon(QIcon(QPixmap(":/images/tick.svg")));
        }
        else
        {
            statusItem->setIcon(QIcon(QPixmap(":/images/close.svg")));
        }

        mMainUi.tableWidget->setItem(row, 1, statusItem);

        row++;
    }
    mMutex.unlock();
}

void Window::RefreshStatus()
{
    mTimer.stop();
    mMutex.lock();
    for (auto rep : mRepoList)
    {
        QThreadPool::globalInstance()->start(new GitStatusTask(rep));
        rep->Status();
    }

    QThreadPool::globalInstance()->waitForDone(1500);
    mMutex.unlock();

     QThreadPool::globalInstance()->start(new RefreshUiTask(this));

    mTimer.start();
}


void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Gitus"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}
//! [2]

//! [3]
void Window::setIcon(int index)
{
//    QIcon icon = iconComboBox->itemIcon(index);
    trayIcon->setIcon(QIcon(":/images/version.svg"));
    setWindowIcon(QIcon(":/images/version.svg"));

    trayIcon->setToolTip("Gitus");
}
//! [3]

//! [4]
void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
      //  iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}
//! [4]

//! [5]
void Window::showMessage()
{
//    showIconCheckBox->setChecked(true);
//    int selectedIcon = typeComboBox->itemData(typeComboBox->currentIndex()).toInt();
//    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(selectedIcon);

//    if (selectedIcon == -1) { // custom icon
//        QIcon icon(iconComboBox->itemIcon(iconComboBox->currentIndex()));
//        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
//                          durationSpinBox->value() * 1000);
//    } else {
//        trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), msgIcon,
//                          durationSpinBox->value() * 1000);
//    }
}
//! [5]

//! [6]
void Window::messageClicked()
{
    QMessageBox::information(nullptr, tr("Gitus"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}
//! [6]

//void Window::createIconGroupBox()
//{
//    iconGroupBox = new QGroupBox(tr("Tray Icon"));

//    iconLabel = new QLabel("Icon:");

//    iconComboBox = new QComboBox;
//    iconComboBox->addItem(QIcon(":/images/bad.png"), tr("Bad"));
//    iconComboBox->addItem(QIcon(":/images/heart.png"), tr("Heart"));
//    iconComboBox->addItem(QIcon(":/images/trash.png"), tr("Trash"));

//    showIconCheckBox = new QCheckBox(tr("Show icon"));
//    showIconCheckBox->setChecked(true);

//    QHBoxLayout *iconLayout = new QHBoxLayout;
//    iconLayout->addWidget(iconLabel);
//    iconLayout->addWidget(iconComboBox);
//    iconLayout->addStretch();
//    iconLayout->addWidget(showIconCheckBox);
//    iconGroupBox->setLayout(iconLayout);
//}

//void Window::createMessageGroupBox()
//{
//    messageGroupBox = new QGroupBox(tr("Balloon Message"));

//    typeLabel = new QLabel(tr("Type:"));

//    typeComboBox = new QComboBox;
//    typeComboBox->addItem(tr("None"), QSystemTrayIcon::NoIcon);
//    typeComboBox->addItem(style()->standardIcon(
//            QStyle::SP_MessageBoxInformation), tr("Information"),
//            QSystemTrayIcon::Information);
//    typeComboBox->addItem(style()->standardIcon(
//            QStyle::SP_MessageBoxWarning), tr("Warning"),
//            QSystemTrayIcon::Warning);
//    typeComboBox->addItem(style()->standardIcon(
//            QStyle::SP_MessageBoxCritical), tr("Critical"),
//            QSystemTrayIcon::Critical);
//    typeComboBox->addItem(QIcon(), tr("Custom icon"),
//            -1);
//    typeComboBox->setCurrentIndex(1);

//    durationLabel = new QLabel(tr("Duration:"));

//    durationSpinBox = new QSpinBox;
//    durationSpinBox->setRange(5, 60);
//    durationSpinBox->setSuffix(" s");
//    durationSpinBox->setValue(15);

//    durationWarningLabel = new QLabel(tr("(some systems might ignore this "
//                                         "hint)"));
//    durationWarningLabel->setIndent(10);

//    titleLabel = new QLabel(tr("Title:"));

//    titleEdit = new QLineEdit(tr("Cannot connect to network"));

//    bodyLabel = new QLabel(tr("Body:"));

//    bodyEdit = new QTextEdit;
//    bodyEdit->setPlainText(tr("Don't believe me. Honestly, I don't have a "
//                              "clue.\nClick this balloon for details."));

//    showMessageButton = new QPushButton(tr("Show Message"));
//    showMessageButton->setDefault(true);

//    QGridLayout *messageLayout = new QGridLayout;
//    messageLayout->addWidget(typeLabel, 0, 0);
//    messageLayout->addWidget(typeComboBox, 0, 1, 1, 2);
//    messageLayout->addWidget(durationLabel, 1, 0);
//    messageLayout->addWidget(durationSpinBox, 1, 1);
//    messageLayout->addWidget(durationWarningLabel, 1, 2, 1, 3);
//    messageLayout->addWidget(titleLabel, 2, 0);
//    messageLayout->addWidget(titleEdit, 2, 1, 1, 4);
//    messageLayout->addWidget(bodyLabel, 3, 0);
//    messageLayout->addWidget(bodyEdit, 3, 1, 2, 4);
//    messageLayout->addWidget(showMessageButton, 5, 4);
//    messageLayout->setColumnStretch(3, 1);
//    messageLayout->setRowStretch(4, 1);
//    messageGroupBox->setLayout(messageLayout);
//}

void Window::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

#endif
