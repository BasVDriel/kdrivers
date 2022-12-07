#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x90f75b7e, "module_layout" },
	{ 0x1d5a1286, "kobj_sysfs_ops" },
	{ 0xfe990052, "gpio_free" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x81f25d7b, "gpiod_to_irq" },
	{ 0x440140d2, "gpiod_direction_input" },
	{ 0xc29f9195, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xff9ea26f, "kobject_put" },
	{ 0xb809600c, "sysfs_create_file_ns" },
	{ 0xef401a22, "kobject_init_and_add" },
	{ 0x84b183ae, "strncmp" },
	{ 0x92997ed8, "_printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xf08551e, "gpiod_set_raw_value" },
	{ 0x6d44e361, "gpio_to_desc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "25FDDE50CDA34A1AA31CEE0");
