#include "Systems.hpp"
#include <stdlib.h>
#include <time.h>
#include "ImguiXlib.hpp"


namespace asapgl
{
	#ifdef USE_XLIB
	static ContextBase* ContextInit()
	{
		static ContextBase* ret = 0;

		if(ret!=0)
			return ret;

		char* display = getenv("DISPLAY");

		bool hasDisplay = display != 0;


		if(hasDisplay)
		{
		    ret = new Xlib_EGL_ContextType();
		}
		else
		{
		    ret = new DRM_GBM_EGL_ContextType();
		}

		return ret;
	}
	#endif


	bool SYSTEMS::init(const int argc, const char** argv)
	{
		srand (time(NULL));

		RENDERER.SetupEvents();
		#ifdef USE_XLIB
		CONTEXT = ContextInit();
		CONTEXT->Init(argc, argv);
		#else
		CONTEXT.Init(argc, argv);
		#endif
		RENDERER.Init();

		log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
		log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
		log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
		log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


		return true;

	}
	
	void SYSTEMS::cloaseApp()
	{
		#ifdef USE_XLIB
		CONTEXT->CleanUp();
		#else
		CONTEXT.CleanUp();
		#endif
	}

	void SYSTEMS::mainAppLoop()
	{
		#ifdef USE_XLIB
		CONTEXT->MainLoop();
		#else
		CONTEXT.MainLoop();
		#endif
	}

	void SYSTEMS::OnGUI()
	{


				// glViewport(0, 0, m_mainEglWindow->resolution.x, m_mainEglWindow->resolution.y);
				// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				// glClear(GL_COLOR_BUFFER_BIT);
				
		        // Start the Dear ImGui frame
		        ImGui_ImplOpenGL3_NewFrame();
		        ImGui_ImplXlib_NewFrame();
		        ImGui::NewFrame();

		    
	            ImGui::ShowDemoWindow(0);

	            






		ImGui::Begin("Memory Tables"); 

		if (ImGui::TreeNode("Borders, background"))
	    {
	        // Expose a few Borders related flags interactively
	        enum ContentsType { CT_Text, CT_FillButton };
	        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	        static bool display_headers = false;
	        static int contents_type = CT_Text;

	        //PushStyleCompact();
	        ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
	        ImGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
	        
	        ImGui::Indent();

	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
	        ImGui::Indent();
	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
	        ImGui::Unindent();

	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
	        ImGui::Indent();
	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
	        ImGui::Unindent();

	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
	        ImGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags, ImGuiTableFlags_BordersInner);
	        ImGui::Unindent();

	        ImGui::AlignTextToFramePadding(); ImGui::Text("Cell contents:");
	        ImGui::SameLine(); ImGui::RadioButton("Text", &contents_type, CT_Text);
	        ImGui::SameLine(); ImGui::RadioButton("FillButton", &contents_type, CT_FillButton);
	        ImGui::Checkbox("Display headers", &display_headers);
	        ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody); 
	        //PopStyleCompact();

	        if (ImGui::BeginTable("##table1", 3, flags))
	        {
	            // Display headers so we can inspect their interaction with borders.
	            // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
	            if (display_headers)
	            {
	                ImGui::TableSetupColumn("One");
	                ImGui::TableSetupColumn("Two");
	                ImGui::TableSetupColumn("Three");
	                ImGui::TableHeadersRow();
	            }

	            for (int row = 0; row < 5; row++)
	            {
	                ImGui::TableNextRow();
	                for (int column = 0; column < 3; column++)
	                {
	                    ImGui::TableSetColumnIndex(column);
	                    char buf[32];
	                    sprintf(buf, "Hello %d,%d", column, row);
	                    if (contents_type == CT_Text)
	                        ImGui::TextUnformatted(buf);
	                    else if (contents_type)
	                        ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
	                }
	            }
	            ImGui::EndTable();
	        }
	        ImGui::TreePop();
	    }
	    ImGui::End();
	}
}