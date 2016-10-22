LIB_FOLDERS = $(shell find -mindepth 2 -maxdepth 2 -type f -name Makefile)

.PHONY: $(foreach FOLDER, $(LIB_FOLDERS), $(dir $(FOLDER)))

define \n


endef

all:

%:
	@$(foreach FOLDER, $(LIB_FOLDERS), make -C $(dir $(FOLDER)) $@${\n})

lib%:
	@make -C $*
