// SPDX-License-Identifier: GPL-2.0+
// iDroid IR remote for Orange Pi 5 Plus (AOSP) - NEC protocol
#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table idroid[] = {
	{ 0x41a, KEY_POWER },        /* red POWER  */
	{ 0x421, KEY_POWER },        /* black POWER */
	{ 0x422, KEY_TV },           /* SOURCE */
	{ 0x423, KEY_MUTE },         /* MUTE (green) */
	{ 0x424, KEY_VOLUMEDOWN },   /* VOL- (yellow) */
	{ 0x425, KEY_VOLUMEUP },     /* VOL+ (blue) */
	{ 0x44d, KEY_TV },           /* TV */
	{ 0x443, KEY_VIDEO },        /* VOD */
	{ 0x419, KEY_VOLUMEDOWN },   /* pic - */
	{ 0x445, KEY_VOLUMEUP },     /* pic + */
	{ 0x45d, KEY_MENU },         /* MENU -> app/options menu */
	{ 0x41b, KEY_F1 },           /* MOUSE (spare) */
	{ 0x41c, KEY_LEFT },
	{ 0x444, KEY_UP },
	{ 0x448, KEY_RIGHT },
	{ 0x41d, KEY_DOWN },
	{ 0x45c, KEY_ENTER },        /* OK */
	{ 0x41f, KEY_HOMEPAGE },     /* HOME */
	{ 0x40a, KEY_BACK },         /* RETURN */
	{ 0x413, KEY_1 }, { 0x410, KEY_2 }, { 0x411, KEY_3 },
	{ 0x40f, KEY_4 }, { 0x40c, KEY_5 }, { 0x40d, KEY_6 },
	{ 0x40b, KEY_7 }, { 0x408, KEY_8 }, { 0x409, KEY_9 },
	{ 0x447, KEY_0 },
	{ 0x458, KEY_APPSELECT },    /* TV-SYS -> recents/overview */
	{ 0x453, KEY_CONFIG },       /* SETUP -> settings */
};

static struct rc_map_list idroid_map = {
	.map = {
		.scan     = idroid,
		.size     = ARRAY_SIZE(idroid),
		.rc_proto = RC_PROTO_NEC,
		.name     = "rc-idroid",
	}
};

static int __init init_rc_map_idroid(void) { return rc_map_register(&idroid_map); }
static void __exit exit_rc_map_idroid(void) { rc_map_unregister(&idroid_map); }
module_init(init_rc_map_idroid)
module_exit(exit_rc_map_idroid)
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("iDroid remote keymap (Orange Pi 5 Plus)");
