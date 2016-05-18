// JSON simple example
// This example does not handle errors.

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "json_binary.h"
#include "stop_watch.h"

#define TEST_BIN_FILENAME       "C:\\test_json_binary.bin"
#define TEST_BIG_BIN_FILENAME   "C:\\test_big_binary.bin"

using namespace rapidjson;

class Employee {
private:
    std::string name_;
    unsigned age_;
    bool married_;
    std::string dependents_;
    std::string file_content_;
    std::size_t content_size_;

public:
    Employee(const std::string& name, unsigned age, bool married) : name_(name), age_(age),
        married_(married), dependents_(""), file_content_(""), content_size_(0) {}
    Employee(const Employee& rhs) : name_(rhs.name_), age_(rhs.age_),
        married_(rhs.married_), dependents_(rhs.dependents_),
        file_content_(rhs.file_content_), content_size_(rhs.content_size_) {
        if (this != &rhs) {
            //
        }
    }
    virtual ~Employee() {};

    Employee& operator =(const Employee& rhs) {
        if (this != &rhs) {
            this->name_ = rhs.name_;
            this->age_ = rhs.age_;
            this->married_ = rhs.married_;
            this->dependents_ = rhs.dependents_;
            this->file_content_ = rhs.file_content_;
            this->content_size_ = rhs.content_size_;
        }
        return *this;
    }

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("name");
        writer.String(name_.c_str());

        writer.String("age");
        writer.Uint(age_);

        writer.String("married");
        writer.Bool(married_);

        writer.String("dependents");
        writer.String(dependents_.c_str());

        writer.String("binary-hex");
        //writer.String(buffer_);
        //writer.String("Content\u5176\u5b9e\u8fd9\u4e2a\u4e0d\u53d7\u7406\u8d8a\u7ea7\u4e0a \u8bbf\u7684\u653f\u7b56\u771f\u7684\u633a\u597d\u7684\uff0c\u8fd9\u6837\u53ef\u4ee5\u5feb \u901f\u7d2f\u79ef\u5e95\u5c42\u8bbf\u6c11\u66b4\u623e\u7684\u813e\u6c14\u3001\u4f7f\u4e4b \u628a\u8bbf\u6c11\u53d8\u5f97\u8d8a\u6765\u8d8a\u60f3\u4f9d\u9760\u66b4\u529b\u6765\u89e3 \u51b3\u4ed6\u4eec\u7684\u95ee\u9898\uff0c\u4ece\u800c\u6d88\u706d\u771f\u6b63\u7684\u654c \u4eba\n\u002d\u0000");
        //writer.String(R"(Content\u5176\u5b9e\u8fd9\u4e2a\u4e0d\u53d7\u7406\u8d8a\u7ea7\u4e0a \u8bbf\u7684\u653f\u7b56\u771f\u7684\u633a\u597d\u7684\uff0c\u8fd9\u6837\u53ef\u4ee5\u5feb \u901f\u7d2f\u79ef\u5e95\u5c42\u8bbf\u6c11\u66b4\u623e\u7684\u813e\u6c14\u3001\u4f7f\u4e4b \u628a\u8bbf\u6c11\u53d8\u5f97\u8d8a\u6765\u8d8a\u60f3\u4f9d\u9760\u66b4\u529b\u6765\u89e3 \u51b3\u4ed6\u4eec\u7684\u95ee\u9898\uff0c\u4ece\u800c\u6d88\u706d\u771f\u6b63\u7684\u654c \u4eba\n\u002d\u0000)");
        if (content_size_ > 0)
            writer.RawValue(file_content_.c_str(), content_size_, kStringType);
        else
            writer.String("[Empty]");

        writer.EndObject();
    }

    bool EncodeFromFile(const std::string & filename, bool add_quote = false) {
        return json_binary<double_escape>::encodeFromFile(filename, file_content_, add_quote);
    }

    bool EncodeHexFromFile(const std::string & filename, bool add_quote = false) {
        return json_binary_hex::encodeFromFile(filename, file_content_, add_quote);
    }

    bool SaveContentToFile(const std::string & filename) {
        return json_binary_utils::saveToFile(filename, file_content_, file_content_.length());
    }
};

void test_simple_dom()
{
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\": \"rapidjson\",\"stars\": 10, \"binary-hex\": \"\"}";
    Document doc;
    doc.Parse(json);

    // 2. Modify it by DOM.
    Value& s = doc["stars"];
    s.SetInt(s.GetInt() + 1);

    Value& bin = doc["binary-hex"];
    std::string content;
    std::size_t content_size = 0;
    json_binary<single_escape>::encodeFromFile(TEST_BIN_FILENAME, content, false);
    bin.SetString(content.c_str(), (rapidjson::SizeType)content_size);  

    // 3. Stringify the DOM
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << std::endl;
    std::cout << buffer.GetString() << std::endl;
    std::cout << std::endl;
}

void test_serialize()
{
    std::vector<Employee> employees;

    Employee milo("Milo YIP", 34, true);
    milo.EncodeFromFile(TEST_BIN_FILENAME, true);
    milo.SaveContentToFile("C:\\test_json_binary_out.bin");

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    milo.Serialize(writer);

    std::cout << std::endl;
    ::puts(sb.GetString());
    std::cout << std::endl;
}

