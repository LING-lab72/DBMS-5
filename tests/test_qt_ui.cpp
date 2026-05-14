#include "ui/MainWindow.h"

#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QTest>

class QtUiSmokeTest : public QObject
{
    Q_OBJECT

private slots:
    void sqlInputExecutesThroughMainWindow();
};

void QtUiSmokeTest::sqlInputExecutesThroughMainWindow()
{
    MainWindow window;
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));

    auto *userEdit = window.findChild<QLineEdit *>(QStringLiteral("sessionUserEdit"));
    auto *passwordEdit = window.findChild<QLineEdit *>(QStringLiteral("sessionPasswordEdit"));
    auto *connectButton = window.findChild<QPushButton *>(QStringLiteral("connectButton"));
    auto *sqlEditor = window.findChild<QTextEdit *>(QStringLiteral("sqlEditor"));
    auto *executeButton = window.findChild<QPushButton *>(QStringLiteral("executeSqlButton"));
    auto *resultTable = window.findChild<QTableWidget *>(QStringLiteral("resultTable"));

    QVERIFY(userEdit);
    QVERIFY(passwordEdit);
    QVERIFY(connectButton);
    QVERIFY(sqlEditor);
    QVERIFY(executeButton);
    QVERIFY(resultTable);

    userEdit->clear();
    QTest::keyClicks(userEdit, "root");
    passwordEdit->clear();
    QTest::keyClicks(passwordEdit, "root");
    QTest::mouseClick(connectButton, Qt::LeftButton);

    auto execSql = [&](const QString &sql) {
        sqlEditor->setPlainText(sql);
        QVERIFY(executeButton->isEnabled());
        QTest::mouseClick(executeButton, Qt::LeftButton);
        QCoreApplication::processEvents();
    };

    execSql(QStringLiteral("DROP DATABASE IF EXISTS qt_pipe_smoke"));
    execSql(QStringLiteral("CREATE DATABASE qt_pipe_smoke"));
    execSql(QStringLiteral("USE qt_pipe_smoke"));
    execSql(QStringLiteral("CREATE TABLE items (id INT PRIMARY KEY, name VARCHAR(20))"));
    execSql(QStringLiteral("INSERT INTO items (id, name) VALUES (1, 'alpha')"));
    execSql(QStringLiteral("SELECT name FROM items WHERE id = 1"));

    QCOMPARE(resultTable->rowCount(), 1);
    QCOMPARE(resultTable->columnCount(), 1);
    QVERIFY(resultTable->item(0, 0));
    QCOMPARE(resultTable->item(0, 0)->text(), QStringLiteral("alpha"));

    execSql(QStringLiteral("DROP DATABASE qt_pipe_smoke"));
}

QTEST_MAIN(QtUiSmokeTest)
#include "test_qt_ui.moc"
