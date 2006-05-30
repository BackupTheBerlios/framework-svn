#include "framework.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
    {
        printf("The xml definition for the model must be specified.\n");
        printf("  SYNTAX: framework <file.xml>\n\n");
        getc(stdin);
        return 0;
    }
	try
	{
	    char *configFile = argv[1];
		Population p = Population(configFile);
		p.Run();
	}
	catch(char * str )
    {
        printf("\n** Error occured: %s **\n", str);
    }
	getc(stdin);
	return 0;
}

