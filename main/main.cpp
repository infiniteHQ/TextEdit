#include "./src/module.hpp"

#ifndef CTextEdit
TextEdit::Context *CTextEdit = NULL;
#endif

class Module : public ModuleInterface {
public:
  void execute() override {
    // Create the context pointer of this module
    TextEdit::CreateContext();

    // Get the interface pointer
    CTextEdit->m_interface =
        ModuleInterface::GetEditorModuleByName(this->m_name);

    // Add item handler for simple txt files
    this->AddContentBrowserItemHandler(ItemHandlerInterface(
        "file_txt", TextEdit::StartTextEditorInstance, "Edit",
        "Edit this txt file", TextEdit::GetPath("resources/icons/edit.png")));

    this->AddContentBrowserItemHandler(ItemHandlerInterface(
        "text_edit:superfile", TextEdit::StartTextEditorInstance, "Super Edit",
        "Edit this txt file", TextEdit::GetPath("resources/icons/edit.png")));

    this->AddContentBrowserItemIdentifier(ItemIdentifierInterface(
        TextEdit::IsValidFile, "text_edit:superfile", "Super file", "#553333"));

    this->AddCreditsFile(TextEdit::GetPath("CREDITS"));
  }

  void init_ui() override {
    CherryApp.AddFont(
        "JetBrainsMono",
        TextEdit::GetPath("resources/fonts/JetBrainsMono-Regular.ttf"), 40.0f);
  }

  void destroy() override {
    // Reset module
    this->ResetModule();

    // Clear windows
    for (auto i : CTextEdit->m_text_editor_instances) {
      CherryApp.DeleteAppWindow(i->GetAppWindow());
    }

    // Clear context
    // DestroyContext();
  }
};

#ifdef _WIN32
extern "C" __declspec(dllexport) ModuleInterface *create_em() {
  return new Module();
}
#else
extern "C" ModuleInterface *create_em() { return new Module(); }
#endif
