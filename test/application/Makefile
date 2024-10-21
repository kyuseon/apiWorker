################################################################################
#
#
#
################################################################################

$(CC)=$(CC) $(GLOBAL_CC_OPT) $(GLOBAL_CC_INC)
$(CL)=$(CL) $(GLOBAL_CL_OPT) $(GLOBAL_CL_LIB)

TARGET=TestApp.exe

SRCS_CPP=testMain.cpp

OBJS=$(SRCS_CPP:%.cpp=%.o)

INC=
LIB=-lnyang

CFLAGS=$(INC)
LFLAGS=$(LIB)

all: $(TARGET)

%.o: %.cpp
	$(CC) -c -o $(<:%.cpp=%.o) $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CL) -o $@ $(OBJS) $(LFLAGS)

run:
	./$(TARGET)

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)
