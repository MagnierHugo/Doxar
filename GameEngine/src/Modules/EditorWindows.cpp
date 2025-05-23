#include "Modules/EditorWindows.hpp"

#include "imgui/imgui_internal.h"

#include "Modules/RenderModule.hpp"
#include "Modules/ImGUIModule.hpp"

#include "Render/Renderer.hpp"

#define ICON_FA_FILE_3D "\xef\x87\x86" // Pour les fichiers degueulasse qui represente les fichier obj et fbx


//static bool operator!=(const ImVec2& lhs, const ImVec2 rhs) {
//    return
//        ((lhs.x - rhs.x) >= 0.0001f) || 
//        ((lhs.x - rhs.x) <= -0.0001f) ||
//        ((lhs.y - rhs.y) >= 0.0001f) || 
//        ((lhs.y - rhs.y) <= -0.0001f)
//    ;
//}


struct WindowParams {
    ImVec2 position;
    ImVec2 currentPosition;
    bool isMoving;
    ImGuiID dockID;
};

static void ManageDocking(std::string windowName) {
    static std::unordered_map<std::string, WindowParams> windows;

    if (!windows.contains(windowName)) {
        windows[windowName] = {
            .position = ImGUI::GetWindowPos(),
            .currentPosition = ImGUI::GetWindowPos(),
            .isMoving = false,
            .dockID = ImGUI::GetWindowDockID()
        };
        return;
    }

    WindowParams& window = windows[windowName];
    ImVec2 newPosition = ImGUI::GetWindowPos();

    if (!window.isMoving) {
        if (window.position != newPosition) {
            window.isMoving = true;
            window.currentPosition = newPosition;
        }
        return;
    }


    if (window.currentPosition != newPosition) {
        window.currentPosition = newPosition;
        return;
    }
    window.isMoving = false;

    if (!ImGUI::IsWindowDocked()) {
        ImGUI::SetWindowPos(window.position);
    }
    else {
        window.position = newPosition;
        window.dockID = ImGUI::GetWindowDockID();
    }
}


#pragma region Draw and Update Icon

void EditorWindows::DrawFileName(std::string text) {
    if (text.size() > maxCharacter) {
        text = text.substr(0, maxCharacter - 3) + "...";
        ImGui::Text(text.c_str());
    }
    else {
        ImGui::Text(text.c_str());
    }


}

void EditorWindows::DirectoryIcon(std::shared_ptr<FileNode> child) {
    ImGui::BeginGroup();

    std::string button_id = "Button_" + child->name;
    ImGui::PushID(button_id.c_str());

    if (ImGui::Button(ICON_FA_FOLDER, sizeDirectoryIcon)) {
        currentNode = child;
    }
    else {
        DrawFileName(child->name);

        //if ((i + 1) % 6 != 0) { // Pour pas afficher trop de fichier sur la meme ligne
        //    ImGui::SameLine();
        //}
        ImGui::NewLine();
    }

    ImGui::PopID();
    ImGui::EndGroup();
    ImGui::SameLine();
}

void EditorWindows::SongIcon(std::shared_ptr<FileNode> child) {
    ImGui::BeginGroup();


    std::string button_id = "Button_Song" + child->name;
    ImGui::PushID(button_id.c_str());


    if (ImGui::Button(ICON_FA_MUSIC,sizeSongIcon)) {
        audiomanager->AddAudio(child->name, child->path.c_str());
        audiomanager->AudioStart(child->name);
    }

    DrawFileName(child->name);

    ImGui::PopID();
    ImGui::EndGroup();
    ImGui::SameLine();
}

void EditorWindows::ObjectIcon(std::shared_ptr<FileNode> child) {
    ImGui::BeginGroup();


    std::string button_id = "Button_Object" + child->name;
    ImGui::PushID(button_id.c_str());

    if (ImGui::Button("\xef\x87\x86", sizeObjectIcon)) {
        // entrez ici la réaction au click
    }

    DrawFileName(child->name);

    ImGui::PopID();
    ImGui::EndGroup();
    ImGui::SameLine();
}

