#include "notepad.h"
#include "ui_notepad.h"

#include <QFile> 
#include <QFileDialog> 
#include <QTextStream> 
#include <QCloseEvent>

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    connect(ui->actionNew, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionClose, &QAction::triggered, this, &Notepad::close);
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::newDocument() {
    currentFile.clear(); 
    ui->textEdit->setText(QString());
}

void Notepad::open() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file"); 
    if(fileName.isEmpty()) 
        return; 

    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString()); 
        return; 
    } 

    setWindowTitle(fileName); 
    QTextStream in(&file);
    QString text = in.readAll(); 
    ui->textEdit->setText(text); 
    QTextCursor cursor = ui->textEdit->textCursor(); 
    cursor.movePosition(QTextCursor::MoveOperation::End);
    ui->textEdit->setTextCursor(cursor);
    file.close();
}
    
void Notepad::save() {
    QString fileName; 
    if(currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        if(fileName.isEmpty()) 
            return; 
        currentFile = fileName; 
    }
    else {
        fileName = currentFile; 
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can't save file: " + file.errorString());
        return;         
    }

    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText(); 
    out << text; 
    file.close();
}

void Notepad::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save");
    if(fileName.isEmpty()) 
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can't save file: " + file.errorString());
        return;         
    }

    currentFile = fileName; 

    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText(); 
    out << text; 
    file.close();
}

void Notepad::close() {
    if(!isTextModified()) 
        exit(0);

    auto reply = closePrompt();
    if(reply == QMessageBox::Cancel) 
        return;
    else if(reply == QMessageBox::Yes)
        save(); 

    exit(0); 
}

void Notepad::closeEvent(QCloseEvent* event) {
    if(!isTextModified()) {
        event->accept();
        return;
    }

    auto reply = closePrompt(); 
    if(reply == QMessageBox::Cancel) {
        event->ignore(); 
        return; 
    }
    else if(reply == QMessageBox::Yes) 
        save(); 
    
    event->accept();
}

void Notepad::keyPressEvent(QKeyEvent* event) {
    if(event->modifiers() & Qt::ControlModifier) {
        switch(event->key()) {
        case Qt::Key_N: 
            newDocument(); 
            break; 
        case Qt::Key_S: 
            if(event->modifiers() & Qt::ShiftModifier) 
                saveAs(); 
            else 
                save(); 
            break; 
        case Qt::Key_O: 
            open(); 
            break;
        case Qt::Key_W: 
            close(); 
            break;
        }

    }
}

bool Notepad::isTextModified() {
    if(currentFile.isEmpty() && ui->textEdit->toPlainText().isEmpty()) 
        return false;
    else if(!currentFile.isEmpty())  {
        QFile file(currentFile);
        if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString()); 
            return false; 
        } 

        QTextStream in(&file);
        QString text = in.readAll(); 
        if(ui->textEdit->toPlainText() == text) 
            return false;
    }

    return true;
}

QMessageBox::StandardButton Notepad::closePrompt() {
    return QMessageBox::question(this, "Close Document", "Do you want to save your changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
}




