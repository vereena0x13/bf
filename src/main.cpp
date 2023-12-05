#define VSTD_IMPL
#include "vstd.hpp"

// TODO: don't use a Hash_Table here...
Hash_Table<u32, u32> compute_jump_table(str code) {
    Hash_Table<u32, u32> jump_table;
    jump_table.init();

    Array<u32> stack;

    for(u32 i = 0; i < strsz(code); i++) {
        if(code[i] == '[') {
            stack.push(i);
        } else if(code[i] == ']') {
            u32 j = stack.pop();
            jump_table.set(i, j + 1);
            jump_table.set(j, i + 1);
        }
    }

    return jump_table;
}

s32 main(s32 argc, cstr *argv) {
    if(argc < 2) {
        printf("Usage: bf <file>\n");
        return 1;
    }

    auto code = read_entire_file(argv[1]);

    // TODO: filter non-bf characters out
    // TODO: convert raw bf to an IR (contraction, etc.)
    
    auto jump_table = compute_jump_table(code);

    Static_Array<u32, 1024 * 128> tape;
    u32 ip = 0;
    u32 dp = 0;

    while(ip < strsz(code)) {
        switch(code[ip]) {
            case '+': {
                tape[dp]++;
                ip++;
                break;
            }
            case '-': {
                tape[dp]--;
                ip++;
                break;
            }
            case '<': {
                dp--;
                ip++;
                break;
            }
            case '>': {
                dp++;
                ip++;
                break;
            }
            case ',': {
                // TODO
                break;
            }
            case '.': {
                printf("%c", tape[dp]);
                fflush(stdout);
                ip++;
                break;
            }
            case '[': {
                if(tape[dp] == 0) {
                    ip = jump_table.get(ip);
                } else {
                    ip++;
                }
                break;
            }
            case ']': {
                if(tape[dp] != 0) {
                    ip = jump_table.get(ip);
                } else {
                    ip++;
                }
                break;
            }
            default: {
                ip++;
                break;
            }
        }
    }


    return 0;
}