#ifndef PROTOLUA_TOLUA_GENERATOR_H__
#define PROTOLUA_TOLUA_GENERATOR_H__

#include <string>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>


namespace protolua {

class ToluaGenerator : public ::google::protobuf::compiler::CodeGenerator {
public:
    ToluaGenerator();
    virtual ~ToluaGenerator();

    virtual bool Generate(const ::google::protobuf::FileDescriptor* file,
                          const std::string& parameter,
                          ::google::protobuf::compiler::GeneratorContext* generator_context,
                          std::string* error) const;

private:
    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ToluaGenerator);
};

} // namespace protolua

#endif // PROTOLUA_TOLUA_GENERATOR_H__
