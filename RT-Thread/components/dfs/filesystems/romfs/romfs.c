#include <dfs_romfs.h>

const static unsigned char _mytest_txt[] = {
0x68,0x65,0x6c,0x6c,0x6f,0x20,0x77,0x6f,0x72,0x6c,0x64,
};

const static unsigned char _nihao_txt[] = {
0x6c,0x61,0x6c,0x61,0x6c,0x61,0x6c,0x61,
};

const struct romfs_dirent _root_dirent[] = {
	{ROMFS_DIRENT_FILE, "mytest.txt", _mytest_txt, sizeof(_mytest_txt)},
	{ROMFS_DIRENT_FILE, "nihao.txt", _nihao_txt, sizeof(_nihao_txt)},
};

const struct romfs_dirent romfs_root = {ROMFS_DIRENT_DIR, "/", (rt_uint8_t*) _root_dirent, sizeof(_root_dirent)/sizeof(_root_dirent[0])};

