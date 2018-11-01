# Настройки системы автоинкремента версии сборки

PROJECT  = $(shell cat PROJECT)
VERSION  = $(shell cat VERSION)
BUILD    = $(shell cat BUILD)
TYPE     = $(shell cat TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Настройки компилятора и линкера

CC      = sdcc
CFLAGS  = -I./include -L/lib/x86_64-linux-gnu/ -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Настройки M3P

M3P		 = m3p
COMPORT	 = /dev/ttyUSB0
# COMPORT	 = COM4 # Порты с номером >9 должны писаться как \\.\COM10
COMLOG   = comport_log.txt
BAUD	 = 9600	

# Правила компиляции и загрузки

NAME = prog
TARGET = $(NAME).bin
SRCDIR = src

SRCS = $(wildcard ./$(SRCDIR)/*.c)

LIST_OBJ = $(SRCS:.c=.rel)

all: $(TARGET)

$(TARGET) : $(LIST_OBJ) Makefile
	$(CC) $(LIST_OBJ) -o $(NAME).hex $(LFLAGS)
	$(M3P) hb166 $(NAME).hex $(TARGET) bye

$(LIST_OBJ) : %.rel : %.c
	$(CC) -c $(CFLAGS) $< -o $@  

clean:
	rm -f  $(TARGET) \
			$(NAME).hex \
			$(NAME).map \
			$(NAME).mem \
			$(NAME).lnk \
			pm3p_*.txt \
			$(TARBALL).gz \
			$(SRCDIR)/*.asm \
			$(SRCDIR)/*.rel \
			$(SRCDIR)/*.rst \
			$(SRCDIR)/*.sym \
			$(SRCDIR)/*.lst \
			# $(COMPORT)_log.txt 


load: $(TARGET) load.m3p
	@echo > $(COMPORT) || { echo -e "Permission denied to $(COMPORT).\n Consider running chmod o+rw $(COMPORT)" && false; }
	$(M3P) lfile load.m3p & sleep 10 && pkill m3p &

# Save with cp1251 encoding for this to work
define loadm3pprog
terminateonerror                                                     \n\
-- Открытие com-порта ($(COMPORT)) на скорости $(BAUD) бит/с         \n\
$(BAUD) openchannel $(COMPORT)                                   \n\
: wait                                                               \n\
                                                                     \n\
    cr cr                                                            \n\
    .\" Включите питание и нажмите кнопку RESET на стенде SDK.\" cr cr \n\
    .\" Ожидание перезапуска... \"                                     \n\
                                                                     \n\
    begin rsio dup emit 109 == until                                 \n\
                                                                     \n\
    .\" Ok\" cr cr                                                     \n\
;                                                                    \n\
                                                                     \n\
    wait                                                             \n\
                                                                     \n\
    T_RAM                                                            \n\
                                                                     \n\
    0x2100       write $(TARGET)                                     \n\
    0x2100       jmp                                                 \n\
                                                                     \n\
    0 term                                                           \n\
                                                                     \n\
bye  
endef

load.m3p: Makefile
	echo "$(loadm3pprog)" >load.m3p

term:
	$(M3P) echo $(COMLOG) $(BAUD) openchannel $(COMPORT) +echo 6 term -echo bye

dist:
	tar cvf $(TARBALL) *
	gzip $(TARBALL)

report:
	cd report && xelatex -shell-escape *.tex && cd - && zathura report/*.pdf &

.PHONY: report

