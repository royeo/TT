CC := gcc
CFLAGS := -O3
Target := client
Libs := -lpthread -lncursesw -lsqlite3
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c, %.o, $(Source));
Modules := main tcp_connect my_str menu detail_func login friend regist password_back modify_signature friend_list chat send_msg search_friend message_record set_up change_password edit_data group_chat send_group_msg 
AllObjs := $(addsuffix /src/*.o, $(Modules))
