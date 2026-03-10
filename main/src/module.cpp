#include "module.hpp"

void TextEdit::CreateContext() {
  TextEdit::Context *ctx = VX_NEW(TextEdit::Context);
  CTextEdit = ctx;
}

void TextEdit::DestroyContext() { VX_FREE(CTextEdit); }

void TextEdit::HelloWorld() {
  std::cout << "Hello Vortex World !!" << std::endl;
}

void TextEdit::OutputHandleHello() {
  std::cout << "Handling the HEllow output event...." << std::endl;
}

bool TextEdit::IsValidFile(const std::string &path) {
  namespace fs = std::filesystem;

  if (!fs::is_directory(path)) {
    return false;
  }

  for (const auto &entry : fs::directory_iterator(path)) {
    if (entry.is_regular_file() &&
        entry.path().filename() == "SampleConfig.txt") {
      return true;
    }
  }

  return false;
}

void TextEdit::StartTextEditorInstance(const std::string &path) {
  std::string filename = fs::path(path).filename().string();

  const size_t maxLen = 24;
  if (filename.size() > maxLen) {
    filename = filename.substr(0, maxLen - 3) + "...";
  }

  std::string window_name =
      filename + "####" +
      std::to_string(CTextEdit->m_text_editor_instances.size());

  auto inst = ModuleUI::TextEditorAppWindow::Create(path, window_name);
  Cherry::AddAppWindow(inst->GetAppWindow());
  CTextEdit->m_text_editor_instances.push_back(inst);
}

void TextEdit::InputHello() {
  std::cout << "Input event hello triggered !!!" << std::endl;
}

void TextEdit::FunctionWithArg(ArgumentValues &arg) {
  // std::string name = val.GetJsonValue()["name"].get<std::string>();
  std::cout << "print the name given in aguments"
            << arg.GetJsonValue()["name"].get<std::string>() << std::endl;
}

std::string TextEdit::GetPath(const std::string &path) {
  return CTextEdit->m_interface->GetPath() + "/" + path;
}

void TextEdit::FunctionWithRet(ReturnValues &ret) {
  // Set the return value (time for this example)
  ret.SetJsonValue(nlohmann::json::parse("{\"time\":\"current\"}"));
}
void TextEdit::FunctionWithArgRet(ArgumentValues &arg, ReturnValues &ret) {
  // std::string name = val.GetJsonValue()["name"].get<std::string>();
  std::string name = arg.GetJsonValue()["name"].get<std::string>();
  ret.SetJsonValue(
      nlohmann::json::parse("{\"time\":\"current_name_" + name + "\"}"));
}