void EditorWindows::PictureFile(std::shared_ptr<FileNode> child) {
    ImGui::BeginGroup();

    std::string button_id = "Button_Picture" + child->name;
    ImGui::PushID(button_id.c_str());

    if (ImGui::Button(ICON_FA_IMAGE, sizePictureIcon)) {
        // entrez ici la réaction au click
    }
    DrawFileName(child->name);

    ImGui::PopID();
    ImGui::EndGroup();
    ImGui::SameLine();
}

#pragma endregion

#pragma region Files search

void EditorWindows::GenerateFileTree(std::shared_ptr<FileNode> node) {
    if (!node->is_directory)
        return;

    for (const auto& entry : std::filesystem::directory_iterator(node->path)) {
        std::shared_ptr<FileNode> child = std::make_shared<FileNode>();

        child->name = entry.path().filename().string();
        child->path = entry.path().string();
        child->is_directory = entry.is_directory();

        if (child->name != "Assets")
            child->sourceDirectory = node;


        if (child->is_directory) {
            GenerateFileTree(child);

        }

        node->children.push_back(std::move(child));
    }
}

void EditorWindows::UpdateCurrentFolder() {

    int i = 0;
    for (std::shared_ptr<FileNode> child : currentNode->children) {
        if (child->is_directory) {
            DirectoryIcon(child);
        }
        else if (child->name.ends_with('.mp3') || child->name.ends_with('.wav')) {  // audioFile
            SongIcon(child);
        }
        else if (child->name.ends_with('.obj') || child->name.ends_with('.fbx')) {  // objectFile
            ObjectIcon(child);
        }
        else if (child->name.ends_with('.png') || child->name.ends_with('.jpg')) {  // pngFile
            PictureFile(child);
        }
    } 
}

void EditorWindows::DisplayFileTree(std::shared_ptr<FileNode> node) {
	if (node->is_directory) {
		if (ImGui::TreeNode(node->name.c_str())) {
			for (std::shared_ptr<FileNode> child : node->children) {
			DisplayFileTree(child);
			}
			ImGui::TreePop();
		}
	}
	else {
	ImGui::BulletText("%s", node->name.c_str());
	}
}

int EditorWindows::SetCurrentPath(std::string name, std::string path, std::shared_ptr<FileNode> node) {
    if (node->is_directory) {
        std::cout << "Checking node: " << name << " (" << path << ")\n";
        if (node->name == name && node->path == path) {
            currentNode = node;
            std::cout << "enter" << std::endl;
            return 1;
        }
        else {
            for (std::shared_ptr<FileNode> child : node->children) {
                if (SetCurrentPath(name, path, child))
                    return 1;
            }
        }
    }
    return 0;
}


void EditorWindows::NavMenuArbo() {

    std::string currentPath = currentNode->path.c_str();
    std::unordered_map<std::string, std::string> linkMap;

    std::filesystem::path pathObj = std::filesystem::relative(currentPath);
    std::string accumulatedPath;

    for (const auto& part: pathObj) {
    accumulatedPath += part.string() + "\\";
    linkMap[part.string()] = accumulatedPath;
    }


    for (const auto& pair : linkMap) {
        ImGui::SameLine();
        std::string button_id = "NavMenuArboButton_" + pair.first;
        ImGui::PushID(button_id.c_str());

        if (ImGui::Button(pair.first.c_str())) {
            std::string test = pair.second;
            test.pop_back();
            SetCurrentPath(pair.first,test, rootNode);
        }
        ImGui::SameLine();
        ImGui::Text(">");
        ImGui::PopID();

    }

    ImGui::Separator();
}

#pragma endregion

#pragma region Viewport importCode

