/**
 *
 * Kernel Module which creates an echo proc file.
 *
 * @copyright: GPLv2 (see LICENSE), by Timo Furrer <tuxtimo@gmail.com>
 *
 */

// use kernel module name in front of kernel log messages
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

// module header information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Timo Furrer");
MODULE_DESCRIPTION("Kernel module exposing an echo proc file");
MODULE_VERSION("1.0");

// the internal buffer to capture the echo
#define ECHO_PROC_FILENAME "echo"
#define ECHO_BUFFER_MAX_SIZE 64
static char echo_buffer[ECHO_BUFFER_MAX_SIZE];

static struct proc_dir_entry *echo_proc_entry;

static ssize_t echo_proc_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    ssize_t bytes_read = 0;
    loff_t current_offset = *offset;
    char *current_buffer = echo_buffer;

    while(length && (ECHO_BUFFER_MAX_SIZE - current_offset) > 0) {
        put_user(*(current_buffer++), buffer++);
        length--;
        current_offset++;
        bytes_read++;
    }

    *offset = bytes_read;
    return bytes_read;
}

static ssize_t echo_proc_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
    ssize_t write_size;
    ssize_t bytes_written;
    ssize_t bytes_not_written;

    // clear internal echo buffer
    memset(echo_buffer, '\0', ECHO_BUFFER_MAX_SIZE);

    // copy data from user space to kernel space echo buffer
    write_size = (length > ECHO_BUFFER_MAX_SIZE) ? ECHO_BUFFER_MAX_SIZE : length;
    bytes_not_written = copy_from_user(echo_buffer, buffer, write_size);
    bytes_written = write_size - bytes_not_written;
    *offset += bytes_written;

    pr_info("captured '%s' in echo buffer\n", echo_buffer);
    return bytes_written;
}

static const struct proc_ops ops = {
	.proc_read = echo_proc_read,
	.proc_write = echo_proc_write,
};

static int __init echo_proc_init(void)
{
	echo_proc_entry = proc_create(ECHO_PROC_FILENAME, 0646, NULL, &ops);
	return 0;
}

static void __exit echo_proc_exit(void)
{
	proc_remove(echo_proc_entry);
}

module_init(echo_proc_init);
module_exit(echo_proc_exit);
