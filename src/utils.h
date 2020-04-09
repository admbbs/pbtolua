#ifndef PROTOLUA_UTILS_H__
#define PROTOLUA_UTILS_H__

#include <string>
#include <vector>
#include <sstream>

#include <google/protobuf/descriptor.pb.h>

namespace protolua {
namespace utils {

namespace string {

std::string ToUpper(std::string str);
std::string ToLower(std::string str);
std::string UnderscoresToCamelCase(const std::string& string, bool cap_first_letter);
bool HasSuffix(const std::string& str, const std::string& suffix);
std::string StripSuffix(const std::string& filename, const char* suffix);

void Split(const std::string& str, const char* delim, std::vector<std::string>* result);
void Strip(std::string* s, const char* remove, char replacewith);
void Replace(const std::string& s, const std::string& oldsub, const std::string& newsub, bool replace_all, std::string* res);
std::string Replace(const std::string& s, const std::string& oldsub, const std::string& newsub, bool replace_all);

std::string ReplaceDotsWithUnderscores(const std::string& name);
std::string ReplaceDotsWithDoubleColons(const std::string& name);

template <typename IntegerType>
std::string SimpleItoa(IntegerType i);

} // namespace string

namespace protoc {

std::string ProtoHeaderName(const std::string& filename);

bool HasDescriptorMethods(const google::protobuf::FileDescriptor* file);
bool HasEnumDefinitions(const google::protobuf::FileDescriptor* file);
bool HasGeneratedMethods(const google::protobuf::FileDescriptor* file);

std::string ClassName(const google::protobuf::Descriptor* descriptor, bool qualified);
std::string ClassName(const google::protobuf::EnumDescriptor* enum_descriptor, bool qualified);
std::string SuperClassName(const google::protobuf::Descriptor* descriptor);
std::string FieldName(const google::protobuf::FieldDescriptor* field);

const char* PrimitiveTypeName(google::protobuf::FieldDescriptor::CppType type);
std::string SafeFunctionName(const google::protobuf::Descriptor* descriptor,
                             const google::protobuf::FieldDescriptor* field,
                             const std::string& prefix);

std::string EnumIToA(int i);


} // namespace protoc

} // namespace utils
} // namespace protolua

template <typename IntegerType>
std::string protolua::utils::string::SimpleItoa(IntegerType i)
{
    std::stringstream s;
    s << i;
    return s.str();
}

#endif // PROTOLUA_UTILS_H__
