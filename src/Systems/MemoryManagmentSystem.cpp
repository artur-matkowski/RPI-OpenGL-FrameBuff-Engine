#include "MemoryManagmentSystem.hpp"
#include "ImguiXlib.hpp"

namespace asapgl
{
	void convert(size_t& big, size_t& small)
	{
	    big = small / 1024;
	    small = small % 1024;
	}

	void convert(size_t& gb, size_t& mb, size_t& kb, size_t& b)
	{
	    convert(kb, b);
	    convert(mb, kb);
	    convert(gb, mb);
	}



	void MemoryManagmentSystem::OnGUI()
	{
		



		ImGui::Begin("Memory Tables"); 

   
        static ImGuiTableFlags flags = ImGuiTableFlags_Borders 
        							| ImGuiTableFlags_RowBg
        							| ImGuiTableFlags_BordersH
        							| ImGuiTableFlags_BordersOuterH
        							| ImGuiTableFlags_BordersInnerH
        							| ImGuiTableFlags_BordersV
        							| ImGuiTableFlags_BordersOuterV
        							| ImGuiTableFlags_BordersInnerV
        							| ImGuiTableFlags_BordersOuter
        							| ImGuiTableFlags_BordersInner;


        ImGui::BeginTable("Memory Table", 6, flags);
        
        // Display headers so we can inspect their interaction with borders.
        // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
        
        ImGui::TableSetupColumn("Allocator name");			// 0
        ImGui::TableSetupColumn("Allocator origin ptr");	// 1
        ImGui::TableSetupColumn("Allocations");				// 2
        ImGui::TableSetupColumn("Deallocations");			// 3
        ImGui::TableSetupColumn("Allocated Memory");		// 4
        ImGui::TableSetupColumn("Available Memory");		// 5
        ImGui::TableHeadersRow();
        

        for (int row = 0; row < v_memBlocks.size(); row++)
        {
            char buff[128];
            size_t bytes = 0;
		    size_t gb = 0, mb = 0, kb = 0;

            ImGui::TableNextRow();

            {
                ImGui::TableSetColumnIndex(0);
                sprintf(buff, v_memBlocks[row]->GetDescription() );
                ImGui::TextUnformatted(buff);

                ImGui::TableSetColumnIndex(1);
                sprintf(buff, "%llu", v_memBlocks[row]->getRefPtr() );
                ImGui::TextUnformatted(buff);

                ImGui::TableSetColumnIndex(2);
                sprintf(buff, "%d", v_memBlocks[row]->GetAllocationsCount() );
                ImGui::TextUnformatted(buff);

                ImGui::TableSetColumnIndex(3);
                sprintf(buff, "%d", v_memBlocks[row]->GetDeallocationsCount() );
                ImGui::TextUnformatted(buff);

                bytes = v_memBlocks[row]->getUsedMemory();
		    	convert(gb, mb, kb, bytes);
                ImGui::TableSetColumnIndex(4);
                sprintf(buff, "%dGb, %dMb, %dkb, %db", gb, mb, kb, bytes );
                ImGui::TextUnformatted(buff);

                ImGui::TableSetColumnIndex(5);
                bytes = v_memBlocks[row]->getFreeMemory();
		    	convert(gb, mb, kb, bytes);
                sprintf(buff, "%dGb, %dMb, %dkb, %db", gb, mb, kb, bytes );
                ImGui::TextUnformatted(buff);
            }
        }
        ImGui::EndTable();
        
	    
	    ImGui::End();
	}
}