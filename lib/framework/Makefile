################################################################################
#
#
#
################################################################################

$(CC)=$(CC) $(GLOBAL_CC_OPT) $(GLOBAL_CC_INC)
$(CL)=$(CL) $(GLOBAL_CL_OPT) $(GLOBAL_CL_LIB)

SRCS_CPP=Application.cpp \
	Connect.cpp \
	Console.cpp \
	Logger.cpp

OBJS=$(SRCS_CPP:%.cpp=%.o)

TARGET=../libnyang.a

INC=
LIB=

CFLAGS=$(INC)
LFLAGS=$(LIB)

all: $(TARGET)

%.o: %.cpp
	$(MD) obj
	$(CC) -c -o $(<:%.cpp=%.o) $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(AR) ar rscv $@ $(OBJS)

clean:
	$(RM) $(TARGET)
