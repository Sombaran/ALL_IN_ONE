#ifndef __GTK_GPIO_H__
#define __GTK_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _gpio_direction {
	GPIO_IN,
	GPIO_OUT,
} gpio_direction;

typedef enum _gpio_value {
	GPIO_LOW = 0,
	GPIO_HIGH = 1,
} gpio_value;

typedef enum _gpio_irq_mode {
	GPIO_NONE,
	GPIO_RISING,
	GPIO_FALLING,
	GPIO_BOTH,
} gpio_irq_mode;

typedef enum _gpio_status {
	GPIO_INVALID = 0,
	GPIO_VALID = 1,
} gpio_status;

typedef struct _gpio_pin {
	unsigned int no;
	gpio_direction direction;
	gpio_irq_mode irq_mode;
	int fd;
	gpio_status valid;
} gpio_pin;

int gpio_open (gpio_pin *pin, unsigned int no);
int gpio_open_by_name (gpio_pin *pin, const char *name);
int gpio_open_dir (gpio_pin *pin, unsigned int no, gpio_direction dir);
int gpio_open_by_name_dir (gpio_pin *pin, const char *name, gpio_direction dir);

int gpio_close (gpio_pin *pin);
void gpio_destroy (void);

int gpio_out (gpio_pin *pin);
int gpio_in (gpio_pin *pin);

int gpio_set_value (gpio_pin *pin, gpio_value value);
int gpio_get_value (gpio_pin *pin, gpio_value *value);

int gpio_enable_irq (gpio_pin *pin, gpio_irq_mode m);
int gpio_irq_wait (gpio_pin *pin, gpio_value *value);
int gpio_irq_timed_wait (gpio_pin *pin, gpio_value *value, int timeout_ms);

int gpio_get_fd (gpio_pin *pin);

void gtk_gpio_set(int gpio_num, int val);
int gtk_gpio_get(int gpio_num);

#ifdef __cplusplus
}

#endif
#endif /* __GTK_GPIO_H__ */

