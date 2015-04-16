/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout;
    QLabel *display_mcuName;
    QLabel *display_frequency;
    QLabel *display_fileName;
    QLabel *display_fuseBits;
    QLineEdit *enterFuseBits;
    QPushButton *selectFile;
    QComboBox *chooseDevice;
    QComboBox *chooseFrequency;
    QLabel *lbl_frequency;
    QLabel *lbl_file;
    QLabel *lbl_fuseBits;
    QLabel *lbl_mcu;
    QHBoxLayout *horizontalLayout_2;
    QLabel *commandText;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_write;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(481, 282);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 0, 461, 221));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(100);
        sizePolicy1.setVerticalStretch(100);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        display_mcuName = new QLabel(frame);
        display_mcuName->setObjectName(QStringLiteral("display_mcuName"));
        display_mcuName->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(display_mcuName->sizePolicy().hasHeightForWidth());
        display_mcuName->setSizePolicy(sizePolicy2);
        display_mcuName->setInputMethodHints(Qt::ImhNone);
        display_mcuName->setFrameShape(QFrame::StyledPanel);
        display_mcuName->setFrameShadow(QFrame::Plain);
        display_mcuName->setMidLineWidth(1);
        display_mcuName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(display_mcuName, 0, 2, 1, 1);

        display_frequency = new QLabel(frame);
        display_frequency->setObjectName(QStringLiteral("display_frequency"));
        sizePolicy2.setHeightForWidth(display_frequency->sizePolicy().hasHeightForWidth());
        display_frequency->setSizePolicy(sizePolicy2);
        display_frequency->setFrameShape(QFrame::StyledPanel);
        display_frequency->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(display_frequency, 1, 2, 1, 1);

        display_fileName = new QLabel(frame);
        display_fileName->setObjectName(QStringLiteral("display_fileName"));
        sizePolicy2.setHeightForWidth(display_fileName->sizePolicy().hasHeightForWidth());
        display_fileName->setSizePolicy(sizePolicy2);
        display_fileName->setFrameShape(QFrame::StyledPanel);
        display_fileName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(display_fileName, 2, 2, 1, 1);

        display_fuseBits = new QLabel(frame);
        display_fuseBits->setObjectName(QStringLiteral("display_fuseBits"));
        sizePolicy2.setHeightForWidth(display_fuseBits->sizePolicy().hasHeightForWidth());
        display_fuseBits->setSizePolicy(sizePolicy2);
        display_fuseBits->setFrameShape(QFrame::StyledPanel);
        display_fuseBits->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(display_fuseBits, 3, 2, 1, 1);

        enterFuseBits = new QLineEdit(frame);
        enterFuseBits->setObjectName(QStringLiteral("enterFuseBits"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(enterFuseBits->sizePolicy().hasHeightForWidth());
        enterFuseBits->setSizePolicy(sizePolicy3);
        enterFuseBits->setMinimumSize(QSize(125, 0));
        enterFuseBits->setMaximumSize(QSize(125, 16777215));
        enterFuseBits->setFocusPolicy(Qt::ClickFocus);
        enterFuseBits->setAlignment(Qt::AlignCenter);
        enterFuseBits->setClearButtonEnabled(false);

        gridLayout->addWidget(enterFuseBits, 3, 1, 1, 1);

        selectFile = new QPushButton(frame);
        selectFile->setObjectName(QStringLiteral("selectFile"));
        sizePolicy3.setHeightForWidth(selectFile->sizePolicy().hasHeightForWidth());
        selectFile->setSizePolicy(sizePolicy3);
        selectFile->setMinimumSize(QSize(125, 0));

        gridLayout->addWidget(selectFile, 2, 1, 1, 1);

        chooseDevice = new QComboBox(frame);
        chooseDevice->setObjectName(QStringLiteral("chooseDevice"));
        sizePolicy3.setHeightForWidth(chooseDevice->sizePolicy().hasHeightForWidth());
        chooseDevice->setSizePolicy(sizePolicy3);
        chooseDevice->setMinimumSize(QSize(125, 0));
        chooseDevice->setFocusPolicy(Qt::StrongFocus);
        chooseDevice->setInsertPolicy(QComboBox::InsertAlphabetically);
        chooseDevice->setFrame(false);

        gridLayout->addWidget(chooseDevice, 0, 1, 1, 1);

        chooseFrequency = new QComboBox(frame);
        chooseFrequency->setObjectName(QStringLiteral("chooseFrequency"));
        sizePolicy3.setHeightForWidth(chooseFrequency->sizePolicy().hasHeightForWidth());
        chooseFrequency->setSizePolicy(sizePolicy3);
        chooseFrequency->setMinimumSize(QSize(125, 0));

        gridLayout->addWidget(chooseFrequency, 1, 1, 1, 1);

        lbl_frequency = new QLabel(frame);
        lbl_frequency->setObjectName(QStringLiteral("lbl_frequency"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lbl_frequency->sizePolicy().hasHeightForWidth());
        lbl_frequency->setSizePolicy(sizePolicy4);
        lbl_frequency->setMinimumSize(QSize(150, 0));
        lbl_frequency->setFrameShape(QFrame::Panel);
        lbl_frequency->setFrameShadow(QFrame::Sunken);
        lbl_frequency->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbl_frequency, 1, 0, 1, 1);

        lbl_file = new QLabel(frame);
        lbl_file->setObjectName(QStringLiteral("lbl_file"));
        sizePolicy4.setHeightForWidth(lbl_file->sizePolicy().hasHeightForWidth());
        lbl_file->setSizePolicy(sizePolicy4);
        lbl_file->setMinimumSize(QSize(150, 0));
        lbl_file->setFrameShape(QFrame::Panel);
        lbl_file->setFrameShadow(QFrame::Sunken);
        lbl_file->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbl_file, 2, 0, 1, 1);

        lbl_fuseBits = new QLabel(frame);
        lbl_fuseBits->setObjectName(QStringLiteral("lbl_fuseBits"));
        sizePolicy4.setHeightForWidth(lbl_fuseBits->sizePolicy().hasHeightForWidth());
        lbl_fuseBits->setSizePolicy(sizePolicy4);
        lbl_fuseBits->setMinimumSize(QSize(150, 0));
        lbl_fuseBits->setMaximumSize(QSize(150, 16777215));
        lbl_fuseBits->setFrameShape(QFrame::Panel);
        lbl_fuseBits->setFrameShadow(QFrame::Sunken);
        lbl_fuseBits->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbl_fuseBits, 3, 0, 1, 1);

        lbl_mcu = new QLabel(frame);
        lbl_mcu->setObjectName(QStringLiteral("lbl_mcu"));
        sizePolicy4.setHeightForWidth(lbl_mcu->sizePolicy().hasHeightForWidth());
        lbl_mcu->setSizePolicy(sizePolicy4);
        lbl_mcu->setMinimumSize(QSize(150, 0));
        lbl_mcu->setFrameShape(QFrame::Panel);
        lbl_mcu->setFrameShadow(QFrame::Sunken);
        lbl_mcu->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbl_mcu, 0, 0, 1, 1);


        horizontalLayout_3->addLayout(gridLayout);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        commandText = new QLabel(frame);
        commandText->setObjectName(QStringLiteral("commandText"));
        sizePolicy2.setHeightForWidth(commandText->sizePolicy().hasHeightForWidth());
        commandText->setSizePolicy(sizePolicy2);
        commandText->setFrameShape(QFrame::NoFrame);
        commandText->setFrameShadow(QFrame::Sunken);
        commandText->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(commandText);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_write = new QPushButton(frame);
        btn_write->setObjectName(QStringLiteral("btn_write"));
        btn_write->setEnabled(false);
        btn_write->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(btn_write);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 481, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(chooseDevice, chooseFrequency);
        QWidget::setTabOrder(chooseFrequency, selectFile);
        QWidget::setTabOrder(selectFile, btn_write);

        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        chooseFrequency->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        display_mcuName->setText(QString());
        display_frequency->setText(QString());
        display_fileName->setText(QString());
        display_fuseBits->setText(QApplication::translate("MainWindow", " --fuses w:c0,d9", 0));
        enterFuseBits->setText(QApplication::translate("MainWindow", "c0,d9", 0));
        selectFile->setText(QApplication::translate("MainWindow", "Select File", 0));
        chooseFrequency->clear();
        chooseFrequency->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "2.0", 0)
         << QApplication::translate("MainWindow", "4.0", 0)
         << QApplication::translate("MainWindow", "6.0", 0)
         << QApplication::translate("MainWindow", "8.0", 0)
         << QApplication::translate("MainWindow", "10.0", 0)
        );
        lbl_frequency->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Frequency ( Mhz )</span></p></body></html>", 0));
        lbl_file->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">File to use</span></p></body></html>", 0));
        lbl_fuseBits->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Fuse and Lock Bits</span></p></body></html>", 0));
        lbl_mcu->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">MCU</span></p></body></html>", 0));
        commandText->setText(QApplication::translate("MainWindow", "Command to be constructed here", 0));
        btn_write->setText(QApplication::translate("MainWindow", "Error", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
