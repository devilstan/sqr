use ExtUtils::MakeMaker;
WriteMakefile(
	CC      => 'g++',
	LD      => 'g++',
	NAME    => 'SQR',
	DEFINE  => '',
	INC     => '-I/usr/include/cairo',
	LIBS    => '-lcairo',
	OBJECT  => '$(O_FILES) draw/qdr_draw.o',
);
