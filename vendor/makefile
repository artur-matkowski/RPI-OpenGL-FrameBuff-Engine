CC 		  		= g++ -std=c++11 -O3 -DIMGUI_IMPL_OPENGL_ES2 
ARCHITECTURE 	= $(shell dpkg --print-architecture)
OBJDIR			= ./lib/editorOnly/


IMGUI_DIR = ./imgui/
IMGUI_DIR_DIALOG = ./ImGuiFileDialog/ImGuiFileDialog/

HEADER_DEPS += 	-I$(IMGUI_DIR)
HEADER_DEPS += 	-I$(IMGUI_DIR_DIALOG)
HEADER_DEPS += 	-I./vendor/glm/glm/

SOURCES += $(IMGUI_DIR)/imgui $(IMGUI_DIR)/imgui_demo $(IMGUI_DIR)/imgui_draw $(IMGUI_DIR)/imgui_tables $(IMGUI_DIR)/imgui_widgets
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_opengl3 
SOURCES += $(IMGUI_DIR_DIALOG)/ImGuiFileDialog


OBJECTS 	= $(SOURCES:%.cpp=$(OBJDIR)%.o)
OBJECTS2 	= $(patsubst %, %.o, $(SOURCES))
OBJECTS 	= $(OBJECTS2:%.cpp=$(OBJDIR)%.o)

DEPGL 		= -lGL -lEGL -lGLESv2


lib: $(SOURCES)

$(SOURCES):
	$(CC) -c $(HEADER_DEPS) $(@).cpp -o $(OBJDIR)$(notdir $@).o -fPIC
	@echo 

clean:
	rm lib/*