void test_simple_dom_hex()
{
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\": \"rapidjson\",\"stars\": 10, \"binary-hex\": \"\"}";
    Document doc;
    doc.Parse(json);

    // 2. Modify it by DOM.
    Value& s = doc["stars"];
    s.SetInt(s.GetInt() + 1);

    Value& bin = doc["binary-hex"];
    std::string content;
    std::size_t content_size = 0;
    json_binary_hex::encodeFromFile(TEST_BIN_FILENAME, content, false);
    bin.SetString(content.c_str(), (rapidjson::SizeType)content.length());  

    // 3. Stringify the DOM
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << std::endl;
    std::cout << buffer.GetString() << std::endl;
    std::cout << std::endl;
}

void test_serialize_hex()
{
    std::vector<Employee> employees;

    Employee milo("Milo YIP", 34, true);
    milo.EncodeHexFromFile(TEST_BIN_FILENAME, true);
    milo.SaveContentToFile("C:\\test_json_binary_hex_out.bin");

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    milo.Serialize(writer);

    std::cout << std::endl;
    ::puts(sb.GetString());
    std::cout << std::endl;
}

void json_binary_big_file_test()
{
    StopWatch sw;

    std::cout << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;

    //
    // json_binary_utils::readFromFile()
    //
    {
        std::string content;

        sw.start();
        if (json_binary_utils::readFromFile(TEST_BIG_BIN_FILENAME, content)) {
            sw.stop();
            std::cout << "json_binary_utils::readFromFile():" << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary_utils::readFromFile(): failure." << std::endl;
        }
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "-------------------------------------------------------------------" << std::endl;

    //
    // json_binary<single_escape>
    //
    {
        std::string content;

        sw.start();
        if (json_binary<single_escape>::encodeFromFile(TEST_BIG_BIN_FILENAME, content)) {
            sw.stop();
            std::cout << "json_binary<single_escape>::encodeFromFile():" << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary<single_escape>::encodeFromFile(): failure." << std::endl;
        }
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "-------------------------------------------------------------------" << std::endl;

    //
    // json_binary<double_escape>
    //
    {
        std::string content;

        sw.start();
        if (json_binary<double_escape>::encodeFromFile(TEST_BIG_BIN_FILENAME, content)) {
            sw.stop();
            std::cout << "json_binary<double_escape>::encodeFromFile():" << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary<double_escape>::encodeFromFile(): failure." << std::endl;
        }
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "-------------------------------------------------------------------" << std::endl;

    //
    // json_binary_hex::encode()
    //
    {
        std::string content;

        sw.start();
        if (json_binary_hex::encodeFromFile(TEST_BIG_BIN_FILENAME, content)) {
            sw.stop();
            std::cout << "json_binary_hex::encodeFromFile():" << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary_hex::encodeFromFile(): failure." << std::endl;
        }
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;

        {
            std::size_t buf_size = json_binary_hex::get_decode_capacity(content.length());
            std::unique_ptr<char> buffer(new char[buf_size]);
            sw.start();
            std::streamsize decode_size = json_binary_hex::decode(content.c_str(), content.length(), buffer.get(), buf_size);
            if (decode_size >= 0) {
                sw.stop();
                std::cout << "json_binary_hex::decode():" << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size = " << decode_size << std::endl;
            }
            else {
                sw.stop();
                std::cout << "json_binary_hex::decode(): failure." << std::endl;
            }
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "-------------------------------------------------------------------" << std::endl;

    //
    // json_binary_hex::encode_std()
    //
    {
        std::string content;

        sw.start();
        if (json_binary_hex::encodeStdFromFile(TEST_BIG_BIN_FILENAME, content)) {
            sw.stop();
            std::cout << "json_binary_hex::encodeStdFromFile():" << std::endl;
            std::cout << "content_size = " << content.length() << std::endl;
        }
        else {
            sw.stop();
            std::cout << "json_binary_hex::encodeStdFromFile(): failure." << std::endl;
        }
        std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
        std::cout << std::endl;

        {
            std::size_t buf_size = json_binary_hex::get_decode_capacity(content.length());
            std::unique_ptr<char> buffer(new char[buf_size]);
            sw.start();
            std::streamsize decode_size = json_binary_hex::decode_std(content.c_str(), content.length(), buffer.get(), buf_size);
            if (decode_size >= 0) {
                sw.stop();
                std::cout << "json_binary_hex::decode_std():" << std::endl;
                std::cout << "content_size = " << content.length() << std::endl;
                std::cout << "decode_size = " << decode_size << std::endl;
            }
            else {
                sw.stop();
                std::cout << "json_binary_hex::decode_std(): failure." << std::endl;
            }
            std::cout << "time spent: " << sw.getMillisec() << " ms." << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "-------------------------------------------------------------------" << std::endl;
}

int main(int argc, char * argv[])
{
    test_simple_dom();
    test_serialize();

    test_simple_dom_hex();
    test_serialize_hex();

    json_binary_big_file_test();

    ::system("pause");
    return 0;
}
