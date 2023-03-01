#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <iostream>
#include <string>
#include <QCloseEvent>
#include <QPrinter>
#include <QPrintDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //меню "Файл"

    void on_openAction_triggered(); //кнопка "Открыть"

    void on_textEdit_textChanged(); //изменение текста

    void on_saveAsAction_triggered(); //кнопка "Сохранить как"

    void on_saveAction_triggered(); //кнопка "Сохранить"

    void on_createAction_triggered(); //кнопка "Создать"

    //меню "Изменить"

    void on_exitAction_triggered(); //кнопка "Выход"

    void on_undoTextAction_triggered(); //кнопка "Отмена"

    void on_cutTextAction_triggered(); //кнопка "Вырезать"

    void on_copyTextAction_triggered(); //кнопка "Копировать"

    void on_pastTextAction_triggered(); //кнопка "Вставить"

    void on_deleteTextAction_triggered(); //кнопка "Удалить"

    void on_allTextAction_triggered(); //кнопка "Выбрать все"

    void on_dateTimeAction_triggered(); //кнопка "Дата и время"

    //дополнительные слоты

    void closeEvent(QCloseEvent *event); //слот выхода из документа

    //оформление

    void on_LightThemeAction_triggered(); //светлое оформление

    void on_DarkThemeAction_triggered(); //темное оформление


private:
    Ui::MainWindow *ui;

    //глобальные переменные

    QString text = ""; //переменная для считывания текста документа
    QString fileName = "Без имени - Блокнот"; //переменная для считывания названия документа

    //функция существования документа в системе

    bool fileExists(QString path)
    {//если существует, возвращает true
        QFileInfo check_file(path);
        return check_file.exists() && check_file.isFile();
    }

    QString changedTitle(QString path) //обработка пути до названия
    {
        int lastIndex = path.lastIndexOf("/"); //определили послединй индекс /
        QString fileTitle = path; //обрезали пути до названия документа
        fileTitle = fileTitle.remove(0, lastIndex+1); //изменили название

        return fileTitle;
    }
};

#endif // MAINWINDOW_H
