#include <google/protobuf/compiler/plugin.h>

#include "tolua_generator.h"

int main(int argc, char* argv[]) {
    protolua::ToluaGenerator tolua_generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &tolua_generator);
}
