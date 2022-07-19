#include"Header.h"
#pragma warning(disable : 4996)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int ret_val(std::string str)
{
    std::istringstream ss(str);
    std::string arr[10];
    std::string word;
    int i = 0;
    while (ss >> word)
    {
        arr[i] = word;
        i++;
    }
    int val;
    std::stringstream calorie_ret(arr[(i-1)]);
    calorie_ret >> val;
    return(val);
}
std::string check(std::string str)
{
    std::istringstream ss(str);
    std::string arr[10];
    std::string word;
    int i = 0;
    while (ss >> word)
    {
        arr[i] = word;
        i++;
    }
    return(arr[0]);
}
std::string ret_date();
std::string ret_time();
std::string burn(const char* search);
std::string take(const char* search1);
void run();
int main(int, char**)
{
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(880, 500, "Diet PLanner", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    /* Enable Docker Mode by Default */
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.Fonts->AddFontFromFileTTF("../Font.ttf", 16.0f);

    ImFont* title_font = io.Fonts->AddFontFromFileTTF("../Font.ttf", 32.0f);
    ImFont* text_font = io.Fonts->AddFontFromFileTTF("../Font.ttf", 18.0f);
    ImFont* large_font = io.Fonts->AddFontFromFileTTF("../Font.ttf", 54.0f);

    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f)
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    int calorie = 0;
    int cal1 = 0;
    int cal2 = 0;
    int daily_goal = 0;
    std::fstream MyFile("data\\daily_goal.txt");
    if (MyFile)
    {
        if (MyFile.is_open()) {
            MyFile >> daily_goal;
        }
    }
    else
    {
        run();
    }
    MyFile.close();
    std::ifstream read_cal("data\\daily_calorie.txt");
    std::string myText = "";
    std::string today_date = "";
    std::string check_date = "";
    if (read_cal)
    {
        while (getline(read_cal, myText)) {
            myText;
        }
        check_date = check(myText);
        today_date = ret_date();
        int res = check_date.compare(today_date);
        if (res == 0)
        {
            calorie = ret_val(myText);
        }
    }
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        /* Setting the window and default docking flags for the main window */
        ImGuiWindowFlags window_flags = 0;
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;
        static bool light_theme = true;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        /* Setting up default style as dockspace */
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(900, 500), ImGuiCond_FirstUseEver);
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        /* Actual Start of the Window Drawing */
        ImGui::Begin("Window", nullptr, window_flags);
        ImGui::SetCursorPosX(350);
        ImGui::PushFont(title_font);
        ImGui::Text("Diet planner");
        ImGui::PopFont();

        /* Diplaying the Calorie*/
        ImVec2 TextPos1(70, 65);
        ImGui::SetCursorPos(TextPos1);
        ImGui::PushFont(title_font);
        ImGui::Text("Your daily calorie intake:");
        ImGui::PopFont();

        ImGui::SetCursorPosX(375);
        ImGui::SetCursorPosY(55);
        ImGui::PushFont(large_font);
        ImGui::Text("%d", calorie);
        ImGui::PopFont();
        //Calorie Display Section End


        //Calorie burn section
        ImVec2 TextPos(70, 185);
        ImGui::SetCursorPos(TextPos);
        ImGui::PushFont(title_font);
        ImGui::Text("Calorie Burnt:");
        ImGui::PopFont();

        static const char* items1[]{ "Aerobics","Archery","Badminton","Ballet","Basketball","Bowling","Climbing-hills","Cricket","Cycling","Darts","Fencing","Fishing","Football","Golf","Gymnastics","Handball","Hockey","Jumping","Karate","Polo","Rugby","Running","Skateboarding" };
        static bool selected_ca[30];
        int cal_bn = 0;
        static std::string previewva = "";
        std::string burn_line = "";
        ImGui::SetCursorPosX(70);
        ImGui::SetCursorPosY(235);
        ImGui::PushItemWidth(250);
        if (ImGui::BeginCombo(" ", previewva.c_str()))
        {
            previewva = "";
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(items1); i++)
            {
                ImGui::Selectable(items1[i], &selected_ca[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (selected_ca[i])
                {
                    vec.push_back(items1[i]);
                    burn_line = burn(items1[i]);
                    cal_bn += ret_val(burn_line);
                }
            }

            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    previewva += vec.at(i);
                else if (!(i == vec.size() - 1))
                    previewva += vec.at(i) + ",";
                else
                    previewva += vec.at(i);
            }
            cal1 = cal_bn;
            ImGui::EndCombo();
        }

        ImVec2 button_sz1(120, 40);
        ImVec2 ButtonPos1(90, 300);
        ImGui::SetCursorPos(ButtonPos1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.85f));
        if (ImGui::Button("Burn", button_sz1)) {
            calorie -= cal1;
        }
        ImGui::PopStyleColor(1);
        //Burn Section End

        //Calorie burn section
        ImVec2 TextPos2(570, 185);
        ImGui::SetCursorPos(TextPos2);
        ImGui::PushFont(title_font);
        ImGui::Text("Calorie Intake:");
        ImGui::PopFont();

        static const char* items[]{ "Apple","Banana","Bagel","Biscuit","Breadsticks","Bread","Beans","Carrot","Corn","Cereal","Cookie","Cornbread","Grapes","Orange","Peas","Potato","Pizza","Popcorn","Pear","Pineapple","Paisins","Raspberries","Rice","Strawberries","Sweet-Potato","Spaghetti","Waffles","Watermelon"};
        int cal_te = 0;
        static bool selected_cal[30];
        static std::string previewval = "";
        std::string take_line;
        ImGui::SetCursorPosX(570);
        ImGui::SetCursorPosY(235);
        ImGui::PushItemWidth(250);
        if (ImGui::BeginCombo("  ", previewval.c_str()))
        {
            previewval = "";
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(items); i++)
            {
                ImGui::Selectable(items[i], &selected_cal[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (selected_cal[i])
                {
                    vec.push_back(items[i]);
                    take_line = take(items[i]);
                    cal_te += ret_val(take_line);
                }
            }

            for (size_t i = 0; i < vec.size(); i++)
            {

                if (vec.size() == 1)
                    previewval += vec.at(i);
                else if (!(i == vec.size() - 1))
                    previewval += vec.at(i) + ",";
                else
                    previewval += vec.at(i);
            }
            cal2 = cal_te;
            ImGui::EndCombo();
        }

        ImVec2 button_sz2(120, 40);
        ImVec2 ButtonPos2(590, 300);
        ImGui::SetCursorPos(ButtonPos2);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.4f, 0.8f, 0.75f));
        if (ImGui::Button("Intake", button_sz2)) {
            calorie += cal2;
        }
        ImGui::PopStyleColor(1);
        //Burn Section End

        //Daily Goal Section
        ImGui::SetCursorPosX(300);
        ImGui::SetCursorPosY(400);
        ImGui::PushFont(title_font);
        ImGui::Text("Daily goal:");
        ImGui::PopFont();

        ImGui::SetCursorPosX(428);
        ImGui::SetCursorPosY(390);
        ImGui::PushFont(large_font);
        ImGui::Text("%d", daily_goal);
        ImGui::PopFont();
        if (daily_goal == 0)
        {
            std::fstream MyFile2("data\\daily_goal.txt");
            if (MyFile2.is_open()) {
                MyFile2 >> daily_goal;
            }
            MyFile2.close();
        }
        //Goal Section end
        ImGui::End();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    std::ofstream cal("data\\daily_calorie.txt", std::ios::out | std::ios::app);
    {
        std::string pt = ret_time();
        cal << pt << calorie;
    }
    cal.close();
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
std::string burn(const char* search)
{
    std::fstream MyFile("Database/Excersice.txt");
    std::string f_line;
    std::string line;
    while (getline(MyFile, line)) {
        if (line.find(search, 0) != std::string::npos) {
            f_line = line;
        }
    }
    return(f_line);
}
std::string take(const char* search1)
{
    std::fstream MyFile1("Database/Calorie.txt");
    std::string f_line1;
    std::string line1;
    while (getline(MyFile1, line1)) {
        if (line1.find(search1, 0) != std::string::npos) {
            f_line1 = line1;
        }
    }
    return(f_line1);
}
void run()
{
    int a = 0;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(TEXT("calorie.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        a += 1;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
std::string ret_time()
{
    time_t curr_time;
    tm* curr_tm;
    char date_string[100];

    time(&curr_time);
    curr_tm = localtime(&curr_time);

    strftime(date_string, 50, "\n%Y-%B-%d [%T] ---->\t", curr_tm);
    return (date_string);
}
std::string ret_date()
{
    time_t curr_time;
    tm* curr_tm;
    char date_string[100];

    time(&curr_time);
    curr_tm = localtime(&curr_time);

    strftime(date_string, 50, "%Y-%B-%d", curr_tm);
    return (date_string);
}