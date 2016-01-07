#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static int __init zacetni_modul(void);
static void __exit koncni_modul(void);
int odpri(struct inode *, struct file *);
int sprosti(struct inode *, struct file *);
ssize_t beri(struct file *, char *, size_t, loff_t *);
ssize_t pisi(struct file *, const char *, size_t, loff_t *);

#define DEVICE_NAME "ogrodje"

int Major;
struct file_operations fops = {
	.open = odpri,
	.release = sprosti,
	.read = beri,
	.write = pisi
};

module_init(zacetni_modul);
module_exit(koncni_modul);

static int __init zacetni_modul(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
		printk(KERN_ALERT "Registracija znakovne naprave spodletela.\n");

		return Major;
	}

	printk(KERN_INFO "Glavno stevilo je %d.\n", Major);

	return 0;
}

static void __exit koncni_modul(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
}

int odpri(struct inode *inode, struct file *file)
{ 
	return 0; 
}

int sprosti(struct inode *inode, struct file *file)
{ 
	return 0; 
}

ssize_t beri(struct file *filp, char __user *buff, size_t len, loff_t *offset)
{
	char *msg = "Zivjo!\n";
	int size = strlen(msg);
	if ( *offset >= size)
		return 0;

	if ( len > size - *offset)
		len = size - *offset;

	if ( copy_to_user( buff, msg, len) )
		return -EFAULT;

	*offset += len;
	return len;
}

ssize_t pisi(struct file *filp, const char __user *buff, size_t len, loff_t *offset)
{
	return 0;
}

