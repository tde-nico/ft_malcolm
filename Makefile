#####   COLORS   #####

END				= \033[0m

GREY			= \033[30m
RED				= \033[31m
GREEN			= \033[32m
YELLOW			= \033[33m
BLUE			= \033[34m
PURPLE			= \033[35m
CYAN			= \033[36m

HIGH_RED		= \033[91m

#####   INFO   #####

NAME			= ft_malcolm


SOURCE_IP		= 192.168.1.72
SOURCE_IP		= myhost_72
SOURCE_MAC		= 00:11:22:33:44:55
TARGET_IP		= 192.168.1.230
TARGET_IP		= 3232236006
TARGET_MAC		= 64:6c:80:03:39:7b
EXEC			= sudo ./$(NAME) -v $(SOURCE_IP) $(SOURCE_MAC) $(TARGET_IP) $(TARGET_MAC)

#####   COMMANDS   #####

CC				= gcc
EXTENSION		= c
CFLAGS			= -Werror -Wextra -Wall
IFLAGS			= -I includes

MD				= mkdir -p
RM				= rm -rf


#####   RESOURCES   #####

SRC_DIR			= srcs
OBJ_DIR			= objs

SRC_SUB_DIRS	= $(shell find $(SRC_DIR) -type d)
OBJ_SUB_DIRS	= $(SRC_SUB_DIRS:$(SRC_DIR)%=$(OBJ_DIR)%)

SRCS			= $(foreach DIR, $(SRC_SUB_DIRS), $(wildcard $(DIR)/*.$(EXTENSION)))
OBJS			= $(SRCS:$(SRC_DIR)/%.$(EXTENSION)=$(OBJ_DIR)/%.o)



#####   BASE RULES   #####

all: $(NAME)

$(NAME): $(OBJ_SUB_DIRS) $(OBJS)
	@ $(CC) $(CFLAGS) $(OBJS) -o $@
	@ echo "$(GREEN)[+] $(NAME)$(END)"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXTENSION)
	@ $(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@ echo "$(BLUE)[+] $@$(END)"

$(OBJ_SUB_DIRS):
	@ $(MD) $(OBJ_SUB_DIRS)
	@ echo "$(PURPLE)[+] $(SRC_DIR) -> $(OBJ_DIR) $(END)"


clean:
	@ $(RM) $(OBJ_DIR)
	@ echo "$(YELLOW)[+] $(OBJ_DIR)$(END)"

fclean: clean
	@ $(RM) $(NAME)
	@ echo "$(YELLOW)[+] $(NAME)$(END)"

re: fclean all

bonus: all



#####   EXTRA RULES   #####

run: all
	clear
	@ $(EXEC)

rrun: fclean run

val: all
	valgrind --leak-check=full $(EXEC)
var: val

norm:
	norminette


#####   PHONY   #####

.PHONY: all clean fclean re bonus run rrun val var norm
