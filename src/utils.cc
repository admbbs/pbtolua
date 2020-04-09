#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctype.h>

#include "utils.h"

#include <google/protobuf/descriptor.pb.h>


namespace {

const char* const kKeywordList[] = {
    "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case",
    "catch", "char", "class", "compl", "const", "const_cast", "continue",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "extern", "false", "float", "for", "friend", "goto", "if",
    "inline", "int", "long", "mutable", "namespace", "new", "not", "not_eq",
    "operator", "or", "or_eq", "private", "protected", "public", "register",
    "reinterpret_cast", "return", "short", "signed", "sizeof", "static",
    "static_cast", "struct", "switch", "template", "this", "throw", "true", "try",
    "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
    "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
};

std::set<std::string> MakeKeywordsMap() {
    std::set<std::string> result;
    for (int i = 0; i < GOOGLE_ARRAYSIZE(kKeywordList); i++) {
        result.insert(kKeywordList[i]);
    }
    return result;
}

std::set<std::string> kKeywords = MakeKeywordsMap();

} // namespace

namespace protolua {
namespace utils {

namespace string {

std::string ToUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string UnderscoresToCamelCase(const std::string& input, bool cap_first_letter) {
    bool cap_next_letter = cap_first_letter;
    std::string result;
    for (size_t i = 0; i < input.size(); i++) {
        if ('a' <= input[i] && input[i] <= 'z') {
            if (cap_next_letter) {
                result += input[i] + ('A' - 'a');
            } else {
                result += input[i];
            }
            cap_next_letter = false;
        } else if ('A' <= input[i] && input[i] <= 'Z') {
            result += input[i];
            cap_next_letter = false;
        } else if ('0' <= input[i] && input[i] <= '9') {
            result += input[i];
            cap_next_letter = true;
        } else {
            cap_next_letter = true;
        }
    }
    return result;
}

bool HasSuffix(const std::string& str,
                            const std::string& suffix) {
  return str.size() >= suffix.size() &&
         str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string StripSuffix(const std::string& filename, const char* suffix) {
    const std::string str_suffix = suffix;
    if (HasSuffix(filename, str_suffix)) {
        return filename.substr(0, filename.size() - str_suffix.size());
    } else {
        return filename;
    }
}

void Split(const std::string& str, const char* delim, std::vector<std::string>* result) {
    if (delim[0] != '\0' && delim[1] == '\0') {
        char c = delim[0];
        const char* p = str.data();
        const char* end = p + str.size();
        while (p != end) {
        if (*p == c) {
            ++p;
        } else {
            const char* start = p;
            while (++p != end && *p != c);
            result->push_back(std::string(start, p - start));
        }
        }
        return;
    }

    std::string::size_type begin_index, end_index;
    begin_index = str.find_first_not_of(delim);
    while (begin_index != std::string::npos) {
        end_index = str.find_first_of(delim, begin_index);
        if (end_index == std::string::npos) {
            result->push_back(str.substr(begin_index));
            return;
        }
        result->push_back(str.substr(begin_index, (end_index - begin_index)));
        begin_index = str.find_first_not_of(delim, end_index);
    }
}

std::string ReplaceDotsWithUnderscores(const std::string& name) {
  return Replace(name, ".", "_", true);
}

std::string ReplaceDotsWithDoubleColons(const std::string& name) {
  return Replace(name, ".", "::", true);
}

void Strip(std::string* s, const char* remove, char replacewith) {
    const char* str_start = s->c_str();
    const char* str = str_start;
    for (str = strpbrk(str, remove);
        str != NULL;
        str = strpbrk(str + 1, remove)) {
        (*s)[str - str_start] = replacewith;
    }
}

void Replace(const std::string& s, const std::string& oldsub,
                   const std::string& newsub, bool replace_all, std::string* res) {
    if (oldsub.empty()) {
        res->append(s);
        return;
    }

    std::string::size_type start_pos = 0;
    std::string::size_type pos;
    do {
        pos = s.find(oldsub, start_pos);
        if (pos == std::string::npos) {
        break;
        }
        res->append(s, start_pos, pos - start_pos);
        res->append(newsub);
        start_pos = pos + oldsub.size();
    } while (replace_all);
    res->append(s, start_pos, s.length() - start_pos);
}

std::string Replace(const std::string& s, const std::string& oldsub,
                          const std::string& newsub, bool replace_all) {
    std::string ret;
    Replace(s, oldsub, newsub, replace_all, &ret);
    return ret;
}

} // namespace string

namespace protoc {

std::string ProtoHeaderName(const std::string& filename) {
    return utils::string::StripSuffix(filename, ".proto") + ".pb.h";
}

bool HasDescriptorMethods(const google::protobuf::FileDescriptor* file) {
  return file->options().optimize_for() != google::protobuf::FileOptions::LITE_RUNTIME;
}

bool HasEnumDefinitions(const google::protobuf::Descriptor* message_type) {
  if (message_type->enum_type_count() > 0) return true;
  for (int i = 0; i < message_type->nested_type_count(); ++i) {
    if (HasEnumDefinitions(message_type->nested_type(i))) return true;
  }
  return false;
}

bool HasEnumDefinitions(const google::protobuf::FileDescriptor* file) {
  if (file->enum_type_count() > 0) return true;
  for (int i = 0; i < file->message_type_count(); ++i) {
    if (HasEnumDefinitions(file->message_type(i))) return true;
  }
  return false;
}

bool HasGeneratedMethods(const google::protobuf::FileDescriptor* file) {
    return file->options().optimize_for() != google::protobuf::FileOptions::CODE_SIZE;
}

std::string ClassName(const google::protobuf::Descriptor* descriptor, bool qualified) {
    const google::protobuf::Descriptor* outer = descriptor;
    while (outer->containing_type() != NULL) outer = outer->containing_type();

    const std::string& outer_name = outer->full_name();
    std::string inner_name = descriptor->full_name().substr(outer_name.size());

    if (qualified) {
        return utils::string::ReplaceDotsWithDoubleColons(outer_name) + utils::string::ReplaceDotsWithUnderscores(inner_name);
    } else {
        return outer->name() + utils::string::ReplaceDotsWithUnderscores(inner_name);
    }
}

std::string ClassName(const google::protobuf::EnumDescriptor* enum_descriptor, bool qualified) {
    if (enum_descriptor->containing_type() == NULL) {
    if (qualified) {
        return utils::string::ReplaceDotsWithDoubleColons(enum_descriptor->full_name());
    } else {
        return enum_descriptor->name();
    }
    } else {
        std::string result = ClassName(enum_descriptor->containing_type(), qualified);
        result += '_';
        result += enum_descriptor->name();
        return result;
    }
}

std::string SuperClassName(const google::protobuf::Descriptor* descriptor) {
    return HasDescriptorMethods(descriptor->file()) ?
      "::google::protobuf::Message" : "::google::protobuf::MessageLite";
}

std::string FieldName(const google::protobuf::FieldDescriptor* field) {
    std::string result = utils::string::ToLower(field->name());
    if (kKeywords.count(result) > 0) {
        result.append("_");
    }
    return result;
}

const char* PrimitiveTypeName(google::protobuf::FieldDescriptor::CppType type) {
    switch (type) {
        case google::protobuf::FieldDescriptor::CPPTYPE_INT32  : return "int32_t";
        case google::protobuf::FieldDescriptor::CPPTYPE_INT64  : return "int64_t";
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT32 : return "uint32_t";
        case google::protobuf::FieldDescriptor::CPPTYPE_UINT64 : return "uint64_t";
        case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE : return "double";
        case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT  : return "float";
        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL   : return "bool";
        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM   : return "int";
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING : return "std::string";
        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: return NULL;

        // No default so that the compiler will complain if any new CppTypes are added.
    }

    return NULL;
}

std::string SafeFunctionName(const google::protobuf::Descriptor* descriptor,
                             const google::protobuf::FieldDescriptor* field,
                             const std::string& prefix) {
    std::string name = utils::string::ToLower(field->name());
    std::string function_name = prefix + name;
    if (descriptor->FindFieldByName(function_name)) {
        function_name.append("__");
    } else if (kKeywords.count(name) > 0) {
        function_name.append("_");
    }
    return function_name;
}

std::string EnumIToA(int i) {
    if (i == std::numeric_limits<int>::min()) {
        return "(~0x7fffffff)";
    } else {
        return utils::string::SimpleItoa(i);
    }
}

} // namespace protoc

} // namespace utils
} // namespace protolua
