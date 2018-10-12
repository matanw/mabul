/*TODO:splint this file*/
#define MAX_LINE_LENGTH 80


#define SOURCE_ADDRESSING_POSITION 9
#define OP_CODE_POSITION 5
#define TARGET_ADDRESSING_POSITION 2
#define ARE_POSITION 0

#define SOURCE_REGISTER_POSITION 7
#define TARGET_REGISTER_POSITION 2
#define INITIAL_CODE_ADDRESS 100

#define NUMBER_POSITION 2


/*todo: a r e*/

enum OPERATION {Mov=0,Cmp=1,Add=2,Sub=3,Not=4,Clr=5,Lea=6,Inc=7, Dec=8,
    Jmp=9,Bne=10,Red=11, Prn=12,Jsr=13, Rst=14,Stop=15,Unknown=16  };
typedef enum OPERATION operation;


enum ADDRESSING_METHOD {
    ImmediateAddressing=1, DirectAddressing= 3, RegisterAddressing =5};
typedef enum ADDRESSING_METHOD addressing_method;

struct command_line{
    int bits;
    int original_line_number;
    char* label;
};
typedef struct command_line CommandLine;

struct argument_details{
    addressing_method ad_method;
    int num;
    char *label;
};
typedef struct argument_details ArgumentDetails;

struct label_data {
    char *label;
    int code_address;
};
typedef struct label_data LabelData;
