#include "DaeParser.h"

int main(void)
{
	DaeParser parser;
	Dae_Data data;
	parser.Parse("dragon.dae",data);

	system("pause");

	return 0;
}