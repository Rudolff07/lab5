#include "mainwindow.h"
#include "purchase_delegate.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_model(nullptr)
    , m_view(nullptr)
    , m_input(nullptr)
    , m_status(nullptr)
    , m_addBtn(nullptr)
    , m_removeBtn(nullptr)
    , m_clearBtn(nullptr)
    , m_rootLayout(nullptr)
    , m_inputRow(nullptr)
    , m_btnRow(nullptr)
{
    setupUi();
    refreshStatus();
    qDebug() << "[INIT] окно создано";
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    setWindowTitle(QStringLiteral("Менеджер списка покупок - Model-View"));
    resize(520, 560);

    m_rootLayout = new QVBoxLayout(this);
    m_rootLayout->setContentsMargins(16, 16, 16, 16);
    m_rootLayout->setSpacing(10);

    auto *title = new QLabel(QStringLiteral("<b>Список покупок</b>"), this);
    m_rootLayout->addWidget(title);

    m_model = new QStringListModel(this);
    m_model->setStringList({"Молоко", "Хлеб", "Яблоки"});

    m_view = new QListView(this);
    m_view->setModel(m_model);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked);

    m_view->setItemDelegate(new PurchaseDelegate(m_view));
    m_rootLayout->addWidget(m_view, 1);

    m_inputRow = new QHBoxLayout();
    m_input = new QLineEdit(this);
    m_input->setPlaceholderText(QStringLiteral("Новый товар…"));
    m_addBtn = new QPushButton(QStringLiteral("Добавить"), this);
    m_inputRow->addWidget(m_input, 1);
    m_inputRow->addWidget(m_addBtn);
    m_rootLayout->addLayout(m_inputRow);

    m_btnRow = new QHBoxLayout();
    m_removeBtn = new QPushButton(QStringLiteral("Удалить выделенное"), this);
    m_clearBtn = new QPushButton(QStringLiteral("Очистить"), this);
    m_btnRow->addWidget(m_removeBtn);
    m_btnRow->addWidget(m_clearBtn);
    m_btnRow->addStretch();
    m_rootLayout->addLayout(m_btnRow);

    m_status = new QLabel(this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status);

    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::addItem);
    connect(m_removeBtn, &QPushButton::clicked, this, &MainWindow::removeSelected);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(m_input, &QLineEdit::returnPressed, this, &MainWindow::addItem);

    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this](const QItemSelection &, const QItemSelection &){
                refreshStatus();
            });
}

void MainWindow::addItem()
{
    const QString text = m_input->text().trimmed();
    if (text.isEmpty()) return;

    const int row = m_model->rowCount();
    m_model->insertRow(row);
    m_model->setData(m_model->index(row, 0), text, Qt::EditRole);

    m_input->clear();
    m_input->setFocus();
    refreshStatus();
    qDebug() << "[CRUD] addItem: добавлено" << text;
}

void MainWindow::removeSelected()
{
    const QModelIndex current = m_view->currentIndex();
    if (!current.isValid()) return;

    m_model->removeRow(current.row());
    refreshStatus();
    qDebug() << "[CRUD] removeSelected: удалена строка" << current.row();
}

void MainWindow::clearAll()
{
    m_model->setStringList({});
    refreshStatus();
    qDebug() << "[CRUD] clearAll: модель сброшена";
}

void MainWindow::refreshStatus()
{
    const int n = m_model->rowCount();
    const bool hasSel = m_view->currentIndex().isValid();

    if (m_status) {
        m_status->setText(QStringLiteral("Элементов в модели: %1 | выделение: %2")
                              .arg(n)
                              .arg(hasSel ? "есть" : "нет"));
    }
    if (m_removeBtn) {
        m_removeBtn->setEnabled(hasSel);
    }
}