#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>


MODULE_LICENSE("GPL");

struct my_dev
{
	void **data;
	unsigned Q;
	unsigned S;
	unsigned size;
};

static int __init zacetni_modul(void);
static void __exit koncni_modul(void);
int my_open(struct inode *, struct file *);
int my_release(struct inode *, struct file *);
ssize_t my_read(struct file *, char *, size_t, loff_t *);
ssize_t my_write(struct file *, const char *, size_t, loff_t *);
void trim(struct my_dev *dev);

#define DEVICE_NAME "my_module"
#define N_BLK 32
#define BLK_SIZE 4

int Major;
struct file_operations fops = {
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write
};

struct my_dev *my_dev_ptr;

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
	
	my_dev_ptr = kmalloc(sizeof(struct my_dev), GFP_KERNEL);
	
	if(!my_dev_ptr) {
		koncni_modul();
	
		return -ENOMEM;
	}
	
	memset(my_dev_ptr, 0, sizeof(struct my_dev));
	
	my_dev_ptr->Q = BLK_SIZE;
	my_dev_ptr->S = N_BLK;
	
	return 0;
}

static void __exit koncni_modul(void)
{
	if(my_dev_ptr) {
		trim(my_dev_ptr);
		kfree(my_dev_ptr);
	}
	
	unregister_chrdev(Major, DEVICE_NAME);
}

void trim(struct my_dev *dev)
{
	int i;
	
	if(dev->data) {
		for(i = 0; i < dev->S; i++) {
			if(dev->data[i]) {
				kfree(dev->data[i]);
			}
		}
		
		kfree(dev->data);
		dev->data = NULL;
	}
	
	dev->size = 0;
}

int my_open(struct inode *inode, struct file *file)
{ 
	file->private_data = my_dev_ptr;
	
	if((file->f_flags & O_ACCMODE) == O_WRONLY) {
		trim(my_dev_ptr);
	}
	
	return 0;
}

int my_release(struct inode *inode, struct file *file)
{ 
	return 0; 
}

ssize_t my_read(struct file *filp, char __user *buff, size_t len, loff_t *offset)
{
	struct my_dev *dev = filp->private_data;
	long s_pos, q_pos;
	
	if (*offset >= dev->size) {
		return 0;
	}

	if (len > dev->size - *offset) {
		len = dev->size - *offset;
	}

	s_pos = (long) *offset / BLK_SIZE;
	q_pos = (long) *offset % BLK_SIZE;
	
	if(len > BLK_SIZE - q_pos) {
		len = BLK_SIZE - q_pos;
	}

	if (copy_to_user(buff, dev->data[s_pos] + q_pos, len)) {
		return -EFAULT;
	}

	*offset += len;
	
	printk(KERN_INFO "[MY_MODULE:read] len: %ld, offset: %ld\n", (long)len, (long)(*offset));

	return len;
}

ssize_t my_write(struct file *filp, const char __user *buff, size_t len, loff_t *offset)
{
	struct my_dev *dev = filp->private_data;
	long s_pos, q_pos;
	
	s_pos = (long) *offset / BLK_SIZE;
	q_pos = (long) *offset % BLK_SIZE;
	
	if(s_pos >= N_BLK) {
		return len;
	}
	
	if(!dev->data) {
		// alloc qset
		dev->data = kmalloc(N_BLK * sizeof(char *), GFP_KERNEL);
		
		if(!dev->data) {
			return -ENOMEM;
		}
		
		memset(dev->data, 0, N_BLK * sizeof(char *));
	}
	
	if(!dev->data[s_pos]) {
		dev->data[s_pos] = kmalloc(BLK_SIZE * sizeof(char), GFP_KERNEL);
		
		if(!dev->data[s_pos]) {
			return -ENOMEM;
		}
	}
	
	if(len > BLK_SIZE - q_pos) {
		len = BLK_SIZE - q_pos;
	}
	
	if(copy_from_user(dev->data[s_pos] + q_pos, buff, len)) {
		return -EFAULT;
	}
	
	*offset += len;
	
	printk(KERN_INFO "[MY_MODULE:write] len: %ld, offset: %ld\n", (long)len, (long)(*offset));
	
	if(dev->size < *offset) {
		dev->size = *offset;
	}
	
	return len;
}

