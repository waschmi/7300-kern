obj-y 	+=display.o
display-objs=vout_notify.o  vout_serve.o



obj-$(CONFIG_AM_TV_OUTPUT)  += am_tvout.o
am_tvout-objs :=   tvoutc.o  tvconf.o enc_clk_config.o

obj-$(CONFIG_AM_LCD_OUTPUT)  += lcdoutc.o

#EXTRA_CFLAGS += -DCONFIG_PM