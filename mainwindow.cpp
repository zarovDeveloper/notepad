#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //изменения

    this->setCentralWidget(ui->textEdit); //замена центрального виджета
    setWindowTitle(fileName); //меняем название документа

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_createAction_triggered() //кнопка "Создать"
{
    QString oldPath = this->windowTitle(); //узнали название документа

    if (count(oldPath.begin(), oldPath.end(), "*"))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Блокнот");
        msgBox.setText("Вы хотите сохранить изменения в файле \n" + oldPath.remove("*") + "?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int res = msgBox.exec();
        oldPath.append("*");
        switch (res)
        {//какую кнопку нажал юзер
            case QMessageBox::Save:
            {// Нажата кнопка Save
                if (fileExists(oldPath.remove("*"))) //если файл существует в системе
                    on_saveAction_triggered(); //сохраняем файл в системе
                else //если не существует
                    on_saveAsAction_triggered(); //сохраняем файл
                on_createAction_triggered(); //вызываем создание еще раз
                break;
            }
            case QMessageBox::Discard:
            {// Нажата кнопка Discard
                this->setWindowTitle(fileName.remove("*")); //убрать "*"
                on_createAction_triggered(); //вызываем создание еще раз
                break;
            }
            case QMessageBox::Cancel:
            {// Нажата кнопка Cancel
                break;
            }
        }
    }
    else //если файл сохранен или он не изменялся
    {
        fileName = "Без имени - Блокнот"; //изменили fileName
        this->setWindowTitle(fileName); //меняем название документа
        ui->textEdit->clear(); //делаем чистый документ
    }
}

void MainWindow::on_openAction_triggered() //кнопка "Открыть"
{
    QString oldPath = this->windowTitle(); //узнали название документа

    if (count(oldPath.begin(), oldPath.end(), "*")) //условие на проверку изменения документа
    {//если изменяли документ, то вызываем окно сообщением
        QMessageBox msgBox;
        msgBox.setWindowTitle("Блокнот");
        msgBox.setText("Вы хотите сохранить изменения в файле \n" + oldPath.remove("*") + "?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int res = msgBox.exec();
        oldPath.append("*");
        switch (res)
        {//какую кнопку нажал юзер
            case QMessageBox::Save:
            {// Нажата кнопка Save
                if (fileExists(oldPath.remove("*"))) //если файл существует в системе
                    on_saveAction_triggered(); //сохраняем файл в системе
                else //если не существует
                    on_saveAsAction_triggered(); //сохраняем файл

                on_openAction_triggered(); //вызываем открытие еще раз
                break;
            }
            case QMessageBox::Discard:
            {// Нажата кнопка Discard
                fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("Текстовые файлы (*.txt)")); //открываем файл

                if (fileName != "") //проверка на название файла
                {
                    QFile file(fileName); //читаем название

                    if (!file.open(QIODevice::ReadOnly)) //если ошибка в файле
                    {
                        QMessageBox::warning(this, tr("Ошибка"), tr("Не могу открыть файл") + file.errorString());
                        return;
                    }
                    QString fileTitle = changedTitle(fileName);
                    QTextStream in(&file); //открытие файла
                    in.setCodec("UTF-8"); //смена кодека на ЮТФ-8 для чтения русских файлов
                    text = in.readAll();
                    ui->textEdit->setText(text); //чтение файла
                    file.close(); //закрыли файла
                    setWindowTitle(fileTitle); //смена названия файла
                }
                else
                    fileName = oldPath;
                break;
            }
            case QMessageBox::Cancel:
            {// Нажата кнопка Cancel
                break;
            }
        }
    }
    else
    {//если изменений не было, или сохранили файл
        fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("Текстовые файлы (*.txt)")); //открываем файл

        if (fileName != "") //проверка на название файла
        {
            QFile file(fileName); //читаем название

            if (!file.open(QIODevice::ReadOnly)) //если ошибка в файле
            {
                QMessageBox::warning(this, tr("Ошибка"), tr("Не могу открыть файл") + file.errorString());
                return;
            }
            QString fileTitle = changedTitle(fileName);
            QTextStream in(&file); //открытие файла
            in.setCodec("UTF-8"); //смена кодека на ЮТФ-8 для чтения русских файлов
            text = in.readAll();
            ui->textEdit->setText(text); //чтение файла
            file.close(); //закрыли файла
            setWindowTitle(fileTitle); //смена названия файла
        }
        else//если нажали отмена
            fileName = oldPath; //оставляем прошлое название
    }
}

void MainWindow::on_saveAsAction_triggered() //кнопка "Сохранить как"
{
    QString oldPath = this->windowTitle(); //старое название
    fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("Текстовые файлы (*.txt)")); //сохранить файл

    if (fileName != "") //проверка на название файла
    {
        QFile file(fileName); //читаем название

        if (!file.open(QIODevice::WriteOnly)) //если ошибка в файле
        {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не могу сохранить файл") + file.errorString());
            return;
        }
        else
        {//нет ошибок
            QString fileTitle = changedTitle(fileName);
            QTextStream stream(&file); //сохранение текста
            stream.setCodec("UTF-8"); //записываем в кодеке ЮТФ-8
            stream << ui->textEdit->toPlainText(); //записывание текста
            stream.flush(); //чистка буферизованных данных, ожидающих записи на утройстве
            file.close(); //закрыли файл
            setWindowTitle(fileTitle); //поменяли название окна
        }
    }
    else //если нажали отмена
        fileName = oldPath; //присвоили старое значение названию документа
}

