#pragma once

#include <QWidget>

class QComboBox;
class QPushButton;
class QTextEdit;

class SqlEditorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SqlEditorPanel(QWidget *parent = nullptr);
    void setExamples(const QStringList &examples);

signals:
    void executeRequested(const QString &sql);

private:
    QTextEdit *editor_;
    QComboBox *examples_;
    QPushButton *executeButton_;
};
