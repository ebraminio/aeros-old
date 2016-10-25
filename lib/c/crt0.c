extern void _init(void);
extern void _fini(void);
extern int main(int, char*[]);

int _start(int argc, char* argv[])
{
	_init();
	int ret = main(argc, argv);
	_fini();
	return ret;
}