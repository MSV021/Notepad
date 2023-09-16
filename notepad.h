#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();
private slots: 
    void newDocument(); 
    void open();
    void save();
    void saveAs();
    void close();
private:
    bool isTextModified(); 
    QMessageBox::StandardButton closePrompt();
    void closeEvent(QCloseEvent*);

    Ui::Notepad *ui;
    QString currentFile;
};
#endif // NOTEPAD_H
