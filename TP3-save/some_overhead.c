#define NOTHING ' '

typedef struct	s_oper
{
	int rep;
	struct s_oper *a;
	struct s_oper *b;
	char op;
}				t_oper;

int main()
{
	t_oper vars[15];

	vars[0].rep = 4; // a
	vars[0].op = NOTHING;
	vars[1].rep = 8; // b
	vars[1].op = NOTHING;
	vars[2].rep = 14; // c
	vars[2].op = NOTHING;
	vars[3].rep = 2; // d
	vars[3].op = NOTHING;
	vars[4].rep = 54; // e
	vars[4].op = NOTHING;
	vars[5].rep = 47; // f
	vars[5].op = NOTHING;
	vars[6].rep = 76; // g
	vars[6].op = NOTHING;
	vars[7].rep = 32; // h
	vars[7].op = NOTHING;

	// a + b
	vars[8].a = &(vars[0]);
	vars[8].b = &(vars[1]);
	vars[8].op = '+';

	// c * d
	vars[9].a = &(vars[2]);
	vars[9].b = &(vars[3]);
	vars[9].op = '*';

	// e - f
	vars[10].a = &(vars[4]);
	vars[10].b = &(vars[5]);
	vars[10].op = '-';

	// g + h
	vars[11].a = &(vars[6]);
	vars[11].b = &(vars[7]);
	vars[11].op = '+';

	// the middle division
	vars[12].a = &(vars[9]);
	vars[12].b = &(vars[10]);
	vars[12].op = '/';

	// the big substraction
	vars[13].a = &(vars[8]);
	vars[13].b = &(vars[12]);
	vars[13].op = '-';

	// the big addition
	vars[14].a = &(vars[13]);
	vars[14].b = &(vars[11]);
	vars[14].op = '+';
	return 0;
}

