use ExtUtils::MakeMaker;
WriteMakefile(
	NAME    => 'QREncode',
	DEFINE  => '',
	INC     => '',
	LIBS    => '',
	OBJECT  => 'qre_wrap.o QR_Encode.o'
);

