#include <google/protobuf/compiler/command_line_interface.h>

#include "tolua_generator.h"

int main(int argc, char* argv[]) {
    google::protobuf::compiler::CommandLineInterface cli;
    cli.AllowPlugins("protoc-");

    protolua::ToluaGenerator tolua_generator;
    cli.RegisterGenerator("--tolua_out", &tolua_generator,
                            "Generate toluapp .pkg file.");

    return cli.Run(argc, argv);
}
