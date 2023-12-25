/********************************************************************************
** Form generated from reading UI file 'bibo.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIBO_H
#define UI_BIBO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLaden;
    QAction *actionSpeichern;
    QAction *actionNeu;
    QAction *actionListe;
    QAction *action_ndern;
    QAction *actionL_schen;
    QAction *actionNeu_2;
    QAction *actionListe_2;
    QAction *action_ndern_2;
    QAction *actionL_schen_2;
    QAction *actionListe_3;
    QAction *actionAusleihen;
    QAction *actionZur_ckgeben;
    QAction *actionBenutzer;
    QAction *actionMedien;
    QAction *actionAusleihen_2;
    QAction *actionR_ckgaben;
    QWidget *centralwidget;
    QLabel *Lbl_VName;
    QLabel *Lbl_Name_2;
    QLabel *Lbl_UID;
    QLineEdit *In_VName;
    QLineEdit *In_Name;
    QLineEdit *In_Uid;
    QPushButton *Bt_CreateUser;
    QPushButton *Bt_SearchUser;
    QPushButton *Bt_EraseUser;
    QTextBrowser *Display_User;
    QLabel *Lbl_Title;
    QLabel *Lbl_Autor;
    QLabel *Lbl_Verlag;
    QLabel *Lbl_MID;
    QLineEdit *In_Titel;
    QLineEdit *In_Autor;
    QLineEdit *In_Verlag;
    QLineEdit *In_MID;
    QPushButton *Bt_CreateM;
    QPushButton *Bt_SearchM;
    QPushButton *Bt_EraseM;
    QTextBrowser *Display_M;
    QPushButton *Bt_ChangeUser;
    QPushButton *Bt_ChangeM;
    QComboBox *Box_MArt;
    QTextBrowser *Display_L;
    QTextBrowser *Lends_U;
    QTextBrowser *Lends_M;
    QPushButton *Bt_Lend;
    QPushButton *Bt_Restore;
    QLabel *Lbl_Lend;
    QPushButton *Bt_ListLends;
    QPushButton *Bt_DueLends;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;
    QToolBar *toolBar_3;
    QMenuBar *menubar;
    QMenu *menuLaden_Speichern;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1800, 900);
        actionLaden = new QAction(MainWindow);
        actionLaden->setObjectName(QString::fromUtf8("actionLaden"));
        actionSpeichern = new QAction(MainWindow);
        actionSpeichern->setObjectName(QString::fromUtf8("actionSpeichern"));
        actionNeu = new QAction(MainWindow);
        actionNeu->setObjectName(QString::fromUtf8("actionNeu"));
        actionListe = new QAction(MainWindow);
        actionListe->setObjectName(QString::fromUtf8("actionListe"));
        action_ndern = new QAction(MainWindow);
        action_ndern->setObjectName(QString::fromUtf8("action_ndern"));
        actionL_schen = new QAction(MainWindow);
        actionL_schen->setObjectName(QString::fromUtf8("actionL_schen"));
        actionNeu_2 = new QAction(MainWindow);
        actionNeu_2->setObjectName(QString::fromUtf8("actionNeu_2"));
        actionListe_2 = new QAction(MainWindow);
        actionListe_2->setObjectName(QString::fromUtf8("actionListe_2"));
        action_ndern_2 = new QAction(MainWindow);
        action_ndern_2->setObjectName(QString::fromUtf8("action_ndern_2"));
        actionL_schen_2 = new QAction(MainWindow);
        actionL_schen_2->setObjectName(QString::fromUtf8("actionL_schen_2"));
        actionListe_3 = new QAction(MainWindow);
        actionListe_3->setObjectName(QString::fromUtf8("actionListe_3"));
        actionAusleihen = new QAction(MainWindow);
        actionAusleihen->setObjectName(QString::fromUtf8("actionAusleihen"));
        actionZur_ckgeben = new QAction(MainWindow);
        actionZur_ckgeben->setObjectName(QString::fromUtf8("actionZur_ckgeben"));
        actionBenutzer = new QAction(MainWindow);
        actionBenutzer->setObjectName(QString::fromUtf8("actionBenutzer"));
        actionMedien = new QAction(MainWindow);
        actionMedien->setObjectName(QString::fromUtf8("actionMedien"));
        actionAusleihen_2 = new QAction(MainWindow);
        actionAusleihen_2->setObjectName(QString::fromUtf8("actionAusleihen_2"));
        actionR_ckgaben = new QAction(MainWindow);
        actionR_ckgaben->setObjectName(QString::fromUtf8("actionR_ckgaben"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Lbl_VName = new QLabel(centralwidget);
        Lbl_VName->setObjectName(QString::fromUtf8("Lbl_VName"));
        Lbl_VName->setGeometry(QRect(20, 10, 60, 20));
        Lbl_Name_2 = new QLabel(centralwidget);
        Lbl_Name_2->setObjectName(QString::fromUtf8("Lbl_Name_2"));
        Lbl_Name_2->setGeometry(QRect(20, 50, 60, 20));
        Lbl_UID = new QLabel(centralwidget);
        Lbl_UID->setObjectName(QString::fromUtf8("Lbl_UID"));
        Lbl_UID->setGeometry(QRect(20, 90, 60, 20));
        In_VName = new QLineEdit(centralwidget);
        In_VName->setObjectName(QString::fromUtf8("In_VName"));
        In_VName->setGeometry(QRect(140, 5, 300, 30));
        In_Name = new QLineEdit(centralwidget);
        In_Name->setObjectName(QString::fromUtf8("In_Name"));
        In_Name->setGeometry(QRect(140, 45, 300, 30));
        In_Uid = new QLineEdit(centralwidget);
        In_Uid->setObjectName(QString::fromUtf8("In_Uid"));
        In_Uid->setGeometry(QRect(140, 85, 300, 30));
        Bt_CreateUser = new QPushButton(centralwidget);
        Bt_CreateUser->setObjectName(QString::fromUtf8("Bt_CreateUser"));
        Bt_CreateUser->setGeometry(QRect(10, 170, 100, 30));
        Bt_SearchUser = new QPushButton(centralwidget);
        Bt_SearchUser->setObjectName(QString::fromUtf8("Bt_SearchUser"));
        Bt_SearchUser->setGeometry(QRect(130, 170, 100, 30));
        Bt_EraseUser = new QPushButton(centralwidget);
        Bt_EraseUser->setObjectName(QString::fromUtf8("Bt_EraseUser"));
        Bt_EraseUser->setGeometry(QRect(250, 170, 100, 30));
        Display_User = new QTextBrowser(centralwidget);
        Display_User->setObjectName(QString::fromUtf8("Display_User"));
        Display_User->setGeometry(QRect(5, 210, 480, 300));
        Display_User->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        Lbl_Title = new QLabel(centralwidget);
        Lbl_Title->setObjectName(QString::fromUtf8("Lbl_Title"));
        Lbl_Title->setGeometry(QRect(565, 10, 60, 20));
        Lbl_Autor = new QLabel(centralwidget);
        Lbl_Autor->setObjectName(QString::fromUtf8("Lbl_Autor"));
        Lbl_Autor->setGeometry(QRect(565, 50, 60, 20));
        Lbl_Verlag = new QLabel(centralwidget);
        Lbl_Verlag->setObjectName(QString::fromUtf8("Lbl_Verlag"));
        Lbl_Verlag->setGeometry(QRect(565, 90, 60, 20));
        Lbl_MID = new QLabel(centralwidget);
        Lbl_MID->setObjectName(QString::fromUtf8("Lbl_MID"));
        Lbl_MID->setGeometry(QRect(565, 130, 60, 20));
        In_Titel = new QLineEdit(centralwidget);
        In_Titel->setObjectName(QString::fromUtf8("In_Titel"));
        In_Titel->setGeometry(QRect(665, 5, 300, 30));
        In_Autor = new QLineEdit(centralwidget);
        In_Autor->setObjectName(QString::fromUtf8("In_Autor"));
        In_Autor->setGeometry(QRect(665, 45, 300, 30));
        In_Verlag = new QLineEdit(centralwidget);
        In_Verlag->setObjectName(QString::fromUtf8("In_Verlag"));
        In_Verlag->setGeometry(QRect(665, 85, 300, 30));
        In_MID = new QLineEdit(centralwidget);
        In_MID->setObjectName(QString::fromUtf8("In_MID"));
        In_MID->setGeometry(QRect(665, 125, 170, 30));
        Bt_CreateM = new QPushButton(centralwidget);
        Bt_CreateM->setObjectName(QString::fromUtf8("Bt_CreateM"));
        Bt_CreateM->setGeometry(QRect(555, 170, 100, 30));
        Bt_SearchM = new QPushButton(centralwidget);
        Bt_SearchM->setObjectName(QString::fromUtf8("Bt_SearchM"));
        Bt_SearchM->setGeometry(QRect(675, 170, 100, 30));
        Bt_EraseM = new QPushButton(centralwidget);
        Bt_EraseM->setObjectName(QString::fromUtf8("Bt_EraseM"));
        Bt_EraseM->setGeometry(QRect(795, 170, 100, 30));
        Display_M = new QTextBrowser(centralwidget);
        Display_M->setObjectName(QString::fromUtf8("Display_M"));
        Display_M->setGeometry(QRect(550, 210, 480, 300));
        Bt_ChangeUser = new QPushButton(centralwidget);
        Bt_ChangeUser->setObjectName(QString::fromUtf8("Bt_ChangeUser"));
        Bt_ChangeUser->setGeometry(QRect(370, 170, 100, 30));
        Bt_ChangeM = new QPushButton(centralwidget);
        Bt_ChangeM->setObjectName(QString::fromUtf8("Bt_ChangeM"));
        Bt_ChangeM->setGeometry(QRect(915, 170, 100, 30));
        Box_MArt = new QComboBox(centralwidget);
        Box_MArt->setObjectName(QString::fromUtf8("Box_MArt"));
        Box_MArt->setGeometry(QRect(840, 125, 120, 30));
        Box_MArt->setAcceptDrops(false);
        Display_L = new QTextBrowser(centralwidget);
        Display_L->setObjectName(QString::fromUtf8("Display_L"));
        Display_L->setGeometry(QRect(550, 620, 480, 200));
        Lends_U = new QTextBrowser(centralwidget);
        Lends_U->setObjectName(QString::fromUtf8("Lends_U"));
        Lends_U->setGeometry(QRect(5, 540, 480, 120));
        Lends_M = new QTextBrowser(centralwidget);
        Lends_M->setObjectName(QString::fromUtf8("Lends_M"));
        Lends_M->setGeometry(QRect(5, 700, 480, 120));
        Bt_Lend = new QPushButton(centralwidget);
        Bt_Lend->setObjectName(QString::fromUtf8("Bt_Lend"));
        Bt_Lend->setGeometry(QRect(200, 665, 100, 30));
        Bt_Restore = new QPushButton(centralwidget);
        Bt_Restore->setObjectName(QString::fromUtf8("Bt_Restore"));
        Bt_Restore->setGeometry(QRect(330, 665, 100, 30));
        Lbl_Lend = new QLabel(centralwidget);
        Lbl_Lend->setObjectName(QString::fromUtf8("Lbl_Lend"));
        Lbl_Lend->setGeometry(QRect(560, 560, 120, 20));
        Lbl_Lend->setTextFormat(Qt::AutoText);
        Lbl_Lend->setWordWrap(false);
        Bt_ListLends = new QPushButton(centralwidget);
        Bt_ListLends->setObjectName(QString::fromUtf8("Bt_ListLends"));
        Bt_ListLends->setGeometry(QRect(20, 665, 150, 30));
        Bt_DueLends = new QPushButton(centralwidget);
        Bt_DueLends->setObjectName(QString::fromUtf8("Bt_DueLends"));
        Bt_DueLends->setGeometry(QRect(780, 560, 150, 30));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);
        toolBar_3 = new QToolBar(MainWindow);
        toolBar_3->setObjectName(QString::fromUtf8("toolBar_3"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_3);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1800, 25));
        menuLaden_Speichern = new QMenu(menubar);
        menuLaden_Speichern->setObjectName(QString::fromUtf8("menuLaden_Speichern"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuLaden_Speichern->menuAction());
        menuLaden_Speichern->addSeparator();
        menuLaden_Speichern->addSeparator();
        menuLaden_Speichern->addAction(actionLaden);
        menuLaden_Speichern->addAction(actionSpeichern);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionLaden->setText(QApplication::translate("MainWindow", "Laden", 0, QApplication::UnicodeUTF8));
        actionSpeichern->setText(QApplication::translate("MainWindow", "Speichern", 0, QApplication::UnicodeUTF8));
        actionNeu->setText(QApplication::translate("MainWindow", "Neu", 0, QApplication::UnicodeUTF8));
        actionListe->setText(QApplication::translate("MainWindow", "Liste", 0, QApplication::UnicodeUTF8));
        action_ndern->setText(QApplication::translate("MainWindow", "\303\204ndern", 0, QApplication::UnicodeUTF8));
        actionL_schen->setText(QApplication::translate("MainWindow", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        actionNeu_2->setText(QApplication::translate("MainWindow", "Neu", 0, QApplication::UnicodeUTF8));
        actionListe_2->setText(QApplication::translate("MainWindow", "Liste", 0, QApplication::UnicodeUTF8));
        action_ndern_2->setText(QApplication::translate("MainWindow", "\303\204ndern", 0, QApplication::UnicodeUTF8));
        actionL_schen_2->setText(QApplication::translate("MainWindow", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        actionListe_3->setText(QApplication::translate("MainWindow", "Liste", 0, QApplication::UnicodeUTF8));
        actionAusleihen->setText(QApplication::translate("MainWindow", "Ausleihen", 0, QApplication::UnicodeUTF8));
        actionZur_ckgeben->setText(QApplication::translate("MainWindow", "Zur\303\274ckgeben", 0, QApplication::UnicodeUTF8));
        actionBenutzer->setText(QApplication::translate("MainWindow", "Benutzer", 0, QApplication::UnicodeUTF8));
        actionMedien->setText(QApplication::translate("MainWindow", "Medien", 0, QApplication::UnicodeUTF8));
        actionAusleihen_2->setText(QApplication::translate("MainWindow", "Ausleihen", 0, QApplication::UnicodeUTF8));
        actionR_ckgaben->setText(QApplication::translate("MainWindow", "R\303\274ckgaben", 0, QApplication::UnicodeUTF8));
        Lbl_VName->setText(QApplication::translate("MainWindow", "Vorname", 0, QApplication::UnicodeUTF8));
        Lbl_Name_2->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        Lbl_UID->setText(QApplication::translate("MainWindow", "ID", 0, QApplication::UnicodeUTF8));
        Bt_CreateUser->setText(QApplication::translate("MainWindow", "Anlegen", 0, QApplication::UnicodeUTF8));
        Bt_SearchUser->setText(QApplication::translate("MainWindow", "Suchen", 0, QApplication::UnicodeUTF8));
        Bt_EraseUser->setText(QApplication::translate("MainWindow", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        Lbl_Title->setText(QApplication::translate("MainWindow", "Titel", 0, QApplication::UnicodeUTF8));
        Lbl_Autor->setText(QApplication::translate("MainWindow", "Autor", 0, QApplication::UnicodeUTF8));
        Lbl_Verlag->setText(QApplication::translate("MainWindow", "Verlag", 0, QApplication::UnicodeUTF8));
        Lbl_MID->setText(QApplication::translate("MainWindow", "ID", 0, QApplication::UnicodeUTF8));
        Bt_CreateM->setText(QApplication::translate("MainWindow", "Anlegen", 0, QApplication::UnicodeUTF8));
        Bt_SearchM->setText(QApplication::translate("MainWindow", "Suchen", 0, QApplication::UnicodeUTF8));
        Bt_EraseM->setText(QApplication::translate("MainWindow", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        Bt_ChangeUser->setText(QApplication::translate("MainWindow", "\303\204ndern", 0, QApplication::UnicodeUTF8));
        Bt_ChangeM->setText(QApplication::translate("MainWindow", "\303\204ndern", 0, QApplication::UnicodeUTF8));
        Box_MArt->clear();
        Box_MArt->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Buch", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Zeitschrift", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "CD/DVD", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Alle", 0, QApplication::UnicodeUTF8)
        );
        Bt_Lend->setText(QApplication::translate("MainWindow", "leiht aus", 0, QApplication::UnicodeUTF8));
        Bt_Restore->setText(QApplication::translate("MainWindow", "gibt zur\303\274ck", 0, QApplication::UnicodeUTF8));
        Lbl_Lend->setText(QApplication::translate("MainWindow", "Ausleih\303\274bersicht", 0, QApplication::UnicodeUTF8));
        Bt_ListLends->setText(QApplication::translate("MainWindow", "Ausleihen anzeigen", 0, QApplication::UnicodeUTF8));
        Bt_DueLends->setText(QApplication::translate("MainWindow", "f\303\244llige Ausleihen", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0, QApplication::UnicodeUTF8));
        toolBar_3->setWindowTitle(QApplication::translate("MainWindow", "toolBar_3", 0, QApplication::UnicodeUTF8));
        menuLaden_Speichern->setTitle(QApplication::translate("MainWindow", "Laden/Speichern", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIBO_H