void ShowExampleAppDockSpace(bool* p_open)
{
    // Variables to configure the Dockspace example.
    static bool opt_fullscreen = true; // Is the Dockspace full-screen?
    static bool opt_padding = false; // Is there padding (a blank space) between the window edge and the Dockspace?
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // Config flags for the Dockspace
    // In this example, we're embedding the Dockspace into an invisible parent window to make it more configurable.
    // We set ImGuiWindowFlags_NoDocking to make sure the parent isn't dockable into because this is handled by the Dockspace.
    //
    // ImGuiWindowFlags_MenuBar is to show a menu bar with config options. This isn't necessary to the functionality of a
    // Dockspace, but it is here to provide a way to change the configuration flags interactively.
    // You can remove the MenuBar flag if you don't want it in your app, but also remember to remove the code which actually
    // renders the menu bar, found at the end of this function.
    ImGuiWindowFlags window_flags = /*ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;

    // Is the example in Fullscreen mode?
    if (opt_fullscreen)
    {
        // If so, get the main viewport:
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Set the parent window's styles to match that of the main viewport:
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        // The example is not in Fullscreen mode (the parent window can be dragged around and resized), disable the
        // ImGuiDockNodeFlags_PassthruCentralNode flag.
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so the parent window should not have its own background:
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // If the padding option is disabled, set the parent window's padding size to 0 to effectively hide said padding.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("DockSpace Demo", p_open, window_flags);

    // Remove the padding configuration - we pushed it, now we pop it:
    if (!opt_padding)
        ImGui::PopStyleVar();

    // Pop the two style rules set in Fullscreen mode - the corner rounding and the border size.
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Check if Docking is enabled:
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // If it is, draw the Dockspace with the DockSpace() function.
        // The GetID() function is to give a unique identifier to the Dockspace - here, it's "MyDockSpace".
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        // Docking is DISABLED - Show a warning message
        //ShowDockingDisabledMessage();
    }

    //// This is to show the menu bar that will change the config settings at runtime.
    //// If you copied this demo function into your own code and removed ImGuiWindowFlags_MenuBar at the top of the function,
    //// you should remove the below if-statement as well.
    //if (ImGui::BeginMenuBar())
    //{
    //    if (ImGui::BeginMenu("Options"))
    //    {
    //        // Disabling fullscreen would allow the window to be moved to the front of other windows,
    //        // which we can't undo at the moment without finer window depth/z control.
    //        ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
    //        ImGui::MenuItem("Padding", NULL, &opt_padding);
    //        ImGui::Separator();

    //        // Display a menu item for each Dockspace flag, clicking on one will toggle its assigned flag.
    //        if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
    //        if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
    //        if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
    //        if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
    //        if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
    //        ImGui::Separator();

    //        // Display a menu item to close this example.
    //        if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
    //            if (p_open != NULL) // Remove MSVC warning C6011 (NULL dereference) - the `p_open != NULL` in MenuItem() does prevent NULL derefs, but IntelliSense doesn't analyze that deep so we need to add this in ourselves.
    //                *p_open = false; // Changing this variable to false will close the parent window, therefore closing the Dockspace as well.
    //        ImGui::EndMenu();
    //    }

    //    // Show a help marker that gives an overview of what this example is and does.
    //    //HelpMarker(
    //    //    "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
    //    //    "- Drag from window title bar or their tab to dock/undock." "\n"
    //    //    "- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
    //    //    "- Hold SHIFT to disable docking." "\n"
    //    //    "This demo app has nothing to do with it!" "\n\n"
    //    //    "This demo app only demonstrates the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
    //    //    "Read comments in ShowExampleAppDockSpace() for more details.");

    //    ImGui::EndMenuBar();
    //}

    // End the parent window that contains the Dockspace:
    ImGui::End();
}

#pragma endregion

#pragma region Hierarchy

void EditorWindows::HierarchyWindow() {

    ImGUI::Begin("Hierarchy");
    for (auto& [sceneName, scene] : scenesManager->scenes)
    {
        if (ImGUI::TreeNode(sceneName.c_str()))
        {
            bool doExpandScene = expandedScenes.contains(sceneName);
            if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                if (doExpandScene)
                {
                    expandedScenes.erase(sceneName);
                    doExpandScene = false;
                }
                else
                {
                    expandedScenes.emplace(sceneName);
                    doExpandScene = true;
                }
            }


            if (doExpandScene)
            {
                for (GameObject* gameObject : scene->gameObjects)
                {
                    if (ImGUI::TreeNode(gameObject->name.c_str()))
                    {
                        inspectedGameObject = gameObject;
                        ImGui::TreePop();
                    }
                }
            }



            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right)) {
            ImGUI::OpenPopup("SceneRightClickMenu");
        }


        if (ImGui::BeginPopup("SceneRightClickMenu"))
        {
            if (ImGUI::MenuItem("Add Empty GameObject")) {
                int count = 0;
                for (GameObject* gameObject : scene->gameObjects) {
                    if (gameObject->name.find("GameObject") != std::string::npos) {
                        count++;
                    }
                }
                scene->AddGameObjectUntracked("GameObject" + (count == 0 ? "" : std::to_string(count)), true);
            }
            if (ImGUI::MenuItem("Add Katana")) {
                int count = 0;
                for (GameObject* gameObject : scene->gameObjects) {
                    if (gameObject->name.find("Katana") != std::string::npos) {
                        count++;
                    }
                }
                scene->AddGameObjectUntracked("Katana" + (count == 0 ? "" : std::to_string(count)), true)->AddComponent<MeshRenderer>()->LoadModel("Assets/katana_V2.obj");
            }

            ImGui::EndPopup();
        }
    }
    ManageDocking("Hierarchy");
    ImGUI::End();


    ImGUI::Begin("Inspector");

    if (inspectedGameObject != nullptr)
    {

        if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            ImGUI::OpenPopup("HierarchyRightClickMenu");
        }

        if (ImGui::BeginPopup("HierarchyRightClickMenu")) {
            if (ImGUI::MenuItem("Add Camera")) { inspectedGameObject->AddComponent<Camera>(); }
            if (ImGUI::MenuItem("Add MeshRenderer")) { inspectedGameObject->AddComponent<MeshRenderer>(); }
            if (ImGUI::MenuItem("Add Script")) { inspectedGameObject->AddComponent<ScriptAdder>(); }

            ImGui::EndPopup();
        }

        int idx = 0;
        for (std::shared_ptr<Component> component : inspectedGameObject->components)
        {
            const char* str = typeid(*component.get()).name();
            int size = (int)std::strlen(str) + 5; // 2 for ## and 3 for the number ( who tf s gonna use more than 999 components on a single GameObject
            char* result = new char[size];

            std::snprintf(result, size, "%s%s%d", str, "##", idx++);
            if (ImGUI::TreeNode(StripClassPrefix(result)))
            {
                component->OnInspect();
                ImGui::TreePop();
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right)) {
                ImGUI::OpenPopup("ComponentRightClickMenu");
                savedComponentIndex = idx - 1;
            }


            delete[] result;
        }
        if (ImGui::BeginPopup("ComponentRightClickMenu")) {
            if (ImGUI::MenuItem("Delete Component")) { inspectedGameObject->RemoveComponent<Component>(savedComponentIndex); }

            ImGui::EndPopup();
        }

    }

    ImGUI::End();
}

#pragma endregion

#pragma region Rendu
void EditorWindows::GraphicWindows() {
    ImGui::Begin("Rendu");
    Engine::GetModule<ImGUIModule>()->Image(RenderModule::GetInstance()->renderer->GetRenderTexture(), glm::vec2(ImGUI::GetWindowWidth(), ImGUI::GetWindowHeight()));
    ImGui::End();
}


#pragma endregion

void EditorWindows::Init() {
    scenesManager = Engine::GetModule<ScenesManager>();
    audiomanager = AudioManager::GetInstance();
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 }; // Plage Unicode FontAwesome

    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "Assets/imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("Assets/Font-Awesome-6-Free-Solid-900.otf", 32.0f, &config, icons_ranges); // Charger FontAwesome
    io.Fonts->Build(); // Construire les polices

    //Button style
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    //

    rootNode = std::make_shared<FileNode>();
    rootNode->name = "Assets";
    rootNode->path = "Assets";
    rootNode->is_directory = true;

    GenerateFileTree(rootNode);
    currentNode = rootNode;
}

void EditorWindows::Update() {

    bool test = true;
    ShowExampleAppDockSpace(&test);

    HierarchyWindow();

    GraphicWindows();

    ImGui::Begin("Arbo");
    DisplayFileTree(rootNode);
    ImGui::End();

    ImGui::Begin("TreeNode");
    NavMenuArbo();
    UpdateCurrentFolder();
    ImGui::End();
    
}
