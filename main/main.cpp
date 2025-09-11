#include "./src/module.hpp"
#include "./ui/main/ui.hpp"

#ifndef CSampleModule
SampleCppModule::Context *CSampleModule = NULL;
#endif

class Module : public ModuleInterface {
public:
  void execute() override {
    // Create the context pointer of this module
    SampleCppModule::CreateContext();

    // Get the interface pointer (for GUI launcher, from other modules)
    CSampleModule->m_interface =
        ModuleInterface::GetEditorModuleByName(this->m_name);

    // Adding functions
    this->AddFunction(SampleCppModule::HelloWorld, "HelloWorld");
    this->AddFunction(SampleCppModule::FunctionWithArg, "Arg");
    this->AddFunction(SampleCppModule::FunctionWithArgRet, "ArgRet");
    this->AddFunction(SampleCppModule::FunctionWithRet, "Ret");

    this->AddOutputEvent(SampleCppModule::OutputHandleHello,
                         "OutputHandleHello");
    this->AddInputEvent(SampleCppModule::InputHello, "InputHello");

    this->AddContentBrowserItemHandler(ItemHandlerInterface(
        "file_txt", SampleCppModule::StartTextEditorInstance,
        "Edit this file"));

    // this->SetContentBrowserSaveAllCallback();
    // AddMainSettingsEntry

    {
      ArgumentValues values("{\"name\":\"hohoho\"}");
      ReturnValues ret;
      this->CallInputEvent(this->m_name, "InputHello", values, ret);
    }

    {
      ArgumentValues values("{\"name\":\"hohoho\"}");
      ReturnValues ret;
      this->CallOutputEvent("OutputHandleHello", values, ret);
    }

    {
      ArgumentValues values("{\"name\":\"hohoho\"}");
      this->ExecuteFunction("Arg", values);
    }

    {
      ArgumentValues values("{\"name\":\"hohoho\"}");
      ReturnValues ret;
      this->ExecuteFunction("ArgRet", values, ret);
      std::cout << "The return of ArgRet is : "
                << ret.GetJsonValue()["time"].get<std::string>() << std::endl;
    }

    {
      ReturnValues ret;
      this->ExecuteFunction("Ret", ret);
      std::cout << "The return of Ret is : "
                << ret.GetJsonValue()["time"].get<std::string>() << std::endl;
    }
  }

  /**
   * @brief Renders the GUI for the Packages.
   *
   * This function is responsible for rendering the GUI for the Packages.
   * It displays each package's name along with an "Open" button.
   * When the button is clicked, it creates and adds a render instance for the
   * package.
   */
  void render() override {
    /*ImGui::Begin("Packages");

    // Display the package's name

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit |
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
    | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

    if (ImGui::BeginTable("table_", 3, flags))
    {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();
        // Iterate through each package
        for (int i = 0; i < CSampleModule->m_packages.size(); i++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++)
            {
                ImGui::TableSetColumnIndex(column);

                if (column == 0)
                {
                    std::string label = "Open ###" +
    CSampleModule->m_packages[i]->name; if (ImGui::Button(label.c_str()))
                    {
                        // Create a render instance for the package and add it
                        VxContext *ctx = VortexMaker::GetCurrentContext();
                        std::shared_ptr<PackageRenderInstance> instance =
    std::make_shared<PackageRenderInstance>(ctx, CSampleModule->m_packages[i]);
                        instance->name = CSampleModule->m_packages[i]->name;
                        this->AddModuleRenderInstance(instance);
                    }
                }
                if (column == 1)
                {
                    ImGui::Text(CSampleModule->m_packages[i]->name.c_str());
                }
                if (column == 2)
                {
                    ImGui::Text("Thu 28 Apr 2024");
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();*/
  }
};

extern "C" ModuleInterface *create_em() { return new Module(); }
