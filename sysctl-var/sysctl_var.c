/**
 *
 * Kernel Module which creates a sysctl variable
 *
 * @copyright: GPLv2 (see LICENSE), by Timo Furrer <tuxtimo@gmail.com>
 *
 */

// use kernel module name in front of kernel log messages
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/seq_file.h>

// module header information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Timo Furrer");
MODULE_DESCRIPTION("Kernel module exposing a sysctl variable");
MODULE_VERSION("1.0");

// var settings
#define SYSCTL_VAR_DIRECTORY "lightning-talk"
#define SYSCTL_VAR_NAME "sample"

static struct ctl_table_header *sample_sysctl_header;

static struct sample_sysctl_settings {
    int value;
} sample_sysctl_settings;

static int default_value;
module_param(default_value, int, 0);

static int sample_sysctl_handler(struct ctl_table *ctl, int write, void __user *buffer, size_t *length, loff_t *offset) {
    int ret = proc_dointvec(ctl, write, buffer, length, offset);
    return ret;
}

static struct ctl_table sample_table[] = {
    {
        .procname = SYSCTL_VAR_NAME,
        .data = &sample_sysctl_settings.value,
        .maxlen = sizeof(int),
        .mode = 0644,
        .proc_handler = sample_sysctl_handler,
    }, { }
};

static struct ctl_table sample_root_table[] = {
    {
        .procname = SYSCTL_VAR_DIRECTORY,
        .maxlen = 0,
        .mode = 0555,
        .child = sample_table,
    }, { }
};

static int __init sysctl_var_init(void) {
    sample_sysctl_header = register_sysctl_table(sample_root_table);
    sample_sysctl_settings.value = default_value;
    pr_info("Loaded with default sample value %d\n", sample_sysctl_settings.value);
    return 0;
}

static void __exit sysctl_var_exit(void) {
    unregister_sysctl_table(sample_sysctl_header);
}

module_init(sysctl_var_init);
module_exit(sysctl_var_exit);