void MainWindow::on_saveAction_triggered() //кнопка "Сохранить"
{
    QString path = this->windowTitle();

    if (fileExists(fileName))
    {//если документ есть в системе
        QFile file(fileName); //читаем название

        if (!file.open(QIODevice::WriteOnly)) //если ошибка в файле
        {
            QMessageBox::warning(this, tr("Ошибка"), tr("Не могу сохранить файл") + file.errorString());
            return;
        }
        else
        {//нет ошибок
            QString fileTitle = changedTitle(path); //измели название окна
            QTextStream stream(&file); //сохранение текста
            stream.setCodec("UTF-8"); //записываем в кодеке ЮТФ-8
            stream << ui->textEdit->toPlainText(); //записывание текста
            stream.flush(); //чистка буферизованных данных, ожидающих записи на утройстве
            file.close(); //закрыли файл
            setWindowTitle(fileTitle); //изменили название окна
        }
    }
    else //если файла нет в системе
        on_saveAsAction_triggered(); //вызываем фукнцию сохранить как
}

void MainWindow::on_exitAction_triggered() //кнопка "Выход"
{
    QString path = windowTitle(); //узнали старое название

    if (!count(path.begin(), path.end(), "*"))
        this->close();
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Блокнот");
        msgBox.setText("Вы хотите сохранить изменения в файле \n" + fileName + "?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int res = msgBox.exec();
        switch (res)
        {
            case QMessageBox::Save:
            {// Нажата кнопка Save
                if (fileExists(fileName)) //проверка на существование документа в системе
                    on_saveAction_triggered(); //сохраинть документ
                else
                    on_saveAsAction_triggered(); // сохранить как доумент

                this->close(); //закрыли документ
                break;
            }
            case QMessageBox::Discard:
            {// Нажата кнопка Discard
                QApplication::quit(); //закрыть документ
                break;
            }
            case QMessageBox::Cancel:
            {// Нажата кнопка Cancel
                break;
            }
        }
    }
}

void MainWindow::on_textEdit_textChanged() //при изменении текста появление звездочки
{
    QString fileTitle = changedTitle(fileName);

    if (!fileExists(fileName))
    {//если документа не существует в системе
        if (ui->textEdit->toPlainText().length() == 0)
            this->setWindowTitle(fileTitle.remove("*")); //если документ пустой, то убрать "*"
        else
            this->setWindowTitle("*" + fileTitle); //добавление к названию "*" при его отсутсвие
    }
    else
    {
        if(ui->textEdit->toPlainText().toUtf8() == text)
            this->setWindowTitle(fileTitle.remove("*")); //если документ пустой, то убрать "*"
        else
            this->setWindowTitle("*" + fileTitle); //добавление к названию "*" при его отсутсвие
    }
}

void MainWindow::closeEvent(QCloseEvent *event) //функция выхода из документа
{
    QString path = windowTitle(); //узнали старое название

    if (!count(path.begin(), path.end(), "*"))
        event->accept();
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Блокнот");
        msgBox.setText("Вы хотите сохранить изменения в файле \n" + fileName + "?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int res = msgBox.exec();
        switch (res)
        {
            case QMessageBox::Save:
            {// Нажата кнопка Save
                if (fileExists(fileName)) //проверка на существование документа в системе
                    on_saveAction_triggered(); //сохраинть документ
                else
                    on_saveAsAction_triggered(); // сохранить как доумент

                event->accept(); //закрыли документ
                break;
            }
            case QMessageBox::Discard:
            {// Нажата кнопка Discard
                event->accept(); //закрыли документ
                break;
            }
            case QMessageBox::Cancel:
            {// Нажата кнопка Cancel
                event->ignore(); //игнорируем
                break;
            }
        }
    }
}


//каталог "Изменить"


void MainWindow::on_undoTextAction_triggered() //кнопка "Отменить"
{
    ui->textEdit->undo();
}


void MainWindow::on_cutTextAction_triggered() //кнопка "Вырезать"
{
    ui->textEdit->cut();
}


void MainWindow::on_copyTextAction_triggered() //кнопка "Копировать"
{
    ui->textEdit->copy();
}

void MainWindow::on_pastTextAction_triggered() //кнопка "Вставить"
{
    ui->textEdit->paste();
}

void MainWindow::on_deleteTextAction_triggered() //кнопка "Удалить"
{
    ui->textEdit->clear();
}

void MainWindow::on_allTextAction_triggered() //кнопка "Выбрать все"
{
    ui->textEdit->selectAll();
}

void MainWindow::on_dateTimeAction_triggered() //кнопка "Дата и время"
{
    ui->textEdit->append(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
}
