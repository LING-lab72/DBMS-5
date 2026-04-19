#include "engine/DBEngine.h"
#include "cli/Formatter.h"
#include "cli/Session.h"
#include "cli/Repl.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    Formatter::enableAnsi();

    const std::string dataDir = "data";

    DBEngine  engine(dataDir);
    CLISession session;
    session.engineSession.user = "root";

    Repl repl(engine, session);

    // 支持批处理模式：./dbms < script.sql 或 ./dbms script.sql
    if (argc > 1) {
        repl.runFile(argv[1]);
    } else {
        std::cout << Formatter::green("DBMS v0.1") << "  Type \\help for help.\n";
        repl.run();
    }

    return 0;
}
