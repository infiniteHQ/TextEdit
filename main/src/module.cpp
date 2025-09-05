#include "module.hpp"

void SampleCppModule::CreateContext()
{
    SampleCppModule::Context *ctx = VX_NEW(SampleCppModule::Context);
    CSampleModule = ctx;
}

void SampleCppModule::HelloWorld()
{
    std::cout << "Hello Vortex World !!" << std::endl;
}

void SampleCppModule::OutputHandleHello()
{
    std::cout << "Handling the HEllow output event...." << std::endl;
}

void SampleCppModule::InputHello()
{
    std::cout << "Input event hello triggered !!!" << std::endl;
}

void SampleCppModule::FunctionWithArg(ArgumentValues &arg)
{
    // std::string name = val.GetJsonValue()["name"].get<std::string>();
    std::cout << "print the name given in aguments" << arg.GetJsonValue()["name"].get<std::string>() << std::endl;
}

void SampleCppModule::FunctionWithRet(ReturnValues &ret)
{
    // Set the return value (time for this example)
    ret.SetJsonValue(nlohmann::json::parse("{\"time\":\"current\"}"));
}
void SampleCppModule::FunctionWithArgRet(ArgumentValues &arg, ReturnValues &ret)
{
    // std::string name = val.GetJsonValue()["name"].get<std::string>();
    std::string name = arg.GetJsonValue()["name"].get<std::string>();
    ret.SetJsonValue(nlohmann::json::parse("{\"time\":\"current_name_" + name + "\"}"));
}
