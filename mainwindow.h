#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QListView;
class QStringListModel;
class QLineEdit;
class QPushButton;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addItem();
    void removeSelected();
    void clearAll();

private:
    void setupUi();
    void refreshStatus();

    QStringListModel *m_model;
    QListView *m_view;
    QLineEdit *m_input;
    QLabel *m_status;
    QPushButton *m_addBtn;
    QPushButton *m_removeBtn;
    QPushButton *m_clearBtn;
    QVBoxLayout *m_rootLayout;
    QHBoxLayout *m_inputRow;
    QHBoxLayout *m_btnRow;
};

#endif // MAINWINDOW_H