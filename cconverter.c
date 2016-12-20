// Simple BrainFuck C converter
// written by Thomas CARTON

#include <stdio.h>
#include <string.h>
#include <limits.h>


static const unsigned int kMemSize = 32;
static const unsigned int kMaxCodeSize = 65536;

void tab(FILE *f, unsigned int n)
{
    char tabs[8 + 1] = "\t\t\t\t\t\t\t\t";
    if (n < 8)
    {
        tabs[n] = '\0';
        fprintf(f, "%s", tabs);
    }
}

int main(int argc, char **argv)
{
    char infile[PATH_MAX];
    char outfile[FILENAME_MAX] = "main.c";

    if (argc < 2)
    {
        goto usage;
    }

    // read parameters
    for (unsigned int i = 0; i < argc; ++i)
    {
        // output file
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (argc > i + 1)
            {
                strcpy(outfile, argv[i + 1]);

                memmove(argv + i + 1, argv + i + 2, (argc - i - 2) * sizeof *argv);
                --argc;
                continue;
            }
        }
        // help
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            goto usage;
        }
        // input file
        else
        {
            strcpy(infile, argv[i]);
        }
    }

    // files
    FILE *in = NULL;
    FILE *out = NULL;

    // input
    if (!(in = fopen(infile, "r")))
    {
        fprintf(stderr, "failed to open `%s' for reading\n", infile);
        goto error;
    }

    // output
    if (!(out = fopen(outfile, "w")))
    {
        fprintf(stderr, "failed to open `%s' for output\n", outfile);
        goto error;
    }


    fprintf(out,
            "// C converted BrainFuck program\n\n"

            "#include <stdio.h>\n"
            "#include <stdlib.h>\n\n"

            "static const unsigned int kMemSize = %d;\n\n"

            "int main(int argc, char **argv)\n{\n"
            "\tunsigned char mem[kMemSize];\n"
            "\tunsigned int memp = 0;\n\n", kMemSize
            );

    unsigned int ti = 1;
    int c;
    while ((c = fgetc(in)) != EOF)
    {
        switch (c)
        {
            case '[': fprintf(out, "\n"); tab(out, ti); fprintf(out, "while (mem[memp])\n"); tab(out, ti++); fprintf(out, "{\n"); break;
            case ']': tab(out, --ti); fprintf(out, "}\n\n"); break;

            case '>': tab(out, ti); fprintf(out, "++memp;\n"); break;
            case '<': tab(out, ti); fprintf(out, "--memp;\n"); break;

            case '+': tab(out, ti); fprintf(out, "++mem[memp];\n"); break;
            case '-': tab(out, ti); fprintf(out, "--mem[memp];\n"); break;

            case ',': tab(out, ti); fprintf(out, "mem[memp] = getchar();\n\n"); break;
            case '.': tab(out, ti); fprintf(out, "putchar(mem[memp]);\n\n"); break;
        }
    }

    fprintf(out, "\treturn 0;\n}\n\n");

    fclose(in);
    fclose(out);

    return 0;

usage:
    fprintf(stderr, "\n");
    fprintf(stderr, "   BrainFuck to C converter\n");
    fprintf(stderr, "   ------------------------\n\n");

    fprintf(stderr, "   usage:\n");
    fprintf(stderr, "     %s path -o output\n\n", argv[0]);
    fprintf(stderr, "   parameters:\n");
    fprintf(stderr, "     -h, --help     - display this\n");
    fprintf(stderr, "     -o, --output   - output file [default: \"program.c\"]\n");

    fprintf(stderr, "   example:\n");
    fprintf(stderr, "     %s helloworld.bf -o helloworld.c\n", argv[0]);

    fprintf(stderr, "\n");

    return 0;

error:
    if (in) fclose(in);
    if (out) fclose(out);

    return 1;
}
