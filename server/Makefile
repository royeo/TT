CC := gcc
CFLAGS := -O3
Target := server
Libs := -lsqlite3
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c, %.o, $(Source))
Modules := main my_str link_opt handle_log handle_account handle_reg handle_password_back handle_modify_signature handle_chat handle_message_record handle_change_password handle_send_file handle_modify_nickname handle_group_msg handle_advert_info
AllObjs := $(addsuffix /src/*.o, $(Modules))

modules_make = $(MAKE) -C $(1);
modules_clean = $(MAKE) clean -C $(1);

.PHONY : all clean mm mc

all : $(Target)

mm : 
	@ $(foreach n, $(Modules), $(call modules_make, $(n)))

mc : 
	@ $(foreach n, $(Modules), $(call modules_clean, $(n)))

$(Target) : mm
	$(CC) $(CFLAGS) -o $(Target) $(AllObjs) $(Libs)
	@ echo $(Target) make done!

clean : mc
	rm -rf $(Target)
	@ echo make clean!
