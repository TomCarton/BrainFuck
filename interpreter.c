// Simple BrainFuck interpreter
// ----------------------------
// written by Thomas CARTON
//

#include <memory.h>
#include <string.h>
#include <stdio.h>


static const unsigned int kMemSize = 32;
static const unsigned int kMaxCodeSize = 65536;


int main(int argc, char **argv)
{
    if (argc != 2)
        goto usage;


    // input, read the provided code file
    FILE *f = NULL;
    if (!(f = fopen(argv[1], "r")))
    {
        fprintf(stderr, "failed to open `%s' for reading\n", argv[1]);
        return 1;
    }

    char code[kMaxCodeSize];
    int clen = fread(code, 1, kMaxCodeSize, f);
    fclose(f);

 
    // get the scopes (between '[' & ']')
    int scope[kMaxCodeSize];

    int stack[kMaxCodeSize];
    int sp = 0;

    for (int cp = 0; cp < clen; ++cp)
    {
        if (code[cp] == '[')
        {
            stack[sp++] = cp;
        }
        else if (code[cp] == ']')
        {
            if (sp)
            {
                scope[cp] = stack[--sp];
                scope[stack[sp]] = cp;
            }
            else
            {
                fprintf(stderr, "Unmatched ']' at byte %d.", cp);
                return 1;
            }
        }
    }

    if (sp > 0)
    {
        fprintf(stderr, "Unmatched '[' at byte %d.", stack[sp - 1]);
        return 1;
    }


    // memory slots
    unsigned char mem[kMemSize];
    memset(mem, 0, kMemSize);
    unsigned int memp = 0;


    // interpret the code
    for (int cp = 0; cp < clen; ++cp)
    {
        switch (code[cp])
        {
            case '[': if (!mem[memp]) cp = scope[cp]; break;
            case ']': if (mem[memp]) cp = scope[cp]; break;

            case '<': --memp; break;
            case '>': ++memp; break;

            case '-': --mem[memp]; break;
            case '+': ++mem[memp]; break;

            case '.': putchar(mem[memp] == 10 ? '\n' : mem[memp]); break;
        }
    }

    putchar('\n');

    return 0;

usage:
    fprintf(stderr, "\n");
    fprintf(stderr, "   BrainFuck interpreter\n");
    fprintf(stderr, "   ---------------------\n\n");

    fprintf(stderr, "   usage:\n");
    fprintf(stderr, "     %s path\n\n", argv[0]);

    fprintf(stderr, "   example:\n");
    fprintf(stderr, "     %s hello_world.bf\n", argv[0]);

    fprintf(stderr, "\n");

    return 0;
}
