## Games and Graphic libs names
GAMES		= snake solarfox
GRAPHS		= sfml caca opengl
EXE		= arcade

## Folders
SRCS_FO		= sources/
INCS_FO		= includes/
LIBS_FO		= lib/
GAMES_FO	= games/
EXT_LIBS_FO	:= $(LIBS_FO)external_libs/
OBJS_FO		:= $(SRCS_FO)objs/
GAMES_SRCS_FO	:= $(SRCS_FO)games/
DISP_SRCS_FO	:= $(SRCS_FO)display_managers/
EXE_SRCS_FO	:= $(SRCS_FO)$(EXE)/

## Sources
GAMES_OBJS	:= $(patsubst $(GAMES_SRCS_FO)%.cpp, $(OBJS_FO)%.o, $(wildcard $(GAMES_SRCS_FO)*.cpp))
DISP_OBJS	:= $(patsubst $(DISP_SRCS_FO)%.cpp, $(OBJS_FO)%.o, $(wildcard $(DISP_SRCS_FO)*.cpp))
EXE_OBJS	:= $(patsubst $(EXE_SRCS_FO)%.cpp, $(OBJS_FO)%.o, $(wildcard $(EXE_SRCS_FO)*.cpp))

## Dynamic variables
COMP		=
COMP_GAME	= gamevoid
COMP_GRAPH	= graphvoid
COMPGAM_SRCS_FO	:= $(GAMES_SRCS_FO)$(COMP)/
COMPGRA_SRCS_FO	:= $(DISP_SRCS_FO)$(COMP)/
COMP_NAME	:= libmy$(COMP).so
COMPGAM_OBJS	:= $(patsubst $(COMPGAM_SRCS_FO)%.cpp, $(OBJS_FO)%.o, $(wildcard $(COMPGAM_SRCS_FO)*.cpp))
COMPGRA_OBJS	:= $(patsubst $(COMPGRA_SRCS_FO)%.cpp, $(OBJS_FO)%.o, $(wildcard $(COMPGRA_SRCS_FO)*.cpp))

## Flags
CC_FLAGS	= -Wall -Wextra # -Werror
CC_FLAGS	+= -I$(INCS_FO)
CC_FLAGS	+= -std=c++11 -MMD
sfml_FLAGS	:= -lsfml-graphics -lsfml-system -lsfml-audio
caca_FLAGS	:= -lcaca
opengl_FLAGS	:= -lGL -lGLU -lglut -lsfml-graphics -lsfml-system

## Other
CC		= g++
RM		= rm -rf

all:
	@$(foreach game, $(GAMES), make $(GAMES_FO)libmy$(game).so COMP=$(game) COMP_GAME=$(game);)
	@$(foreach graph, $(GRAPHS), make $(LIBS_FO)libmy$(graph).so COMP=$(graph) COMP_GRAPH=$(graph);)
	@make $(EXE)

vpath %.cpp $(patsubst %/,%,$(GAMES_SRCS_FO)):$(patsubst %/,%,$(COMPGAM_SRCS_FO))
$(GAMES_FO)libmy$(COMP_GAME).so: CC_FLAGS += -fPIC
$(GAMES_FO)libmy$(COMP_GAME).so: $(GAMES_OBJS) $(COMPGAM_OBJS)
	$(CC) -shared -g3 -o $@ $^

vpath %.cpp $(patsubst %/,%,$(DISP_SRCS_FO)):$(patsubst %/,%,$(COMPGRA_SRCS_FO))
$(LIBS_FO)libmy$(COMP_GRAPH).so: CC_FLAGS += -fPIC
$(LIBS_FO)libmy$(COMP_GRAPH).so: $(DISP_OBJS) $(COMPGRA_OBJS)
	$(CC) -shared -L$(EXT_LIBS_FO)$(COMP) -Wl,-rpath,$(EXT_LIBS_FO)$(COMP) $($(COMP)_FLAGS) -o $@ $^

vpath %.cpp $(patsubst %/,%,$(EXE_SRCS_FO))
$(EXE): $(EXE_OBJS)
	$(CC) -ldl -o $@ $^

$(OBJS_FO)%.o: %.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS_FO)*.o
	$(RM) $(OBJS_FO)*.d

fclean: clean
	$(foreach name, $(GAMES), $(RM) $(GAMES_FO)libmy$(name).so;)
	$(foreach name, $(GRAPHS), $(RM) $(LIBS_FO)libmy$(name).so;)
	$(RM) $(EXE)

re: fclean all

.PHONY: all clean fclean re

-include $(OBJFILES:.o=.d)
