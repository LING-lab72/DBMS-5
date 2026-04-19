#include "MetaCommands.h"

MetaResult MetaCommandHandler::handle(const std::string& /*input*/,
                                       CLISession& /*session*/) {
    // TODO(成员B): 实现 \help \quit \status \desc \use \tables \databases \history source
    return {false, ""};
}
