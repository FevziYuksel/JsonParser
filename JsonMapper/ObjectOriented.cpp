//#include "TestData.h"
//#include "Utility.h";
//
////JBase yap, JObject ayrı bi class
////parse'ı implemente et her biri için
//
//namespace OOP {
//    class JObject
//    {
//    protected:
//        std::string mTag;
//        std::stringstream ss;
//        JObject(const std::string& tag) : mTag{ tag } {}
//        JObject(std::string&& tag) : mTag{ std::move(tag) } {}
//    public:
//        virtual ~JObject() {}
//        virtual JObject parse() = 0;
//
//    };
//
//    class JString : public JObject
//    {
//    private:
//        std::string mValue;
//    public:
//        JString(const std::string& tag, const std::string& value) : JObject::JObject(tag), mValue{ value } {}
//        JString(std::string&& tag, std::string&& value) : JObject::JObject(std::move(tag)), mValue{ std::move(value) } {}
//
//        virtual ~JString() override {}
//
//    };
//
//    //template<typename TNumber>
//    class JNumber : public JObject
//    {
//    private:
//        //TNumber mValue;
//        long long mValue;
//
//    public:
//        JNumber(const std::string& tag, const std::string& value) : JObject::JObject(tag)
//        {
//            ss << value;
//            ss >> mValue;
//        }
//        JNumber(std::string&& tag, std::string&& value) : JObject::JObject(std::move(tag))
//        {
//            ss << value;
//            ss >> mValue;
//        }
//
//        virtual ~JNumber() override {}
//    };
//
//    class JBool : public JObject
//    {
//    private:
//        bool mValue;
//    public:
//        JBool(const std::string& tag, const std::string& value) : JObject::JObject(tag)
//        {
//            ss << value;
//            ss >> mValue;
//        }
//        JBool(std::string&& tag, std::string&& value) : JObject::JObject(std::move(tag))
//        {
//            ss << value;
//            ss >> mValue;
//        }
//
//        virtual ~JBool() override {}
//    };
//
//
//    class JNull : public JObject
//    {
//    private:
//        nullptr_t mValue;
//    public:
//        JNull(const std::string& tag) : JObject::JObject(tag), mValue{ nullptr }{}
//        JNull(std::string&& tag) : JObject::JObject(std::move(tag)), mValue{ nullptr } {}
//        virtual ~JNull() override {}
//    };
//
//    class JArray : public JObject
//    {
//    private:
//        std::vector<JObject*> mValue;
//    public:
//        JArray(const std::string& tag) : JObject::JObject(tag) {}
//        JArray(std::string&& tag) : JObject::JObject(std::move(tag)) {}
//
//        void add(JObject* jObject)
//        {
//            mValue.push_back(jObject);
//        }
//
//        virtual ~JArray() override {}
//    };
//}
//
