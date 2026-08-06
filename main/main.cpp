#include "./src/module.hpp"

class infinitehq_textedit : public ModuleInterface {
public:
  std::shared_ptr<TextEdit::Context> ctx;
  void execute() override {
    ctx = TextEdit::create_context();

    auto m = ModuleInterface::get_editor_module_by_name(this->name());
    TextEdit::get_current_context()->m_interface = m;

    // Add item handler for simple txt files
    this->add_content_browser_item_handler(ItemHandlerInterface(
        "file_txt", TextEdit::StartTextEditorInstance, "Edit",
        "Edit this txt file", TextEdit::get_path("resources/icons/edit.png")));

    this->add_content_browser_item_handler(ItemHandlerInterface(
        "text_edit:superfile", TextEdit::StartTextEditorInstance, "Super Edit",
        "Edit this txt file", TextEdit::get_path("resources/icons/edit.png")));

    this->add_content_browser_item_identifier(ItemIdentifierInterface(
        TextEdit::IsValidFile, "text_edit:superfile", "Super file", "#553333"));

    this->set_credits_file(TextEdit::get_path("CREDITS"));

    this->add_output_event(
        ModuleOutputEvent(TextEdit::oe_save_all, "save_all"));
  }

  void init_ui() override {
    // CherryApp.AddFont(
    //     "JetBrainsMono",
    //     TextEdit::GetPath("resources/fonts/JetBrainsMono-Regular.ttf"), 40.0f);
  }

  void destroy() override {
    // Reset module
    this->reset_module();

    // Clear windows
    for (auto i : TextEdit::get_current_context()->m_text_editor_instances) {
      CherryApp.DeleteAppWindow(i->GetAppWindow());
    }

    TextEdit::destroy_context(ctx);
    ctx.reset();
  }
};

#ifdef _WIN32
extern "C" __declspec(dllexport) ModuleInterface *create_em() {
  return new infinitehq_textedit();
}
#else
extern "C" ModuleInterface *create_em() { return new infinitehq_textedit(); }
#endif
